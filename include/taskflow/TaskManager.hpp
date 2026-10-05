#pragma once
#include "Task.hpp"
#include <vector>
namespace taskflow {
// Owns tasks and business rules; does not print or access files.
class TaskManager {
public:
    explicit TaskManager(std::vector<Task> tasks = {});
    int add(const std::string& title, int priority);
    bool setDone(int id, bool done);
    bool remove(int id);
    std::vector<Task> search(const std::string& keyword) const;
    const std::vector<Task>& all() const;
    Stats stats() const;
private:
    std::vector<Task> tasks_;
};
}
