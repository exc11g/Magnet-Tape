#include "../lib/SortTape.h"

#include <exception>
#include <algorithm>
#include <ranges>


SortTape::SortTape(const std::filesystem::path &input_tape, const std::filesystem::path &output_tape, size_t read_delay,
                   size_t write_delay, size_t move_delay, size_t rewind_delay, size_t memory) :
        input_tape_(input_tape, read_delay, write_delay, move_delay, rewind_delay, true),
        output_tape_(output_tape, read_delay, write_delay, move_delay, rewind_delay),
        max_memory_(std::min(memory, input_tape_.GetSize())),
        current_chunk_(max_memory_),
        output_path_(output_tape) {
}

void SortTape::Sort() {
    std::filesystem::create_directory(util::kTempDir);
    if (max_memory_ == 0) {
        SortWithoutMemory();
        return;
    }
    size_t input_size = input_tape_.GetSize();
    size_t chunk_count = (input_size + max_memory_ - 1) / max_memory_;
    for (size_t i = 0; i < chunk_count; ++i) {
        auto size = std::min(max_memory_, input_size - i * max_memory_);
        auto view = current_chunk_ | std::views::take(size);
        for (auto& num : view) {
            num = input_tape_.Read();
            input_tape_.Forward();
        }
        std::ranges::sort(view);
        SaveCurrentChunk(view, i, size);
    }
    Merge();
}

void SortTape::Merge() noexcept {
    std::filesystem::copy_file(output_tape_.GetPath(), output_path_,
                               std::filesystem::copy_options::overwrite_existing);
}

FileTape SortTape::MergeTapes(FileTape &lhs, FileTape &&rhs, size_t i) noexcept {
    FileTape merged = FileTape::Create(util::kTempDir / ("merged_" + std::to_string(i) + util::kExtension), lhs,
                                       lhs.GetSize() + rhs.GetSize());
    int32_t left;
    int32_t right;
    while (!lhs.EndOfTape() && !rhs.EndOfTape()) {
        left = lhs.Read();
        right = rhs.Read();
        if (left < right) {
            merged.Rewrite(left);
            lhs.Forward();
        } else {
            merged.Rewrite(right);
            rhs.Forward();
        }
        merged.Forward();
    }
    while (!lhs.EndOfTape()) {
        merged.Rewrite(lhs.Read());
        merged.Forward();
        lhs.Forward();
    }
    while (!rhs.EndOfTape()) {
        merged.Rewrite(rhs.Read());
        merged.Forward();
        rhs.Forward();
    }
    merged.Rewind();
    return merged;
}

void SortTape::SwapBy(bool forward) {
    int32_t curr = output_tape_.Read();
    output_tape_.ForwardBy(!forward);
    int32_t other = output_tape_.Read();
    output_tape_.Rewrite(curr);
    output_tape_.ForwardBy(forward);
    output_tape_.Rewrite(other);
}

void SortTape::SortWithoutMemory() {
    output_tape_.Fill(input_tape_.GetSize());
    int32_t curr;
    int64_t prev;
    for (size_t i = 0; i < input_tape_.GetSize(); ++i) {
        output_tape_.Rewrite(input_tape_.Read());
        output_tape_.Forward();
        input_tape_.Forward();
    }
    output_tape_.Back();
    int64_t output_size = output_tape_.GetSize();
    bool forward = false;
    for (size_t i = 1; i < output_size; ++i) {
        prev = output_tape_.Read();
        bool is_sorted = true;
        for (size_t j = 1; j < output_size; ++j) {
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

void SortTape::SaveCurrentChunk(auto& view, size_t i, size_t size) noexcept {
    FileTape temp_tape = FileTape::Create(util::GetTempPath(i), input_tape_, size);
    for (auto num : view) {
        temp_tape.Rewrite(num);
        temp_tape.Forward();
    }
    temp_tape.Rewind();
    output_tape_ = MergeTapes(output_tape_, std::move(temp_tape), i);
}
