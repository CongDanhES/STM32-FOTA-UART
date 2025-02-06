i = 0  # Bắt đầu từ 50
with open('Firmware/App.ino.bin', 'rb') as f:
    while True:
        byte = f.read(4)  # Đọc 4 byte mỗi lần
        if not byte:
            break  # Dừng nếu không còn dữ liệu

        # Sửa lỗi sử dụng hàm `str` để chuyển đổi số nguyên sang chuỗi
        # print(str(i) + " " + str(byte))  # Hiển thị số thứ tự và dữ liệu dạng Hex
        print(str(i) + " " + byte.hex()) 
        i += 1

        # if i == 50:  # Chỉ đọc 5 lần để thử nghiệm (50 -> 54)
        #     break
