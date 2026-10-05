# Hiểu cách dự án hoạt động

Luồng chính: main() gọi run(); Console đọc đối số; FileStorage đọc dữ liệu;
TaskManager kiểm tra và xử lý công việc; FileStorage lưu nếu có thay đổi.

- Task: một struct biểu diễn id, title, priority, done.
- TaskManager: sở hữu vector<Task>, bảo vệ các quy tắc dữ liệu.
- FileStorage: chuyển vector<Task> thành text và đọc text thành vector<Task>.
- Console: hiển thị, xử lý lệnh và chuyển exception thành thông báo lỗi.

Khai báo trong .hpp cho biết một class có những hàm nào; .cpp chứa cách cài đặt.
Dùng const ở các hàm chỉ đọc để không thay đổi đối tượng.
`all()` trả const reference để tránh copy vector và ngăn nơi gọi sửa trực tiếp.
Thêm/xóa có thể làm reference tới phần tử cũ không còn hợp lệ.
Tìm kiếm dùng vòng lặp và string::find; độ phức tạp phụ thuộc số task và độ dài tiêu đề.

File dữ liệu bắt đầu bằng TASKFLOW_V1. Mỗi dòng tiếp theo có:
id priority done "title". std::quoted xử lý dấu ngoặc kép và backslash.
Đọc file lỗi sẽ dừng, không tự bỏ qua rồi ghi đè mất bản ghi.
Ứng dụng trả mã 0 khi thành công, 1 khi gặp lỗi.

Thứ tự học: Task.hpp → TaskManager.hpp/.cpp → test_main.cpp →
FileStorage.hpp/.cpp → Console.cpp → CMakeLists.txt → ci.yml.
