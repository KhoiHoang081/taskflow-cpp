#pragma once
#include <string>
namespace taskflow {
struct Task {
    int id;
    std::string title;
    int priority; // 1 = low, 2 = medium, 3 = high
    bool done;
};
struct Stats {
    std::size_t total;
    std::size_t completed;
    std::size_t pending;
};
} // namespace taskflow
