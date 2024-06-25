/**
 * I2C1 Generated Driver File
 *
 * @file i2c1.c
 *
 * @ingroup i2c_client
 *
 * @brief This file contains the driver code for I2C1 module.
 *
 * @version I2C1 Driver Version 2.1.0
 */

/*
? [2023] Microchip Technology Inc. and its subsidiaries.

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

#include <xc.h>
#include "../../../mcc_generated_files/system/config_bits.h"
#include "../i2c1_client.h"

/**
  Section: Driver Interface
 */
const struct I2C_CLIENT_INTERFACE I2C1_Client = {
    .Initialize = I2C1_Client_Initialize,
    .Deinitialize = I2C1_Client_Deinitialize,
    .WriteByte = I2C1_Client_WriteByte,
    .ReadByte = I2C1_Client_ReadByte,
    .TransferDirGet = I2C1_Client_TransferDirGet,
    .LastByteAckStatusGet = I2C1_Client_LastByteAckStatusGet,
    .ErrorGet = I2C1_Client_ErrorGet,
    .CallbackRegister = I2C1_Client_CallbackRegister,
    .Tasks = NULL
};

/**
 Section: Private functions declaration
 */

static bool I2C1_Client_DefaultCallback(i2c_client_transfer_event_t event);

/**
 Section: Private Variable Definitions
 */
static volatile uint16_t i2c1Addr;
static volatile i2c_client_error_t i2c1ErrorState;
static bool(*I2C1_Client_InterruptHandler)(i2c_client_transfer_event_t clientEvent) = NULL;

/**
 Section: Public functions
 */
void I2C1_Client_Initialize(void) {
    /* CSTR Enable clocking; S Cleared by hardware after Start; MODE two 7-bit address with masking; EN disabled; RSEN disabled;  */
    I2C1CON0 = 0x1;
    /* TXU No underflow; CSD Clock Stretching enabled; RXO No overflow; P Cleared by hardware after sending Stop; ACKDT Acknowledge; ACKCNT Acknowledge;  */
    I2C1CON1 = 0x0;
    /* ABD enabled; GCEN disabled; ACNT disabled; SDAHT 300 ns hold time; BFRET 8 I2C Clock pulses;  */
    I2C1CON2 = 0x0;
    /* ACNTMD 8 bits are loaded into I2CxCNTL; FME Standard Mode;  */
    I2C1CON3 = 0x0;
    /* CNT 0x0;  */
    I2C1CNTL = 0XFF;
    I2C1CNTH = 0XFF;
    I2C1ADR0 = 0x66;
    I2C1ADR1 = 0xFE;
    I2C1ADR2 = 0x66;
    I2C1ADR3 = 0xFE;
    /* BAUD 127;  */
    I2C1BAUD = 0x7F;
    /* Enable Interrupts */
    PIE7bits.I2C1IE = 1;
    PIE7bits.I2C1EIE = 1;
    PIE7bits.I2C1RXIE = 1;
    PIE7bits.I2C1TXIE = 1;
    I2C1PIEbits.PCIE = 1;
    I2C1PIEbits.ADRIE = 1;
    I2C1ERRbits.NACKIE = 1;
    I2C1_Client_CallbackRegister(I2C1_Client_DefaultCallback);

    /* Software fix for Silicon bug */
    I2C1PIEbits.PCIE = 0;
    I2C1CON0bits.EN = 1;
    /* Silicon-Errata: Section: 1.3.2 */
#warning "Refer to erratum DS80000870F: https://www.microchip.com/content/dam/mchp/documents/MCU08/ProductDocuments/Errata/PIC18F27-47-57Q43-Silicon-Errata-and-Datasheet-Clarifications-80000870J.pdf"
    I2C1PIEbits.SCIE = 0;
    I2C1PIEbits.PCIE = 0;
    I2C1CON0bits.EN = 1;
    __delay_us(1);
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    I2C1PIRbits.SCIF = 0;
    I2C1PIRbits.PCIF = 0;
    I2C1PIEbits.PCIE = 1;
}

void I2C1_Client_Deinitialize(void) {
    I2C1CON0 = 0x00;
    I2C1CON1 = 0x00;
    I2C1CON2 = 0x00;
    I2C1CON3 = 0x00;
    I2C1CNTL = 0x00;
    I2C1CNTH = 0x00;
    I2C1ADR0 = 0x00;
    I2C1ADR1 = 0x00;
    I2C1ADR2 = 0x00;
    I2C1ADR3 = 0x00;
    I2C1BAUD = 0x00;
    I2C1STAT0 = 0x00;
    I2C1ERR = 0x00;
    /* Disable Interrupts */
    PIE7bits.I2C1IE = 0;
    PIE7bits.I2C1EIE = 0;
    PIE7bits.I2C1RXIE = 0;
    PIE7bits.I2C1TXIE = 0;
    I2C1PIEbits.PCIE = 0;
    I2C1PIEbits.ADRIE = 0;
    I2C1_Client_CallbackRegister(I2C1_Client_DefaultCallback);
}

void I2C1_Client_WriteByte(uint8_t data) {
    I2C1TXB = data;
}

uint8_t I2C1_Client_ReadByte(void) {
    return I2C1RXB;
}

uint16_t I2C1_Client_ReadAddr(void) {
    return (I2C1ADB0 >> 1);
}

