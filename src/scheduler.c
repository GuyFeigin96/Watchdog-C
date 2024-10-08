/*
	Name: Guy Feigin
	Exercise: Scheduler
	File Type: Source code
	Reviewer: Alex Sychev
	Last Updated: Mon 04 Mar 2024 18:17:21   
*/

#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include <unistd.h> /* sleep() */

#include "pqueue.h" /* pqueue_t */
#include "scheduler.h" /* action_func_t */
#include "task.h" /* task_t */

#define PQENQUEUE_SUCCESS (0)

static int PriorityRule(const void *data, const void *dest_data);
static int FindToRemove(const void *data, void *param );

struct scheduler
{
    pq_t *priority_queue;
    task_t *active;
    int is_running;
};

/******************************* Global Functions *****************************/

scheduler_t *SchedCreate(void)
{	
	scheduler_t *sched = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (!sched)
	{
		return NULL;
	}
	
	sched->priority_queue = PQCreate(PriorityRule);
	if (!sched->priority_queue)
	{
		free(sched);
		return NULL;
	}
	
	sched->active = NULL;
	sched->is_running = 0;
	
	return (sched);
}

void SchedDestroy(scheduler_t *sched)
{
	assert(sched);
	
	SchedClear(sched);
	
	PQDestroy(sched->priority_queue);
	
	free(sched);
}

ilrd_uid_t SchedAddTask(scheduler_t *sched, size_t interval, 
						action_func_t action, void *action_param, 
						cleanup_func_t cleanup_func, 
						void *cleanup_param)
{
	task_t *new_task = NULL;
	
	assert(sched);
	assert(action);
	
	new_task = TaskCreate(interval, action, cleanup_func, action_param, 
								  cleanup_param);
	
	if (!new_task)
	{
		return (bad_uid);
	}
	
	if (PQENQUEUE_SUCCESS != PQEnqueue(sched->priority_queue, new_task))
	{
		TaskDestroy(new_task);
		return (bad_uid);
	}
	
	return (TaskGetUID(new_task));
}

int SchedRemoveTask(scheduler_t *sched, ilrd_uid_t task_id)
{
	void *removed_task = NULL;
	
	assert(sched);
	
	removed_task = PQErase(sched->priority_queue, FindToRemove, &task_id);
	
	if (removed_task)
	{
		TaskDestroy(removed_task);
		return SUCCESS;
	}
	
	return ERROR; 
}

int SchedRun(scheduler_t *sched)
{
	int status = SUCCESS;
	
	assert(sched);

	sched->is_running = 1;
	
	while (!SchedIsEmpty(sched) && sched->is_running && status == PQENQUEUE_SUCCESS)
	{			
		while (TaskGetTimeToRun(PQPeek(sched->priority_queue)) > time(NULL))
		{
	  		sleep(1);
		}
		
		sched->active = PQDequeue(sched->priority_queue);
		status = TaskRun(sched->active);
		
		if (status == REPEAT)
		{
			TaskUpdateTimeToRun(sched->active);
			status = PQEnqueue(sched->priority_queue, sched->active);
			
			if (status == 1)
			{
				TaskDestroy(sched->active);
			}
		}
		else
		{
			TaskDestroy(sched->active);
		}

		sched->active = NULL;
	}
	
	sched->is_running = 0;
	
	return status;
}

int SchedStop(scheduler_t *sched)
{
	sched->is_running = 0;
	
	return STOP;
}

void SchedClear(scheduler_t *sched)
{
	assert(sched);
	
	while (!SchedIsEmpty(sched))
	{
		TaskDestroy((task_t *)(PQPeek(sched->priority_queue)));
		PQDequeue(sched->priority_queue);
	}

	if (sched->active)
	{
		TaskDestroy(sched->active);
		sched->active = NULL;
	}
} 

size_t SchedSize(const scheduler_t *sched)
{
	return (PQCount(sched->priority_queue) + (sched->active != NULL));
}

int SchedIsEmpty(const scheduler_t *sched)
{
	assert(sched);
	
	return (PQIsEmpty(sched->priority_queue) && !sched->active);
} 

/******************************* Static Functions *****************************/

static int PriorityRule(const void *data, const void *dest_data)
{
	assert(data);
	assert(dest_data);
	
	return (difftime(TaskGetTimeToRun((task_t *)data), 
					 TaskGetTimeToRun((task_t *)dest_data)));
}

static int FindToRemove(const void *data, void *param )
{
	assert(data);
	assert(param);
	
	return (UIDIsEqual(*(ilrd_uid_t *)data, *(ilrd_uid_t *)param));
}                 
