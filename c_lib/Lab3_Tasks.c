#include "Lab3_Tasks.h"

#include "led_interface.h"

// Setup Filter for Battery Readings update later with new filter just testing the abiltiy to filter

static float bat_volt_sample = 0.0f;  // initialize the battery voltage varible

float low_battery = 3.6;  // The low battery threshold to turn on the LOW BAT warning 900mV *4
static bool isLow = false;

// Put your lab-specific task functionality and data_structures (if necessary) here so it is accessable to both
// message handeling and the Lab main loops.
void Send_Encoder( float _time_since_last )
{
    struct __attribute__( ( __packed__ ) ) {
        float left_counts;
        float right_counts;
    } data;

    data.left_counts  = Encoder_Counts_Left();
    data.right_counts = Encoder_Counts_Right();

    if( task_encoder_read.run_period > 0 )
        USB_Send_Msg( "cff", 'E', &data, sizeof( data ) );
    else
        USB_Send_Msg( "cff", 'e', &data, sizeof( data ) );
    return;
}

void Send_Battery( float _time_since_last )
{
    struct __attribute__( ( __packed__ ) ) {
        float bat_volt;
    } data;

    data.bat_volt = Filter_Last_Output( &battery_filt );
    // data.bat_volt = Battery_Voltage();

    if( task_battery_read.run_period > 0 )
        USB_Send_Msg( "cf", 'B', &data, sizeof( data ) );
    else
        USB_Send_Msg( "cf", 'b', &data, sizeof( data ) );
    return;
}

void Sample_Battery( float _time_since_last )
{

    bat_volt_sample = Filter_Value( &battery_filt, Battery_Voltage() );

    if( ( bat_volt_sample < low_battery ) && !isLow ) {
        isLow = true;
        Task_Activate( &task_battery_low, 1 );  // Turn on the BAT LOW warning every second with the voltage
    }
    return;
}

void Low_Battery( float _time_since_last )
{

    struct __attribute__( ( __packed__ ) ) {
        char let[7];
        float volt;
    } data = { .let = { 'B', 'A', 'T', ' ', 'L', 'O', 'W' }, .volt = Filter_Last_Output( &battery_filt ) };

    // Send Warning to Serial that barries need to be charged
    USB_Send_Msg( "c7sf", '!', &data, sizeof( data ) );

    if( ( bat_volt_sample > ( 1.1 * low_battery ) ) && isLow ) {
        isLow = false;
        Task_Cancel( &task_battery_low );
    }

    return;
}
