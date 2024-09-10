
#include "mcc_generated_files/system/system.h"
#include "i2c_app.h"
#include "timers.h"
#include "i2c_regs_data.h"

typedef enum {
    kI2C_Dummy,
    kI2C_Error,
    kI2C_Success
} I2CState;

volatile I2CState i2c_state = kI2C_Dummy;
void I2C1_Close(void);
void I2C1_BusReset(void);


//Private functions
bool Client_Application(i2c_client_transfer_event_t event);


// Private variable
//NOTES:
//BYTE 1 MSB is sticky bit
volatile uint8_t CLIENT_DATA[I2C_CLIENT_LOCATION_SIZE] = {
//    0    1      2     3      4      5       6      7      8      9
    0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x09
};

volatile static uint8_t clientLocation = 0x00;
volatile static bool isClientLocation = false;
bool Client_Application(i2c_client_transfer_event_t event) {
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
                //allow only writable registers
                if (clientLocation < REG_ADDR_WR_START ||
                        clientLocation > REG_ADDR_WR_END) {
                    clientLocation = 0x00;
                }else{
                    CLIENT_DATA[clientLocation++] = I2C1_Client.ReadByte();
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
            CLIENT_DATA[REG_STAT_I2C_ERR_AND_STICKY_ADDR] |= errorState;
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

static int I2CWriteImpl(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len) {
    I2CSwitchMode(I2C1_HOST_MODE);
    int rc = 0;
    //tx_buf: byte0-reg_addr, byte1:... data
    for (size_t i = 0; i < I2C_TRY_CNT; i++) {
        uint64_t start_time_ms = GetTimeMs();
        i2c_state = kI2C_Dummy;

        if (!I2C1_Write(dev_addr, tx_buf, tx_len)) {
            I2C1_Close(); //todo remove
            I2C1_BusReset();
            DelayMS(10);
            rc = -2;
            continue;
        }
        while (i2c_state == kI2C_Dummy) {
            if (GetTimeMs() - start_time_ms > I2C_TMOUT_MS) {
                //todo check if continue of return
                I2C1_Close(); //todo check if needed
                I2C1_BusReset();
                //                return -1;
                rc = -1;
                break;
            }
        }
        if (i2c_state == kI2C_Error) {
            I2C1_Close(); //todo remove
            I2C1_BusReset();
            rc = -3;
        }
        if (i2c_state == kI2C_Success) {
            rc = 0;
            break;
        }
    }

    I2CSwitchMode(I2C1_CLIENT_MODE);
    return rc;
}

int I2CWriteByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t val) {
    uint8_t tx[2] = {reg_addr, val};
    I2C_SEL_N_SetLow(); //disable pi i2c bus    
    int rc = I2CWriteImpl(dev_addr, tx, 2);
    I2C_SEL_N_SetHigh(); //enable pi i2c bus
    return rc;
}


int I2CWrite(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len) {

    I2C_SEL_N_SetLow(); //disable pi i2c bus
    int rc = I2CWriteImpl(dev_addr, tx_buf, tx_len);
    I2C_SEL_N_SetHigh(); //enable pi i2c bus
    return rc;
}

void I2CSuccess() {
    i2c_state = kI2C_Success;
}

void I2CError() {
    i2c_state = kI2C_Error;
}

static int I2CWriteReadImpl(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len, uint8_t* rx_buf, size_t rx_len) {
    I2CSwitchMode(I2C1_HOST_MODE);
    int rc = 0;
    for (size_t i = 0; i < I2C_TRY_CNT; i++) {
        uint64_t start_time_ms = GetTimeMs();
        i2c_state = kI2C_Dummy;

        if (!I2C1_WriteRead(dev_addr, tx_buf, tx_len, rx_buf, rx_len)) {
            I2C1_Close(); //todo remove
            I2C1_BusReset();
            DelayMS(10);
            rc = -2;
            continue;
        }

        while (i2c_state == kI2C_Dummy) {
            if ((GetTimeMs() - start_time_ms) > I2C_TMOUT_MS) {
                //todo check if return or continue
                I2C1_Close(); //todo remove
                I2C1_BusReset();
                //                return -1;
                rc = -1;
                break;
            }
        }
        if (i2c_state == kI2C_Error) {
            I2C1_Close(); //todo remove
            I2C1_BusReset();
            rc = -3;
        }

        if (i2c_state == kI2C_Success) {
            rc = 0;
            break;
        }
    }
    I2CSwitchMode(I2C1_CLIENT_MODE);
    return rc;
}

int I2CReadByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t* dest) {

    I2C_SEL_N_SetLow(); //disable pi i2c bus    
    int rc = I2CWriteReadImpl(dev_addr, &reg_addr, 1, dest, 1);
    I2C_SEL_N_SetHigh(); //enable pi i2c bus
    return rc;
}


int I2CWriteRead(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len, uint8_t* rx_buf, size_t rx_len) {

    I2C_SEL_N_SetLow(); //disable pi i2c bus    
    int rc = I2CWriteReadImpl(dev_addr, tx_buf, tx_len, rx_buf, rx_len);
    I2C_SEL_N_SetHigh(); //enable pi i2c bus
    return rc;

}

int I2CWriteReadWithPI(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len, uint8_t* rx_buf, size_t rx_len) {
    I2C_SEL_N_SetHigh(); //enable pi i2c bus
    int rc = I2CWriteReadImpl(dev_addr, tx_buf, tx_len, rx_buf, rx_len);
    return rc;
}

void I2CSwitchMode(enum I2C1_Mode new_mode) {
    if (new_mode == I2C1_Current_Mode()) {
        return;
    }
    I2C1_Switch_Mode(new_mode);
    if (new_mode == I2C1_HOST_MODE) {
        I2C1_Host_ReadyCallbackRegister(I2CSuccess);
        I2C1_Host_CallbackRegister(I2CError);
//        I2C_SEL_N_SetLow(); //disable PI from I2C bus
    } else if (new_mode == I2C1_CLIENT_MODE) {
        I2C1_Client.CallbackRegister(Client_Application);
        I2C_SEL_N_SetHigh(); //enable PI from I2C bus
    }
}