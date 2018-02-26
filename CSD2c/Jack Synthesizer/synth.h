#ifndef SYNTH_H
#define SYNTH_H

#include "oscillator.h"

template <class numType>
class Synth{
public:
    Synth() {
        oscpoint[0] = &saw;
        oscpoint[1] = &sine;
    }

    void process(numType* buffer, int buffersize) {
        // oscP->nextAudioBlock(buffer, buffersize);
        oscpoint[oscType]->nextAudioBlock(buffer, buffersize);
    }

    void setSamplerate(int sr) {
        sine.setSamplerate(sr);
        saw.setSamplerate(sr);
    }

    // Oscillator<numType>* oscP = new Sine<numType>;
    //Sine<numType>* oscP = new Sine<numType>;

    Sine<numType> sine;
    Saw<numType> saw;

    int oscType = 1;

    Oscillator<numType>** oscpoint = new Oscillator<numType>* [3];
};

#endif
