/*
    << Watchdog Project >>
    File: Source code
    Name: Guy Feigin
    Date: April 18 2024
    Reviewer: Hila Cohen 
*/
#define _POSIX_C_SOURCE (200112L)

#include <pthread.h> /* pthread_create() */
#include <unistd.h> /* fork() */
#include <stdio.h> /* printf() */
#include <string.h> /* strcmp */
#include <signal.h> /* sigaction */
#include <stdlib.h> /* getenv() */
#include <stdatomic.h> /* atomic_int */
#include <semaphore.h> /* semopen() */
#include <fcntl.h> 

#include "scheduler.h" /* schedcreate() */  

#ifndef DNDEBUG
    #define DEBUG_EXPR(x) (x) 
#else
    #define DEBUG_EXPR(x) 
#endif

#define FAIL_FACTOR (5)

atomic_int alive_counter = 0;
atomic_int stop_flag = 0;
 
pid_t other_pid = 0;
pthread_t scheduler_thread = 0;
const char *curr_proccess = NULL;
scheduler_t *sched = NULL;
sem_t *sem_user = NULL;
sem_t *sem_wd = NULL;

typedef enum wd_status
{
    WD_SUCCESS = 0,
    WD_FAILURE
} wd_status_t;

/* Init functions */
static scheduler_t *InitSched(const char **cmd);
static void InitSignalHandlers();
static wd_status_t InitSem();

/* Helper functions */
static void *RunSched(void *arg);
static void SyncSchedulers();
static wd_status_t Fork(const char *to_exec, const char **cmd);
static wd_status_t CreateThread();
static wd_status_t Revive(const char **cmd);
static void DestroySem();
static wd_status_t SetEnv();

/* Tasks */
static int SendSig1(void *param);
static int CheckCounter(void *param);
static int SendSig2(void *param);

/* Signal handlers */
static void AliveSignalHandler(int sig, siginfo_t *info, void *uncontext);
static void StopHandler(int sig, siginfo_t *info, void *uncontext);

wd_status_t WDStart(const char **cmd)
{
    wd_status_t status = 0;

    curr_proccess = cmd[0];

    InitSem();

    InitSignalHandlers();

    sched = InitSched(cmd);
    if (NULL == sched)
    {
        return (status);
    }

    if (0 == strcmp(*cmd, "./watchdog"))
    {
        DEBUG_EXPR(printf("Inside WD process | pid: %d\n", getpid()));

        other_pid = getppid();

        status = SetEnv();
        if (WD_FAILURE == status)
        {
            return (status);
        }
        
        RunSched(sched);
    }
    else
    {
        if (!getenv("WD_PID"))
        {
            status = Fork("./watchdog", cmd);
            if (WD_FAILURE == status)
            {
                return (status);
            }
        }
        else
        {
            other_pid = atoi(getenv("WD_PID"));
        }

        DEBUG_EXPR(printf("Inside thread\n"));

        status = CreateThread();
        if (WD_FAILURE == status)
        {
            return (status);
        }
    }
 
    return (WD_SUCCESS);
}

void WDStop(void)
{
    kill(other_pid, SIGUSR2);

    sem_wait(sem_user);
    SchedStop(sched);
    unsetenv("WD_PID");
    DestroySem();

    pthread_join(scheduler_thread, NULL);
}

/******************************************************************************/
/****************************** Init Functions ******************************/
/******************************************************************************/

static scheduler_t *InitSched(const char **cmd)
{
    ilrd_uid_t uid;

    sched = SchedCreate(); 
    if (NULL == sched)
    {
        DEBUG_EXPR(printf("SchedCreate failed\n"));
        return (NULL);
    }

    uid = SchedAddTask(sched, 1, SendSig1, NULL, NULL, NULL);
    if (UIDIsEqual(uid, bad_uid))
    {
        DEBUG_EXPR(printf("SchedAddTask 1 failed\n"));
        SchedDestroy(sched);
        return (NULL);
    }
    uid = SchedAddTask(sched, 1, CheckCounter, cmd, NULL, NULL);
    if (UIDIsEqual(uid, bad_uid))
    {
        DEBUG_EXPR(printf("SchedAddTask 2 failed\n"));
        SchedDestroy(sched);
        return (NULL);
    }

    uid = SchedAddTask(sched, 2, SendSig2, sched, NULL, NULL);
    if (UIDIsEqual(uid, bad_uid))
    {
        DEBUG_EXPR(printf("SchedAddTask 3 failed\n"));
        SchedDestroy(sched);
        return (NULL);
    }  

    return (sched); 
}

static wd_status_t InitSem()
{
    sem_user = sem_open("/user_sem", O_CREAT, 0666, 0);
    if (SEM_FAILED == sem_user)
    {
        DEBUG_EXPR(printf("user sem_open failed\n"));
        return (WD_FAILURE);
    }

    sem_wd = sem_open("/wd_sem", O_CREAT, 0666, 0);
    if (SEM_FAILED == sem_wd)
    {
        DEBUG_EXPR(printf("wd sem_open failed\n"));
        return (WD_FAILURE);
    }

    return (WD_SUCCESS);
}

static void InitSignalHandlers()
{
    struct sigaction alive = {0};
    struct sigaction stop = {0};

    alive.sa_sigaction = AliveSignalHandler;
    alive.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &alive, NULL);

    stop.sa_sigaction = StopHandler;
    stop.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &stop, NULL);
}

