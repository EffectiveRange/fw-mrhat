/* 
 * File:   onoff.h
 * Author: fecja
 *
 * Created on November 25, 2023, 9:33 AM
 */

#ifndef ONOFF_H
#define	ONOFF_H

#ifdef	__cplusplus
extern "C" {
#endif


    void ONOFF_Initialize(void);
    
    enum ONOFFTypes{
        BTN_1L, // 1 long press
        BTN_1S_1L, // 1 short followed by 1 long press        
        BTN_1S_1S_1L, // 1 short followed by 1 short followed by 1 long press

    };

    void ONOFF_CallbackRegister(void (* CallbackHandler)(enum ONOFFTypes));


#ifdef	__cplusplus
}
#endif

#endif	/* ONOFF_H */

