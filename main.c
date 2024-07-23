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
© [2023] Microchip Technology Inc. and its subsidiaries.

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
/*
    Main application
 */

extern void MiliSecTimerOverflow();
#define I2C_CLIENT_LOCATION_SIZE 10

//Private functions
static bool Client_Application(i2c_client_transfer_event_t event);


//reg 0 FW id
//reg1 IRQ reg
//  bit0: shallow shut
//  bit1: deep shutdown reg
//reg2 STAT
//  bit0: battery_available

// Private variable
volatile uint8_t CLIENT_DATA[I2C_CLIENT_LOCATION_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09
};

static uint8_t clientLocation = 0x00;
static bool isClientLocation = false;

static bool Client_Application(i2c_client_transfer_event_t event) {
    switch (event) {
        case I2C_CLIENT_TRANSFER_EVENT_ADDR_MATCH: //Address Match Event
            if (I2C1_Client.TransferDirGet() == I2C_CLIENT_TRANSFER_DIR_WRITE) {
                isClientLocation = true;
            }
            break;

        case I2C_CLIENT_TRANSFER_EVENT_RX_READY: //Read the data sent by I2C Host
            if (isClientLocation) {
                clientLocation = I2C1_Client.ReadByte();
                isClientLocation = false;
                break;
            } else {
                CLIENT_DATA[clientLocation++] = I2C1_Client.ReadByte();
                if (clientLocation >= I2C_CLIENT_LOCATION_SIZE) {
                    clientLocation = 0x00;
                }
            }
            break;

        case I2C_CLIENT_TRANSFER_EVENT_TX_READY: //Provide the Client data requested by the I2C Host
            I2C1_Client.WriteByte(CLIENT_DATA[clientLocation++]);
            if (clientLocation >= I2C_CLIENT_LOCATION_SIZE) {
                clientLocation = 0x00;
            }
            break;

        case I2C_CLIENT_TRANSFER_EVENT_STOP_BIT_RECEIVED: //Stop Communication
            clientLocation = 0x00;
            break;

        case I2C_CLIENT_TRANSFER_EVENT_ERROR: //Error Event Handler
            clientLocation = 0x00;
            i2c_client_error_t errorState = I2C1_Client.ErrorGet();
            if (errorState == I2C_CLIENT_ERROR_BUS_COLLISION) {
                // Bus Collision Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_WRITE_COLLISION) {
                // Write Collision Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_RECEIVE_OVERFLOW) {
                // Receive Overflow Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_TRANSMIT_UNDERFLOW) {
                // Transmit Underflow Error Handling
            } else if (errorState == I2C_CLIENT_ERROR_READ_UNDERFLOW) {
                // Read Underflow Error Handling
            }
            break;

        default:
            break;
    }
    return true;
}




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
        I2C1_Switch_Mode(I2C1_CLIENT_MODE);
        I2C1_Client.CallbackRegister(Client_Application);
    } else {
        I2C1_Switch_Mode(I2C1_HOST_MODE);
    }
    rm_task(TASK_I2C_SWITCH_MODE);
}

void PIRunModeChanged() {
    if (PI_RUN_GetValue()) {
        add_task(TASK_I2C_SWITCH_MODE, switch_i2c_mode, &client_mode);
    } else {
        add_task(TASK_I2C_SWITCH_MODE, switch_i2c_mode, &host_mode);
    }
}


void OnOffSwithcPressed(enum ONOFFTypes type) {
    switch (type) {
        case BTN_1L:
            if (I2C1_Current_Mode() == I2C1_HOST_MODE) {
                add_task(TASK_I2C_WAKEUP, read_device_id, regAddrBuff);
            }
            CHG_DISA_Toggle();
            break;
        case BTN_1S_1L:
            if (I2C1_Current_Mode() == I2C1_HOST_MODE) {
                add_task(TASK_I2C_WAKEUP, read_device_id, regAddrBuff2);
            }
            I2C_SEL_N_Toggle();
            break;
        case BTN_1S_1S_1L:
            if (I2C1_Current_Mode() == I2C1_HOST_MODE) {
                add_task(TASK_I2C_WAKEUP, read_device_id, regAddrBuff2);
            }
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

    PI_RUN_SetInterruptHandler(PIRunModeChanged);
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
    CheckBattery();
    
    //go back to client mode
    I2C1_Switch_Mode(I2C1_CLIENT_MODE);
    I2C1_Client.CallbackRegister(Client_Application);

    
    run_tasks();
    return 0;
}





