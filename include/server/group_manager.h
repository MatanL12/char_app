#ifndef __GROUP_MANAGER_H__
#define __GROUP_MANAGER_H__

#include "g_vector.h"


typedef struct GroupManager GroupManager;


typedef enum GroupManagerErr
{
    GROUP_MANAGER_SUCCESS,
    GROUP_MANAGER_NOT_INITIALIZED,
    GROUP_MANAGER_ALLOCATION_ERR,
    GROUP_MANAGER_OVERFLOW,
    GROUP_MANAGER_UNAVAILBLE_NAME
}GroupManagerErr;

/**
 * @brief Create a new Group Manager object
 * 
 * @return GroupManager* if the Group Manger created succesfully
 * @retval NULL in case of dynamic allocaion failed
 */
GroupManager* CreateGroupManager();

/**
 * @brief destroy a Group Manager object
 * 
 * @param _groupManager a Group Manager object to be destroyed
 */
void DestroyGroupManager(GroupManager* _groupManager);


/**
 * @brief create a new group, each group should has a unique name
 * 
 * @param _groupManager a pointer to a Group Manager object
 * @param _name the name of the new group, must be uni
 * @param _ip a buffer to which the group's ip will be assigned to
 * @param _port a buffer to which the group's port will be assigned to
 * @param _nameBuffer a pointer to a buffer to which the pointer to the group's name will be assigned to
 * @return GROUP_MANAGER_SUCCESS in case that the group was added successfuly
 * @retval GROUP_MANAGER_NOT_INITIALIZED in case that _groupManager or _name is NULL
 * @retval GROUP_MANAGER_UNAVAILBLE_NAME in case that the name is already exist
 * @retval GROUP_MANAGER_ALLOCATION_ERR in case that the dynamic allocaion failed
 */
GroupManagerErr AddGroup(GroupManager* _groupManager, char* _name, char* _ip, char* _port, char **_nameBuffer);


/**
 * @brief add a new user to a group - i.e. increase the number of users in a group by 1 and extract the ip and port
 * 
 * @param _groupManager a pointer to a Group Manager object
 * @param _name the name of the group
 * @param _ip a buffer to which the group's ip will be assigned to
 * @param _port a buffer to which the group's port will be assigned to
 * @param _nameBuffer a pointer to a buffer to which the pointer to the group's name will be assigned to
 * @return GROUP_MANAGER_SUCCESS in case that the user joined the group successfully 
 * @retval GROUP_MANAGER_NOT_INITIALIZED in case that _groupManager or _name or _ip or _port or _nameBuffer is NULL
 * @retval GROUP_MANAGER_UNAVAILBLE_NAME in case that the group does not exist
 */
GroupManagerErr JoinGroup(GroupManager* _groupManager, char* _name, char* _ip, char* _port, char **_nameBuffer);

/**
 * @brief decrease the number of users in a group by 1.  
 *        Destroy the group if the number of users after the decrement is 0.
 * 
 * @param _groupManager a pointer to a Group Manager object
 * @param _name the name of the group
 * @return GROUP_MANAGER_SUCCESS on success
 * @retval GROUP_MANAGER_NOT_INITIALIZED in case that the _groupManager or _name is NULL
 * @retval GROUP_MANAGER_UNAVAILBLE_NAME in case that there is no group with such name
 * @retval GROUP_MANAGER_ALLOCATION_ERR in case that the dynamic allocation failed
 */
GroupManagerErr LeaveGroup(GroupManager* _groupManager, char* _name);


/**
 * @brief Get the Group Names object
 * 
 * @param _groupManager a pointer to a Group Manager object
 * @param _vector a vector to which all the group names currently active will be assigned to (by referance)
 * @return GROUP_MANAGER_SUCCESS if all the group names were appended to the vector successfully
 * @retval GROUP_MANAGER_NOT_INITIALIZED if _groupManager or _vector is NULL
 */
GroupManagerErr GetGroupNames(GroupManager *_groupManager, Vector* _vector);



/**
 * @brief Get the number of groups currently in the GroupManager object
 * 
 * @param _groupManager a pointer to a Group Manager object
 * @return return the number of groups
 * @retval return 0 in case that _groupManager is NULL
 */
size_t GetNumOfGroups(GroupManager* _groupManager);

#endif /* __GROUP_MANAGER_H__ */
