/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.

*/

#include "Message_Handling.h"

/**
 * Function _Message_Length returns the number of bytes associated with a command string per the
 * class documentation; This is only accessable from this file.
 * @param cmd
 * @return Size of expected string. Returns 0 if unrecognized.
 */
static uint8_t _Message_Length( char cmd );

/**
 * Function Task_Message_Handling processes USB messages as necessary and sets
 * status flags to control the flow of the program.
 */
void Task_Message_Handling( float _time_since_last )
{
    // *** MEGN540  ***

    if( !USB_Msg_Length() ) {  // if there is nothing to process...
        return;
    }

    char command = USB_Msg_Peek();  // use Peek to get the operator without removing it so the process keeps going

    bool command_processed = false;

    switch( command ) {  // process operator using a switch statement
        case '*':
            if( USB_Msg_Length() >= _Message_Length( '*' ) ) {  // then process your multiplication...

                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we know it is '*' so it isn't saved as a variable

                struct __attribute__( ( __packed__ ) ) {  // makes a struct called data with two floats
                    float v1;
                    float v2;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );  // copies bytes from usb receive buffer to struct

                Multiply_And_Send( data.v1, data.v2 );  // does the multiplication, sends the usb message

                // /* MEGN540 -- LAB 2 */ command_processed = true; (future dev)
                command_processed = true;
            }
            break;
        case '/':
            if( USB_Msg_Length() >= _Message_Length( '/' ) ) {  // then process your divide...

                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we know it is '/' so it isn't saved as a variable

                struct __attribute__( ( __packed__ ) ) {  // makes a struct called data with two floats
                    float v1;
                    float v2;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );  // copies bytes from usb receive buffer to struct

                Divide_And_Send( data.v1, data.v2 );  // does the division, sends the usb message

                // /* MEGN540 -- LAB 2 */ command_processed = true; (future dev)
                command_processed = true;
            }
            break;
        case '+':
            if( USB_Msg_Length() >= _Message_Length( '+' ) ) {  // then process your addition...

                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we know it is '+' so it isn't saved as a variable

                struct __attribute__( ( __packed__ ) ) {  // makes a struct called data with two floats
                    float v1;
                    float v2;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );  // copies bytes from usb receive buffer to struct

                Add_And_Send( data.v1, data.v2 );  // does the addition, sends the usb message

                // /* MEGN540 -- LAB 2 */ command_processed = true; (future dev)
                command_processed = true;
            }
            break;
        case '-':
            if( USB_Msg_Length() >= _Message_Length( '-' ) ) {  // then process your subtraction...

                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we know it is '-' so it isn't saved as a variable

                struct __attribute__( ( __packed__ ) ) {  // makes a struct called data with two floats
                    float v1;
                    float v2;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );  // copies bytes from usb receive buffer to struct

                Subtract_And_Send( data.v1, data.v2 );  // does the subtraction, sends the usb message

                // /* MEGN540 -- LAB 2 */ command_processed = true; (future dev)
                command_processed = true;
            }
            break;
        case 't':
            if( USB_Msg_Length() >= _Message_Length( 't' ) ) {
                // then process your minus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                struct __attribute__( ( __packed__ ) ) {
                    uint8_t time_index;
                    float time_now;
                } data;

                data.time_index = USB_Msg_Get();

                switch( data.time_index )  // Check if next value is 0 or 1 else return error message
                {
                    case 0:  // Sends the current time

                        // Turns on this task so that it is run
                        Task_Activate( &task_send_time, -1 );

                        break;
                    case 1:  // Sends the time to complete one MAIN loop

                        // Turns on this task so that it is run
                        Task_Activate( &task_time_loop, -1 );

                        break;

                    default: USB_Send_Msg( "cB", '?', &data.time_index, sizeof( data.time_index ) ); break;
                }
            }
            command_processed = true;
            break;

        case 'T':
            if( USB_Msg_Length() >= _Message_Length( 'T' ) ) {

                // then process your minus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                struct __attribute__( ( __packed__ ) ) {
                    uint8_t time_index;
                    float run_period;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );
                if( data.run_period > 0 ) {
                    switch( data.time_index )  // Check if next value is 0 or 1 else return error message
                    {
                        case 0:  // Sends the current time

                            // USB_Send_Msg( "cf", 'R', &data.run_period, sizeof( data.run_period ) );
                            // Turns on this task so that it is run

                            Task_Activate( &task_send_time, data.run_period * 1e-3 );

                            break;
                        case 1:  // Sends the time to complete one MAIN loop

                            // Turns on this task so that it is run
                            Task_Activate( &task_time_loop, data.run_period * 1e-3 );
                            break;

                        default: USB_Send_Msg( "cB", '?', &data.time_index, sizeof( data.time_index ) ); break;
                    }
                } else if( data.run_period <= 0 ) {
                    Task_Cancel( &task_time_loop );
                    Task_Cancel( &task_send_time );
                };
            }
            command_processed = true;
            break;
        case 'e':
            if( USB_Msg_Length() >= _Message_Length( 'e' ) ) {
                // then process your minus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                Task_Activate( &task_encoder_read, -1 );
                // Build a meaningful structure to put your data in. Here we want two
                // floats.

                command_processed = true;
            }
            break;
        case 'E':
            if( USB_Msg_Length() >= _Message_Length( 'E' ) ) {

                // then process your minus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                struct __attribute__( ( __packed__ ) ) {
                    float run_period;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );

                if( data.run_period > 0 ) {
                    // Turns on this task so that it is run
                    Task_Activate( &task_encoder_read, data.run_period * 1e-3 );
                } else {
                    Task_Cancel( &task_encoder_read );
                };
            }
            command_processed = true;
            break;
        case 'b':
            if( USB_Msg_Length() >= _Message_Length( 'b' ) ) {
                // then process your minus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                Task_Activate( &task_battery_read, -1 );
                // Build a meaningful structure to put your data in. Here we want two
                // floats.

                command_processed = true;
            }
            break;
        case 'B':
            if( USB_Msg_Length() >= _Message_Length( 'B' ) ) {
                // then process your minus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                // we already know it was a * so no need to save it as a
                // variable
                struct __attribute__( ( __packed__ ) ) {
                    float run_period;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );

                if( data.run_period > 0 ) {
                    // Turns on this task so that it is run
                    Task_Activate( &task_battery_read, data.run_period * 1e-3 );
                } else {
                    Task_Cancel( &task_battery_read );
                };

                command_processed = true;
            }
            break;
        case '~':
            if( USB_Msg_Length() >= _Message_Length( '~' ) ) {  // then process your reset...

                // task_restart = true; // Sets the task_restart flag defined in Lab1_Tasks.h
                USB_Msg_Get();
                USB_Send_Byte( 0 );
                Task_Activate( &task_restart, -1 );

                return;

                // /* MEGN540 -- LAB 2 */ command_processed = true; (future dev)
            }
            break;
        default:                   // case for unknown command character (unknown operator)
            USB_Msg_Get();         // clears the unknown operator
            USB_Send_Byte( '?' );  // sends a '?'
            break;
    }

    //********* MEGN540 -- LAB 2 ************//
    if( command_processed ) {
        // RESET the WATCHDOG TIMER
        Task_ReActivate( &task_message_handling_watchdog );
    }
}

