/**
 * I2C1 Generated Driver API Header File
 *
 * @file i2c1.h
 *
 * @defgroup i2c_client I2C1_CLIENT
 *
 * @brief This file contains API prototypes and other data types for I2C1 module.
 *
 * @version I2C1 Driver Version 2.1.0
 */

/*
� [2023] Microchip Technology Inc. and its subsidiaries.

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

#ifndef I2C1_CLIENT_H
#define I2C1_CLIENT_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c_client_interface.h"

/**
 Section: Data Type Definitions
 */

#define i2c1_client_client_interface I2C1_Client

/**
 * @ingroup I2C1_client
 * @brief External object for I2C1_Client
 */
extern const struct I2C_CLIENT_INTERFACE I2C1_Client;

/**
 * @ingroup i2c_client
 * @brief This routine initializes the I2C1 module.
 *        This routine must be called before any other I2C1 routine is called.
 *        This routine should only be called once during system initialization.
 * @param void
 * @return void
 */
void I2C1_Client_Initialize(void);

/**
 * @ingroup i2c_client
 * @brief This routine Deinitializes the I2C1 module.
 *        This function disables the module.
 * @param void
 * @return void
 */
void I2C1_Client_Deinitialize(void);

/**
 * @ingroup i2c_client
 * @brief This function writes data to a host on the bus.
 *        I2C1_Client_Initialize() must have been called for the associated.
 * @param [in] wrByte - I2C1 client write byte
 * @return void
 */
void I2C1_Client_WriteByte(uint8_t wrByte);

/**
 * @ingroup i2c_client
 * @brief This function read data from a host on the bus.
 *        I2C1_Client_Initialize() must have been called for the associated.
 * @param void
 * @return data byte -I2C1 client read byte
 */
uint8_t I2C1_Client_ReadByte(void);

/**
 * @ingroup i2c_client
 * @brief This function read requested address from a host on the bus.
 *        I2C1_Client_Initialize() must have been called for the associated.
 * @param void
 * @return address byte -I2C1 client read byte
 */
uint16_t I2C1_Client_ReadAddr(void);

/**
 * @ingroup i2c_client
 * @brief This function the error occurred during I2C Transmit and Receive.
 *        I2C1_Client_Initialize() must have been called for the associated.
 * @param void
 * @return
 *        I2C_CLIENT_ERROR_BUS_COLLISION    - I2C Bus Collision Error
 *        I2C_CLIENT_ERROR_WRITE_COLLISION  - I2C Write Collision Error
 *        I2C_CLIENT_ERROR_RECEIVE_OVERFLOW - I2C Receive overflow
 *        I2C_CLIENT_ERROR_NONE             - No Error
 */
i2c_client_error_t I2C1_Client_ErrorGet(void);

/**
 * @ingroup i2c_client
 * @brief This function get the I2C Transfer direction.
 *        I2C1_Client_Initialize() must have been called for the associated.
 * @param void
 * @return
 *        I2C_CLIENT_TRANSFER_DIR_WRITE  - I2C Host is writing to client
 *        I2C_CLIENT_TRANSFER_DIR_READ   - I2C Host is reading from client
 */
i2c_client_transfer_dir_t I2C1_Client_TransferDirGet(void);

/**
 * @ingroup i2c_client
 * @brief This function get the I2C Host ACK status.
 *        I2C1_Client_Initialize() must have been called for the associated.
 * @param void
 * @return
 *        I2C_CLIENT_ACK_STATUS_RECEIVED_ACK   - I2C Host is send ACK to client
 *        I2C_CLIENT_ACK_STATUS_RECEIVED_NACK  - I2C Host is send NACK to client
 */
i2c_client_ack_status_t I2C1_Client_LastByteAckStatusGet(void);

/**
 * @ingroup i2c_client
 * @brief This function sets the pointer to be called "back" when
 *        the given I2C transfer events, Bus collision and Write collision occur.
 * @param handler - A pointer to a function.
 * @return void
 */
void I2C1_Client_CallbackRegister(bool (*callback)(i2c_client_transfer_event_t clientEvent));



#endif //I2C1_CLIENT_H
