#include <stdio.h> /* printf, scanf */
#include "clientUI.h"
#include "limitations.h"

#define TERMINAL_ECHO_OFF system("stty -icanon -echo")
#define TERMINAL_ECHO_ON system("stty icanon echo")
#define ESC ('27')

void PrintMainScreen(char *_actionFromUser)
{
    /* TERMINAL_ECHO_OFF; */
    putchar('\n');
    printf("Hello, please choose an action:\n");
    printf("1 - Registation\n");
    printf("2 - Login\n");
    printf("3 - Exit\n");
    putchar('\n');
    scanf("%s", _actionFromUser);
}

void PrintLoginScreen(char *_actionFromUser)
{
    putchar('\n');
    printf("Please choose an action:\n");
    printf("1 - Create Group\n");
    printf("2 - Join Group\n");
    printf("3 - Leave Group\n");
    printf("4 - Logout\n");
    putchar('\n');
    scanf("%s", _actionFromUser);
}

void PrintInvalidInput()
{
    printf("Invalid input\n");
}

void PrintInvalidLength()
{
    printf("Invalid input length\n");
}

void AskForGroupName(char *_groupName)
{
    printf("choose group name (max len - %d)\n", MAX_GROUP_NAME_SIZE);
    scanf("%s", _groupName);
}

void AskForUserName(char *_userName)
{
    printf("choose user name (without spaces)\n");

    scanf("%s", _userName);
}

void AskForPassword(char *_password)
{
    printf("choose password (without spaces)\n");
    scanf("%s", _password);
}

void PrintResponseRegiSucc()
{
    putchar('\n');
    printf("Registartion success\n");
}

void PrintResponseRegiFail()
{
    putchar('\n');
    printf("Registartion failed\n");
}

void PrintResponseLoginSucc()
{
    putchar('\n');
    printf("Login success\n");
}

void PrintResponseLoginFail()
{
    putchar('\n');
    printf("Login failed\n");
}

void PrintCreateGroupFail()
{
    putchar('\n');
    printf("Group creation failed\n");
}

void PrintCreateGroupSuccess()
{
    putchar('\n');
    printf("Group creation success\n");
}

void PrintOptionManu()
{
    printf("press group number for option\n");
}

void PrintIndexGroup(size_t counter, char *group)
{
    printf("%ld. %s\n", counter, group);
}

void PrintEmptyGroup()
{
    printf("no group exist\n");
}

void GetUserOptionJoinGroup(char *_option)
{
    scanf("%s", _option);
}

void printErrorJoinGroup()
{
    printf("you already joined\n");
}

void JoinGroupSuccess()
{
    printf("success join group\n");
}

void GetLeaveGroup(char *_input)
{
    scanf("%s", _input);
}

void PrintLeaveGroup()
{
    printf("Choose a group to leave\n");
}

void PrintLeaveSucess(char *_groupName)
{
    printf("You have left group - %s\n", _groupName);
}

void PrintNoGroups()
{
    printf("You haven't joined any group yet\n");
}

void PrintExit()
{
    printf("Good bye!\n");
}

void PrintFailServer()
{
    printf("Action failed - send/recv from server!\n");
}

void PrintLogoutSuccess()
{
    printf("Logout success\n");
}


