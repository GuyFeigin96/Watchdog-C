/*
	Name: Guy Feigin
	Exercise: Scheduler task
	File Type: Source code task
	Reviewer: 
	Last Updated: Sun 03 Mar 2024 16:37:37 
*/

#include <time.h> /* time_t */
#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */

#include "uid.h" /* ilrd_uid_t */
#include "task.h" /* task_action_func_t */

struct task
{
	ilrd_uid_t uid;
	task_action_func_t action;
	task_clean_func_t clean_func;
	void *action_params;
	void *cleanup_params;
	size_t interval;
	time_t run_time;
};

task_t *TaskCreate(size_t interval, task_action_func_t action, 
				  task_clean_func_t cleanup, 
				  void *action_params, void *cleanup_params)
{
	task_t *task = NULL;
	
	assert(action);
		
	task = (task_t *)malloc(sizeof(task_t));
	if (!task)
	{
		return NULL;
	}
	
	task->uid = UIDGenerate();
	if (UIDIsEqual(task->uid, bad_uid))
	{
		free(task);
		return NULL;
	}
	
	task->action = action;
	task->action_params = action_params;

	task->clean_func = cleanup;
	task->cleanup_params = cleanup_params;
	
	task->interval = interval;
	
	task->run_time = time(NULL) + interval;
	
	return (task);
} 

void TaskDestroy(task_t *task)
{
	assert(task);
	
	if (task->clean_func)
	{
		task->clean_func(task->cleanup_params);
	}
	
	free(task);
}

ilrd_uid_t TaskGetUID(const task_t *task)
{
	assert(task);
	
	return (task->uid);
}

int TaskRun(task_t *task)
{
	int status = task->action(task->action_params);
	
	if (status < 0 && task->clean_func)
	{
		task->clean_func(task->cleanup_params);
	}
	
	return (status);
}

int TaskIsSame(const task_t *task1, const task_t *task2)
{
	assert(task1);
	assert(task2);
	
	return (UIDIsEqual(task1->uid, task2->uid));
}

time_t TaskGetTimeToRun(const task_t *task)
{
	assert(task);
	
	return (task->run_time);
}

void TaskUpdateTimeToRun(task_t *task)
{
	assert(task);
	
	task->run_time = time(0) + task->interval;
}

