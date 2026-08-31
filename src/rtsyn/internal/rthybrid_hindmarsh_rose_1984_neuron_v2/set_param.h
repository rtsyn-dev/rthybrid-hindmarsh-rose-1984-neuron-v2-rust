/**
 * @file rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/set_param.h
 * @brief set_param callback declaration for RTHybrid Hindmarsh-Rose 1984 Neuron v2.
 */
#ifndef RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_SET_PARAM_H
#define RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_SET_PARAM_H
#include <rtsyn/abi.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
rtsyn_abi_status_t RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_set_param(void *instance, uint32_t param_index, const void *value);
#ifdef __cplusplus
}
#endif
#endif
