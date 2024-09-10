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

/**************************************************
 * 
 * BYTE 0x0 : ALWAYS ZERO
 * 
 * ************************************************/
//BYTE 0x00 - ALWAYS ZERO
#define REG_ALWAYS_ZERO_ADDR 0x00
    
 
/**************************************************
 * 
 * BYTE 0x1-0x9 : WRITABLE REGISTERS
 * 
 * ************************************************/  

//BYTE 0x1 STICKY BIY and I2C_ERR_STATUS
//reg 1: REG_VAL_I2C_CLIENT_ERRORs + STICKY bit ( i2c err + | 0x80 sticky bit)
//  bit0-6: i2c err status
//  bit7:   always 1
#define REG_STAT_I2C_ERR_AND_STICKY_ADDR 0x1
#define REG_VAL_I2C_CLIENT_ERROR_NONE 0
#define REG_VAL_I2C_CLIENT_ERROR_BUS_COLLISION 1
#define REG_VAL_I2C_CLIENT_ERROR_WRITE_COLLISION 2
#define REG_VAL_I2C_CLIENT_ERROR_RECEIVE_OVERFLOW 3
#define REG_VAL_I2C_CLIENT_ERROR_TRANSMIT_UNDERFLOW 4
#define REG_VAL_I2C_CLIENT_ERROR_READ_UNDERFLOW 5

#define REG_CMD_HALT_ACTION 0x8
#define REG_VAL_HALT_ACTION_RST (1 << 0)
/**************************************************
 * 
 * BYTE 0xA-0x13 : READONLY REGISTERS
 * 
 * ************************************************/  

//BYTE 0xA: Status flags
//  bit0: shutdown req
//  bit1: PI_HB_OK
#define REG_STAT_0_ADDR 0xA
#define SHUT_REQ_POS 0
#define SHUT_REQ (1<<SHUT_REQ_POS)
#define SHUT_REQ_MASK ((1<<SHUT_REQ_POS)) & 0xFF

#define PI_HB_POS 1
#define PI_HB (1<<PI_HB_POS)
#define PI_HB_MASK ((1<<PI_HB_POS)) & 0xFF

//BYTE 0xB: power related
//  bit0: battery present
//  bit1: bat check error occured
#define REG_BAT_STAT_0_ADDR 0x0B
    
#define BAT_AVAIL_POS 0
#define BAT_AVAIL (1<<BAT_AVAIL_POS)
#define BAT_AVAIL_MASK ((1<<BAT_AVAIL_POS)) & 0xFF
    
#define BAT_CHECK_ERR_POS 1
#define BAT_CHECK_ERR (1<<BAT_CHECK_ERR_POS)
#define BAT_CHECK_ERR_MASK ((1<<BAT_CHECK_ERR_POS)) & 0xFF


//BYTE 0xC-0xD Battery current - IBAT 
#define REG_IBAT_ADDR 0x0C
#define REG_IBAT_LEN 2 


    
//BYTE 0x11-0x13 SW VERSION
#define REG_SW_VER_MAJOR_ADDR 0x11
#define REG_SW_VER_MINOR_ADDR 0x12
#define REG_SW_VER_PATCH_ADDR 0x13

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_REGS_H */

