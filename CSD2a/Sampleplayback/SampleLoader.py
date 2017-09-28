import simpleaudio as sa
import random

class Sample():
    sample = 0

    def __init__(self, name="sample", fileLocation="kick2.wav", length=16):
        self.name = name
        self.fileLocation = fileLocation
        self.length = length

        self.initialize()
        self.refillList()

    def initialize(self):
        self.sample = sa.WaveObject.from_wave_file(self.fileLocation)
        self.playSample = self.sample.play()
        self.triggerList = [0]*self.length
        self.liveTriggerList = self.triggerList

    def refresh(self):
        self.liveTriggerList = self.triggerList

    def play(self):
        self.playSample.stop()
        self.playSample = self.sample.play()

    def refillList(self):
        for n in range(len(self.triggerList)):
            self.triggerList[n] = random.getrandbits(1)

    def playSequence(self, n):
        if self.liveTriggerList[n] > 0:
            self.play()