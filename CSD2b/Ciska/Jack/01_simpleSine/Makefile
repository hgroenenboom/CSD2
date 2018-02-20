CC = g++

# Include map (header files)
CFLAGS = -I/usr/local/include -Wall -std=c++1z
#LDFLAGS= -ljack

# Library map (cpp of lib files)
LDFLAGS= -L"C:/Program Files (x86)/Jack/lib" -llibjack
# -llibjack64 voor cygwin

all: example

example : jack_module.o main.o
	$(CC) -o $@ $(CFLAGS) jack_module.o main.o $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
	rm -f example
