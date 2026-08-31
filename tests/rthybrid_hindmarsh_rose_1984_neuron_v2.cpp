#include <array>
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>
extern "C" {
#include <rtsyn/node/instance/plugin.h>
}
#include "rtsyn/rthybrid_hindmarsh_rose_1984_neuron_v2.h"

namespace {
struct Host {
    std::array<double, 4> inputs{};
    std::array<double, 4> outputs{};
    std::array<bool, 4> written{};
};
rtsyn_abi_status_t read(const rtsyn_abi_runtime_context_t *context, uint32_t port_index, void *out_value)
{
    if (!context || !out_value || port_index >= 4) return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
    const auto *host = static_cast<const Host *>(context->host_context);
    std::memcpy(out_value, &host->inputs[port_index], sizeof(double));
    return RTSYN_ABI_STATUS_OK;
}
rtsyn_abi_status_t write(const rtsyn_abi_runtime_context_t *context, uint32_t port_index, const void *value)
{
    if (!context || !value || port_index >= 4) return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
    auto *host = const_cast<Host *>(static_cast<const Host *>(context->host_context));
    std::memcpy(&host->outputs[port_index], value, sizeof(double));
    host->written[port_index] = true;
    return RTSYN_ABI_STATUS_OK;
}
bool connected(const rtsyn_abi_runtime_context_t *, uint32_t) { return true; }

TEST(RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2, DescriptorShape)
{
    const auto *descriptor = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_get_descriptor();
    ASSERT_NE(descriptor, nullptr);
    EXPECT_STREQ(descriptor->name, "rthybrid_hindmarsh_rose_1984_neuron_v2");
    EXPECT_EQ(descriptor->node_type, RTSYN_ABI_NODE_PLUGIN);
    EXPECT_EQ(descriptor->port_count, 4U);
    EXPECT_EQ(descriptor->param_count, 13U);
    EXPECT_EQ(descriptor->state_count, 3U);
}

TEST(RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2, DescriptorCreatesRuntimeNode)
{
    const auto *descriptor = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_get_descriptor();

    rtsyn_node_t *node = rtsyn_node_create_plugin(descriptor);

    ASSERT_NE(node, nullptr);
    rtsyn_node_destroy(node);
}

TEST(RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2, ProcessesAndPublishesFiniteOutput)
{
    const auto *descriptor = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_get_descriptor();
    void *instance = nullptr;
    Host host{};
    for (double &input : host.inputs) input = 1.0;
    const rtsyn_abi_runtime_api_t api{read, write, connected};
    const rtsyn_abi_runtime_context_t context{0, 0, 1000000, RTSYN_RUNTIME_CYCLE_STAGE_INTERMEDIATE, &api, &host};
    ASSERT_EQ(descriptor->callbacks.create(&instance), RTSYN_ABI_STATUS_OK);
    ASSERT_NE(instance, nullptr);
    ASSERT_EQ(descriptor->callbacks.start(instance), RTSYN_ABI_STATUS_OK);
    ASSERT_EQ(descriptor->callbacks.process(instance, &context), RTSYN_ABI_STATUS_OK);
    bool saw_output = false;
    for (bool written : host.written) saw_output = saw_output || written;
    EXPECT_TRUE(saw_output);
    descriptor->callbacks.destroy(instance);
}

TEST(RTHYBRID_HINDMARSH_ROSE_1984_NEURON_V2, StateAndInvalidAccess)
{
    const auto *descriptor = rtsyn_rthybrid_hindmarsh_rose_1984_neuron_v2_get_descriptor();
    void *instance = nullptr;
    double value = 1.0;
    ASSERT_EQ(descriptor->callbacks.create(&instance), RTSYN_ABI_STATUS_OK);
    ASSERT_NE(instance, nullptr);
    if (descriptor->state_count > 0) {
        EXPECT_EQ(descriptor->callbacks.read_state(instance, 0, &value), RTSYN_ABI_STATUS_OK);
    }
    EXPECT_EQ(descriptor->callbacks.read_state(instance, descriptor->state_count, &value), RTSYN_ABI_STATUS_INVALID_ARGUMENT);
    EXPECT_EQ(descriptor->callbacks.set_param(instance, descriptor->param_count, &value), RTSYN_ABI_STATUS_INVALID_ARGUMENT);
    descriptor->callbacks.destroy(instance);
}
}
