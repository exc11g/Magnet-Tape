#ifndef TATLIN_SORTTAPE_H
#define TATLIN_SORTTAPE_H

#include <fstream>
#include <iostream>
#include <filesystem>

#include "TapeInterface.h"

const size_t kIntSize = 4;

void ToBinary(const std::filesystem::path &path, const std::filesystem::path &binary);

void FromBinary(const std::filesystem::path &input, const std::filesystem::path &output) noexcept;

class FileTape : TapeInterface {
public:

    explicit FileTape(const std::filesystem::path &path, size_t read_delay,
                      size_t write_delay, size_t move_delay, size_t rewind_delay, bool is_input = false);

    FileTape() = default;

    void Rewrite(int32_t num) noexcept override;

    int32_t Read() noexcept override;

    void Forward() noexcept override;

    void ForwardBy(bool forward) noexcept;

    void Back() noexcept override;

    size_t GetSize() const noexcept;

    void Fill(size_t n) noexcept;

    void Rewind() noexcept;

private:
    std::fstream file_tape_;
    size_t tape_size_;
    size_t read_delay_;
    size_t write_delay_;
    size_t move_delay_;
    size_t rewind_delay_;
};

class SortTape {
public:
    SortTape(const std::filesystem::path &input_tape, const std::filesystem::path &output_tape, size_t read_delay,
             size_t write_delay, size_t move_delay, size_t rewind_delay) :
            input_tape_(input_tape, read_delay, write_delay, move_delay, rewind_delay, true),
            output_tape_(output_tape, read_delay, write_delay, move_delay, rewind_delay) {
    }

    void Sort();

    void SwapBy(bool forward);

private:
    FileTape input_tape_;
    FileTape output_tape_;
};


#endif //TATLIN_SORTTAPE_H
