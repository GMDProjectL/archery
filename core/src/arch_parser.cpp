#include <arch_parser.hpp>
#include <arch_package_parsing_state.hpp>
#include <fstream>



ArchPackage parse_package_metadata(const std::string& contents) {
    std::string line;
    std::istringstream stream(contents);

    ArchPackage pkg;
    ArchPackageParsingState state = ArchPackageParsingState::PARSING_NONE;

    while (std::getline(stream, line)) {
        // Process each line as needed
        if (line == "%NAME%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_NAME;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_NAME) {
            pkg.m_name = line;
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%VERSION%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_VERSION;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_VERSION) {
            pkg.m_version = line;
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%DESC%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_DESCRIPTION;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_DESCRIPTION) {
            pkg.m_description = line;
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%FILENAME%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_FILENAME;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_FILENAME) {
            pkg.m_filename = line;
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%SHA256SUM%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_SHA256;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_SHA256) {
            pkg.m_sha256 = line;
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%DEPENDS%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_DEPENDENCIES;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_DEPENDENCIES && !line.empty()) {
            pkg.m_dependencies.push_back(line);
        }
        if (line.empty() && state == ArchPackageParsingState::PARSING_DEPENDENCIES) {
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%OPTDEPENDS%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_OPTIONAL_DEPENDENCIES;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_OPTIONAL_DEPENDENCIES && !line.empty()) {
            if (!pkg.m_optionalDependencies.has_value()) {
                pkg.m_optionalDependencies = std::vector<std::string>();
            }
            pkg.m_optionalDependencies.value().push_back(line);
        }
        if (line.empty() && state == ArchPackageParsingState::PARSING_OPTIONAL_DEPENDENCIES) {
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%CONFLICTS%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_CONFLICTS;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_CONFLICTS && !line.empty()) {
            if (!pkg.m_conflicts.has_value()) {
                pkg.m_conflicts = std::vector<std::string>();
            }
            pkg.m_conflicts.value().push_back(line);
        }
        if (line.empty() && state == ArchPackageParsingState::PARSING_CONFLICTS) {
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%LICENSE%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_LICENSE;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_LICENSE) {
            pkg.m_license = line;
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if ((line == "%SIZE%" || line == "%ISIZE%") && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_UNCOMPRESSED_SIZE;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_UNCOMPRESSED_SIZE) {
            try {
                pkg.m_uncompressedSize = std::stoi(line);
            }
            catch (const std::out_of_range& e) {
                // We dont talk about this
            }
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }

        if (line == "%CSIZE%" && state == ArchPackageParsingState::PARSING_NONE) {
            state = ArchPackageParsingState::PARSING_COMPRESSED_SIZE;
            continue;
        }
        if (state == ArchPackageParsingState::PARSING_COMPRESSED_SIZE) {

            try {
                pkg.m_compressedSize = std::stoi(line);
            }
            catch (const std::out_of_range& e) {
                // This one too
            }
            state = ArchPackageParsingState::PARSING_NONE;
            continue;
        }
    }

    return pkg;
}


ArchPackage parse_package_desc(const std::filesystem::path descFilePath) {
    if (!std::filesystem::exists(descFilePath)) {
        throw std::runtime_error("Package description file does not exist: " + descFilePath.string());
    }

    // read the file

    auto fileDescriptor = std::ifstream(descFilePath);
    if (!fileDescriptor.is_open()) {
        throw std::runtime_error("Failed to open package description file: " + descFilePath.string());
    }

    std::stringstream buffer;
    buffer << fileDescriptor.rdbuf();

    return parse_package_metadata(buffer.str());
}