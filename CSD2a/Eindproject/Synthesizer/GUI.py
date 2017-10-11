# Contains the GUI Class with the GUI design.

from tkinter import *

# GUI is een extension van Frame.
# Frame is een systeem class die het mogelijk maakt om windows te maken.
class callGUI(Frame):
    """ A GUI app """
    """ A GUI app """

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
        #self.button = Button(self, text="Button 2")
        #self.button.pack()

        #create all sliders
        self.sliderFreq = self.makeSlider("Main Frequency", 20, 1000, 200)
        self.sliderFreq.pack()
        self.sliderAmp = self.makeSlider("Amplitude", 0, 1, 0.1, 0.001)
        self.sliderAmp.pack()
        self.sliderRatio = self.makeSlider("Modulator Ratio", 0.001, 10.00, 1.02, 0.001)
        self.sliderRatio.pack()
        self.sliderModDepth = self.makeSlider("Modulation Depth (FM/AM)", 0, 0.4, 0.13, 0.001)
        self.sliderModDepth.pack()
        self.sliderType = self.makeSlider("Synthesis Type: Sine, Pulse, FM, White Noise, AM", 1, 5, 1, 1)
        self.sliderType.pack()
        self.sliderAtt = self.makeSlider("Attack of the ADSR", 0, 1, 0.01, 0.0001)
        self.sliderAtt.pack()
        self.sliderDec = self.makeSlider("Decay of the ADSR", 0, 1, 0.4, 0.0001)
        self.sliderDec.pack()
        self.sliderArpTime = (self.makeSlider("Rate of the metronome", 0, 2, 0.5, 0.001))
        self.sliderArpTime.pack()
        self.sliderEnvDamp = self.makeSlider("Damping of the envelope", 1.0, 4.0, 2.0, 0.01)
        self.sliderEnvDamp.pack()
        self.sliderProbRange = self.makeSlider("'Naturalness'", 0.0, 0.12, 0.03, 0.001)
        self.sliderProbRange.pack()

    # A function which makes it easier to create sliders - TODO MUST - Kijk of het nog voldoet aan mijn behoeftes. Zelfde voor andere widgets
    def makeSlider(self, name="slider", frm=0.0, to=1.0, set=0.0, resolution=0.1, len=400, wdth=10, sliderlen=30):
        x = Scale(self) #slider object
        # configureer slider
        x.config(orient=HORIZONTAL, length=len, width=wdth, sliderlength=sliderlen, from_=frm, to_=to, tickinterval=200, resolution=resolution)
        x.set(set)  #Not sure TODO MUST - zoek uit wat dit deed. grid plaatsing?
        x.config(label=name)    #assigning a name to the slider
        return x

# NOTES
    # def callMainLoop(self):
    #     print("yolo")
    #     self.root.mainloop()
