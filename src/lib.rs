use rtsyn_plugin::prelude::*;
use serde_json::Value;

#[derive(Debug)]
struct RthybridHindmarshRose1984NeuronV2Rust {
    input_syn: f64,
    input_burst_duration: f64,
    out_0: f64,
    out_1: f64,
    x: f64,
    y: f64,
    z: f64,
    i: f64,
    a: f64,
    b: f64,
    c: f64,
    d: f64,
    r: f64,
    s: f64,
    xr: f64,
    vh: f64,
    dt: f64,
    burst_duration_value: f64,
    s_points: usize,
}

impl Default for RthybridHindmarshRose1984NeuronV2Rust {
    fn default() -> Self {
        Self {
            input_syn: 0.0,
            input_burst_duration: 1.0,
            out_0: 0.0,
            out_1: 0.0,
            x: -0.9013747551021072,
            y: -3.15948829665501,
            z: 3.247826955037619,
            i: 3.0,
            a: 1.0,
            b: 3.0,
            c: 1.0,
            d: 5.0,
            r: 0.0021,
            s: 4.0,
            xr: -1.6,
            vh: 1.0,
            dt: 0.0015,
            burst_duration_value: 1.0,
            s_points: 1,
        }
    }
}

impl RthybridHindmarshRose1984NeuronV2Rust {
    fn select_optimal_dt(pts_match: f64) -> f64 {
        const DTS: [f64; 12] = [
            0.0005, 0.001, 0.0015, 0.002, 0.003, 0.005, 0.01, 0.015, 0.02, 0.03, 0.05, 0.1,
        ];
        const PTS: [f64; 12] = [
            577638.0, 286092.5, 189687.0, 142001.8, 94527.4, 56664.4, 28313.6, 18381.1,
            14223.2, 9497.0, 5716.9, 2829.7,
        ];

        for i in 0..PTS.len() {
            if PTS[i] <= pts_match {
                return DTS[i];
            }
        }
        DTS[DTS.len() - 1]
    }

    fn update_burst_settings(&mut self, period_seconds: f64) {
        if !period_seconds.is_finite() || period_seconds <= 0.0 {
            self.s_points = 1;
            return;
        }
        let mut burst = if self.burst_duration_value <= -1.0 {
            self.input_burst_duration
        } else {
            self.burst_duration_value
        };
        if !burst.is_finite() || burst <= 0.0 {
            burst = 1e-9;
        }

        let pts_burst = burst / period_seconds;
        self.dt = Self::select_optimal_dt(pts_burst);
        if !self.dt.is_finite() || self.dt <= f64::MIN_POSITIVE {
            self.dt = 0.0015;
        }
        self.s_points = 1;
    }
}

impl PluginDescriptor for RthybridHindmarshRose1984NeuronV2Rust {
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

impl PluginRuntime for RthybridHindmarshRose1984NeuronV2Rust {
    fn set_config_value(&mut self, key: &str, value: &Value) {
        let Some(v) = value.as_f64() else {
            return;
        };
        match key {
            "x" | "x0" => self.x = v,
            "y" | "y0" => self.y = v,
            "z" | "z0" => self.z = v,
            "I" | "i" | "e" | "E" => self.i = v,
            "a" => self.a = v,
            "b" => self.b = v,
            "c" => self.c = v,
            "d" => self.d = v,
            "r" | "mu" => self.r = v,
            "s" | "S" => self.s = v,
            "xr" => self.xr = v,
            "Vh" | "vh" => self.vh = v,
            "Burst duration (s)" => self.burst_duration_value = v,
            _ => {}
        }
    }

    fn set_input_value(&mut self, key: &str, v: f64) {
        let value = if v.is_finite() { v } else { 0.0 };
        match key {
            "Isyn (nA)" => self.input_syn = value,
            "Burst duration (s)" => self.input_burst_duration = if value > 0.0 { value } else { 1.0 },
            _ => {}
        }
    }

    fn process_tick(&mut self, _tick: u64, period_seconds: f64) {
        if !period_seconds.is_finite() || period_seconds <= 0.0 {
            return;
        }

        self.update_burst_settings(period_seconds);

        let steps = self.s_points.clamp(1, 50);
        for _ in 0..steps {
            let prev = [self.x, self.y, self.z];
            let mut vars = prev;
            let i = self.i;
            let a = self.a;
            let b = self.b;
            let c = self.c;
            let d = self.d;
            let r = self.r;
            let s = self.s;
            let xr = self.xr;
            let vh = self.vh;
            let input_syn = self.input_syn;

            rk4_step(&mut vars, self.dt, |st, der| {
                der[0] = st[1] + b * st[0] * st[0] - a * st[0] * st[0] * st[0] - (vh * st[2]) + i
                    - input_syn;
                der[1] = c - d * st[0] * st[0] - st[1];
                der[2] = r * (s * (st[0] - xr) - (st[2] * vh));
            });

            if vars.iter().any(|v| !v.is_finite()) {
                break;
            }
            self.x = vars[0];
            self.y = vars[1];
            self.z = vars[2];
        }

        self.out_0 = self.x / 1000.0;
        self.out_1 = self.x;
    }

    fn get_output_value(&self, key: &str) -> f64 {
        match key {
            "Vm (v)" => self.out_0,
            "Vm (mV)" => self.out_1,
            _ => 0.0,
        }
    }

    fn get_internal_value(&self, key: &str) -> Option<f64> {
        match key {
            "x" => Some(self.x),
            "y" => Some(self.y),
            "z" => Some(self.z),
            _ => None,
        }
    }
}

rtsyn_plugin::export_plugin!(RthybridHindmarshRose1984NeuronV2Rust);
