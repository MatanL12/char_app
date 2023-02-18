# makefile clientUI

TARGET= clientUITest.out
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS = clientUITest.o clientUI.o 

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
	
clientUITest.o: clientUITest.c  
	$(CC) $(CFLAGS) -c clientUITest.c 
	
clientUI.o: clientUI.c 
	$(CC) $(CFLAGS) -c clientUI.c

clean:
	rm -f $(TARGET) $(OBJS)
	
	
	
