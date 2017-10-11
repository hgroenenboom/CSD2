# My first coded synthesizer
# Made by: Harold Groenenboom
#
# General info: Notes in the bottom of main. Made to be expandable to polyphony, or midi input
#
# Based on the basic pyaudio program playing a real time mono sine wave by (ME) 2015 Marc Groenewegen
#
# TODO MUST
# - add stream to 'Audio' subfile
# - think about changing the buffer to [[frame01], [frame02], etc.] with every frame holding 'CHANNELS' channels
# - understand all GUI widgets and design an GUI
# TODO COULD
# - fix filter
# - implement general smoothing and fades
# - Multiple oscillators / polyphony

import pyaudio
import array
import numpy as np
from tkinter import *    #GUI

import GUI              #GUI Class - Tkinter
import Synthesizer         #Synthese classes - Osc, Env, Vca, LP
import Audio            #PyAudio & Stream - TODO MUST - stream toevoegen

#de full caps dingetjes zijn constanten. Je maakt iets dus full caps als je aan wilt geven dat dit nooit runtime verandert.
#Audio settings
WIDTH = 2 # sample size in bytes, 2 = int16 -> max 32676
CHANNELS = 2 # number of samples in a frame
AUDIORATE = 44100   #audiorate in
FRAMESPERBUFFER = 1028     #Frames per buffer TODO SHOULD - Test optimal value

callbackCount = 0   #a count mainly used to shut the program down for debugging
outputDevice=3      #selected outputDevice TODO MUST - GUI of input afhankelijk maken
multipleOscillators = 0     #selects the multiple oscillator mode
guiUpdateSpeed = 5000      #updatespeed for the GUI output to the Synth input.    TODO COULD - Test optimal value
guiUpdateCount = 0    #update count voor de guiUpdateSpeed

# Create array
masterOutputBuffer = [] #this array will contain signed ints in order to hold one sample buffer

# initialize all Synthesizer objects
osc1 = Synthesizer.Oscillator(frequency=300, phase=0, type=Synthesizer.Oscillator.SINE, channels=CHANNELS, rate=AUDIORATE, amp=0.5, pulsewidth=0.5, ratio=1.005, modDepth=0.015, framesPerBuffer=FRAMESPERBUFFER)
adsr1 = Synthesizer.ADSR(att=0.01, dec=1, sus=0, rel=0, audiorate=AUDIORATE, channels=CHANNELS, framesPerBuffer=FRAMESPERBUFFER)
vca1 = Synthesizer.VCA()
print("\nDe Synthesizer objecten zijn geinitializeerd.\n")


# Create the callback function which is called by pyaudio's stream whenever it needs output-data or has input-data
# As we are working with 16-bit integers, the range is from -32768 to 32767
#
def callback(in_data,frame_count,time_info,status): #callback zoals pyAudio deze verwacht,  TODO COULD - document incoming prameters
    global callbackCount, masterOutputBuffer

    # count how many times the callback is called - used for stopping the stream and debugging
    callbackCount += 1
    #if callbackCount > 1: stream.stop_stream();  #de stop: de stream wordt gestopt als de threshold bereikt wordt

    #Synthese doormiddel van de object uit 'Synthesizer'
    vca1.multiply(osc1.Run(), adsr1.Run())      #VCA = ADSR * Osc
    masterOutputBuffer = vca1.get()             #Master = VCA

    return(masterOutputBuffer, pyaudio.paContinue)


    #########################
    # Start of main program #
    #########################

# PyAudio settings and initializing.
paHandle = pyaudio.PyAudio()    #init a Handle
pW = Audio.paAuWrapper(outputDevice)    #init the paAuWrapper
#print("\nAvailable devices: ")  #device check TODO MUST - dit moet via de command line gebeuren voordat de GUI en audio starten.
#pW.showDevices(paHandle)
#print("\n")
# select a device
pW.setOutputDevice(paHandle)    #selects output device
print("Selected device name: ",paHandle.get_device_info_by_index(outputDevice).get('name'))

# GUI settings and initializing
root = Tk()     # create a tkinter window
root.title("Probable Synthesizer")    # set window title
myGui = GUI.callGUI(master=root)  # initialize the GUI Class

