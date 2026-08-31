#include <stddef.h>
        #include "rtsyn/rthybrid_hindmarsh_rose_1984_neuron_v2.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/create.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/destroy.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/process.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/read_state.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/set_param.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/start.h"
        #include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/stop.h"

        static const rtsyn_abi_port_descriptor_t rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_ports[RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_COUNT] = {
            [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_ISYN_NA] =
        {
            .name = "Isyn (nA)",
            .value_type = RTSYN_ABI_VALUE_F64,
            .direction = RTSYN_ABI_PORT_DIRECTION_IN,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_BURST_DURATION_S] =
        {
            .name = "Burst duration (s)",
            .value_type = RTSYN_ABI_VALUE_F64,
            .direction = RTSYN_ABI_PORT_DIRECTION_IN,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_VM_V] =
        {
            .name = "Vm (v)",
            .value_type = RTSYN_ABI_VALUE_F64,
            .direction = RTSYN_ABI_PORT_DIRECTION_OUT,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_VM_MV] =
        {
            .name = "Vm (mV)",
            .value_type = RTSYN_ABI_VALUE_F64,
            .direction = RTSYN_ABI_PORT_DIRECTION_OUT,
        },
        };

        static const rtsyn_abi_param_descriptor_t rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_params[RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_COUNT] = {
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_X0] =
        {
            .name = "x0",
            .description = "Initial x state",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_Y0] =
        {
            .name = "y0",
            .description = "Initial y state",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_Z0] =
        {
            .name = "z0",
            .description = "Initial z state",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_E] =
        {
            .name = "e",
            .description = "Injected current",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_A] =
        {
            .name = "a",
            .description = "Hindmarsh-Rose a parameter",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_B] =
        {
            .name = "b",
            .description = "Hindmarsh-Rose b parameter",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_C] =
        {
            .name = "c",
            .description = "Hindmarsh-Rose c parameter",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_D] =
        {
            .name = "d",
            .description = "Hindmarsh-Rose d parameter",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_MU] =
        {
            .name = "mu",
            .description = "Slow adaptation parameter",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_S] =
        {
            .name = "S",
            .description = "Slow current scale",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_XR] =
        {
            .name = "xr",
            .description = "Resting x value",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_VH] =
        {
            .name = "Vh",
            .description = "z coupling scale",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_BURST_DURATION] =
        {
            .name = "burst_duration",
            .description = "Burst duration in seconds, or -1 to use the input port",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
};

        static const rtsyn_abi_state_descriptor_t rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_states[RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_COUNT] = {
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_X] =
        {
            .name = "x",
            .description = "x state",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_Y] =
        {
            .name = "y",
            .description = "y state",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
    [RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_Z] =
        {
            .name = "z",
            .description = "z state",
            .value_type = RTSYN_ABI_VALUE_F64,
        },
};

        static const rtsyn_abi_node_descriptor_t rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_descriptor = {
            .name = "rthybrid_hindmarsh_rose_1984_neuron_v2",
            .node_type = RTSYN_ABI_NODE_PLUGIN,
            .port_count = RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PORT_COUNT,
            .ports = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_ports,
            .param_count = RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_COUNT,
            .params = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_params,
            .state_count = RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_STATE_COUNT,
            .states = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_states,
            .callbacks = {
                .create = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_create,
                .set_param = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_set_param,
                .read_state = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_read_state,
                .start = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_start,
                .process = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_process,
                .stop = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_stop,
                .destroy = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_destroy,
            },
        };

        RTSYN_ABI_EXPORT const rtsyn_abi_node_descriptor_t *RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_get_descriptor(void)
        {
            return &rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_descriptor;
        }

        RTSYN_ABI_EXPORT const rtsyn_abi_node_descriptor_t *RTSYN_ABI_CALL rtsyn_module_get_descriptor(void)
        {
            return rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_get_descriptor();
        }
