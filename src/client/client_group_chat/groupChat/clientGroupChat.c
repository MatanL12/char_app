# include "clientGroupChat.h"
# include "limitations.h"
# include <string.h>  /* srtcpy, strcat */
# include <stdio.h>   /* FILE */
# include <stdlib.h>  /* atoi */
# include <signal.h>  /* kill */
#include <sys/types.h> /* kill */

/********************************** UTILITY FUNCTIONS *********************************** */
static GroupChat RunListener(char* _groupIP ,char* _groupPort, char *_groupName);
static GroupChat RunSender(char* _groupIP ,char* _groupPort,char *_groupName, char* _userName);


/************************************ API FUNCTIONS ************************************* */

GroupChat OpenGroupChatWindows(char* _groupIP ,char* _groupPort,char *_groupName, char* _userName)
{
    if (_groupIP == NULL || _groupPort == NULL|| _groupName == NULL || _userName == NULL)
    {
        return GROUP_CHAT_FAILED;
    }

    RunListener(_groupIP ,_groupPort,_groupName);
    RunSender(_groupIP ,_groupPort,_groupName, _userName);

    return GROUP_CHAT_SUCCESS;
}



GroupChat GetPIDsToKill(pid_t *_listenProcToKill, pid_t *_sendProcToKill) 
{
    char pidToKillListener[PID_MAX_LENGTH],pidToKillSender[PID_MAX_LENGTH];
    FILE *fpListner, *fpSender;

    if (_listenProcToKill == NULL || _sendProcToKill == NULL )
    {
        return GROUP_CHAT_NULL_INPUT_PIDS_TO_KILL;
    }

    if (((fpListner = fopen("ListenerPIDsToKill.txt","r")) == NULL)
        || ((fpSender = fopen("SenderPIDsToKill.txt","r")) == NULL))
    {
        return GROUP_CHAT_CANNOT_OPEN_FILE;
    }

    fgets(pidToKillListener,PID_MAX_LENGTH,fpListner);
    *_listenProcToKill = atoi(pidToKillListener);

    fgets(pidToKillSender,PID_MAX_LENGTH,fpSender);
    *_sendProcToKill = atoi(pidToKillSender);
    
    fclose(fpListner);
    fclose(fpSender);

    return GROUP_CHAT_SUCCESS;
}

 


GroupChat KillListenSenderProcess(pid_t _listenProcToKill,pid_t _sendProcToKill)
{
    int ans1,ans2;
    if (_listenProcToKill <= 0  || _sendProcToKill <= 0)
    {
        return GROUP_CHAT_INVALID_PID_TO_KILL;
    }

    ans1 = kill(_sendProcToKill, SIGTERM);
    ans2 = kill(_listenProcToKill, SIGTERM);
    

    return GROUP_CHAT_SUCCESS;
}





/********************************** UTILITY FUNCTIONS *********************************** */

static GroupChat RunListener(char* _groupIP ,char* _groupPort, char *_groupName)
{

    char listener[4000];

    strcpy (listener, "gnome-terminal -e ") ;
    strcat (listener, "\"") ; 
    strcat (listener, "../client_group_chat/listener/listener.out ") ; 
    strcat (listener, _groupIP) ; 
    strcat (listener, " ") ; 
    strcat (listener, _groupPort) ; 
    strcat (listener, " ") ; 
    strcat (listener, _groupName) ; 
    strcat (listener, ";") ; 
    strcat (listener, "\"") ; 


    system(listener);
    return GROUP_CHAT_SUCCESS;
}


static GroupChat RunSender(char* _groupIP ,char* _groupPort,char *_groupName, char* _userName)
{
    char sender[4000];

    strcpy (sender, "gnome-terminal -e ") ;
    strcat (sender, "\"") ; 
    strcat (sender, "../client_group_chat/sender/sender.out ") ; 
    strcat (sender, _groupIP); 
    strcat (sender, " ") ; 
    strcat (sender, _groupPort); 
    strcat (sender, " ") ; 
    strcat (sender, _groupName); 
    strcat (sender, " ") ; 
    strcat (sender, _userName);
    strcat (sender, ";") ; 
    strcat (sender, "\"") ; 

    system(sender);
    return GROUP_CHAT_SUCCESS;
}









    
    



