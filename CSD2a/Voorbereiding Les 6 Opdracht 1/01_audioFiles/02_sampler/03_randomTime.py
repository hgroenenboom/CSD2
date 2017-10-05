import simpleaudio as sa
import time
import random

"""
An example project in which three wav files are played after eachother with a
break in between of a random duration.
Used durations are: 0.125, 0.25 and 0.5 seconds

------ HANDS-ON TIPS ------
- Alter the code:
  Add a noteDurations list, with the numbers 0.25, 0.5, 1.0. These values stand
  for a sixteenth, eighth and quarter note.
  Add a bpm value to the project and calculate the corresponding timeIntervals
  accordingly. Add these values to the timeIntervals list, instead of its
  current values.

- Alter the code:
  Write a function around the playback forloop, which takes two arguments:
  - a list with samples
  - a list with timeIntervals
  Use this function.
"""

bpm = 120.0
timeFactor = 60.0 / bpm

#load 3 audioFiles into a list
samples = [sa.WaveObject.from_wave_file("../audioFiles/Pop.wav"),
              sa.WaveObject.from_wave_file("../audioFiles/Laser1.wav"),
              sa.WaveObject.from_wave_file("../audioFiles/Dog2.wav")]

#create a list to hold the timeIntervals 0.25, 0.5, 1.0
timeIntervals = [0.25, 0.5, 1]
timeIntervals = [i * timeFactor for i in timeIntervals]
print(timeIntervals)

def playSamples(sampleList, intervalList):
	#play samples and wait in between (random duration)
	for sample in sampleList:
	  #display the sample object
	  print(sample)
	  #play sample
	  sample.play()
	  #retrieve a random index value -> 0 till 2
	  randomIndex = random.randint(0, 2)
	  #dislay the selected timeInterval
	  print("waiting: " + str(intervalList[randomIndex]) + " seconds.")
	  #wait!
	  time.sleep(intervalList[randomIndex])

playSamples(samples, timeIntervals)
