/*
Name: Guy Feigin
Exercise: Doubly linked list
File Type: Source code
Code Reviewer: 
Last Updated: Feb 20 
*/

#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include <stddef.h> /* size_t */

#include "dlist.h" /* dlist_t */

enum status
{
	INSERT_FAIL = -2,
	FAIL = -1,
	SUCCESS
};

typedef struct dlist_node node_t;

struct dlist_node
{
    void *data;
    node_t *next;
    node_t *prev;
};

struct dlist
{
    node_t head;
    node_t tail;
};

static int CountNodes(void *data, void *param);
static dlist_iter_t NodeToIter(node_t *node);
static node_t *IterToNode(dlist_iter_t iter);
static dlist_iter_t GoToEnd(dlist_iter_t iter);
static node_t *CreateNode();

dlist_t *DListCreate()
{
	dlist_t *list = (dlist_t *)malloc(sizeof(dlist_t));
	if (!list)
	{
		return NULL;
	}
	
	list->head.next = &list->tail;
	list->head.prev = NULL;
	list->tail.next = NULL;
	list->tail.prev = &list->head;	

	return list;
}

void DListDestroy(dlist_t *list)
{
	node_t *curr = list->head.next;
	dlist_iter_t temp = NULL;
	
	assert(list);
	
	curr = NodeToIter(curr);
	
	while (!DListIsIterSame(curr, DListEnd(list)))
	{
		temp = curr->next;
		free(curr);
		curr = temp;	
	}
	
	free(list);
	
	list = NULL;
}

dlist_iter_t DListBegin(const dlist_t *list)
{
	assert(list);
	
	return (NodeToIter(list->head.next));
}

dlist_iter_t DListEnd(const dlist_t *list)
{
	assert(list);
	
	return ((dlist_iter_t)&(list->tail));
}

dlist_iter_t DListNext(dlist_iter_t iter)
{	
	node_t *node = IterToNode(iter);
	
	assert(node);
	
	return (NodeToIter(node->next));
}

dlist_iter_t DListPrev(dlist_iter_t iter)
{
	node_t *node = IterToNode(iter);
	
	assert(node);
	
	return (NodeToIter(node->prev));
}

int DListIsIterSame(dlist_iter_t iter1, dlist_iter_t iter2)
{
	assert(iter1);
	assert(iter2);
	
	return (iter1 == iter2);
}

dlist_iter_t DListInsert(dlist_iter_t where, void* data)
{
	node_t *new_node = NULL;
	node_t *where_node = IterToNode(where);
	
	assert(where_node);
	assert(data);
	
	new_node = CreateNode(data, where_node, where_node->prev);
	if (!new_node)
	{
		return (GoToEnd(where));
	}
	
	new_node->prev->next = new_node;
	where_node->prev = new_node;
		
	return (NodeToIter(new_node));
}

dlist_iter_t DListRemove(dlist_iter_t to_remove)
{
	node_t *to_remove_next = NULL;
	to_remove = IterToNode(to_remove);
	to_remove_next = to_remove->next;
	
	assert(to_remove);
	
	to_remove->prev->next = to_remove->next;
	to_remove->next->prev = to_remove->prev;
	
	free(to_remove);
	
	return (NodeToIter(to_remove_next));
}

void *DListGetData(const dlist_iter_t iter)
{
	assert(iter);
	
	return (IterToNode(iter)->data);
}

void DListSetData(dlist_iter_t iter, void *data)
{
	assert(iter);
	
	IterToNode(iter)->data = data;
}

size_t DListCount(const dlist_t *list)
{
	size_t count = 0;
	
	assert(list);
	
	DListForEach(DListBegin(list), DListEnd(list), CountNodes, &count);
	
	return count;
}

int DListIsEmpty(const dlist_t *list)
{
	assert(list);
	
	return (DListIsIterSame(DListBegin(list), DListEnd(list)));
}

dlist_iter_t DListFind(dlist_iter_t from, 
						dlist_iter_t to, 
						match_func_t is_match, 
						void *param)
{
	dlist_iter_t iter = from;
	
	assert(from);
	assert(to);
	assert(is_match);
	
	while (!DListIsIterSame(iter, to) && !(is_match(DListGetData(iter), param)))
	{
		iter = DListNext(iter);
	}
	
	return iter;
}

int DListForEach(dlist_iter_t from, dlist_iter_t to, action_func_t action, 
				 void* param)
{
	dlist_iter_t iter = from;
	int status = SUCCESS;
	
	assert(from);
	assert(to);
	assert(action);
	
	while ((!DListIsIterSame(iter, to)) && SUCCESS == status)
	{
		status = action(DListGetData(iter), param);
		iter = DListNext(iter);
	}
	
	return status;
}

void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
	node_t *from_prev = NULL;
	
	from = IterToNode(from);
	to = IterToNode(to);
	where = IterToNode(where);
	
	from_prev = from->prev;
		
	from->prev->next = to;
	
	to->prev->next = where;
	from->prev = where->prev;
	where->prev->next = from;
	where->prev = to->prev;
	
	to->prev = from_prev;
}

int DListMultiFind(dlist_iter_t from,
				dlist_iter_t to,
				match_func_t is_match,
				void *param, dlist_t *found_elements)
{
	int status = FAIL;
	dlist_iter_t push_back_result = NULL;
	
	assert(from);
	assert(to);
	assert(is_match);
	assert(found_elements);
	
	while (from != to)
	{
		if (is_match(DListGetData(from), param))
		{
			push_back_result = DListPushBack(found_elements, DListGetData(from));
			if (DListIsIterSame(push_back_result, DListEnd(found_elements)))
			{
				return INSERT_FAIL;
			}
			
			status = SUCCESS;
		}
		
		from = DListNext(from);
	}
	
	return (status);
}

dlist_iter_t DListPushBack(dlist_t *list, void *data)
{
	return (DListInsert(DListEnd(list), data));
}

void *DListPopBack(dlist_t *list)
{
	void *removed_node_data = DListGetData(DListPrev(DListEnd(list)));
	
	assert(list); 
	assert(!DListIsEmpty(list));
	
	DListRemove(DListPrev(DListEnd(list)));
	
	return (removed_node_data);
}

dlist_iter_t DListPushFront(dlist_t *list, void *data)
{
	return (DListInsert(DListBegin(list), data));
}

void *DListPopFront(dlist_t *list)
{
	void *removed_node_data = DListGetData(DListBegin(list));
	
	assert(list); 
	assert(!DListIsEmpty(list));
	
	DListRemove(DListBegin(list));
	
	return (removed_node_data);
}

static dlist_iter_t NodeToIter(node_t *node)
{
	assert(node);
	
	return ((dlist_iter_t)node);
}

static node_t *IterToNode(dlist_iter_t iter)
{
	assert(iter);
	
	return ((node_t *)iter);
}

static dlist_iter_t GoToEnd(dlist_iter_t iter)
{
	assert(iter);
	
	while (DListNext(iter))
	{
		iter = DListNext(iter);
	}
	
	return iter;
}

static node_t *CreateNode(void *data, void *next, void *prev)
{
	node_t *node = NULL;
	
	assert(data);
	assert(next);
	assert(prev);
	
	node = (node_t *)(malloc)(sizeof(node_t));
	if (!node)
	{
		return NULL;
	}	
	
	node->data = data;
	node->next = next;
	node->prev = prev;
	
	return node;
}

static int CountNodes(void *data, void *param)
{	
	(void)data;
	
	assert(param);
		
	*(size_t *)param += 1;
	
	return SUCCESS;
}
