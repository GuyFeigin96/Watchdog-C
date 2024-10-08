#ifndef ILRD_UID_H
#define ILRD_UID_H

#include <stddef.h> /*size_t*/
#include <sys/types.h> /*pid_t*/
#include <time.h> /*time_t*/

/******************************************************************************/
/* Each unique id will have the following structure to help identify it and   */
/* make it as truly unique as possible. Counter is some number that 		  */
/* increments each time a new uid is generated. pid is the process id of the  */
/* process that generated the uid. time is the time at which the uid was 	  */
/* generated. 																  */
typedef struct ilrd_uid
{
    size_t counter;
    pid_t pid;
    time_t time;
} ilrd_uid_t;

/******************************************************************************/
/* This uid is used to signal if a uid failed to generate correctly, for 	  */
/* example if the time field did not generate correctly. It is defined in the */
/* source code of the library and cannot be altered.						  */
extern const ilrd_uid_t bad_uid;

/******************************************************************************/
/* Description:  generates an instance of a uid struct uniquely identified by */
/*               its members. 												  */
/* Arguments:    None														  */
/* Return value: returns an instance of a uid struct                          */
/* Note: 		 if the uid generation fails it will return a bad_uid instance*/
ilrd_uid_t UIDGenerate(void);

/******************************************************************************/
/* Description:  checks if two uid structs are identical					  */
/* Arguments:    receives two uid structs to be compared 					  */
/* Return value: returns 1 if the uid structs are identical, 0 otherwise.     */
int UIDIsEqual(ilrd_uid_t one, ilrd_uid_t other);

#endif /*ILRD_UID_H*/
