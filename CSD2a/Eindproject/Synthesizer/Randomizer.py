#A class which will compute normal distributed random values with a scaled output.

import numpy as np
import random
from statistics import mean

#This class will create a list filled with a normal distribution. This class is used to implement natural randomness in audio.
class NaturalRandomness():
    #Initial values
    nrList = []  #the initial natural random list
    nrScaledList = []    #the scaled natural random list
    outputValue = 1   #output value
    smoothOutputValue = 1   #smoothed output value (average filter)
    functionCallCount = 0   #counts how often the setNewValue function is called
    tempList = []
    for i in range(20):
        tempList.append(i*0.1 + 0.1)

    def __init__(self, center=1, range=0.02, exp=True, amountOfNormalDistributedNumbers=100, refreshWait=150, smoothing=10):

        #scaling values:
        self.center = center    #center value of the normal distribution
        self.range = range  #positive range of the normal distribution from the center value (mirrors in negative range too)
        #UNUSED -  boolean for exponential scaling of the normal distribution, used for manipulating logaritmicly experienced parameters (Hz, dB).
        self.exp = exp  #the exponential toggle for the nrlist. Used for frequencies.

        #amount of elements in the random list:
        self.amountOfNormalDists = amountOfNormalDistributedNumbers     #amount of probability numbers in the list.

        #intializing scheduling variables:
        self.valueRefreshThreshold = refreshWait      #the amount of times to wait before a new value is obtained from the list

        #smoothing / average variables:
        self.smoothing = smoothing  #amount of previous values to average with.
        self.smoothList = np.zeros(self.smoothing)  #a list which will hold the previous outputs.

        #fill the nrList with the normal distribution
        self.fillNRList()
        #make nrList exponentional if (self.exp == True)
        self.makeListExponential(self.nrList)
        #scale list to center en range values
        self.autoScaleList()

    #Fills nrList with a normal distribution.
    def fillNRList(self):
        self.nrList = []
        for i in range(self.amountOfNormalDists):
            self.nrList.append(np.random.normal(1, 0.5))   # (values from 0 to 2; center value being 1)
        return self.nrList

    #Makes nrList exponentional if wanted.
    def makeListExponential(self, list): #center value should be 1
        #TODO SHOULD - make the upper range extend till 2, now it's till 4
        #run only if self.exp == True.
        if self.exp == True:
            # clip values under zero
            list = np.clip(list, 0.001, 100.0)
            # calculating an exponential value for every value in the list.
            for i in range(len(list)):
                if list[i] >= 1:    #for the numbers larger then 1: input(1 to 2) -> output (1-4)
                    list[i] = np.square(list[i])    #resulting values: (1 - 4)
                if list[i] < 1:     #for the numbers smaller then 1: input(1 to 0) -> output(
                    list[i] = np.sqrt(list[i])
                    #list[i] = 1 - (np.square(2 - list[i]) - 1) / 4      #newVal = 1 - (exponential(2 - value0to1) - 1) / 3
            list *= (1/3)   #(0 - 0.333 - 1.3333)
            list += (2/3)   #(0.666 - 1 - 2
            return list

    #Creates a scaled list from nrList called nrScaledList by using self.center and self.range
    def autoScaleList(self):    #center of pList should be 1, for wanted results
        #make sure the center of nrList is 0.
        self.nrScaledList = np.add(self.nrList, -1.0)
        #multiply nrList with the range.
        self.nrScaledList = np.multiply(self.nrScaledList, self.range)
        #add the center value to nrList.
        self.nrScaledList = np.add(self.nrScaledList, self.center)
        return self.nrList

    #this function is an average filter with 'smoothing' amount of input values. It smoothens the output values
    def smoothenValue(self):
        #delete oldest value from the smoothList
        self.smoothList = np.delete(self.smoothList, self.smoothing - 1)
        #insert new output value in the smoothList
        self.smoothList = np.insert(self.smoothList, 0, self.outputValue)
        #compute the average value of the list.
        self.smoothOutputValue = np.mean(self.smoothList)

    #if setNewValue is called, the function will decide if it's time to update to a new output Value
    def setNewValue(self):
        #the count checks how often the function is called
        self.functionCallCount += 1
        #if the amount of functioncalls is bigger then the refreshThreshold:
        if self.functionCallCount > self.valueRefreshThreshold:
            #select new output value
            self.outputValue = self.nrScaledList[random.randint(0, self.amountOfNormalDists - 1)]
            #reset counter
            self.functionCallCount = 0
            #if smoothing is on, call the 'smoothing' function and create a smoothOutputValue, else do nothing
            if self.smoothing > 1:
                self.smoothenValue()
            else:
                self.smoothOutputValue = self.outputValue

    #get outputValue
    def getValue(self):
        return self.outputValue

    #get smoothOutputValue
    def getSmoothValue(self):
        return self.smoothOutputValue

    #get the scaled naturalrandomnessList.
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