/* 
 * File:   i2c_regs.h
 * Author: emtszeb
 *
 * Created on August 2, 2024, 5:34 PM
 */

#ifndef I2C_REGS_H
#define	I2C_REGS_H

#ifdef	__cplusplus
extern "C" {
#endif


//reg 0 FW id
//reg1 IRQ reg
//  bit0: shallow shut
//  bit1: deep shutdown reg
//reg2 STAT
//  bit0: battery_available
//  bit1: PI_RUN heartbeat OK
//reg 3 BTN_STAT
//reg 4
    //irq test, number of button long press count
    //0x55 means shutdown is requested from PIC
// reg5
    //PI will fill it to sign shutdown request has been acknowledged
    //0xAA means PI has acked shutdown req
//reg 9 i2c error state
    //value of i2c_client_error_t enum

#define REG_ALWAYS_ZERO_ADDR 0x0
//////////////////////////////ADDR 0x1
    
    
#define REG_SHUTDOWN_CTRL_ADDR 0x01
//bit 0 acked
//////////////////////////////ADDR 0x2

#define REG_STAT_0_ADDR 0x02
    
//bit 0 shutdown req 
#define SET_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= 0x1; } while(0);
#define CLEAR_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] &=~(0x1) ; } while(0);
#define IS_SHUTDOWN_REQ()  ((CLIENT_DATA[REG_STAT_0_ADDR]&0x1) == 0x1)
    
//bit 1 PI_RUN heartbeat OK
//1means PI is not running
//1means PI is RUNNING
//xxxx x11x
#define SET_PI_HB_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= 0x2; } while(0);
#define SET_PI_HB_NOT_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] &=~(0x2) ; } while(0); 
#define IS_PI_HB_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR]&0x2) == 0x2)
#define IS_PI_HB_NOT_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR]&0x2) == 0x0)

    
//////////////////////////////ADDR 0x3
#define REG_CTRL_0_ADDR 0x03
//bit 0 - RTC WATCH - PI requested RTC wakeup
    
#define REG_POWER_STAT_ADDR 0x04
 
#define REG_I2C_CTRL_ADDR 0x5
#define REG_I2C_STAT_ADDR 0x6
    
    
#define I2C_CLIENT_LOCATION_SIZE 10
    

extern volatile  uint8_t CLIENT_DATA[] ;
//easy macros
//
//#define REG_SHUTDOWN_REQ_SET() do { CLIENT_DATA[REG_SHUTDOWN_STAT_ADDR]|=1; } while(0);
//#define REG_SHUTDOWN_ACKED_RESET() do { CLIENT_DATA[REG_SHUTDOWN_CTRL_ADDR]&=~(1); } while(0);
//#define REG_SHUTDOWN_REQ_RESET() do { CLIENT_DATA[REG_SHUTDOWN_STAT_ADDR]&=~(1); REG_SHUTDOWN_ACKED_RESET(); } while(0);
//
//#define IS_REG_SHUTDOWN_ACKED()  ((CLIENT_DATA[REG_SHUTDOWN_CTRL_ADDR]&0x1) == 0x1)

    


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_REGS_H */

