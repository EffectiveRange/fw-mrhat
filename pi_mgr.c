
#include "mcc_generated_files/system/system.h"

#include "tasks.h"
#include "i2c_regs_data.h"
#include "timers.h"
#include "i2c_app.h"
#include "led_ctrl.h"
#include "power_mgr.h"

//
//defines
//
//2sec means pi is out of heartbeat - 
#define PI_RUN_HEARTBEAT_MAX_TIME_MS 2500 

//
//types
//
typedef struct{
    bool pin_val;
    uint64_t time_ms;
    bool active;
}MCU_INT_SetReset;

//
//local vars
//
static volatile uint64_t pi_run_last_falling_time_ms=0;
static volatile MCU_INT_SetReset mcu_int_set_reset ={
    .active=false,
    .pin_val=false,
    .time_ms=0
};

//another function sets the MCU_INT pin, then it ask this function to set the pin to other
//value after a certain time
//e.g. other task set MCU_INT to "0", then we need to wait before set it back to "1"

void SetResetMCU_INT_Pin(volatile struct TaskDescr* taskd){
    MCU_INT_SetReset* mcu_int_set_reset_p = (MCU_INT_SetReset *)(taskd->task_state);
    if(mcu_int_set_reset_p->active == false){
        //rm this task 
        rm_task(TASK_SET_RESET_MCU_INT_PIN);
        return;
    }
    if(GetTimeMs()> mcu_int_set_reset_p->time_ms){
        //it is time to set/reset the pin
        if(mcu_int_set_reset_p->pin_val){
            MCU_INT_N_SetHigh();
        }else{
            MCU_INT_N_SetLow();
        }
        //rm this task
        rm_task(TASK_SET_RESET_MCU_INT_PIN);
    }
    //else nothing happens, will check the time on next call
}

//PI_RUN pin state change irq handler
//save last falling edge timestampe
void PIRunModeChanged() {
    if(!PI_RUN_GetValue()){
        //save last fall time
        pi_run_last_falling_time_ms = GetTimeMs();
    }
}

//todo mve somewhere 
void BQ_INT_TEST(void);

volatile int pi_monitor=0;
volatile bool pi_running = false;
volatile bool pi_monitor_first_time=true;
volatile uint64_t pi_down_time = 0;
//this task runs always
//check if last falling time is older then heartbeat time
// -> we became I2C master
//check if the falling edge happens first time
// -> add PI the I2C and promote PI to I2C master, we will became I2C client
void TaskPIMonitor(volatile struct TaskDescr* taskd){
    //if we just have bootup, wait a little time to check the PI
    if(GetTimeMs() < (PI_RUN_HEARTBEAT_MAX_TIME_MS*2)){
        return;
    }
    
    
    bool prev_pi_running = pi_running;
    if (GetTimeMs() > (pi_run_last_falling_time_ms+PI_RUN_HEARTBEAT_MAX_TIME_MS)){
        SET_PI_HB_NOT_OK();
        pi_running=false;
    }else{
        SET_PI_HB_OK();
        pi_running=true;
    }
    //for first time set based on pins
    if(pi_monitor_first_time){
        if(pi_running){
            //PI got up
            I2CSwitchMode(I2C1_CLIENT_MODE);
            LEDSetToggleTime(100);
        }else{
             //PI went down
            I2CSwitchMode(I2C1_HOST_MODE);
            LEDSetPattern(&sleep_pattern);
            
            //do ibat mes
            pi_down_time=GetTimeMs();
            BQ_INT_N_SetInterruptHandler(BQ_INT_TEST);
            //PowMgrMesIBAT sets starts ibat measurement
            //WD reseted->BQ_INT_TEST invoked
            //TASK_CHECK_BQ_IRQ created
            //when wd reseted -> bq interrupt removed
            PowMgrMesIBAT();
        }
    }
    
    //if not first time - check for changes
    if(!pi_monitor_first_time){
        if(pi_running && !prev_pi_running){
            //PI got up
            I2CSwitchMode(I2C1_CLIENT_MODE);
            LEDSetToggleTime(100);
            timer_blink_period=100;
        }
        
        else if(!pi_running && prev_pi_running){
            //PI went down
            I2CSwitchMode(I2C1_HOST_MODE);
            LEDSetPattern(&sleep_pattern);
            
            //do ibat mes
            pi_down_time=GetTimeMs();
            BQ_INT_N_SetInterruptHandler(BQ_INT_TEST);
            //PowMgrMesIBAT sets starts ibat measurement
            //WD reseted->BQ_INT_TEST invoked
            //TASK_CHECK_BQ_IRQ created
            //when wd reseted -> bq interrupt removed
            PowMgrMesIBAT();

        }
        
        
        
    }
    
    pi_monitor_first_time=false;
    
    rm_task(TASK_PI_MONITOR);
}

