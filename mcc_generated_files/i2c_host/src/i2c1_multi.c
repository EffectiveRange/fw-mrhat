#include "../i2c1_multimode.h"
#include "../../system/interrupt.h"

static enum I2C1_Mode _I2C1_Multi_Mode = I2C1_NOT_INITED;

enum I2C1_Mode I2C1_Current_Mode(void) {
    return _I2C1_Multi_Mode;
}

void I2C1_Switch_Mode(enum I2C1_Mode target) {
    if(target == _I2C1_Multi_Mode){
        return;
    }
    INTERRUPT_GlobalInterruptLowDisable();
    INTERRUPT_GlobalInterruptHighDisable();
    if (target == I2C1_HOST_MODE) {
        I2C1_Client_Deinitialize();
        I2C1_Host_Initialize();
        _I2C1_Multi_Mode = I2C1_HOST_MODE;
    } else {
        I2C1_Host_Deinitialize();
        I2C1_Client_Initialize();
        _I2C1_Multi_Mode = I2C1_CLIENT_MODE;
    }
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
}

void I2C1_Multi_Initialize(void) {
    _I2C1_Multi_Mode = I2C1_CLIENT_MODE;
    I2C1_Client_Initialize();
}

/**
 * @ingroup i2c_host
 * @brief This API Deinitializes the I2C1 driver.
 *        This routine disables the I2C1 module.
 * @param void
 * @return void
 */
void I2C1_Multi_Deinitialize(void) {
    if (_I2C1_Multi_Mode == I2C1_CLIENT_MODE) {
        I2C1_Client_Deinitialize();
    } else {
        I2C1_Host_Deinitialize();
    }
}
