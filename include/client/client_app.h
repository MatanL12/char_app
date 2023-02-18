#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__

#include "data_net.h"

typedef enum ClientAppERR
{
    CLIENT_APP_SUCCESS,
    CLIENT_APP_NOT_INITIALIZED,
    CLIENT_APP_CONNECTION_FAILED,
    CLIENT_APP_SEND_FAILED,
    CLIENT_APP_RECV_FAILED,
    CLIENT_APP_RENDER_FAIL,
    CLIENT_APP_JOIN_FAIL,
    CLIENT_APP_LEAVE_FAIL
} ClientAppERR;


typedef struct ClientApp ClientApp;

/**
 * @brief Create a Client App object and connect to the client network
 *
 * @param _port a port number
 * @return a pointer to a ClientApp object on success
 * @retval return NULL in case of:
 *         (1) the dynamic allocation failed
 *         (2) the port number is out ranged (minimum - 1027, maximum 65529)
 */
ClientApp *CreateClientApp(int _port);

/**
 * @brief run the Client App (chat group)
 *
 * @param _clientApp
 * @return CLIENT_APP_SUCCESS on success
 * @retval CLIENT_APP_NOT_INITIALIZED in case that _clientApp is NULL
 */
ClientAppERR RunClientApp(ClientApp *_clientApp);


/**
 * @brief destroy the Client APP and close the connection to client network
 *
 * @param _clientApp a pointer to the ClientApp object to be destroyed.
 */
void DestroyClientApp(ClientApp *_clientApp);

#endif /* __CLIENT_APP_H__ */