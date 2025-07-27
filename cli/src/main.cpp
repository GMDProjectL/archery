#include <cli.hpp>
#include <archery.hpp>
#include <iostream>
#include <format>

int main(int, char**){
    std::string dbName = "core";

    auto packages = get_remote_packages(dbName);
    for (auto& package : packages) {
        std::cout << std::format(
            "{} (version {}) - {:.2f} MiB\n", 
            package.m_name, package.m_version, (package.m_uncompressedSize / 1024.f / 1024.f)
        );
    }
    return 0;
}
