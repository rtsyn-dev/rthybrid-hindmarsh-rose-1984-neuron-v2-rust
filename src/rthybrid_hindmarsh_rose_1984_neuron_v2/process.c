#include <math.h>
#include <string.h>
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2.h"
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/process.h"

static double period_seconds_from_context(const rtsyn_abi_runtime_context_t *context)
{
    if (!context || context->period_ns == 0U) return 0.001;
    return (double)context->period_ns / 1000000000.0;
}

static int read_f64(const rtsyn_abi_runtime_context_t *context, uint32_t port_index, double *out_value)
{
    if (!context || !context->api || !context->api->read || !out_value) return 0;
    double value = 0.0;
    if (context->api->read(context, port_index, &value) != RTSYN_ABI_STATUS_OK) return 0;
    if (!isfinite(value)) return 0;
    *out_value = value;
    return 1;
}

static void clamp_f64(double *value, double fallback)
{
    if (!isfinite(*value)) {
        *value = fallback;
    } else if (fabs(*value) > 1e6) {
        *value = *value > 0.0 ? 1e6 : -1e6;
    }
}

typedef void (*rtsyn_rk4_deriv_fn_t)(const double *state, double *deriv, void *user_data);
static void rk4_step(double *state, size_t n, double dt, rtsyn_rk4_deriv_fn_t deriv_fn, void *user_data)
{
    double k1[8] = {0};
    double k2[8] = {0};
    double k3[8] = {0};
    double k4[8] = {0};
    double tmp[8] = {0};
    if (!state || !deriv_fn || n == 0 || n > 8 || dt <= 0.0) return;
    deriv_fn(state, k1, user_data);
    for (size_t i = 0; i < n; ++i) tmp[i] = state[i] + 0.5 * dt * k1[i];
    deriv_fn(tmp, k2, user_data);
    for (size_t i = 0; i < n; ++i) tmp[i] = state[i] + 0.5 * dt * k2[i];
    deriv_fn(tmp, k3, user_data);
    for (size_t i = 0; i < n; ++i) tmp[i] = state[i] + dt * k3[i];
    deriv_fn(tmp, k4, user_data);
    for (size_t i = 0; i < n; ++i) state[i] += (dt / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
}


    typedef struct {
        double input_syn;
        double a;
        double b;
        double c;
        double d;
        double mu;
        double s;
        double xr;
        double e;
        double vh;
    } rthybrid_hindmarsh_rose_1984_neuron_v2_ctx_t;

    static void deriv(const double *state, double *dstate, void *user_data) {
        const rthybrid_hindmarsh_rose_1984_neuron_v2_ctx_t *ctx = (const rthybrid_hindmarsh_rose_1984_neuron_v2_ctx_t *)user_data;
        const double x = state[0];
        const double y = state[1];
        const double self_z = state[2];
        dstate[0] = y + ctx->b * x * x - ctx->a * x * x * x - (ctx->vh * self_z) + ctx->e - ctx->input_syn;
        dstate[1] = ctx->c - ctx->d * x * x - y;
        dstate[2] = ctx->mu * (ctx->s * (x - ctx->xr) - (self_z * ctx->vh));
    }

    static double select_hr_dt(double pts_match) {
        static const double dts[] = {0.0005,0.001,0.0015,0.002,0.003,0.005,0.01,0.015,0.02,0.03,0.05,0.1};
        static const double pts[] = {577638.0,286092.5,189687.0,142001.8,94527.4,56664.4,28313.6,18381.1,14223.2,9497.0,5716.9,2829.7};
        for (size_t i = 0; i < 12; ++i) {
            if (pts[i] <= pts_match) return dts[i];
        }
        return dts[11];
    }

    static size_t hr_steps(rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *self, double period_seconds) {
        double burst = self->burst_duration_value <= -1.0 ? self->input_burst_duration : self->burst_duration_value;
        if (burst <= 0.0) burst = 1e-9;
        self->burst_duration = burst;
        self->dt = select_hr_dt(burst / period_seconds);
        if (!isfinite(self->dt) || self->dt <= 0.0) self->dt = 0.0015;
        return 1;
    }


rtsyn_abi_status_t RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_process(void *instance, const rtsyn_abi_runtime_context_t *context)
{
    rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *self = (rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *)instance;
    if (!self || !context || !context->api || !context->api->write)
        return RTSYN_ABI_STATUS_INVALID_ARGUMENT;

    (void)read_f64(context, RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_ISYN_NA, &self->input_syn);
    (void)read_f64(context, RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_BURST_DURATION_S, &self->input_burst_duration);
    const double period_seconds = period_seconds_from_context(context);
    self->period_seconds = period_seconds;
    if (!isfinite(self->x) || !isfinite(self->y) || !isfinite(self->z)) {
        self->x = -0.9013747551021072;
        self->y = -3.15948829665501;
        self->z = 3.247826955037619;
    }
    rthybrid_hindmarsh_rose_1984_neuron_v2_ctx_t rkctx = {
        .input_syn = self->input_syn,
        .a = self->a,
        .b = self->b,
        .c = self->c,
        .d = self->d,
        .mu = self->mu,
        .s = self->s,
        .xr = self->xr,
        .e = self->e, .vh = self->vh
    };
    double state[3] = {self->x, self->y, self->z};
    size_t steps = hr_steps(self, period_seconds);
    for (size_t i = 0; i < steps; ++i) {
        double previous[3] = {state[0], state[1], state[2]};
        rk4_step(state, 3, self->dt, deriv, &rkctx);
        if (!isfinite(state[0]) || !isfinite(state[1]) || !isfinite(state[2])) {
            state[0] = previous[0]; state[1] = previous[1]; state[2] = previous[2];
            break;
        }
    }
    self->x = state[0]; self->y = state[1]; self->z = state[2];
    double vm_v = self->x / 1000.0;
    double vm_mv = self->x;
    (void)context->api->write(context, RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_VM_V, &vm_v);
    (void)context->api->write(context, RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_VM_MV, &vm_mv);
    return RTSYN_ABI_STATUS_OK;

}