# Start PyAudio stream with some given properties. TODO SHOULD - move de stream naar file 'Audio.py'
stream = paHandle.open(format=paHandle.get_format_from_width(WIDTH),
                       channels=CHANNELS,
                       rate=AUDIORATE,
                       frames_per_buffer=FRAMESPERBUFFER,
                       input=False,  # no input
                       output=True,  # only output
                       output_device_index=outputDevice,  # choose output device
                       stream_callback=callback)

# Start de stream.
stream.start_stream()   #TODO Vraag - is dit een thread?

# Start de GUI loop.
while stream.is_active() == True:
    myGui.update()     #update de GUI window
    guiUpdateCount += 1    #A counter for guiUpdateSpeed
    if guiUpdateCount >= guiUpdateSpeed & multipleOscillators == 0:  #runs once per 'guiUpdateSpeed'
        #Update de synthese waardes naar de GUI waardes     #TODO SHOULD - Hebben de if vergelijkingen hier nut qua processing time?
        if osc1.freq != myGui.sliderFreq.get():
            osc1.freq = myGui.sliderFreq.get()
        if osc1.ratio != myGui.sliderRatio.get():
            osc1.ratio = myGui.sliderRatio.get()
        if osc1.type != myGui.sliderType.get():
            osc1.type = myGui.sliderType.get()
        if osc1.modDepth != myGui.sliderModDepth.get()**2.0:
            osc1.modDepth = myGui.sliderModDepth.get() ** 2.0
        if osc1.amp != myGui.sliderAmp.get()**2.0:
            osc1.amp = myGui.sliderAmp.get() ** 2.0
        if osc1.prob.range != myGui.sliderProbRange.get():      #alle probability aangestuurt met een slider.
            osc1.prob.range = myGui.sliderProbRange.get()
            osc1.prob2.range = myGui.sliderProbRange.get() / 10.
            osc1.prob.autoScaleList()
            osc1.prob2.autoScaleList()
        if adsr1.att != myGui.sliderAtt.get():
            adsr1.att = myGui.sliderAtt.get()
        if adsr1.dec != myGui.sliderDec.get():
            adsr1.dec = myGui.sliderDec.get()
        if adsr1.arpTime != myGui.sliderArpTime.get():
            adsr1.arpTime = myGui.sliderArpTime.get()
        if adsr1.adsrDamp != myGui.sliderEnvDamp.get():
            adsr1.adsrDamp = myGui.sliderEnvDamp.get()
        guiUpdateCount = 0  #reset counter

# TODO SHOULD - Clean afsluiten van alle streams, threads en whiles

#NOTES:
# ARRAYS:
# outbuf = array.array('h', [0]) * FRAMESPERBUFFER * CHANNELS # array of signed ints
# outbuftest = np.zeros(FRAMESPERBUFFER * CHANNELS, np.dtype('int16'))
#
# MULTIPLE OSCILLATORS:  #TODO  - Multiple oscillators is heel erg outdated
# if multipleOscillators == 1:
#     oscA = [Synthesizer.Oscillator((i + 1) * 100, 0, Synthesizer.Oscillator.SINE, CHANNELS, AUDIORATE, 0.5 / (i + 1), i * 0.1 + 0.2, 1, 1, FRAMESPERBUFFER) for i in range(3)]
    # if multipleOscillators == 1:
    #     output = oscA[0].Run()
    #     for i in range(1, len(oscA)):
    #         output = np.add(output, oscA[i].Run())
    # else:
#
# UNUSED SYNTHESIS OBJECTS:
# buffer = Synthesizer.Buffer(audiorate=AUDIORATE, channels=CHANNELS, framesperbuffer=FRAMESPERBUFFER)
# lp1 = Synthesizer.AverageFilter(2, 2, CHANNELS)
    # IN DE CALLBACK:
    # lp1.Run(buffer.getOutput(), n) #TODO - filter laten controleren, en up to date brengen. De filter moet gebruik maken van een input lijst en een output lijst.
#
# CALLBACK:
    # return(buffer.getOutput().tobytes(), pyaudio.paContinue)  #return als er met het buffer object gewerkt wordt.
