//! Ten timer slots. Actions start and stop a slot by id; the main loop polls
//! and forwards the event that slot stored.
//!
//! `TIMER_SLOTS` is the whole capacity. Raise it when a diagram uses a higher
//! id. An id of `TIMER_SLOTS` or above does nothing. This blinker uses slots
//! 0 and 1; the other eight stay free.

/// How many timers this service can run at once.
pub const TIMER_SLOTS: usize = 10;

#[derive(Debug, Clone, Copy)]
struct Slot<E> {
    period_ms: u32,
    elapsed_ms: u32,
    event: Option<E>,
}

impl<E: Copy> Default for Slot<E> {
    fn default() -> Self {
        Self {
            period_ms: 0,
            elapsed_ms: 0,
            event: None,
        }
    }
}

/// `N` slots, each either stopped or counting toward the event it stores.
#[derive(Debug, Clone, Copy)]
pub struct Ticker<E, const N: usize> {
    slots: [Slot<E>; N],
}

impl<E: Copy, const N: usize> Default for Ticker<E, N> {
    fn default() -> Self {
        Self {
            slots: [Slot::default(); N],
        }
    }
}

impl<E: Copy, const N: usize> Ticker<E, N> {
    /// Starts slot `id` from now. The stored event is what `poll` returns.
    pub fn start(&mut self, id: usize, period_ms: u32, event: E) {
        let Some(slot) = self.slots.get_mut(id) else {
            return;
        };
        if period_ms == 0 {
            return;
        }
        *slot = Slot {
            period_ms,
            elapsed_ms: 0,
            event: Some(event),
        };
        trace_start(period_ms);
    }

    /// Stops slot `id`. A slot that is already stopped stays quiet.
    pub fn stop(&mut self, id: usize) {
        let Some(slot) = self.slots.get_mut(id) else {
            return;
        };
        if slot.event.is_none() {
            return;
        }
        *slot = Slot::default();
        trace_stop();
    }

    /// The period of a running slot.
    pub fn period_ms(&self, id: usize) -> Option<u32> {
        self.slots
            .get(id)
            .and_then(|slot| slot.event.map(|_| slot.period_ms))
    }

    /// Lets `elapsed_ms` pass on every running slot.
    /// One entry per slot, in id order: `Some` is the event whose period elapsed.
    /// A firing slot keeps running, and its next period starts now.
    pub fn poll(&mut self, elapsed_ms: u32) -> [Option<E>; N] {
        let mut fired = [None; N];
        for (slot, out) in self.slots.iter_mut().zip(fired.iter_mut()) {
            let Some(event) = slot.event else {
                continue;
            };
            slot.elapsed_ms = slot.elapsed_ms.saturating_add(elapsed_ms);
            if slot.elapsed_ms >= slot.period_ms {
                slot.elapsed_ms = 0;
                *out = Some(event);
            }
        }
        fired
    }
}

fn trace_start(period_ms: u32) {
    #[cfg(feature = "timer-trace")]
    let _ = cortex_m_semihosting::hprintln!("          timer {} ms", period_ms);
    #[cfg(not(feature = "timer-trace"))]
    let _ = period_ms;
}

fn trace_stop() {
    #[cfg(feature = "timer-trace")]
    let _ = cortex_m_semihosting::hprintln!("          timer stopped");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ten_slots_and_no_more() {
        assert_eq!(TIMER_SLOTS, 10);
        let mut ticker = Ticker::<char, TIMER_SLOTS>::default();
        ticker.start(TIMER_SLOTS - 1, 10, 'z');
        assert_eq!(ticker.period_ms(TIMER_SLOTS - 1), Some(10));
        ticker.start(TIMER_SLOTS, 10, 'z');
        assert_eq!(ticker.period_ms(TIMER_SLOTS), None);
    }

    #[test]
    fn each_slot_fires_its_own_event() {
        let mut ticker = Ticker::<char, TIMER_SLOTS>::default();
        ticker.start(1, 500, 's');
        ticker.start(0, 100, 'f');

        let fired = ticker.poll(100);
        assert_eq!(fired[1], None);
        assert_eq!(fired[0], Some('f'));

        let fired = ticker.poll(400);
        assert_eq!(fired[1], Some('s'));
        assert_eq!(fired[0], Some('f'));
    }

    #[test]
    fn a_stopped_slot_does_not_fire() {
        let mut ticker = Ticker::<char, TIMER_SLOTS>::default();
        ticker.start(0, 10, 'a');
        ticker.stop(0);
        ticker.stop(0);
        assert_eq!(ticker.poll(10)[0], None);
    }
}
