/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.0
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

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA2 aliases
#define RTC_IRQ_N_TRIS                 TRISAbits.TRISA2
#define RTC_IRQ_N_LAT                  LATAbits.LATA2
#define RTC_IRQ_N_PORT                 PORTAbits.RA2
#define RTC_IRQ_N_WPU                  WPUAbits.WPUA2
#define RTC_IRQ_N_OD                   ODCONAbits.ODCA2
#define RTC_IRQ_N_ANS                  ANSELAbits.ANSELA2
#define RTC_IRQ_N_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define RTC_IRQ_N_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define RTC_IRQ_N_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define RTC_IRQ_N_GetValue()           PORTAbits.RA2
#define RTC_IRQ_N_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define RTC_IRQ_N_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define RTC_IRQ_N_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define RTC_IRQ_N_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define RTC_IRQ_N_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define RTC_IRQ_N_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define RTC_IRQ_N_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define RTC_IRQ_N_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)
#define RA2_SetInterruptHandler  RTC_IRQ_N_SetInterruptHandler

// get/set RA4 aliases
#define ER_MODE_0_TRIS                 TRISAbits.TRISA4
#define ER_MODE_0_LAT                  LATAbits.LATA4
#define ER_MODE_0_PORT                 PORTAbits.RA4
#define ER_MODE_0_WPU                  WPUAbits.WPUA4
#define ER_MODE_0_OD                   ODCONAbits.ODCA4
#define ER_MODE_0_ANS                  ANSELAbits.ANSELA4
#define ER_MODE_0_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define ER_MODE_0_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define ER_MODE_0_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define ER_MODE_0_GetValue()           PORTAbits.RA4
#define ER_MODE_0_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define ER_MODE_0_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define ER_MODE_0_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define ER_MODE_0_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define ER_MODE_0_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define ER_MODE_0_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define ER_MODE_0_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define ER_MODE_0_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set RA5 aliases
#define GPIO_RA5_TRIS                 TRISAbits.TRISA5
#define GPIO_RA5_LAT                  LATAbits.LATA5
#define GPIO_RA5_PORT                 PORTAbits.RA5
#define GPIO_RA5_WPU                  WPUAbits.WPUA5
#define GPIO_RA5_OD                   ODCONAbits.ODCA5
#define GPIO_RA5_ANS                  ANSELAbits.ANSELA5
#define GPIO_RA5_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define GPIO_RA5_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define GPIO_RA5_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define GPIO_RA5_GetValue()           PORTAbits.RA5
#define GPIO_RA5_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define GPIO_RA5_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define GPIO_RA5_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define GPIO_RA5_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define GPIO_RA5_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define GPIO_RA5_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define GPIO_RA5_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define GPIO_RA5_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set RB5 aliases
#define SDA_TRIS                 TRISBbits.TRISB5
#define SDA_LAT                  LATBbits.LATB5
#define SDA_PORT                 PORTBbits.RB5
#define SDA_WPU                  WPUBbits.WPUB5
#define SDA_OD                   ODCONBbits.ODCB5
#define SDA_ANS                  ANSELBbits.
#define SDA_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define SDA_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define SDA_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define SDA_GetValue()           PORTBbits.RB5
#define SDA_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define SDA_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define SDA_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define SDA_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define SDA_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define SDA_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define SDA_SetAnalogMode()      do { ANSELBbits. = 1; } while(0)
#define SDA_SetDigitalMode()     do { ANSELBbits. = 0; } while(0)

// get/set RB6 aliases
#define SCL_TRIS                 TRISBbits.TRISB6
#define SCL_LAT                  LATBbits.LATB6
#define SCL_PORT                 PORTBbits.RB6
#define SCL_WPU                  WPUBbits.WPUB6
#define SCL_OD                   ODCONBbits.ODCB6
#define SCL_ANS                  ANSELBbits.
#define SCL_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define SCL_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define SCL_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define SCL_GetValue()           PORTBbits.RB6
#define SCL_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define SCL_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define SCL_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define SCL_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define SCL_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define SCL_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define SCL_SetAnalogMode()      do { ANSELBbits. = 1; } while(0)
#define SCL_SetDigitalMode()     do { ANSELBbits. = 0; } while(0)

