#pragma once
#include <arch_package.hpp>

std::vector<ArchPackage> get_local_packages();
std::vector<ArchPackage> get_remote_packages(std::string dbName);

std::string get_pacman_lib_path();