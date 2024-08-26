#include "mcc_generated_files/system/system.h"
#include "tasks.h"

volatile uint64_t free_run_timer = 0;
volatile uint64_t timer_blink_period = 0;
volatile uint64_t pi_monitor_period = 16; //run pi monitor every 16msec

uint64_t GetTimeMs(void) {
    return free_run_timer;
}


void TaskPIMonitor(volatile struct TaskDescr* taskd);

void MiliSecTimerOverflow(void) {
    free_run_timer++;

    //do not use modulo operand, it is very slow
    //use and operator
    //x%1024 =  (x&(1024-1)==(1024-1))
    if (((free_run_timer > 1000) &&
            (free_run_timer & (pi_monitor_period - 1)) == (pi_monitor_period - 1))
            ) {
        add_task(TASK_PI_MONITOR, TaskPIMonitor, NULL);
    }
}

void DelayMS(uint32_t delay_ms) {
    uint64_t now_ms = free_run_timer;
    while (free_run_timer < (now_ms + delay_ms));
}