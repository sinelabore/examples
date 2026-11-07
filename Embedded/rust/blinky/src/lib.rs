//! The blinky state machine with a host stand-in for the board, so the
//! machine can be checked without hardware. The library is `no_std`.
#![cfg_attr(not(test), no_std)]

pub mod fsm;
mod ticker;

pub use ticker::{Ticker, TIMER_SLOTS};

/// What the actions switch: here only a flag instead of a GPIO pin.
#[derive(Debug, Default)]
pub struct Board {
    pub led: bool,
}

impl Board {
    pub fn led_on(&mut self) {
        self.led = true;
    }
    pub fn led_off(&mut self) {
        self.led = false;
    }
}