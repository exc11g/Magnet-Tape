#ifndef TATLIN_FILETAPE_H
#define TATLIN_FILETAPE_H

#include <thread>
#include <filesystem>
#include <fstream>

#include "TapeInterface.h"

class FileTape : TapeInterface {
public:

    explicit FileTape(const std::filesystem::path &path, size_t read_delay,
                      size_t write_delay, size_t move_delay, size_t rewind_delay, bool is_input = false);

    static FileTape Create(const std::filesystem::path &path, const FileTape &tape, size_t size);

    FileTape() = default;

    void Rewrite(int32_t num) noexcept override;

    int32_t Read() noexcept override;

    void Forward() noexcept override;

    void ForwardBy(bool forward) noexcept;

    void Back() noexcept override;

    size_t GetSize() const noexcept;

    void Fill(size_t n) noexcept;

    void Rewind() noexcept;

    bool EndOfTape() noexcept;

    std::filesystem::path GetPath() const noexcept;

private:
    std::fstream file_tape_;
    size_t tape_size_;
    size_t read_delay_;
    size_t write_delay_;
    size_t move_delay_;
    size_t rewind_delay_;
    std::filesystem::path path_;
};

#endif //TATLIN_FILETAPE_H
