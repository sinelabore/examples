// @generated from blinky.xml by the Sinelabore code generator. Do not edit.
// Blinky example: generated from model/blinky.xml

// Command line options: -p ssc -l rust -o src/fsm/blinky model/blinky.xml
// Generated on: Thu Nov 05 18:28:37 CET 2026 / 8.0

//! State machine Blinky, generated from blinky.xml.

#![allow(unused_variables, dead_code)]
#![allow(
    clippy::collapsible_if,
    clippy::collapsible_match,
    clippy::single_match
)]

use core::fmt;

/// States of the state machine, of every level.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum BlinkyStates {
    FailSafeOff,
    FailSafeOn,
    Failsafe,
    Operational,
    Off,
    On,
    Dummy,
}

impl BlinkyStates {
    /// Name of the variant as written in the model.
    pub const fn as_str(self) -> &'static str {
        match self {
            BlinkyStates::FailSafeOff => "FailSafeOff",
            BlinkyStates::FailSafeOn => "FailSafeOn",
            BlinkyStates::Failsafe => "Failsafe",
            BlinkyStates::Operational => "Operational",
            BlinkyStates::Off => "Off",
            BlinkyStates::On => "On",
            BlinkyStates::Dummy => "Dummy",
        }
    }
}

impl fmt::Display for BlinkyStates {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.pad(self.as_str())
    }
}

/// Events that can be sent to the state machine.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
#[allow(non_camel_case_types)]
pub enum BlinkyEvents {
    evTimeout,
    evResume,
    evGoFailsafe,
    BlinkyNoMsg,
}

impl BlinkyEvents {
    /// Name of the variant as written in the model.
    pub const fn as_str(self) -> &'static str {
        match self {
            BlinkyEvents::evTimeout => "evTimeout",
            BlinkyEvents::evResume => "evResume",
            BlinkyEvents::evGoFailsafe => "evGoFailsafe",
            BlinkyEvents::BlinkyNoMsg => "BlinkyNoMsg",
        }
    }
}

impl fmt::Display for BlinkyEvents {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.pad(self.as_str())
    }
}

/// Outcome of one call of an event handler. The variants are in order of
/// precedence: when regions handle the same event, the highest one is returned.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum BlinkyStep {
    /// No transition was taken.
    Ignored,
    /// A transition on a conditional event (written with #) was taken.
    Condition,
    /// A transition was taken.
    Taken,
}

type BlinkyData = crate::Board;
type BlinkyTimerService = crate::Ticker<BlinkyEvents, { crate::TIMER_SLOTS }>;

/// The state machine. Build it with `Blinky::default()` and send
/// events with `handle_event`. The readers return the active states.
#[derive(Debug, Clone)]
pub struct Blinky {
    initialized: bool,
    state: BlinkyStates,
    state_failsafe: BlinkyStates,
    state_r1: BlinkyStates,
    state_r2: BlinkyStates,
}

impl Default for Blinky {
    fn default() -> Self {
        Self {
            initialized: false,
            state: BlinkyStates::Operational,
            state_failsafe: BlinkyStates::FailSafeOn,
            state_r1: BlinkyStates::Off,
            state_r2: BlinkyStates::Dummy,
        }
    }
}

impl Blinky {
    /// Active state at the top level.
    pub fn state(&self) -> BlinkyStates {
        self.state
    }

    /// Active state inside Failsafe. Only meaningful while Failsafe is active.
    pub fn state_failsafe(&self) -> BlinkyStates {
        self.state_failsafe
    }

    /// Active state inside R1. Only meaningful while R1 is active.
    pub fn state_r1(&self) -> BlinkyStates {
        self.state_r1
    }

    /// Active state inside R2. Only meaningful while R2 is active.
    pub fn state_r2(&self) -> BlinkyStates {
        self.state_r2
    }

    /// The innermost active states, one per active region.
    pub fn innermost_active_states(&self) -> impl Iterator<Item = BlinkyStates> {
        let mut active = [BlinkyStates::FailSafeOff; 2];
        let mut count = 0;
        match self.state {
            BlinkyStates::Failsafe => {
                active[count] = self.state_failsafe;
                count += 1;
            }
            BlinkyStates::Operational => {
                active[count] = self.state_r1;
                count += 1;
                active[count] = self.state_r2;
                count += 1;
            }
            _ => {}
        }
        active.into_iter().take(count)
    }

    /// Processes one event and tells whether a transition was taken.
    /// The first call runs the entry code of the initial states first.
    pub fn handle_event(
        &mut self,
        ev: BlinkyEvents,
        data: &mut BlinkyData,
        timer_service: &mut BlinkyTimerService,
    ) -> BlinkyStep {
        if !self.initialized {
            self.initialize(data, timer_service);
        }
        self.blinky_main_machine(ev, data, timer_service)
    }

