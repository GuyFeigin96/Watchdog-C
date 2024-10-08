/*
	Name: Guy Feigin
	Exercise: UID
	File type: Source code
	Reviewer: Alon
	Last updated: Monday 15 April 2024 (Phase 2) 
*/

#include <unistd.h> /* getpid() */
#include <pthread.h> /* mutex_t */

#include "uid.h" /* ilrd_uid_t */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

const ilrd_uid_t bad_uid = {0, -1, -1};

ilrd_uid_t UIDGenerate(void)
{
	ilrd_uid_t uid;
	static size_t count = 1;

	if (0 != pthread_mutex_lock(&mutex))
	{
		return (bad_uid);
	} 

	uid.counter = count++;

	if (0 != pthread_mutex_unlock(&mutex))
	{
		return (bad_uid);
	}

	uid.pid = getpid();
	uid.time = time(NULL);
	
	if (-1 == uid.time || -1 == uid.pid)
	{
		return (bad_uid); 
	}

	return (uid);
}

int UIDIsEqual(ilrd_uid_t one, ilrd_uid_t other)
{
	
	return (one.counter == other.counter && 
			one.pid == other.pid && 
			one.time == other.time);
}
