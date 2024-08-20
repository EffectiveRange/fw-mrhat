/* 
 * File:   led_ctrl.h
 * Author: emtszeb
 *
 * Created on August 4, 2024, 10:27 AM
 */

#ifndef LED_CTRL_H
#define	LED_CTRL_H

#ifdef	__cplusplus
extern "C" {
#endif
    //
    //types
    //
    
    typedef struct{
        uint8_t* pattern;
        uint8_t len;
    }LEDPattern;
    
    //
    //glob vars
    //
    
    LEDPattern sleep_pattern;


    //
    //glob funcs
    //
            
    void LED_UpdateCallback(void);
    void LEDSetToggleTime(uint16_t);
    void LEDSetValue(uint8_t );
    void LEDSetPattern(LEDPattern* pattern);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_CTRL_H */

