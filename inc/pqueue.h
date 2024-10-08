/*
	Name: Guy Feigin
	Exercise: Priority queue
	File type: Header
	Reviewer: Yuval 
	Last updated: Sat 02 Mar 2024 13:10:56  
*/

#ifndef P_Q
#define P_Q

#include <stddef.h> /* size_t */

/******************************************************************************/
/* Description:  This function is used to define how priority is set in 	  */
/*				 the queue. To be defined by the client according to 		  */
/*				 individial needs.			                                  */
/* Arguments:    data - data inside the queue to be compared against	param */
/*				 param - to be provided by the user to be compared against 	  */
/*						 data												  */
/* Return value: returns an integer based on how the user compares the data	  */
/*				 and parameter.												  */ 
typedef int (*cmp_func_t)(const void *data, const void *param);

/******************************************************************************/
/* This helper function is used in the PQErase function to find the element   */
/* that the user would like to erase. To be defined by the user according	  */
/* to individial needs.														  */
typedef int (*match_func_t)(const void *data, void *param);

/******************************************************************************/
/* type definition for the priority queue									  */
typedef struct pq pq_t;

/******************************************************************************/
/* Description:  Creates an empty priority list								  */
/* Arguments:    receives a compare function that's used to define priority	  */
/* Return value: returns a pointer to the newly created priority queue		  */
pq_t *PQCreate(cmp_func_t cmp_func); /* O(1) */

/******************************************************************************/
/* Description:  Frees memory of a given priority queue 					  */
/* Arguments: 	 receives a pointer to the priority queue to be freed		  */
/* Return value: None														  */
void PQDestroy(pq_t *pq); /* O(n) */

/******************************************************************************/
/* Description:  Adds an element to the priority list and 					  */
/*				 places it based on the user-defined priority				  */
/* Arguments: 	 receives a pointer to an existing priority queue and a 	  */
/*				 pointer to the data to be inserted into the queue			  */
/* Return value: returns 0 if successful, 1 otherwise						  */
int PQEnqueue(pq_t *pq, void *data); /* O(n) */

/******************************************************************************/
/* Description:  Removes an element from the queue with the highest priority  */
/* Arguments: 	 receives a priority queue									  */
/* Return value: returns a void pointer to the data removed					  */
/* Note:         Attempting to remove from an empty list will result in 	  */
/*				 undefined behavior 										  */
void *PQDequeue(pq_t *pq); /* O(1) */

/******************************************************************************/
/* Description:  Returns the element with the highest priority in the queue	  */
/* Arguments: 	 receives a priority queue									  */
/* Return value: returns a pointer to the element with the highest priority	  */
/* Note		     Using Peek on an empty list will result in undefined behavior*/
void *PQPeek(const pq_t *pq); /* O(1) */

/******************************************************************************/
/* Description:  Checks if the queue is empty								  */
/* Arguments: 	 receives a pointer to a priority queue						  */
/* Return value: returns 1 if empty, 0 otherwise							  */
int PQIsEmpty(const pq_t *pq); /* O(1) */

/******************************************************************************/
/* Description:  Counts the number of elements in the queue					  */
/* Arguments: 	 receives a pointer to a priority queue						  */
/* Return value: returns the number of elements in the queue as size_t		  */
size_t PQCount(const pq_t *pq); /* O(n) */

/******************************************************************************/
/* Description:  Removes a given element from the queue. The user must define */
/*               a match function to be able to identify the element to be 	  */
/*				 removed from the queue										  */
/* Arguments: 	 receives a pointer to the priority queue, a pointer to a     */
/*				 match function, and a pointer to the parameter to be removed */
/* Return value: returns a void pointer to the erased element				  */
void *PQErase(pq_t *pq, match_func_t match_func, void *param); /* O(n) */

/******************************************************************************/
/* Description:  Clears the entire priority queue from elements without 	  */
/*				 destroying it												  */
/* Arguments: 	 receives a pointer to a priority queue						  */
/* Return value: returns a pointer to the cleared priority queue			  */
void PQClear(pq_t *pq); /* O(n) */

#endif /* P_Q */


