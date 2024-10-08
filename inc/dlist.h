
/*
Name: Guy Feigin
Exercise: Doubly linked list
File Type: Header
Code Reviewer: 
Last Updated: Feb 20 
*/

#ifndef DLIST_H
#define DLIST_H

#include <stddef.h> /*size_t*/

typedef struct dlist dlist_t;
typedef struct dlist_node *dlist_iter_t;

/****************************************************************/
/* General description: 										*/
/*   A doubly linked list is a linked data structure that 		*/
/*   consists of a set of sequentially linked records called    */
/*   nodes. Each node contains three fields: two link fields    */
/*   (references to the previous and to the next node in the    */
/*   sequence of nodes) and one data field. The beginning and   */
/*   ending nodes' previous and next links, respectively, point */
/*   to a terminator, a dummy node, to facilitate traversal     */
/*   of the list.												*/
/****************************************************************/

/****************************************************************/
/* Description: This function receives some data and a param	*/
/*				and checks if there is a match between them.  	*/
/*              It returns 1 if there is a match, 0 otherwise   */  
typedef int (*match_func_t)(const void *data, void *param);

/****************************************************************/
/* Description: This function receives some data and a param	*/
/*				from the list and performs an action on or with */
/*              the given parameter   							*/
typedef int (*action_func_t)(void *data, void *param);

