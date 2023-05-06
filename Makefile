CC=g++
CFLAGS=-c -Wall -D_WIN32_WINNT=0x600

all: StarRailFPSViewer

StarRailFPSViewer: main.o
	$(CC) main.o -o StarRailFPSViewer -lpsapi

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o StarRailFPSViewer
