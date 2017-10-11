#A class which will compute normal distributed probabilities in a scaled way.
import numpy as np
import random
from statistics import mean

class Probability():
    pList = []
    pScaledList = []
    value = 1
    changeValueCount = 0
    smoothValue = 1.0

    def __init__(self, center=1, range=0.02, exp=True, amount=100, refreshRate=150, smoothing=10):
        self.center = center
        self.range = range
        self.exp = exp
        self.amount = amount
        self.refreshRate = refreshRate
        self.smoothing = smoothing
        self.smoothList = np.zeros(self.smoothing)

        self.fillProbList()
        self.makeListExponential()
        self.autoScaleList()

    def fillProbList(self):
        self.pList = []
        for i in range(self.amount):
            self.pList.append(np.random.normal(1, 0.5))   # (0, 2; center being 1)
        return self.pList

    def makeListExponential(self): #center should be 1
        #TODO - make the upper range 2, now it's 4
        if self.exp == True:
            self.pList = np.clip(self.pList, 0.001, 100.0)
            self.pList = np.square(self.pList)                # (0, 4; center being 1)
            return self.pList

    def autoScaleList(self):    #center should be 1
        self.pScaledList = np.add(self.pList, -1.0)     #maak center 0
        self.pScaledList = np.multiply(self.pScaledList, self.range)   #vermenigvuldig met de range
        self.pScaledList = np.add(self.pScaledList , self.center) #verplaats naar juiste center
        return self.pList

    # def scaleList(self, center=1, range=0.02):    #center should be 1
    #     self.pList = np.add(self.pList, -1.0)
    #     self.pList = np.multiply(self.pList, range)
    #     self.pList = np.add(self.pList, center)
    #     return self.pList

    def setValue(self):
        self.changeValueCount += 1
        if self.changeValueCount > self.refreshRate:
            self.value = self.pScaledList[random.randint(0, self.amount - 1)]
            self.changeValueCount = 0
            if self.smoothing > 1:
                self.setSmoothValue()
            else:
                self.smoothValue = self.value

    def getScaledList(self):
        return self.pScaledList

    def getValue(self):
        return self.value

    def setSmoothValue(self):
        self.smoothList = np.delete(self.smoothList, self.smoothing - 1)
        self.smoothList = np.insert(self.smoothList, 0, self.value)
        self.smoothValue = np.mean(self.smoothList)
        #print(self.smoothList)
