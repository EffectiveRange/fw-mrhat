
#include "mcc_generated_files/system/system.h"

#include "utils.h"
//#define ENABLE_IBAT_TEST

#ifdef ENABLE_IBAT_TEST

#include "tasks.h"
#include "power_mgr.h"
#include "tasks.h"
#include "onoff.h"
#include "power_mgr.h"
#include "main.h"
#include "timers.h"
#include "i2c_app.h"
#include "i2c_regs_data.h"
#include "led_ctrl.h"
#include "pi_mgr.h"

void LED_UpdateCallback(void);
void I2C1_Multi_Initialize(void) ;
int I2CReadByte(uint8_t dev_addr, uint8_t addr, uint8_t* dest);
//int I2CReadByteNoIsolator(uint8_t dev_addr, uint8_t addr, uint8_t* dest);

int I2CWriteByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t val);
//int I2CWriteByteNoIsolator(uint8_t dev_addr, uint8_t reg_addr, uint8_t val);   
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

void UtilsIBATTest(void){
    SYSTEM_Initialize();
    I2C1_Multi_Initialize();
   
    
    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable();
    
    //1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflowMock);
    
    
    //led control setup
    //set led 0 by default
    PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);
//    LEDSetValue(0);
    LEDSetToggleTime(100);
    
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
    ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_2, &val);
    SET_BIT(val, 7); //BIT7 REG_RST
    ret += I2CWriteByte(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_2, val);

    
    // reset watchdog , enable charing
    ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, &val);
    SET_BIT(val, 2); //BIT2 WATCHDOG reset
    SET_BIT(val, 5); // EN_CHG
    ret += I2CWriteByte(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, val);
    CHG_DISA_SetLow();//en phis charge


    //enable ibat adc
    ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_ADC_FUN_DISABLE_0, &val);
    CLEAR_BIT(val, 6); //BIT6 IBAT_ADC_DIS => 0 to enable
    CLEAR_BIT(val, 4); //BIT4 VBAT_ADC_DIS => 0 to enable
    ret += I2CWriteByte(BQ_I2C_ADDR, BQ_REG_ADC_FUN_DISABLE_0, val);   

    //set up adc params
    ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, &val);
    SET_BIT(val, 7); //ADC_EN=enable
    CLEAR_BIT(val, 6); //ADC_RATE  - 0 continous, 1 one shot
    SET_BIT(val, 5); //ADC_SAMPLE => 3
    SET_BIT(val, 4); //ADC_SAMPLE => 3 9 bit effective resolution
    CLEAR_BIT(val, 3); //ADC_AVG => 0 - singel value mes
    SET_BIT(val, 2); //ADC_AVG_INIT => 0 use existing, 1 start new conversion
    ret += I2CWriteByte(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, val);
    while(1){
        //get iba
        ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_IBAT_ADC, &val);
        ibat_l = val;
        ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_IBAT_ADC+1, &val);
        ibat_h= val;
        ibat = (int)((uint16_t)(ibat_h<<8) | ibat_l);
        float alpha = 0.01f;
        ibat_ema = (float)(alpha * ibat) + (float)(1.0 - alpha) * ibat_ema;


        // get vbat
        ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_VBAT_ADC, &val);
        vbat_l = val;
        ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_VBAT_ADC+1, &val);
        vbat_h= val;
        vbat = (int)((uint16_t)(vbat_h<<8) | vbat_l);

        //reset watdchod
        ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, &val);   
        SET_BIT(val, 2); //BIT2 WATCHDOG reset
        ret += I2CWriteByte(BQ_I2C_ADDR, BQ_REG_CHARGE_CTRL_1, val);
        
        ret += I2CReadByte(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, &val);
        SET_BIT(val, 7); //ADC_EN=enable
        ret += I2CWriteByte(BQ_I2C_ADDR, BQ_REG_ADC_CTRL, val);


//        DelayMS(100);
        __idle2();
    }


}
//end todo
#endif