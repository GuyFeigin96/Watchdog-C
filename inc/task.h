/*
	Name: Guy Feigin
	Exercise: Scheduler
	File Type: Header
	Reviewer: Alex Sychev
	Last Updated: Sun 03 Mar 2024 19:43:25  
*/

#ifndef TASK_H
#define TASK_H

#include "uid.h" /* ilrd_uid_t */

/******************************************************************************/
/*
	--Description:
	
	Defines a type task_t representing a task in the scheduler.
*/
typedef struct task task_t;
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Defines a function pointer type task_action_func_t for the action function 
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
typedef int (*task_action_func_t)(void* param);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Defines a function pointer type task_clean_func_t for the cleanup function 
	associated with a task.

	--Signature:

    void function_name(void* param)

	--Arguments:

    param: A pointer to any parameters needed by the cleanup function.

	--Return Value:

    None.
*/
typedef void (*task_clean_func_t)(void* param);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Creates a new task for the scheduler.

	--Arguments:

    interval: Time interval (in seconds) after which the task should be executed 
    		  again.
    action:   Pointer to the function that performs the task's action.
    cleanup:  Pointer to the cleanup function that should be called when the 
    		  task is destroyed.
    action_params: Pointer to any parameters needed by the action function.
    cleanup_params: Pointer to any parameters needed by the cleanup function.

	--Return Value:

    Returns a pointer to the created task on success.
    Returns NULL if memory allocation fails or if the UID generation fails.

	--Undefined Behavior:

    If the action function pointer (action) or parameter action_params are NULL, 
    the behavior is undefined.
*/

task_t *TaskCreate(size_t interval, 
				   task_action_func_t action, 
				   task_clean_func_t cleanup, 
				   void *action_params, 
				   void *cleanup_params); 
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Destroys the given task and frees allocated memory.

	--Arguments:

	task: Pointer to the task to be destroyed.

	--Return Value:

	None.

	--Undefined Behavior:

	If task is NULL, the behavior is undefined.
*/

void TaskDestroy(task_t *task);
/******************************************************************************/ 

/******************************************************************************/
/*
	--Description:
	
	Gets the UID (Unique Identifier) of the given task.

	--Arguments:

    task: Pointer to the task.

	--Return Value:

    Returns the UID of the task.

	--Undefined Behavior:

    If task is NULL, the behavior is undefined.
*/				 
ilrd_uid_t TaskGetUID(const task_t *task);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Runs the action associated with the given task.

	--Arguments:

    task: Pointer to the task to be executed.

	--Return Value:

    Returns the status returned by the action function.
    Returns a negative value if an error occurs during the action execution.

	--Undefined Behavior:

    If task is NULL, the behavior is undefined.
*/
int TaskRun(task_t *task);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Checks if two tasks have the same UID.

	--Arguments:

    task1: Pointer to the first task.
    task2: Pointer to the second task.

	--Return Value:

    Returns 1 if the UIDs of the tasks are equal.
    Returns 0 if the UIDs are different.

	--Undefined Behavior:

    If either task1 or task2 is NULL, the behavior is undefined.
*/
int TaskIsSame(const task_t *task1, const task_t *task2);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Gets the time at which the given task is scheduled to run next.

	--Arguments:

    task: Pointer to the task.

	--Return Value:

    Returns the time (in seconds since the epoch) when the task is scheduled to 
    run next.

	--Undefined Behavior:

    If task is NULL, the behavior is undefined.
*/
time_t TaskGetTimeToRun(const task_t *task);
/******************************************************************************/

/******************************************************************************/
/*
	--Description:
	
	Updates the time at which the given task is scheduled to run next based on 
	its interval.

	--Arguments:

    task: Pointer to the task.

	--Return Value:

    None.

	--Undefined Behavior:

    If task is NULL, the behavior is undefined.
*/
void TaskUpdateTimeToRun(task_t *task);
/******************************************************************************/

#endif /*TASK_H*/


