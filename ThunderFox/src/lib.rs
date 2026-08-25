pub mod engine;
pub mod stylo;
pub mod hw_accel;
pub mod gui;

pub use engine::GeckoEngine;
pub use stylo::StyloOptimizer;
pub use hw_accel::apply_thunder_optimizations;
pub use gui::start_gui;

pub fn init_thunderfox() {
    println!("--- ThunderFox: Multiversal Browser Core ---");
    apply_thunder_optimizations();
}
