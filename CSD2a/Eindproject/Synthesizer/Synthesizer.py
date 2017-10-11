# This file contains all synthesis classes. All of these classes are designed to be called by a callback.

import numpy as np
from math import ceil
import random

import Probability as Pb    #importeer de probability

# Deze class bevat alle mogelijke oscillators
class Oscillator():
    """De Oscillator class bevat meerdere soorten oscillators. Deze worden met een callback aangeroepen."""
    #TODO MUST - finish sine, only after that start other synthesis techniques
    #TODO MUST - think if theres a way to constrain the synthesis type to lesser code.
    #TODO MUST - Comment synthesis types

    #Synthesis types.
    SINE = 1
    PULSE = 2
    FM = 3
    WNOISE = 4
    AM = 5
    "Types: Sine, Pulse, FM, White Noise, AM"

    # synthese variabellen: de waardes moeten onthouden worden, daarom moeten ze hier al worden aangemaakt.
    monoPulseAmp = 0 #value: 1 of -1
    phaseModulator = 0     #
    ringMod = True #a boolean for switching between AM & Ringmod

    # Probabability objecten.
    prob = Pb.Probability(1, 0.03, exp=True, refreshWait=150)
    prob2 = Pb.Probability(0.8, 0.03, exp=True, refreshWait=150)

    def __init__(self, frequency=100, phase=0, type=SINE, channels=2, rate=44100, amp=0.5, pulsewidth=0.5, ratio=1.25, modDepth = 1, framesPerBuffer = 256):
        """De initializer van Oscillator"""

        #Init values.
        self.type = type
        self.freq = frequency
        self.phas = phase
        self.plswdth = pulsewidth
        self.amp = amp
        self.ratio = ratio
        self.modDepth = modDepth

        #Init audio information.
        self.chan = channels
        self.rate = rate
        self.nFrames = framesPerBuffer

        #create output buffer.
        self.outputBuffer = np.zeros(self.chan * self.nFrames, dtype='int16')

    def Run(self, ampMod=1):
        """De Run zal voor een audio frame een buffer volschrijven aan de hand van het gekozen synthese type"""
        #SINE Synthese:
        if self.type == self.SINE:          #simple sinewave on all channels
            for b in range(self.nFrames):
                self.prob.setNewValue()    #get new probability value
                self.prob2.setNewValue()
                for c in range(self.chan):  # fill buffer with the same sin on every channel
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.prob2.smoothValue * np.sin(self.phas) * ampMod)
                    self.phas += 2 * np.pi * (self.freq * self.prob.smoothValue) / self.rate
        #PULSE Synthese:
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
        #FM Synthese:
        elif self.type == self.FM:  #mono FM Synthesis
            for b in range(self.nFrames):
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * np.sin(self.phas) * ampMod)
                self.phas += 2 * np.pi * (self.freq / self.rate + np.sin(self.phaseModulator) * self.modDepth)
                self.phaseModulator += 2 * np.pi * (self.ratio * self.freq) / self.rate
        #White Noise Synthese:
        elif self.type == self.WNOISE:
            for b in range(self.nFrames):
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(random.uniform(-32767, 32767) * ampMod)     #Noise
        #AM / Ringmod Synthese:
        elif self.type == self.AM:  #mono AM
            for b in range(self.nFrames):
                if self.ringMod == True:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * (np.sin(self.phas) * np.sin(self.phaseModulator)) * ampMod)  # Ring Mod
                    self.phas += 2 * np.pi * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.ratio / self.rate
                else:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * (np.sin(self.phas) * (np.sin(self.phaseModulator) * 0.5 + 1)) * ampMod)  # AM
                    self.phas += 2 * np.pi * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.ratio / self.rate
        #Non existing value for self.type
        else:
            print("non existing synthesis type")
            for n in range(len(self.outputBuffer)):
                self.outputBuffer[n] = 0      #Fill buffer with zeros
        return (self.outputBuffer)

