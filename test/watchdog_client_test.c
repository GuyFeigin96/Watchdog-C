#include <stdio.h> /* printf() */
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h>

#include "watchdog_client.h"
 

int main(int argc, const char *argv[])
{
    const char *cmd[3] = {0};
    time_t start_time = time(NULL);

    (void)argc;
    cmd[0] = "./watchdog_client.out";
    cmd[1] = (char *)argv;
    cmd[2] = 0; 

    printf("user pid: %d\n\n", getpid());

    WDStart(cmd);
    printf("Critical code section\n");

    while(start_time + 20 > time(NULL))
    {
        sleep(1);
    }

    WDStop();

    return 0;
}
