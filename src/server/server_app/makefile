#makefile server_app 

VLG = valgrind --leak-check=yes --track-origins=yes
TARGET= server_app.out
CC= gcc
CFLAGS=  -I../include/ -g -pedantic -ansi
OBJS= server_app.o 

all: build run

vlg: $(TARGET)
	$(VLG) ./$(TARGET)
run:
	./$(TARGET)
	
build: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $(TARGET) -g -ansi net_server_test.c net_server.c -L../lib -lddslib

net_server.o: server_app.c
	$(CC) $(CFLAGS) -c server_app.c

clean:
	rm -f $(TARGET) $(OBJS)
