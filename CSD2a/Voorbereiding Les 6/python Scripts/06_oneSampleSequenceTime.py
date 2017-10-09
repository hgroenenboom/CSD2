import simpleaudio as sa
import time
import random

"""
An example project in which a sequence (one measure, 1 sample) is played.
  - Sixteenth note is the smallest used note duration.
  - One meassure, time signature: 3 / 4

Instead of using steps to iterate through a sequence, we are checking the time.
We will trigger events based on their eventtime.

------ HANDS-ON TIPS ------
- Run the code, read the code and answer the following question:
  - This script transforms a list of sixteenth notes into a list with timeEvents.
    In the playback loop, the current time is compared to the next timeEvent.
    Why is this a more accurate method then the methods used in the examples
    "04_randomNoteDuration.py" and "05_oneSampleSequenceSteps.py"?
		The sleep will only make the code stop for n amount of time, neglecting how long the code takes to run.
		If the code takes long to run, the sleep will take longer then intended.

- Alter the code:
  Currently one sample is played. Add another sample to the script.
  Each time when a sample needs to be played, choice one of the two samples
  randomly.
"""

#load 1 audioFile and store it into a list
#note: using a list taking the next step into account: using multiple samples
samples = [sa.WaveObject.from_wave_file("../audioFiles/Dog2.wav"), sa.WaveObject.from_wave_file("../audioFiles/Laser1.wav")]

#set bpm
bpm = 120
#calculate the duration of a quarter note
quarterNoteDuration = 60 / bpm
#calculate the duration of a sixteenth note
sixteenthNoteDuration = quarterNoteDuration / 4.0

#create a list to hold the events
timeEvents = []
#create a list with the moments (in 16th) at which we should play the sample
sixteenthNoteSequence = [0, 2, 4, 8, 11]

#transform the sixteenthNoteSequence to an eventlist with time values
for sixteenNoteIndex in sixteenthNoteSequence:
  timeEvents.append(sixteenNoteIndex * sixteenthNoteDuration)

#retrieve first event
#NOTE: pop(0) returns and removes the element at index 0
timeEvent = timeEvents.pop(0)
#retrieve the startime: current time
startTime = time.time()
keepPlaying = True
#play the sequence
while keepPlaying:
	#retrieve current time
	currentTime = time.time()
	#check if the timeEvent's time is passed
	if(currentTime - startTime >= timeEvent):
		#play sample
		samples[random.randint(0, len(samples) - 1)].play()

		#if there are events left in the timeEvents list
		if timeEvents:
			#retrieve the next event
			timeEvent = timeEvents.pop(0)
		else:
			#list is empty, stop loop
			keepPlaying = False
	else:
		#wait for a very short moment
		time.sleep(0.001)
