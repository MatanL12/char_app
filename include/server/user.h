#ifndef __USER_H__
#define __USER_H__

typedef struct User User;

typedef struct NetworkData NetworkData;


typedef enum UserERR
{
    USER_SUCCESS,
    USER_NOT_INITIALIZED,
    ADD_GROUP_FAIL,
    USER_GROUP_NOT_EXIST,
    ERR_USER_OFFLINE
} UserERR;

/**
 * @brief Create a User
 *
 * @param _userName user name
 * @param _password password
 * @return User* a pointer to the user that was created
 * @return return NULL in case of:
 *         (1) _userName or _password is NULL;
 *         (2) dynamic allocation failed
 */
User *CreateUser(char _userName[], char _password[]);

/**
 * @brief Destroy a user
 *
 * @param _user
 */
void DestroyUser(User *_user);
/**
 * @brief Get the User Name object
 *
 * @param _user
 * @return char*
 */
char *GetUserName(User *_user);
char *GetPassword(User *_user);
void SetToLog(User *_user);

/**
 * @brief Create a User
 *
 * @param User *_user pointer to user.
 * @param char **_groupName - group name
 * @return USER_SUCCESS in case of success
 * @return fail if not
 */
UserERR UpdateUserGroupList(User *_user, char *_groupName);

/**
 * @brief delete group from user.
 *
 * @param User *_user pointer to user.
 * @param char **_groupName - group name
 * @return USER_SUCCESS in case of success
 * @return fail if not
 */
UserERR UserLeaveGroup(User *_user, char *_groupName);

#endif /* __USER_H__ */
