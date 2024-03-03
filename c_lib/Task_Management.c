#include "Task_Management.h"

#include "Timing.h"

/** Function Initialize_Task initializes the task to a default state that is inactive.
 *  Note that a negative run_period indicates the task should only be performed once, while
 *  a run_period of 0 indicates the task should be run every time.
 */
void Initialize_Task( Task_t* task, void ( *task_fcn_ptr )( float ) )
{
    task->is_active              = false;
    task->run_period             = -1;
    task->time_last_ran.microsec = 0;
    task->time_last_ran.millisec = 0;
    task->task_fcn_ptr           = task_fcn_ptr;
}

/** Function Task_Activate changes the internal state to enable the task **/
void Task_Activate( Task_t* task, float run_period )
{
    //****** MEGN540 --  START IN LAB 1, UPDATE IN Lab 2 ******//
    // Here you should change the state of the is_active member and set the time to now (lab 2)
    task->is_active = true; // set the task to active
    task->run_period = run_period; // add the run period as prescribed

     task->time_last_ran = Timing_Get_Time();
}

/**
 * @brief Function Task_ReActivate sets the state of is_active to true and resets the time last ran to the current time.
 * This has the same functionality as Task_Activate but it does not allow changing the tasks's run period.
 *
 * @param task is a pointer to the task object of interest
 */
void Task_ReActivate( Task_t* task )
{
    //****** MEGN540 --  START IN LAB 1, UPDATE IN Lab 2 ******//
    // Here you should change the state of the is_active member and set the time to now (lab 2)
    task->is_active = true; // resets the task state to active

    task->time_last_ran = Timing_Get_Time();
}

/** Function Task_Cancel changes the internal state to disable the task **/
void Task_Cancel( Task_t* task )
{
    //****** MEGN540 -- Lab 1 ******//
    // Here you should change the state of the is_active member
    task->is_active = false; // sets the task is inactive
}

/** Function Task_Is_Ready indicates if the task should be run. It checks both
 * the active status and the timing.
 */
bool Task_Is_Ready( Task_t* task )
{
    //****** MEGN540 --  START IN LAB 1, UPDATE IN Lab 2 ******//
    // Note a run_period of 0 indicates the task should be run every time if it is active.
    // Checks if the task is active and has not yet reached its run period 
    if( task->is_active &&
        task->run_period <=
            Timing_Seconds_Since( &( task->time_last_ran ) ) )  // Checks that the task is active and run time is larger than the last run time for task
    {
        return true;
    }

    // Note a run_period of 0 indicates the task should be run every time if it is active.
    return false;  // MEGN540 Update to set the return statement based on is_active and time_last_ran.

}

/**
 * @brief Function Task_Run runs the task-function (if not NULL) and updates the time last ran to the current time. If the run_period is -1, this also
 * deactivates the task.
 *
 * @param task pointer to the task object of interest
 */
void Task_Run( Task_t* task )
{
    // If the function pointer is not NULL (0), run task.
    // To call a void functor (function pointer):  functor_variable();
    // Update time_last_ran and is_active as appropriate.
    // Note that a negative run_period indicates the task should only be performed once, while
    // a run_period of 0 indicates the task should be run every time if it is active.
    if( task->task_fcn_ptr != 0 ) {
        // task->is_active = true;
        task->task_fcn_ptr( Timing_Seconds_Since( &task->time_last_ran ) );  // Does this need to be different?
        // task->task_fcn_ptr( 100 );
    }
    if( task->run_period < 0 ) {
        Task_Cancel( task );
    }
    task->time_last_ran = Timing_Get_Time();
}

/** Function Task_Run_If_Ready Function Task_Run_If_Ready checks to see if the given task is ready for execution, executes the task,
 *  and resets the time_last_ran appropriately. If the task function pointer is NULL then it just
 *  returns if the task is ready and resets the time_last_ran.
 */
bool Task_Run_If_Ready( Task_t* task )
{
    //****** MEGN540 --  START IN LAB 1, UPDATE IN Lab 2   ******//
    // use the prior functions to help with this.
    // Check to see if the task is ready to run.
    // Run it if it is ready

    if (Task_Is_Ready(task)) {
        //If this isn't neede then we can replace the whole thing with return task->is_active
        // Task_Run(task) ; // I don't know if this is supposed to be here for lab 1
        Task_Run(task);
        return true ; 
    } else {
        return false;  // true if it ran, false if it did not run
    }


    
}
