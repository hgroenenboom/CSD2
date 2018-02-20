CC = g++
CFLAGS = -I/usr/local/include -Wall -std=c++1z
LDFLAGS= --L"C:/Program Files (x86)/Jack/lib" -llibjack64

all: synthesizer

synthesizer: main.o oscilator.o jack_module.o synth.o
	$(CC) -o $@ $(CFLAGS) synthesizer main.o oscilator.o jack_module.o synth.o

clean:
	rm -f *.o
