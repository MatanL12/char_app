#ifndef __CLIENTUI_H__
#define __CLIENTUI_H__


/**
 * @brief print the main screen:
 *          (1) Registration
 *          (2) Login
 *          (3) Exit
 * @param _actionFromUser a pointer to which the input from the user will be assigned
 */
void PrintMainScreen(char *_actionFromUser);


/**
 * @brief print the main screen:
 *          (1) Create Group
 *          (2) Join Group
 *          (3) Leave Group
 *          (4) Logout
 *
 * @param _actionFromUser
 */
void PrintLoginScreen(char *_actionFromUser);


/**
 * @brief print "Invalid input"
 *
 */
void PrintInvalidInput();


/**
 * @brief print "choose user name (without spaces)"
 * 
 * @param _userName a pointer to which the input from the user will be assigned to
 */
void AskForUserName(char *_userName);


/**
 * @brief print "choose password (without spaces)"
 * 
 * @param _password a pointer to which the input from the user will be assigned to
 */
void AskForPassword(char *_password);

/**
 * @brief print "Registartion failed"
 *
 */
void PrintResponseRegiFail();

/**
 * @brief print "Registartion success"
 *
 */
void PrintResponseRegiSucc();

/**
 * @brief print "Login success"
 *
 */
void PrintResponseLoginSucc();

/**
 * @brief print "Login failed"
 *
 */
void PrintResponseLoginFail();

/**
 * @brief print "Group creation failed"
 *
 */
void PrintCreateGroupFail();

/**
 * @brief print "Group creation success"
 *
 */
void PrintCreateGroupSuccess();

/**
 * @brief print "press group number for option"
 * 
 */
void PrintOptionManu();

/**
 * @brief print the group name
 * 
 * @param counter index of the group
 * @param group group name to be print
 */
void PrintIndexGroup(size_t counter, char *group);

/**
 * @brief print "no group exist"
 * 
 */
void PrintEmptyGroup();

/**
 * @brief scan the input received from the user
 * 
 * @param _option 
 */
void GetUserOptionJoinGroup(char *_option);

/**
 * @brief print "you already joined"
 * 
 */
void printErrorJoinGroup();

/**
 * @brief print "success join group"
 * 
 */
void JoinGroupSuccess();


/**
 * @brief print "Choose a group to leave"
 * 
 */
void PrintLeaveGroup();

/**
 * @brief print "You have left group - 'group name'"
 * 
 * @param _groupName the group's name
 */
void PrintLeaveSucess(char* _groupName);

/**
 * @brief print "You haven't joined any group yet"
 * 
 */
void PrintNoGroups();

/**
 * @brief print "Logout success"
 * 
 */
void PrintLogoutSuccess();


/**
 * @brief print "Good bye!"
 * 
 */
void PrintExit();


/**
 * @brief print "Action failed - send/recv from server!"
 * 
 */
void PrintFailServer();


/**
 * @brief print "Invalid input length"
 * 
 */
void PrintInvalidLength();


/**
 * @brief 
 * 
 */

#endif /* __CLIENTUI_H__ */