# include "../../../Include/clientGroupChat.h"
# include <string.h>  /* srtcpy, strcat */
# include <stdio.h>   /* FILE */
# include <stdlib.h>  /* atoi */



void TestWindowsFromTwoUsers()
{
    char* groupIP = "224.0.0.0";
    char*  groupPort = "6666";
    char* userName1 = "ork";
    char* userName2 = "dani";
    char* groupName = "TOMER_POPO";

    GroupChat ans1,ans2;

    ans1 = OpenGroupChatWindows(groupIP,groupPort,groupName,userName1);;
    ans2 = OpenGroupChatWindows(groupIP,groupPort,groupName,userName2);;

    if (ans1!= GROUP_CHAT_SUCCESS || ans2!= GROUP_CHAT_SUCCESS)
    {
        printf("TestWindowsFromTwoUsers \033[31mFAIL\033[0m\n");
    }
    else
    {
        printf("TestWindowsFromTwoUsers \033[32mPASS\033[0m\n\n");
    }
}



void TestGetPIDsToKill()
{
    char* groupIP = "224.0.0.0";
    char*  groupPort = "6666";
    char* userName1 = "ork";
    char* userName2 = "dani";
    char* groupName = "TOMER_POPO";

    pid_t p1,p2;
    GroupChat ans1,ans2;

    ans1 = OpenGroupChatWindows(groupIP,groupPort,groupName,userName1);
    ans2 = OpenGroupChatWindows(groupIP,groupPort,groupName,userName2);
    

    /* GetPIDsToKill(&p1, &p2); */
    /* printf("p1 is %d\np2 is %d\n", p1,p2); */


    /* KillListenSenderProcess(p1,p2); */
}




int main()
{
    TestGetPIDsToKill();
    /* TestWindowsFromTwoUsers(); */
} 
