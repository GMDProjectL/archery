#include <cli.hpp>
#include <archery.hpp>
#include <iostream>
#include <format>

int main(int, char**){
    std::string dbName = "core";
    
    auto packages = get_remote_packages(dbName);
    for (auto& package : packages) {
        std::cout << std::format("{} (version {}) - {} bytes", package.m_name, package.m_version, package.m_uncompressedSize) << std::endl;
    }
    return 0;
}
