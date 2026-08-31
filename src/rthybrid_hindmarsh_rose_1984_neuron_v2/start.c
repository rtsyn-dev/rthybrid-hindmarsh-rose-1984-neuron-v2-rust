#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2.h"
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/start.h"
rtsyn_abi_status_t RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_start(void *instance)
{
    return instance ? RTSYN_ABI_STATUS_OK : RTSYN_ABI_STATUS_INVALID_ARGUMENT;
}
