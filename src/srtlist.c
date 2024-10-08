/*
Name: Guy Feigin
Exercise: Sorted list
File Type: Source Code
Code Reviewer: 
Last Updated: Wed 21 Feb 2024 08:03:37   
*/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert() */

#include "srtlist.h" /* srtlist_t */

#ifndef NDEBUG
    #define DEBUG_EXPR(x) {x;}
#else
    #define DEBUG_EXPR(x)
#endif

struct srtlist
{
	dlist_t *list;
	cmp_func_t cmp_func;
};

/*							  Global Functions								  */
/******************************************************************************/

srtlist_t *SrtListCreate(cmp_func_t cmp_func)
{
	srtlist_t *srt_list = NULL;
	
	assert(cmp_func);
	
	srt_list = (srtlist_t *)malloc(sizeof(srtlist_t));
	if (!srt_list)
	{
		return NULL;
	}
	
	srt_list->list = DListCreate();
	if (!srt_list->list)
	{
		free(srt_list);
		return NULL;
	}
	
	srt_list->cmp_func = cmp_func;
	
	return (srt_list);	
}

void SrtListDestroy(srtlist_t *list)
{
	assert(list);
	
	DListDestroy(list->list);
	free(list);
}

srtlist_iter_t SrtListBegin(const srtlist_t *list)
{
    srtlist_iter_t srt_iter;
	
	assert(list);
	
	srt_iter.iter = DListBegin(list->list);
	
	DEBUG_EXPR(srt_iter.list =  (srtlist_t *)list)
	
	return (srt_iter);
}
											
srtlist_iter_t SrtListEnd(const srtlist_t *list)
{
	srtlist_iter_t srt_iter;
	
	assert(list);
	
	srt_iter.iter = DListEnd(list->list);
	
	DEBUG_EXPR(srt_iter.list = (srtlist_t *)list)
	
	return (srt_iter);
}

srtlist_iter_t SrtListNext(srtlist_iter_t iter)
{
	iter.iter = DListNext(iter.iter);
	
	return (iter); 
}

srtlist_iter_t SrtListPrev(srtlist_iter_t iter)
{
	
	iter.iter = DListPrev(iter.iter);
	
	return (iter); 
}

int SrtListIsIterSame(srtlist_iter_t iter1, srtlist_iter_t iter2)
{
	return (iter1.iter == iter2.iter);
}

srtlist_iter_t SrtListInsert(srtlist_t *srtlist, void *data)
{
	srtlist_iter_t iter = SrtListBegin(srtlist);
	
	assert(srtlist);
	assert(data);
	
	while (!SrtListIsIterSame(SrtListEnd(srtlist), iter) && 
		    srtlist->cmp_func(SrtListGetData(iter), data) <= 0)
		  {
		  		iter = SrtListNext(iter);
		  }
		   
	iter.iter = DListInsert(iter.iter, data);
	
	
	DEBUG_EXPR(iter.list = srtlist)
	
	return (iter);
}

srtlist_iter_t SrtListRemove(srtlist_iter_t to_remove)
{
	srtlist_iter_t iter;
	
	iter.iter = DListRemove(to_remove.iter);
	
	return (iter);
}

void *SrtListGetData(srtlist_iter_t iter)
{
	return (DListGetData(iter.iter)); 
}

size_t SrtListCount(const srtlist_t *list)
{
	assert(list);
	  
	return (DListCount(list->list));
}

int SrtListIsEmpty(const srtlist_t *list)
{
	assert(list);
	
	return (DListIsEmpty(list->list));
}

srtlist_iter_t SrtListFind(srtlist_t *list, srtlist_iter_t from,
					srtlist_iter_t to,
					void *data)
{
	int result = -1;

	assert(from.list == to.list);
	
	while (!SrtListIsIterSame(from, to) && (result < 0))
	{
		result = list->cmp_func(SrtListGetData(from), data);
		
		from = SrtListNext(from);
	}
	
	return ((0 == result) ? (SrtListPrev(from)) : to);
}

srtlist_iter_t SrtListFindIf(srtlist_iter_t from,              
					srtlist_iter_t to,
					match_func_t is_match,
					void *param)
{
	srtlist_iter_t iter;
	
	assert(is_match);
	
	iter.iter = DListFind(from.iter, to.iter, is_match, param);
	
	return (iter);
}

int SrtListForEach(srtlist_iter_t from, 
				srtlist_iter_t to,
				action_func_t action,
				void *param)
{
	return (DListForEach(from.iter, to.iter, action, param));
}

void *SrtListPopBack(srtlist_t *list)
{
	assert(list->list);
	
	return (DListPopBack(list->list));
}

void *SrtListPopFront(srtlist_t *list)
{
	assert(list->list);
	
	return (DListPopFront(list->list));
}

void SrtListMerge(srtlist_t *dest, srtlist_t *src)
{
	srtlist_iter_t from;
	srtlist_iter_t to;
	srtlist_iter_t where;
	
	assert(dest);
	assert(src);
	assert(dest != src);
	
	where = SrtListBegin(dest);
	
	while (!SrtListIsEmpty(src))
	{
		/* Place 'from' iterator at the start of source and 'to' after it */
		from = SrtListBegin(src);
		to = SrtListNext(SrtListBegin(src));
		
		/* Move 'where' iterator until it's in the right position */
		while (!SrtListIsIterSame(where, SrtListEnd(dest)) &&
				src->cmp_func(SrtListGetData(where),
				SrtListGetData(from)) <= 0)
		{
			where = SrtListNext(where);
		}
		
		/* Move 'to' iterator until it's in the right position */	
		while (!SrtListIsIterSame(to, SrtListEnd(src)) &&
				src->cmp_func(SrtListGetData(to), 
			    SrtListGetData(where)) < 0)
		{
			to = SrtListNext(to);
		}
		
		DListSplice(from.iter, to.iter, where.iter);
	}
}


