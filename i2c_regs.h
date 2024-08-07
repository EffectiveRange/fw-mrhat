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


    
//regs
    //0 - always 0 - not writable
    //1-9 CTRL registers -> written by PI
    //10-19 STATUS registers -> only readable
    
#define REG_ADDR_WR_START 1
#define REG_ADDR_WR_END 9

#define REG_ADDR_ALWAYS_ZERO 0
    
#define REG_ADDR_RD_START 10
#define REG_ADDR_RD_END 19


//
//ALWAYS ZERO 
//
#define REG_ALWAYS_ZERO_ADDR 0x0
    
    
//
//Witable regs 1-9    
//
//reg 1:
    //bit 0


//
//Read only regs 10-19
//    

//reg 10:
//  bit0: shutdown req
//  bit1: PI_HB_OK
#define REG_STAT_0_ADDR 10


#define SET_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= 0x1; } while(0);
#define CLEAR_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] &=~(0x1) ; } while(0);
#define IS_SHUTDOWN_REQ()  ((CLIENT_DATA[REG_STAT_0_ADDR]&0x1) == 0x1)
    
#define SET_PI_HB_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= 0x2; } while(0);
#define SET_PI_HB_NOT_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] &=~(0x2) ; } while(0); 
#define IS_PI_HB_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR]&0x2) == 0x2)
#define IS_PI_HB_NOT_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR]&0x2) == 0x0)

    
//reg 11: power related
//  bit0: battery present
//  bit1: bat check error occured
#define REG_BAT_STAT_0_ADDR 11
#define SET_BAT_AVAIL()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] |= 0x1; } while(0);
#define CLEAR_BAT_AVAIL()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] &=~(0x1) ; } while(0);
#define IS_BAT_AVAIL()  ((CLIENT_DATA[REG_BAT_STAT_0_ADDR]&0x1) == 0x1)
 
#define SET_BAT_CHCEK_ERR()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] |= 0x2; } while(0);
#define CLEAR_BAT_ERR()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] &=~(0x2) ; } while(0);
#define IS_BAT_CHECK_ERR()  ((CLIENT_DATA[REG_BAT_STAT_0_ADDR]&0x1) == 0x2)
  
    
//reg 19: i2c error reg + stciky bit ( i2c err + | 0x80 sticky bit)
//  bit0-6: i2c err status
//  bit7:   always 1
#define REG_STAT_I2C_ERR_AND_STICKY_ADDR 19


    
    

#define I2C_CLIENT_LOCATION_SIZE 20
    

extern volatile  uint8_t CLIENT_DATA[] ;



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_REGS_H */

