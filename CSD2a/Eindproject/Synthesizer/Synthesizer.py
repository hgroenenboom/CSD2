import numpy as np
import array
from math import ceil
import random

import Probability as Pb

class Oscillator():
    """De Oscillator class bevat meerdere soorten oscillators. Deze worden met een callback aangeroepen."""
    #TODO - finish sine, only after that start other synthesis techniques
    SINE = 1
    PULSE = 2
    FM = 3
    WNOISE = 4
    AM = 5
    "Types: Sine, Pulse, FM, White Noise, AM"

    monoPulseAmp = 0 #1 of -1
    monoAddSawAmpA = [0] * 6  # de amplitudes van de boventonen van de additieve saw
    monoAddSawPhasA = [0] * 6  # de fases van de boventonen van de additieve saw
    phaseModulator = 0
    ringMod = 0

    prob = Pb.Probability(1, 0.03, exp=True, refreshRate=150)
    prob2 = Pb.Probability(0.8, 0.03, exp=True, refreshRate=150)

    def __init__(self, frequency=100, phase=0, type=SINE, channels=2, rate=44100, amp=0.5, pulsewidth=0.5, ratio=1.25, modDepth = 1, framesPerBuffer = 256):
        """De initializer van Oscillator"""
        self.freq = frequency
        self.phas = phase
        self.type = type
        self.chan = channels
        self.rate = rate
        self.plswdth = pulsewidth
        self.amp = amp
        self.ratio = ratio
        self.modDepth = modDepth
        self.nFrames = framesPerBuffer

        #self.outputFrame = array.array('h', [0]) * self.chan
        self.outputBuffer = np.zeros(self.chan * self.nFrames, dtype='int16')

    def Run(self, ampMod=1):
        """De Run zal voor een audio frame een buffer volschrijven aan de hand van het gekozen synthese type"""
        if self.type == self.SINE:          #simple sinewave on all channels
            for b in range(self.nFrames):
                self.prob.setValue()    #get new probability value
                self.prob2.setValue()
                for c in range(self.chan):  # fill buffer with the same sin on every channel
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.prob2.smoothValue * np.sin(self.phas) * ampMod)
                    self.phas += 2 * np.pi * (self.freq * self.prob.smoothValue) / self.rate
        elif self.type == self.PULSE:
            for b in range(self.nFrames):
                if self.phas > self.plswdth:     #square wave oscillator with pulse width
                    self.monoPulseAmp = -1
                else:
                    self.monoPulseAmp = 1
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.monoPulseAmp * ampMod)
                self.phas += self.freq / self.rate
                self.phas = self.phas % 1.0
        elif self.type == self.FM:  #mono FM Synthesis
            for b in range(self.nFrames):
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * np.sin(self.phas) * ampMod)
                self.phas += 2 * np.pi * (self.freq / self.rate + np.sin(self.phaseModulator) * self.modDepth)
                self.phaseModulator += 2 * np.pi * (self.ratio * self.freq) / self.rate
        elif self.type == self.WNOISE:
            for b in range(self.nFrames):
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(random.uniform(-32767, 32767) * ampMod)     #Noise
        elif self.type == self.AM:  #mono AM
            for b in range(self.nFrames):
                if self.ringMod == 0:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * (np.sin(self.phas) * np.sin(self.phaseModulator)) * ampMod)  #original sinewave
                    self.phas += 2 * np.pi * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.ratio / self.rate
                else:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * (np.sin(self.phas) * (np.sin(self.phaseModulator) * 0.5 + 1)) * ampMod)  # original sinewave
                    self.phas += 2 * np.pi * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.ratio / self.rate
        else:
            print("non existing synthesis type")
            for n in range(len(self.outputBuffer)):
                self.outputBuffer[n] = 0.0
        return (self.outputBuffer)

    # def getOutput(self):              #nog niet gebruikt
    #     return self.outputBuffer

