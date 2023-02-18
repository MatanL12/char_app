#include "../../Include/g_queue.h"
#include "../../Include/group_manager.h"
#include "../../Include/group.h"
#include "../../Include/limitations.h"
#include <string.h>
#include <stdio.h>

#define PORT_SIZE 5

Queue* GetQueue(GroupManager* _groupManager);
Group* GetGroup(GroupManager* _groupManager, char* _name);
size_t GetGroupCounter(Group* _group);
int PrintQueueActionFunc(const void *_element, void *_context);


void PosTestPrintAllIp()
{
    GroupManager* groupManger;
    Queue* queue;
    
    groupManger = CreateGroupManager();
    queue = GetQueue(groupManger);

    QueueForEach(queue,PrintQueueActionFunc, NULL);
}


void PosTestAddGroup()
{
    GroupManager* groupManger;
    Group* group1, *group2;
    GroupManagerErr status1, status2;
    char ip1[MAX_IP_LEN], ip2[MAX_IP_LEN], port1[PORT_SIZE], port2[PORT_SIZE],  groupName1[MAX_NAME_LENGTH] = "TomersGroup", ipRecv1[MAX_IP_LEN], *groupNameRecv1,
    groupName2[MAX_NAME_LENGTH] = "OrsGroup", ipRecv2[MAX_IP_LEN], *groupNameRecv2, *nameBuff1, *nameBuff2;

    groupManger = CreateGroupManager();

    status1 = AddGroup(groupManger, groupName1, ip1, port1, &nameBuff1);
    
    status2 = AddGroup(groupManger, groupName2, ip2, port2, &nameBuff2);

    group1 = GetGroup(groupManger, groupName1);
    GetGroupName(group1, &groupNameRecv1);
    GetGroupIP(group1, ipRecv1); 

    group2 = GetGroup(groupManger, groupName2);
    GetGroupName(group2, &groupNameRecv2);
    GetGroupIP(group2, ipRecv2); 

    if( (strcmp(groupNameRecv1, groupName1) != 0) ||
        (strcmp(ipRecv1, "224.0.0.1") != 0) ||
        GetGroupCounter(group1) != 1 ||
        (strcmp(port1, "1500") != 0) ||
        (strcmp(groupName1, nameBuff1) != 0) ||
        (strcmp(groupNameRecv2, groupName2) != 0) ||
        (strcmp(ipRecv2, "224.0.0.2") != 0) ||
        GetGroupCounter(group2) != 1 ||
        (strcmp(port2, "1501") != 0) ||
        (strcmp(groupName2, nameBuff2) != 0))
    {
        printf("PosTestAddGroup - failed\n");
    }
    else
    {
        printf("PosTestAddGroup - pass\n");
    }

    DestroyGroupManager(groupManger);
    return;
}


void TestAddGroupDuplicate()
{
    GroupManager* groupManger;
    Group* group1, *group2;
    GroupManagerErr status1, status2;
    char ip1[MAX_IP_LEN], ip2[MAX_IP_LEN], port1[PORT_SIZE],port2[PORT_SIZE], groupName1[MAX_NAME_LENGTH] = "TomersGroup", ipRecv1[MAX_IP_LEN], *groupNameRecv1,
    groupName2[MAX_NAME_LENGTH] = "TomersGroup", ipRecv2[MAX_IP_LEN], *groupNameRecv2, *nameBuff1, *nameBuff2;

    groupManger = CreateGroupManager();

    status1 = AddGroup(groupManger, groupName1, ip1, port1, &nameBuff1);
    status2 = AddGroup(groupManger, groupName2, ip2, port2, &nameBuff2);

    group1 = GetGroup(groupManger, groupName1);
    GetGroupName(group1, &groupNameRecv1);
    GetGroupIP(group1, ipRecv1); 

    group2 = GetGroup(groupManger, groupName2);
    GetGroupName(group2, &groupNameRecv2);
    GetGroupIP(group2, ipRecv2); 

    if( status1 != GROUP_MANAGER_SUCCESS ||
        status2 != GROUP_MANAGER_UNAVAILBLE_NAME)
    {
        printf("TestAddGroupDuplicate - failed\n");
    }
    else
    {
        printf("TestAddGroupDuplicate - pass\n");
    }

    DestroyGroupManager(groupManger);
    return;
}


