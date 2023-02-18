#makefile listener


TARGET= listener.out
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS= listener.o
LFLAGS= -I../../../include/misc./

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

listener.o: listener.c
	$(CC) $(CFLAGS) $(LFLAGS) -c  listener.c


clean:
	rm -f $(TARGET) $(OBJS)