    /// Runs the entry code of the initial states. Later calls do nothing.
    pub fn initialize(&mut self, data: &mut BlinkyData, timer_service: &mut BlinkyTimerService) {
        if self.initialized {
            return;
        }
        self.initialized = true;
        timer_service.start(1, 500, BlinkyEvents::evTimeout); // ms

        data.led_off();
    }

    /// Handles the event in region R1.
    fn blinky_r1(
        &mut self,
        ev: BlinkyEvents,
        data: &mut BlinkyData,
        timer_service: &mut BlinkyTimerService,
    ) -> BlinkyStep {
        let mut ev_consumed = BlinkyStep::Ignored;

        match self.state_r1 {
            BlinkyStates::On => {
                if ev == BlinkyEvents::evTimeout {
                    // Transition from On to Off
                    ev_consumed = BlinkyStep::Taken;

                    // OnEntry code of state Off
                    data.led_off();

                    // adjust state variables
                    self.state_r1 = BlinkyStates::Off;
                }
            }

            BlinkyStates::Off => {
                if ev == BlinkyEvents::evTimeout {
                    // Transition from Off to On
                    ev_consumed = BlinkyStep::Taken;

                    // OnEntry code of state On
                    data.led_on();

                    // adjust state variables
                    self.state_r1 = BlinkyStates::On;
                }
            }

            _ => {}
        }
        ev_consumed
    }

    /// Handles the event in region R2.
    fn blinky_r2(
        &mut self,
        ev: BlinkyEvents,
        data: &mut BlinkyData,
        timer_service: &mut BlinkyTimerService,
    ) -> BlinkyStep {
        let ev_consumed = BlinkyStep::Ignored;

        match self.state_r2 {
            BlinkyStates::Dummy => {}

            _ => {}
        }
        ev_consumed
    }

    fn blinky_main_machine(
        &mut self,
        ev: BlinkyEvents,
        data: &mut BlinkyData,
        timer_service: &mut BlinkyTimerService,
    ) -> BlinkyStep {
        let mut ev_consumed = BlinkyStep::Ignored;

        match self.state {
            BlinkyStates::Failsafe => {
                match self.state_failsafe {
                    BlinkyStates::FailSafeOn => {
                        if ev == BlinkyEvents::evTimeout {
                            // Transition from FailSafeOn to FailSafeOff
                            ev_consumed = BlinkyStep::Taken;

                            // OnEntry code of state FailSafeOff
                            data.led_off();

                            // adjust state variables
                            self.state_failsafe = BlinkyStates::FailSafeOff;
                        }
                    }

                    BlinkyStates::FailSafeOff => {
                        if ev == BlinkyEvents::evTimeout {
                            // Transition from FailSafeOff to FailSafeOn
                            ev_consumed = BlinkyStep::Taken;

                            // OnEntry code of state FailSafeOn
                            data.led_on();

                            // adjust state variables
                            self.state_failsafe = BlinkyStates::FailSafeOn;
                        }
                    }

                    _ => {}
                }

                // Check if event was already processed
                if ev_consumed == BlinkyStep::Ignored {
                    if ev == BlinkyEvents::evResume {
                        // Transition from Failsafe to Operational
                        ev_consumed = BlinkyStep::Taken;

                        timer_service.stop(0);

                        // OnEntry code of state Operational
                        timer_service.start(1, 500, BlinkyEvents::evTimeout); // ms

                        // Entry code for regions in state Operational
                        data.led_off();
                        self.state_r1 = BlinkyStates::Off; // Default in entry chain
                        self.state_r2 = BlinkyStates::Dummy; // Default in entry chain

                        // adjust state variables
                        self.state = BlinkyStates::Operational;
                    }
                }
            }

            BlinkyStates::Operational => {
                // calling region code
                ev_consumed = ev_consumed.max(self.blinky_r1(ev, data, timer_service));
                ev_consumed = ev_consumed.max(self.blinky_r2(ev, data, timer_service));

                // Check if event was already processed
                if ev_consumed == BlinkyStep::Ignored {
                    if ev == BlinkyEvents::evGoFailsafe {
                        // Transition from Operational to Failsafe
                        ev_consumed = BlinkyStep::Taken;

                        // Exit code for regions in state Operational

                        timer_service.stop(1);

                        // OnEntry code of state Failsafe
                        timer_service.start(0, 100, BlinkyEvents::evTimeout);
                        data.led_off();
                        data.led_on();
                        self.state = BlinkyStates::Failsafe; // Default in entry chain
                        self.state_failsafe = BlinkyStates::FailSafeOn; // Default in entry chain
                    }
                }
            }

            _ => {}
        }
        ev_consumed
    }
}
