''' (c) sinelabore.com '''
''' Command line options: -l PYTHON -p ssc -o oven oven.xml  '''
''' This file is generated from oven.xml - do not edit manually '''
''' Generated on: Sat Oct 28 20:14:25 CEST 2023 / Version 6.1.3706'''


from oven_base import *

class oven(oven_base):

	def __init__(self, errorhandler=None):

		''' Set state vars to default states '''
		self.stateVar = "Super" # set init state of top state 
		self.stateVarSuper = "Idle" # set init state of Super


		''' call init of base class '''
		super().__init__()

		''' call on entry code of default states '''
		self.radiator.ovenOff();
		


		self.events = {
			'evPwrLow',
			'evTimeout',
			'evDec',
			'evDoorOpen',
			'evDoorClosed',
			'evPwrHigh',
			'evInc',
			'OVEN_NO_MSG'
		}

		self.eventConsumed=False

		if errorhandler is None:
			self.ehlr=self.errorhandler
		else:
			self.ehlr=errorhandler


	def errorhandler(self, msg):
		print ("Error:"  + msg)


	def processEvent(self,  event):

		self.eventConsumed=False

		

		# action code
		# just a comment


	

		if self.stateVar == "Super": 

			

			if self.stateVarSuper == "Completed": 
				if event=="evDoorOpen":
					''' Transition from Completed to Idle'''
					self.eventConsumed=1

					''' OnEntry code of state Idle'''
					self.radiator.ovenOff();

					''' adjust state variables '''
					self.stateVarSuper = "Idle";
				else:
					pass  #Intentionally left blank

			

			elif self.stateVarSuper == "Cooking": 
				if event=="evDoorOpen":
					''' Transition from Cooking to CookingPause'''
					self.eventConsumed=1

					''' Action code for transition '''
					self.radiator.ovenOff();
					self.tm.pause();


					''' adjust state variables '''
					self.stateVarSuper = "CookingPause";
				elif event=="evTimeout":
					''' Transition from Cooking to Completed'''
					self.eventConsumed=1

					''' Action code for transition '''
					self.radiator.ovenOff();
					self.tm.clear();


					''' adjust state variables '''
					self.stateVarSuper = "Completed";
				else:
					pass  #Intentionally left blank

			

			elif self.stateVarSuper == "CookingPause": 
				if event=="evDoorClosed":
					''' Transition from CookingPause to Cooking'''
					self.eventConsumed=1

					''' Action code for transition '''
					self.tm.cont();

					''' OnEntry code of state Cooking'''
					self.radiator.ovenOn();

					''' adjust state variables '''
					self.stateVarSuper = "Cooking";
				else:
					pass  #Intentionally left blank

			

			elif self.stateVarSuper == "Idle": 
				if event=="evDoorClosed":
					if self.tm.preset()>0:
						''' Transition from Idle to Cooking'''
						self.eventConsumed=1

						''' Action code for transition '''
						self.tm.start();

						''' OnEntry code of state Cooking'''
						self.radiator.ovenOn();

						''' adjust state variables '''
						self.stateVarSuper = "Cooking";
					else:
						pass  #Intentionally left blank

				else:
					pass  #Intentionally left blank

			

			

			''' Check if event was already processed '''
			if self.eventConsumed !=True:

				if event=="evDec":
					''' Transition from Super to Super'''
					self.eventConsumed=1
					
					''' Action code for transition '''
					self.tm.dec();

					self.stateVar = "Super";''' entry chain '''
					if self.stateVarSuper== "Cooking":
						self.radiator.ovenOn();

					elif self.stateVarSuper== "Idle":
						self.radiator.ovenOff();

					###

				elif event=="evInc":
					''' Transition from Super to Super'''
					self.eventConsumed=1
					
					''' Action code for transition '''
					self.tm.inc();

					self.stateVar = "Super";''' entry chain '''
					if self.stateVarSuper== "Cooking":
						self.radiator.ovenOn();

					elif self.stateVarSuper== "Idle":
						self.radiator.ovenOff();

					###

				elif event=="evPwrHigh":
					''' Transition from Super to Super'''
					self.eventConsumed=1
					
					''' Action code for transition '''
					self.radiator.ovenSetPwrHigh();

					self.stateVar = "Super";''' entry chain '''
					if self.stateVarSuper== "Cooking":
						self.radiator.ovenOn();

					elif self.stateVarSuper== "Idle":
						self.radiator.ovenOff();

					###

				elif event=="evPwrLow":
					''' Transition from Super to Super'''
					self.eventConsumed=1
					
					''' Action code for transition '''
					self.radiator.ovenSetPwrLow();

					self.stateVar = "Super";''' entry chain '''
					if self.stateVarSuper== "Cooking":
						self.radiator.ovenOn();

					elif self.stateVarSuper== "Idle":
						self.radiator.ovenOff();

					###

				else:
					pass  #Intentionally left blank

			
		

	
		return self.eventConsumed


	# Helper(s) to find out if the machine is in a certain state

	def isInCompleted(self):
		return(( True if((self.stateVarSuper== "Completed") and (self.stateVar== "Super")) else False))

	def isInCooking(self):
		return(( True if((self.stateVarSuper== "Cooking") and (self.stateVar== "Super")) else False))

	def isInCookingPause(self):
		return(( True if((self.stateVarSuper== "CookingPause") and (self.stateVar== "Super")) else False))

	def isInIdle(self):
		return(( True if((self.stateVarSuper== "Idle") and (self.stateVar== "Super")) else False))

	def isInSuper(self):
		return(( True if((self.stateVar== "Super")) else False))


	# Helper to get id of innermost active state

	def getInnermostActiveState(self):
		if self.isInIdle():
			return "Idle"
		elif self.isInCookingPause():
			return "CookingPause"
		elif self.isInCooking():
			return "Cooking"
		elif self.isInCompleted():
			return "Completed"
		else:
			return None
		
	
