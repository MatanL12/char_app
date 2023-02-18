#include "client_app.h"
#include "data_net.h"
#include <stdlib.h> /* NULL */

#define FAIL -1
#define OK 0

int main(void)
{

    ClientApp* ptrClientApp;

    if( (ptrClientApp = CreateClientApp(PORT)) == NULL)
    {
        return FAIL;
    }

    if(RunClientApp(ptrClientApp) != CLIENT_APP_SUCCESS)
    {
        return FAIL;
    }

    DestroyClientApp(ptrClientApp);

    return OK;
}