#A class which will compute normal distributed randomness with scaled output.

import numpy as np
import random
from statistics import mean

#This class will creates a normally distributed scaleble list. This class is used to implement natural randomness in audio.
class NaturalRandomness():
    #Initial values
    nrList = []  #the initial natural random list
    nrScaledList = []    #the scaled natural random list
    outputValue = 1   #output value
    smoothValue = 1   #smoothed output value (average filter)
    functionCallCount = 0   #counts how often the setNewValue function is called

    def __init__(self, center=1, range=0.02, exp=True, amountOfNormalDistributedNumbers=100, refreshWait=150, smoothing=10):

        #scaling values:
        self.center = center    #center value of the normal distribution
        self.range = range  #positive range of the normal distribution from the center value
        #UNUSED -  boolean for exponential scaling of the normal distribution, used for manipulating logaritmicly experienced parameters (Hz, dB).
        self.exp = exp

        #init list values:
        self.amountOfNormalDists = amountOfNormalDistributedNumbers     #amount of probability numbers in the list.

        #scheduling values:
        self.valueRefreshThreshold = refreshWait      #the amount of times to wait before a new value is obtained from the list

        #smoothing / average values:
        self.smoothing = smoothing  #amount of past values to average with.
        self.smoothList = np.zeros(self.smoothing)

        #fill the pList with the normal distribution
        self.fillRNList()
        #make pList exponentional if (self.exp == True)
        self.makeListExponential()
        #scale list to center en range values
        self.autoScaleList()

    #Fills nrList with a normal distribution.
    def fillRNList(self):
        self.nrList = []
        for i in range(self.amountOfNormalDists):
            self.nrList.append(np.random.normal(1, 0.5))   # (0, 2; center being 1)
        return self.nrList

    #Makes nrList exponentional if wanted.
    def makeListExponential(self): #center should be 1
        #TODO SHOULD - make the upper range extend till 2, now it's till 4
        if self.exp == True:
            # clip values under zero
            self.nrList = np.clip(self.nrList, 0.001, 100.0)
            # square of pList
            self.nrList = np.square(self.nrList)    #resulting values: (0, 4; center being 1)
            return self.nrList

    #Creates a scaled list from nrList called nrScaledList by using self.center and self.range
    def autoScaleList(self):    #center of pList should be 1, for wanted results
        #maak de center van pList 0
        self.nrScaledList = np.add(self.nrList, -1.0)
        #vermenigvuldig pList met de range
        self.nrScaledList = np.multiply(self.nrScaledList, self.range)
        #verplaats de center van pList naar aangegeven center
        self.nrScaledList = np.add(self.nrScaledList, self.center)
        return self.nrList

    #this function is an average filter with 'smoothing' amount of input values. It smoothens the output values
    def smoothenValue(self):
        #delete oldest value from value list
        self.smoothList = np.delete(self.smoothList, self.smoothing - 1)
        #insert new output value in value list
        self.smoothList = np.insert(self.smoothList, 0, self.outputValue)
        #average filter
        self.smoothValue = np.mean(self.smoothList)
        #print(self.smoothList)

    #if setNewValue is called, the function will decide if it's time to update to a new output Value
    def setNewValue(self):
        #the count checks how often the function is called
        self.functionCallCount += 1
        #if the amount of times called is bigger then the refreshThreshold:
        if self.functionCallCount > self.valueRefreshThreshold:
            #select new output value
            self.outputValue = self.nrScaledList[random.randint(0, self.amountOfNormalDists - 1)]
            #reset counter
            self.functionCallCount = 0
            #if smoothing is on, call the 'smoothing' function, else do nothing
            if self.smoothing > 1:
                self.smoothenValue()
            else:
                self.smoothValue = self.outputValue

    #get outputValue
    def getValue(self):
        return self.outputValue

    def getSmoothValue(self):
        return self.smoothValue

    #get the scaled normal distributed list.
    def getScaledList(self):
        return self.nrScaledList

#NOTES
#
#    # def scaleList(self, center=1, range=0.02):    #center should be 1
    #     self.pList = np.add(self.pList, -1.0)
    #     self.pList = np.multiply(self.pList, range)
    #     self.pList = np.add(self.pList, center)
    #     return self.pList
#