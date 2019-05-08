TARGETS = main
CC = g++
CFLAGS = -Wall -Werror -std=c++11 -pthread -O2
SOURCES = init.cpp main.cpp

all: clean $(TARGETS)

main:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SOURCES) $(HEADERS) -o $@

clean:
	rm -rf $(TARGETS) *.o
