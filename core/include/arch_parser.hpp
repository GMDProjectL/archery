#pragma once
#include <arch_package.hpp>
#include <filesystem>

ArchPackage parse_package_desc(const std::filesystem::path descFilePath);