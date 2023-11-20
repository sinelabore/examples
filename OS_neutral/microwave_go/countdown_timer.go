package main

import (
	"fmt"
)

const (
	Stop  int = iota
	Run   int = 1
	Pause int = 2
)

var index uint8 = 0

// Tick iterates through the  CountdownTimers, decrements active timers,
// and identifies those that have expired. Expired timers are marked as stopped.
// The information of expired timers is collected into a slice that is returned.
//
// In each iteration the timers' counters are decremented by 100 (i.e. 100ms) on each
// invocation of the Tick method.
//
// Example:
//
//	timers := NewCountdownTimers()
//	// ... (create and start timers)
//	expiredTimers := timers.Tick()
//
// Returns:
//
//	A slice containing pointers to CountdownTimer instances that have expired.
//	The remaining active timers continue running.
func (cdt *CountdownTimers) Tick() []*CountdownTimer {
	var vals []*CountdownTimer
	for key, value := range cdt.timers {
		if value.Status == Run {
			debugPrint(fmt.Sprintf("Timer %d running but not expired (Cnt=%d)\n", key, value.Cnt))
			value.Cnt = value.Cnt - 100
			if value.Cnt <= 0 {
				// expired
				if value.Singleshot {
					// singleshot
					value.Cnt = 0
					value.Status = Stop
				} else {
					// cylic
					value.Cnt = value.Milli
				}
				vals = append(vals, value)
			}
		}
	}
	return vals
}

// Create adds a new CountdownTimer to the CountdownTimers collection with the specified parameters.
// It returns the identifier (ID) assigned to the newly created timer.
//
// Parameters:
//   - evt:   An event associated with the timer.
//   - t:     The duration of the timer in milliseconds.
//   - kind:  A boolean indicating whether the timer is a singleshot timer (true) or not (false).
//
// Example:
//
//	timers := NewCountdownTimers()
//	timerID := timers.Create("exampleEvent", 5000 /* ms */, true)
//
// Returns:
//
//	The identifier (ID) assigned to the newly created CountdownTimer.
func (cdt *CountdownTimers) Create(evt any, kind bool) (id uint8) {
	index++
	newTimer := CountdownTimer{Stop, 0, 0, kind, evt, index}
	cdt.timers[index] = &newTimer
	//fmt.Printf("%#v", newTimer)
	return index
}

// Stops a CountdownTimer identified by its id.
// After stopping a CountdownTimer the preset timeout value is set to zero
func (cdt *CountdownTimers) Stop(id uint8) {
	value, ok := cdt.timers[id]
	if ok {
		value.Status = Stop
		value.Cnt = 0
	} else {
		panic("Stop got unknown id")
	}
}

// Starts a CountdownTimer identified by its id.
func (cdt *CountdownTimers) Start(id uint8, t uint32) {
	value, ok := cdt.timers[id]
	if ok {
		value.Status = Run
		value.Cnt = t
		value.Milli = t
	} else {
		panic("Start got unknown id")
	}
}

// GetPresetInSec returns the preset timeout time of a CountdownTimer is seconds.
func (cdt *CountdownTimers) GetPresetInSec(id uint8) uint32 {
	value, ok := cdt.timers[id]
	if ok {
		return value.Cnt / 1000
	} else {
		panic("Start got unknown id")
	}
}

// Starts a CountdownTimer with the given preset value
func (cdt *CountdownTimers) StartWithPreset(id uint8) {
	value, ok := cdt.timers[id]
	if ok {
		value.Status = Run
	} else {
		panic("Start got unknown id")
	}
}

func (cdt *CountdownTimers) Pause(id uint8) {
	value, ok := cdt.timers[id]
	if ok {
		value.Status = Pause
	} else {
		panic("Pause got unknown id")
	}
}

func (cdt *CountdownTimers) Resume(id uint8) {
	value, ok := cdt.timers[id]
	if ok {
		value.Status = Run
	} else {
		panic("Restart got unknown id")
	}
}

// NewCountdownTimers creates and returns a new instance of the CountdownTimers struct.
// CountdownTimers is a utility for managing a collection of CountdownTimers.
// The function initializes the timers map with an empty map to store CountdownTimer instances.
//
// Example:
// timers := NewCountdownTimers()
//
// Returns:
//
//	A pointer to a newly created CountdownTimers struct.
func NewCountdownTimers() *CountdownTimers {
	return &CountdownTimers{
		timers: make(map[uint8]*CountdownTimer),
	}
}

// Increment increments the timeout value by one second
func (cdt *CountdownTimers) Increment(id uint8, t uint32) {
	value, ok := cdt.timers[id]
	if ok {
		value.Cnt += t
	} else {
		panic("Restart got unknown id")
	}
}

// Decrement decrements the timeout value by one second down to zero.
func (cdt *CountdownTimers) Decrement(id uint8, t uint32) {
	value, ok := cdt.timers[id]
	if ok {
		value.Cnt -= t
		if value.Cnt <= 0 {
			value.Cnt = 0
		}
	} else {
		panic("Restart got unknown id")
	}
}

type CountdownTimers struct {
	timers map[uint8]*CountdownTimer
}

// CountdownTimer represents a single timer
type CountdownTimer struct {
	Status     int
	Cnt        uint32
	Milli      uint32 // used to restart the timer for cyclic timers
	Singleshot bool
	Event      any
	id         uint8
}
