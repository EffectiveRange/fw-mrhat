/* 
 * File:   i2c_app.h
 * Author: emtszeb
 *
 * Created on August 2, 2024, 5:33 PM
 */

#ifndef I2C_APP_H
#define	I2C_APP_H
#include "mcc_generated_files/i2c_host/i2c1_multimode.h"

#ifdef	__cplusplus
extern "C" {
#endif


    
    
#define I2C_CLIENT_LOCATION_SIZE 10
    
#define I2C_TRY_CNT 3U
#define I2C_TMOUT_MS 100U

extern volatile  uint8_t CLIENT_DATA[] ;


int I2CWrite(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len);
int I2CWriteNoIsolator(uint8_t dev_addr, uint8_t* tx_buf, size_t tx_len);

int I2CWriteRead(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len);
int I2CWriteReadNoIsolator(uint8_t dev_addr, uint8_t* tx_buf,size_t tx_len, uint8_t* rx_buf, size_t rx_len);


void I2CSwitchMode(enum I2C1_Mode new_mode);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_APP_H */

