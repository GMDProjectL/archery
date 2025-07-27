#pragma once
#include <string>
#include <vector>
#include <optional>

class ArchPackage {
public:
    std::string m_name;
    std::string m_version;
    std::string m_description;
    std::optional<std::string> m_filename;
    std::optional<std::string> m_sha256;
    std::vector<std::string> m_dependencies;
    std::optional<std::vector<std::string>> m_optionalDependencies;
    std::optional<std::vector<std::string>> m_conflicts;
    std::optional<std::string> m_license;
    int m_compressedSize; // In bytes
    int m_uncompressedSize; // In bytes
};