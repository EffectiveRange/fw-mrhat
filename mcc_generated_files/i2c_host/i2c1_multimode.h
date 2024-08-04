/* 
 * File:   i2c1_multimode.h
 * Author: fecja
 *
 * Created on November 26, 2023, 12:44 PM
 */

#ifndef I2C1_MULTIMODE_H
#define	I2C1_MULTIMODE_H


#include "i2c1.h"
#include "i2c1_client.h"


#ifdef	__cplusplus
extern "C" {
#endif

    
enum I2C1_Mode{
    I2C1_HOST_MODE,
    I2C1_CLIENT_MODE,
    I2C1_NOT_INITED,
};

enum I2C1_Mode I2C1_Current_Mode(void);

void I2C1_Switch_Mode(enum I2C1_Mode);


/**
 * @ingroup i2c_host
 * @brief This API initializes the I2C1 driver in multi mode.
 *        This routine initializes the I2C1 module.
 *        This routine must be called before any other I2C1 routine is called.
 *        This routine should only be called once during system initialization.
 * @param void
 * @return void
 */
void I2C1_Multi_Initialize(void);

/**
 * @ingroup i2c_host
 * @brief This API Deinitializes the I2C1 driver.
 *        This routine disables the I2C1 module.
 * @param void
 * @return void
 */
void I2C1_Multi_Deinitialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C1_MULTIMODE_H */

