#
#
import Metronome
import Probability

class Sequencer():
    met = Metronome.Metronome()
    playChance = Probability.Probability(exp=False, refreshWait=1, range=0.01)

    #pitchStrings = ['c', 'd', 'e', 'f', 'g', 'a', 'b', 'cis', 'dis', 'eis', 'fis', 'gis', 'ais', 'bis', 'ces', 'des', 'es', 'fes', 'ges', 'as', 'bes']
    #pitchInts = [0,2,4,5,7,9,11]

    playSequence = [1, 0, 0, 1]
    pitchSequence = [60, 60, 60 ,60]

    plays = playSequence
    pitches = pitchSequence
    play = 0
    pitch = 0
    frequency = 0

    sequencerCounter = 0

    def __init__(self, bpm = 120, playList = [1, 1, 1, 1, 0, 1, 0, 0], pitchList = [60] * 8, randomness = 1):
        self.playSequence = playList
        self.pitchSequence = pitchList

        self.plays = self.playSequence
        self.pitches = self.pitchSequence

        self.playChance.range = randomness

    def Run(self):  #should be called in a while loop.
        self.met.metronome()    #keep the metronome running

        if self.met.getTrigger() == True:    #runs if theres a timed trigger
            self.play = self.plays[self.sequencerCounter]
            self.pitch = self.pitches[self.sequencerCounter]
            self.frequency = pow(2, ((self.pitch - 60) / 12))
            #print(self.frequency)

            self.sequencerCounter += 1
            #print(self.play)

            if self.sequencerCounter >= len(self.plays):    #reset sequencer
                self.plays = self.playSequence
                self.pitches = self.pitchSequence
                self.sequencerCounter = 0
        return self.play

    def getPlay(self):
        return self.play

    def getPitch(self):
        return self.frequency