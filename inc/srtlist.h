/*
Name: Guy Feigin
Exercise: Sorted list
File Type: Header
Code Reviewer: 
Last Updated: Wed 21 Feb 2024 08:02:51 
*/

#ifndef SRTLIST_H
#define SRTLIST_H

#include <stddef.h> /* size_t */
#include "dlist.h" /* dlist_iter_t */

/****************************************************************/
/* Sorted linked list: a data structure that inherits the 		*/
/* 	characterstics of a linked list and in addition the 		*/
/*  elements in the list are sorted according to some 			*/
/*  predefined property.										*/  

/****************************************************************/
/* Compare function that returns data - param to check if they	*/
/* match and their relative difference if they do not.		    */
typedef int (*cmp_func_t)(const void *data, const void *param);

typedef struct srtlist srtlist_t;
typedef struct srtlist_iter
{
    dlist_iter_t iter;
    #ifndef NDEBUG
    srtlist_t *list;
    #endif 
} srtlist_iter_t;   

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  allocated memory for a new sorted list			*/
/* Arguments:    cmp_func - compare function to be used in 		*/
/*				           sorting 								*/
/* Return value: returns aa pointer to the new sorted list      */
srtlist_t *SrtListCreate(cmp_func_t cmp_func);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  frees memory allocated for given sorted list   */
/* Arguments:    *list - pointer to the sorted list				*/
/* Return value: None											*/  
void SrtListDestroy(srtlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator to the first element		    */
/* Arguments:    *list - pointer to the sorted list				*/
/* Return value: returns iterator to the first element			*/  
srtlist_iter_t SrtListBegin(const srtlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator to end of the list		    */
/* Arguments:    *list - pointer to the sorted list				*/
/* Return value: returns iterator to the end of the list		*/  
srtlist_iter_t SrtListEnd(const srtlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator to the next element		    */
/* Arguments:    iter - iterator to a given element				*/
/* Return value: returns iterator to the next element			*/  
srtlist_iter_t SrtListNext(srtlist_iter_t iter);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  returns iterator to the previous element		*/
/* Arguments:    iter - iterator to an element   				*/
/* Return value: returns iterator to the previous element		*/  
srtlist_iter_t SrtListPrev(srtlist_iter_t iter);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  checks if two given iterators match		    */
/* Arguments:    iter1/iter2 - iterators to compare				*/
/* Return value: returns 1 if the iterators match, 0 otherwise	*/  
int SrtListIsIterSame(srtlist_iter_t iter1, srtlist_iter_t iter2);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  inserts an element to the list and 		    */
/*               automatically places it in the correct spot 	*/
/*				 to keep the list sorted						*/
/* Arguments:    *srtlist - pointer to the sorted list			*/
/*               data - data to be inserted into the list		*/
/* Return value: returns iterator to the inserted element		*/  
/* Note:         When insert fails, the return value is 		*/
/*               iterator to the end of list. If the data		*/     
/* 				 inserted is equal to another element in the 	*/
/* 				 list, it will be inserted after all the equal 	*/
/*				 elements										*/
srtlist_iter_t SrtListInsert(srtlist_t *srtlist, void* data);   

/****************************************************************/
/*	Complexity: O(1)     										*/
/* Description:  removes an element from the list 			    */
/* Arguments:    to_remove - iter to the element to be removed	*/
/* Return value: returns iterator to the next element			*/  
/* Note:         Inputting invalid iterator will result  in		*/
/*				 undefined behavior								*/       
srtlist_iter_t SrtListRemove(srtlist_iter_t to_remove);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  extracts data from element in the list		    */
/* Arguments:    iter - iterators to element					*/
/* Return value: returns a void pointer to the data 			*/
void *SrtListGetData(srtlist_iter_t iter);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  counts the number of elements in the list	    */
/* Arguments:    *list - pointer to the sorted list				*/
/* Return value: returns the number of elements in the list		*/ 
size_t SrtListCount(const srtlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  checks if the list is empty					*/
/* Arguments:    *list - pointer to the list 					*/
/* Return value: returns 1 if empty, 0 otherwise. 				*/
int SrtListIsEmpty(const srtlist_t *list);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  finds an element in a given list		 		*/
/* Arguments:    from - the beginning of the search range		*/
/* 				 to - the end of the search range which is 		*/
/*				 excluded from the search						*/
/*				 data - data that is sent as an argument 		*/
/*					     to a match func, on which the 			*/
/*						 comparison is based					*/
/*				 list - pointer to the list						*/
/* Return value: returns iterator that points to the first found*/
/*				 element, or end if no match is found 			*/
/* Note:		 inserting invalid from and to arguments will   */
/*			 	 lead to undefined behavior						*/		
srtlist_iter_t SrtListFind(srtlist_t *list, srtlist_iter_t from,
					srtlist_iter_t to,
					void *data);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  searches for an element in the list based on	*/
/*				 some criteria in the match function			*/
/* Arguments:    from - the beginning of the search range		*/
/* 				 to - the end of the search range which is 		*/
/*				 excluded from the search						*/
/*				 is_match - function that checks if element 	*/
/*							mathces some condition				*/
/*				 param - argument that goes into match function */
/* Return value: returns iterator that points to the found  	*/
/*				 element, or end if no match is found 			*/ 		
/* Note:		 inserting invalid from, to, or match_func 		*/
/*				 arguments will lead to undefined behavior	 	*/	
srtlist_iter_t SrtListFindIf(srtlist_iter_t from,              
					srtlist_iter_t to,
					match_func_t is_match,
					void *param);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  checks if the list is empty					*/
/* Arguments:    *list - pointer to the list 					*/
/* Return value: returns 1 if empty, 0 otherwise. 				*/
/* Note : 		 sending a callback function that changes the  	*/
/*        		 element values will ruin the sorting of the 	*/
/*				 list											*/ 
int SrtListForEach(srtlist_iter_t from,                     
				srtlist_iter_t to,
				action_func_t action,
				void *param);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  removes an element from the list				*/
/* Arguments:    *list - pointer to the list					*/
/* Return value: returns void pointer to the data in the 		*/
/*               removed element								*/
/* Note:		 using this function on an empty list will 		*/
/*				 result in undefined behavior					*/	
void *SrtListPopBack(srtlist_t *list);

/****************************************************************/
/*	Complexity: O(1)											*/
/* Description:  removes an element from the front of the list	*/
/* Arguments:    *list - pointer to the list					*/
/* Return value: returns void pointer to the data from the		*/
/*				 removed element								*/
/* Note:		 using this function on an empty list will 		*/
/*				 result in undefined behavior					*/	
void *SrtListPopFront(srtlist_t *list);

/****************************************************************/
/*	Complexity: O(n)											*/
/* Description:  Merges two sorted lists into one sorted list	*/
/* Arguments:    *dest - pointer to the destination list with	*/
/*				         which the source list will merge 		*/
/*				 *src - source list that will merge with dest	*/
/* Return value: None											*/
/* Note:         Make sure the two lists use the same compare 	*/
/*               function for their sorting algorithm. 			*/
/*				 Otherwise, it will result in undefined behavior*/
void SrtListMerge(srtlist_t *dest, srtlist_t *src);        

#endif /* SRTLIST_H */
