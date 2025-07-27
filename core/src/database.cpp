#include "arch_package.hpp"
#include <archery.hpp>
#include <filesystem>
#include <arch_parser.hpp>
#include <arch_package_parsing_state.hpp>
#include <iostream>
#include <stdexcept>
#include <archive.h>
#include <archive_entry.h>


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

std::vector<ArchPackage> get_remote_packages(const std::string dbName) {
    std::vector<ArchPackage> packages;
    auto dbPath = get_pacman_lib_path() + "/sync/" + (dbName + ".db");

    struct archive *a;
    struct archive_entry *entry;
    int r;
    
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    
    r = archive_read_open_filename(a, dbPath.c_str(), 10240);
    if (r != ARCHIVE_OK) {
        std::cerr << "Error opening archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        throw std::runtime_error("Failed to open database archive file");
    }
    
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        std::string pathname = archive_entry_pathname(entry);
        
        if (pathname.find("/desc") != std::string::npos) {
            size_t slash_pos = pathname.find('/');
            std::string package_name;
            if (slash_pos != std::string::npos) {
                package_name = pathname.substr(0, slash_pos);
            }
            
            size_t size = archive_entry_size(entry);
            std::vector<char> buffer(size + 1);
            la_ssize_t bytes_read = archive_read_data(a, buffer.data(), size);
            
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                std::string content(buffer.data());
                
                ArchPackage pkg = parse_package_metadata(content);
                packages.push_back(pkg);
            }
        } else {
            archive_read_data_skip(a);
        }
    }
    
    r = archive_read_free(a);
    if (r != ARCHIVE_OK) {
        std::cerr << "Error closing archive: " << archive_error_string(a) << std::endl;
        throw std::runtime_error("Error closing archive");
    }

    return packages;
}