// get/set RB7 aliases
#define MCU_INT_N_TRIS                 TRISBbits.TRISB7
#define MCU_INT_N_LAT                  LATBbits.LATB7
#define MCU_INT_N_PORT                 PORTBbits.RB7
#define MCU_INT_N_WPU                  WPUBbits.WPUB7
#define MCU_INT_N_OD                   ODCONBbits.ODCB7
#define MCU_INT_N_ANS                  ANSELBbits.ANSELB7
#define MCU_INT_N_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define MCU_INT_N_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define MCU_INT_N_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define MCU_INT_N_GetValue()           PORTBbits.RB7
#define MCU_INT_N_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define MCU_INT_N_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define MCU_INT_N_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define MCU_INT_N_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define MCU_INT_N_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define MCU_INT_N_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define MCU_INT_N_SetAnalogMode()      do { ANSELBbits.ANSELB7 = 1; } while(0)
#define MCU_INT_N_SetDigitalMode()     do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set RC0 aliases
#define PWR_LED_CTRL_TRIS                 TRISCbits.TRISC0
#define PWR_LED_CTRL_LAT                  LATCbits.LATC0
#define PWR_LED_CTRL_PORT                 PORTCbits.RC0
#define PWR_LED_CTRL_WPU                  WPUCbits.WPUC0
#define PWR_LED_CTRL_OD                   ODCONCbits.ODCC0
#define PWR_LED_CTRL_ANS                  ANSELCbits.
#define PWR_LED_CTRL_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define PWR_LED_CTRL_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define PWR_LED_CTRL_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define PWR_LED_CTRL_GetValue()           PORTCbits.RC0
#define PWR_LED_CTRL_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define PWR_LED_CTRL_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define PWR_LED_CTRL_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define PWR_LED_CTRL_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define PWR_LED_CTRL_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define PWR_LED_CTRL_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define PWR_LED_CTRL_SetAnalogMode()      do { ANSELCbits. = 1; } while(0)
#define PWR_LED_CTRL_SetDigitalMode()     do { ANSELCbits. = 0; } while(0)

// get/set RC1 aliases
#define BQ_QON_N_TRIS                 TRISCbits.TRISC1
#define BQ_QON_N_LAT                  LATCbits.LATC1
#define BQ_QON_N_PORT                 PORTCbits.RC1
#define BQ_QON_N_WPU                  WPUCbits.WPUC1
#define BQ_QON_N_OD                   ODCONCbits.ODCC1
#define BQ_QON_N_ANS                  ANSELCbits.
#define BQ_QON_N_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define BQ_QON_N_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define BQ_QON_N_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define BQ_QON_N_GetValue()           PORTCbits.RC1
#define BQ_QON_N_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define BQ_QON_N_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define BQ_QON_N_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define BQ_QON_N_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define BQ_QON_N_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define BQ_QON_N_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define BQ_QON_N_SetAnalogMode()      do { ANSELCbits. = 1; } while(0)
#define BQ_QON_N_SetDigitalMode()     do { ANSELCbits. = 0; } while(0)

// get/set RC3 aliases
#define LED_0_TRIS                 TRISCbits.TRISC3
#define LED_0_LAT                  LATCbits.LATC3
#define LED_0_PORT                 PORTCbits.RC3
#define LED_0_WPU                  WPUCbits.WPUC3
#define LED_0_OD                   ODCONCbits.ODCC3
#define LED_0_ANS                  ANSELCbits.ANSELC3
#define LED_0_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define LED_0_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define LED_0_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define LED_0_GetValue()           PORTCbits.RC3
#define LED_0_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define LED_0_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define LED_0_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define LED_0_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define LED_0_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define LED_0_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define LED_0_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define LED_0_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 aliases
#define I2C_SEL_N_TRIS                 TRISCbits.TRISC4
#define I2C_SEL_N_LAT                  LATCbits.LATC4
#define I2C_SEL_N_PORT                 PORTCbits.RC4
#define I2C_SEL_N_WPU                  WPUCbits.WPUC4
#define I2C_SEL_N_OD                   ODCONCbits.ODCC4
#define I2C_SEL_N_ANS                  ANSELCbits.ANSELC4
#define I2C_SEL_N_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define I2C_SEL_N_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define I2C_SEL_N_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define I2C_SEL_N_GetValue()           PORTCbits.RC4
#define I2C_SEL_N_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define I2C_SEL_N_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define I2C_SEL_N_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define I2C_SEL_N_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define I2C_SEL_N_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define I2C_SEL_N_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define I2C_SEL_N_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define I2C_SEL_N_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC5 aliases
#define CHG_DISA_TRIS                 TRISCbits.TRISC5
#define CHG_DISA_LAT                  LATCbits.LATC5
#define CHG_DISA_PORT                 PORTCbits.RC5
#define CHG_DISA_WPU                  WPUCbits.WPUC5
#define CHG_DISA_OD                   ODCONCbits.ODCC5
#define CHG_DISA_ANS                  ANSELCbits.ANSELC5
#define CHG_DISA_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define CHG_DISA_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define CHG_DISA_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define CHG_DISA_GetValue()           PORTCbits.RC5
#define CHG_DISA_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define CHG_DISA_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define CHG_DISA_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define CHG_DISA_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define CHG_DISA_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define CHG_DISA_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define CHG_DISA_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define CHG_DISA_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RC6 aliases
#define PI_RUN_TRIS                 TRISCbits.TRISC6
#define PI_RUN_LAT                  LATCbits.LATC6
#define PI_RUN_PORT                 PORTCbits.RC6
#define PI_RUN_WPU                  WPUCbits.WPUC6
#define PI_RUN_OD                   ODCONCbits.ODCC6
#define PI_RUN_ANS                  ANSELCbits.ANSELC6
#define PI_RUN_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define PI_RUN_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define PI_RUN_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define PI_RUN_GetValue()           PORTCbits.RC6
#define PI_RUN_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define PI_RUN_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define PI_RUN_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define PI_RUN_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define PI_RUN_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define PI_RUN_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define PI_RUN_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define PI_RUN_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)
#define RC6_SetInterruptHandler  PI_RUN_SetInterruptHandler

