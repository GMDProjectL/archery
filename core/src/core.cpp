#include <archery.hpp>
#include <filesystem>
#include <arch_parser.hpp>
#include <arch_package_parsing_state.hpp>
#include <stdexcept>


std::string get_pacman_lib_path() {
    return "/var/lib/pacman";
}

std::vector<ArchPackage> get_local_packages() {
    std::vector<ArchPackage> packages;

    auto dirPath = std::filesystem::path(get_pacman_lib_path()) / "local";

    if (!std::filesystem::exists(dirPath)) {
        throw std::runtime_error("Local package directory does not exist: " + dirPath.string());
    }

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_directory()) {
            auto pkgPath = entry.path();
            auto descPath = pkgPath / "desc";
            auto pkg = parse_package_desc(descPath);

            packages.push_back(pkg);
        }
    }

    return packages;
}