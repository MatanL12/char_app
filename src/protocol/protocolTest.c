#include "../Include/protocol.h"
# include <string.h>
# include <stdio.h>

#define PROTOCOL_TAG_INDEX (0)
#define PROTOCOL_TOTAL_LEN_INDEX (1)



/***************************** TEST FUNCTIONS */
void TestPackRegistrationShortPassword()
{
    int i=0;
    char buffer[20];
    char* userName = "ork";
    char* password = "590";
    size_t ans = PackRegistration(buffer,userName,password);

    if (ans != 10)
    {
        printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
    }
 
    if (buffer[PROTOCOL_TAG_INDEX] != TAG_REGISTER )
    {
        printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
    }

    if (buffer[PROTOCOL_TOTAL_LEN_INDEX] != strlen(userName) + strlen(password) + 2)
    {
        printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
       
    }
 
    if (buffer[2] != strlen(userName))
    {
        printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
    }

    i = 3;

    while (*userName != '\0')
    {
        if (buffer[i++] != *userName)
        {
            printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
        }
        userName++;
    }
    

    if (buffer[i++] != strlen(password))
    {
        printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
    }
 

    while (*password != '\0')
    {
        if (buffer[i++] != *password)
        {
            printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
        }
        password++;
    }

    if (i != ans)
    {
        printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
    }

    i = 0;
    printf("should be: 1 8 3 111 114 107 3 53 57 48\nActual:    ");
    
    while (i < ans)
    {
        printf("%d ", buffer[i]);
        i++;
    }
    printf("\n");

    printf("TestPackRegistrationShortPassword \033[32mPASS\033[0m\n");
}





void TestPackRegistrationlongPassword()
{
    char buffer[100] = {0};
    char* userName = "ork";
    char* password = "5900000000";

    if (PackRegistration(buffer,userName,password) != 17)
    {
        printf("TestPackRegistrationlongPassword \033[31mFAIL\033[0m\n");
    }
    else
    {
        printf("TestPackRegistrationlongPassword \033[32mPASS\033[0m\n");
    }

    
}



void TestUnPackRegistrationShort()
{
    int i=3;
    char buffer[30] = {0};
    size_t messageLen = 50;
    char userName[30] = "ork";
    char password[30] = "590";

    char incUserName[30];
    char incPassword[30];

    PackRegistration(buffer,userName,password);
    UnPackRegistration(buffer,messageLen,incUserName,incPassword);

    if ((strcmp(incUserName,userName) == 0) && (strcmp(incPassword,password) == 0))
    {
        printf("TestUnPackRegistration \033[32mPASS\033[0m\n");
    }
    else
    {
        printf("TestUnPackRegistration \033[31mFAIL\033[0m\n");
    }    
}


void TestUnPackRegistrationLong()
{
    int i=3;
    char buffer[30] = {0};
    size_t messageLen = 50;
    char* userName = "orktomermatan";
    char* password = "590999";

    char incUserName[30];
    char incPassword[30];

    PackRegistration(buffer,userName,password);
    UnPackRegistration(buffer,messageLen,incUserName,incPassword);

    if ((strcmp(incUserName,userName) == 0) && (strcmp(incPassword,password) == 0))
    {
        printf("TestUnPackRegistrationLong \033[32mPASS\033[0m\n");
    }
    else
    {
        printf("TestUnPackRegistrationLong \033[31mFAIL\033[0m\n");
    }    
}


void TestPackLoginShortPassword()
{
    int i=0;
    char buffer[20] = {0};
    char* userName = "ork";
    char* password = "590";
    size_t ans = PackLogin(buffer,userName,password);

    if (ans != 10)
    {
        printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
    }
 
    if (buffer[PROTOCOL_TAG_INDEX] != TAG_LOGIN )
    {
        printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
    }

    if (buffer[PROTOCOL_TOTAL_LEN_INDEX] != strlen(userName) + strlen(password) + 2)
    {
        printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
       
    }
 
    if (buffer[2] != strlen(userName))
    {
        printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
    }

    i = 3;

    while (*userName != '\0')
    {
        if (buffer[i++] != *userName)
        {
            printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
        }
        userName++;
    }
    

    if (buffer[i++] != strlen(password))
    {
        printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
    }
 

    while (*password != '\0')
    {
        if (buffer[i++] != *password)
        {
            printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
        }
        password++;
    }

    if (i != ans)
    {
        printf("TestPackLoginShortPassword \033[31mFAIL\033[0m\n");
    }

    i = 0;
    printf("should be: 1 8 3 111 114 107 3 53 57 48\nActual:    ");
    while (i < ans)
    {
        printf("%d ", buffer[i]);
        i++;
    }
    printf("\n");

    printf("TestPackLoginShortPassword \033[32mPASS\033[0m\n");
}


