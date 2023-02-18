#include <stdio.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>
#include "../../Include/hash_map.h"
#include "../../Include/user_manager.h"

int loggedGood1();
int isLogged(UserManager *_userManager);
void testAddToGroup();
void TestPrintUserList(UserManager *_userManager, char *username);

int main()
{
    testAddToGroup();
    loggedGood1();

    return 0;
}

void testAddToGroup()
{
    UserManager *manager = CreateUserMan();
    char **foo;
    char *userName = "ork";
    char *grouName1 = "ors group1";
    char *grouName2 = "ors group2";
    char *grouName3 = "ors group3";

    char *res1 = malloc(sizeof(strlen(grouName1) + 1));
    char *res2 = malloc(sizeof(strlen(grouName2) + 1));
    char *res3 = malloc(sizeof(strlen(grouName3) + 1));

    strcpy(res1, grouName1);
    foo = &res1;
    /* printf("%s\n", *foo); */
    InserGroupToUser(manager, userName, foo);

    strcpy(res2, grouName2);
    foo = &res2;
    InserGroupToUser(manager, userName, foo);

    strcpy(res3, grouName3);
    foo = &res3;

    InserGroupToUser(manager, userName, foo);

    TestPrintUserList(manager, userName);

    printf("---------------\n");
    strcpy(res2, "hello zain sheli");

    TestPrintUserList(manager, userName);

    /* free(res1);
    free(res2);
    free(res3); */

    /*
    if (manager == NULL)
    {
        printf("user create fail\n");
    }


    InserGroupToUser(manager, userName, &grouName2);
    InserGroupToUser(manager, userName, &grouName3);

     */
}

int loggedGood1()
{
    char user[100] = "ork";
    char pass[100] = "590";
    UserManager *manager = CreateUserMan();

    if (LoginUser(manager, user, pass) == SUCCESS_USER_MANAGER)
    {
        printf("success\n");
    }
    else
    {
        printf("fail\n");
    }
}

/* char name[200] = "tamar";
    char pass[12] = "123456";

    char name1[200] = "natan";
    char pass1[12] = "123456";

    char name2[200] = "yoav";
    char pass2[12] = "123456";

    UserManager *manager;

    manager = CreateUserMan();

    if (manager == NULL)
    {
        printf("fail to create ...\n");
    }
    AddNewUser(manager, name, pass);
    AddNewUser(manager, name1, pass1);
    AddNewUser(manager, name2, pass2);

    DestroyUserMan(&manager); */