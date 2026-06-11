# ASL Gloves Setup and Assembly Guide

## 1. Required Components
See the complete parts inventory in [hardware/bom.csv](../hardware/bom.csv).

## 2. Mechanical Construction
1. Lay the base glove flat.
2. Affix custom tendon sleeves along the back of each finger.
3. Thread the flex sensors through the sleeves. Ensure they align perfectly over the knuckles (MCP and PIP joints).
4. Securely glue or stitch the base end of the sensor to the wrist interface, while leaving the finger ends free to slide during knuckle contraction.

## 3. Electronics & Wiring
1. Mount the ESP32 and MPU6050 to the proto-board.
2. Construct 5 voltage dividers (one for each flex sensor) using the 10k Ohm resistors as pull-downs connected to ground.
3. Connect output nodes of the voltage dividers to the ESP32 analog pins: GPIO 36, 39, 34, 35, and 32.
4. Interface the MPU6050 to ESP32 I2C pins: SDA (GPIO 21) and SCL (GPIO 22).

## 4. Software Setup
1. Install [esp32_gloves.ino](../firmware/esp32_gloves/esp32_gloves.ino) using Arduino IDE.
2. Compile and flash the code to the ESP32.
3. Open serial monitor (115200 baud) to check comma-separated sensor outputs.
