#makefile game 

VLG = valgrind --leak-check=yes --track-origins=yes
TARGET= a.out
CC= gcc
CFLAGS=  -I../include/ -g -pedantic
OBJS= user_manager.o user_manager_test.o user.o
#-ansi
all: build run

vlg: $(TARGET)
	$(VLG) ./$(TARGET)
run:
	./$(TARGET)
	
build: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $(TARGET) -g -ansi user_manager.c user_manager_test.c ../User/user.c -L../../lib -lddslib -lm

user_manager_test.o: user_manager_test.c
	$(CC) $(CFLAGS) -c user_manager_test.c -lm

user_manager.o: user_manager.c
	$(CC) $(CFLAGS) -c user_manager.c -lm

user.o: ../User/user.c
		$(CC) $(CFLAGS) -c ../User/user.c

clean:
	rm -f $(TARGET) $(OBJS)