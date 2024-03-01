#include "Lab3_Tasks.h"

void Send_Encoder( float _time_since_last )
{
    struct __attribute__( ( __packed__ ) ) {
        float left_counts;
        float right_counts;
    } encoder;

    encoder.left_counts  = Encoder_Counts_Left();
    encoder.right_counts = Encoder_Counts_Right();

    if( task_encoder_read.run_period > 0 )
        USB_Send_Msg( "cff", 'E', &encoder, sizeof( encoder ) );
    else
        USB_Send_Msg( "cff", 'e', &encoder, sizeof( encoder ) );
    return;

void Send_Battery( float _time_since_last )
{
    struct __attribute__( ( __packed__ ) ) {
        float bat_volt;
    } battery;

    battery.bat_volt = Filter_Last_Output( &battery_filt );

    if( task_battery_read.run_period > 0 )
        USB_Send_Msg( "cf", 'B', &battery, sizeof( battery ) );
    else
        USB_Send_Msg( "cf", 'b', &battery, sizeof( battery ) );
    return;
}

// Samples the current battery voltage
void Sample_Battery( float _time_since_last )
{

    Filter_Value( &battery_filt, 2.0f );

    return;
}
