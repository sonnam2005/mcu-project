# MCU Project

Dự án MCU dùng FreeRTOS, làm việc nhóm theo Git branch.

## Nhánh
- main: bản ổn định, cấm commit trực tiếp
- develop: nhánh tích hợp chính
- feature/<ten-task>: nhánh làm việc cá nhân
- integration/<phien-ban>: nhánh ghép nối trước khi release

## Quy tắc commit
- feat(task-xxx): thêm chức năng
- fix(rtos): sửa lỗi
- refactor(...): tối ưu code

## Phân công công việc
- Nam: OLED + INA219 dùng chung một bus I2C.
- Hùng: Đọc 2 cảm biến ánh sáng LDR - ADC
- Trung: Điều khiển một động cơ servo - 5V - PWM
