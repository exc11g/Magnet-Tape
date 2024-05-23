#include <iostream>
#include <vector>
#include <tuple>

#include <boost/program_options.hpp>

#include "lib/Util.h"
#include "lib/SortTape.h"

namespace po = boost::program_options;

auto ReadArgs(int argc, char **argv) {
    std::filesystem::path in, out, cfg;
    po::options_description cmd_options("CMD options");
    cmd_options.add_options()
            ("help,h", "produce help message")
            ("input,i", po::value<std::filesystem::path>(), "get input tape")
            ("output,o", po::value<std::filesystem::path>(), "get output tape")
            ("config,c", po::value<std::filesystem::path>(), "get config file");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmd_options), vm);
    po::notify(vm);
    if (vm.contains("help")) {
        std::cout << util::kHelpMessage << std::endl;
    }
    try {
        in = vm["input"].as<std::filesystem::path>();
        out = vm["output"].as<std::filesystem::path>();
        cfg = vm["config"].as<std::filesystem::path>();
    } catch (...) {
        throw std::invalid_argument("Not enough arguments, use --help for more information");
    }
    return std::tuple(in, out, cfg);
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
    auto [input_path, output_path, config] = ReadArgs(argc, argv);
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
                          memory / util::kIntSize);
        sortTape.Sort();
    }

    util::FromBinary(output_binary, output_path);

    std::filesystem::remove_all(util::kTempDir);
    return 0;
}
