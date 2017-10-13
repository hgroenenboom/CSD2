# A Fade class used to stop clicks or make fades

class Fade():
	fadeCount = 0
	fadeOn = True
	setValues = True

	#initializes fade values, only works if the fade doesn't have to be initialized in a loop.
	def __init__(self, inputValue, destValue, fadeLength, timePerCall):
		self.outputValue = inputValue
		self.destValue = destValue

		self.nSteps = fadeLength / timePerCall
		self.step = (destValue - inputValue) / self.nSteps

	#UNTESTED, set fades in a loop
	def setFadeOnce(self, inVal, destVal, fadeLengthMs, callTimeMs):
		#this can be used to set the values of a fade once, so that they wont keep changing
		if self.setValues == True:
			self.outputValue = inVal
			self.destValue = destVal

			self.nSteps = fadeLengthMs / callTimeMs
			self.step = (destVal - inVal) / self.nSteps

			self.resetFade()  #reset the fade so it will run again.
			self.setValues = False		#make sure it doesn't rerun

	#TESTED, this function will automaticly fade once with the set input values. Used inside a loop.
		#- fade is only lineair
	def fFade(self): #float, float, float(ms), float(ms)
		if self.fadeOn == True:
			if self.fadeCount < self.nSteps:
				self.fadeCount += 1
				self.outputValue += self.step
				return self.outputValue
			else:
				self.fadeCount = 0
				self.outputValue = self.destValue
				self.fadeOn = False
		return self.outputValue

	#Resets the fade so it can be used again.
	def resetFade(self):
		self.fadeCount = 0
		self.fadeOn = True

	#Get the current output value of the fade
	def getOutputValue(self):
		return self.outputValue
#
# Example:
# fade = Fade(0, 2.3, 3000, 10)
# 	inputValue = 0
#	wantedValue = 2.3
#

#class Smoothen():