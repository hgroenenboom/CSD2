# This file contains all synthesis classes. All of these classes are designed to be called by a callback.

import numpy as np
from math import ceil   #ceil is used to clip incoming values
import random

import Randomizer as Rand    #importeer the natural randomness, for natural frequency & amplitude fluctuations.
import Fade #import fade to remove clicks from the ADSR class.

# This class contains all oscillators.
class Oscillator():
    """De Oscillator class contains multiple types of oscillators. They are used by running the Run() function in a callback."""
    #TODO MUST - think if theres a way to constrain the synthesis type to lesser code.
    #TODO MUST - Comment synthesis types

    #Synthesis types.
    SINE = 1    #sine synthesis with fluctuating pitch & amplitude
    PULSE = 2   #pulse synthesis with pulse width & fluctuating pitch & amplitude (-1 or 1)
    FM = 3      #FM synthesis with a modulator ratio, modulator amplitude & fluctuating pitch & amplitude
    WNOISE = 4  #noise synthesis with fluctuating pitch & amplitude (random number generator)
    AM = 5      #AM synthesis with ringmod and AM. Contains a modulator ratio & fluctuating pitch & amplitude
    "Types: Sine, Pulse, FM, White Noise, AM"   #all synthesis types summed up

    # Synthesis variables: The values of these variables have to be stored while the oscillator is used.
    monoPulseAmp = 0 #the amplitude of the pulse oscillator, value: 1 of -1
    phaseModulator = 0     #phase of the FM & AM modulator
    ringMod = True #a boolean for switching between AM & Ringmod
    freq = 0    #the used frequency for the synthesis

    # Randomness objects.
    randHz = Rand.NaturalRandomness(1, 0.03, exp=True, refreshWait=150, smoothing=5)
    randAmp = Rand.NaturalRandomness(0.8, 0.1, exp=True, refreshWait=150, smoothing=5)
    randOther = Rand.NaturalRandomness(1, 0.3, exp=True, refreshWait=150, smoothing=5)

    def __init__(self, frequency=100, phase=0, type=SINE, channels=2, rate=44100, amp=0.5, pulsewidth=0.5, ratio=1.25
                 , modDepth = 1, framesPerBuffer = 256):
        """Init function of the oscillator class."""

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

    #set a listener for the oscillator to receive frequency input. Used for input from the sequencer. (input *  centerfrequentie)
    def setListener(self, triggerObject):   #input is an object of the sequencer class
        self.triggerObject = triggerObject  #link the sequener object to a local object. #TODO - is dit wat er gebeurd?

    #generate random values everytime the function is called. Used inside the Run() function.
    def randomize(self):
        self.randHz.setNewValue()  # get new probability values
        self.randAmp.setNewValue()
        self.randOther.setNewValue()

    #Run has to be called inside a callback function. The function will automaticly fill one audiobuffer.
    #   Used inside the callback of the pyAudio stream.
    def Run(self, ampMod=1):
        """Run will fill one audio buffer with audio information."""

        #frequency input from the sequencer. input from the sequencer is a modulating ratio.
        self.freq = self.centerFreq * self.triggerObject.getPitch()

        #SINE Synthesis:
            #the basic functions of this sine are used with all types of synthesis.
        if self.type == self.SINE:
            for b in range(self.nFrames):   #loop for the amount of frames in the buffer
                self.randomize()    #create new randomness values.
                for c in range(self.chan):  #loop for amount of channels per frame
                    # compute amplitude value and place it in the buffer
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothOutputValue
                                                               * np.sin(self.phas) * ampMod)
                # compute new phase value
                self.phas += 2 * np.pi * (self.freq * self.randHz.smoothOutputValue) / self.rate
        #PULSE Synthesis:
        elif self.type == self.PULSE:
            for b in range(self.nFrames):
                self.randomize()
                #compute the pulse amplitude corresponding to the phase and the pulse width
                if self.phas > self.plswdth:
                    self.monoPulseAmp = -0.35
                else:
                    self.monoPulseAmp = 0.35
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothOutputValue * self.monoPulseAmp * ampMod)
                self.phas += self.freq * self.randHz.smoothOutputValue / self.rate
                self.phas = self.phas % 1.0     #modulo the phase so it can be used in the above if statements
        #FM Synthesis:
        elif self.type == self.FM:  #mono FM Synthesis
            for b in range(self.nFrames):
                self.randomize()
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(32767 * self.amp
                                                               * self.randAmp.smoothOutputValue * np.sin(self.phas) * ampMod)
                self.phas += 2 * np.pi * (self.freq * self.randHz.smoothOutputValue / self.rate + np.sin(self.phaseModulator) * self.modDepth)
                self.phaseModulator += 2 * np.pi * (self.ratio * self.freq * self.randOther.smoothOutputValue) / self.rate
        #White Noise Synthesis:
        elif self.type == self.WNOISE:
            for b in range(self.nFrames):
                self.randomize()
                for c in range(self.chan):
                    self.outputBuffer[b * self.chan + c] = int(random.uniform(-32767, 32767)
                                                               * ampMod * 0.25 * self.randAmp.smoothOutputValue)     #Noise
        #AM or Ringmod Synthesis:
        elif self.type == self.AM:  #mono AM
            for b in range(self.nFrames):
                self.randomize()
                if self.ringMod == True:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothOutputValue
                                                                   * (np.sin(self.phas) * np.sin(self.phaseModulator)) * ampMod)  # Ring Mod
                    self.phas += 2 * np.pi * self.randHz.smoothOutputValue * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.randOther.smoothOutputValue * self.ratio / self.rate
                else:
                    for c in range(self.chan):
                        self.outputBuffer[b * self.chan + c] = int(32767 * self.amp * self.randAmp.smoothOutputValue
                                                                   * (np.sin(self.phas) * (np.sin(self.phaseModulator) * 0.5 + 1)) * ampMod)  # AM
                    self.phas += 2 * np.pi * self.randHz.smoothOutputValue * self.freq / self.rate
                    self.phaseModulator += 2 * np.pi * self.freq * self.randOther.smoothOutputValue * self.ratio / self.rate
        #If non existing value for self.type:
        else:
            print("non existing synthesis type")
            for n in range(len(self.outputBuffer)):
                self.outputBuffer[n] = 0      #Fill buffer with zeros
        #return the outputBuffer
        return (self.outputBuffer)


