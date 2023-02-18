#ifndef __GROUP_H__
#define __GROUP_H__

#include <stddef.h> /* size_t */


typedef struct Group Group;

typedef enum GroupErr
{
    GROUP_SUCCESS,
    GROUP_NOT_INITIALIZED,
    GROUP_EMPTY
}GroupErr;

/**
 * @brief Create a new Group object
 * 
 * @param _name the group's name must be dynamically allocated (saved by referance)
 * @param _ip the group's ip 
 * @param _port the group's port
 * @return a pointer to a Group object on success
 * @retval return NULL in case that 
 *         (1)_ip or _name or _port is NULL
 *         (2) the dynamic allocation failed
 */
Group* CreateGroup(char* _ip, char* _name, char* _port);


/**
 * @brief destroy a Group object
 * 
 * @param _group a Group object to be destroyed
 */
void DestroyGroup(Group* _group);


/**
 * @brief increase the number of users currently active by 1
 * 
 * @param _group a pointer to a Group object
 * @return GROUP_SUCCESS on success
 * @retval GROUP_NOT_INITIALIZED in case that _group is NULL
 */
GroupErr IncreaseUsersCounter(Group* _group);


/**
 * @brief decrease the number of users currently active by 1
 * 
 * @param _group a pointer to a Group object
 * @return GROUP_SUCCESS on success
 * @retval GROUP_NOT_INITIALIZED in case that _group is NULL
 * @retval GROUP_EMPTY in case that the number of users after the decrement is 0
 */
GroupErr DecreaseUsersCounter(Group* _group);


/**
 * @brief Get the Group's ip (by value)
 * 
 * @param _group a pointer to a Group object
 * @param _ip a buffer to which the group's ip will be assigned to (by value)
 * @return GROUP_SUCCESS on success
 * @retval GROUP_NOT_INITIALIZED in case that _group or _ip is NULL
 */
GroupErr GetGroupIp(Group *_group, char *_ip);


/**
 * @brief Get the Group's port (by value)
 * 
 * @param _group a pointer to a Group object
 * @param _port a buffer to which the group's port will be assigned to (by value)
 * @return GROUP_SUCCESS on success
 * @retval GROUP_NOT_INITIALIZED in case that _group or _port is NULL
 */
GroupErr GetGroupPort(Group* _group, char* _port);


/**
 * @brief Get the Group's name (by referance)
 * 
 * @param _group a pointer to a Group object
 * @param _name a buffer to which the group's name will be assigned to (by referance)
 * @return GROUP_SUCCESS on success
 * @retval GROUP_NOT_INITIALIZED in case that _group or _name is NULL
 */
GroupErr GetGroupName(Group* _group, char** _name);


#endif /* __GROUP_H__ */
