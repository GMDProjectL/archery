#include <cli.hpp>
#include <archery.hpp>
#include <iostream>

int main(int, char**){
    auto packages = get_local_packages();
    for (auto& package : packages) {
        std::cout << package.m_name << std::endl;
        if (package.m_uncompressedSize == 0) {
            std::cerr << "Error: Package " << package.m_name << " has no size." << std::endl;
            return 1;
        }
    }
    return 0;
}
