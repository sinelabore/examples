//! The cooking time: set in whole seconds with inc/dec, then counted down
//! against the clock. The main loop waits until `deadline()` at the latest and
//! asks `poll(now)` whether the time is up.

use std::time::{Duration, Instant};

const STEP: Duration = Duration::from_secs(1);
const MAX_PRESET: Duration = Duration::from_secs(60);

/// What the countdown is doing. Each state carries exactly what it needs.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Countdown {
    Off,
    Running { until: Instant },
    Paused { left: Duration },
}

#[derive(Debug)]
pub struct CookingTimer {
    preset: Duration,
    countdown: Countdown,
}

impl Default for CookingTimer {
    fn default() -> Self {
        Self {
            preset: Duration::ZERO,
            countdown: Countdown::Off,
        }
    }
}

impl CookingTimer {
    /// The cooking time in seconds; the model's guard checks it before cooking.
    pub fn preset(&self) -> u64 {
        self.preset.as_secs()
    }

    pub fn inc(&mut self) {
        if self.preset < MAX_PRESET {
            self.preset += STEP;
            println!("Timer::inc -> preset={}", self.preset());
        }
    }

    pub fn dec(&mut self) {
        if !self.preset.is_zero() {
            self.preset -= STEP;
            println!("Timer::dec -> preset={}", self.preset());
        }
    }

    pub fn clear(&mut self) {
        self.preset = Duration::ZERO;
    }

    /// Starts counting down the preset. A preset of zero does not start the timer.
    pub fn start(&mut self) {
        println!("Timer::start ({} s)", self.preset());
        self.countdown = if self.preset.is_zero() {
            Countdown::Off
        } else {
            Countdown::Running { until: Instant::now() + self.preset }
        };
    }

    pub fn pause(&mut self) {
        if let Countdown::Running { until } = self.countdown {
            let left = until.saturating_duration_since(Instant::now());
            println!("Timer::pause ({:.1} s left)", left.as_secs_f32());
            self.countdown = Countdown::Paused { left };
        }
    }

    pub fn resume(&mut self) {
        if let Countdown::Paused { left } = self.countdown {
            self.countdown = Countdown::Running { until: Instant::now() + left };
        }
    }

    /// When the cooking time is up, while the timer runs.
    pub fn deadline(&self) -> Option<Instant> {
        match self.countdown {
            Countdown::Running { until } => Some(until),
            _ => None,
        }
    }

    /// True once, when the cooking time is over at `now`.
    pub fn poll(&mut self, now: Instant) -> bool {
        match self.countdown {
            Countdown::Running { until } if now >= until => {
                println!("Timer::timeout");
                self.countdown = Countdown::Off;
                true
            }
            _ => false,
        }
    }
}
