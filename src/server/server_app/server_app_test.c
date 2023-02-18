
#include <stdio.h>
#include "server_app.h"

#define FAIL -1
#define OK 0

int main()
{
    ServerAppManager *server = CreateAppManager();

    if( (server = CreateAppManager()) == NULL)
    {
        return FAIL;
    }

    if(ServerManagerRun(server) != SERVER_APP_SUCCESS)
    {
        return FAIL;
    }

    DestroyAppManager(server);

    return OK;
}