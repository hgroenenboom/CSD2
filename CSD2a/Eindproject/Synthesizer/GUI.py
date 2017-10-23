# The GUI Class containing the GUI design.

import random
from tkinter import *
from functools import partial   #lets the user input a function + variable as a functionvariable. i.e. partial(Function, Arguments)

# GUI is an extension from Frame.
# Unconfirmed: Frame is a system class which enables the creation of windows.
class callGUI(Frame):
    """ A GUI app """

    #graphical variables
    gridRowCounter = 0  #counts up to make it easy to place widgets in the grid. TODO - Make a more efficient system
    sequenceColors = ["#ff8484", "#fdff70", "#72ff70", "#70fff2"]   #colors per savesSequence

    scale = ["T", "T", "S", "T", "T", "T", "S"]     #logic based tonal scale which will be shuffled at startup
    pitches = [60]    #list which will hold the generated tonal scale

    #initialize the savedSequences list.
    savedSequences=[]
    for i in range(4):
        savedSequences.append([0] * 16)
    selectedSequence = 0    #variable which holds the used savedSequence

    #initialize the trigger & pitch list of the GUI class.
    triggerSequence = [] * 8
    pitchSequence = [] * 8

    def __init__(self, master):
        """ Constructor: init the frame """
        # callGui uses a tkinter window as a master:
        Frame.__init__(self, master)

        # Make a new random tonal scale
        self.createTonalScale()
        # Create all widgets and give them their graphical orientation
        self.create_widgets()
        # Place widgets on the grid     TODO COULD - understand how grid works
        self.grid()
        # create 4 tonal scale based sequences at startup
        for n in range(4):
            self.randomSequence(n)

    # Create all widgets. De volgorde bepaalt de grafische locatie!! Hou dit in de gaten. TODO - Maak een design op papier?
    def create_widgets(self):
        #create title window
        self.text = Label(text="The Natural Arpeggiator", relief=RIDGE, font=("Courier", 44))
        self.text.grid(row=self.gridRowCounter, columnspan=9)
        self.gridRowCounter += 1

        #create subtitle window
        self.text2 = Label(text="The naturalness slider will add the beauty of analog circuits to your sound! Be sure experiment with all the sliders :D"
                           , relief=RIDGE)
        self.text2.grid(row=self.gridRowCounter, columnspan=9)
        self.gridRowCounter += 1

        #create the pitch component of the sequencer and the center frequency slider
        self.pitchSeqSliders = self.makePitchSliders()
        self.sliderFreq = self.makeSlider("Center frequency", 20, 500, 150, column=9)
        self.gridRowCounter += 1

        #create the modulation amount slider for FM synthesis
        self.sliderModDepth = self.makeSlider("Modulation amount (FM)", 0, 0.4, 0.07, 0.001, column=9)
        self.gridRowCounter += 1

        #create the trigger component of the sequencer and the ratio slider for FM & AM synthesis
        self.triggerSeqButtonsValues = []   #variable for the checkbuttons to write to
        self.triggerSeqButtons = self.makeCheckButtons(self.triggerSeqButtonsValues)
        self.sliderRatio = self.makeSlider("Modulator frequency ratio (FM/AM)", 0.001, 3.00, 1.00, 0.005, column=9)
        self.gridRowCounter += 1

        #create the tempo slider and the envelope damping slider
        self.sliderBPM = (self.makeSlider("Beats per minute", 40, 200, 120, 1, len=400))
        self.sliderEnvDamp = self.makeSlider("Damping of the envelope", 1.0, 4.0, 2.0, 0.01, column=9)
        self.gridRowCounter += 1

        #create the synthesis type slider and the envelope attack slider.
        self.sliderType = self.makeSlider("Synthesis Type: Sine, Pulse, FM, White Noise, AM", 1, 5, 1, 1, len=400)
        self.sliderAtt = self.makeSlider("Attack of the envelope", 0, 1, 0.01, 0.0001, column=9)
        self.gridRowCounter += 1

        #create the master volume slider and the envelope decay slider.
        self.sliderAmp = self.makeSlider("Synth volume", 0, 1, 0.5, 0.001, len=400)
        self.sliderDec = self.makeSlider("Decay of the envelope", 0, 1, 0.4, 0.0001, len=400, column=9)
        self.gridRowCounter += 1

        #create the custom naturalness slider.
        self.sliderProbRange = self.makeSlider("Naturalness", 0.0, 0.08, 0.02, 0.001, len=400, showvalue=0, tickinterval=0, sliderlen=50) #command=newText("")
        self.sliderProbRange.config(font=("Courier", 18), borderwidth=2)
        #self.randomizeButtons = self.makeButtons(self.randomSequence, amount=4, columnspan=2, column=9)
        self.gridRowCounter += 1

        #create the savedSequence buttons. These buttons get their corresponding functions as a parameter.
        self.useButtons = self.makeButtons(self.useSequence, amount=4, columnspan=2, column=0, text="use")
        self.saveButtons = self.makeButtons(self.saveSequence, amount=4, columnspan=2, column=9, text="save to")

    # A function which makes it easier to create sliders. Used to create all the individual sliders.
    def makeSlider(self, name="slider", frm=0.0, to=1.0, set=0.0, resolution=0.1, len=400, wdth=10, sliderlen=30, columnspan=8, column=0, tickinterval=200, showvalue=1):
        slider = Scale(self) #create the slider object
        # configuration of the slider
        slider.config(orient=HORIZONTAL, length=len, width=wdth, sliderlength=sliderlen,
                 relief=RIDGE, from_=frm, to_=to, tickinterval=tickinterval
                 , resolution=resolution, showvalue=showvalue)
        slider.set(set)  #create initial value
        slider.config(label=name)    #assigning a name to the slider
        #place the slider on the grid
        slider.grid(row=self.gridRowCounter, columnspan=columnspan, column=column)
        return slider

    #create a list of checkbuttons. Used to create the sequencer toggles.
    def makeCheckButtons(self, variables, amount = 8, columnspan = 1, column=0):
        tempButtons = []    #create an empty temporary tempButtons list
        for n in range(amount):     #generate selected amount of checkbuttons
            #add a integer variable to the variable list.
            variables.append(IntVar())
            #add a checkbutton to tempButtons.
            tempButtons.append(Checkbutton(self, text=str(n+1) #autoname the tempButtons
                                       , var = variables[n]     #assinging the corresponding index of the variable list.
                                       , width = 3, height = 3, bd=3, relief=GROOVE
                                       ))
            #UNUSED - tempButtons[n].toggle()    #toggles checkbuttons on on default
            #place the tempButtons on the grid
            tempButtons[n].grid(row=self.gridRowCounter, column=column + n, columnspan=columnspan)
        return tempButtons

    #create a list of buttons widgets. Used to create the useSequence, saveSequence & randomSequence buttons.
    def makeButtons(self, function, amount=4, columnspan=2, column=9, text="but"):
        tempButtons = []    #create an empty temporary tempButtons list
        for n in range(amount):     #generate selected amount of buttons
            #add a button to tempButtons.
            tempButtons.append(Button(self, text=text + " " + str(n+1)  #autoname the tempButtons
                                    # giving the button a function with the index as a variable.
                                    # The imported partial function allows the user to use functions with variables as a functionvariable.
                                  , command=partial(function, n)
                                  , width = 10
                                  , height = 3
                                  , bd=3
                                  , relief=GROOVE
                                  , background=self.sequenceColors[n]   #give every button their corresponding colour.
                                  ))
            #place the buttons on a grid
            tempButtons[n].grid(row=self.gridRowCounter, column=column + n * columnspan, columnspan=columnspan)
        return tempButtons

    #create a list of vertical slider widgets. Used to create the pitch sliders for the sequencer
    def makePitchSliders(self, amount = 8):
        #create empty temporary pitches list.
        tempPitches = []
        for n in range(amount): #add selected amount of scales to the tempPitches list.
            tempSlider = Scale(self)      #create scale
            tempSlider.config(label=n+1, orient=VERTICAL, showvalue=1, length=120, width=10, sliderlength=20, relief=GROOVE
                        , borderwidth=0, from_=72, to_=60, tickinterval=0, resolution=1)    #configure scale
            tempPitches.append(tempSlider)  #add slider to tempPitches
            #place the new slider in the grid.
            tempPitches[n].grid(row=self.gridRowCounter, column=n, rowspan=2)
        return tempPitches

    #create and return a list with values from a GUI widget list. Used to pass the GUI sequencer values to the 'seq' object.
    def updateCheckList(self, list):
        tempList = []   #create empty temporary list.
        for n in range(len(list)):      #loop for the amount of widgets in the input list
            tempList.append(list[n].get())  #insert values of the widgets into the tempList
        return tempList

    #UNUSED - create a randomSequence for the selected savedSequence
    def randomSequence(self, index=0):  #the index specifies the selected savedSequence used.
        #Generate trigger values and add them to the specified savedSequence.
        for n in range(0, 8):
            self.savedSequences[index][n] = random.randint(0,1)     #create a random trigger value.
        #Generate pitch values and add them to the specified savedSequence.
        for n in range(8, 16):
            self.savedSequences[index][n] = self.pitches[random.randint(0,6)]   #get a random pitch from the generated tonal scale.

    #use the selected savedSequence in the sequencer. Used to toggle created sequences in the GUI.
    def useSequence(self, index=0):
        # change gui background colour and pass through the new selected sequence
        self.selectedSequence = index
        self.config(background=self.sequenceColors[index])
        #read trigger values and pass values to the trigger checkbuttons
        for n in range(0, 8):
            if self.savedSequences[index][n] == 1:
                self.triggerSeqButtons[n].select()
            else:
                self.triggerSeqButtons[n].deselect()
        #read pitch values and pass values to the pitch sliders
        for n in range(8, 16):
            self.pitchSeqSliders[n - 8].set(self.savedSequences[index][n])

    #save the sequencer pitches and toggles to one of the savedSequences. Used to save created sequences in the GUI.
    def saveSequence(self, index=0):
        #read the playSequence variable and pass the values through to the corresponding savedSequence.
        for n in range(0, 8):
            if self.triggerSequence[n] == 1:
                self.savedSequences[index][n] = 1
            else:
                self.savedSequences[index][n] = 0
        #read the pitchSequence variable and pass the values through to the corresponding savedSequence.
        for n in range(8, 16):
            self.savedSequences[index][n] = self.pitchSequence[n-8]

    #create a one octave midi list called pitches based on the TTSTTTS scale. Used to generate a tonal scale at initialization.
    def createTonalScale(self):
        cut = random.randint(0,5)                           #create cut point
        self.pitches = [60]                                 #initiate pitches
        self.scale = self.scale[cut:7]+self.scale[0:cut]    #create scale list with cut
        for n in range(len(self.scale)):                    #fill pitches with used scale in a forloop:
            if self.scale[n] == "T":                            #if the element in scale is "T":
                self.pitches.extend([self.pitches[n]+2])            #add 'last element from pitches + 2' to pitches
            elif self.scale[n] == "S":                          #if the element in scale is "S":
                self.pitches.extend([self.pitches[n]+1])            #add 'last element from pitches + 1' to pitches
        return self.scale

# NOTES
#
# def callMainLoop(self):
#     self.root.mainloop()
