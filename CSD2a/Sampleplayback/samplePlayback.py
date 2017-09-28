import simpleaudio as sa
import time
import random
import threading
import SampleLoader

bpm=140
subDivision=4
timeStep= 60.0 / (subDivision*bpm)
print(timeStep)
listLength = 16
userInput = 0

hat = SampleLoader.Sample("hat", "hat.wav", listLength)

kick = sa.WaveObject.from_wave_file("kick2.wav")
snare = sa.WaveObject.from_wave_file("snare.wav")

kickPlay = kick.play()
snarePlay = snare.play()

kickList=[0]*listLength
snareList=[0]*listLength

liveKickList = kickList
liveSnareList = snareList

def fillList(lst):
    for n in range(len(lst)):
        lst[n] = random.getrandbits(1)

fillList(kickList)
fillList(snareList)

def refreshList():
    global liveKickList, liveSnareList
    liveKickList = kickList
    liveSnareList = kickList

def inputSwag():
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


t = threading.Thread(target=inputSwag)
t.start()


while True:
    for n in range(listLength):
        if n == 0:
            refreshList()
            hat.refresh()
        time.sleep(timeStep)
        if liveKickList[n] > 0:
            kickPlay.stop()
            kickPlay = kick.play()
        if liveSnareList[n] > 0:
            snarePlay.stop()
            snarePlay = snare.play()
        hat.playSequence(n)