void TaskWakeupPI(volatile struct TaskDescr* taskd){
    
    //i2c setup
    I2CSwitchMode(I2C1_HOST_MODE);
    //add PI to I2C bus
    I2C_SEL_N_SetHigh(); //enable pi i2c bus();
    
    DelayMS(100);
    
    //do one transaction
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
   
    //test i2c read, if could not read partid do not check battery return
    uint8_t i=0;
    for(int i =0;i<10;i++){
        //raead partid
        tx[0]=0x38;
        ret += I2CWriteReadNoIsolator(0x6b, tx,1, rx, 1);
        if((rx[0] & (1<<3)) == (1<<3)){
            break;
        }
        DelayMS(10);
        
    }   
    //if read partid failed return
    if((rx[0] & (1<<3)) != (1<<3)){
         LEDSetToggleTime(200);//todo remove me
    }else{
        LEDSetToggleTime(2000);//todo remove me
    }
    
    
//    
//    tx[0]=0x1D; //charge stat
//    ret += I2CWriteReadNoIsolator(0x6b, tx,1, rx, 1);
//    if(ret != 0){
//        LEDSetToggleTime(200);//todo remove me
//    }else{
//         LEDSetToggleTime(2000);//todo remove me
//    }
    DelayMS(100);
    
    //go back to client mode
    I2CSwitchMode(I2C1_CLIENT_MODE);
    
    //remove myself
    rm_task(TASK_WAKE_UP_PI);
    
}

//button 1L (1 long press calls this)
//if PI is running -> request it to shut down
//if PI is not running -> wakeup PI
void TaskPIShutdownOrWakeup(volatile struct TaskDescr* taskd){
    //if PI running
    if(IS_PI_HB_OK()){
        
        //trigger shutdown
        SET_SHUTDOWN_REQ();
        
        //trigger pi interrupt
        //SET MCU INT generate falling edge interrupt for PI
        MCU_INT_N_SetLow();
        //set MCU_INT high after 100msec
        mcu_int_set_reset.active=true;
        mcu_int_set_reset.pin_val=true;
        mcu_int_set_reset.time_ms=GetTimeMs()+100; //+ 100 msec        
        add_task(TASK_SET_RESET_MCU_INT_PIN, SetResetMCU_INT_Pin, (void*)&mcu_int_set_reset);
        
    }else{
        //wakeup PI
         add_task(TASK_WAKE_UP_PI, TaskWakeupPI, NULL);
        
    }
    rm_task(TASK_PI_SHUTDOWN_OR_WAKEUP);
    
}


void TaskCheckRTC(volatile struct TaskDescr* taskd){
    
    int ret=0;
    uint8_t tx[2];
    uint8_t rx[2];
      
    //af
    tx[0]=0x1D;
    ret += I2CWriteReadNoIsolator(0x32, tx,1, rx, 1);
    if(!ret){
        const bool af = rx[0] & (1<<3U);
        const bool tf = rx[0] & (1<<4U);
        if(af || tf){
            if(tf){
                
            }
            //wakeup PI
             add_task(TASK_WAKE_UP_PI, TaskWakeupPI, NULL);
             LEDSetToggleTime(1000);
        }
    } else{
        LEDSetToggleTime(50);//todo remove
    }
    
    //read reg rtc
    rm_task(TASK_CHECK_RTC);
}
int rtc_fall = 0;

void RTCPinChanged(void) {
    if(!RTC_IRQ_N_GetValue()){
        if(IS_PI_HB_NOT_OK()){
            //fall of RTC PIN
            rtc_fall++;
            add_task(TASK_CHECK_RTC, TaskCheckRTC, NULL);     
//        }else{
//            int ret=0;
//            uint8_t tx[2];
//            uint8_t rx[2];
//
//            //af
//            tx[0]=0x1D;
//            ret += I2CWriteReadNoIsolator(0x32, tx,1, rx, 1);
//            if(!ret){
//                const bool af = rx[0] & (1<<3U);
//                const bool tf = rx[0] & (1<<4U);
//                if(af || tf){
//                    if(tf){
//
//                    }
//                    LEDSetToggleTime(5000);//todo remove
//                }
//            } else{
//                LEDSetToggleTime(4000);//todo remove
//            }
//            
        }
    }
}

