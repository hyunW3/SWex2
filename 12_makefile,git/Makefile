#Makefile
CC = gcc
CFLAG = -g -O -Wall
CSRCS = main.c hello.c 
TARGET = run
OBJECTS = $(CSRCS:.c=.o)

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@
c.o:
	$(CC) $(FLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS) $(TARGET)

