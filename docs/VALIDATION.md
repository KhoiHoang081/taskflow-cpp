# Kết quả kiểm tra bản đóng gói

- Build bằng g++ C++17 trên Linux với -Wall -Wextra -Wpedantic: thành công.
- 27 kiểm tra tự động: đạt.
- Chạy CLI qua nhiều tiến trình để xác nhận lưu/đọc, thêm, hoàn thành, mở lại,
  tìm kiếm, xóa, thống kê: đạt.
- Tham số lỗi trả mã 1 và không thay đổi dữ liệu đã lưu: đạt.
- Chưa chạy trực tiếp trên Windows hoặc bằng CMake trong môi trường tạo bản này.
  Workflow GitHub Actions đã cấu hình kiểm tra CMake trên Windows/Linux khi push.
