#makefile clientGroupChat

VLG = valgrind --leak-check=yes --track-origins=yes -s
TARGET= clientGroupChat.out
CC= gcc
CFLAGS= -ansi -I../../../include/misc./ -I../../../include/client/ -g -pedantic
OBJS= clientGroupChat.o  
all: build run

vlg: $(TARGET)
	$(VLG) ./$(TARGET)
run:
	./$(TARGET)
	
build: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clientGroupChat.o: clientGroupChat.c
	$(CC) $(CFLAGS) $(CFLAGS) -c  clientGroupChat.c


clean:
	rm -f $(TARGET) $(OBJS)