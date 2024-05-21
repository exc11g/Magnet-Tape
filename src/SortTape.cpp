#include "../lib/SortTape.h"

#include <thread>
#include <exception>

void ToBinary(const std::filesystem::path &path, const std::filesystem::path &binary) {
    std::ifstream ifstream(path, std::ios::in);
    std::ofstream ofstream(binary, std::ios::out | std::ios::binary);
    int32_t curr;
    if (!ifstream.is_open()) {
        throw std::ios::failure("No such input file: " + path.string());
    }
    while (ifstream >> curr) {
        ofstream.write(reinterpret_cast<char *>(&curr), kIntSize);
    }
}

void FromBinary(const std::filesystem::path &input, const std::filesystem::path &output) noexcept {
    std::ifstream ifstream(input, std::ios::binary | std::ios::in);
    std::ofstream ofstream(output, std::ios::out);
    int32_t curr;

    while (ifstream.read(reinterpret_cast<char *>(&curr), kIntSize)) {
        ofstream << curr << " ";
    }
}

void FileTape::Rewrite(int32_t num) noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(write_delay_));
    auto curr_pos = file_tape_.tellg();
    file_tape_.write(reinterpret_cast<char *>(&num), kIntSize);
    file_tape_.seekg(curr_pos);
}

int32_t FileTape::Read() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(read_delay_));
    int32_t res;
    auto curr_pos = file_tape_.tellg();
    file_tape_.read(reinterpret_cast<char *>(&res), kIntSize);
    file_tape_.seekg(curr_pos);
    return res;
}

void FileTape::Forward() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(move_delay_));
    file_tape_.seekg(kIntSize, std::ios::cur);
}

void FileTape::Back() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(move_delay_));
    file_tape_.seekg(-static_cast<int64_t>(kIntSize), std::ios::cur);
}

size_t FileTape::GetSize() const noexcept {
    return tape_size_;
}

FileTape::FileTape(const std::filesystem::path &path, size_t read_delay,
                   size_t write_delay, size_t move_delay, size_t rewind_delay, bool is_input) : read_delay_(read_delay),
                                                                                                write_delay_(
                                                                                                        write_delay),
                                                                                                move_delay_(move_delay),
                                                                                                rewind_delay_(
                                                                                                        rewind_delay) {
    if (!is_input) {
        file_tape_ = std::fstream(path, std::ios::out |
                                        std::ios::binary |
                                        std::ios::in |
                                        std::ios::trunc);
        return;
    }
    file_tape_ = std::fstream(path, std::ios::ate | std::ios::binary | std::ios::in);
    if (!file_tape_.is_open()) {
        throw std::ios::failure("file cannot be opened: " + path.string());
    }
    tape_size_ = file_tape_.tellg() / kIntSize;
    file_tape_.seekg(std::ios::beg);
}

void FileTape::Fill(size_t n) noexcept {
    int32_t zero = 0;
    for (size_t i = 0; i < n; ++i) {
        file_tape_.write(reinterpret_cast<char *>(&zero), kIntSize);
    }
    tape_size_ = n;
    file_tape_.seekg(std::ios::beg);
}

void FileTape::ForwardBy(bool forward) noexcept {
    if (forward) {
        Forward();
    } else {
        Back();
    }
}

[[maybe_unused]] void FileTape::Rewind() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(rewind_delay_));
    file_tape_.seekg(std::ios::beg);
}

void SortTape::Sort() {
    output_tape_.Fill(input_tape_.GetSize());
    int32_t curr;
    int64_t prev;
    for (size_t i = 0; i < input_tape_.GetSize(); ++i) {
        output_tape_.Rewrite(input_tape_.Read());
        output_tape_.Forward();
        input_tape_.Forward();
    }
    output_tape_.Back();
    bool forward = false;
    for (size_t i = 0; i < output_tape_.GetSize(); ++i) {
        prev = output_tape_.Read();
        bool is_sorted = true;
        for (size_t j = 1; j < output_tape_.GetSize(); ++j) {
            output_tape_.ForwardBy(forward);
            curr = output_tape_.Read();
            if (curr < prev && forward || curr > prev && !forward) {
                SwapBy(forward);
                is_sorted = false;
            }
            prev = curr;
        }
        if (is_sorted) {
            break;
        }
        forward = (1 + forward) % 2;
    }
}

void SortTape::SwapBy(bool forward) {
    int32_t curr = output_tape_.Read();
    output_tape_.ForwardBy(!forward);
    int32_t other = output_tape_.Read();
    output_tape_.Rewrite(curr);
    output_tape_.ForwardBy(forward);
    output_tape_.Rewrite(other);
}
