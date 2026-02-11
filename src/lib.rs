use rtsyn_plugin::prelude::*;
use serde_json::Value;
use std::mem::MaybeUninit;

#[repr(C)]
struct RthybridHindmarshRose1984NeuronV2CState {
    vars: [f64; 3],
    params: [f64; 11],
    burst_duration: f64,
    burst_duration_value: f64,
    freq: f64,
    period_seconds: f64,
    s_points: usize,
    input_syn: f64,
    input_burst_duration: f64,
}

unsafe extern "C" {
    fn rthybrid_hindmarsh_rose_1984_neuron_v2_c_init(
        state: *mut RthybridHindmarshRose1984NeuronV2CState,
    );
    fn rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_config(
        state: *mut RthybridHindmarshRose1984NeuronV2CState,
        key: *const u8,
        len: usize,
        value: f64,
    );
    fn rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_input(
        state: *mut RthybridHindmarshRose1984NeuronV2CState,
        key: *const u8,
        len: usize,
        value: f64,
    );
    fn rthybrid_hindmarsh_rose_1984_neuron_v2_c_process(
        state: *mut RthybridHindmarshRose1984NeuronV2CState,
        period_seconds: f64,
    );
    fn rthybrid_hindmarsh_rose_1984_neuron_v2_c_get_output(
        state: *const RthybridHindmarshRose1984NeuronV2CState,
        key: *const u8,
        len: usize,
    ) -> f64;
}

struct RthybridHindmarshRose1984NeuronV2C {
    state: RthybridHindmarshRose1984NeuronV2CState,
}

impl Default for RthybridHindmarshRose1984NeuronV2C {
    fn default() -> Self {
        let mut state = MaybeUninit::<RthybridHindmarshRose1984NeuronV2CState>::uninit();
        unsafe {
            rthybrid_hindmarsh_rose_1984_neuron_v2_c_init(state.as_mut_ptr());
            Self {
                state: state.assume_init(),
            }
        }
    }
}

impl PluginDescriptor for RthybridHindmarshRose1984NeuronV2C {
    fn name() -> &'static str {
        "RTHybrid Hindmarsh-Rose 1984 Neuron v2"
    }

    fn kind() -> &'static str {
        "rthybrid_hindmarsh_rose_1984_neuron_v2"
    }

    fn plugin_type() -> PluginType {
        PluginType::Computational
    }

    fn inputs() -> &'static [&'static str] {
        &["Isyn (nA)", "Burst duration (s)"]
    }

    fn outputs() -> &'static [&'static str] {
        &["Vm (v)", "Vm (mV)"]
    }

    fn internal_variables() -> &'static [&'static str] {
        &["x", "y", "z"]
    }

    fn default_vars() -> Vec<(&'static str, Value)> {
        vec![
            ("x0", (-0.9013747551021072).into()),
            ("y0", (-3.15948829665501).into()),
            ("z0", 3.247826955037619.into()),
            ("e", 3.0.into()),
            ("a", 1.0.into()),
            ("b", 3.0.into()),
            ("c", 1.0.into()),
            ("d", 5.0.into()),
            ("mu", 0.0021.into()),
            ("S", 4.0.into()),
            ("xr", (-1.6).into()),
            ("Vh", 1.0.into()),
            ("Burst duration (s)", 1.0.into()),
        ]
    }

    fn behavior() -> PluginBehavior {
        PluginBehavior {
            supports_start_stop: true,
            supports_restart: true,
            supports_apply: false,
            extendable_inputs: ExtendableInputs::None,
            loads_started: false,
            external_window: false,
            starts_expanded: true,
            start_requires_connected_inputs: Vec::new(),
            start_requires_connected_outputs: Vec::new(),
        }
    }
}

impl PluginRuntime for RthybridHindmarshRose1984NeuronV2C {
    fn set_config_value(&mut self, key: &str, value: &Value) {
        if let Some(v) = value.as_f64() {
            unsafe {
                rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_config(
                    &mut self.state,
                    key.as_ptr(),
                    key.len(),
                    v,
                );
            }
        }
    }

    fn set_input_value(&mut self, key: &str, value: f64) {
        unsafe {
            rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_input(
                &mut self.state,
                key.as_ptr(),
                key.len(),
                value,
            );
        }
    }

    fn process_tick(&mut self, _tick: u64, period_seconds: f64) {
        unsafe { rthybrid_hindmarsh_rose_1984_neuron_v2_c_process(&mut self.state, period_seconds) };
    }

    fn get_output_value(&self, key: &str) -> f64 {
        unsafe {
            rthybrid_hindmarsh_rose_1984_neuron_v2_c_get_output(&self.state, key.as_ptr(), key.len())
        }
    }

    fn get_internal_value(&self, key: &str) -> Option<f64> {
        match key {
            "x" => Some(self.state.vars[0]),
            "y" => Some(self.state.vars[1]),
            "z" => Some(self.state.vars[2]),
            _ => None,
        }
    }
}

rtsyn_plugin::export_plugin!(RthybridHindmarshRose1984NeuronV2C);
