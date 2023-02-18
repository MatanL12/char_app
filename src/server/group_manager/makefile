#makefile group_manager

VLG = valgrind --leak-check=yes --track-origins=yes
TARGET= group_manager_test.out
CC= gcc
CFLAGS= -ansi -g -pedantic
LFLAGS= -L../../lib/ -lddslib -lm -I../../Include/
OBJS= group_manager_test.o group_manager.o group.o protocol.o
GROUP_PATH= ../Group/
PROTOCOL_PATH= ../../Protocol/
#-ansi
all: build run

vlg: $(TARGET)
	$(VLG) ./$(TARGET)
run:
	./$(TARGET)
	
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LFLAGS)

group_manager_test.o: group_manager_test.c
	$(CC) $(CFLAGS) -c group_manager_test.c -lm

group_manager.o: group_manager.c
	$(CC) $(CFLAGS) -c group_manager.c -lm

group.o: $(GROUP_PATH)group.c
	$(CC) $(CFLAGS) -c $(GROUP_PATH)group.c -lm

protocol.o: $(PROTOCOL_PATH)protocol.c
		$(CC) $(CFLAGS) -c $(PROTOCOL_PATH)protocol.c

clean:
	rm -f $(TARGET) $(OBJS)