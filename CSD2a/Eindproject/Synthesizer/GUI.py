# Contains the GUI Class with the GUI design.

from tkinter import *

# GUI is een extension van Frame.
# Frame is een systeem class die het mogelijk maakt om windows te maken.
class callGUI(Frame):
    """ A GUI app """
    """ A GUI app """

    gridRowCounter = 0
    playSequence = [] * 8
    pitchSequence = [] * 8

    def __init__(self, master):
        """ Constructor: init the frame """
        # Frame is de ... van callGUI. Hier wordt de Frame subclass? geinit. TODO COULD - snappen wat dit doet.
        Frame.__init__(self, master)

        # This function creates all widgets and orders their graphical orientation
        self.create_widgets()
        # Not sure? Plaatst de widgets op de grid? TODO COULD - snappen wat dit doet.
        self.grid()

    # Create all widgets. De volgorde bepaalt de grafische locatie!! Hou dit in de gaten.
    def create_widgets(self):
        self.text = Label(text="The Probable Arpeggiator", relief=RIDGE, font=("Courier", 44))
        self.text.grid(row=self.gridRowCounter, columnspan=9)
        self.gridRowCounter += 1
        self.text2 = Label(text="The naturalness will add a natural distributed randomness to your sound! Be sure experiment with all the sliders :D"
                          , relief=RIDGE)
        self.text2.grid(row=self.gridRowCounter, columnspan=9)
        self.gridRowCounter += 1

        self.pitchSliders = self.makePitches()
        self.sliderFreq = self.makeSlider("Main Frequency", 20, 500, 150, column=9)
        self.gridRowCounter += 1

        self.sliderModDepth = self.makeSlider("Modulation Depth (FM/AM)", 0, 0.4, 0.13, 0.001, column=9)
        self.gridRowCounter += 1

        self.triggerButtonsValues = []
        self.triggerButtons = self.makeButtons(self.triggerButtonsValues)
        self.sliderRatio = self.makeSlider("Modulator Ratio", 0.001, 10.00, 1.02, 0.001, column=9)
        self.gridRowCounter += 1

        self.sliderBPM = (self.makeSlider("Rate of the metronome", 40, 200, 120, 1, len=400))
        self.sliderEnvDamp = self.makeSlider("Damping of the envelope", 1.0, 4.0, 2.0, 0.01, column=9)
        self.gridRowCounter += 1

        self.sliderType = self.makeSlider("Synthesis Type: Sine, Pulse, FM, White Noise, AM", 1, 5, 1, 1, len=400)
        self.sliderAtt = self.makeSlider("Attack of the ADSR", 0, 1, 0.01, 0.0001, column=9)
        self.gridRowCounter += 1

        self.sliderAmp = self.makeSlider("Amplitude", 0, 1, 0.5, 0.001, len=400)
        self.sliderDec = self.makeSlider("Decay of the ADSR", 0, 1, 0.4, 0.0001, column=9)
        self.gridRowCounter += 1

        self.sliderProbRange = self.makeSlider("'Naturalness'", 0.0, 0.12, 0.02, 0.001, len=400) #command=newText("")


    # A function which makes it easier to create sliders - TODO MUST - Kijk of het nog voldoet aan mijn behoeftes. Zelfde voor andere widgets
    def makeSlider(self, name="slider", frm=0.0, to=1.0, set=0.0, resolution=0.1, len=250, wdth=10, sliderlen=30, columnspan=8, column=0):
        x = Scale(self) #slider object
        # configureer slider
        x.config(orient=HORIZONTAL, length=len, width=wdth, sliderlength=sliderlen,
                 relief=RIDGE, from_=frm, to_=to, tickinterval=200, resolution=resolution)
        x.set(set)  #Not sure TODO MUST - zoek uit wat dit deed. grid plaatsing?
        x.config(label=name)    #assigning a name to the slider

        x.grid(row=self.gridRowCounter, columnspan=columnspan, column=column)
        return x

    def makeButtons(self, variables, amount = 8):
        buttons = []
        for n in range(amount):
            variables.append(IntVar())
            buttons.append(Checkbutton(self, text=str(n+1)
                                       , var = variables[n]
                                       , width = 3
                                       , height = 3
                                       , bd=3
                                       , relief=GROOVE
                                       ))
            buttons[n].toggle()
            buttons[n].grid(row=self.gridRowCounter, column=n)
        return buttons

    def makePitches(self, amount = 8):
        pitches = []
        for n in range(amount):
            temp = Scale(self)
            temp.config(label=n, orient=VERTICAL, showvalue=1, length=120, width=10, sliderlength=20, relief=GROOVE
                        , borderwidth=0, from_=72, to_=60, tickinterval=0, resolution=0.1)
            pitches.append(temp)
            pitches[n].grid(row=self.gridRowCounter, column=n, rowspan=2)
        return pitches

    def updateCheckList(self, list):
        temp = []
        for n in range(len(list)):
            temp.append(list[n].get())
        return temp

    # NOTES
    # def callMainLoop(self):
    #     print("yolo")
    #     self.root.mainloop()
