// @generated from oven.xml by the Sinelabore code generator. Do not edit.
// (C) Your company

// Command line options: -p ssc -l rust -o src/oven oven.xml
// Generated on: Tue Nov 03 19:49:25 CET 2026 / 8.0

//! State machine Oven, generated from oven.xml.

#![allow(unused_variables, dead_code)]
#![allow(
    clippy::collapsible_if,
    clippy::collapsible_match,
    clippy::single_match
)]

use core::fmt;

/// States of the state machine, of every level.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum OvenStates {
    Super,
    Completed,
    Cooking,
    CookingPause,
    Idle,
}

impl OvenStates {
    /// Name of the variant as written in the model.
    pub const fn as_str(self) -> &'static str {
        match self {
            OvenStates::Super => "Super",
            OvenStates::Completed => "Completed",
            OvenStates::Cooking => "Cooking",
            OvenStates::CookingPause => "CookingPause",
            OvenStates::Idle => "Idle",
        }
    }
}

impl fmt::Display for OvenStates {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.pad(self.as_str())
    }
}

/// Events that can be sent to the state machine.
#[derive(Debug, Clone, Copy, PartialEq)]
#[allow(non_camel_case_types)]
pub enum OvenEvents {
    evTimeout,
    evDec,
    evDoorOpen,
    evDoorClosed,
    evInc,
    evPwr { pwr: u16 },
    OvenNoMsg,
}

impl OvenEvents {
    /// Name of the variant as written in the model.
    pub const fn as_str(self) -> &'static str {
        match self {
            OvenEvents::evTimeout => "evTimeout",
            OvenEvents::evDec => "evDec",
            OvenEvents::evDoorOpen => "evDoorOpen",
            OvenEvents::evDoorClosed => "evDoorClosed",
            OvenEvents::evInc => "evInc",
            OvenEvents::evPwr { .. } => "evPwr",
            OvenEvents::OvenNoMsg => "OvenNoMsg",
        }
    }
}

impl fmt::Display for OvenEvents {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.pad(self.as_str())
    }
}

/// Outcome of one call of an event handler. The variants are in order of
/// precedence: when regions handle the same event, the highest one is returned.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum OvenStep {
    /// No transition was taken.
    Ignored,
    /// A transition on a conditional event (written with #) was taken.
    Condition,
    /// A transition was taken.
    Taken,
}

type OvenData = crate::magnetron::Magnetron;
type OvenTimerService = crate::cooking_timer::CookingTimer;

/// The state machine. Build it with `Oven::default()` and send
/// events with `handle_event`. The readers return the active states.
#[derive(Debug, Clone)]
pub struct Oven {
    initialized: bool,
    state: OvenStates,
    state_main_region: OvenStates,
}

impl Default for Oven {
    fn default() -> Self {
        Self {
            initialized: false,
            state: OvenStates::Super,
            state_main_region: OvenStates::Idle,
        }
    }
}

impl Oven {
    /// Active state at the top level.
    pub fn state(&self) -> OvenStates {
        self.state
    }

    /// Active state inside MainRegion. Only meaningful while MainRegion is active.
    pub fn state_main_region(&self) -> OvenStates {
        self.state_main_region
    }

    /// Processes one event and tells whether a transition was taken.
    /// The first call runs the entry code of the initial states first.
    pub fn handle_event(
        &mut self,
        ev: OvenEvents,
        data: &mut OvenData,
        timer_service: &mut OvenTimerService,
    ) -> OvenStep {
        if !self.initialized {
            self.initialize(data, timer_service);
        }
        self.oven_main_machine(ev, data, timer_service)
    }

    /// Runs the entry code of the initial states. Later calls do nothing.
    pub fn initialize(&mut self, data: &mut OvenData, timer_service: &mut OvenTimerService) {
        if self.initialized {
            return;
        }
        self.initialized = true;
        data.off();
    }

    /// Handles the event in region MainRegion.
    fn oven_main_region(
        &mut self,
        ev: OvenEvents,
        data: &mut OvenData,
        timer_service: &mut OvenTimerService,
    ) -> OvenStep {
        let mut ev_consumed = OvenStep::Ignored;

        match self.state_main_region {
            OvenStates::Completed => {
                if ev == OvenEvents::evDoorOpen {
                    // Transition from Completed to Idle
                    ev_consumed = OvenStep::Taken;

                    // OnEntry code of state Idle
                    data.off();

                    // adjust state variables
                    self.state_main_region = OvenStates::Idle;
                }
            }

            OvenStates::Cooking => {
                if ev == OvenEvents::evDoorOpen {
                    // Transition from Cooking to CookingPause
                    ev_consumed = OvenStep::Taken;

                    // Action code for transition
                    data.off();
                    timer_service.pause();

                    // adjust state variables
                    self.state_main_region = OvenStates::CookingPause;
                } else if ev == OvenEvents::evTimeout {
                    // Transition from Cooking to Completed
                    ev_consumed = OvenStep::Taken;

                    // Action code for transition
                    data.off();
                    timer_service.clear();

                    // adjust state variables
                    self.state_main_region = OvenStates::Completed;
                }
            }

            OvenStates::CookingPause => {
                if ev == OvenEvents::evDoorClosed {
                    // Transition from CookingPause to Cooking
                    ev_consumed = OvenStep::Taken;

                    // Action code for transition
                    timer_service.resume();

                    // OnEntry code of state Cooking
                    data.on();

                    // adjust state variables
                    self.state_main_region = OvenStates::Cooking;
                }
            }

            OvenStates::Idle => {
                if ev == OvenEvents::evDoorClosed {
                    if timer_service.preset() > 0 {
                        // Transition from Idle to Cooking
                        ev_consumed = OvenStep::Taken;

                        // Action code for transition
                        timer_service.start();

                        // OnEntry code of state Cooking
                        data.on();

                        // adjust state variables
                        self.state_main_region = OvenStates::Cooking;
                    }
                }
            }

            _ => {}
        }
        ev_consumed
    }

    fn oven_main_machine(
        &mut self,
        ev: OvenEvents,
        data: &mut OvenData,
        timer_service: &mut OvenTimerService,
    ) -> OvenStep {
        let mut ev_consumed = OvenStep::Ignored;

        match self.state {
            OvenStates::Super => {
                // calling region code
                ev_consumed = ev_consumed.max(self.oven_main_region(ev, data, timer_service));

                // Check if event was already processed
                if ev_consumed == OvenStep::Ignored {
                    if ev == OvenEvents::evDec {
                        // Transition from Super to Super

                        // Exit code for regions in state Super

                        // Action code for transition
                        timer_service.dec();

                        // Entry code for regions in state Super
                        data.off();
                        self.state_main_region = OvenStates::Idle; // Default in entry chain

                        // adjust state variables
                        self.state = OvenStates::Super;
                    } else if ev == OvenEvents::evInc {
                        // Transition from Super to Super

                        // Exit code for regions in state Super

                        // Action code for transition
                        timer_service.inc();

                        // Entry code for regions in state Super
                        data.off();
                        self.state_main_region = OvenStates::Idle; // Default in entry chain

                        // adjust state variables
                        self.state = OvenStates::Super;
                    } else if let OvenEvents::evPwr { pwr } = ev {
                        // Transition from Super to Super

                        // Exit code for regions in state Super

                        // Action code for transition
                        data.set_power(pwr);

                        // Entry code for regions in state Super
                        data.off();
                        self.state_main_region = OvenStates::Idle; // Default in entry chain

                        // adjust state variables
                        self.state = OvenStates::Super;
                    }
                }
            }

            _ => {}
        }
        ev_consumed
    }
}
