/**
 * @file rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/process.h
 * @brief process callback declaration for RTHybrid Hindmarsh-Rose 1984 Neuron v2.
 */
#ifndef RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PROCESS_H
#define RTSYN_INTERNAL_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PROCESS_H
#include <rtsyn/abi.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
rtsyn_abi_status_t RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_process(void *instance, const rtsyn_abi_runtime_context_t *context);
#ifdef __cplusplus
}
#endif
#endif
