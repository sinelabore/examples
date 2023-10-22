oven = require "oven"

local oven = oven:new()

-- timer must be implemented here
function oven:timer_preset()
	print ("timer_preset")
	return 4
end

function oven:timer_pause()
	print ("timer pause")
end

function oven:timer_clear()
	print ("timer clear")
end

function oven:timer_cont()
	print ("timer_cont")
end

function oven:timer_start()
	print ("timer_start")
end

function oven:evTimeout()
	print ("Timer finished")
end

function oven:timer_dec()
	print ("timer_dec")
end

function oven:timer_inc()
	print ("timer_inc")
end

-- microwave related functions implemented here
function oven:ovenOff()
  print ("oven off")
end

function oven:ovenOn()
	print ("oven on")
end

function oven:ovenSetPwr(pwr)
	print ("set pwr =" .. pwr)
end

Event = {event = oven.events.OVEN_NO_MSG};
oven:processEvent(Event)
print("---")

Event = {event = oven.events.evInc};
oven:processEvent(Event)
print("---")

Event = {event = oven.events.evInc};
oven:processEvent(Event)
print("---")

Event = {event = oven.events.evPwr, pwr="high"};
oven:processEvent(Event)
print("---")

Event = {event = oven.events.evDoorClosed};
oven:processEvent(Event)
print("---")

Event = {event = oven.events.evTimeout};
oven:processEvent(Event)
print("---")
