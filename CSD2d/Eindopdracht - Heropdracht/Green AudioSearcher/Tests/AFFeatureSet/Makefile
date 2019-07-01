CC = g++

# Include map (header files)
CFLAGS = -I"../../JuceLibraryCode" -I"C:/Users/HAROL/Documents/JUCE/modules" -Wall -std=c++1z

# Library map (cpp of lib files)
LDFLAGS= -L"../../JuceLibraryCode" -L"C:/Users/HAROL/Documents/JUCE/modules"

# Preprocessor defitions
# PPDFLAGS= -D_CRT_SECURE_NO_WARNINGS -DWIN32 -D_WINDOWS -DDEBUG -D_DEBUG -DJUCER_VS2017_78A5024=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0

all: example cleano

example : main.o 
	$(CC) -o $@ $(PPDFLAGS) $(CFLAGS) main.o  $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(PPDFLAGS)

full: clean all cleano
	
cleano:
	rm -f *.o
	
clean:
	rm -f *.o
	rm -f example