/**
 * @brief Function Task_Message_Handling_Watchdog clears the USB recieve (deleting all messages) to flush the buffer if a complete message is not recieved
 * whithin an appropriate amount of time (say 250ms)
 *
 * @param _unused_
 */
void Task_Message_Handling_Watchdog( float _unused_ )
{
    USB_Flush_Input_Buffer();
}

/**
 * Function _Message_Length returns the number of bytes associated with a
 * command string per the class documentation;
 * @param cmd
 * @return Size of expected string. Returns 0 if unreconized.
 */
static uint8_t _Message_Length( char cmd )
{
    switch( cmd ) {
        case '~': return 1; break;
        case '*': return 9; break;
        case '/': return 9; break;
        case '+': return 9; break;
        case '-': return 9; break;
        case 't': return 2; break;
        case 'T': return 6; break;
        case 'e': return 1; break;
        case 'E': return 5; break;
        case 'b': return 1; break;
        case 'B': return 5; break;
        case 'p': return 5; break;
        case 'P': return 9; break;
        case 's': return 1; break;
        case 'S': return 1; break;
        case 'q': return 1; break;
        case 'Q': return 5; break;
        case 'd': return 9; break;
        case 'D': return 13; break;
        case 'v': return 9; break;
        case 'V': return 13; break;
        default: return 0; break;
    }
}