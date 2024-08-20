/* 
 * File:   timers.h
 * Author: emtszeb
 *
 * Created on August 2, 2024, 5:41 PM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif
    volatile uint64_t timer_blink_period;

    uint64_t GetTimeMs(void);
    void DelayMS(uint32_t delay_ms);
    void MiliSecTimerOverflow(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

