/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
 */

/*
� [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/i2c_host/i2c1_multimode.h"

#include "tasks.h"
#include "onoff.h"
#include "power_mgr.h"
#include "main.h"
#include "timers.h"
#include "i2c_app.h"
#include "i2c_regs_data.h"
#include "led_ctrl.h"
#include "pi_mgr.h"
/*
    Main application
 */


static uint8_t regAddrBuff[] = {0x18};
static uint8_t regAddrBuff2[] = {0x38};
static uint8_t readBuff[] = {0x00};

void process_device_id() {
    __delay_ms(10);
}

void retry_read_device_id(void);

void read_device_id(volatile struct TaskDescr* taskd) {

    uint8_t* addr = (uint8_t*) taskd->task_state;
    if (!I2C1_WriteRead(0x6b, addr, 1, readBuff, 1)) {
        return;
    }
    suspend_task(TASK_I2C_WAKEUP);
    I2C1_Host_ReadyCallbackRegister(process_device_id);
    I2C1_Host_CallbackRegister(retry_read_device_id);
}

void retry_read_device_id(void) {

    if (I2C1_ErrorGet() == I2C_ERROR_BUS_COLLISION) {
        resume_task(TASK_I2C_WAKEUP);
    }
}

static int client_mode = 0;
static int host_mode = 0;

void switch_i2c_mode(volatile struct TaskDescr* taskd) {
    if (taskd->task_state == &client_mode) {
        I2CSwitchMode(I2C1_CLIENT_MODE);
        
    } else {
        I2CSwitchMode(I2C1_HOST_MODE);
    }
    rm_task(TASK_I2C_SWITCH_MODE);
}


void OnOffSwithcPressed(enum ONOFFTypes type) {
    switch (type) {
        case BTN_1L:
            add_task(TASK_PI_SHUTDOWN_OR_WAKEUP,TaskPIShutdownOrWakeup,NULL);
            
            
//            ShutdownButtonPressed();
            break;
        case BTN_1S_1L:
            //wake up pi
//            MCU_INT_N_SetHigh();
//            if (I2C1_Current_Mode() == I2C1_HOST_MODE) {
//                add_task(TASK_I2C_WAKEUP, read_device_id, regAddrBuff2);
//            }
//            I2C_SEL_N_Toggle();
            break;
        case BTN_1S_1S_1L:
            add_task(TASK_POWER_IC_SYSTEM_RESET,PowMgrSystemReset,NULL);
            
//            if (I2C1_Current_Mode() == I2C1_HOST_MODE) {
//                add_task(TASK_I2C_WAKEUP, read_device_id, regAddrBuff2);
//            }
            break;
    };

}


int main(){
//    ibat_test(); //todo remove
    SYSTEM_Initialize();
    I2C1_Multi_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to:
    

    TASKS_Initialize();
    ONOFF_Initialize();

    ONOFF_CallbackRegister(OnOffSwithcPressed);
    
    //add rtc irq handler
    RTC_IRQ_N_SetInterruptHandler(RTCPinChanged);

    //enable PI RUN pin irq
    SET_PI_HB_NOT_OK(); //by default no HB recorded
    PI_RUN_SetInterruptHandler(PIRunModeChanged);
    
    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable();
    
    //1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflow);
    
    //go to host mode
    I2CSwitchMode(I2C1_HOST_MODE);
    
    
    //set charge enable when battery is present
    PowMgrEnableDisableCharging();    
    
    //go back to client mode
    I2CSwitchMode(I2C1_CLIENT_MODE);
    
    //led control setup
    //set led 0 by default
    PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);
    LEDSetValue(0);


    
    
   
    run_tasks();
    return 0;
}

# if 0


//todo move me somewhere
void BQ_INT_TEST();

//void read_bq(volatile struct TaskDescr* taskd);
//end todo
//todo remove
int I2CReadByte(uint8_t dev_addr, uint8_t addr, uint8_t* dest);
int I2CReadByteNoIsolator(uint8_t dev_addr, uint8_t addr, uint8_t* dest);

int I2CWriteByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t val);
int I2CWriteByteNoIsolator(uint8_t dev_addr, uint8_t reg_addr, uint8_t val);   
volatile uint8_t ibat_l,ibat_h,fault0;
volatile int ibat=0;
volatile int vbat=0;
volatile uint8_t vbat_l,vbat_h;

volatile float ibat_ema=0;

volatile uint64_t free_run_timer = 0;

void MiliSecTimerOverflowMock(void)
{
    free_run_timer++;
}
static void __idle2() {
    for (int i =0; i<  100; i++ ){
    CPUDOZEbits.IDLEN = 1;
    SLEEP();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    }
}

