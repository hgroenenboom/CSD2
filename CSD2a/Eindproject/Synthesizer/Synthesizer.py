# This file contains all synthesis classes. All of these classes are designed to be called by a callback.

import numpy as np
from math import ceil
import random

import Randomizer as Rand    #importeer de probability, voor random frequentie en amplitude fluctuaties.
import Fade #importeer fade om clicks in de adsr weg te halen.

# Deze class bevat alle mogelijke oscillators
class Oscillator():
    """De Oscillator class bevat meerdere soorten oscillators. Deze worden met een callback aangeroepen."""
    #TODO MUST - think if theres a way to constrain the synthesis type to lesser code.
    #TODO MUST - Comment synthesis types

    #Synthesis types.
    SINE = 1    #sine synthesis with fluctuating pitch & amplitude
    PULSE = 2   #pulse synthesis with pulse width & fluctuating pitch & amplitude (-1 or 1)
    FM = 3      #FM synthesis with a modulator ratio, modulator amplitude & fluctuating pitch & amplitude
    WNOISE = 4  #noise synthesis with fluctuating pitch & amplitude (random number generator)
    AM = 5      #AM synthesis with ringmod and AM. Contains a modulator ratio & fluctuating pitch & amplitude
    "Types: Sine, Pulse, FM, White Noise, AM"   #all synthesis types summed up

    # synthese variabellen: de waardes moeten onthouden worden, daarom moeten ze hier al worden aangemaakt.
    monoPulseAmp = 0 #the amplitude of the pulse oscillator, value: 1 of -1
    phaseModulator = 0     #phase of the FM & AM modulator
    ringMod = True #a boolean for switching between AM & Ringmod
    freq = 0    #the used frequency for the synthesis

    # Probabability objecten.
    randHz = Rand.NaturalRandomness(1, 0.03, exp=True, refreshWait=150, smoothing=5)
    randAmp = Rand.NaturalRandomness(0.8, 0.1, exp=True, refreshWait=150, smoothing=5)
    randOther = Rand.NaturalRandomness(1, 0.3, exp=True, refreshWait=150, smoothing=5)

    def __init__(self, frequency=100, phase=0, type=SINE, channels=2, rate=44100, amp=0.5, pulsewidth=0.5, ratio=1.25
                 , modDepth = 1, framesPerBuffer = 256):
        """De initializer van Oscillator"""

        #Init values.
        self.type = type              # synthesis type
        self.centerFreq = frequency   # the center frequency
        self.phas = phase             # oscillator phase
        self.amp = amp                # oscillator amplitude
        self.ratio = ratio            # FM & AM modulator ratio
        self.modDepth = modDepth      # FM modulator amplitude
        self.plswdth = pulsewidth     # pulse oscillator pulsewidth

        #Init audio information.
        self.chan = channels                #amount of output channels
        self.rate = rate                    #audio rate
        self.nFrames = framesPerBuffer      #amount of frames per callback buffer

        #create output buffer based on audio information.
        self.outputBuffer = np.zeros(self.chan * self.nFrames, dtype='int16')

    #set a listener for the oscillator to receive frequentie input, in this case the sequencer. (input *  centerfrequentie)
    def setListener(self, triggerObject):   #input is an object of the sequencer class
        self.triggerObject = triggerObject  #link the sequener object to a local object. #TODO - is dit wat er gebeurd?

    #generate probability values everytime the function is called.
    def randomize(self):
        self.randHz.setNewValue()  # get new probability values
        self.randAmp.setNewValue()
        self.randOther.setNewValue()

    #Run wordt aangeroepen in een callback en zal een buffer volschrijven met audio informatie.
    def Run(self, ampMod=1):
        """De Run zal voor een audio frame een buffer volschrijven aan de hand van het gekozen synthese type"""

        #frequentie input van de sequencer in de vorm van een ratio.
        self.freq = self.centerFreq * self.triggerObject.getPitch()

        #SINE Synthese:
        if self.type == self.SINE:
            for b in range(self.nFrames):   #loop for the amount of frames in the buffer
                self.randomize()
                for c in range(self.chan):  #loop for amount of channels per frame
                    # compute amplitude value and place it in the buffer
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothValue
                                                               * np.sin(self.phas) * ampMod)
                # compute new phase value
                self.phas += 2 * np.pi * (self.freq * self.randHz.smoothValue) / self.rate
        #PULSE Synthese:
        elif self.type == self.PULSE:
            for b in range(self.nFrames):
                self.randomize()
                #compute the pulse amplitude corresponding to the phase and the pulse width
                if self.phas > self.plswdth:
                    self.monoPulseAmp = -0.35
                else:
                    self.monoPulseAmp = 0.35
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothValue * self.monoPulseAmp * ampMod)
                self.phas += self.freq * self.randHz.smoothValue / self.rate
                self.phas = self.phas % 1.0     #modulo the phase so it can be used in the above if statements
        #FM Synthese:
        elif self.type == self.FM:  #mono FM Synthesis
            for b in range(self.nFrames):
                self.randomize()
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp
                                                               * self.randAmp.smoothValue * np.sin(self.phas) * ampMod)
                self.phas += 2 * np.pi * (self.freq * self.randHz.smoothValue / self.rate + np.sin(self.phaseModulator) * self.modDepth)
                self.phaseModulator += 2 * np.pi * (self.ratio * self.freq * self.randOther.smoothValue) / self.rate
        #White Noise Synthese:
        elif self.type == self.WNOISE:
            for b in range(self.nFrames):
                self.randomize()
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(random.uniform(-32767, 32767)
                                        * ampMod * 0.25 * self.randAmp.smoothValue)     #Noise
        #AM / Ringmod Synthese:
        elif self.type == self.AM:  #mono AM
            for b in range(self.nFrames):
                self.randomize()
                if self.ringMod == True:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothValue
                                                                   * (np.sin(self.phas) * np.sin(self.phaseModulator)) * ampMod)  # Ring Mod
                    self.phas += 2 * np.pi * self.randHz.smoothValue * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.randOther.smoothValue * self.ratio / self.rate
                else:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothValue
                                                                   * (np.sin(self.phas) * (np.sin(self.phaseModulator) * 0.5 + 1)) * ampMod)  # AM
                    self.phas += 2 * np.pi * self.randHz.smoothValue * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.randOther.smoothValue * self.ratio / self.rate
        #If non existing value for self.type
        else:
            print("non existing synthesis type")
            for n in range(len(self.outputBuffer)):
                self.outputBuffer[n] = 0      #Fill buffer with zeros
        #return the outputBuffer
        return (self.outputBuffer)


