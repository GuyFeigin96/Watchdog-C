#ifndef __ILRD_WD_1556__
#define __ILRD_WD_1556__

typedef enum wd_status
{
    WD_SUCCESS = 0,
    WD_FAILURE
} wd_status_t;

/*
compile with:
gd -pthread watchdog.c watchdog_client.c scheduler.c pqueue.c srtlist.c dlist.c task.c uid.c -I../inc -lm -o watchdog.out
mv watchdog.out ../mk/universal/
make TARGET=wd_client LF=-pthread
*/

/*
Description:
    -Defends a critical section: if this section crashes, the program is revived
Params:
    -cmd: command line to reinitiate the process {"./a.out", "arguments"...}
Return:
    -status:
        -SUCCESS: section is protected
        -FAILURE: section isn't protected
Notes:
    -this utility uses SIGUSR1 SIGUSR2 signals
*/
wd_status_t WDStart(const char **cmd);

/*
Description:
    -Ends the critical section
Notes:
    -Function does not guarentee to free used resources immediately at function exit
*/
void WDStop(void);

#endif /* __ILRD_WD_1556__ */