#pragma once
#include "Task.hpp"
#include <filesystem>
#include <vector>
namespace taskflow {
class FileStorage {
public:
    explicit FileStorage(std::filesystem::path path);
    std::vector<Task> load() const;
    void save(const std::vector<Task>& tasks) const;
private:
    std::filesystem::path path_;
};
}
