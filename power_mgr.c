
#include "mcc_generated_files/system/system.h"
#include "tasks.h"
#include "i2c_app.h"
#include "timers.h"
#include "i2c_regs_data.h"
#include "power_mgr.h"


extern volatile uint8_t CLIENT_DATA[];


#define BQ_I2C_ADDR (0x6b)

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
    ret += I2CWrite(BQ_I2C_ADDR, tx, 2);
    
    //here the POWER IC should be reseted
    
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
    
    ret += I2CWriteRead(BQ_I2C_ADDR, tx,1, rx, 1);
    //enable BATFET_CTRL_WVBUS
    //go to ship mode
    tx[1] = rx[0] | (0x2) | (1<<3) ;
    //default it takes 10sec to go to ship mode after request
    ret += I2CWrite(BQ_I2C_ADDR, tx, 2);
    return ret;
    
}


volatile int ibatcont = 0;
int read_bq_IRQ=0;
volatile int bq_read_cnt=0;
volatile int bq_read_cnt2=0;
volatile uint64_t dfff=0;//todo remove
volatile uint64_t pi_down_time;//todo remove

void read_bq(volatile struct TaskDescr* taskd){
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
    
    bq_read_cnt++;
    
    //read ibat adc
    //REG0x1D_Charger_Status_0
    tx[0] = 0x1d;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    if (rx[0] & (1<<0)){
        bq_read_cnt2++;
        //wd timer expired
        // reset watchdog
        //#REG0x16_Charger_Control_1 Register, 
        // BIT2 WATCHDOG reset
//        tx[0]=0x16;
//        ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
//        tx[1] = rx[0] | (1<<2);
//        ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);


        //read ibat adc
        //REG0x2A_IBAT_ADC Register
        tx[0] = 0x2A;
        ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 2);
        CLIENT_DATA[REG_IBAT_ADDR] =rx[0];
        CLIENT_DATA[REG_IBAT_ADDR+1] =rx[1];
//        ibatcont = (int)((rx[0] | (rx[1]<<8))) /4;
//        //todo clean,nice
//        //Little endian
//        CLIENT_DATA[REG_IBAT_ADDR] = ((ibatcont & 0xFFFF) & 0xFF ) ;
//        CLIENT_DATA[REG_IBAT_ADDR+1] = ((ibatcont & 0xFFFF) & 0xFF00 ) >>8;
        BQ_INT_N_SetInterruptHandler(NULL);
        dfff = GetTimeMs() - pi_down_time;
        dfff+=10;
        
    }
    rm_task(TASK_CHECK_BQ_IRQ);   

    
}
//todo move me somewhere
void BQ_INT_TEST(){
    if(BQ_INT_N_GetValue()){
        //rising edge
        return;
    }
    //fallin edege
//    read_bq_IRQ=1;
    add_task(TASK_CHECK_BQ_IRQ,read_bq,NULL);   
}

volatile int ibat1=0;
volatile int ibat2=0;
volatile int ibat3=0;
int PowMgrMesIBAT(){
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
    
    
     //read ibat adc
    //REG0x2A_IBAT_ADC Register
    tx[0] = 0x2A;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 2);
    ibat1 = (int)((rx[0] | (rx[1]<<8)) >> 2);
    
    // reset watchdog
    //#REG0x16_Charger_Control_1 Register, 
    // BIT2 WATCHDOG reset
    tx[0]=0x16;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    tx[1] = rx[0] | (1<<2);
    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
    
    //REG0x27_ADC_Function_Disable_0 Registe
    //   bit6: IBAT_ADC_DIS , BAT ADC control
    //     0h = Enable (Default) 1h = Disable
    tx[0] = 0x27;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    tx[1] = rx[0] & (~(1<<6));
    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
    
    //REG0x26_ADC_Control Register
    tx[0] = 0x26;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    //set bits
    // bit7:ADC_EN=enable, bit3:running average, bit2:start new conversion
    tx[1] = rx[0] | (1<<7) | (1<<3) | (1<<2); 
    //clear bits
    //bit6:ADC_RATE=0 (continous mes)
    //bit4:5 ADC_SAMPLE=0 (12bit)
    tx[1] = tx[1] & (~(1<<6)) & (~( 1<<4 | (1<<5))) ;    
    
    
    
