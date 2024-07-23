
#include "mcc_generated_files/system/system.h"

extern void DelayMS(uint32_t delay_ms);
extern volatile uint8_t CLIENT_DATA[];


//todo move to somewhere else
volatile uint64_t free_run_timer = 0;
void MiliSecTimerOverflow(void)
{
    free_run_timer++;
    
    if((CLIENT_DATA[2] & 0x1) == 1){
        //bat charge enabled
        if((free_run_timer % 500) == 0){
            PWR_LED_CTRL_Toggle();
        }
    }else{
        //bat charge disabled
        if((free_run_timer % 2000) == 0){
            PWR_LED_CTRL_Toggle();
        }
    }
}
uint64_t GetTimeMs(){
    return free_run_timer;
}

void DelayMS(uint32_t delay_ms){
    uint64_t now_ms = free_run_timer;
    while(free_run_timer < (now_ms+delay_ms));
}
//end todo


#define I2C_TRY_CNT 3U
#define I2C_TMOUT_MS 100U
typedef enum {
    kI2C_Dummy,
    kI2C_Error,
    kI2C_Success
}I2CState;
volatile I2CState i2c_state = kI2C_Dummy;


void I2CSuccess(){
    i2c_state=kI2C_Success;
}
void I2CError(){
    i2c_state=kI2C_Error;
}
int I2CWriteRead(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len){
    for(size_t i = 0; i<I2C_TRY_CNT; i++){
        uint64_t start_time_ms = GetTimeMs();
        i2c_state=kI2C_Dummy;
        if (!I2C1_WriteRead(dev_addr, tx_buf, tx_len, rx_buf, rx_len)) {
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


void DisableBatteryCharge(){
    // /CE pin of battery management ic is active low
    // CHG_DISA pin=1 -> Charge disable
    CHG_DISA_SetHigh();
    //clear bit0 of reg2 STAT
    CLIENT_DATA[2] &= ~(1);
}
void EnableBatteryCharge(){
    // /CE pin of battery management ic is active low
    // CHG_DISA pin=0 -> Charge enable
    CHG_DISA_SetLow();
    //set bit0 of reg2 STAT
    CLIENT_DATA[2] |= 1;
}
int CheckBattery(){
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
    //i2c setup
    I2C1_Host_ReadyCallbackRegister(I2CSuccess);
    I2C1_Host_CallbackRegister(I2CError);     

    //# enable Force a battery discharging current (~30mA)
    //#REG0x16_Charger_Control_1 Register, BIT6 FORCE_IBATDIS
    tx[0]=0x16;
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    tx[1] = rx[0] | (1<<6);
    ret += I2CWrite(0x6b, tx, 2);
    
    //little delay for discharge current, I know it has a speed of light :D
    DelayMS(10);

    //# enable ADC  
    //#REG0x26_ADC_Control Register,BIT7 ADC_EN
    tx[0]=0x26;
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    tx[1] = rx[0] | (1<<7);
    ret += I2CWrite(0x6b, tx, 2);
    
    //adc sample takes 24milisec
    DelayMS(100);

    //# read ADC 
    //#REG0x30_VBAT_ADC Register bits 1:12
    tx[0]=0x30;
    ret += I2CWriteRead(0x6b, tx,1, rx, 2);
    //value is little endian
    uint16_t adc_bits = (uint16_t) (rx[0] + (rx[1]<<8));
    adc_bits &= (0x1ffe);
    adc_bits = adc_bits >> 1;
    float adc_mV = adc_bits * 1.99f; //mili Volt
    
     //VSYSMIN value is ~2500mV under 2000mV there is no battery
    if(ret == 0 && adc_mV > 2000){
        EnableBatteryCharge();
    }else{
        DisableBatteryCharge();
    }
    
    //disable ADC
    //#REG0x26_ADC_Control Register,BIT7 ADC_EN
    tx[0]=0x26;
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    tx[1] = rx[0] & ~(1<<7);
    ret += I2CWrite(0x6b, tx, 2);
    return ret;
    
}