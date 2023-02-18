#ifndef __SERVER_APP_H__
#define __SERVER_APP_H__

typedef struct ServerAppManager ServerAppManager;

typedef enum ServerAppErr
{
    SERVER_APP_SUCCESS,
    SERVER_APP_UNINITIALIZED,
    SERVER_APP_ALLOCATION_FAILED,
    SERVER_APP_SERVER_RUN_FAILED
}ServerAppErr;

/**
 * @brief Create an AppManager object
 * 
 * @param _port a port in which the server will run
 * @return a pointer to a ServerAppManager object on sucess
 * @retval NULL in case that:
 *         (1) the port number is out of range (minimum - 1027, maximum 65529)
 *         (2) dynamic allocation failed
 */
ServerAppManager *CreateAppManager(int _port);


/**
 * @brief destroy a ServerAppManager object
 * 
 * @param _appManager a ServerAppManager object to be destroyed
 */
void DestroyAppManager(ServerAppManager **_appManager);


/**
 * @brief run the server
 * 
 * @param _appManager a pointer to a ServerAppManager object
 * @return SERVER_APP_SUCCESS on success
 * @retval SERVER_APP_UNINITIALIZED in case that _appManager is NULL
 * @retval SERVER_APP_SERVER_RUN_FAILED if the server run failed
 */
ServerAppErr ServerManagerRun(ServerAppManager *_appManager);


#endif /* __SERVER_APP_H__ */