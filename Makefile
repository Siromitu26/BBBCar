CC=gcc
OBJS=main.o BBBCar.o
CFLAGS=-W -Wall -lBBB
PROGRAM=BBBCarControlServer 
RM=rm -f
.SUFFIXES: .c .o

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
.c .o:
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
	