#include "mcc_generated_files/system/system.h"

volatile uint64_t free_run_timer = 0;
volatile uint64_t timer_blink_period=0;

uint64_t GetTimeMs(void){
    return free_run_timer;
}

void MiliSecTimerOverflow(void)
{
    free_run_timer++;
    if(timer_blink_period != 0){
        if((free_run_timer % timer_blink_period) == 0){
//            PWR_LED_CTRL_Toggle();
        }
    }   
    
}



void DelayMS(uint32_t delay_ms){
    uint64_t now_ms = free_run_timer;
    while(free_run_timer < (now_ms+delay_ms));
}