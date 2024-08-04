
#include "mcc_generated_files/system/system.h"
#include "i2c_app.h"
#include "timers.h"


typedef enum {
    kI2C_Dummy,
    kI2C_Error,
    kI2C_Success
}I2CState;

volatile I2CState i2c_state = kI2C_Dummy;



//Private functions
bool Client_Application(i2c_client_transfer_event_t event);


// Private variable
volatile uint8_t CLIENT_DATA[I2C_CLIENT_LOCATION_SIZE] = {
    0x55, 0x01, 0x0, 0x03, 0, 0x05, 0x06, 0x07, 0x08, 0x09
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
            CLIENT_DATA[9] = errorState;
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


static int I2CWriteImpl(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len){
    
    //tx_buf: byte0-reg_addr, byte1:... data
    for(size_t i = 0; i<I2C_TRY_CNT; i++){
        uint64_t start_time_ms = GetTimeMs();
        i2c_state=kI2C_Dummy;
        if (!I2C1_Write(dev_addr, tx_buf, tx_len)) {
            DelayMS(10);
            continue;
        }
        while(i2c_state==kI2C_Dummy){
            if(GetTimeMs() - start_time_ms > I2C_TMOUT_MS){
                return -1;
            }
        }
        if(i2c_state==kI2C_Success){
            return 0;
        }
    }
    
    return -1;
}
int I2CWrite(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len){
    I2C_SEL_N_SetLow(); //disable pi i2c bus
    int rc = I2CWriteImpl(dev_addr, tx_buf, tx_len);
    I2C_SEL_N_SetHigh(); //enable pi i2c bus
    return rc;
}
int I2CWriteNoIsolator(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len){
    int rc = I2CWriteImpl(dev_addr, tx_buf, tx_len);
    return rc;
}



void I2CSuccess(){
    i2c_state=kI2C_Success;
}
void I2CError(){
    i2c_state=kI2C_Error;
}

static int I2CWriteReadImpl(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len){
    for(size_t i = 0; i<I2C_TRY_CNT; i++){
        uint64_t start_time_ms = GetTimeMs();
        i2c_state=kI2C_Dummy;
        if (!I2C1_WriteRead(dev_addr, tx_buf, tx_len, rx_buf, rx_len)) {
            DelayMS(10);
            continue;
        }
        while(i2c_state==kI2C_Dummy){
            if((GetTimeMs() - start_time_ms) > I2C_TMOUT_MS){
                return -1;
            }
        }
        if(i2c_state==kI2C_Success){
            return 0;
        }
    }
    return -1;
}

int I2CWriteRead(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len){
     
    I2C_SEL_N_SetLow(); //disable pi i2c bus    
    int rc = I2CWriteReadImpl( dev_addr,  tx_buf, tx_len,  rx_buf,  rx_len);
    I2C_SEL_N_SetHigh(); //enable pi i2c bus
    return rc;
    
}
int I2CWriteReadNoIsolator(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len){
    
    int rc = I2CWriteReadImpl( dev_addr,  tx_buf, tx_len,  rx_buf,  rx_len);
    return rc;
}

void I2CSwitchMode(enum I2C1_Mode new_mode){
    if(new_mode == I2C1_Current_Mode()){
        return;
    }
    I2C1_Switch_Mode(new_mode);
    if(new_mode == I2C1_HOST_MODE){
        I2C1_Host_ReadyCallbackRegister(I2CSuccess);
        I2C1_Host_CallbackRegister(I2CError);         
    }else if (new_mode == I2C1_CLIENT_MODE){
        I2C1_Client.CallbackRegister(Client_Application);
    }
    
}