#include "../lib/Util.h"

void util::ToBinary(const std::filesystem::path &path, const std::filesystem::path &binary) {
    std::ifstream ifstream(path, std::ios::in);
    std::ofstream ofstream(binary, std::ios::out | std::ios::binary);
    int32_t curr;
    if (!ifstream.is_open()) {
        throw std::ios::failure("No such input file: " + path.string());
    }
    while (ifstream >> curr) {
        ofstream.write(reinterpret_cast<char *>(&curr), util::kIntSize);
    }
}

void util::FromBinary(const std::filesystem::path &input, const std::filesystem::path &output) noexcept {
    std::ifstream ifstream(input, std::ios::binary | std::ios::in);
    std::ofstream ofstream(output, std::ios::out);
    int32_t curr;

    while (ifstream.read(reinterpret_cast<char *>(&curr), util::kIntSize)) {
        ofstream << curr << " ";
    }
}

std::filesystem::path util::GetTempPath(size_t i) noexcept {
    return util::kTempDir / ("temp_" + std::to_string(i) + util::kExtension);
}