i2c_client_error_t I2C1_Client_ErrorGet(void) {
    i2c_client_error_t error;
    error = i2c1ErrorState;
    i2c1ErrorState = I2C_CLIENT_ERROR_NONE;
    return error;
}

i2c_client_transfer_dir_t I2C1_Client_TransferDirGet(void) {
    return (I2C1STAT0bits.R ? I2C_CLIENT_TRANSFER_DIR_READ : I2C_CLIENT_TRANSFER_DIR_WRITE);
}

i2c_client_ack_status_t I2C1_Client_LastByteAckStatusGet(void) {
    return (I2C1ERRbits.NACKIF ? I2C_CLIENT_ACK_STATUS_RECEIVED_NACK : I2C_CLIENT_ACK_STATUS_RECEIVED_ACK);
}

void I2C1_Client_CallbackRegister(bool(*callback)(i2c_client_transfer_event_t clientEvent)) {
    if (callback != NULL) {
        I2C1_Client_InterruptHandler = callback;
    }
}

/*
void __interrupt(irq(I2C1TX, I2C1RX, I2C1), base(8)) I2C1_Client_ISR(void)
{
    I2C1_Client_EventHandler();
}

void __interrupt(irq(I2C1E), base(8)) I2C1_Client_ERROR_ISR(void)
{
    I2C1_Client_ErrorEventHandler();
}
 */

/**
 Section: Private functions
 */
void I2C1_Client_EventHandler(void) {
    if (I2C1PIRbits.PCIF) {
        I2C1PIR = 0x00; /* Clear client interrupt */
        I2C1STAT1bits.CLRBF = 1;
        I2C1CNTL = 0xFF;
        I2C1CNTH = 0xFF;
        I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_STOP_BIT_RECEIVED);
    } else if (I2C1PIRbits.ADRIF) {
        I2C1PIRbits.ADRIF = 0; /* Clear Address interrupt */
        /* Received I2C address must be read out */
        i2c1Addr = I2C1_Client_ReadAddr();
        /* Clear Software Error State */
        i2c1ErrorState = I2C_CLIENT_ERROR_NONE;
        /* Notify that a address match event has occurred */
        if (I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ADDR_MATCH) == true) {
            /* I2C host wants to read */
            if (I2C1STAT0bits.R) {
                if (I2C1STAT1bits.TXBE) {
                    /* In the callback, client must write to transmit register by calling I2Cx_WriteByte() */
                    if (I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_TX_READY) == false) {
                        I2C1CON1bits.ACKDT = 1; /* Send NACK */
                    }
                }
            }

            I2C1CON1bits.ACKDT = 0; /* Send ACK */
        } else {
            I2C1CON1bits.ACKDT = 1; /* Send NACK */
        }
    } else {
        /* Host reads from client, client transmits */
        if (I2C1STAT0bits.R) {
            if ((I2C1STAT1bits.TXBE) && (!I2C1CON1bits.ACKSTAT)) {
                /* I2C host wants to read. In the callback, client must write to transmit register */
                if (I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_TX_READY) == false) {
                    I2C1CON1bits.ACKDT = 1; /* Send NACK */
                }
            }
        } else {
            if (I2C1STAT1bits.RXBF) {
                /* I2C host wants to write. In the callback, client must read data by calling I2Cx_ReadByte()  */
                if (I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_RX_READY) == true) {
                    I2C1CON1bits.ACKDT = 0; /* Send ACK */
                } else {
                    I2C1CON1bits.ACKDT = 1; /* Send NACK */
                }
            }
        }
    }

    /* Data written by the application; release the clock stretch */
    I2C1CON0bits.CSTR = 0;
}

void I2C1_Client_ErrorEventHandler(void) {
    if (I2C1ERRbits.BCLIF) {
        i2c1ErrorState = I2C_CLIENT_ERROR_BUS_COLLISION;
        I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ERROR);
        I2C1ERRbits.BCLIF = 0; /* Clear the Bus collision */
    } else if (I2C1STAT1bits.TXWE) {
        i2c1ErrorState = I2C_CLIENT_ERROR_WRITE_COLLISION;
        I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ERROR);
        I2C1STAT1bits.TXWE = 0; /* Clear the Write collision */
    } else if (I2C1CON1bits.RXO) {
        i2c1ErrorState = I2C_CLIENT_ERROR_RECEIVE_OVERFLOW;
        I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ERROR);
        I2C1CON1bits.RXO = 0; /* Clear the Rx Overflow */
    } else if (I2C1CON1bits.TXU) {
        i2c1ErrorState = I2C_CLIENT_ERROR_TRANSMIT_UNDERFLOW;
        I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ERROR);
        I2C1CON1bits.TXU = 0; /* Clear the Transmit underflow*/
    } else {
        i2c1ErrorState = I2C_CLIENT_ERROR_READ_UNDERFLOW;
        I2C1_Client_InterruptHandler(I2C_CLIENT_TRANSFER_EVENT_ERROR);
        I2C1STAT1bits.RXRE = 0; /* Clear the Receive underflow */
    }

    I2C1ERRbits.NACKIF = 0; /* Clear the common NACKIF */
    I2C1CON0bits.CSTR = 0; /* I2C Clock stretch release */
}

static bool I2C1_Client_DefaultCallback(i2c_client_transfer_event_t event) {
    /* User has to register callback. Refer example code */
    return false;
}
