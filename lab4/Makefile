CC=g++

all: executable

debug: CFLAGS= -g -Wall -ansi -l rt -O0
debug: executable

release: CFLAGS= -Wall -ansi -l rt -O2
release: executable

executable:
	$(CC) $(CFLAGS) -o timer timer.cpp
clean:
	rm timer
