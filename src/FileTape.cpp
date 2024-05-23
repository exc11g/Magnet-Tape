#include "../lib/FileTape.h"
#include "../lib/Util.h"

void FileTape::Rewrite(int32_t num) noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(write_delay_));
    auto curr_pos = file_tape_.tellg();
    file_tape_.write(reinterpret_cast<char *>(&num), util::kIntSize);
    file_tape_.seekg(curr_pos);
}

int32_t FileTape::Read() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(read_delay_));
    int32_t res;
    auto curr_pos = file_tape_.tellg();
    file_tape_.read(reinterpret_cast<char *>(&res), util::kIntSize);
    file_tape_.seekg(curr_pos);
    return res;
}

void FileTape::Forward() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(move_delay_));
    file_tape_.seekg(util::kIntSize, std::ios::cur);
}

void FileTape::Back() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(move_delay_));
    file_tape_.seekg(-static_cast<int64_t>(util::kIntSize), std::ios::cur);
}

size_t FileTape::GetSize() const noexcept {
    return tape_size_;
}

FileTape::FileTape(const std::filesystem::path &path, size_t read_delay,
                   size_t write_delay, size_t move_delay, size_t rewind_delay, bool is_input) : tape_size_(0),
                                                                                                read_delay_(read_delay),
                                                                                                write_delay_(
                                                                                                        write_delay),
                                                                                                move_delay_(move_delay),
                                                                                                rewind_delay_(
                                                                                                        rewind_delay),
                                                                                                path_(path) {
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
    tape_size_ = file_tape_.tellg() / util::kIntSize;
    file_tape_.seekg(std::ios::beg);
}

void FileTape::Fill(size_t n) noexcept {
    int32_t zero = 0;
    for (size_t i = 0; i < n; ++i) {
        file_tape_.write(reinterpret_cast<char *>(&zero), util::kIntSize);
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

void FileTape::Rewind() noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(rewind_delay_));
    file_tape_.seekg(std::ios::beg);
}

FileTape FileTape::Create(const std::filesystem::path &path, const FileTape &tape, size_t size) {
    FileTape res(path, tape.read_delay_, tape.write_delay_, tape.move_delay_, tape.rewind_delay_, false);
    res.Fill(size);
    return res;
}

bool FileTape::EndOfTape() noexcept {
    return file_tape_.tellg() == tape_size_ * util::kIntSize;
}

std::filesystem::path FileTape::GetPath() const noexcept {
    return path_;
}

