/**
 * @file rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/read_state.h
 * @brief read_state callback declaration for RTHybrid Hindmarsh-Rose 1984 Neuron v2.
 */
#ifndef RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_READ_STATE_H
#define RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_READ_STATE_H
#include <rtsyn/abi.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
rtsyn_abi_status_t RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_read_state(const void *instance, uint32_t state_index, void *out_value);
#ifdef __cplusplus
}
#endif
#endif
