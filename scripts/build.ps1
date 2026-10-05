$ErrorActionPreference = "Stop"
Push-Location (Join-Path $PSScriptRoot "..")
try {
    New-Item -ItemType Directory -Force build | Out-Null
    g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/Console.cpp src/TaskManager.cpp src/FileStorage.cpp -o build/taskflow.exe
    if ($LASTEXITCODE -ne 0) { throw "Application build failed" }
    g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude tests/test_main.cpp src/TaskManager.cpp src/FileStorage.cpp -o build/taskflow_tests.exe
    if ($LASTEXITCODE -ne 0) { throw "Test build failed" }
    & .\build\taskflow_tests.exe
    if ($LASTEXITCODE -ne 0) { throw "Tests failed" }
} finally { Pop-Location }