###########################################################
# Deze class bevat een ADSR, op dit moment is het een AR.
class ADSR:
    fade = Fade.Fade(0, 0, 1, 5) #random init values

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

    trigger = False

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

    def setListener(self, triggerObject):
        self.triggerObject = triggerObject

    def Run(self):
        """De Run zal voor een audio frame een buffer volschrijven met amplitude waardes uit actieve ADSR fases"""
        #loop voor de hoeveelheid frames in een buffer
        try:
            # get trigger value
            self.trigger = self.triggerObject.getTrigger()
            # set fade value
            self.fade.setFadeOnce(self.adsrOutput, 0, 10, self.rate / self.nFrames)
        except:
            pass

        for b in range(self.nFrames):
            #initialize of restart de ADSR / arpeggiator
            #if self.adsrCounter > self.arpTime * self.rate or self.adsrCounter == 0:
            if self.trigger == True:
                #print("trigger")
                #amplitude naar 0
                self.adsrCounter = 0    #de tijds counter naar 0
                self.adsrAttStage = True    #de attack stage mag weer aan
                self.trigger = False
                self.triggerObject.trigger = 0

            #De tijdsteller van de ADSR
            self.adsrCounter += 1

            #Attack stage
            if self.adsrAttStage == True:   #alleen als boolean true is.
                #korte fade naar 0
                self.fade.runFade()
                #clippen van att tijd: self.att > 0
                if self.att <= 0:
                    self.att = 0.001
                #berekent met hoeveel de output moet optellen om 1 te worden na de gekozen attack tijd.
                self.adsrOutput += 1 / int(ceil(self.att * self.rate))
                #einde van attack stage
                if self.adsrOutput >= 0.99:
                    self.adsrAttStage = False
                    self.adsrDecStage = True
            #Decay stage: same as attack stage
            if self.adsrDecStage == True:
                if self.dec <= 0:
                    self.dec = 0.001
                self.adsrOutput += -1 / int(ceil(self.dec * self.rate))
                if self.adsrOutput <= 0:
                    self.adsrDecStage = False
            #sustain en release niet nodig in een arpeggiator

            #Exponential scaling of the adsr output, this feels like damping of the envelope.
            try:
                self.adsrOutputScaled = float(self.adsrOutput ** self.adsrDamp)
            except:
                self.adsrOutputScaled = 0

            #filling a frame based on the amount of channels per frame
            for c in range(self.chan):
                self.outputBuffer[b * self.chan + c] = self.adsrOutputScaled
        #The output is the full buffer containing a piece of the ADSR. (or the full ADSR)
        return(self.outputBuffer)

###########################################################
# Deze class fungeert als digitale VCA. VCA kan meerdere lijsten met elkaar verminigvuldigen ( L0[n] * L1[n] )
class VCA:
    def __init__(self, audiorate=44100, channels=1, framesperbuffer=256):
        #Audio information
        self.chan = channels
        self.rate = audiorate

        #Used buffer
        self.vcaBuffer = np.zeros(self.chan, dtype='int16')

    # The actual VCA function. Multiplies multiple lists.
    # The modulator can be a list of the same bufferlength as the input or a float or integer value.
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

###########################################################
#Filter is not yet working
class AverageFilter:  # TODO - filter laten controleren, stereo maken met de channels input, bufferinput gebruiken
    filterOutput = 0

    def __init__(self, framesPerBuffer=256, channels=1):
        self.nFrames = framesPerBuffer
        self.chan = channels

    def Run(self, inputbuffer):
        for b in range(self.nFrames):   #loop for the amount of frames in the buffer
            filterOutput = inputbuffer[b*self.chan - self.chan] +  inputbuffer[b*self.chan]  #de specifieke inputbuffer waardes worden bij filteroutput opgeteld
            filterOutput *= 0.5      #de filteroutput wordt gedeeld door "lowpassAmount" (de hoeveelheid samples uit de buffer
            for c in range(self.chan):  #loop for amount of channels per frame
                filterOutput  = filterOutput.astype('int16')
                inputbuffer[b * self.chan - self.chan + c] = filterOutput    #hier wordt van de !oudste! bufferwaarde de waarde aangepast zodat de nieuwere waardes nog voor de volgende filterwaardes gebruikt kunnen worden
        return inputbuffer

#NOTES
#
#OSCILLATOR
        # self.outputFrame = array.array('h', [0]) * self.chan