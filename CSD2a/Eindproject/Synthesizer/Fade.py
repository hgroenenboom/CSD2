# A Fade class used to stop clicks or make fades
# The fade class uses relative time. This means 'time per function' call will be used to make sure
# it will fade during the insert amount of time.
#
# The fade class will calculate how often it has to run to reach the destination time. (nSteps)
# The fade class will then calculate how much it has to add on every run.
# inputValue + (nSteps * step) = destValue

class Fade():
	#The amount of time the fade is called
	fadeCount = 0
	#fadeOn will make sure the fade function only works if it's toggled.
	fadeOn = True
	#setValues is a toggle that will make sure the values will only be set once.
	setValues = True

	#initializes fade values, only works if the fade doesn't have to be initialized in a loop.
	def __init__(self, inputValue, destValue, fadeLength, timePerCall):
		#input value, destination value, length of the fade, time past per function call
		#set out- and input values
		self.outputValue = inputValue	#initiaze outputValue on inputValue
		self.destValue = destValue

		#calculate the amount of steps and the value per step for the fade function.
		self.nSteps = fadeLength / timePerCall
		self.step = (destValue - inputValue) / self.nSteps

	#UNTESTED, set fades in a loop
	def setFadeOnce(self, inVal, destVal, fadeLengthMs, callTimeMs):
		#this can be used to set the values of a fade once, so that they wont keep changing
		if self.setValues == True:
			#initialze output and destination value.
			self.outputValue = inVal	#initialize outputValue with the input Value
			self.destValue = destVal	#init destination value

			# calculate the amount of steps and the value per step for the fade function.
			self.nSteps = fadeLengthMs / callTimeMs		#amount of times the function will have to be called
			self.step = (destVal - inVal) / self.nSteps	#setp value per function call.

			# reset the fade once.
			self.resetFade()  #reset the fade so it will run again.
			self.setValues = False		#make sure it doesn't set values again.

	#TESTED, this function will automaticly fade once with the set input values. Used inside a loop.
		#TODO add exponential fade?
	def runFade(self): #float, float, float(ms), float(ms)
		# if the fade toggle is on, the function will run.
		if self.fadeOn == True:
			#if the count of runFade has not reached the number of times runFade should run, it will run again.
			if self.fadeCount < self.nSteps:
				self.fadeCount += 1
				self.outputValue += self.step
				return self.outputValue
			# else it will end runFade by switching the fadeOn toggle.
			else:
				self.fadeCount = 0	#reset the fade Counter
				self.outputValue = self.destValue	#make sure it reaches the outputValue
				self.fadeOn = False
		return self.outputValue

	#Resets the fade so it can be used again.
	def resetFade(self):
		self.fadeCount = 0
		self.fadeOn = True #make sure runFade can run again.

	#Get the current output value of the fade
	def getOutputValue(self):
		return self.outputValue
#
#NOTES
#
# Example:
# fade = Fade(0, 2.3, 3000, 10)
# 	inputValue = 0
#	wantedValue = 2.3
#

#class Smoothen():