static void *RunSched(void *arg)
{
    (void)arg;

    SyncSchedulers();

    SchedRun(sched);
    
    SchedDestroy(sched);

    return (NULL);
}

/******************************************************************************/
/************************* Helper functions ***********************************/
/******************************************************************************/

static wd_status_t Fork(const char *to_exec, const char **cmd)
{
    int status = 0;

    pid_t child_pid = fork();

    if (-1 == child_pid)
    {
        DEBUG_EXPR(printf("fork failed\n"));
        return (WD_FAILURE);
    }

    if (0 == child_pid)
    {
        status = execvp(to_exec, (char * const *)cmd);
        if (WD_SUCCESS != status)
        {
            DEBUG_EXPR(printf("execlp failed\n"));
            return (WD_FAILURE);
        }
    }
    
    other_pid = child_pid;

    return (WD_SUCCESS);
}

static wd_status_t CreateThread()
{
    wd_status_t status = WD_SUCCESS;

    status = pthread_create(&scheduler_thread, NULL, RunSched, NULL);
    if (0 != status)
    {
        DEBUG_EXPR(printf("pthread_create failed\n"));
        return (WD_FAILURE);
    }

    return (WD_SUCCESS);
}

static wd_status_t Revive(const char **cmd)
{
    pid_t pid = 0;
    
    pid = fork();
    if (-1 == pid)
    {
        return (WD_FAILURE);
    }

    if (0 == pid)
    {
        if (0 == strcmp(*cmd, "./watchdog"))
        {
            DEBUG_EXPR(printf("%s\n", ((char **)cmd[1])[0]));

            execvp(((char **)cmd[1])[0], (char **)cmd[1]);

            DEBUG_EXPR(printf("execvp failed\n"));
            return (WD_FAILURE);
        }

        execvp("./watchdog", (char **)cmd[1]);
        DEBUG_EXPR(printf("execvp failed\n"));
        return (WD_FAILURE);
    }

    other_pid = pid;

    SyncSchedulers();

    return (WD_SUCCESS);
}

static void DestroySem()
{
    int status = 0;
    
    status = sem_unlink("/user_sem");
    if (0 != status)
    {
        DEBUG_EXPR(printf("unlink user_sem failed\n"));
    }

    status = sem_unlink("/wd_sem");
    if (0 != status)
    {
        DEBUG_EXPR(printf("unlink wd_sem failed\n"));
    }

    status = sem_close(sem_user);
    if (0 != status)
    {
        DEBUG_EXPR(printf("close user_sem failed\n"));
    }

    status = sem_close(sem_wd);
    if (0 != status)
    {
        DEBUG_EXPR(printf("close wd_sem failed\n"));
    }
}

static wd_status_t SetEnv()
{
    pid_t pid = 0;
    char pid_str[20] = {0};

    pid = getpid();

    snprintf(pid_str, sizeof(pid_str), "%d", pid);

    if (0 != setenv("WD_PID", pid_str, 1)) 
    {
        DEBUG_EXPR(printf("Error setting environment variable\n"));
        return (WD_FAILURE);
    }

    return (WD_SUCCESS);
}

static void SyncSchedulers()
{
    if (0 == strcmp(curr_proccess, "./watchdog"))
    {
        sem_post(sem_user);
        sem_wait(sem_wd);
    }
    else
    {
        sem_post(sem_wd);
        sem_wait(sem_user);
    }
}

/******************************************************************************/
/********************************** Tasks *************************************/
/******************************************************************************/

static int SendSig1(void *param)
{
    (void)param;
    atomic_fetch_add(&alive_counter, 1);
    DEBUG_EXPR(printf("Task1 | Send SIG1 from %d\n", getpid()));
    kill(other_pid, SIGUSR1);

    return (REPEAT);
}

static int CheckCounter(void *param)
{
    DEBUG_EXPR(printf("Task2 | PID: %d | Counter: %d\n", getpid(), alive_counter));
    if (alive_counter > FAIL_FACTOR)
    {
        DEBUG_EXPR(printf("Other process died\n"));
        atomic_exchange(&alive_counter, 0);
        if (WD_FAILURE == Revive(param))
        {
            return (ERROR);
        }
    }

    return (REPEAT);
}

static int SendSig2(void *param)
{
    (void)param;

    DEBUG_EXPR(printf("PID: %d | Task3\n", getpid()));

    if (1 == stop_flag)
    {
        DEBUG_EXPR(printf("SIG2 received from pid: %d\n", other_pid));
        sem_post(sem_user);
        SchedStop(sched);

        return(STOP); 
    }

    return (REPEAT);
}

/******************************************************************************/
/****************************** Signal Handlers *******************************/
/******************************************************************************/

static void AliveSignalHandler(int sig, siginfo_t *info, void *uncontext)
{
    (void)sig;
    (void)uncontext;

    DEBUG_EXPR(printf("SIGUSR1 received in pid: %d from: %d\n", getpid(), info->si_pid));

    if (other_pid == info->si_pid)
    {
        atomic_exchange(&alive_counter, 0);
    }
}

static void StopHandler(int sig, siginfo_t *info, void *uncontext)
{
    (void)sig;
    (void)uncontext;
    DEBUG_EXPR(printf("SIGUSR2 received in pid: %d from: %d\n", getpid(), info->si_pid));

    if (other_pid == info->si_pid)
    {
        atomic_exchange(&stop_flag, 1);
    }
}

