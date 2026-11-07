//! The generated blinky machine on a NUCLEO-L432KC (STM32L432KC).
#![no_std]
#![no_main]

use cortex_m_rt::entry;
use panic_halt as _;
use stm32l4xx_hal::{
    delay::Delay,
    gpio::{Output, PushPull, PB3},
    pac,
    prelude::*,
};

// The same generated file the host tests and the QEMU variant use.
#[path = "../../src/fsm/blinky.rs"]
mod blinky;
#[path = "../../src/ticker.rs"]
mod ticker;
use blinky::{Blinky, BlinkyEvents, BlinkyEvents::*};
pub use ticker::{Ticker, TIMER_SLOTS};

/// What the actions switch: the user LED LD3 on PB3 (Arduino pin D13).
pub struct Board {
    led: PB3<Output<PushPull>>,
}

impl Board {
    pub fn led_on(&mut self) {
        self.led.set_high();
    }
    pub fn led_off(&mut self) {
        self.led.set_low();
    }
}

#[entry]
fn main() -> ! {
    let cp = cortex_m::Peripherals::take().unwrap();
    let dp = pac::Peripherals::take().unwrap();

    let mut flash = dp.FLASH.constrain();
    let mut rcc = dp.RCC.constrain();
    let mut pwr = dp.PWR.constrain(&mut rcc.apb1r1);
    let clocks = rcc.cfgr.freeze(&mut flash.acr, &mut pwr);
    let mut delay = Delay::new(cp.SYST, clocks);

    let mut gpiob = dp.GPIOB.split(&mut rcc.ahb2);
    let led = gpiob
        .pb3
        .into_push_pull_output(&mut gpiob.moder, &mut gpiob.otyper);
    let mut gpioa = dp.GPIOA.split(&mut rcc.ahb2);
    // a wire from A0 (PA0) to GND puts the machine into Failsafe
    let failsafe_pin = gpioa
        .pa0
        .into_pull_up_input(&mut gpioa.moder, &mut gpioa.pupdr);

    let mut board = Board { led };
    let mut ticker = Ticker::<BlinkyEvents, TIMER_SLOTS>::default();
    let mut machine = Blinky::default();
    machine.initialize(&mut board, &mut ticker);

    let mut failsafe = false;
    loop {
        let wanted = failsafe_pin.is_low();
        if wanted != failsafe {
            let ev = if wanted { evGoFailsafe } else { evResume };
            machine.handle_event(ev, &mut board, &mut ticker);
            failsafe = wanted;
        }
        delay.delay_ms(10_u32);
        for ev in ticker.poll(10).into_iter().flatten() {
            machine.handle_event(ev, &mut board, &mut ticker);
        }
    }
}
