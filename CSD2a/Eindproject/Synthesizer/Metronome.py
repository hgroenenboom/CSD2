#This class contains a metronome with a metronomeTrigger and a musical freeTrigger.
#The freeTrigger variates from the metronomeTrigger to become more natural and less static.
#

import time
import Randomizer   #Used to calculate the freeTrigger timing.

#

class Metronome():
    pastTime = 0
    currentTime = 0

    #these variables are used to create a time varying trigger.
    chance = Randomizer.NaturalRandomness(exp=False, refreshWait=1, range=0.01)
    freeTriggerTimes = [0.0, 0.0]  #the time values for the time varying triggers
    pastMetronomeTimes = [0.0, 0.0]     #saves the pastTime value to be able to compute if the pTrigger should be triggered
    playCount = 0

    freeTrigger = False  #the freeTrigger
    metronomeTrigger = False #UNUSED - the metronomeTrigger

    metronomeSet = False #The initialization toggle of the metronome.
    metronomeCount = 0  #A counter for the amount of metronomeTriggers

    def __init__(self, bpm=100, stepsPerBeat=2, beatsPerBar=4):
        #obtain time info
        self.bpm = bpm
        self.stepsPerBeat = stepsPerBeat
        self.beatsPerBar = beatsPerBar

        #compute time values
        self.timePerBeat = 60.0 / bpm
        self.timePerStep = self.timePerBeat / self.stepsPerBeat

    #Set metronome values: Used to set the class variables once, when the metronome function is called
    def setMetronome(self):
        #save the first time values
        self.pastTime = time.time() #pastTime is initialized

        # The freeTrigger uses a bufferlist which holds the next and the current triggerValues:
        # Otherwise theres a chance for the list to be empty when a new trigger is called.
        #set first 2 metronome pastTime values:
        self.pastMetronomeTimes[0] = self.pastTime #synchronize the start values of the trigger and the metronome
        self.pastMetronomeTimes[1] = self.pastTime + self.stepsPerBeat
        #set first 2 freeTrigger values:
        self.freeTriggerTimes[0] = self.timePerStep + 0.01
        self.freeTriggerTimes[1] = self.timePerStep + 0.01

        #make sure the metronome wont set itself again
        self.metronomeSet = True

    #metronome() should be called in a while loop. Used by the sequencer class.
    def metronome(self):
        #set the metronome once, once metronome() is called.
        if self.metronomeSet == False:
            print("\nMetronome is set\n")
            self.setMetronome()

        #make both triggers False unless it's made true
        self.metronomeTrigger = False
        self.freeTrigger = False

        # retrieve current time
        self.currentTime = time.time()

        #Check if the metronomeTrigger should be triggered.
        if (self.currentTime - self.pastTime > self.timePerStep):
            self.metronomeTrigger = True    #set metronomeTrigger to true.
            self.pastTime = self.currentTime    #create the new pastTime value
            self.computeNewTime()   #recompute BPM. Used if the metronome received a new bpm value by the GUI.
            self.setFreeTriggerBuffer()  #set the bufferLists for the freeTrigger
            self.metronomeCount += 1    #the metronome's counter.

        #Check if the freeTrigger should be triggered.
        #The freeTrigger always triggers with one trigger delay compared to the corresponding metronomeTrigger.
        if (self.currentTime - self.pastMetronomeTimes[0] > self.freeTriggerTimes[0]):
            self.freeTrigger = True     #set freeTrigger to be true.
            self.playCount += 1     #the freeTrigger's counter.
            #delete current values from the freeTrigger's bufferLists.
            self.pastMetronomeTimes.pop(0)
            self.freeTriggerTimes.pop(0)
        return self.metronomeTrigger

    # Add the new pastTime and triggerTime values for the freeTrigger.
    def setFreeTriggerBuffer(self):
        #get a new probability value
        self.chance.setNewValue()
        #add a new trigger time value to the pTriggerTimes list
        self.freeTriggerTimes.extend([self.timePerStep * self.chance.getValue()])
        #save the pastTime value so it can be used for the trigger
        self.pastMetronomeTimes.extend([self.timePerStep + self.pastTime])

    # Compute the time values of the metronome
    def computeNewTime(self):
        #compute time values
        self.timePerBeat = 60.0 / self.bpm
        self.timePerStep = self.timePerBeat / self.stepsPerBeat

    # Change the bpm value
    def setBPM(self, bpm):
        self.bpm = bpm

    # get the current freeTrigger value (True/False). Used by the sequencer and turned off by the ADSR object.
    def getTrigger(self):
        return self.freeTrigger

###############
# NOTES.
# m = Metronome()

# while True:
#     m.metronome()
#
#     if m.freeTrigger == True:
#         print("Trigger ", m.playCount, "time = ", time.time())
#
#     if m.metronomeTrigger == True:
#         #print("Tick", m.metronomeCount, "time = ", time.time())