# Deze class bevat een ADSR, op dit moment is het een AR.
class ADSR:

    #arpeggiator values
    adsrCounter = 0     #a counter for the arpeggiator
    arpTime = 1         #amount of seconds betweeen arpeggiator steps

    #volume
    adsrOutput = 0  #output value of ADSR
    adsrDamp = 2.0  #exponent for the adsr output. This creates a damping / dynamic feeling for the ADSR.
    adsrOutputScaled = 0    #exponentially scaled output value of ADSR

    #ADSR Stages
    adsrAttStage = False    #booleans for triggering the ADSR stages
    adsrDecStage = False
    # adsrSusStage = False    #NIET GEBRUIKT
    # adsrRelStage = False    #NIET GEBRUIKT

    def __init__(self, att=0.01, dec=0.3, sus=0, rel=0.04, audiorate=44100, channels=1, framesPerBuffer=256):
        #ADSR time value worden geinitializeerd
        self.att = att
        self.dec = dec
        # self.sus = sus  #NIET GEBRUIKT
        # self.rel = rel  #NIET GEBRUIKT

        #Audio information init
        self.rate = audiorate
        self.chan = channels
        self.nFrames = framesPerBuffer

        #output buffer
        self.outputBuffer = np.zeros(self.chan * self.nFrames, dtype='float16')

    def Run(self):
        """De Run zal voor een audio frame een buffer volschrijven met amplitude waardes uit actieve ADSR fases"""
        #loop voor de hoeveelheid frames in een buffer
        for b in range(self.nFrames):
            #initialize of restart de ADSR / arpeggiator
            if self.adsrCounter > self.arpTime * self.rate or self.adsrCounter == 0:
                self.adsrOutput = 0     #amplitude naar 0
                self.adsrCounter = 0    #de tijds counter naar 0
                self.adsrAttStage = True    #de attack stage mag weer aan

                # self.freq *=   #3/4 fun

            #De tijdsteller van de ADSR
            self.adsrCounter += 1

            #Attack stage
            if self.adsrAttStage == True:   #alleen als boolean true is.
                #clippen van att tijd: self.att > 0
                if self.att <= 0:
                    self.att = 0.001
                #berekent met hoeveel de output moet optellen om 1 te worden na de gekozen attack tijd.
                self.adsrOutput += 1 / int(ceil(self.att * self.rate))
                #einde van attack stage
                if self.adsrOutput > 0.99:
                    self.adsrAttStage = False
                    self.adsrDecStage = True
            #Decay stage: same as attack stage
            if self.adsrDecStage == True:
                if self.dec <= 0:
                    self.dec = 0.001
                self.adsrOutput += -1 / int(ceil(self.dec * self.rate))
                if self.adsrOutput < 0.01:
                    self.adsrDecStage = False
            #sustain en release niet nodig in een arpeggiator

            #Exponential scaling of the adsr output, this feels like damping of the envelope.
            self.adsrOutputScaled = self.adsrOutput ** self.adsrDamp

            #filling a frame based on the amount of channels per frame
            for c in range(self.chan):
                self.outputBuffer[b * self.chan + c] = self.adsrOutputScaled
        #The output is the full buffer containing a piece of the ADSR. (or the full ADSR)
        return(self.outputBuffer)

# Deze class fungeert als digitale VCA. VCA kan meerdere lijsten met elkaar verminigvuldigen ( L0[n]*L1[n] )
class VCA:
    def __init__(self, audiorate=44100, channels=1, framesperbuffer=256):
        #Audio information
        self.chan = channels
        self.rate = audiorate

        #Used buffer
        self.vcaBuffer = np.zeros(self.chan, dtype='int16')

    # The actual VCA function. Multiplies multiple lists.
    def multiply(self, input1, modulator=1):
        """Multiply the incoming buffers. Used for amplitude control."""
        self.vcaBuffer = np.multiply(input1, modulator)
        #make sure the integer type stays the same so the stream will be able to read the buffer.
        self.vcaBuffer = self.vcaBuffer.astype('int16')
        #return output buffer
        return self.vcaBuffer

    #Used to obtain the buffer of the VCA. For instance to create the master output buffer.
    def get(self):
        return self.vcaBuffer

###############################################################
#EVERYTHING BENEATH THIS LINE IS UNUSED, BACKUP / WIP MATERIAL##
#_________________________________________________________________________________________________

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

#Filter is not yet working
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

#NOTES
#
#OSCILLATOR
        # self.outputFrame = array.array('h', [0]) * self.chan