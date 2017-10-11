# My first coded synthesizer
# Made by: Harold Groenenboom
#
# Based on the basic pyaudio program playing a real time mono sine wave by (ME) 2015 Marc Groenewegen
#
# TODO MUST
# - add stream to 'Audio' subfile
# - think about changing the buffer to [[frame01], [frame02], etc.] with every frame holding 'CHANNELS' channels
# TODO COULD
# - fix filter
# - implement general smoothing and fades

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
updateSpeed = 5000      #updatespeed for the GUI objects    TODO COULD - Test optimal value

# Create array of signed ints to hold one sample buffer
# Make it global so it doesn't get re-allocated for every frame
# THIS IS NOT BEING USED
outbuf = array.array('h', [0]) * FRAMESPERBUFFER * CHANNELS # array of signed ints
outbuftest = np.zeros(FRAMESPERBUFFER * CHANNELS, np.dtype('int16'))
output = outbuf


#
# initialize all Synthesizer objects
if multipleOscillators == 1:
    oscA = [Synthesizer.Oscillator((i + 1) * 100, 0, Synthesizer.Oscillator.SINE, CHANNELS, AUDIORATE, 0.5 / (i + 1), i * 0.1 + 0.2, 1, 1, FRAMESPERBUFFER) for i in range(3)]
else:
    osc1 = Synthesizer.Oscillator(frequency=300, phase=0, type=Synthesizer.Oscillator.SINE, channels=CHANNELS, rate=AUDIORATE, amp=0.5, pulsewidth=0.5, ratio=1.005, modDepth=0.015, framesPerBuffer=FRAMESPERBUFFER)
    #lp1 = Synthesizer.AverageFilter(2, 2, CHANNELS)
    ampEnv1 = Synthesizer.ADSR(att=0.01, dec=1, sus=0, rel=0, audiorate=AUDIORATE, channels=CHANNELS, framesPerBuffer=FRAMESPERBUFFER)
    vca1 = Synthesizer.VCA()
    #buffer = Synthesizer.Buffer(audiorate=AUDIORATE, channels=CHANNELS, framesperbuffer=FRAMESPERBUFFER)
print("De Synthesizer objecten zijn geinitializeerd.\n")


# Create the callback function which is called by pyaudio
#   whenever it needs output-data or has input-data
#
# As we are working with 16-bit integers, the range is from -32768 to 32767
#
def callback(in_data,frame_count,time_info,status): #callback zoals pyAudio deze verwacht,  TODO - document incoming prameters
    global callbackCount, output, multipleOscillators, guiMode

    callbackCount += 1
    #if callbackCount > 2: stream.stop_stream(); guiMode=False  #de countdown: de audio wordt gestopt als de threshold bereikt wordt wanneer deze regel aanstaat

    # if multipleOscillators == 1:       #TODO - Multiple oscillators is heel erg outdated
    #     output = oscA[0].Run()
    #     for i in range(1, len(oscA)):
    #         output = np.add(output, oscA[i].Run())
    # else:

    # lp1.Run(buffer.getOutput(), n) #TODO - filter laten controleren, en up to date brengen. De filter moet gebruik maken van een input lijst en een output lijst.
    vca1.multiply(osc1.Run(), ampEnv1.Run())
    output = vca1.get()

    # return(buffer.getOutput().tobytes(), pyaudio.paContinue)  #return als er met het buffer object gewerkt wordt.
    return(output, pyaudio.paContinue)

    #########################
    # Start of main program #
    #########################


#
# get a handle to the pyaudio interface
#
paHandle = pyaudio.PyAudio()
pW = Audio.paAuWrapper(outputDevice)
#print("\nAvailable devices: ")  #device check
#pW.showDevices(paHandle)
#print("\n")
# select a device
pW.setOutputDevice(paHandle)
print("Selected device name: ",paHandle.get_device_info_by_index(outputDevice).get('name'))

root = Tk()     # create a tkinter window
root.title("ME Gui")    # set window props
myGui = GUI.callGUI(master=root)  # root.geometry("500x500")

#
# open a stream with some given properties
#
stream = paHandle.open(format=paHandle.get_format_from_width(WIDTH),
                       channels=CHANNELS,
                       rate=AUDIORATE,
                       frames_per_buffer=FRAMESPERBUFFER,
                       input=False,  # no input
                       output=True,  # only output
                       output_device_index=outputDevice,  # choose output device
                       stream_callback=callback)

stream.start_stream()

# Make sure that the main program doesn't finish until all
#  audio processing is done

while stream.is_active() == True:
    myGui.update()
    updateSpeed += 1
    if updateSpeed >= 99 & multipleOscillators == 0:
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
        if osc1.prob.range != myGui.sliderProbRange.get():
            osc1.prob.range = myGui.sliderProbRange.get()
            osc1.prob2.range = myGui.sliderProbRange.get() / 10.
            osc1.prob.autoScaleList()
            osc1.prob2.autoScaleList()
        if ampEnv1.att != myGui.sliderAtt.get():
            ampEnv1.att = myGui.sliderAtt.get()
        if ampEnv1.dec != myGui.sliderDec.get():
            ampEnv1.dec = myGui.sliderDec.get()
        if ampEnv1.arpTime != myGui.sliderArpTime.get():
            ampEnv1.arpTime = myGui.sliderArpTime.get()
        if ampEnv1.adsrDamp != myGui.sliderEnvDamp.get():
            ampEnv1.adsrDamp = myGui.sliderEnvDamp.get()
        updateSpeed = 0

