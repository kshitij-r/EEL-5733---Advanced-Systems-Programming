#Kshitij Raj
#UFID-13584965
CC=gcc

CFlags=-c -pthread

all: code

code: main.o	
	$(CC) -pthread assignment4.c -o assignment4

main.o: assignment4.c
	$(CC) $(CFlags) assignment4.c

clean:
	rm -rf *o assignment4
