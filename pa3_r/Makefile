#-----------------------------------------------------------
#
# SSE2033: System Software Experiment 2 (Fall 2018)
#
# Makefile for PA#3
#
# Nov 1, 2018.
# CSLab, Sungkyunkwan University
#
#-----------------------------------------------------------


CC		= gcc
CFLAGS 	= -g -O -Wall
TARGET = swsh cp cat head tail

$(TARGET): swsh.c cp.c cat.c head.c tail.c
	$(CC) $(CFLAGS) -o swsh swsh.c
	$(CC) $(CFLAGS) -o cp cp.c
	$(CC) $(CFLAGS) -o cat cat.c
	$(CC) $(CFLAGS) -o head head.c
	$(CC) $(CFLAGS) -o tail tail.c

clean:
	rm -f $(TARGET)
