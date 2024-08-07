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
#include "i2c_regs.h"
#include "led_ctrl.h"
#include "pi_mgr.h"
/*
    Main application
 */

//todo move to timer init
extern void MiliSecTimerOverflow();

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
    SYSTEM_Initialize();

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
//    add_task(TASK_PI_MONITOR, TaskPIMonitor, NULL);
    
    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable();
    
    //1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflow);
    
    //go to host mode
    I2C1_Switch_Mode(I2C1_HOST_MODE);
    
    //set charge enable when battery is present
    PowMgrEnableDisableCharging();
    
    //go back to client mode
    I2CSwitchMode(I2C1_CLIENT_MODE);
    I2C_SEL_N_SetHigh();
    
    //led control setup
    //set led 0 by default
    PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);
    LEDSetValue(0);
    
    run_tasks();
    return 0;
}



