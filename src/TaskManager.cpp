#include "taskflow/TaskManager.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <unordered_set>
namespace taskflow {
namespace {
void validate(const Task& task) {
    if (task.id <= 0 || task.priority < 1 || task.priority > 3)
        throw std::invalid_argument("Invalid ID or priority (use 1..3).");
    if (task.title.find_first_not_of(" \t\r\n") == std::string::npos ||
        task.title.find_first_of("\r\n") != std::string::npos)
        throw std::invalid_argument("Title must be non-empty and on one line.");
}
}
TaskManager::TaskManager(std::vector<Task> tasks) : tasks_(std::move(tasks)) {
    std::unordered_set<int> ids;
    for (const auto& task : tasks_) {
        validate(task);
        if (!ids.insert(task.id).second)
            throw std::invalid_argument("Duplicate task ID.");
    }
}
int TaskManager::add(const std::string& title, int priority) {
    int maximum = 0;
    for (const auto& task : tasks_) maximum = std::max(maximum, task.id);
    if (maximum == std::numeric_limits<int>::max())
        throw std::overflow_error("No more task IDs available.");
    Task task{maximum + 1, title, priority, false};
    validate(task);
    tasks_.push_back(task);
    return task.id;
}
bool TaskManager::setDone(int id, bool done) {
    for (auto& task : tasks_) {
        if (task.id == id) { task.done = done; return true; }
    }
    return false;
}
bool TaskManager::remove(int id) {
    const auto it = std::find_if(tasks_.begin(), tasks_.end(),
                               [id](const Task& task) { return task.id == id; });
    if (it == tasks_.end()) return false;
    tasks_.erase(it);
    return true;
}
std::vector<Task> TaskManager::search(const std::string& keyword) const {
    std::vector<Task> found;
    for (const auto& task : tasks_)
        if (task.title.find(keyword) != std::string::npos) found.push_back(task);
    return found;
}
const std::vector<Task>& TaskManager::all() const { return tasks_; }
Stats TaskManager::stats() const {
    const auto done = static_cast<std::size_t>(std::count_if(
        tasks_.begin(), tasks_.end(), [](const Task& task) { return task.done; }));
    return {tasks_.size(), done, tasks_.size() - done};
}
}
