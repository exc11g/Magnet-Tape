#include <iostream>
#include <vector>
#include <tuple>

#include "lib/Util.h"
#include "lib/SortTape.h"

void
ReadArgs(int argc, char **argv, std::filesystem::path &in,
         std::filesystem::path &out, std::filesystem::path &cfg) {
    if (argc != 4) {
        throw std::invalid_argument("Wrong number of arguments, expected 4");
    }
    in = argv[1];
    out = argv[2];
    cfg = argv[3];
}

auto ReadConfig(const std::filesystem::path &path) {
    std::fstream config(path, std::ios::in);
    if (!config.is_open()) {
        throw std::ios::failure("Cannot read config file");
    }
    size_t read, write, move, rewind, memory;
    if (!(config >> read >> write >> move >> rewind >> memory)) {
        throw std::invalid_argument("Not enough params in config file, expected 4");
    }
    return std::tuple(read, write, move, rewind, memory);
}

std::filesystem::path GetBinaryPath(const std::filesystem::path &path) {
    return util::kTempDir / (path.stem().string() + util::kExtension);
}

int main(int argc, char **argv) {
    std::filesystem::path input_path, output_path, config;

    ReadArgs(argc, argv, input_path, output_path, config);
    auto [read_delay, write_delay, move_delay, rewind_delay, memory] = ReadConfig(config);

    std::filesystem::create_directory(util::kTempDir);
    std::filesystem::create_directories(output_path.parent_path());

    std::filesystem::path input_binary = GetBinaryPath(input_path);
    std::filesystem::path output_binary = GetBinaryPath(output_path);

    util::ToBinary(input_path, input_binary);
    {
        SortTape sortTape(input_binary,
                          output_binary,
                          read_delay,
                          write_delay,
                          move_delay,
                          rewind_delay,
                          (memory + util::kIntSize - 1) / util::kIntSize);
        sortTape.Sort();
    }

    util::FromBinary(output_binary, output_path);

    std::filesystem::remove_all(util::kTempDir);
    return 0;
}
