/**
 * Generated Driver File
 * 
 * @file pins.c
 * 
 * @ingroup  pinsdriver
 * 
 * @brief This is generated driver implementation for pins. 
 *        This file provides implementations for pin APIs for all pins selected in the GUI.
 *
 * @version Driver Version 3.1.0
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "../pins.h"

void (*RTC_IRQ_N_InterruptHandler)(void);
void (*PI_RUN_InterruptHandler)(void);
void (*BQ_INT_N_InterruptHandler)(void);

void PIN_MANAGER_Initialize(void)
{
   /**
    LATx registers
    */
    LATA = 0x10;
    LATB = 0x80;
    LATC = 0x1;

    /**
    TRISx registers
    */
    TRISA = 0x7;
    TRISB = 0x0;
    TRISC = 0xC2;

    /**
    ANSELx registers
    */
    ANSELA = 0x3;
    ANSELB = 0x0;
    ANSELC = 0x0;

    /**
    WPUx registers
    */
    WPUA = 0x10;
    WPUB = 0x0;
    WPUC = 0x0;

    /**
    ODx registers
    */
    ODCONA = 0x10;
    ODCONB = 0xE0;
    ODCONC = 0xB9;

    /**
    SLRCONx registers
    */
    SLRCONA = 0x37;
    SLRCONB = 0xE0;
    SLRCONC = 0xFB;

    /**
    INLVLx registers
    */
    INLVLA = 0x3F;
    INLVLB = 0xE0;
    INLVLC = 0xFB;

   /**
    RxyI2C | RxyFEAT registers   
    */
    RC0FEAT = 0x20;
    RC1FEAT = 0x20;
    RC4FEAT = 0x0;
    RC5FEAT = 0x0;
    RB5FEAT = 0x21;
    RB6FEAT = 0x21;
    /**
    PPS registers
    */
    INT0PPS = 0x11; //RC1->INTERRUPT MANAGER:INT0;
    INT1PPS = 0x2; //RA2->INTERRUPT MANAGER:INT1;
    INT2PPS = 0x17; //RC7->INTERRUPT MANAGER:INT2;
    RC0PPS = 0x07;  //RC0->PWM1_16BIT:PWM11;
    I2C1SCLPPS = 0xE;  //RB6->I2C1:SCL1;
    RB6PPS = 0x1C;  //RB6->I2C1:SCL1;
    I2C1SDAPPS = 0xD;  //RB5->I2C1:SDA1;
    RB5PPS = 0x1D;  //RB5->I2C1:SDA1;

   /**
    IOCx registers 
    */
    IOCAP = 0x0;
    IOCAN = 0x4;
    IOCAF = 0x0;
    IOCWP = 0x0;
    IOCWN = 0x0;
    IOCWF = 0x0;
    IOCBP = 0x0;
    IOCBN = 0x0;
    IOCBF = 0x0;
    IOCCP = 0x0;
    IOCCN = 0xC0;
    IOCCF = 0x0;

    RTC_IRQ_N_SetInterruptHandler(RTC_IRQ_N_DefaultInterruptHandler);
    PI_RUN_SetInterruptHandler(PI_RUN_DefaultInterruptHandler);
    BQ_INT_N_SetInterruptHandler(BQ_INT_N_DefaultInterruptHandler);

    // Enable PIE3bits.IOCIE interrupt 
    PIE3bits.IOCIE = 1; 
}
  
void PIN_MANAGER_IOC(void)
{
    // interrupt on change for pin RTC_IRQ_N
    if(IOCAFbits.IOCAF2 == 1)
    {
        RTC_IRQ_N_ISR();  
    }
    // interrupt on change for pin PI_RUN
    if(IOCCFbits.IOCCF6 == 1)
    {
        PI_RUN_ISR();  
    }
    // interrupt on change for pin BQ_INT_N
    if(IOCCFbits.IOCCF7 == 1)
    {
        BQ_INT_N_ISR();  
}
}
   
/**
   RTC_IRQ_N Interrupt Service Routine
*/
void RTC_IRQ_N_ISR(void) {

    // Add custom RTC_IRQ_N code

    // Call the interrupt handler for the callback registered at runtime
    if(RTC_IRQ_N_InterruptHandler)
    {
        RTC_IRQ_N_InterruptHandler();
    }
    IOCAFbits.IOCAF2 = 0;
}

/**
  Allows selecting an interrupt handler for RTC_IRQ_N at application runtime
*/
void RTC_IRQ_N_SetInterruptHandler(void (* InterruptHandler)(void)){
    RTC_IRQ_N_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for RTC_IRQ_N
*/
void RTC_IRQ_N_DefaultInterruptHandler(void){
    // add your RTC_IRQ_N interrupt custom code
    // or set custom function using RTC_IRQ_N_SetInterruptHandler()
}
   
/**
   PI_RUN Interrupt Service Routine
*/
void PI_RUN_ISR(void) {

    // Add custom PI_RUN code

    // Call the interrupt handler for the callback registered at runtime
    if(PI_RUN_InterruptHandler)
    {
        PI_RUN_InterruptHandler();
    }
    IOCCFbits.IOCCF6 = 0;
}

/**
  Allows selecting an interrupt handler for PI_RUN at application runtime
*/
void PI_RUN_SetInterruptHandler(void (* InterruptHandler)(void)){
    PI_RUN_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for PI_RUN
*/
void PI_RUN_DefaultInterruptHandler(void){
    // add your PI_RUN interrupt custom code
    // or set custom function using PI_RUN_SetInterruptHandler()
}
   
/**
   BQ_INT_N Interrupt Service Routine
*/
void BQ_INT_N_ISR(void) {

    // Add custom BQ_INT_N code

    // Call the interrupt handler for the callback registered at runtime
    if(BQ_INT_N_InterruptHandler)
    {
        BQ_INT_N_InterruptHandler();
    }
    IOCCFbits.IOCCF7 = 0;
}

/**
  Allows selecting an interrupt handler for BQ_INT_N at application runtime
*/
void BQ_INT_N_SetInterruptHandler(void (* InterruptHandler)(void)){
    BQ_INT_N_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for BQ_INT_N
*/
void BQ_INT_N_DefaultInterruptHandler(void){
    // add your BQ_INT_N interrupt custom code
    // or set custom function using BQ_INT_N_SetInterruptHandler()
}
/**
 End of File
*/