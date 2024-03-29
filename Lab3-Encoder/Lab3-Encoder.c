/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

// put your includes here for all modules you need to use
#include "Encoder.h"           
#include "Message_Handling.h"  
#include "SerialIO.h"          
#include "Task_Management.h"   
#include "Timing.h"            
 
#include "Lab1_Tasks.h"
#include "Lab2_Tasks.h"
#include "Lab3_Tasks.h"



// put your global variables (only if absolutely required) here.
// Best to identify them as "static" to make them indentified as internal and start with a "_" to identify as internal.
// Try to initialize them if possible, so their values are never arbitrary.

void Initialize_Modules( float _time_not_used_ )
{

         Initialize_USB();
    // reset USB input buffers
    USB_Flush_Input_Buffer();

    // Initialize all modules except USB (it can only be called once without messing things up)
    Initialize_Timing();

    // Intialize Encoders
    Initialize_Encoders();

    // Initalize Battery Monitor
    Initialize_Battery_Monitor();

    // Setup task handling
    Initialize_Task( &task_restart, Initialize_Modules /*function pointer to call*/ );

    // Setup message handling to get processed at some desired rate.
    Initialize_Task( &task_message_handling, Task_Message_Handling );

    // Setup Loop timer and sending time now
    Initialize_Task( &task_time_loop, Send_Loop_Time );
    Initialize_Task( &task_send_time, Send_Time_Now );

    // Setup Encoder Tasks
    Initialize_Task( &task_encoder_read, Send_Encoder );

    // Setup Battery Reading Tasks
    Initialize_Task( &task_battery_read, Send_Battery );
    Initialize_Task( &task_battery_sample, Sample_Battery );

    Initialize_Task( &task_battery_low, Low_Battery) ; 


         
         //Filter Coeffeceints gotten from MATLAB
         // 500 HZ sample rate. 10hz cuttoff. 4th order, Low pass
    float b[] = { 0.1329 * 1.0e-04 ,    0.5317 * 1.0e-04 ,    0.7976 * 1.0e-04 ,    0.5317 * 1.0e-04 ,    0.1329 * 1.0e-04 } ; 
    float a[] = {1.0000   -3.6717    5.0680   -3.1160    0.7199} ; 

    uint8_t filter_order = 4 ; 

         // Initlizes the battery filter
    Filter_Init( &battery_filt, b, a, filter_order) ; 

         // Shifts the battery filter so that its centered around voltages we care about
    Filter_ShiftBy( &battery_filt, Battery_Voltage() );


         
    // Activate relevant tasks
    Task_Activate( &task_message_handling, 0.0 );
    Task_Activate( &task_battery_sample, 2 * 1e-3 );

    Initialize_Task( &task_message_handling_watchdog, Task_Message_Handling_Watchdog );
    Task_Activate( &task_message_handling_watchdog, 100 * 1e-3 );



         
         
}



/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main( void )
{

    // call initialization stuff
         Initialize_Modules( 0.0 );
    for( ;; ) {
        Task_USB_Upkeep();

        Task_Run_If_Ready( &task_message_handling );
        Task_Run_If_Ready( &task_battery_sample );
        // LAB2
        Task_Run_If_Ready( &task_time_loop );
        Task_Run_If_Ready( &task_send_time );
        // LAB3
        Task_Run_If_Ready( &task_encoder_read );
        Task_Run_If_Ready( &task_battery_read );

        Task_Run_If_Ready( &task_restart );

        Task_Run_If_Ready( &task_message_handling_watchdog );
    }

    return 0;
}

// put your task function definitions here
