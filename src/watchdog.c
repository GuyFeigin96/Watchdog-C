#include "watchdog_client.h"
#include <stdlib.h>
#include <unistd.h> 

int main(int argc, char *argv[])
{
    const char *cmd[3] = {0};
    (void)argc;
    
    cmd[0] = "./watchdog";
    cmd[1] = (char *)argv;
    cmd[2] = 0;

    WDStart(cmd);

    return 0;
}
