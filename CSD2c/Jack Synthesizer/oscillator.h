#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <math.h>

#define PI_2 6.28318530717959

template <class numType>
class Oscillator{
public:
    // virtual constructor met de samplerate?
    virtual void nextAudioBlock(numType* buffer, int buffersize)=0;

    void setSamplerate(numType s) { samplerate = s; }

    void setAmplitude(numType a) { amplitude = a; }

    void setFrequency(numType f) { frequency = f; }
protected:
    numType samplerate = 44100.0f;
    numType amplitude = 1.0f;
    numType phase = 0.0f;
    numType frequency = 440.0f;
};

template <class numType>
class Sine   : public Oscillator<numType> {
public:
    void nextAudioBlock(numType* buffer, int buffersize) {
        for(int i = 0; i < buffersize; i++) {
            buffer[i] = this->amplitude * sin(PI_2 * this->phase);
            this->phase += (this->frequency / this->samplerate);
        }
    }
};

template <class numType>
class Saw   : public Oscillator<numType> {
public:
    void nextAudioBlock(numType* buffer, int buffersize) {
        for(int i = 0; i < buffersize; i++) {
            buffer[i] = this->amplitude * (this->phase * 2.0 - 1.0) * 0.5 + oldVal;
            this->phase += (this->frequency / this->samplerate);
            this->phase = fmod(this->phase, 1.0);
            oldVal = buffer[i] * 0.5;
        }
    }

    numType oldVal = 0;
};

// void Sine<numType>::nextAudioBlock(numType* buffer, int buffersize) {
//
// }


// 10Hz
// 100ms
// 441 samples per cycle
// 1 / 441
//
// 100 / 44100

#endif
