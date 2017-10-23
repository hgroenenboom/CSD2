# A sequencer class which outputs triggers and pitches.
# The sequencer class also starts the master metronome.
# The sequencer is run bij calling the Run() function. This should always be called in a loop without time delays.
#

import Metronome    #Used to start master metronome
import Randomizer   #UNUSED - Used to create a chance for the trigger to be used.

class Sequencer():
    #Initialize metronome
    met = Metronome.Metronome()
    #UNUSED - Initialze triggerchance
    triggerChance = Randomizer.NaturalRandomness(exp=False, refreshWait=1, range=0.01)

    #UNUSED LISTS FOR BACKUP:
    #pitchStrings = ['c', 'd', 'e', 'f', 'g', 'a', 'b', 'cis', 'dis', 'eis', 'fis', 'gis', 'ais', 'bis', 'ces', 'des', 'es', 'fes', 'ges', 'as', 'bes']
    #pitchInts = [60, 62, 64, 65, 67, 69, 71]

    #Individial set sequences for the sequencer
    triggerSequence = [1, 0, 0, 1]
    pitchSequence = [60, 60, 60 ,60]

    #Runtime lists for the sequencer
    triggers = triggerSequence
    pitches = pitchSequence
    #Current outputs of the sequencer
    trigger = 0
    pitch = 0   #midi number
    frequency = 0   #frequency derived from the pitch variable.

    #Counts the steps of the sequencer
    sequencerCounter = 0

    def __init__(self, bpm = 120, triggerList = [1, 1, 1, 1, 0, 1, 0, 0], pitchList =[60] * 8, randomness = 1):
        # Initialize sequences
        self.triggerSequence = triggerList
        self.pitchSequence = pitchList

        # Initialize runtime sequences
        self.triggers = self.triggerSequence
        self.pitches = self.pitchSequence

        # UNUSED - Initialize the triggerChance (0-1)
        self.triggerChance.range = randomness

    #Run the sequencer. Should be a called in a while loop. Used in the main while loop of Main.py
    def Run(self):  #should be called in a while loop.
        self.met.metronome()    #keep the metronome running

        # execute if there's a timed trigger
        if self.met.getTrigger() == True:
            #select the correct trigger value from the runtime trigger sequence.
            self.trigger = self.triggers[self.sequencerCounter]
            #update pitch only if the current note is triggered
            if self.trigger == 1:
                #select the correct pitch value from the runtime pitch sequence.
                self.pitch = self.pitches[self.sequencerCounter]
                self.frequency = pow(2, ((self.pitch - 60) / 12)) #calculate corresponding frequency value

            #give gui visual feedback on the sequencerCounter of the sequencer
            for n in range(len(self.actionListener.triggerSeqButtons)):
                # if the sequencerCounter corresponds to the correct button, make the button yellow.
                if self.sequencerCounter == n:
                    self.actionListener.triggerSeqButtons[n].config(background="yellow")
                # if the sequencerCounter doesn't correspond to the correct button, make the button grey.
                else:
                    self.actionListener.triggerSeqButtons[n].config(background="#afafaf")

            # go to the next sequencer step
            self.sequencerCounter += 1

            # Reset sequencer: Create new runtime sequencer lists and reset sequencerCounter
            if self.sequencerCounter >= len(self.triggers):    #reset sequencer
                self.triggers = self.triggerSequence
                self.pitches = self.pitchSequence
                self.sequencerCounter = 0
        return self.trigger

    # return current trigger value. Used by the adsr1 object in main.
    # The adsr1 object will turn the trigger value off after it has used it.
            # This is done because the timing of the callback and the while loop differs significantly.
    def getTrigger(self):
        return self.trigger

    # return current frequency value. Used by the osc1 object in main.
    def getPitch(self):
        return self.frequency

    # set a actionListener for the sequencer. Used to change the colors of the gui.
    def setListener(self, listener):
        self.actionListener = listener