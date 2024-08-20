
#include "mcc_generated_files/system/system.h"
#include "led_ctrl.h"

volatile int act_period = 50;
volatile uint64_t next_period_update = 0;
int dir=1;

enum LEDMode{
    LED_MODE_NONE,
    LED_MODE_TOGGLE, //toggle in every X milisec
    LED_MODE_PATTERN, //using pattern
    LED_MODE_STATIC, //0 or 1
};
enum LEDMode led_mode = LED_MODE_NONE;

uint16_t toggle_period_100ms = 0;
uint16_t toggle_cnt = 0;
uint8_t toggle_state = 0;


void PWM_DutyCycleSetPercentage_Slice1(uint16_t current, uint16_t max, uint16_t periodCountTop)
{
    uint16_t PWM_DytyCyclePercentage = (uint16_t) ((1.0f - ((double) current / (double) max)) * periodCountTop);
    PWM1_16BIT_SetSlice1Output1DutyCycleRegister(PWM_DytyCyclePercentage);
    PWM1_16BIT_LoadBufferRegisters();
}
uint16_t period_cnt =0;
uint8_t led_pattern[] = {
    0,5,10,15,20,25,30,35,40,45, // 0-50:10
    50,55,60,65,70,75,80,85,90,95,//50-100:10
    100,100,100,100,100,100,100,100,100,100, //100:20
    95,90,85,80,75,70,65,60,55,50,//100-50:10
    45,40,35,30,25,20,15,10,5,0,//50-0:10
    0,0,0,0,0,0,0,0,0,0 //
};
uint8_t led_pattern2[] = {
    100,100,100,100,100,100,100,100,100,100, //100:20
    0,0,0,0,0,0,0,0,0,0 //
};
uint8_t led_pattern_cnt = 0;


LEDPattern sleep_pattern ={
    .pattern = led_pattern,
    .len = (sizeof(led_pattern)/sizeof(led_pattern[0]))
};
LEDPattern* act_pattern = NULL;
uint8_t act_pattern_cnt = 0;
#define NUM_LED_PTR  
void LED_UpdateCallback(void){
    //got here every msec
    period_cnt++;
    if(period_cnt < 100-1){
        return;
    }
    period_cnt=0;
    //here at every 100msec
    
    if(led_mode == LED_MODE_STATIC){
        //value already set
        return;
    }
    else if(led_mode == LED_MODE_TOGGLE){        
        if(toggle_cnt>=toggle_period_100ms){
            toggle_cnt=0;
            if(toggle_state){
                toggle_state=0;
                PWM_DutyCycleSetPercentage_Slice1(0,100,63999);

            }else{
                toggle_state=1;
                PWM_DutyCycleSetPercentage_Slice1(100,100,63999);
            }
        }
        toggle_cnt++;
    }
    else if(led_mode == LED_MODE_PATTERN){    
        if(act_pattern == NULL){
            return;
        }
        if(act_pattern_cnt > act_pattern->len - 1){
            act_pattern_cnt=0;
        }
        PWM_DutyCycleSetPercentage_Slice1(
                (uint16_t) (act_pattern->pattern[act_pattern_cnt]),100,63999);

        
        act_pattern_cnt++;
    }
    
}

void LEDSetPattern(LEDPattern* pattern){
    led_mode = LED_MODE_PATTERN;
    act_pattern = pattern;
    act_pattern_cnt=0;
    
    
    
}
//create led blinking with fix toggle time
void LEDSetToggleTime(uint16_t period_ms){
    //for ex 1sec on 1 sec off
    //10 x 0 = 10x10msec off
    //10 x 1 = 10x10msec on
    led_mode = LED_MODE_TOGGLE;
    toggle_period_100ms = period_ms / 100;
    
}
void LEDSetValue(uint8_t val){
    led_mode = LED_MODE_STATIC;
    if (val){
        PWM_DutyCycleSetPercentage_Slice1(100,100,63999);

    }else{
        PWM_DutyCycleSetPercentage_Slice1(0,100,63999);
    }
}