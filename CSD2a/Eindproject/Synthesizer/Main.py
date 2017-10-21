# The probable synthesizer
# Made by: Harold Groenenboom
#
# General info: Notes are in the bottom of Main. Made to be expandable to polyphony, or midi input
#
# Based on the basic pyaudio program playing a real time mono sine wave by (ME) 2015 Marc Groenewegen & Ciska Vriezinga
#

# import libraries
import pyaudio
from tkinter import *    #GUI

# import classes
import GUI              #GUI Class - Tkinter
import Synthesizer         #Synthese classes - Osc, Env, Vca, LP
import Audio            #PyAudio & Stream - TODO MUST - stream toevoegen - Lukt niet
import Sequencer        #Sequencer class - Sequencer

#de full caps dingetjes zijn constanten. Je maakt iets dus full caps als je aan wilt geven dat dit nooit runtime verandert.
#Audio settings
WIDTH = 2 # sample size in bytes, 2 = int16 -> max 32676, KAN NIET AAGEPAST WORDEN
CHANNELS = 2 # number of samples in a frame
AUDIORATE = 44100   #audiorate in
FRAMESPERBUFFER = 256     #Frames per buffer TODO SHOULD - Test optimal value

# runtime variables
callbackCount = 0   #a count mainly used to shut the program down for debugging
outputDevice=3      #selected outputDevice TODO MUST - GUI- of input afhankelijk maken
multipleOscillators = 0     #selects the multiple oscillator mode  - NOT USED
guiUpdateSpeed = 200000      #updatespeed for the GUI output to the Synth input.    TODO COULD - Test optimal value
guiUpdateCount = 0    #update count voor de guiUpdateSpeed
masterAmp = 1   #master amplitude - NOT USED
masterOutputBuffer = [] #this array will contain signed ints in order to hold one sample buffer

# Create sequencer
seq = Sequencer.Sequencer()

# Initialize all Synthesizer objects:
# Oscillator object
osc1 = Synthesizer.Oscillator(frequency=300, phase=0, type=Synthesizer.Oscillator.SINE, amp=0.5, pulsewidth=0.5, ratio=1.005
                              , modDepth=0.015, framesPerBuffer=FRAMESPERBUFFER, channels=CHANNELS, rate=AUDIORATE)
osc1.setListener(seq)   #make sure the oscillator receives the frequencies from the sequencer
# ADSR object
adsr1 = Synthesizer.ADSR(att=0.01, dec=1, sus=0, rel=0, audiorate=AUDIORATE, channels=CHANNELS, framesPerBuffer=FRAMESPERBUFFER)
adsr1.setListener(seq)  #the metronome which triggers the adsr
# VCA object (which basicly multiplies lists
vca1 = Synthesizer.VCA()
print("\nDe Synthesizer objecten zijn geinitializeerd.\n")

# Create GUI
root = Tk()  # create a tkinter window
root.title("Probable Synthesizer")  # set window title
myGui = GUI.callGUI(master=root)  # initialize the GUI Class

# Create the callback function which is called by pyaudio's stream whenever it needs output-data or has input-data
# As we are working with 16-bit integers, the range is from -32768 to 32767
#
def callback(in_data,frame_count,time_info,status): #callback zoals pyAudio deze verwacht,  TODO COULD - document incoming prameters
    global callbackCount, masterOutputBuffer

    # count how many times the callback is called - used for stopping the stream and debugging
    callbackCount += 1
    #if callbackCount > 1: stream.stop_stream();  #de stop: de stream wordt gestopt als de threshold bereikt wordt

    #The actual synthesis. Used objects: osc1, adsr1, vca1
    vca1.multiply(osc1.Run(), adsr1.Run())      #VCA = ADSR * Osc
    masterOutputBuffer = vca1.get()             #Master = VCA
    masterOutputBuffer *= masterAmp             #init with no output

    #callback output: buffer, pyAudio's paContinue function
    return(masterOutputBuffer, pyaudio.paContinue)


    #########################
    # Start of main program #
    #########################

# PyAudio settings and initializing.
pW = Audio.paAuWrapper(outputDevice)    #init the paAuWrapper

#select device via console input
#print available devices
print("\nAvailable devices: ", pW.handle.get_host_api_info_by_index(0).get('deviceCount'), "\n")  #device check
pW.showDevices(pW.handle)
#input loop
while True:
    #userinput
    userInput = input("\nChoose your audio device: ")
    #try if user input is an integer
    try:
        userInput = int(userInput)
        #if userInput is in range of the available devices, change outputDevice to userInput and break the while loop.
        if userInput > 0 and userInput < pW.handle.get_host_api_info_by_index(0).get('deviceCount'):
            outputDevice = userInput
            break
        else:
            print("Error: Integer out of range - Try again.")
    except:
        print("Error: Input must be an integer - Try again.")

#set output device
pW.setOutputDevice(pW.handle)    #selects output device
print("Selected device name: ",pW.handle.get_device_info_by_index(outputDevice).get('name'))

# Open PyAudio stream with some given properties. TODO SHOULD - move de stream naar file 'Audio.py' - Lukt niet
stream = pW.handle.open(format=pW.handle.get_format_from_width(WIDTH),
                       channels=CHANNELS,
                       rate=AUDIORATE,
                       frames_per_buffer=FRAMESPERBUFFER,
                       input=False,  # no input
                       output=True,  # only output
                       output_device_index=outputDevice,  # choose output device
                       stream_callback=callback)

# Start stream.
stream.start_stream()   #TODO Vraag - is dit een thread?

# Start main loop.
while stream.is_active() == True:
    # run the sequencer & metronome
    seq.Run()
    # update de GUI window
    myGui.update()
    # count the amount of times the while loop has looped
    guiUpdateCount += 1
    if guiUpdateCount >= guiUpdateSpeed & multipleOscillators == 0:  #runs once per 'guiUpdateSpeed'
        #get oscillator values
        osc1.centerFreq = myGui.sliderFreq.get()
        osc1.ratio = myGui.sliderRatio.get()
        osc1.type = myGui.sliderType.get()
        osc1.modDepth = myGui.sliderModDepth.get() ** 2.0
        osc1.amp = myGui.sliderAmp.get() ** 2.0

        #get probability values from the naturalness slider
        osc1.probHz.range = myGui.sliderProbRange.get() / 2.
        osc1.probO.range = myGui.sliderProbRange.get() / 2.
        osc1.probAmp.range = myGui.sliderProbRange.get() / 5.
        seq.met.chance.range = myGui.sliderProbRange.get()
        osc1.probHz.autoScaleList()
        osc1.probO.autoScaleList()
        osc1.probAmp.autoScaleList()
        seq.met.chance.autoScaleList()

        #get ADSR values
        adsr1.att = myGui.sliderAtt.get()
        adsr1.dec = myGui.sliderDec.get()
        adsr1.adsrDamp = myGui.sliderEnvDamp.get()

        #get BPM value
        seq.met.setBPM(myGui.sliderBPM.get())

        #get Sequencer values
        myGui.playSequence = myGui.updateCheckList(myGui.triggerButtonsValues)
        seq.playSequence = myGui.playSequence
        myGui.pitchSequence = myGui.updateCheckList(myGui.pitchSliders)
        seq.pitchSequence = myGui.pitchSequence

        #reset GUI update counter
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
