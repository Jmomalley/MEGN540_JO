#include "Lab3_Tasks.h"

//Stores the min voltage that the batteries can be without warning
//This includes a %10 increase to ensure that the battery voltage doesn't go below the minimum voltage of the car
float min_voltage = 3.6 + 0.4 ; 

//Setting this to be initally higher that the min voltage as to not trip the warning immediately
static float = temp_bat_voltage = 4 ; 

// Stores weather the battery voltage has been low
static bool isLow = false; 

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
}

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

    temp_bat_voltage = Filter_Value( &battery_filt, Battery_Voltage() );

    if( ( temp_bat_voltage < min_voltage ) && !isLow ) {
        isLow = true;
        Task_Activate( &task_battery_low, 1 );  // Turn on the BAT LOW warning every second with the voltage
    }
    return;
}

void Low_Battery( float _time_since_last){

    // The data that needs to be sent for the warning
    struct __attribute__( ( __packed__ ) ) {
        char message[7]; //Message
        float voltage; //Voltage
    } data = { .message = { 'B', 'A', 'T', ' ', 'L', 'O', 'W' }, .voltage = Filter_Last_Output( &battery_filt ) };

    // Send Warning 
    USB_Send_Msg( "c7sf", '!', &data, sizeof( data ) );

    // Checks if the voltage is low or near the 
    if( ( bat_volt_sample > ( min_voltage ) ) && isLow ) {
        isLow = false; //Reseting for next pass through
        Task_Cancel( &task_battery_low ); //Cancels the task so that it doesn't run continuously 
    }

    return;


}
