#makefile main_server

VLG = valgrind --leak-check=yes --track-origins=yes
TARGET= main_server.out
CC= gcc
CFLAGS= -ansi -g -pedantic
LFLAGS= -L../../../lib/ -lddslib -lm -I../../../include/server/ -I../../../include/client/ -I../../../include/data_structures/ -I../../../include/misc./ -I../../../include/protocol/ 
OBJS= server_app_test.o server_app.o net_server.o user.o user_manager.o  group.o group_manager.o protocol.o

SERVER_NET_PATH= ../net_server/
SERVER_APP_PATH= ../server_app/
SERVER_USER_PATH= ../user/
SERVER_GROUP_PATH = ../group/
SERVER_GROUP_MANAGER_PATH =../group_manager/
SERVER_USER_MANAGER_PATH= ../user_manager/
PROTOCOL_PATH= ../../protocol/


# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../../lib/ 
# export LD_LIBRARY_PATH 

all: build run

vlg: $(TARGET)
	$(VLG) ./$(TARGET)
run:
	./$(TARGET)
	
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LFLAGS)

server_app_test.o: server_app_test.c
	$(CC) $(CFLAGS) $(LFLAGS) -c server_app_test.c -lm

server_app.o: $(SERVER_APP_PATH)server_app.c
	$(CC) $(CFLAGS) $(LFLAGS)-c $(SERVER_APP_PATH)server_app.c -lm

net_server.o: $(SERVER_NET_PATH)net_server.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SERVER_NET_PATH)net_server.c -lm

group.o: $(SERVER_GROUP_PATH)group.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SERVER_GROUP_PATH)group.c -lm

group_manager.o: $(SERVER_GROUP_MANAGER_PATH)group_manager.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SERVER_GROUP_MANAGER_PATH)group_manager.c -lm

user.o: $(SERVER_USER_PATH)user.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SERVER_USER_PATH)user.c -lm

user_manager.o: $(SERVER_USER_MANAGER_PATH)user_manager.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SERVER_USER_MANAGER_PATH)user_manager.c -lm

protocol.o: $(PROTOCOL_PATH)protocol.c
		$(CC) $(CFLAGS) $(LFLAGS) -c $(PROTOCOL_PATH)protocol.c

clean:
	rm -f $(TARGET) $(OBJS)