###########################################################
# An ADSR class. Used to shape the volume envelope of the Oscillator class. At this moment it's only an AR.
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
    # adsrSusStage = False    #UNUSED
    # adsrRelStage = False    #UNUSED

    trigger = False

    def __init__(self, att=0.01, dec=0.3, sus=0, rel=0.04, audiorate=44100, channels=1, framesPerBuffer=256):
        #initialize ADSR time values
        self.att = att
        self.dec = dec
        # self.sus = sus  #UNUSED
        # self.rel = rel  #UNUSED

        #Inizialize audio information
        self.rate = audiorate
        self.chan = channels
        self.nFrames = framesPerBuffer

        #Initialize ADSR output buffer
        self.outputBuffer = np.zeros(self.chan * self.nFrames, dtype='float16')

    #sets the listenener for the ADSR. Used to receive triggers from the Sequencer class.
    def setListener(self, triggerObject):
        self.triggerObject = triggerObject

    # Run has to be called inside a callback function. The function will automaticly fill one audiobuffer.
        #   Used inside the callback of the pyAudio stream.
    def Run(self):
        """The Run function will fill the ADSR buffer with values from the active envelope stages."""
        #Try receiving a new trigger
        try:
            # get trigger value
            self.trigger = self.triggerObject.getTrigger()
            # set fade value
            self.fade.setFadeOnce(self.adsrOutput, 0, 10, self.rate / self.nFrames)
        except:
            pass

        #write buffer with ADSR values
        for b in range(self.nFrames):
            #initialize of restart de ADSR / arpeggiator
            if self.trigger == True:
                #print("trigger")
                #amplitude naar 0
                self.adsrCounter = 0    #Reset ADSR time counter
                self.adsrAttStage = True    #Toggle attack stage
                self.trigger = False        #Set trigger to false.
                self.triggerObject.trigger = 0  #set the sequencers trigger to 0

            #ADSR Counter
            self.adsrCounter += 1

            #Attack stage
            if self.adsrAttStage == True:   #If attack stage is true.
                #run the fade function to remove starting clicks.
                self.fade.runFade()
                #clip Attack value so it wont reach zero: self.att > 0
                if self.att <= 0:
                    self.att = 0.001
                #calculate how much the adsrOuput should increase per frame.
                self.adsrOutput += 1 / int(ceil(self.att * self.rate))
                #End attack stage, start decay stage
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
            #Sustain & Release stage are not yet build. The current design does not need a sustain or release value.

            #Exponential scaling of the adsr output. This is used as envelope damping.
            try:
                self.adsrOutputScaled = float(self.adsrOutput ** self.adsrDamp)
            except:
                self.adsrOutputScaled = 0

            #filling a single frame based on the amount of output channels per frame.
            for c in range(self.chan):
                self.outputBuffer[b * self.chan + c] = self.adsrOutputScaled
        #The output is a filled adsr outputBuffer.
        return(self.outputBuffer)

###########################################################
# A list multiplyer class based on the logic of a VCA. VCA can multiply multiple lists. ( L0[n] * L1[n] )
class VCA:
    def __init__(self, audiorate=44100, channels=1, framesperbuffer=256):
        #Audio information
        self.chan = channels
        self.rate = audiorate

        #Used buffer. This buffer gets overwritten.
        self.vcaBuffer = np.zeros(self.chan, dtype='int16')

    # The actual VCA function. Multiplies multiple lists.
    # The modulator can be a list of the same bufferlength as the input or a float or integer value.
    def multiply(self, input1, modulator=1):
        """Multiply the incoming buffers. Used for amplitude control."""
        self.vcaBuffer = np.multiply(input1, modulator)
        #make sure the integer type stays the same after the multiplication, so the stream will be able to read the buffer.
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