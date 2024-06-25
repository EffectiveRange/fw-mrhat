/**
 * I2C Generated Driver Interface Header File
 *
 * @file i2c_client_interface.h
 *
 * @defgroup i2c_client_interface I2C_CLIENT_INTERFACE
 *
 * @brief This file contains other data types for I2C module.
 *
 * @version I2C Driver Version 2.1.0
 */

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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

#ifndef I2C_CLIENT_INTERFACE_H
#define I2C_CLIENT_INTERFACE_H
/**
 * @brief This file contains API prototypes and data structures for I2C Client driver.
 * @ingroup i2c_client_interface  I2C Client Driver
 * \{
 */

/**
  Section: Included Files
 */
#include <stdbool.h>
#include <stdint.h>
#include "i2c_client_types.h"

/**
 Section: Data Type Definitions
 */

/**
 * \defgroup i2c_client_interface
 * @strcut i2c_client_interface_t
 * @brief Structure containing the function pointers of I2C driver.
 */
struct I2C_CLIENT_INTERFACE
{
    void (*Initialize)(void);
    void (*Deinitialize)(void);
    void (*WriteByte)(uint8_t wrByte);
    uint8_t(*ReadByte)(void);
    i2c_client_error_t (*ErrorGet)(void);
    i2c_client_transfer_dir_t (*TransferDirGet)(void);
    i2c_client_ack_status_t (*LastByteAckStatusGet)(void);
    void (*CallbackRegister)(bool (*handler)(i2c_client_transfer_event_t clientEvent));
    void (*Tasks)(void);
};

#endif
