# ifndef __CLIENTAPPDEMO_H__
# define __CLIENTAPPDEMO_H__

#include <unistd.h>  /* pid_t */
#include <sys/types.h>/* pid_t */


typedef enum GroupChat 
{
    GROUP_CHAT_FAILED =0,
    GROUP_CHAT_CANNOT_OPEN_FILE,
    GROUP_CHAT_NULL_INPUT_PIDS_TO_KILL,
    GROUP_CHAT_INVALID_PID_TO_KILL,
    GROUP_CHAT_SUCCESS


}GroupChat;



/**
 * @brief open window for the user to chat,
 * and scond window of the relevant group.
 * 
 * @param _groupIP IP of the group.
 * @param _groupPort port of the group.
 * @param _groupName name of the group.
 * @param _userName user name.
 * @return GROUP_CHAT_SUCCESS if good. 
 */
GroupChat OpenGroupChatWindows(char* _groupIP , char* _groupPort, char *_groupName, char* _userName);


/**
 * @brief take PIDs of listener and sender from files 
 * and stroe at pidToKillListen pidToKillSend.
 * 
 * @param pidToKillListen - PID var to store listener PID 
 * @param pidToKillSend - PID var to store sender PID 
 * @return GROUP_CHAT_SUCCESS if good. 
 */
GroupChat GetPIDsToKill(pid_t *_listenProcToKill,pid_t *_sendProcToKill);



/**
 * @brief recieve to process and perform kill.
 * 
 * @param _listenProcToKill - listener process to kill.
 * @param _sendProcToKill - sender process to kill.
 * @return GROUP_CHAT_SUCCESS if good. 
 */
GroupChat KillListenSenderProcess(pid_t _listenProcToKill,pid_t _sendProcToKill);






/* GroupChat RunListenerAPI(char* _groupIP ,char* _groupPort); */

/* GroupChat RunSenderAPI(char* _groupIP ,char* _groupPort, char* _userName); */




#endif   /*__CLIENTAPPDEMO_H__  */