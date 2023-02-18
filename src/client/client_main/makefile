 #!/bin/sh
FILES = $(shell chmod 777 "../client_group_chat/listener/listener.out")
FILE2= $(shell chmod 777 "../client_group_chat/sender/sender.out")

VLG = valgrind --leak-check=yes --track-origins=yes
TARGET= main_client.out
CC= gcc
CFLAGS= -ansi -g -pedantic
LFLAGS= -L../../../lib/ -lddslib -lm -I../../../include/client/ -I../../../include/protocol/ -I../../../include/data_structures/ -I../../../include/misc./

OBJS= main_client.o client_app.o client_net.o clientUI.o clientGroupChat.o protocol.o
CLIENT_NET_PATH= ../client_net/
CLIENT_APP_PATH= ../client_app/
CLIENT_UI_PATH= ../client_UI/
CLIENT_GROUP_CHAT_PATH= ../client_group_chat/groupChat/
PROTOCOL_PATH= ../../protocol/

#-ansi
all: build run

vlg: $(TARGET)
	$(VLG) ./$(TARGET)
run:
	./$(TARGET)

	
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LFLAGS)
	$(FILES) 
	$(FILE2)
main_client.o: main_client.c
	$(CC) $(CFLAGS) $(LFLAGS) -c main_client.c -lm

client_app.o: $(CLIENT_APP_PATH)client_app.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $(CLIENT_APP_PATH)client_app.c -lm

client_net.o: $(CLIENT_NET_PATH)client_net.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $(CLIENT_NET_PATH)client_net.c -lm

clientUI.o: $(CLIENT_UI_PATH)clientUI.c
		$(CC) $(CFLAGS) $(LFLAGS) -c $(CLIENT_UI_PATH)clientUI.c

clientGroupChat.o: $(CLIENT_GROUP_CHAT_PATH)clientGroupChat.c
		$(CC) $(CFLAGS) $(LFLAGS) -c $(CLIENT_GROUP_CHAT_PATH)clientGroupChat.c

protocol.o: $(PROTOCOL_PATH)protocol.c
		$(CC) $(CFLAGS) $(LFLAGS) -c $(PROTOCOL_PATH)protocol.c

clean:
	rm -f $(TARGET) $(OBJS)