void ibat_test(){
    SYSTEM_Initialize();
    I2C1_Multi_Initialize();
   
    
    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable();
    
    //1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflowMock);
    
    //go to host mode
    I2CSwitchMode(I2C1_HOST_MODE);
    I2C_SEL_N_SetLow();//disable PI from I2C bus
    
    //led control setup
    //set led 0 by default
    PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);
    LEDSetValue(0);
    
    //set charge enable when battery is present
    PowMgrEnableDisableCharging();    
    
    
    //

    #define SET_BIT(x, pos) (x |= (1U << pos))
    #define CLEAR_BIT(x, pos) (x &= (~(1U<< pos)))
    #define TOGGLE_BIT(x, pos) x ^= (1U<< pos)
    #define CHECK_BIT(x, pos) (x & (1UL << pos) )
    #define GET_BITS(x, pos) ((x & ( 1 << pos)) >> pos)

    #define BQ_REG_CHARGE_CTRL_1 0x16
    #define BQ_REG_CHARGE_CTRL_2 0x17

    #define BQ_REG_CHARGE_STATUS_0 0x1D
    #define BQ_REG_CHARGE_STATUS_1 0x1E


    
    #define BQ_REG_CHARGER_FLAG_0 0x20
    #define BQ_REG_FAULT_FLAG_0 0x22
    #define BQ_REG_ADC_CTRL 0x26
    #define BQ_REG_ADC_FUN_DISABLE_0 0x27

    #define BQ_REG_IBUS_ADC  0x28
    #define BQ_REG_IBAT_ADC  0x2A
    #define BQ_REG_VBAT_ADC  0x30

    int ret = 0;
    uint8_t val=0;
    
    //do reg reset
    //#REG0x17_Charger_Control_2
    // BIT7 REG_RST
    ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_2, &val);
    SET_BIT(val, 7); //BIT7 REG_RST
    ret += I2CWriteByteNoIsolator(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_2, val);

    
    // reset watchdog , enable charing
    ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, &val);
    SET_BIT(val, 2); //BIT2 WATCHDOG reset
    SET_BIT(val, 5); // EN_CHG
    ret += I2CWriteByteNoIsolator(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, val);
    CHG_DISA_SetLow();//en phis charge


    //enable ibat adc
    ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_ADC_FUN_DISABLE_0, &val);
    CLEAR_BIT(val, 6); //BIT6 IBAT_ADC_DIS => 0 to enable
    CLEAR_BIT(val, 4); //BIT4 VBAT_ADC_DIS => 0 to enable
    ret += I2CWriteByteNoIsolator(BQ_I2C_ADDR, BQ_REG_ADC_FUN_DISABLE_0, val);   

    //set up adc params
    ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, &val);
    SET_BIT(val, 7); //ADC_EN=enable
    CLEAR_BIT(val, 6); //ADC_RATE  - 0 continous, 1 one shot
    SET_BIT(val, 5); //ADC_SAMPLE => 3
    SET_BIT(val, 4); //ADC_SAMPLE => 3 9 bit effective resolution
    CLEAR_BIT(val, 3); //ADC_AVG => 0 - singel value mes
    SET_BIT(val, 2); //ADC_AVG_INIT => 0 use existing, 1 start new conversion
    ret += I2CWriteByteNoIsolator(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, val);
    while(1){
        //get iba
        ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_IBAT_ADC, &val);
        ibat_l = val;
        ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_IBAT_ADC+1, &val);
        ibat_h= val;
        ibat = (int)((uint16_t)(ibat_h<<8) | ibat_l);
        float alpha = 0.2;
        ibat_ema = (alpha * ibat) + (1.0 - alpha) * ibat_ema;


        //get get vbat
        ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_VBAT_ADC, &val);
        vbat_l = val;
        ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_VBAT_ADC+1, &val);
        vbat_h= val;
        vbat = (int)((uint16_t)(vbat_h<<8) | vbat_l);

        //reset watdchod
        ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, &val);   
        SET_BIT(val, 2); //BIT2 WATCHDOG reset
        ret += I2CWriteByteNoIsolator(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, val);
        
        ret += I2CReadByteNoIsolator(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, &val);
        SET_BIT(val, 7); //ADC_EN=enable
        ret += I2CWriteByteNoIsolator(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, val);


//        DelayMS(100);
        __idle2();
    }


}
//end todo
#endif



//RTC CHECK - move to main at the end
# if 0 
    
    //todo FECCC check this out
    
    //todo remove timer
    //go to host mode
    I2CSwitchMode(I2C1_HOST_MODE);
    int ret=0;
    uint8_t tx[2];
    uint8_t rx[2];
    
    //disable timer, set 1sec period
    //BYTE 0x1C bit4 -> 0
    tx[0]=0x1C;
    ret += I2CWriteReadNoIsolator(0x32, tx,1, rx, 1);
    tx[1]=rx[0];
    tx[1] &= ~(1<<4);//TE=0
    tx[1] &= ~(0x3); //TSEL:0:2=0 => 1sec period
    ret += I2CWriteNoIsolator(0x32, tx,2);
     
    
    //TIMER_counter0=10
    tx[0]=0x1A;
    tx[1]=0xA;
    ret += I2CWriteNoIsolator(0x32, tx,2);
    //TIMER_counter1=0
    tx[0]=0x1B;
    tx[1]=0x0;
    ret += I2CWriteNoIsolator(0x32, tx,2);
    
    //Control Reg0
    tx[0]=0x1E;
    ret += I2CWriteReadNoIsolator(0x32, tx,1, rx, 1);
    tx[1]=rx[0];
    tx[1] |= (1<<4);//TIE=1
    tx[1] &= ~(0x7); //TSTP=0,TBKE=0, TBKON=0,
    ret += I2CWriteNoIsolator(0x32, tx,2);
    
    
    //Flag Register
    tx[0]=0x1D;
    ret += I2CWriteReadNoIsolator(0x32, tx,1, rx, 1);
    tx[1]=rx[0];
    tx[1] &= ~(1<<4); //TF=0
    ret += I2CWriteNoIsolator(0x32, tx,2);
    
    
    //enable timer
    tx[0]=0x1C;
    ret += I2CWriteReadNoIsolator(0x32, tx,1, rx, 1);
    tx[1]=rx[0];
    tx[1] |= (1<<4);//TEN=1
    ret += I2CWriteNoIsolator(0x32, tx,2);
    //go back to client mode
    I2CSwitchMode(I2C1_CLIENT_MODE);
    //end todo
#endif