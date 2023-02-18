#makefile sender

TARGET= sender.out
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS= sender.o
LFLAGS= -I../../../include/misc./ 


$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) 

sender.o: sender.c
	$(CC) $(CFLAGS) $(LFLAGS) -c sender.c


clean:
	rm -f $(TARGET) $(OBJS)