void TestAddGroupOverflow()
{
    GroupManager* groupManger;
    int i;
    GroupManagerErr status;
    char ip[MAX_IP_LEN], groupName[MAX_NAME_LENGTH], port1[PORT_SIZE], *nameBuffer;

    groupManger = CreateGroupManager();

    for(i=0; i<MAX_ACTIVE_GROUP_NUMBER; i++)
    {
        sprintf(groupName, "group%d", i); 
        groupName[strlen(groupName)] = '\0';
        if(AddGroup(groupManger, groupName, ip, port1, &nameBuffer) != GROUP_MANAGER_SUCCESS)
        {
            printf("TestAddGroupOverflow - failed\n");
        }
    }

    status = AddGroup(groupManger, "group101", ip, port1, &nameBuffer);

    if( status != GROUP_MANAGER_OVERFLOW)
    {
        printf("TestAddGroupOverflow - failed\n");
    }
    else
    {
        printf("TestAddGroupOverflow - pass\n");
    }

    DestroyGroupManager(groupManger);
    return;
}



void PosTestJoinGroup()
{
    GroupManager* groupManger;
    Group* group1, *group2;
    GroupManagerErr status1, status2;
    char ip1[MAX_IP_LEN], ip2[MAX_IP_LEN], port1[PORT_SIZE], port2[PORT_SIZE],  groupName1[MAX_NAME_LENGTH] = "TomersGroup", ipRecv1[MAX_IP_LEN], *groupNameRecv1,
    groupName2[MAX_NAME_LENGTH] = "OrsGroup", ipRecv2[MAX_IP_LEN], *groupNameRecv2, *nameBuff1, *nameBuff2;

    groupManger = CreateGroupManager();

    status1 = AddGroup(groupManger, groupName1, ip1, port1, &nameBuff1);
    status2 = AddGroup(groupManger, groupName2, ip2, port2, &nameBuff2);

    group1 = GetGroup(groupManger, groupName1);
    GetGroupName(group1, &groupNameRecv1);
    GetGroupIP(group1, ipRecv1); 

    JoinGroup(groupManger, groupName1, ip1, port1); 

    if( (strcmp(ipRecv1, "224.0.0.1") != 0) ||
        GetGroupCounter(group1) != 2 ||
        (strcmp(port1, "1500") != 0))
    {
        printf("PosTestJoinGroup - failed\n");
    }
    else
    {
        printf("PosTestJoinGroup - pass\n");
    }

    DestroyGroupManager(groupManger);
    return;
}


void TestJoinGroupNotfound()
{
    GroupManager* groupManger;
    Group* group1, *group2;
    GroupManagerErr status1, status2;
    char ip1[MAX_IP_LEN], ip2[MAX_IP_LEN], port1[PORT_SIZE], port2[PORT_SIZE],  groupName1[MAX_NAME_LENGTH] = "TomersGroup", ipRecv1[MAX_IP_LEN], *groupNameRecv1,
    groupName2[MAX_NAME_LENGTH] = "OrsGroup", ipRecv2[MAX_IP_LEN], *groupNameRecv2, *nameBuff1, *nameBuff2;

    groupManger = CreateGroupManager();

    status1 = AddGroup(groupManger, groupName1, ip1, port1, &nameBuff1);
    status2 = AddGroup(groupManger, groupName2, ip2, port2, &nameBuff2);

    group1 = GetGroup(groupManger, groupName1);
    GetGroupName(group1, &groupNameRecv1);
    GetGroupIP(group1, ipRecv1); 

    JoinGroup(groupManger, groupName2, ip1, port1); 

    if(GetGroupCounter(group1) != 1) 
        
    {
        printf("TestJoinGroupNotfound - failed\n");
    }
    else
    {
        printf("TestJoinGroupNotfound - pass\n");
    }

    DestroyGroupManager(groupManger);
    return;
}



int main(void)
{
    PosTestAddGroup();
    TestAddGroupDuplicate();
    TestAddGroupOverflow();
    PosTestJoinGroup();
    TestJoinGroupNotfound();
    return 0;
}



int PrintQueueActionFunc(const void *_element, void *_context)
{
    printf("%s\n", (char*)_element);
    return 1;
}