#include "taskflow/TaskManager.hpp"
#include "taskflow/FileStorage.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <limits>
using namespace taskflow;
int checks = 0;
void check(bool condition, const char* message) {
    ++checks;
    if (!condition) throw std::runtime_error(message);
}
template<class F> void rejects(F action) {
    bool caught = false;
    try { action(); } catch (const std::exception&) { caught = true; }
    check(caught, "Expected rejection");
}
int main() {
    const auto path = std::filesystem::temp_directory_path() /
        ("taskflow-test-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    try {
        TaskManager manager;
        check(manager.stats().total == 0, "Empty stats");
        check(manager.add("Study C++", 3) == 1, "First ID");
        check(manager.add("Math", 2) == 2, "Second ID");
        check(manager.setDone(1, true), "Complete task");
        check(manager.stats().completed == 1 && manager.stats().pending == 1, "Stats");
        check(manager.search("C++").size() == 1, "Search match");
        check(manager.search("missing").empty(), "Search no match");
        check(manager.setDone(1, false), "Reopen");
        check(!manager.setDone(99, true) && !manager.remove(99), "Missing IDs");
        rejects([&] { manager.add("   ", 2); });
        rejects([&] { manager.add("Bad\nTitle", 2); });
        rejects([&] { manager.add("Bad priority", 4); });
        rejects([] { TaskManager invalid({{1,"A",1,false},{1,"B",2,false}}); });
        rejects([] { TaskManager invalid({{0,"A",1,false}}); });
        TaskManager full({{std::numeric_limits<int>::max(),"A",1,false}});
        rejects([&] { full.add("B", 2); });
        check(manager.remove(1) && manager.all().size() == 1, "Remove");
        check(manager.add("Read \"C++\" at C:\\books", 1) == 3, "ID after deletion");
        FileStorage storage(path / "nested" / "tasks.db");
        check(storage.load().empty(), "Missing file");
        storage.save(manager.all());
        const auto loaded = storage.load();
        check(loaded.size() == 2 && loaded[1].title == manager.all()[1].title,
              "Quoted title roundtrip");
        check(loaded[0].id == 2 && loaded[0].priority == 2 && !loaded[0].done,
              "Fields roundtrip");
        manager.setDone(2, true);
        storage.save(manager.all());
        check(storage.load()[0].done, "Overwrite roundtrip");
        const auto corrupt = path / "corrupt.db";
        const auto malformed = {"BAD\n", "TASKFLOW_V1\n1 2 7 \"x\"\n",
            "TASKFLOW_V1\n1 2 0 \"x\" trailing\n",
            "TASKFLOW_V1\n1 2 0 \"x\"\n1 1 1 \"y\"\n",
            "TASKFLOW_V1\n1 2 0 \"unfinished\n"};
        for (const auto* data : malformed) {
            { std::ofstream out(corrupt); out << data; }
            rejects([&] { FileStorage(corrupt).load(); });
        }
        storage.save({});
        check(storage.load().empty(), "Empty save");
        std::filesystem::remove_all(path);
        std::cout << "PASS: " << checks << " checks\n";
        return 0;
    } catch (const std::exception& e) {
        std::error_code ignored;
        std::filesystem::remove_all(path, ignored);
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
