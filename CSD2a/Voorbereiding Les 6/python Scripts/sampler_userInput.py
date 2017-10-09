import simpleaudio as sa
import time
import random
import threading

"""
An example project in which a sequence (one measure, multiple samples) is played.
  - Sixteenth note is the smallest used note duration.
  - One meassure, time signature: 3 / 4

Instead of using steps to iterate through a sequence, we are checking the time.
We will trigger events based on their eventtime.

------ HANDS-ON TIPS ------
- Answer the following questions before running the code:
  - Line 74 is outcommented. However, this line is essential to enable correct
    playback of the sequence.
    Read the code: what will go wrong?
    Check your answer by running the script with the outcommented line.
  - Remove the [#] at the of the line, what will happen now?
    Check your answer by running the script.

- Add comments:
  A few comments are missing in this script.
  The lines: 63, 67, 71 contain the numbers 0, 1, 2, and are added to the
  timeEvents list. To 'what' do these numbers refer?
  Add meaningfull comments.

- Alter the code:
  Currently the sequence is only played once.
  Alter the code to play it multiple times.
  hint: The events list is emptied using the pop() function. Copy it first?
"""

#______________________________________________________________________________
#NOTE: THIS SCRIPT CONTAINS DUPLICATE CODE = USEFULL EXAMPLE, SEE HANDS-ON TIPS
#______________________________________________________________________________

#load 3 audioFiles and store it into a list
samples = [ sa.WaveObject.from_wave_file("../audioFiles/hat.wav"),
            sa.WaveObject.from_wave_file("../audioFiles/kick2.wav"),
            sa.WaveObject.from_wave_file("../audioFiles/snare.wav"), ]

#set bpm
bpm = 120
#calculate the duration of a quarter note
quarterNoteDuration = 60 / bpm
#calculate the duration of a sixteenth note
sixteenthNoteDuration = quarterNoteDuration / 4.0
#number of beats per sequence (time signature: 3 / 4 = 3 beats per sequence)
beatsPerMeasure = 3
#calculate the duration of a measure
measureDuration = beatsPerMeasure  * quarterNoteDuration
			
def setBPM(inputBpm):

	global bpm, quarterNoteDuration, sixteenthNoteDuration, beatsPerMeasure, measureDuration
	#set bpm
	bpm = inputBpm
	#calculate the duration of a quarter note
	quarterNoteDuration = 60 / bpm
	#calculate the duration of a sixteenth note
	sixteenthNoteDuration = quarterNoteDuration / 4.0
	#number of beats per sequence (time signature: 3 / 4 = 3 beats per sequence)
	beatsPerMeasure = 3
	#calculate the duration of a measure
	measureDuration = beatsPerMeasure  * quarterNoteDuration
setBPM
	
changebpm = False

def changeBPM():
	global bpm
	print("the default BPM: ", bpm)
	if changebpm == False:
		userInput = input('would you like to change the bpm? (y/n) \n')
		if userInput == "y":
			while True:
				try:
					bpmInput = int(input('new BPM (number) = '))
					break
				except ValueError:
					print("\ninput is not a number, try again")
			setBPM(bpmInput)
			return
		if userInput == "n":
			return
		else: 
			print("\n'", userInput, "' is an unknown command, resetting...")
			changeBPM()			
changeBPM()

events = []  #create a list to hold the events
#create lists with the moments (in 16th) at which we should play the samples
sequence1 = []
#[0, 2, 4, 8, 12, 14]
sequence2 = [0, 6, 8]
sequence3 = []

#transform the sixteenthNoteSequece to an eventlist with time values
def eventToTime(list, eventIndex):
	global events
	for sixteenNoteIndex in list:
		events.append([sixteenNoteIndex * sixteenthNoteDuration, eventIndex])

keepPlaying = True

def initLoop():
	global events, event, startTime
	events = []
	eventToTime(sequence1, 0)	
	eventToTime(sequence2, 1)
	eventToTime(sequence3, 2)
	
	events.sort()
	event = events.pop(0)
	
	startTime = time.time()
initLoop()

#threaded function
def liveInputSwag():
	global bpm
	while True:
		try:
			bpmInput = int(input('\nnew BPM (number) = '))
			break
		except ValueError:
			print("\ninput is not a number, try again")
	setBPM(bpmInput)
	
	print(sixteenthNoteDuration)
	liveInputSwag()
	return
	
#live thread
inputThread = threading.Thread(target=liveInputSwag)
inputThread.start()

#play the sequence
while keepPlaying:
	#retrieve current time
	currentTime = time.time()
	#check if the event's time (which is at index 0 of event) is passed
	if(currentTime - startTime >= event[0]):
		#play sample -> sample index is at index 1
		samples[event[1]].play()
		#if there are events left in the events list
		if events:
			#retrieve the next event
			event = events.pop(0)
		else:
			#list is empty, stop loop
			#keepPlaying = False
			initLoop()
	else:
		#wait for a very short moment
		time.sleep(0.0001)
