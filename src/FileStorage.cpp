#include "taskflow/FileStorage.hpp"
#include "taskflow/TaskManager.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
namespace taskflow {
FileStorage::FileStorage(std::filesystem::path path) : path_(std::move(path)) {}
std::vector<Task> FileStorage::load() const {
    if (!std::filesystem::exists(path_)) return {};
    std::ifstream input(path_);
    if (!input) throw std::runtime_error("Cannot open data file.");
    std::string line;
    if (!std::getline(input, line) || line != "TASKFLOW_V1")
        throw std::runtime_error("Invalid data header.");
    std::vector<Task> tasks;
    int row = 1;
    while (std::getline(input, line)) {
        ++row;
        std::istringstream record(line);
        Task task{};
        int done = 0;
        if (!(record >> task.id >> task.priority >> done))
            throw std::runtime_error("Invalid record at line " + std::to_string(row));
        record >> std::ws;
        if (record.peek() != '"' || !(record >> std::quoted(task.title)) ||
            (done != 0 && done != 1))
            throw std::runtime_error("Invalid title/status at line " + std::to_string(row));
        record >> std::ws;
        if (!record.eof()) throw std::runtime_error("Unexpected trailing data.");
        task.done = done == 1;
        tasks.push_back(task);
    }
    if (input.bad()) throw std::runtime_error("Cannot read data file.");
    TaskManager validated(tasks); // Validate IDs and domain rules before returning.
    return tasks;
}
void FileStorage::save(const std::vector<Task>& tasks) const {
    TaskManager validated(tasks);
    if (path_.has_parent_path()) std::filesystem::create_directories(path_.parent_path());
    // One local process at a time. Full crash-safe replacement is a roadmap item.
    std::ofstream output(path_, std::ios::trunc);
    if (!output) throw std::runtime_error("Cannot write data file.");
    output << "TASKFLOW_V1\n";
    for (const auto& task : tasks)
        output << task.id << ' ' << task.priority << ' ' << (task.done ? 1 : 0)
               << ' ' << std::quoted(task.title) << '\n';
    output.close();
    if (!output) throw std::runtime_error("Failed to finish writing data file.");
}
}
