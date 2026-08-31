#include <math.h>
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2.h"
#include "rtsyn/internal/rthybrid_hindmarsh_rose_1984_neuron_v2/set_param.h"
static double finite_or(double value, double fallback) { return isfinite(value) ? value : fallback; }
rtsyn_abi_status_t RTSYN_ABI_CALL rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_set_param(void *instance, uint32_t param_index, const void *value)
{
    rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *self = (rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_instance_t *)instance;
    if (!self || !value) return RTSYN_ABI_STATUS_INVALID_ARGUMENT;

    switch (param_index) {
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_X0: self->x = finite_or(*(const double *)value, self->x); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_Y0: self->y = finite_or(*(const double *)value, self->y); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_Z0: self->z = finite_or(*(const double *)value, self->z); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_E: self->e = finite_or(*(const double *)value, 3.0); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_A: self->a = finite_or(*(const double *)value, 1.0); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_B: self->b = finite_or(*(const double *)value, 3.0); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_C: self->c = finite_or(*(const double *)value, 1.0); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_D: self->d = finite_or(*(const double *)value, 5.0); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_MU: self->mu = finite_or(*(const double *)value, 0.0021); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_S: self->s = finite_or(*(const double *)value, 4.0); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_XR: self->xr = finite_or(*(const double *)value, -1.6); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_VH: self->vh = finite_or(*(const double *)value, 1.0); return RTSYN_ABI_STATUS_OK;
    case RTSYN_RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2_PARAM_BURST_DURATION: self->burst_duration_value = finite_or(*(const double *)value, 1.0); return RTSYN_ABI_STATUS_OK;
    default: return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
    }

}
