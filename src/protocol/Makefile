# makefile protocolTest.c

TARGET= protocolTest.out
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS = protocolTest.o protocol.o 

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
	
protocolTest.o: protocolTest.c  
	$(CC) $(CFLAGS) -c protocolTest.c 
	
protocol.o: protocol.c 
	$(CC) $(CFLAGS) -c protocol.c

clean:
	rm -f $(TARGET) $(OBJS)
	
	
	
