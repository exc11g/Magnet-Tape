#define CATCH_CONFIG_MAIN

#include "../lib/SortTape.h"

#include <catch2/catch.hpp>

using Catch::Generators::chunk;
using Catch::Generators::random;

using namespace std::chrono_literals;

namespace {

    const std::filesystem::path kTestDir = "./__test/";
    constexpr auto kIntMax = std::numeric_limits<int32_t>::max();

    void Check(std::vector<int32_t> &v, size_t read = 1, size_t write = 1, size_t move = 1, size_t rewind = 1) {
        std::filesystem::create_directory(kTestDir);
        std::fstream stream(kTestDir / "input.txt", std::ios::out);
        for (auto x: v) {
            stream << x << " ";
        }
        stream.close();
        ToBinary(kTestDir / "input.txt", kTestDir / "input_binary.tape");
        {
            SortTape sortTape(kTestDir / "input_binary.tape", kTestDir / "output_binary.tape",
                              read,
                              write,
                              move,
                              rewind);
            sortTape.Sort();
        }
        FromBinary(kTestDir / "output_binary.tape", kTestDir / "output.txt");
        std::vector<int32_t> res;
        int32_t curr;
        std::fstream out(kTestDir / "output.txt", std::ios::in);
        while (out >> curr) {
            res.push_back(curr);
        }
        std::sort(v.begin(), v.end());
        std::filesystem::remove_all(kTestDir);
        REQUIRE(v == res);
    }

    TEST_CASE("EASY") {
        std::vector<int32_t> v = {5, 6, -1, 2, 0, -1};
        Check(v);
        v = {};
        Check(v);
        v = {1, 2, 3, 4};
        Check(v);
        v = {-1, -1, -5234};
        Check(v);
    }

    TEST_CASE("RANDOM") {
        auto size = GENERATE(take(5, random(0, 10)));
        auto v = chunk(size, random(-kIntMax, kIntMax)).get();
        Check(v);
    }

    TEST_CASE("READ DELAY") {
        auto size = GENERATE(take(10, random(0, 20)));
        auto v = chunk(size, random(-kIntMax, kIntMax)).get();
        auto start = std::chrono::steady_clock::now();
        Check(v, 5);
        auto end = std::chrono::steady_clock::now();
        REQUIRE((end - start) / 1ms > 5 * size * size);
    }

    TEST_CASE("ALL DELAYS") {
        auto size = GENERATE(take(10, random(0, 10)));
        auto v = chunk(size, random(-kIntMax, kIntMax)).get();
        auto start = std::chrono::steady_clock::now();
        Check(v, 5, 10, 15, 20);
        auto end = std::chrono::steady_clock::now();
        REQUIRE((end - start) / 1ms > (5 + 15) * size * size + 15 * size);
    }

    TEST_CASE("NO FILES") {
        CHECK_THROWS(ToBinary("nofile.xxx", "skdaks.txt"));
    }
}