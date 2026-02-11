#include "rthybrid_hindmarsh_rose_1984_neuron_v2_c.h"
#include <float.h>
#include <math.h>
#include <string.h>

static void rthybrid_hindmarsh_rose_1984_neuron_v2_c_update_burst_settings(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s);
static int key_eq(const char *key, size_t len, const char *lit);
static double rthybrid_hindmarsh_rose_1984_neuron_v2_c_select_optimal_dt(double pts_match);
typedef void (*rtsyn_rk4_deriv_fn_t)(const double *state, double *deriv, size_t n, void *user_data);
extern void rtsyn_plugin_rk4_step_n(double *state, size_t n, double dt, rtsyn_rk4_deriv_fn_t deriv_fn, void *user_data);

enum { DT=0, B=1, A=2, I=3, SYN=4, C=5, D=6, R=7, S=8, XR=9, VH=10 };

typedef struct {
  double input_syn;
  double a;
  double b;
  double c;
  double d;
  double r;
  double s;
  double xr;
  double vh;
  double i;
} rthybrid_hindmarsh_rose_1984_neuron_v2_c_ctx_t;

static void rthybrid_hindmarsh_rose_1984_neuron_v2_c_deriv(const double *vars, double *deriv, size_t n, void *user_data) {
  (void)n;
  if (vars == NULL || deriv == NULL || user_data == NULL) return;
  const rthybrid_hindmarsh_rose_1984_neuron_v2_c_ctx_t *ctx =
      (const rthybrid_hindmarsh_rose_1984_neuron_v2_c_ctx_t *)user_data;
  deriv[0] = vars[1] + ctx->b * vars[0] * vars[0] - ctx->a * vars[0] * vars[0] * vars[0] - (ctx->vh * vars[2]) + ctx->i - ctx->input_syn;
  deriv[1] = ctx->c - ctx->d * vars[0] * vars[0] - vars[1];
  deriv[2] = ctx->r * (ctx->s * (vars[0] - ctx->xr) - (vars[2] * ctx->vh));
}

void rthybrid_hindmarsh_rose_1984_neuron_v2_c_init(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s) {
  memset(s, 0, sizeof(*s));
  s->vars[0] = -0.9013747551021072;
  s->vars[1] = -3.15948829665501;
  s->vars[2] = 3.247826955037619;
  s->input_syn = 0.0;
  s->params[I] = 3.0;
  s->params[R] = 0.0021;
  s->params[S] = 4.0;
  s->params[XR] = -1.6;
  s->params[A] = 1.0;
  s->params[B] = 3.0;
  s->params[C] = 1.0;
  s->params[D] = 5.0;
  s->params[VH] = 1.0;
  s->params[DT] = 0.0015;
  s->burst_duration = 1.0;
  s->burst_duration_value = 1.0;
  s->period_seconds = 0.001;
  s->freq = 1000.0;
  s->s_points = 1;
  s->input_burst_duration = 1.0;
}

void rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_config(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, const char *key, size_t len, double value) {
  if (key_eq(key, len, "x") || key_eq(key, len, "x0")) s->vars[0] = value;
  else if (key_eq(key, len, "y") || key_eq(key, len, "y0")) s->vars[1] = value;
  else if (key_eq(key, len, "z") || key_eq(key, len, "z0")) s->vars[2] = value;
  else if (key_eq(key, len, "I") || key_eq(key, len, "i") || key_eq(key, len, "e") || key_eq(key, len, "E")) s->params[I] = value;
  else if (key_eq(key, len, "a")) s->params[A] = value;
  else if (key_eq(key, len, "b")) s->params[B] = value;
  else if (key_eq(key, len, "c")) s->params[C] = value;
  else if (key_eq(key, len, "d")) s->params[D] = value;
  else if (key_eq(key, len, "r") || key_eq(key, len, "mu")) s->params[R] = value;
  else if (key_eq(key, len, "s") || key_eq(key, len, "S")) s->params[S] = value;
  else if (key_eq(key, len, "xr")) s->params[XR] = value;
  else if (key_eq(key, len, "Vh") || key_eq(key, len, "vh") || key_eq(key, len, "VH")) s->params[VH] = value;
  else if (key_eq(key, len, "Burst duration (s)")) s->burst_duration_value = value;
  else if (key_eq(key, len, "period_seconds")) s->period_seconds = value;
  rthybrid_hindmarsh_rose_1984_neuron_v2_c_update_burst_settings(s);
}

void rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_input(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, const char *name, size_t len, double value) {
  if (key_eq(name, len, "Isyn (nA)")) s->input_syn = isfinite(value) ? value : 0.0;
  else if (key_eq(name, len, "Burst duration (s)")) s->input_burst_duration = isfinite(value) ? value : 1.0;
}

void rthybrid_hindmarsh_rose_1984_neuron_v2_c_process(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, double period_seconds) {
  if (!isfinite(period_seconds) || period_seconds <= 0.0) return;
  
  if (fabs(s->period_seconds - period_seconds) > DBL_EPSILON) {
    s->period_seconds = period_seconds;
    s->freq = 1.0 / period_seconds;
    rthybrid_hindmarsh_rose_1984_neuron_v2_c_update_burst_settings(s);
  }
  
  if (!isfinite(s->params[DT]) || s->params[DT] <= DBL_MIN) s->params[DT] = 0.0015;
  if (!isfinite(s->vars[0]) || !isfinite(s->vars[1]) || !isfinite(s->vars[2])) {
    s->vars[0] = -0.9013747551021072;
    s->vars[1] = -3.15948829665501;
    s->vars[2] = 3.247826955037619;
  }

  size_t steps = s->s_points;
  if (steps == 0) steps = 1;
  if (steps > 50) steps = 50;

  rthybrid_hindmarsh_rose_1984_neuron_v2_c_ctx_t ctx = {
      .input_syn = s->input_syn,
      .a = s->params[A],
      .b = s->params[B],
      .c = s->params[C],
      .d = s->params[D],
      .r = s->params[R],
      .s = s->params[S],
      .xr = s->params[XR],
      .vh = s->params[VH],
      .i = s->params[I],
  };

  for (size_t step = 0; step < steps; ++step) {
    double prev[3] = {s->vars[0], s->vars[1], s->vars[2]};
    rtsyn_plugin_rk4_step_n(s->vars, 3, s->params[DT], rthybrid_hindmarsh_rose_1984_neuron_v2_c_deriv, &ctx);
    if (!isfinite(s->vars[0]) || !isfinite(s->vars[1]) || !isfinite(s->vars[2])) {
      s->vars[0] = prev[0];
      s->vars[1] = prev[1];
      s->vars[2] = prev[2];
      break;
    }
  }
}

static double rthybrid_hindmarsh_rose_1984_neuron_v2_c_select_optimal_dt(double pts_match) {
  const double dts[] = {0.0005,0.001,0.0015,0.002,0.003,0.005,0.01,0.015,0.02,0.03,0.05,0.1};
  const double pts[] = {577638.0,286092.5,189687.0,142001.8,94527.4,56664.4,28313.6,18381.1,14223.2,9497.0,5716.9,2829.7};
  for (size_t i = 0; i < 12; ++i) {
    if (pts[i] <= pts_match) return dts[i];
  }
  return dts[11];
}

static void rthybrid_hindmarsh_rose_1984_neuron_v2_c_update_burst_settings(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s) {
  if (s->period_seconds <= 0.0) {
    s->s_points = 1;
    return;
  }
  
  double burst = (s->burst_duration_value <= -1.0) ? s->input_burst_duration : s->burst_duration_value;
  if (burst <= 0.0) burst = 1e-9;
  s->burst_duration = burst;
  
  double pts_burst = burst / s->period_seconds;
  
  s->params[DT] = rthybrid_hindmarsh_rose_1984_neuron_v2_c_select_optimal_dt(pts_burst);
  
  if (!isfinite(s->params[DT]) || s->params[DT] <= DBL_MIN) s->params[DT] = 0.0015;
  
  s->s_points = 1;
}

double rthybrid_hindmarsh_rose_1984_neuron_v2_c_get_output(const rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, const char *key, size_t len) {
  if (key_eq(key, len, "Vm (v)")) return s->vars[0] / 1000.0;
  if (key_eq(key, len, "Vm (mV)")) return s->vars[0];
  return 0.0;
}

static int key_eq(const char *key, size_t len, const char *lit) {
  size_t n = strlen(lit);
  return (len == n) && (strncmp(key, lit, n) == 0);
}
