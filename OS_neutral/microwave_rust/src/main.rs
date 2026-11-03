//! Microwave oven simulation. The state machine in `oven.rs` is generated from
//! `oven.xml`; this file feeds it keys and the end of the cooking time.

mod cooking_timer;
mod magnetron;
mod oven;

use std::io::Read;
use std::sync::mpsc::{self, RecvTimeoutError};
use std::thread;
use std::time::Instant;

use cooking_timer::CookingTimer;
use magnetron::Magnetron;
use oven::{Oven, OvenEvents, OvenEvents::*};

const HELP: &str = "Use + for inc; - for dec; o for open and c for close; q to quit.
Use 'p' to set medium power (400 W); 'P' to set high power (1200 W).
Type the keys and press Enter.";

/// The event a key stands for. evPwr carries the power in watts.
fn event_for_key(key: char) -> Option<OvenEvents> {
    match key {
        '+' => Some(evInc),
        '-' => Some(evDec),
        'c' => Some(evDoorClosed),
        'o' => Some(evDoorOpen),
        'p' => Some(evPwr { pwr: 400 }),
        'P' => Some(evPwr { pwr: 1200 }),
        _ => None,
    }
}

fn main() {
    // the two objects the actions in oven.xml use, as data and timer_service
    let mut magnetron = Magnetron::default();
    let mut timer = CookingTimer::default();
    let mut oven = Oven::default();
    oven.initialize(&mut magnetron, &mut timer);

    // keys arrive from the console in a thread of their own
    let (keys, received) = mpsc::channel();
    thread::spawn(move || {
        for byte in std::io::stdin().lock().bytes().map_while(Result::ok) {
            let _ = keys.send(char::from(byte));
        }
    });

    println!("{HELP}");
    loop {
        // wait for the next key, but no longer than until the cooking time is up
        let key = match timer.deadline() {
            Some(until) => received.recv_timeout(until.saturating_duration_since(Instant::now())),
            None => received.recv().map_err(|_| RecvTimeoutError::Disconnected),
        };
        match key {
            Ok('q') | Err(RecvTimeoutError::Disconnected) => return,
            Ok(key) => {
                if let Some(ev) = event_for_key(key) {
                    oven.handle_event(ev, &mut magnetron, &mut timer);
                } else if !key.is_whitespace() {
                    println!("Unknown key. {HELP}");
                }
            }
            Err(RecvTimeoutError::Timeout) => {}
        }
        if timer.poll(Instant::now()) {
            oven.handle_event(evTimeout, &mut magnetron, &mut timer);
        }
    }
}
