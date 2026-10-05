#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p build
compiler="${CXX:-g++}"
"$compiler" -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/Console.cpp src/TaskManager.cpp src/FileStorage.cpp -o build/taskflow
"$compiler" -std=c++17 -Wall -Wextra -Wpedantic -Iinclude tests/test_main.cpp src/TaskManager.cpp src/FileStorage.cpp -o build/taskflow_tests
./build/taskflow_tests
