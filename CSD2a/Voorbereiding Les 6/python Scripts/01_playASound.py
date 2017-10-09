#simpleaudio is imported as sa -> shorter name
import simpleaudio as sa

"""
An example project in which three wav files are played after eachother.

------ HANDS-ON TIPS ------
- Answer the following question before running the code:
  Do you expect to hear the samples played simultaniously or after eachother?
  Why?
	Because the wait_done() function only lets the code continue after the playback is done

- Alter the code:
  Play the sounds simultaniously.
  

- Alter the code:
  Ask the user to choice which one of the three samples should be played and
  only the chosen sample.
"""

#load 3 audioFiles
sampleHigh = sa.WaveObject.from_wave_file("Pop.wav")
sampleMid = sa.WaveObject.from_wave_file("Laser1.wav")
sampleLow = sa.WaveObject.from_wave_file("Dog2.wav")

sampleChoice = False

while sampleChoice == False:
	input = input("Which sample do you want to play?\nPop.wav, Laser1 or Dog2\n")
	if input == "sampleHigh" or input == "Pop.wav" or input == "1":
		#play high sample
		sampleHighPlay = sampleHigh.play()
		#wait till sample is done playing
		sampleHighPlay.wait_done()
		sampleChoice = True

	if input == "sampleMid" or input == "Laser1.wav" or input == "2":
		#play mid sample
		sampleMidPlay = sampleMid.play()
		#wait till sample is done playing
		sampleMidPlay.wait_done()
		sampleChoice = True

	if input == "sampleMid" or input == "Laser1.wav" or input == "3":
		#play low sample
		sampleLowPlay = sampleLow.play()
		#wait till sample is done playing
		sampleLowPlay.wait_done()
		sampleChoice = True
