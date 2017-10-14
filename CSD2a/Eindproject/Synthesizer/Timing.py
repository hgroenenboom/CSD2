#This class contains a metronome which also has a playtrigger.
#The play trigger can variate from the metronome trigger to create a naturally timed trigger.
#

import time
import Probability

class Metronome():
    pastTime = 0
    currentTime = 0

    #these variables are used to create a time varying trigger.
    chance = Probability.Probability(exp=False, refreshWait=1, range=0.02)
    playTriggerTimes = [0.0, 0.0]  #the time values for the time varying triggers
    pastMetrTimes = [0.0, 0.0]     #saves the pastTime value to be able to compute if the pTrigger should be triggered
    playTrigger = False  #the actual trigger

    metronomeTrigger = False    #the metronome trigger (unused)

    metronomeSet = False #de metronome kan dan gestart worden.
    metronomeCount = 0
    playCount = 0

    def __init__(self, bpm=100, stepsPerBeat=2, beatsPerBar=4):
        #obtain time info
        self.bpm = bpm
        self.stepsPerBeat = stepsPerBeat
        self.beatsPerBar = beatsPerBar

        #compute time values
        self.timePerBeat = 60.0 / bpm
        self.timePerStep = self.timePerBeat / self.stepsPerBeat
        print("timePerStep = ", self.timePerStep)

    def setMetronome(self):
        #create first time values
        self.pastTime = time.time() #pastTime is created

        #set playTrigger values:
        self.pastMetrTimes[0] = self.pastTime #synchronize the start values of the trigger and the metronome
        self.pastMetrTimes[1] = self.pastTime + self.stepsPerBeat
        #first trigger values
        self.playTriggerTimes[0] = self.timePerStep + 0.01
        self.playTriggerTimes[1] = self.timePerStep + 0.01

        #make sure the metronome wont set itself again
        self.metronomeSet = True

    def metronome(self):
        #set the metronome once
        if self.metronomeSet == False:
            print("\nMetronome is set\n")
            self.setMetronome()

        #make the metronome trigger False unless it's made true
        self.metronomeTrigger = False
        self.playTrigger = False

        # retrieve current time
        self.currentTime = time.time()

        #check for the metronome time
        if (self.currentTime - self.pastTime > self.timePerStep):
            self.metronomeTrigger = True
            self.pastTime = self.currentTime
            self.setPTrigger()  #set the variables for the play trigger
            self.metronomeCount += 1


        #check for the playTrigger time
        if (self.currentTime - self.pastMetrTimes[0] > self.playTriggerTimes[0]):
            self.playTrigger = True
            self.playCount += 1

            #delete old values
            self.pastMetrTimes.pop(0)
            self.playTriggerTimes.pop(0)
            # print(" --popped playTriggerTimes = ", self.playTriggerTimes)
            # print(" --popped self.pastMetrTime = ", self.pastMetrTimes)
        return self.metronomeTrigger

    def setPTrigger(self):
        #get a new probability value
        self.chance.setNewValue()
        #add a new trigger time value to the pTriggerTimes list
        self.playTriggerTimes.extend([self.timePerStep * self.chance.getValue()])
        #print("chance value = ", self.chance.getValue())
        #save the pastTime value so it can be used for the trigger
        self.pastMetrTimes.extend([self.timePerStep + self.pastTime])
        # print("\n --set playTriggerTimes = ", self.playTriggerTimes)
        # print(" --set self.pastMetrTime = ", self.pastMetrTimes)


# m = Metronome()

# while True:
#     m.metronome()
#
#     if m.playTrigger == True:
#         print("Trigger ", m.playCount, "time = ", time.time())
#
#     if m.metronomeTrigger == True:
#         #print("Tick", m.metronomeCount, "time = ", time.time())