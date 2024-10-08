/*
	Name: Guy Feigin
	Exercise: Priority queue
	File type: Source code
	Reviewer: Yuval 
	Last updated: Thu 29 Feb 2024 20:02:52 
*/

#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include "srtlist.h" /* srtlist_t */

typedef struct pq
{
	srtlist_t *pqueue;
} pq_t;

pq_t *PQCreate(cmp_func_t cmp_func)
{
	pq_t *pqueue = NULL;
	
	assert(cmp_func);
	
	pqueue = (pq_t *)malloc(sizeof(pq_t));
	if (!pqueue)
	{
		return NULL;
	}
	
	pqueue->pqueue = SrtListCreate(cmp_func);
	if (!pqueue->pqueue)
	{
		free(pqueue);
		return NULL;
	}
	
	return (pqueue);
}

void PQDestroy(pq_t *pq)
{
	assert(pq);
	
	SrtListDestroy(pq->pqueue);
	free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
	assert(pq);
	
	return(SrtListIsIterSame(SrtListEnd(pq->pqueue), 
		   					 SrtListInsert(pq->pqueue, data)));
}

void *PQDequeue(pq_t *pq)
{
	assert(pq);
	
	return (SrtListPopFront(pq->pqueue));
}

void *PQPeek(const pq_t *pq)
{
	assert(pq);
	
	return (SrtListGetData(SrtListBegin(pq->pqueue)));
}

int PQIsEmpty(const pq_t *pq)
{
	assert(pq);
	
	return (SrtListIsEmpty(pq->pqueue));
}

size_t PQCount(const pq_t *pq)
{
	assert(pq);
	
	return (SrtListCount(pq->pqueue));
}

void *PQErase(pq_t *pq, match_func_t match_func, void *param)
{
	void *removed_data = NULL;
	srtlist_iter_t srt_iter; 

	assert(pq);
	assert(match_func);
	
	srt_iter = SrtListFindIf(SrtListBegin(pq->pqueue), 
						   	 SrtListEnd(pq->pqueue), 
						   	 match_func, param);
	if (!SrtListIsIterSame(srt_iter, SrtListEnd(pq->pqueue)))
	{
		removed_data = SrtListGetData(srt_iter);
	
		SrtListRemove(srt_iter);
	}
	
	return (removed_data);			
}

void PQClear(pq_t *pq)
{
	assert(pq);
	
	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

