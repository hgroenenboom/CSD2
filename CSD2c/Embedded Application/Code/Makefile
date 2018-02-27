CC = g++

# Include map (header files)
CFLAGS = -I/usr/local/include -Wall -std=c++1z
#LDFLAGS= -ljack

# Library map (cpp of lib files)
LDFLAGS= -L"C:/Program Files (x86)/Jack/lib" -llibjack
# -llibjack64 voor cygwin

all: example cleano

example : polymidi.o wavetable.o oscillator.o synth.o jack_module.o main.o 
	$(CC) -o $@ $(CFLAGS) polymidi.o wavetable.o oscillator.o synth.o jack_module.o main.o $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

full: clean all cleano
	
cleano:
	rm -f *.o
	
clean:
	rm -f *.o
	rm -f example
