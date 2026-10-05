# Đưa TaskFlow lên GitHub

## 1. Chuẩn bị

Giải nén ZIP, mở terminal bên trong thư mục `taskflow-cpp` chứa `README.md`.
Cài Git; chạy `git --version` để kiểm tra.
Trên GitHub tạo repository mới tên `taskflow-cpp`. Để repository trống:
không chọn tạo README, .gitignore hoặc license vì source đã có các file cấu hình cần thiết.

## 2. Đặt danh tính và commit

Thay các chuỗi ví dụ bằng thông tin của chính bạn. Email phải liên kết với tài khoản GitHub;
có thể dùng email noreply chính xác được GitHub hiển thị trong Settings → Emails.
Các lệnh config bên dưới chỉ áp dụng cho repository này.

```bash
git init
git branch -M main
git config user.name "TEN_CUA_BAN"
git config user.email "EMAIL_GITHUB_CUA_BAN"
git add .
git status
git commit -m "feat: initialize C++ task manager"
git remote add origin https://github.com/USERNAME/taskflow-cpp.git
git push -u origin main
```

Thay USERNAME bằng tên tài khoản. Đăng nhập qua Git Credential Manager nếu được hỏi;
không đặt token hoặc mật khẩu vào source code. Nếu Git báo remote origin đã tồn tại,
kiểm tra `git remote -v` trước khi sửa URL. Không cần force push cho dự án mới này.

## 3. Khi sửa dự án lần sau

```bash
git diff
git add .
git commit -m "feat: add task sorting by priority"
git push
```

Chỉ dùng thông điệp trên khi bạn đã thực sự làm tính năng đó. Xem ROADMAP.md để chọn
một cải tiến nhỏ, kiểm thử rồi commit phần công việc hoàn chỉnh.

## 4. Contribution được tính thế nào?

Nhiều folder/file không tự tạo ra nhiều contribution: 30 file trong một commit vẫn là
một commit. Với repository riêng bạn mới tạo, hãy đặt `main` là default branch và dùng
email commit liên kết với GitHub. Commit trong fork không được tính như commit trong
repository độc lập; commit ở nhánh tính năng cần được đưa vào default branch để đủ điều kiện.
Biểu đồ có thể mất tới 24 giờ để cập nhật. Private contribution cần bật hiển thị phù hợp
nếu bạn muốn số lượng đó xuất hiện trên hồ sơ.

Nguồn GitHub (tham khảo ngày 05/10/2026):
- https://docs.github.com/en/account-and-profile/reference/profile-contributions-reference
- https://docs.github.com/en/account-and-profile/how-tos/contribution-settings/troubleshooting-missing-contributions

## 5. CI

Mở tab Actions sau khi push để xem workflow build/test. Cấu hình chạy trên cả Ubuntu
và Windows. Nếu workflow chưa chạy, kiểm tra Actions có được bật trong repository không.
