#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/timer/tmr0.h"
#include "mcc_generated_files/system/interrupt.h"


#include "onoff.h"

static  void (*ONOFF_Callback)(enum ONOFFTypes) = NULL;

void ONOFF_CallbackRegister(void (* CallbackHandler)(enum ONOFFTypes)) {
    if (CallbackHandler != NULL) {
        ONOFF_Callback = CallbackHandler;
    }
}

enum BQQONStates {
    BQQON_IDLE,
    BQQON_FIRST_PRESS_SAMPLING,
    BQQON_WAIT_FOR_2ND_PRESS,
    BQQON_SECOND_PRESS_SAMPLING,
    BQQON_WAIT_FOR_3RD_PRESS,
    BQQON_3RD_PRESS_SAMPLING,

};

// TMR0 is intialized to 50ms counting
#define BTN_LONG_PRESS_TARGET_CNT  (20*3)
#define BTN_SHORT_PRESS_TARGET_CNT  (2)

static struct BQQONState {
    enum BQQONStates m_state;
    unsigned m_cnt;
    unsigned m_target_cnt_lo;
    unsigned m_target_cnt_hi;
} _bqqon_state;

/// State transition helpers
static void BQQONState_to_nth_press_sampling(enum BQQONStates state)
{
    _bqqon_state.m_state = state;
    _bqqon_state.m_cnt = 0;
    _bqqon_state.m_target_cnt_lo = BTN_SHORT_PRESS_TARGET_CNT;
    _bqqon_state.m_target_cnt_hi = BTN_LONG_PRESS_TARGET_CNT;
}

static void BQQONState_wait_for_nth_press(enum BQQONStates state)
{
    _bqqon_state.m_state = state;
    _bqqon_state.m_cnt = 0;
    _bqqon_state.m_target_cnt_lo = BTN_LONG_PRESS_TARGET_CNT;
    _bqqon_state.m_target_cnt_hi = BTN_LONG_PRESS_TARGET_CNT;
}

static void BQQONState_to_first_press_sampling() {
    BQQONState_to_nth_press_sampling(BQQON_FIRST_PRESS_SAMPLING);
    //TMR0H 194; 
    BQQON_Sampling_Start();
}

static void BQQONState_to_idle() {

    _bqqon_state.m_state = BQQON_IDLE;
    _bqqon_state.m_cnt = 0;
    _bqqon_state.m_target_cnt_lo = 0;
    _bqqon_state.m_target_cnt_hi = 0;
    BQQON_Sampling_Stop();
}

static void BQQONState_to_wait_for_second_press() {
    BQQONState_wait_for_nth_press(BQQON_WAIT_FOR_2ND_PRESS);
}

static void BQQONState_to_second_press_sampling() {
    BQQONState_to_nth_press_sampling(BQQON_SECOND_PRESS_SAMPLING);
}

static void BQQONState_to_wait_for_third_press() {
    BQQONState_wait_for_nth_press(BQQON_WAIT_FOR_3RD_PRESS);
}

static void BQQONState_to_third_press_sampling() {
    BQQONState_to_nth_press_sampling(BQQON_3RD_PRESS_SAMPLING);
}

/// Sampling code of ON/OFF switch logic in different states

static void BQQONState_sample_press_impl(enum ONOFFTypes code, void (*lo_pass_transition)(void)) {
    if (BQ_QON_N_GetValue()) {
        if (_bqqon_state.m_cnt >= _bqqon_state.m_target_cnt_lo) {
            lo_pass_transition();
        } else {
            BQQONState_to_idle();
        }
    } else {
        _bqqon_state.m_cnt++;
        if (_bqqon_state.m_cnt >= _bqqon_state.m_target_cnt_hi) {
            BQQONState_to_idle();
            ONOFF_Callback(code);
        }
    }
}

static void BQQONState_sample_first_press() {
    BQQONState_sample_press_impl(BTN_1L, BQQONState_to_wait_for_second_press);
}

static void BQQONState_sample_inbetween() {
    if (BQ_QON_N_GetValue()) {
        _bqqon_state.m_cnt++;
    }
    if (_bqqon_state.m_cnt >= _bqqon_state.m_target_cnt_hi) {
        BQQONState_to_idle();
    }
}

static void BQQONState_sample_second_press() {
    BQQONState_sample_press_impl(BTN_1S_1L, BQQONState_to_wait_for_third_press);
}

static void BQQONState_sample_third_press() {
    BQQONState_sample_press_impl(BTN_1S_1S_1L, BQQONState_to_idle);
}

//// Falling edge interrupt handler on BQQON

static void _start_bqqon_sampling() {
    switch (_bqqon_state.m_state) {
        case BQQON_IDLE:
        case BQQON_FIRST_PRESS_SAMPLING:
            BQQONState_to_first_press_sampling();
            break;
        case BQQON_WAIT_FOR_2ND_PRESS:
            BQQONState_to_second_press_sampling();
            break;
        case BQQON_WAIT_FOR_3RD_PRESS:
            BQQONState_to_third_press_sampling();
            break;
        default:;
    };
}

// Sampling timer callback

static void _sample_bqqon() {
    switch (_bqqon_state.m_state) {
        case BQQON_FIRST_PRESS_SAMPLING:
            BQQONState_sample_first_press();
            break;
        case BQQON_WAIT_FOR_2ND_PRESS:
            BQQONState_sample_inbetween();
            break;
        case BQQON_SECOND_PRESS_SAMPLING:
            BQQONState_sample_second_press();
            break;        
        case BQQON_WAIT_FOR_3RD_PRESS:
            BQQONState_sample_inbetween();
            break;
        case BQQON_3RD_PRESS_SAMPLING:
            BQQONState_sample_third_press();
            break;
        default: // should not happen normally
            BQQONState_to_idle();
            break;
    };
}

static void DefaultHandler(enum ONOFFTypes type) {

}

void ONOFF_Initialize() {
    INT0_SetInterruptHandler(_start_bqqon_sampling);
    BQQON_Sampling_OverflowCallbackRegister(_sample_bqqon);
    ONOFF_CallbackRegister(DefaultHandler);
    BQQONState_to_idle();

}

