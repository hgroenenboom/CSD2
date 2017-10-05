import simpleaudio as sa
import time
import random
import threading
import SampleLoader

bpm=140
subDivision=4
timeStep= 60.0 / (subDivision*bpm)
print("Seconds per step = ", timeStep)
seqLength = 16
userInput = 0

hat = SampleLoader.Sample("hat", "hat.wav", seqLength)    #test van gebruik Class SampleLoader

#TODO - maak een list die de samples bevat en automatisch de directory afgaat voor .wav files (* .wav), zie hieronder:
#samples = []
#wavFiles = []
#for n in wavFiles:
    #samples[n] = SampleLoader.Sample(wavFiles[n])

kick = sa.WaveObject.from_wave_file("kick2.wav")
snare = sa.WaveObject.from_wave_file("snare.wav")       #maakt WaveObject aan

kickPlay = kick.play()
snarePlay = snare.play()    #maakt een play object aan

kickList= [0] * seqLength
snareList= [0] * seqLength    #maakt een sequencerList aan voor snare

liveKickList = kickList
liveSnareList = snareList   #maakt de live sequencer list aan. Deze wordt gebruikt zodat de sequence om de bar verandert.

def fillList(lst):
    """"deze functie vult de inputlijst met random booleans"""
    for n in range(len(lst)):
        lst[n] = random.getrandbits(1)

fillList(kickList)     #vul de sequencer lijsten met booleans.
fillList(snareList)

def refreshList():      #als deze functie aan wordt geroepen worden de live Sequencer lijsten vervangen door de niet live lijsten
    global liveKickList, liveSnareList
    liveKickList = kickList
    liveSnareList = kickList

def inputSwag():        #de functie die in de thread wordt aangeroepen voor live input
    global kickList
    userInput = input('Command: ')
    while True:
        if userInput == "newKicks":
            fillList(kickList)
            print("New kick list created.")
            userInput = input('Command: ')
        elif userInput == "newSnares":
            fillList(snareList)
            print("New snare list created.")
            userInput = input('Command: ')
        elif userInput == "newHats":
            hat.refillList()
            print("New hat list created.")
            userInput = input('Command: ')
        else:
            print("Unknown Command")
            userInput = input('Command: ')


inputThread = threading.Thread(target=inputSwag)
inputThread.start()

pastTime = time.time()
seqCount = 0

while True:
    # print(t0 - time.time())
    if (time.time() - pastTime > timeStep):
        pastTime = time.time()

        print("\nseqCount = ", timeStep)

        if seqCount == 0:
            refreshList()
            hat.refresh()    #refresh functie call van SampleLoader
        if liveKickList[seqCount] == 1:
            kickPlay = kick.play()
        if liveSnareList[seqCount] == 1:
            snarePlay = snare.play()
        hat.playSequence(seqCount) #playSequence functie call van SampleLoader

        seqCount += 1
        seqCount %= seqLength
    else:
        time.sleep(0.01)