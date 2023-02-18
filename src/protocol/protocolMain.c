
#include "protocol.h"

# include <stdio.h>


void TestPackRegistrationlongPassword()
{
    char buffer[100] = {0};
    char* userName = "ork";
    char* password = "5901111111";

    if (PackRegistration(buffer,userName,password) != 17)
    {
        printf("TestPackRegistrationlongPassword \033[31mFAIL\033[0m\n");
    }
    else
    {
        printf("TestPackRegistrationlongPassword \033[32mPASS\033[0m\n");
    }
}


void TestPackRegistrationShortPassword()
{
    char buffer[100];
    char* userName = "ork";
    char* password = "590";

    if (PackRegistration(buffer,userName,password) != 10)
    {
        printf("TestPackRegistrationShortPassword \033[31mFAIL\033[0m\n");
    }
    else
    {
        printf("TestPackRegistrationShortPassword \033[32mPASS\033[0m\n");
    }
}




int main()
{
    TestPackRegistrationlongPassword();
    TestPackRegistrationShortPassword();




    return 0;
}