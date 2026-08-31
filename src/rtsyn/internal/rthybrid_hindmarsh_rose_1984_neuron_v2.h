/**
     * @file rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2.h
     * @brief Internal state for the RTHybrid Hindmarsh-Rose 1984 Neuron v2 module.
     *
     * SPDX-License-Identifier: GPL-3.0-or-later
     */
    #ifndef RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_H
    #define RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_H

    #include <stddef.h>
    #include <stdint.h>

    #ifdef __cplusplus
    extern "C" {
    #endif

    typedef struct rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_s {
        double x;
double y;
double z;
double e;
double a;
double b;
double c;
double d;
double mu;
double s;
double xr;
double vh;
double dt;
double burst_duration;
double burst_duration_value;
double period_seconds;
double input_syn;
double input_burst_duration;
    } rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t;

    typedef enum rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_ports_e : uint32_t {
        RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_ISYN_NA = 0,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_BURST_DURATION_S = 1,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_VM_V = 2,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_VM_MV = 3,
        RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_COUNT = 4,
    } rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_ports_t;

    typedef enum rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_params_e : uint32_t {
        RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_X0 = 0,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_Y0 = 1,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_Z0 = 2,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_E = 3,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_A = 4,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_B = 5,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_C = 6,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_D = 7,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_MU = 8,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_S = 9,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_XR = 10,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_VH = 11,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_BURST_DURATION = 12,
        RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_COUNT = 13,
    } rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_params_t;

    typedef enum rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_states_e : uint32_t {
        RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_X = 0,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_Y = 1,
RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_Z = 2,
        RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_COUNT = 3,
    } rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_states_t;

    #ifdef __cplusplus
    }
    #endif

    #endif /* RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_H */
