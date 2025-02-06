import serial
import time
import struct

# Configure the serial port
ser = serial.Serial(
    port='COM29',  # Change based on your setup
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

#CRC calculation function
def crc16_ccitt(data: bytes, poly=0x1021, init=0xFFFF):
    crc = init
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ poly
            else:
                crc <<= 1
            crc &= 0xFFFF  # Keep it 16-bit
    return crc


# Send OTA start sequence
def send_ota_start():
    ser.write(b"OTA_START\r")

# Send OTA end sequence
def send_ota_end():
    ser.write(b"####")

# Read UART incoming data
def read_incoming_data():
    while ser.in_waiting > 0:
        received_data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
        print(f"Received: {received_data.strip()}")
        return received_data.strip()
    return ""

# Send the firmware file with CRC every 100 chunks
def send_binary_file(file_path):
    print("Sending firmware...")
    try:
        with open(file_path, 'rb') as f:
            buffer = b""  # Buffer for CRC calculation
            chunk_count = 0
            count = 0
            while True:
                chunk = f.read(4)  # Read 4-byte chunk
                if not chunk:
                    break

                ser.write(chunk)  # Send chunk
                buffer += chunk  # Store in buffer for CRC
                chunk_count += 1
                
                # time.sleep(0.01)  # Allow STM32 to process

                # Send CRC every 100 chunks (400 bytes)
                if chunk_count == 100:
                    crc_value = crc16_ccitt(buffer)
                    crc_bytes = struct.pack(">H", crc_value)  # Convert to 2-byte big-endian
                    ser.write(crc_bytes)  # Send CRC
                    print(f"Sent CRC: {hex(crc_value)}")

                    # Wait for ACK before continuing
                    while True:
                        response = read_incoming_data()
                        if "CRC_OK" in response:
                            count += 1
                            print(str(count)+ " CRC OK, sending next 100 word...")
                            break

                    # Reset buffer
                    buffer = b""
                    chunk_count = 0

            # Final CRC for remaining data (if any)
            if buffer:
                send_ota_end()
                crc_value = crc16_ccitt(buffer)
                crc_bytes = struct.pack(">H", crc_value)
                ser.write(crc_bytes)
                print(f"Sent final CRC: {hex(crc_value)}")

                # Wait for final CRC confirmation
                while True:
                    response = read_incoming_data()
                    if "CRC_OK" in response:
                        print("Final CRC OK, firmware transmission complete.")
                        break
                    elif "CRC_ERROR" in response:
                        print("Final CRC ERROR, resending last buffer...")
                        for i in range(0, len(buffer), 4):
                            ser.write(buffer[i:i+4])
                        ser.write(crc_bytes)
                    # time.sleep(0.1)

            print("Firmware sent successfully.")

    except Exception as e:
        print(f"Error sending firmware: {e}")

# Wait for OTA start command
def wait_for_ota_start():
    print("Waiting for 'OTA_START' command from STM32...")
    while True:
        if ser.in_waiting > 0:
            received_data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
            print(f"Received: {received_data.strip()}")
            if "OTA_START" in received_data:
                print("'OTA_START' command received. Starting OTA process.")
                return True
        time.sleep(0.1)

# Main execution
if __name__ == "__main__":
    try:
        send_ota_start()
        if wait_for_ota_start():
            #send_binary_file('Firmware/App.ino.bin')
            send_binary_file('Firmware/Application.bin')
        while True:
            read_incoming_data()
    except KeyboardInterrupt:
        print("Process interrupted by user.")
    finally:
        ser.close()
