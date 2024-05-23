#ifndef TATLIN_UTIL_H
#define TATLIN_UTIL_H

#include <filesystem>
#include <fstream>

namespace util {

    const std::filesystem::path kTempDir = "./tmp/";
    constexpr std::string kExtension = "_binary.tape";
    constexpr size_t kIntSize = 4;

    void ToBinary(const std::filesystem::path &path, const std::filesystem::path &binary);

    void FromBinary(const std::filesystem::path &input, const std::filesystem::path &output) noexcept;

    std::filesystem::path GetTempPath(size_t i) noexcept;

}
#endif //TATLIN_UTIL_H