//    //todo remove SINGLE ADC read
//    //bit3 ADC_AVG=0 single value
//    tx[1] = tx[1] & (~(1<<3));   
//    //end todo
//    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
    
    //todo test ibat stuff
    
//    
//    //todo remove mew
//    DelayMS(1000);
//    //read ibat adc
//    //REG0x2A_IBAT_ADC Register
//    tx[0] = 0x2A;
//    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 2);
//    ibat2 = (int)((rx[0] | (rx[1]<<8)) >> 2);
//    
//    //todo remove mew
//    DelayMS(1000);
//    //read ibat adc
//    //REG0x2A_IBAT_ADC Register
//    tx[0] = 0x2A;
//    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 2);
//    ibat3 = (int)((rx[0] | (rx[1]<<8)) >> 2);
//    
//    
//    
//    //todo remove mew
//    DelayMS(3000);
//    //read ibat adc
//    //REG0x2A_IBAT_ADC Register
//    tx[0] = 0x2A;
//    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 2);
//    ibat1 = (int)((rx[0] | (rx[1]<<8)) >> 2);
    
    //todo remove me- set wd to 50sec
     // reset watchdog
    //#REG0x16_Charger_Control_1 Register, 
    // BIT0:1 ==> 1 WATCHDOG 50sec
//    tx[0]=0x16;
//    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
//    tx[1] = rx[0] | (1<<1);
//    tx[1] = tx[0] & (~(1<<0));
//    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
    return ret;
}
//note: set i2c to host mode before calling
int PowMgrEnableDisableCharging(){
    
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
   
    //test i2c read, if could not read partid do not check battery return
    uint8_t i=0;
    for(int i =0;i<10;i++){
        //raead partid
        tx[0]=0x38;
        ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
        if((rx[0] & (1<<3)) == (1<<3)){
            break;
        }
        DelayMS(10);
        
    }   
    //if read partid failed return
    if((rx[0] & (1<<3)) != (1<<3)){
        return -1;
    }
    

    // enable Force a battery discharging current (~30mA)
    // reset watchdog
    //#REG0x16_Charger_Control_1 Register, 
    // BIT6 FORCE_IBATDIS
    // BIT2 WATCHDOG reset
    tx[0]=0x16;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    tx[1] = rx[0] | (1<<6) | (1<<2);
    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
    //little delay for discharge current, I know it has a speed of light :D
    DelayMS(100);

    //# enable ADC  
    //#REG0x26_ADC_Control Register,BIT7 ADC_EN
    tx[0]=0x26;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    tx[1] = rx[0] | (1<<7);
    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
    //adc sample takes 24milisec
    DelayMS(100);
    //# read ADC 
    //#REG0x30_VBAT_ADC Register bits 1:12
    tx[0]=0x30;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 2);
    uint16_t adc_bits = (uint16_t) (rx[0] + (rx[1]<<8));    //value is little endian
    adc_bits &= (0x1ffe);
    adc_bits = adc_bits >> 1;
    float adc_mV = adc_bits * 1.99f; //mili Volt
       
    //disable ADC
    //#REG0x26_ADC_Control Register,BIT7 ADC_EN
    tx[0]=0x26;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    tx[1] = rx[0] & ~(1<<7);
    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
    
    //# disable Force a battery discharging current (~30mA)
    //#REG0x16_Charger_Control_1 Register, BIT6 FORCE_IBATDIS
    tx[0]=0x16;
    ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
    tx[1] = rx[0] & ~(1<<6);
    ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
    
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
        ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
        tx[1] = rx[0] | (1<<5);
        ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
        
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
        ret += I2CWriteReadNoIsolator(BQ_I2C_ADDR, tx,1, rx, 1);
        tx[1] = rx[0] & ~(1<<5);
        ret += I2CWriteNoIsolator(BQ_I2C_ADDR, tx, 2);
        
        // /CE pin of battery management ic is active low
        // CHG_DISA pin=1 -> Charge disable
        CHG_DISA_SetHigh();
        
        //clear bit0 of reg2 STAT
        CLEAR_BAT_AVAIL();
        
    }
    if(ret != 0){
        SET_BAT_CHCEK_ERR();
    }
    else{
        CLEAR_BAT_ERR();
    }
    
    
    return ret;
    
}