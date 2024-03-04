<<<<<<< Updated upstream
#include "Lab2_Tasks.h"

// Put your lab-specific task functionality and data_structures (if necessary) here so it is accessable to both
// message handeling and the Lab main loops.
void Send_Loop_Time( float _time_since_last )
{

    static bool isIdle  = true;
    static float period = 0;

    if( isIdle ) {
        period = task_time_loop.run_period;

        task_time_loop.run_period = 0;

        isIdle = false;

    } else {

        struct __attribute__( ( __packed__ ) ) {
            uint8_t time_char;
            float time_loop;
        } data;

        data.time_loop = _time_since_last;

        data.time_char = 1;
        if( period < 0 )
            USB_Send_Msg( "cBf", 't', &data, sizeof( data ) );
        else
            USB_Send_Msg( "cBf", 'T', &data, sizeof( data ) );

        task_time_loop.run_period = period;
        isIdle                    = true;
    }

    return;
}

void Send_Time_Now( float _time_since_last )
{

    struct __attribute__( ( __packed__ ) ) {
        uint8_t time_char;
        float time_now;
    } data;

    data.time_now  = Timing_Get_Time_Sec();
    data.time_char = 0;
    if( task_send_time.run_period > 0 )
        USB_Send_Msg( "cBf", 'T', &data, sizeof( data ) );
    else
        USB_Send_Msg( "cBf", 't', &data, sizeof( data ) );
};
=======
#include "Lab2_Tasks.h"

#include "led_interface.h"

// Put your lab-specific task functionality and data_structures (if necessary) here so it is accessable to both
// message handeling and the Lab main loops.
void Send_Loop_Time( float _time_since_last )
{

    static bool isIdle  = true;
    static float period = 0;

    if( isIdle ) {
        period = task_time_loop.run_period;

        task_time_loop.run_period = 0;

        isIdle = false;

    } else {

        struct __attribute__( ( __packed__ ) ) {
            uint8_t time_char;
            float time_now;
        } data;

        data.time_now  = _time_since_last;
        data.time_char = 1;
        if( period < 0 )
            USB_Send_Msg( "cBf", 't', &data, sizeof( data ) );
        else
            USB_Send_Msg( "cBf", 'T', &data, sizeof( data ) );

        task_time_loop.run_period = period;
        isIdle                    = true;
    }

    return;
}

void Send_Time_Now( float _time_since_last )
{

    struct __attribute__( ( __packed__ ) ) {
        uint8_t time_char;
        float time_now;
    } data;

    data.time_now  = Timing_Get_Time_Sec();
    data.time_char = 0;
    if( task_send_time.run_period > 0 )
        USB_Send_Msg( "cBf", 'T', &data, sizeof( data ) );
    else
        USB_Send_Msg( "cBf", 't', &data, sizeof( data ) );
};
>>>>>>> Stashed changes
