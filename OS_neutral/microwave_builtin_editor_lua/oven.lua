-- Sinelabore Software Tools GmbH

--[[  Command line options: -p ssc -l lua -o oven oven.xml  ]]
--[[  This file is generated from oven.xml - do not edit manually ]]
--[[  Generated on: Sun Oct 22 18:58:57 CEST 2023 / Version 6.1.3706]]




oven = {}
oven.__index = oven


function oven:new()
	local new_inst = {}
	setmetatable( new_inst, oven)

	-- machine states
	new_inst.states = {
		Completed="Completed",
		Cooking="Cooking",
		CookingPause="CookingPause",
		Idle="Idle",
		Super="Super",
		__UNKNOWN_STATE__="__UNKNOWN_STATE__"
	}

	-- machine events
	new_inst.events = {
		evTimeout="evTimeout",
		evDec="evDec",
		evDoorOpen="evDoorOpen",
		evDoorClosed="evDoorClosed",
		evInc="evInc",
		evPwr="evPwr",
		OVEN_NO_MSG="OVEN_NO_MSG"
	}

	-- Set state vars to default states
	new_inst.stateVar = new_inst.states.Super
	new_inst.stateVarSuper = new_inst.states.Idle -- set init state of Super 

	new_inst.init=false
	return new_inst
end


function oven:processEvent(Event)

	local evConsumed=0;

	if self.init==false then
		-- call on entry code of default states '''
		self:ovenOff()
		

		self.init=true;
	end

	-- action code
	-- just a comment




	if self.stateVar == self.states.Super then 

		

			if self.stateVarSuper == self.states.Completed then 
				if Event.event==self.events.evDoorOpen then
					--[[  Transition from Completed to Idle]]
					evConsumed=1

					--[[  OnEntry code of state Idle]]
					self:ovenOff()

					--[[  adjust state variables ]]
					self.stateVarSuper = self.states.Idle;
				else
					--  Intentionally left blank
				end

			

			elseif self.stateVarSuper == self.states.Cooking then 
				if Event.event==self.events.evDoorOpen then
					--[[  Transition from Cooking to CookingPause]]
					evConsumed=1

					--[[  Action code for transition ]]
					self:ovenOff()
					self:timer_pause()


					--[[  adjust state variables ]]
					self.stateVarSuper = self.states.CookingPause;
				elseif Event.event==self.events.evTimeout then
					--[[  Transition from Cooking to Completed]]
					evConsumed=1

					--[[  Action code for transition ]]
					self:ovenOff()
					self:timer_clear()


					--[[  adjust state variables ]]
					self.stateVarSuper = self.states.Completed;
				else
					--  Intentionally left blank
				end

			

			elseif self.stateVarSuper == self.states.CookingPause then 
				if Event.event==self.events.evDoorClosed then
					--[[  Transition from CookingPause to Cooking]]
					evConsumed=1

					--[[  Action code for transition ]]
					self:timer_cont()

					--[[  OnEntry code of state Cooking]]
					self:ovenOn()

					--[[  adjust state variables ]]
					self.stateVarSuper = self.states.Cooking;
				else
					--  Intentionally left blank
				end

			

			elseif self.stateVarSuper == self.states.Idle then 
				if Event.event==self.events.evDoorClosed then
					if self:timer_preset()>0 then
						--[[  Transition from Idle to Cooking]]
						evConsumed=1

						--[[  Action code for transition ]]
						self:timer_start()

						--[[  OnEntry code of state Cooking]]
						self:ovenOn()

						--[[  adjust state variables ]]
						self.stateVarSuper = self.states.Cooking;
					else
						--  Intentionally left blank
					end

				else
					--  Intentionally left blank
				end

			

			end

		--[[  Check if event was already processed ]]
		if evConsumed == 0 then

			if Event.event==self.events.evDec then
				--[[  Transition from Super to Super]]
				evConsumed=1
				
				--[[  Action code for transition ]]
				self:timer_dec()

				self.stateVar = self.states.Super;--[[  entry chain ]]
				if self.stateVarSuper==self.states.Cooking then
					self:ovenOn()

				elseif self.stateVarSuper== self.states.Idle then
					self:ovenOff()

				end

			elseif Event.event==self.events.evInc then
				--[[  Transition from Super to Super]]
				evConsumed=1
				
				--[[  Action code for transition ]]
				self:timer_inc()

				self.stateVar = self.states.Super;--[[  entry chain ]]
				if self.stateVarSuper==self.states.Cooking then
					self:ovenOn()

				elseif self.stateVarSuper== self.states.Idle then
					self:ovenOff()

				end

			elseif Event.event==self.events.evPwr then
				--[[  Transition from Super to Super]]
				evConsumed=1
				
				--[[  Action code for transition ]]
				self:ovenSetPwr(Event.pwr)

				self.stateVar = self.states.Super;--[[  entry chain ]]
				if self.stateVarSuper==self.states.Cooking then
					self:ovenOn()

				elseif self.stateVarSuper== self.states.Idle then
					self:ovenOff()

				end

			else
				--  Intentionally left blank
			end

		end
	

	end



	return evConsumed;
end




return oven
