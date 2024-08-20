/* 
 * File:   bat_mgr.h
 * Author: emtszeb
 *
 * Created on July 11, 2024, 6:24 PM
 */

#ifndef BAT_MGR_H
#define	BAT_MGR_H

#ifdef	__cplusplus
extern "C" {
#endif
    #define BQ_I2C_ADDR (0x6b)

    uint64_t GetTimeMs(void);
    void PowMgrSystemReset(volatile struct TaskDescr*);
    int PowMgrEnableDisableCharging(void);
    int PowMgrGoToShipMode(void);
    void DelayMS(uint32_t delay_ms);
    int PowMgrMesIBAT(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BAT_MGR_H */

