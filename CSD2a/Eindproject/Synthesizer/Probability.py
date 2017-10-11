#A class which will compute normal distributed probabilities in a scaled way.

import numpy as np
import random
from statistics import mean

#This class will creates a normally distributed scaleble list. This class is used to implement natural randomness in audio.
class Probability():
    #Initial values
    pList = []  #the initial probability list
    pScaledList = []    #the scaled probability list
    outputValue = 1   #output value
    smoothValue = 1   #smoothed output value (average filter)
    functionCallCount = 0   #counts how often the setNewValue function is called

    def __init__(self, center=1, range=0.02, exp=True, amountOfNormalDistributedNumbers=100, refreshWait=150, smoothing=10):

        #scaling values:
        self.center = center    #center value of the distribution
        self.range = range  #positive range of the distribution from the center value
        # boolean for exponential scaling of the distribution, used for manipulating logaritmicly experienced parameters (Hz, dB).
        self.exp = exp

        #init list values:
        self.amountOfNormalDists = amountOfNormalDistributedNumbers     #amount of probability numbers in the list.

        #scheduling values:
        self.valueRefreshThreshold = refreshWait      #the amount of times to wait before a new value is obtained from the list

        #smoothing / average values:
        self.smoothing = smoothing  #amount of past values to average with.
        self.smoothList = np.zeros(self.smoothing)

        #fill the pList with the normal distribution
        self.fillProbList()
        #make pList exponentional if (self.exp == True)
        self.makeListExponential()
        #scale list to center en range values
        self.autoScaleList()

    #Fills pList with a normal distribution.
    def fillProbList(self):
        self.pList = []
        for i in range(self.amountOfNormalDists):
            self.pList.append(np.random.normal(1, 0.5))   # (0, 2; center being 1)
        return self.pList

    #Makes pList exponentional if wanted.
    def makeListExponential(self): #center should be 1
        #TODO SHOULD - make the upper range extend till 2, now it's till 4
        if self.exp == True:
            # clip values under zero
            self.pList = np.clip(self.pList, 0.001, 100.0)
            # square of pList
            self.pList = np.square(self.pList)    #resulting values: (0, 4; center being 1)
            return self.pList

    #Creates a scaled list from pList called pScaledList by using self.center and self.range
    def autoScaleList(self):    #center of pList should be 1, for wanted results
        #maak de center van pList 0
        self.pScaledList = np.add(self.pList, -1.0)
        #vermenigvuldig pList met de range
        self.pScaledList = np.multiply(self.pScaledList, self.range)
        #verplaats de center van pList naar aangegeven center
        self.pScaledList = np.add(self.pScaledList , self.center)
        return self.pList

    #this function is an average filter with 'smoothing' amount of input values. It smoothens the output values
    def setSmoothValue(self):
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
        #if the amount of calls is bigger then the refreshThreshold
        if self.functionCallCount > self.valueRefreshThreshold:
            #select new output value
            self.outputValue = self.pScaledList[random.randint(0, self.amountOfNormalDists - 1)]
            #reset counter
            self.functionCallCount = 0
            #if smoothing is on, call the 'smoothing' function, else do nothing
            if self.smoothing > 1:
                self.setSmoothValue()
            else:
                self.smoothValue = self.outputValue

    #get outputValue
    def getValue(self):
        return self.outputValue

    #get the scaled normal distributed list.
    def getScaledList(self):
        return self.pScaledList


#NOTES
#
#    # def scaleList(self, center=1, range=0.02):    #center should be 1
    #     self.pList = np.add(self.pList, -1.0)
    #     self.pList = np.multiply(self.pList, range)
    #     self.pList = np.add(self.pList, center)
    #     return self.pList
#