class ADSR:

    #arpeggiator
    adsrCounter = 0     #a counter for the arpeggiator
    arpTime = 1         #amount of seconds betweeen arpeggiator steps

    #volume
    adsrOutput = 0
    adsrDamp = 2.0
    adsrOutputScaled = 0

    #ADSR Stages
    adsrAttStage = False    #booleans for triggering the ADSR stage
    adsrDecStage = False
    adsrSusStage = False    #NIET GEBRUIKT
    adsrRelStage = False    #NIET GEBRUIKT

    def __init__(self, att=0.01, dec=0.3, sus=0, rel=0.04, audiorate=44100, channels=1, framesPerBuffer=256):
        #ADSR times
        self.att = att
        self.dec = dec
        self.sus = sus  #NIET GEBRUIKT
        self.rel = rel  #NIET GEBRUIKT

        #Audio information
        self.rate = audiorate
        self.chan = channels
        self.nFrames = framesPerBuffer

        #Used buffer
        self.outputBuffer = np.zeros(self.chan * self.nFrames, dtype='float16')

    def Run(self):
        """De Run zal voor een audio frame een buffer volschrijven met amplitude waardes uit actieve ADSR fases"""
        for b in range(self.nFrames):   #loop voor de hoeveelheid frames per buffer
            #initialize of restart de ADSR / arpeggiator
            if self.adsrCounter > self.arpTime * self.rate or self.adsrCounter == 0:
                self.adsrOutput = 0
                self.adsrCounter = 0
                self.adsrAttStage = True

                # self.freq *=   #3/4 fun

            self.adsrCounter += 1
            if self.adsrAttStage == True:  #attack stage
                if self.att <= 0:
                    self.att = 0.001
                self.adsrOutput += 1 / int(ceil(self.att * self.rate))
                if self.adsrOutput > 0.99:
                    self.adsrAttStage = False
                    self.adsrDecStage = True
            if self.adsrDecStage == True:  #decay stage
                if self.dec <= 0:
                    self.dec = 0.001
                self.adsrOutput += -1 / int(ceil(self.dec * self.rate))
                if self.adsrOutput < 0.01:
                    self.adsrDecStage = False
            #sustain en release niet nodig in een arpeggiator

            # exponential scaling
            self.adsrOutputScaled = self.adsrOutput ** self.adsrDamp

            #filling a frame based on the amount of channels per frame
            for c in range(self.chan):
                self.outputBuffer[b * self.chan + c] = self.adsrOutputScaled
        #The output is the full buffer
        return(self.outputBuffer)

class VCA:
    def __init__(self, audiorate=44100, channels=1, framesperbuffer=256):
        #Audio information
        self.chan = channels
        self.rate = audiorate

        #Used buffer
        self.vcaBuffer = np.zeros(self.chan, dtype='int16')

    def multiply(self, input1, modulator=1):
        """Multiply the incoming buffers. Used for amplitude control."""
        self.vcaBuffer = np.multiply(input1, modulator)
        #make sure the integer type stays the same so the stream will be able to read the buffer.
        self.vcaBuffer = self.vcaBuffer.astype('int16')
        return self.vcaBuffer

    def get(self):          #Used to obtain the buffer of the VCA
        return self.vcaBuffer

# class Buffer:       #UNUSED
#     def __init__(self, audiorate=44100, channels=1, framesperbuffer=256):
#         self.chan = channels
#         self.rate = audiorate
#         self.framesPerBuffer = framesperbuffer
#         #self.outbuf = array.array('h', [0]) * self.chan * self.framesPerBuffer  # * FRAMESPERBUFFER * CHANNELS  # array of signed ints
#         self.outbuf = np.zeros(self.chan * self.framesPerBuffer, dtype='int16')
#
#     def bufferize(self, input, index, channels):
#         for c in range(channels):
#             self.outbuf[index * channels + c] = input
#         return self.outbuf
#
#     def getOutput(self):
#         return self.outbuf

class AverageFilter:  # TODO - filter laten controleren, stereo maken met de channels input, bufferinput gebruiken
    filterOutput = 0

    def __init__(self, lowpassamount=1, highpassamount=0, channels=1):
        self.lowpassAmount = lowpassamount
        self.highpassAmount = highpassamount
        self.chan = channels

    def Run(self, inputbuffer, index):
        if self.lowpassAmount > 0:      #0 = bypass. Met de waarde filterLowpass kan de filter intensiteit bepaald worden
            filterOutput = 0    #initializeer output met 0
            for p in range(self.lowpassAmount):     #loop "lowpassAmount" keer de lowPass af
                filterOutput += inputbuffer[index - p]  #de specifieke inputbuffer waardes worden bij filteroutput opgeteld
                filterOutput /= self.lowpassAmount      #de filteroutput wordt gedeeld door "lowpassAmount" (de hoeveelheid samples uit de buffer
            inputbuffer[index - self.lowpassAmount] = int(filterOutput)    #hier wordt van de !oudste! bufferwaarde de waarde aangepast zodat de nieuwere waardes nog voor de volgende filterwaardes gebruikt kunnen worden
        # if self.highpassAmount > 0:
        #     filterOutput = 0
        #     for p in range(self.highpassAmount):
        #         filterOutput += inputbuffer[index - p]
        #         filterOutput /= self.highpassAmount
        #     inputbuffer[index - self.highpassAmount] = int(filterOutput)
        return inputbuffer