// get/set RC7 aliases
#define BQ_INT_N_TRIS                 TRISCbits.TRISC7
#define BQ_INT_N_LAT                  LATCbits.LATC7
#define BQ_INT_N_PORT                 PORTCbits.RC7
#define BQ_INT_N_WPU                  WPUCbits.WPUC7
#define BQ_INT_N_OD                   ODCONCbits.ODCC7
#define BQ_INT_N_ANS                  ANSELCbits.ANSELC7
#define BQ_INT_N_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define BQ_INT_N_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define BQ_INT_N_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define BQ_INT_N_GetValue()           PORTCbits.RC7
#define BQ_INT_N_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define BQ_INT_N_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define BQ_INT_N_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define BQ_INT_N_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define BQ_INT_N_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define BQ_INT_N_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define BQ_INT_N_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define BQ_INT_N_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)
#define RC7_SetInterruptHandler  BQ_INT_N_SetInterruptHandler

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handler for the RTC_IRQ_N pin functionality
 * @param none
 * @return none
 */
void RTC_IRQ_N_ISR(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt Handler Setter for RTC_IRQ_N pin interrupt-on-change functionality.
 *        Allows selecting an interrupt handler for RTC_IRQ_N at application runtime
 * @pre Pins intializer called
 * @param InterruptHandler function pointer.
 * @return none
 */
void RTC_IRQ_N_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup  pinsdriver
 * @brief Dynamic Interrupt Handler for RTC_IRQ_N pin.
 *        This is a dynamic interrupt handler to be used together with the RTC_IRQ_N_SetInterruptHandler() method.
 *        This handler is called every time the RTC_IRQ_N ISR is executed and allows any function to be registered at runtime.
 * @pre Pins intializer called
 * @param none
 * @return none
 */
extern void (*RTC_IRQ_N_InterruptHandler)(void);

/**
 * @ingroup  pinsdriver
 * @brief Default Interrupt Handler for RTC_IRQ_N pin. 
 *        This is a predefined interrupt handler to be used together with the RTC_IRQ_N_SetInterruptHandler() method.
 *        This handler is called every time the RTC_IRQ_N ISR is executed. 
 * @pre Pins intializer called
 * @param none
 * @return none
 */
void RTC_IRQ_N_DefaultInterruptHandler(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handler for the PI_RUN pin functionality
 * @param none
 * @return none
 */
void PI_RUN_ISR(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt Handler Setter for PI_RUN pin interrupt-on-change functionality.
 *        Allows selecting an interrupt handler for PI_RUN at application runtime
 * @pre Pins intializer called
 * @param InterruptHandler function pointer.
 * @return none
 */
void PI_RUN_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup  pinsdriver
 * @brief Dynamic Interrupt Handler for PI_RUN pin.
 *        This is a dynamic interrupt handler to be used together with the PI_RUN_SetInterruptHandler() method.
 *        This handler is called every time the PI_RUN ISR is executed and allows any function to be registered at runtime.
 * @pre Pins intializer called
 * @param none
 * @return none
 */
extern void (*PI_RUN_InterruptHandler)(void);

/**
 * @ingroup  pinsdriver
 * @brief Default Interrupt Handler for PI_RUN pin. 
 *        This is a predefined interrupt handler to be used together with the PI_RUN_SetInterruptHandler() method.
 *        This handler is called every time the PI_RUN ISR is executed. 
 * @pre Pins intializer called
 * @param none
 * @return none
 */
void PI_RUN_DefaultInterruptHandler(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handler for the BQ_INT_N pin functionality
 * @param none
 * @return none
 */
void BQ_INT_N_ISR(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt Handler Setter for BQ_INT_N pin interrupt-on-change functionality.
 *        Allows selecting an interrupt handler for BQ_INT_N at application runtime
 * @pre Pins intializer called
 * @param InterruptHandler function pointer.
 * @return none
 */
void BQ_INT_N_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup  pinsdriver
 * @brief Dynamic Interrupt Handler for BQ_INT_N pin.
 *        This is a dynamic interrupt handler to be used together with the BQ_INT_N_SetInterruptHandler() method.
 *        This handler is called every time the BQ_INT_N ISR is executed and allows any function to be registered at runtime.
 * @pre Pins intializer called
 * @param none
 * @return none
 */
extern void (*BQ_INT_N_InterruptHandler)(void);

/**
 * @ingroup  pinsdriver
 * @brief Default Interrupt Handler for BQ_INT_N pin. 
 *        This is a predefined interrupt handler to be used together with the BQ_INT_N_SetInterruptHandler() method.
 *        This handler is called every time the BQ_INT_N ISR is executed. 
 * @pre Pins intializer called
 * @param none
 * @return none
 */
void BQ_INT_N_DefaultInterruptHandler(void);


#endif // PINS_H
/**
 End of File
*/