#
#
import Metronome
import Randomizer

class Sequencer():
    met = Metronome.Metronome()
    triggerChance = Randomizer.NaturalRandomness(exp=False, refreshWait=1, range=0.01)

    #pitchStrings = ['c', 'd', 'e', 'f', 'g', 'a', 'b', 'cis', 'dis', 'eis', 'fis', 'gis', 'ais', 'bis', 'ces', 'des', 'es', 'fes', 'ges', 'as', 'bes']
    #pitchInts = [60, 62, 64, 65, 67, 69, 71]

    triggerSequence = [1, 0, 0, 1]
    pitchSequence = [60, 60, 60 ,60]

    triggers = triggerSequence
    pitches = pitchSequence
    trigger = 0
    pitch = 0
    frequency = 0

    sequencerCounter = 0

    def __init__(self, bpm = 120, triggerList = [1, 1, 1, 1, 0, 1, 0, 0], pitchList =[60] * 8, randomness = 1):
        self.triggerSequence = triggerList
        self.pitchSequence = pitchList

        self.triggers = self.triggerSequence
        self.pitches = self.pitchSequence

        self.triggerChance.range = randomness

    def Run(self):  #should be called in a while loop.
        self.met.metronome()    #keep the metronome running

        if self.met.getTrigger() == True:    #runs if theres a timed trigger
            self.trigger = self.triggers[self.sequencerCounter]
            #update pitch only if the note is played
            if self.trigger == 1:
                self.pitch = self.pitches[self.sequencerCounter]
                self.frequency = pow(2, ((self.pitch - 60) / 12))

            #give gui visual feedback on the timing of the sequencer
            for n in range(len(self.actionListener.triggerSeqButtons)):
                if self.sequencerCounter == n:
                    self.actionListener.triggerSeqButtons[n].config(background="yellow")
                else:
                    self.actionListener.triggerSeqButtons[n].config(background="#afafaf")
            self.sequencerCounter += 1
            #print(self.play)

            if self.sequencerCounter >= len(self.triggers):    #reset sequencer
                self.triggers = self.triggerSequence
                self.pitches = self.pitchSequence
                self.sequencerCounter = 0
        return self.trigger

    def getTrigger(self):
        return self.trigger

    def getPitch(self):
        return self.frequency

    def setListener(self, listener):
        self.actionListener = listener