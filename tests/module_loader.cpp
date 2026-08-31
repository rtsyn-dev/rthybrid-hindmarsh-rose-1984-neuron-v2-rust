#include <gtest/gtest.h>
#include <rtsyn/module_loader.h>
extern "C" {
#include <rtsyn/node/instance/plugin.h>
#include <rtsyn/runtime.h>
#include <rtsyn/runtime/config.h>
}

#ifndef RTSYN_TEST_MODULE_PATH
#error RTSYN_TEST_MODULE_PATH must contain the built plugin path
#endif

TEST(ModuleLoader, LoadsDescriptor)
{
    rtsyn_module_loader_t *loader = rtsyn_module_loader_create(RTSYN_TEST_MODULE_PATH);
    ASSERT_NE(loader, nullptr);
    const rtsyn_abi_node_descriptor_t *descriptor =
        rtsyn_module_loader_get_descriptor(loader);
    ASSERT_NE(descriptor, nullptr);
    EXPECT_STREQ(descriptor->name, "rthybrid_hindmarsh_rose_1984_neuron_v2");
    rtsyn_module_loader_destroy(loader);
}

TEST(ModuleLoader, LoadedDescriptorCreatesRuntimeNode)
{
    rtsyn_module_loader_t *loader = rtsyn_module_loader_create(RTSYN_TEST_MODULE_PATH);
    ASSERT_NE(loader, nullptr);

    const rtsyn_abi_node_descriptor_t *descriptor =
        rtsyn_module_loader_get_descriptor(loader);
    ASSERT_NE(descriptor, nullptr);

    rtsyn_node_t *node = rtsyn_node_create_plugin(descriptor);
    ASSERT_NE(node, nullptr);

    rtsyn_node_destroy(node);
    rtsyn_module_loader_destroy(loader);
}

TEST(ModuleLoader, LoadedDescriptorCanBeAddedToRuntime)
{
    rtsyn_module_loader_t *loader = rtsyn_module_loader_create(RTSYN_TEST_MODULE_PATH);
    ASSERT_NE(loader, nullptr);

    const rtsyn_abi_node_descriptor_t *descriptor =
        rtsyn_module_loader_get_descriptor(loader);
    ASSERT_NE(descriptor, nullptr);

    rtsyn_runtime_config_t config{};
    rtsyn_runtime_config_init(&config);
    rtsyn_runtime_t *runtime = rtsyn_runtime_create(&config);
    ASSERT_NE(runtime, nullptr);

    rtsyn_node_t *node = rtsyn_node_create_plugin(descriptor);
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(rtsyn_runtime_add_node(runtime, node));

    rtsyn_runtime_destroy(runtime);
    rtsyn_module_loader_destroy(loader);
}
