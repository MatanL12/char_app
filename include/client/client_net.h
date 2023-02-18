#ifndef __CLIENT_NET_H__
#define __CLIENT_NET_H__

#include "data_net.h" /* PORT, IP */
#include <stddef.h>              /* size_t */

typedef enum ClientNetERR
{
    CLIENT_NET_SUCCESS,
    CLIENT_NET_NOT_INITIALIZED,
    CLIENT_NET_ID_ERR,
    CLIENT_NET_CONNECT_ERR,
    CLIENT_NET_SEND_ERR,
    CLIENT_NET_RECV_ERR
} ClientNetERR;

/**
 * @brief create a new socket and connect to the the server net
 *       (Protocol - TCP, IP and PORT as define in "data_net.h" )
 *
 * @param _socket a pointer to client ID to which the socket number will be assigned.
 * @return CLIENT_NET_SUCCESS in case of successful socket creation and connection
 * @return CLIENT_NET_NOT_INITIALIZED in case that _socket is NULL
 * @return CLIENT_NET_CONNECT_ERR in case that socket or connect failed
 */
ClientNetERR ClientInit(int *_socket, int _port, char *_ip);

/**
 * @brief close the socket passed as parameter
 *
 * @param _socket socket to close
 * @return CLIENT_NET_SUCCESS in case that the socket closed successfuly
 * @return CLIENT_NET_ID_ERR in case that _socket is lower than MIN_SOCK_NUM or greater then MAX_PORT_NUM
 */
ClientNetERR ClientClose(int _socket);

/**
 * @brief send buffer to the server network and receive the response (TCP)
 *
 * @param _socket
 * @param _bufferSend a buffer to send
 * @param _bufferSize size of buffer to send
 * @param _bufferReceive parameter to which the received buffer will be assigned
 * @return CLIENT_NET_SUCCESS in case of successful send and receive
 * @return CLIENT_NET_ID_ERR in case that _socket is lower than MIN_SOCK_NUM or greater then MAX_PORT_NUM
 * @return CLIENT_NET_SEND_ERR in case that send failed
 * @return CLIENT_NET_RECV_ERR in case that receive failed
 */


/**
 * @brief send buffer to the server network (TCP)
 * 
 * @param _socket the socket id
 * @param _bufferSend buffer to send to the server
 * @param _bufferSize the size of the buffer to send to the server
 * @return CLIENT_NET_SUCCESS on success
 * @retval CLIENT_NET_NOT_INITIALIZED in case that _bufferSend is NULL
 * @retval CLIENT_NET_ID_ERR in case that the socket is less than 3 or greater than 1023
 * @retval CLIENT_NET_SEND_ERR in case that the sent to ther server failed
 */
ClientNetERR ClientSend(int _socket, void *_bufferSend, size_t _bufferSize);


/**
 * @brief receive response from the server (TCP)
 * 
 * @param _socket the socket id
 * @param _bufferReceive a buffer to which the response from the server will be assigned to
 * @return CLIENT_NET_SUCCESS on success
 * @retval CLIENT_NET_NOT_INITIALIZED in case that _bufferReceive is NULL
 * @retval CLIENT_NET_ID_ERR in case that the socket is less than 3 or greater than 1023
 * @retval CLIENT_NET_RECV_ERR in case that the receive from the server failed 
 */
ClientNetERR ClientRecv(int _socket, void *_bufferReceive);

#endif /* __CLIENT_NET_H__ */
