#ifndef TATLIN_SORTTAPE_H
#define TATLIN_SORTTAPE_H

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include "TapeInterface.h"
#include "Util.h"
#include "FileTape.h"

class SortTape {
public:
    SortTape(const std::filesystem::path &input_tape, const std::filesystem::path &output_tape, size_t read_delay,
             size_t write_delay, size_t move_delay, size_t rewind_delay, size_t memory);

    void Sort();

    void SortWithoutMemory();

    void SwapBy(bool forward);

    void Merge() noexcept;

    static FileTape MergeTapes(FileTape &lhs, FileTape &rhs, size_t i) noexcept;

    void SaveCurrentChunk(auto& view, size_t i, size_t size) noexcept;

private:
    FileTape input_tape_;
    FileTape output_tape_;
    size_t max_memory_;
    std::vector<int32_t> current_chunk_;
    std::vector<FileTape> temp_tapes_;
};

#endif //TATLIN_SORTTAPE_H
