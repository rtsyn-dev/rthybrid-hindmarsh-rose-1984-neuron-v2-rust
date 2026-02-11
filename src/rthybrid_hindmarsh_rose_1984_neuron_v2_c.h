#pragma once
#include <stddef.h>

typedef struct {
  double vars[3];
  double params[11];
  double burst_duration;
  double burst_duration_value;
  double freq;
  double period_seconds;
  size_t s_points;
  double input_syn;
  double input_burst_duration;
} rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t;

void rthybrid_hindmarsh_rose_1984_neuron_v2_c_init(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s);
void rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_config(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, const char *key, size_t len, double v);
void rthybrid_hindmarsh_rose_1984_neuron_v2_c_set_input(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, const char *key, size_t len, double v);
void rthybrid_hindmarsh_rose_1984_neuron_v2_c_process(rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, double period_seconds);
double rthybrid_hindmarsh_rose_1984_neuron_v2_c_get_output(const rthybrid_hindmarsh_rose_1984_neuron_v2_c_state_t *s, const char *key, size_t len);
