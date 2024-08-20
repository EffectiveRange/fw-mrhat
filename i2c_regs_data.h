/* 
 * File:   i2c_reg_data.h
 * Author: emtszeb
 *
 * Created on August 11, 2024, 5:57 PM
 */

#ifndef I2C_REG_DATA_H
#define	I2C_REG_DATA_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "i2c_regs.h"


#define SET_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= SHUT_REQ; } while(0);
#define CLEAR_SHUTDOWN_REQ()  do { CLIENT_DATA[REG_STAT_0_ADDR] &= ~(SHUT_REQ) ; } while(0);
#define IS_SHUTDOWN_REQ()  ((CLIENT_DATA[REG_STAT_0_ADDR] & SHUT_REQ_MASK) == SHUT_REQ)

#define SET_PI_HB_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] |= PI_HB; } while(0);
#define SET_PI_HB_NOT_OK()  do { CLIENT_DATA[REG_STAT_0_ADDR] &= ~(PI_HB) ; } while(0); 
#define IS_PI_HB_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR] & PI_HB_MASK) == PI_HB)
#define IS_PI_HB_NOT_OK()  ((CLIENT_DATA[REG_STAT_0_ADDR] & PI_HB_MASK) == 0x0)

#define SET_BAT_AVAIL()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] |= (BAT_AVAIL); } while(0);
#define CLEAR_BAT_AVAIL()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] &= ~(BAT_AVAIL) ; } while(0);
#define IS_BAT_AVAIL()  ((CLIENT_DATA[REG_BAT_STAT_0_ADDR] & BAT_AVAIL_MASK ) == (BAT_AVAIL))


#define SET_BAT_CHCEK_ERR()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] |= BAT_CHECK_ERR; } while(0);
#define CLEAR_BAT_ERR()  do { CLIENT_DATA[REG_BAT_STAT_0_ADDR] &=~(BAT_CHECK_ERR) ; } while(0);
#define IS_BAT_CHECK_ERR()  ((CLIENT_DATA[REG_BAT_STAT_0_ADDR] & BAT_CHECK_ERR_MASK) == (BAT_CHECK_ERR))


#define I2C_CLIENT_LOCATION_SIZE 20
    

extern volatile  uint8_t CLIENT_DATA[] ;



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_REG_DATA_H */

