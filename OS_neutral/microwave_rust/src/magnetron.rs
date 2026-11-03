//! The microwave generator. This example prints what the hardware would do.

#[derive(Debug, Default)]
pub struct Magnetron {
    pub running: bool,
    pub power_w: u16,
}

impl Magnetron {
    pub fn on(&mut self) {
        self.running = true;
        println!("Oven::on");
    }

    pub fn off(&mut self) {
        self.running = false;
        println!("Oven::off");
    }

    pub fn set_power(&mut self, watts: u16) {
        self.power_w = watts;
        println!("Oven::set power to {} W", watts);
    }
}
