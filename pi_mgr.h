/* 
 * File:   pi_mgr.h
 * Author: emtszeb
 *
 * Created on August 4, 2024, 11:08 AM
 */

#ifndef PI_MGR_H
#define	PI_MGR_H

#ifdef	__cplusplus
extern "C" {
#endif

    
//PI_RUN pin state changed interrupt handler
void PIRunModeChanged();

//RTC int state changed interrup handler
void RTCPinChanged(void);


void TaskPIShutdownOrWakeup(volatile struct TaskDescr* taskd);
void TaskPIMonitor(volatile struct TaskDescr* taskd);
#ifdef	__cplusplus
}
#endif

#endif	/* PI_MGR_H */

