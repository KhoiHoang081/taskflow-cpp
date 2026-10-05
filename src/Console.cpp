#include "taskflow/Console.hpp"
#include "taskflow/FileStorage.hpp"
#include "taskflow/TaskManager.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
namespace taskflow {
namespace {
void help() {
    std::cout << "TaskFlow - C++17 task manager\n"
              << "Usage: taskflow [--file PATH] COMMAND [ARGS]\n"
              << "  add \"TITLE\" [1|2|3]   Add task (default priority: 2)\n"
              << "  list [all|pending|done]\n"
              << "  done ID | reopen ID | remove ID\n"
              << "  search \"KEYWORD\"       Case-sensitive search\n"
              << "  stats | help\n";
}
int integer(const std::string& value) {
    if (value.empty() || value.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("Expected a positive integer.");
    std::size_t used = 0;
    const int number = std::stoi(value, &used);
    if (used != value.size() || number <= 0)
        throw std::invalid_argument("Expected a positive integer.");
    return number;
}
void print(const std::vector<Task>& tasks, const std::string& filter = "all") {
    std::size_t shown = 0;
    for (const auto& task : tasks) {
        if ((filter == "done" && !task.done) || (filter == "pending" && task.done)) continue;
        std::cout << '#' << task.id << " [" << (task.done ? 'x' : ' ')
                  << "] P" << task.priority << "  " << task.title << '\n';
        ++shown;
    }
    if (shown == 0) std::cout << "No tasks found.\n";
}
}
int run(int argc, char* argv[]) {
    try {
        int index = 1;
        std::string path = "data/tasks.db";
        if (index < argc && std::string(argv[index]) == "--file") {
            if (index + 1 >= argc) throw std::invalid_argument("Missing file path.");
            path = argv[index + 1];
            index += 2;
        }
        if (index == argc) { help(); return 0; }
        const std::string command = argv[index++];
        const int remaining = argc - index;
        auto require = [&](int count) {
            if (remaining != count) throw std::invalid_argument("Wrong arguments. Run: taskflow help");
        };
        if (command == "help" || command == "--help") { require(0); help(); return 0; }
        FileStorage storage(path);
        TaskManager manager(storage.load());
        if (command == "add") {
            if (remaining < 1 || remaining > 2) throw std::invalid_argument("Use: add \"TITLE\" [1|2|3]");
            const int id = manager.add(argv[index], remaining == 2 ? integer(argv[index + 1]) : 2);
            storage.save(manager.all());
            std::cout << "Added task #" << id << '\n';
        } else if (command == "list") {
            if (remaining > 1) throw std::invalid_argument("Use: list [all|pending|done]");
            const std::string filter = remaining ? argv[index] : "all";
            if (filter != "all" && filter != "pending" && filter != "done")
                throw std::invalid_argument("Unknown list filter.");
            print(manager.all(), filter);
        } else if (command == "done" || command == "reopen" || command == "remove") {
            require(1);
            const int id = integer(argv[index]);
            const bool changed = command == "remove" ? manager.remove(id) : manager.setDone(id, command == "done");
            if (!changed) throw std::invalid_argument("Task ID not found.");
            storage.save(manager.all());
            std::cout << "Saved.\n";
        } else if (command == "search") {
            require(1); print(manager.search(argv[index]));
        } else if (command == "stats") {
            require(0);
            const auto s = manager.stats();
            std::cout << "Total: " << s.total << " | Done: " << s.completed << " | Pending: " << s.pending << '\n';
        } else throw std::invalid_argument("Unknown command. Run: taskflow help");
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
}
