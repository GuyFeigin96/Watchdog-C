/*
	Name: Guy Feigin
	Exercise: Scheduler
	File Type: Header
	Reviewer: Alex Sychev
	Last Updated: Mon 04 Mar 2024 18:18:13  
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "uid.h" /* ilrd_uid_t */

/******************************************************************************/
/*
	--Description:
	
	Defines a function pointer type action_func_t for the action function 
	associated with a task.

	--Signature:

    int function_name(void* param)

	--Arguments:

    param: A pointer to any parameters needed by the action function.

	--Return Value:

    Returns an int status indicating the result of the action function.
    A negative value indicates an error, while a non-negative value indicates 
    success.
*/
typedef int (*action_func_t)(void* param);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Defines a type scheduler_t representing a scheduler.
*/
typedef struct scheduler scheduler_t;
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Defines a function pointer type cleanup_func_t for the cleanup function 
	associated with a task.

	--Signature:

    void function_name(void* param)

	--Arguments:

    param: A pointer to any parameters needed by the cleanup function.

	--Return Value:

    None.
*/
typedef void (*cleanup_func_t)(void* param);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Defines an enumeration representing common status codes used in the
	scheduler.

	--Values:

    ERROR: Indicates an error condition with a value of -1.
    SUCCESS: Indicates successful execution with a value of 0.
    STOP: Indicates stopping or termination with a value of 1.
    REPEAT: Indicates that a task requests to be repeated with a value of 2.
*/
enum 
{
    ERROR = -1,
    SUCCESS,
    STOP,
    REPEAT
};
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Creates a new scheduler.

	--Arguments:

    None.

	--Return Value:

    Returns a pointer to the created scheduler on success.
    Returns NULL if memory allocation fails.

	Undefined Behavior:

    None.
*/
scheduler_t *SchedCreate(void); /* O(1) */
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Destroys the given scheduler and frees allocated memory.

	--Arguments:

    sched: Pointer to the scheduler to be destroyed.

	--Return Value:

    None.

	--Undefined Behavior:

    If sched is NULL, the behavior is undefined.
*/
void SchedDestroy(scheduler_t *sched); /* O(n) */
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Adds a new task to the scheduler.

	--Arguments:

    sched: Pointer to the scheduler.
    interval: Time interval (in seconds) after which the task should be executed 
    		  again.
    action: Pointer to the function that performs the task's action.
    action_param: Pointer to any parameters needed by the action function.
    cleanup_func: Pointer to the cleanup function that could be called if the 
    			  task needs memory to be freed.
    cleanup_param: Pointer to any parameters needed by the cleanup function.

	--Return Value:

    Returns the UID of the added task on success.
    Returns bad_uid if memory allocation fails.

	--Undefined Behavior:

    If sched, action, or action_param is NULL, the behavior is undefined.
*/
ilrd_uid_t SchedAddTask(scheduler_t *sched, 
						size_t interval, 
						action_func_t action, 
						void *action_param, 
						cleanup_func_t cleanup_func, 
						void *cleanup_param);  /* O(n) */ 
/******************************************************************************/

/******************************************************************************/		
/*
	--Description:
	
	Removes a task from the scheduler.

	--Arguments:

    sched: Pointer to the scheduler.
    task_id: UID of the task to be removed.

	--Return Value:

    Returns SUCCESS if the task is successfully removed.
    Returns ERROR if the task is not found.

	--Undefined Behavior:

    If sched is NULL, the behavior is undefined.
*/				
int SchedRemoveTask(scheduler_t *sched, ilrd_uid_t task_id);  /* O(n) */
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Starts running the tasks in the scheduler.

	--Arguments:

    sched: Pointer to the scheduler.

	--Return Value:

    Returns the status of the scheduler's execution.
    Returns SUCCESS if the scheduler runs successfully.
    Returns REPEAT if a task requests to be repeated.
    Returns ERROR if an error occurs during execution.
    Returns STOP if the stop function is sent to the scheduler.

	--Undefined Behavior:

    If sched is NULL, the behavior is undefined.
*/
int SchedRun(scheduler_t *sched);  /* O(n) */
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Stops the execution of the scheduler.

	--Arguments:

    sched: Pointer to the scheduler.

	--Return Value:

    Returns STOP.

	--Undefined Behavior:

    None.
*/
int SchedStop(scheduler_t *sched);  /* O(1) */
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Clears all tasks from the scheduler.

	--Arguments:

    sched: Pointer to the scheduler.

	--Return Value:

    None.

	--Undefined Behavior:

    If sched is NULL, the behavior is undefined.
*/
void SchedClear(scheduler_t *sched);  /* O(n) */   
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Gets the number of tasks in the scheduler.

	--Arguments:

    sched: Pointer to the scheduler.

	--Return Value:

    Returns the number of tasks in the scheduler.

	--Undefined Behavior:

    If sched is NULL, the behavior is undefined.
*/
size_t SchedSize(const scheduler_t *sched);  /* O(1) */ 
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Checks if the scheduler is empty.

	--Arguments:

    sched: Pointer to the scheduler.

	--Return Value:

    Returns 1 if the scheduler is empty.
    Returns 0 if the scheduler is not empty.

	--Undefined Behavior:

    If sched is NULL, the behavior is undefined.
*/
int SchedIsEmpty(const scheduler_t *sched);  /* O(1) */ 
/******************************************************************************/

#endif /*SCHEDULER_H*/
