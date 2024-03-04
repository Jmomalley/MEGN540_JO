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

/*
 * This file defines lab-spcific tasks for execution
 */

#ifndef LAB3_TASKS_H
#define LAB3_TASKS_H

#include "SerialIO.h"  // USB interface for sending results back to user
#include "Task_Management.h"
// Include your lab-specific headers here
#include "Battery_Monitor.h"  //Battery Header
#include "Encoder.h"          // Encoder Header
//#include "Filter.h"           //Filter stuff

// Put your lab-specific tasks here
// e.g. Task_t task_restart;  ///<-- Lab 1: This flag indicates that the device received a restart command from the hoast. Default inactive.
Task_t task_encoder_read;    ///<-- Lab 3:
Task_t task_battery_read;    //<Lab 3 battery read task
Task_t task_battery_sample;  // Task to sample battery volage
Task_t task_battery_low;     // Triggers when the battery is low

Filter_Data_t battery_filt;  // The battery filter

// Put your lab-specific task functionality and data_structures (if necessary) here so it is accessable to both
// message handeling and the Lab main loops.
void Send_Encoder( float _time_since_last );
void Send_Battery( float _time_since_last );
void Sample_Battery( float _time_since_last );
void Low_Battery( float _time_since_last );

#endif  // ifndef LAB3_TASKS_H
