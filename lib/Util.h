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

    constexpr std::string_view kHelpMessage = R"(
    Usage:
      -h [ --help ]         Show help message
      -i [ --input ] arg    Path to the input file containing space-separated int32_t numbers
      -o [ --output ] arg   Path to the output file where sorted numbers will be saved
      -c [ --config ] arg   Path to the config file containing four integers:
                              - Delay for reading
                              - Delay for writing
                              - Delay for moving left or right
                              - Delay for rewinding the tape)";

}
#endif //TATLIN_UTIL_H
