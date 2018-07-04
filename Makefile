CC=gcc
CXX=g++ -std=gnu++11
CFLAGS=-g -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -Wall #-DVERBOSE
BIN=.

all: spybash2



spybash2: spybash2.c
	$(CXX) $(CFLAGS) spybash2.c -lutil -o $(BIN)/spybash2

clean:
	rm -rf $(BIN)/spybash2
