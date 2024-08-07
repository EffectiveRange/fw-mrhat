
#include "mcc_generated_files/system/system.h"
#include "tasks.h"
#include "i2c_app.h"
#include "timers.h"
#include "i2c_regs.h"


extern volatile uint8_t CLIENT_DATA[];




void PowMgrSystemReset(volatile struct TaskDescr* taskd){
    
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
    tx[0]=0x18;
    
     I2CSwitchMode(I2C1_HOST_MODE);
     
     
    //Trigger power IC system reset
    //#REG0x18_Charger_Control_3 Register
    //BIT3 BATFET_CTRL_WV_BUS -allows batfet off or system power reset with adapter present.
    //0->disable
    //1->enable
    
    //BIT2 BATFET_DLY - Delay time added to the taking action in bits [1:0] of the BATFET_CTRL
    //0x0 = Add 20 ms delay time
    //0x1 = Add 10s delay time (default)
    
    //BIT0:1 BATFET_CTRL
    //The control logic of the BATFET to force the device enter different modes.
    //0x0 = Normal (default)
    //0x1 = Shutdown Mode
    //0x2 = Ship Mode
    //0x3 = System Power Reset
    
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    //enable BATFET_CTRL_WVBUS
    //go to ship mode
    tx[1] = rx[0] | (0x3) | (1<<3) ;
    //set delay to 20ms
    tx[1] = tx[1] & ~(1<<2);
    //default it takes 10sec to go to ship mode after request
    ret += I2CWrite(0x6b, tx, 2);
    
    //here the POWER IC should be reseted
    //todo handle error
    //todo blink period just for debug
    timer_blink_period=200;
    rm_task(TASK_POWER_IC_SYSTEM_RESET);

//    return ret;
}
int PowMgrGoToShipMode(void){
    
    
    
        
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
    tx[0]=0x18;
    
    I2CSwitchMode(I2C1_HOST_MODE);
    
    //Trigger power IC go to ship mode
    //#REG0x18_Charger_Control_3 Register
    //BIT3 BATFET_CTRL_WV BUS -allows BATFET off or system power reset with adapter present.
    //0->disable
    //1->enable
    //BIT0:1 BATFET_CTRL
    //The control logic of the BATFET to force the device enter different modes.
    //0x0 = Normal (default)
    //0x1 = Shutdown Mode
    //0x2 = Ship Mode
    //0x3 = System Power Reset
    
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    //enable BATFET_CTRL_WVBUS
    //go to ship mode
    tx[1] = rx[0] | (0x2) | (1<<3) ;
    //default it takes 10sec to go to ship mode after request
    ret += I2CWrite(0x6b, tx, 2);
    return ret;
    
}
//note: set i2c to host mode before calling
int PowMgrEnableDisableCharging(){
    
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
   

    // enable Force a battery discharging current (~30mA)
    // reset watchdog
    //#REG0x16_Charger_Control_1 Register, 
    // BIT6 FORCE_IBATDIS
    // BIT2 WATCHDOG reset
    tx[0]=0x16;
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    tx[1] = rx[0] | (1<<6) | (1<<2);
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
    uint16_t adc_bits = (uint16_t) (rx[0] + (rx[1]<<8));    //value is little endian
    adc_bits &= (0x1ffe);
    adc_bits = adc_bits >> 1;
    float adc_mV = adc_bits * 1.99f; //mili Volt
    
   
    //disable ADC
    //#REG0x26_ADC_Control Register,BIT7 ADC_EN
    tx[0]=0x26;
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    tx[1] = rx[0] & ~(1<<7);
    ret += I2CWrite(0x6b, tx, 2);
    
    
    //# disable Force a battery discharging current (~30mA)
    //#REG0x16_Charger_Control_1 Register, BIT6 FORCE_IBATDIS
    tx[0]=0x16;
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    tx[1] = rx[0] & ~(1<<6);
    ret += I2CWrite(0x6b, tx, 2);
    
    //VSYSMIN value is ~2500mV under 2000mV there is no battery
    //ADC range can be 0mV-5572mV (0h-AF0h)
    if(ret == 0 && 
            adc_mV > 2000 && 
            adc_bits < 0xAF0){
        //# Charger enable register of bat manager ic
        //#REG0x16_Charger_Control_1 Register, BIT5 EN_CHG
        // 1 : enable
        // 0 : disable
        tx[0]=0x16;
        ret += I2CWriteRead(0x6b, tx,1, rx, 1);
        tx[1] = rx[0] | (1<<5);
        ret += I2CWrite(0x6b, tx, 2);
        
        // /CE pin of battery management ic is active low
        // CHG_DISA pin=0 -> Charge enable
        CHG_DISA_SetLow();
        
        //set bit0 of reg2 STAT
        SET_BAT_AVAIL();

    }else{
        //# Charger enable register of bat manager ic
        //#REG0x16_Charger_Control_1 Register, BIT5 EN_CHG
        // 1 : enable
        // 0 : disable
        tx[0]=0x16;
        ret += I2CWriteRead(0x6b, tx,1, rx, 1);
        tx[1] = rx[0] & ~(1<<5);
        ret += I2CWrite(0x6b, tx, 2);
        
        
        // /CE pin of battery management ic is active low
        // CHG_DISA pin=1 -> Charge disable
        CHG_DISA_SetHigh();
        
        //clear bit0 of reg2 STAT
        CLEAR_BAT_AVAIL();
        
    }
    if(ret != 0){
        SET_BAT_CHCEK_ERR();
//        CLIENT_DATA[12]=(uint8_t)(ret*-1);
    }
    else{
        CLEAR_BAT_ERR();
    }
    
    
    return ret;
    
}