#include <stdlib.h>
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2.h"
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/destroy.h"
void RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_destroy(void *instance)
{
    free((rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *)instance);
}
