#include <math.h>
#include <stdlib.h>
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2.h"
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/create.h"

rtsyn_abi_status_t RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_create(void **out_instance)
{
    if (!out_instance) return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
    rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *self = (rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *)malloc(sizeof(rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t));
    if (!self) return RTSYN_ABI_STATUS_ERROR;

    self->x = -0.9013747551021072;
    self->y = -3.15948829665501;
    self->z = 3.247826955037619;
    self->e = 3.0;
    self->a = 1.0;
    self->b = 3.0;
    self->c = 1.0;
    self->d = 5.0;
    self->mu = 0.0021;
    self->s = 4.0;
    self->xr = -1.6;
    self->vh = 1.0;
    self->dt = 0.0015;
    self->burst_duration = 1.0;
    self->burst_duration_value = 1.0;
    self->period_seconds = 0.001;
    self->input_syn = 0.0;
    self->input_burst_duration = 1.0;

    *out_instance = self;
    return RTSYN_ABI_STATUS_OK;
}