void TestPackLoginlongPassword()
{
    char buffer[100] = {0};
    char* userName = "ork";
    char* password = "5900000000";

    if (PackLogin(buffer,userName,password) != 17)
    {
        printf("TestPackRegistrationlongPassword \033[31mFAIL\033[0m\n");
    }
    else
    {
        printf("TestPackRegistrationlongPassword \033[32mPASS\033[0m\n");
    }

}



void TestUnPackLoginShort()
{
    int i=3;
    char buffer[30] = {0};
    size_t messageLen = 50;
    char* userName = "ork";
    char* password = "590";

    char incUserName[30];
    char incPassword[30];

    PackLogin(buffer,userName,password);
    UnPackLogin(buffer,messageLen,incUserName,incPassword);

    if ((strcmp(incUserName,userName) == 0) && (strcmp(incPassword,password) == 0))
    {
        printf("TestUnPackLoginShort \033[32mPASS\033[0m\n");
    }
    else
    {
        printf("TestUnPackLoginShort \033[31mFAIL\033[0m\n");
    }    
}


void TestUnPackLoginLong()
{
    int i=3;
    char buffer[30] = {0};
    size_t messageLen =50;
    char* userName = "orktomermatan";
    char* password = "590999";

    char incUserName[30];
    char incPassword[30];

    PackLogin(buffer,userName,password);
    UnPackLogin(buffer,messageLen,incUserName,incPassword);

    if ((strcmp(incUserName,userName) == 0) && (strcmp(incPassword,password) == 0))
    {
        printf("TestUnPackRegistrationLong \033[32mPASS\033[0m\n");
    }
    else
    {
        printf("TestUnPackRegistrationLong \033[31mFAIL\033[0m\n");
    }    
}



void TestPackResponse1()
{
    int i=0;
    char buffer[20] = {0};
    char msg = '8';
   
    size_t ans = PackResponse(buffer,msg);

    if (ans != 4)
    {
        printf("TestPackResponse1 a  \033[31mFAIL\033[0m\n");
    }
 
    if (buffer[PROTOCOL_TAG_INDEX] != TAG_RESPONSE )
    {
        printf("TestPackResponse1 b \033[31mFAIL\033[0m\n");
    }

    if (buffer[PROTOCOL_TOTAL_LEN_INDEX] != 2)
    {
        printf("TestPackResponse1 c \033[31mFAIL\033[0m\n");
       
    }
 
    if (buffer[2] != 1)
    {
        printf("TestPackResponse1 d \033[31mFAIL\033[0m\n");
    }


    if (buffer[3] != msg)
    {
        printf("TestPackResponse1 e \033[31mFAIL\033[0m\n");
    }
 
    printf("%ld \n",sizeof(buffer[3]));

    printf("should be: 3 2 1 56 \nActual:    ");   
    while (i < ans)
    {
        printf("%d ", buffer[i]);
        i++;
    }
    printf("\n");

    printf("TestPackResponse1 \033[32mPASS\033[0m\n");
}




void TestUnPackResponseShort()
{
    int i=0;
    char buffer[20] = {0};
    char msg = '8';
    size_t _messageLen = 50;

    char incMsg;

    size_t ans = PackResponse(buffer,msg);
    TagStatus ans2 = UnPackResponse(buffer,_messageLen,&incMsg);


    printf("%d ",incMsg);

    if ( incMsg == msg  )  
    {
        printf("TestUnPackResponseShort \033[32mPASS\033[0m\n");
    }
    else
    {
        printf("TestUnPackResponseShort \033[31mFAIL\033[0m\n");
    }    
}



