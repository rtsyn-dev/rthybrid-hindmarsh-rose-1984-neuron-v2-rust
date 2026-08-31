local project_name = "rthybrid-hindmarsh-rose-1984-neuron-v2"
local project_xmake_repo = "rtsyn-xmake-repo"

set_license("GPL-3.0-or-later")
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")
set_defaultmode("release")
if is_mode("release") then
    set_optimize("fastest")
    set_strip("all")
    set_symbols("hidden")
end

option("valgrind", { default = false, description = "Run tests with Valgrind" })
option("tests", { default = true, description = "Build tests" })

set_languages("c23", "cxx23")
add_requires("rtsyn-abi")
if has_config("tests") then
    add_requires("gtest")
    add_requires("rtsyn-test-utils", "rtsyn-node", "rtsyn-runtime")
    add_requires("rtsyn-module-loader")
end

local workspace = os.getenv("RTSYN_WORKSPACE")
if workspace then
    add_repositories(project_xmake_repo .. " " .. path.join(workspace, project_xmake_repo))
else
    add_repositories(project_xmake_repo .. " https://github.com/seregioo/" .. project_xmake_repo .. ".git")
end

target(project_name)
set_kind("shared")
add_files("src/*.c")
add_files("src/**/*.c")
add_includedirs("include", { public = true })
add_packages("rtsyn-abi")
add_includedirs("src")
add_headerfiles("include/(rtsyn/**.h)")

local rtsyn_modules = {
    { path = "rthybrid_hindmarsh_rose_1984_neuron_v2", name = "rthybrid_hindmarsh_rose_1984_neuron_v2", link_plugin = true },
    { path = "module_loader", name = "module_loader", load_plugin = true },
}

if has_config("tests") then
    for _, rtsyn_module in ipairs(rtsyn_modules) do
        local tests_name = "tests/" .. rtsyn_module.path .. "-tests"
        target(tests_name)
        set_kind("binary")
        if has_config("valgrind") then
            add_rules("@rtsyn-test-utils/valgrind")
        end
        add_deps(project_name, { links = rtsyn_module.link_plugin == true })
        add_packages("gtest")
        add_packages("rtsyn-abi", "rtsyn-module-loader", "rtsyn-node", "rtsyn-runtime")
        add_links("gtest_main")
        add_includedirs("src")
        add_files("tests/" .. rtsyn_module.path .. ".cpp")
        if rtsyn_module.link_plugin then
            if is_plat("macosx") then
                add_rpathdirs("@loader_path/..")
            elseif not is_plat("windows") then
                add_rpathdirs("$ORIGIN/..")
            end
        end
        if rtsyn_module.load_plugin then
            add_rules("@rtsyn-test-utils/loadable_package", {
                target = project_name,
                define = "RTSYN_TEST_MODULE_PATH",
            })
        end
        add_tests(rtsyn_module.name)
    end
end