/****************************************************************/
/* Complexity: O(1)												*/
/* Description:  allocates memory for a doubly linked list		*/
/* Arguments:    None											*/
/* Return value: returns a pointer to the list					*/
/* Note: 	     user must use the destroy function to free		*/
/*               memory allocated for the list					*/
dlist_t *DListCreate();

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  deallocates memory of a given list				*/
/* Arguments:    *list - pointer to the list 					*/
/* Return value: None											*/
void DListDestroy(dlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator that points to first element	*/
/*				 in the list									*/
/* Arguments:    *list - pointer to the list 					*/
/* Return value: returns an iterator that points to the first 	*/
/*				 element in the list 							*/
dlist_iter_t DListBegin(const dlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator that points to last element	*/
/*				 in the list									*/
/* Arguments:    *list - pointer to the list 					*/
/* Return value: returns iterator that points to the last 		*/
/*				 element in the list 							*/
dlist_iter_t DListEnd(const dlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator that points to next element	*/
/*				 in the list									*/
/* Arguments:    iter - points to current element  				*/
/* Return value: returns iterator that point to the next 		*/
/*				 element in the list 							*/
dlist_iter_t DListNext(dlist_iter_t iter);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator that points to previous 		*/
/*				 element in the list							*/
/* Arguments:    iter - points to current element				*/
/* Return value: returns iterator that point to the previous	*/
/*				 element in the list 							*/
dlist_iter_t DListPrev(dlist_iter_t iter);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  checks if two iterators point to the same 		*/
/*				 element in the list							*/
/* Arguments:    iter1/iter2 - iterators to be compared			*/
/* Return value: returns 1 if true, 0 if false					*/
int DListIsIterSame(dlist_iter_t iter1, dlist_iter_t iter2);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  inserts new element to the list		 		*/
/* Arguments:    where - position in the list before which the 	*/
/*			             new element will be added				*/
/* Return value: returns iterator that points to the newly		*/
/*				 inserted element								*/
/* Note: 		 inserting at an invalid out of range iterator  */
/*				 will lead to undefined behavior				*/
dlist_iter_t DListInsert(dlist_iter_t where, void* data);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  removes an element from the list		 		*/
/* Arguments:    to_remove - iterator to element to be removed	*/
/* Return value: returns iterator that points to the next 		*/
/*				 element										*/
/* Note:         removing from an empty list will result in     */
/*               undefined behavior								*/
dlist_iter_t DListRemove(dlist_iter_t to_remove);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  extracts the data inside an element	 		*/
/* Arguments:    iter - iterator that points to given element	*/
/* Return value: returns void pointer that points to the data	*/
void *DListGetData(dlist_iter_t iter);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  sets the data inside an element to specified 	*/
/*               value											*/
/* Arguments:    iter - iterator that points to given element	*/
/*				 data - new data to be set to element			*/
/* Return value: None									 		*/
void DListSetData(dlist_iter_t iter, void *data);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  counts the number of elements in the list 		*/
/* Arguments:    *list - pointer to the list				 	*/
/* Return value: returns the number of elements in the list		*/
size_t DListCount(const dlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  checks if the list is empty			 		*/
/* Arguments:    *list - pointer to the list					*/
/* Return value: returns 1 if empty, 0 otherwise				*/
int DListIsEmpty(const dlist_t *list);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  finds an element in a given list		 		*/
/* Arguments:    from - the beginning of the search range		*/
/* 				 to - the end of the search range which is 		*/
/*				 excluded from the search						*/
/*				 is_match - function that checks if the 		*/
/*							elements match						*/
/*				 param - a paramter that is sent as an argument */
/*					     to the match func, on which the 		*/
/*						 comparison is based					*/
/* Return value: returns iterator that points to the found  	*/
/*				 element, or end if no match is found 			*/
/* Note:		 inserting invalid from and to arguments will   */
/*			 	 lead to undefined behavior						*/
dlist_iter_t DListFind(dlist_iter_t from,
					dlist_iter_t to,
					match_func_t is_match,
					void *param);
					
/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  performs an action on each element in the 		*/
/*				 given range in the list 			 			*/
/* Arguments:    from - the beginning of the search range		*/
/* 				 to - the end of the search range which is 		*/
/*				 excluded from the search						*/
/*				 action - function that performs an action on   */
/*						  each element							*/
/* 				 param - parameter to be sent as an argument    */
/*                       to the action funciton					*/
/* Return value: returns 0 if all action functions executed 	*/
/*               successfully. Otherwise returns the error 		*/
/*               status of the function that failed				*/	 
/* Note:		 inserting invalid from and to arguments will   */
/*			 	 lead to undefined behavior						*/
int DListForEach(dlist_iter_t from,
				dlist_iter_t to,
				action_func_t action,
				void *param);
				
/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  moves a given range of elements in one list	*/
/*               into a location in another list				*/
/* Arguments:    from - the beginning of the range 				*/
/*               to - the end of the range that is excluded     */
/*                    from the move 							*/
/*				 where - the position in the target list before */
/*                       which the elements will be inserted    */
/* Return value: None											*/
/* Note:		 inserting invalid from, to and where arguments */
/*			 	 will lead to undefined behavior				*/
void DListSplice(dlist_iter_t from, 
				dlist_iter_t to,
				dlist_iter_t where);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  checks if a given parameter exists anywhere	*/
/*				 in the list and saves all the elements in 		*/
/* 				 which the parameter is found 					*/
/* Arguments:    from - the beginning of the search range		*/
/*				 to - the end of the range (excluded in the 	*/
/*                    search)									*/
/*				 is match - function that checks if the data 	*/
/*                          and parameter match					*/
/*				 param - the parameter that is searched			*/
/*				 *found elements - a list that holds iterators  */
/*                                 that point to the found 		*/
/*								   parameters                   */ 	
/* Return value: returns 0 if at least one match is found,		*/
/*				 non zero otherwise								*/
/* Note:		 inserting invalid from and to arguments will   */
/*			 	 lead to undefined behavior						*/
int DListMultiFind(dlist_iter_t from,
				dlist_iter_t to,
				match_func_t is_match,
				void *param, dlist_t *found_elements);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  inserts an element at the end of the list		*/
/* Arguments:    *list - pointer to the list					*/
/*				 *data - data to be inserted into the list 		*/	
/* Return value: returns an iterator to the element inserted	*/
dlist_iter_t DListPushBack(dlist_t *list, void *data);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  removes an element from the back of the list	*/
/* Arguments:    *list - pointer to the list					*/
/* Return value: returns void pointer to the data in the 		*/
/*               removed element								*/
/* Note:		 using this function on an empty list will 		*/
/*				 result in undefined behavior					*/	  
void *DListPopBack(dlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  inserts an element to the front of the list	*/
/* Arguments:    *list - pointer to the list					*/
/*				 *data - data to be inserted into the list		*/
/* Return value: returns 1 if empty, 0 otherwise				*/
dlist_iter_t DListPushFront(dlist_t *list, void *data);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  removes an element from the front of the list	*/
/* Arguments:    *list - pointer to the list					*/
/* Return value: returns void pointer to the data from the		*/
/*				 removed element								*/
/* Note:		 using this function on an empty list will 		*/
/*				 result in undefined behavior					*/	
void *DListPopFront(dlist_t *list);


#endif /* DLIST_H */


