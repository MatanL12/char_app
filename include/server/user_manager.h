#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__

#define TRUE 1
#define FALSE 0

typedef struct UserManager UserManager;

typedef enum
{
    SUCCESS_USER_MANAGER,
    ERR_USER_EXIST,
    ERR_MANAGER_UNINITIALIZED,
    ERR_ADD_USER_FAIL,
    BACKUP_FAIL,
    INVALID_NAME_OR_PASS,
    USER_MANAGER_FILE_FAIL,
    USER_NOT_EXIST,
    ERR_MANAGER_GROUP_UNINITIALIZED,
    GROUP_NOT_EXIST,
    ERR_USER_LOGGED

} statusUserManager;

/**
 * @brief Create a new user manager.
 * @return pointer to user manager in case of success or null in case of failure.
 */
UserManager *CreateUserMan();
statusUserManager AddNewUser(UserManager *_userManager, char *_name, char *_password);
void DestroyUserMan(UserManager **_userManager);
statusUserManager LoginUser(UserManager *_userManager, char *_name, char *_password);

/**
 * @brief insert a new group to user list
 * @param UserManager *_userManager - pointer to user manager
 * @param _username char *_username username
 * @param _username char **groupName username - pointer to group name.
 * @return status
 */
statusUserManager InserGroupToUser(UserManager *_userManager, char *_username, char *_groupName);

/**
 * @brief manage user leave group.
 * @param UserManager *_userManager - pointer to user manager
 * @param _username char *_username username
 * @param _username char **groupName username - pointer to group name.
 * @return status success if success or failure.
 */
statusUserManager LeaveGroupUserManagment(UserManager *_userManager, char *_userName,
                                          char *_groupName);

/**
 * @brief check wether user already joined to a group.
 * @param UserManager *_userManager - pointer to user manager
 * @param _username char *_username username
 * @param _groupName group name
 * @return yes or not.
 */
int HasJoinedManager(UserManager *_userManager, char *m_userName, char *_groupName);

#endif /* __USERMANAGER__ */