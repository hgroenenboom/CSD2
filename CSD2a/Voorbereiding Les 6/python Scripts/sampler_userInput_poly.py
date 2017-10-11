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

samplesPerPoly = 8  #define the amount of polyphonic samples
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

events = []  #create a list to hold the events
#create lists with the moments (in 16th) at which we should play the samples
sequence1 = [2, 6, 10, 14]	#hats
#[0, 2, 4, 8, 12, 14]
sequence2 = [8, 16]	#kicks
sequence3 = [4, 12]		#snare
	

def polySample(list, amount, path):		#maakt een poly sample list van een ingevoerde sample path
	for i in range(amount):
		wavObj = sa.WaveObject.from_wave_file(path)
		list.append(wavObj)
	return list

#maak alle sample lijsten aan met gebruik van polySample
hatList = []
kickList = []
snareList = []
hatList = polySample(hatList, samplesPerPoly, "../audioFiles/hat.wav")
kickList = polySample(kickList, samplesPerPoly, "../audioFiles/kick2.wav")
snareList = polySample(snareList, samplesPerPoly, "../audioFiles/snare.wav")

samples = [hatList, kickList, snareList]	#een sample lijst, bestaande uit polySample lijsten [[h, h], [k, k], [s,s]]
indexList = len(samples) * [0] #een lijst die voor de playPolySample bijhoudt wat de laatst gespeeld sample is.
			
#deze functie wordt aangeroepen in de afspeel loop. De indexList variabelle	controleert welke van de polysamples
#wordt afgespeeld. 
def playPolySample(sampleList, sample):	
	global indexList
	indexList[sample] += 1
	indexList[sample] %= len(sampleList[0])
	sampleList[sample][indexList[sample]].play()
			
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
	global bpm, changebpm
	print("the default BPM: ", bpm)
	if changebpm == False:
		userInput = input('would you like to change the bpm? (y/n) \n')
		if userInput == "y":
			while True:
				try:
					bpmInput = int(input('new BPM (number) = '))
					if bpmInput > 250 or bpmInput < 20:
						bpmInput = bpm
						print("\nError: Input value out of range (20-250), resetting...")
						changeBPM()
					break
				except ValueError:
					print("\nError: input is not a number, try again")
			setBPM(bpmInput)
			return
		if userInput == "n":
			return
		else: 
			print("\n'", userInput, "' is an unknown command, resetting...")
			changeBPM()	
			
changeBPM()

#transform the sixteenthNoteSequece to an eventlist with time values
def eventToTime(list, eventIndex):
	global events
	for sixteenNoteIndex in list:
		events.append([sixteenNoteIndex * sixteenthNoteDuration, eventIndex])

keepPlaying = True

def fillLoop():
	global events, event
	events = []
	eventToTime(sequence1, 0)	
	eventToTime(sequence2, 1)
	eventToTime(sequence3, 2)
	
	events.sort()
	event = events.pop(0)
	
fillLoop()

#threaded function, will ask for integer bpm input
def liveInputSwag():
	global bpm, keepPlaying
	while True:		#blijft input vragen aan de gebruiker, terwijl hij checkt of de input een integer is.
		bpmInput = input('\nnew BPM (number) = \n')
		try:
			bpmInput = int(bpmInput)
			if bpmInput > 250 or bpmInput < 20:		#constricting input values
				bpmInput = bpm
				print("\nError: input value out of range (20-250)\n		Not changing the bpm value...")
			break
		except ValueError:		#check if input is integer
			# stopping the thread doesn't work yet - TODO: Learn about threading / processes (multiprocess)
			#if (bpmInput == 'q' or bpmInput == 'exit'):
			#	print('\nExiting program...\n')
			#	keepPlaying = False
			print("\nError: input is not a number, try again")
			bpmInput = bpm
	setBPM(bpmInput)	#set the bpm with the 'setBPM' function, if input sufficies
	
	print('\nnew sixtheenth note duration: ', sixteenthNoteDuration)	#	
	liveInputSwag()		#recursion


#Let the user know the program is starting
print('\n||||||||||||||||\nSTARTING PROGRAM\n||||||||||||||||\n')

#live thread, for live bpm input
inputThread = threading.Thread(target=liveInputSwag)
inputThread.start()
	
printCount = 0	

startTime = time.time()

#play the sequence
while keepPlaying:
	#retrieve current time
	currentTime = time.time()
	#check if the event's time (which is at index 0 of event) is passed
	if(currentTime - startTime > event[0]):
		#play sample -> sample index is at index 1 of event
		playPolySample(samples, event[1])
		printCount += 1
		print(printCount, " : ", indexList)
		#if there are events left in the events list
		if events:
			#retrieve the next event
			event = events.pop(0)
		else:
			#list is empty, fill loop
			fillLoop()
			startTime = time.time()
	else:
		#wait for a very short moment
		time.sleep(0.001)