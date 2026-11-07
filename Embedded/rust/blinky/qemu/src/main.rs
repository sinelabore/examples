//! The generated blinky machine on an emulated Cortex-M3 (QEMU lm3s6965evb).
//! The SysTick timer interrupts every 10 ms. On each tick the loop polls the
//! timer service, and a schedule stands in for the failsafe wire. The board prints over
//! semihosting: QEMU has no LED and no pin here.
#![no_std]
#![no_main]

use core::sync::atomic::{AtomicU32, Ordering};

use cortex_m::{asm::wfi, peripheral::syst::SystClkSource};
use cortex_m_rt::{entry, exception};
use cortex_m_semihosting::{debug, hprintln};
use panic_semihosting as _;

// The same generated file the host tests and the NUCLEO-L432KC firmware use.
#[path = "../../src/fsm/blinky.rs"]
mod blinky;
#[path = "../../src/ticker.rs"]
mod ticker;
use blinky::{Blinky, BlinkyEvents, BlinkyEvents::*};
pub use ticker::{Ticker, TIMER_SLOTS};

/// SysTick clock on QEMU's lm3s6965evb (measured: 12 MHz runs 4 % fast).
const CORE_HZ: u32 = 12_500_000;
const TICK_MS: u32 = 10;
const RUN_MS: u32 = 3_000;
/// The failsafe wire: on at 1250 ms, off at 1750 ms.
const SCHEDULE: [(u32, BlinkyEvents); 2] = [(1_250, evGoFailsafe), (1_750, evResume)];

/// What the actions switch. Here the LED is a line on the console.
pub struct Board;

impl Board {
    pub fn led_on(&mut self) {
        hprintln!("          LED on");
    }
    pub fn led_off(&mut self) {
        hprintln!("          LED off");
    }
}

/// SysTick interrupts so far, one per TICK_MS.
static TICKS: AtomicU32 = AtomicU32::new(0);

#[exception]
fn SysTick() {
    TICKS.fetch_add(1, Ordering::Relaxed);
}

#[entry]
fn main() -> ! {
    // SysTick reloads itself every TICK_MS, so the ticks do not drift
    let mut syst = cortex_m::Peripherals::take().unwrap().SYST;
    syst.set_clock_source(SystClkSource::Core);
    syst.set_reload(CORE_HZ / 1_000 * TICK_MS - 1);
    syst.clear_current();
    syst.enable_counter();
    syst.enable_interrupt();

    let mut board = Board;
    let mut ticker = Ticker::<BlinkyEvents, TIMER_SLOTS>::default();
    let mut machine = Blinky::default();

    hprintln!("    0 ms  initialize");
    machine.initialize(&mut board, &mut ticker);

    for tick in 1..=RUN_MS / TICK_MS {
        while TICKS.load(Ordering::Relaxed) < tick {
            wfi(); // sleep until the next SysTick
        }
        let now_ms = tick * TICK_MS;

        // timers first, so one started by an event of this tick counts from the next
        let fired = ticker.poll(TICK_MS);
        let wire = SCHEDULE
            .iter()
            .find(|(at, _)| *at == now_ms)
            .map(|&(_, ev)| ev);

        for ev in fired.into_iter().flatten().chain(wire) {
            hprintln!("{:>5} ms  {}", now_ms, ev);
            machine.handle_event(ev, &mut board, &mut ticker);
            for state in machine.innermost_active_states() {
                hprintln!("          -> {}", state);
            }
        }
    }

    debug::exit(debug::EXIT_SUCCESS);
    loop {
        wfi();
    }
}
