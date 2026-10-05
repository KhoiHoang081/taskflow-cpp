# TaskFlow C++

Ứng dụng quản lý công việc bằng terminal, viết bằng **C++17**, không dùng thư viện bên ngoài.
Dự án học tập mức cơ bản–trung bình: class, struct, vector, string, thuật toán STL,
file stream, exception, chia module, kiểm thử và GitHub Actions.

## Tính năng

- Thêm công việc với độ ưu tiên 1 (thấp), 2 (vừa), 3 (cao).
- Xem tất cả, việc đã hoàn thành hoặc việc chưa hoàn thành.
- Đánh dấu hoàn thành, mở lại và xóa theo ID.
- Tìm kiếm tiêu đề (phân biệt chữ hoa/thường), thống kê.
- Lưu dữ liệu ra file; đọc lại trong lần chạy tiếp theo.
- Báo lỗi khi tham số hoặc dữ liệu không hợp lệ.

## Cấu trúc

| Thư mục | Nội dung |
|---|---|
| `src/` | File `.cpp`: điểm bắt đầu, CLI, nghiệp vụ, lưu trữ |
| `include/taskflow/` | Header khai báo các kiểu dữ liệu và class |
| `tests/` | Kiểm thử nghiệp vụ và file dữ liệu |
| `data/` | Dữ liệu mẫu, file dữ liệu sinh khi chạy |
| `docs/` | Kiến trúc, hướng dẫn GitHub, lộ trình học |
| `examples/` | Các lệnh dùng thử |
| `scripts/` | Script build/chạy test trên Windows và Linux |
| `.github/workflows/` | CI build và test trên Windows/Linux |

Mỗi thư mục chính đều có README.txt. Bắt đầu với `src/main.cpp`, sau đó đọc
`include/taskflow/Task.hpp`, `TaskManager.hpp` và `src/TaskManager.cpp`.

## Chạy nhanh trên Windows — g++

Cần Git và compiler g++ hỗ trợ C++17 (khuyến nghị GCC 9 trở lên) có trong PATH.
Mở PowerShell tại thư mục chứa README này:

```powershell
New-Item -ItemType Directory -Force build
 g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/Console.cpp src/TaskManager.cpp src/FileStorage.cpp -o build/taskflow.exe
.\build\taskflow.exe add "Hoc C++" 3
.\build\taskflow.exe list
.\build\taskflow.exe done 1
.\build\taskflow.exe stats
```

Hoặc chạy `powershell -ExecutionPolicy Bypass -File scripts/build.ps1` để build
cả chương trình và test. Script chỉ áp dụng tùy chọn execution policy cho tiến trình đó.
Dự án có nhiều file `.cpp`: cần biên dịch đầy đủ như lệnh trên, không chỉ chạy `main.cpp`.

## Linux/macOS — g++ hoặc clang++

```bash
bash scripts/build.sh
./build/taskflow add "Learn C++" 3
./build/taskflow list
```

Dùng `CXX=clang++ bash scripts/build.sh` nếu muốn chọn Clang.

## Build bằng CMake (tùy chọn)

Cần CMake 3.16+ và compiler C++17:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Với Visual Studio generator, executable nằm ở `build/Release/taskflow.exe`.
Với Makefiles/Ninja, executable nằm ở `build/taskflow` (Windows thêm `.exe`).

## Các lệnh

Ví dụ bên dưới dùng Linux/macOS; trên Windows đổi `./build/taskflow` thành
`.\build\taskflow.exe` (hoặc đường dẫn CMake tương ứng).

```bash
./build/taskflow help
./build/taskflow add "Lam bai tap ma tran" 2
./build/taskflow list pending
./build/taskflow done 1
./build/taskflow reopen 1
./build/taskflow search "ma tran"
./build/taskflow stats
./build/taskflow remove 1
./build/taskflow --file data/sample.txt list
```

Đặt tiêu đề nhiều từ trong dấu ngoặc kép. `--file PATH` phải đứng trước command.
Mặc định dữ liệu ở `data/tasks.db`, tính theo thư mục terminal hiện tại.
Luôn chạy ở gốc dự án để dùng cùng một file dữ liệu. File `.db` thực chất là text,
không phải SQLite; `.gitignore` loại file dữ liệu cá nhân và file build khỏi Git.
Dùng `--file data/sample.txt` với lệnh thay đổi sẽ sửa trực tiếp dữ liệu mẫu.

## Giới hạn có chủ ý

- Chỉ chạy một tiến trình ghi dữ liệu mỗi lần; chưa có khóa file.
- Ghi trực tiếp vào file, chưa bảo đảm phục hồi nếu mất điện giữa lúc ghi.
- ID mới bằng ID lớn nhất hiện còn + 1; ID đã xóa ở cuối có thể được dùng lại.
- Tìm kiếm phân biệt chữ hoa/thường; tiêu đề hỗ trợ byte UTF-8, không chuẩn hóa Unicode.
- Chưa có hạn chót, chỉnh sửa tiêu đề hoặc sắp xếp. Xem `docs/ROADMAP.md`.

## Push GitHub

Làm theo `docs/GITHUB_GUIDE.md`: tạo repository trống, đặt tên/email Git,
commit rồi push nhánh `main`. Không có sẵn lịch sử Git hoặc tài khoản được nhúng trong dự án.
GitHub Actions đã có cấu hình; kết quả thực tế xuất hiện sau khi bạn push.