void TestPackResponseCreateGroup()
{
    int i=0;
    char buffer[20] = {0};
    char* ip = "172.173.188.192";
    char* port = "8888";
    size_t ans = PackResponseCreateGroup(buffer,ip,port);

    if (ans != 23)
    {
        printf("TestPackResponseCreateGroup a \033[31mFAIL\033[0m\n");
    }
    
    
    if (buffer[PROTOCOL_TAG_INDEX] != TAG_RESPONSE_CREATE_GROUP )
    {
        printf("TestPackResponseCreateGroup b \033[31mFAIL\033[0m\n");
    }

    if (buffer[PROTOCOL_TOTAL_LEN_INDEX] != strlen(ip) + strlen(port) + 2)
    {
        printf("TestPackResponseCreateGroup c \033[31mFAIL\033[0m\n");
       
    }
 
    if (buffer[2] != strlen(ip))
    {
        printf("TestPackResponseCreateGroup d \033[31mFAIL\033[0m\n");
    }

    i = 3;

    while (*ip != '\0')
    {
        if (buffer[i++] != *ip)
        {
            printf("TestPackResponseCreateGroup e \033[31mFAIL\033[0m\n");
        }
        ip++;
    }
    

    if (buffer[i++] != strlen(port))
    {
        printf("TestPackResponseCreateGroup f \033[31mFAIL\033[0m\n");
    }
 

    while (*port != '\0')
    {
        if (buffer[i++] != *port)
        {
            printf("TestPackResponseCreateGroup g \033[31mFAIL\033[0m\n");
        }
        port++;
    }

    if (i != ans)
    {
        printf("TestPackResponseCreateGroup h \033[31mFAIL\033[0m\n");
    }

    i = 0;
    printf("should be: 5 21 15 49 55 50 46 49 55 51 46 49 56 56 46 49 57 50 4 56 56 56 56 \nActual:    ");
    
    while (i < ans)
    {
        printf("%d ", buffer[i]);
        i++;
    }
    printf("\n");

    printf("TestPackResponseCreateGroup \033[32mPASS\033[0m\n");
}


void TestUnPackResponseCreateGroup()
{
    int i=3;
    char buffer[100] = {0};
    size_t messageLen = 50;
    char* ip = "172.173.188.192";
    char* port = "8888";

    char incIP[30];
    char incPort[30];

    PackResponseCreateGroup(buffer,ip,port);
    UnPackResponseCreateGroup(buffer,messageLen,incIP,incPort);

    if ((strcmp(incIP,ip) == 0) && (strcmp(incPort,port) == 0))
    {
        printf("TestUnPackRegistration \033[32mPASS\033[0m\n");
    }
    else
    {
        printf("TestUnPackRegistration \033[31mFAIL\033[0m\n");
    }    
}



void TestPackCreateGroup()
{
    int i=0;
    char buffer[100] = {0};
    char userName[20] = "ork";
    char group[20] = "papa";

    size_t ans = PackCreateGroup(buffer,userName,group);

    if (ans != 11)
    {
        printf("TestPackResponse1 a  \033[31mFAIL\033[0m\n");
    }
 
    if (buffer[PROTOCOL_TAG_INDEX] != TAG_CREATE_GROUP )
    {
        printf("TestPackResponse1 b \033[31mFAIL\033[0m\n");
    }

    if (buffer[PROTOCOL_TOTAL_LEN_INDEX] != 9)
    {
        printf("TestPackResponse1 c \033[31mFAIL\033[0m\n");
       
    }
 
    if (buffer[2] != strlen(userName))
    {
        printf("TestPackResponse1 d \033[31mFAIL\033[0m\n");
    }


    printf("should be: 4 9 3 111 114 107 4 112 97 112 97  \nActual:    ");   
    while (i < ans)
    {
        printf("%d ", buffer[i]);
        i++;
    }
    printf("\n");

    printf("TestPackResponse1 \033[32mPASS\033[0m\n");
}


void TestUnPackCreateGroup()
{
    int i=3;
    size_t messageLen =50;
    char buffer[100] = {0};
    char userName[20] = "ork";
    char group[20] = "papa";

    size_t ans = UnPackCreateGroup(buffer,messageLen,userName,group);

    char incUser[30];
    char incGroup[30];

    PackCreateGroup(buffer,userName,group);
    UnPackCreateGroup(buffer,messageLen,incUser,incGroup);

    if ((strcmp(incUser,userName) == 0) && (strcmp(incGroup,group) == 0))
    {
        printf("TestUnPackRegistrationLong \033[32mPASS\033[0m\n");
    }
    else
    {
        printf("TestUnPackRegistrationLong \033[31mFAIL\033[0m\n");
    }    
}


int main()
{   
   
    TestPackRegistrationShortPassword();
    TestPackRegistrationlongPassword();
    
    TestUnPackRegistrationShort();
    TestUnPackRegistrationLong(); 

    TestPackLoginShortPassword();
    TestPackLoginlongPassword();
    
    TestUnPackLoginLong();
    TestUnPackLoginShort();
  
    TestPackResponse1(); 
    TestUnPackResponseShort(); 
    
    TestPackCreateGroup();
    TestUnPackCreateGroup();

    TestPackResponseCreateGroup();
    TestUnPackResponseCreateGroup(); 
    
     


    return 0;
}


