# Waveshare 1.51inch Transparent OLED (SSD1309)

## 1. Specifications
* **Diagonal Size**: 1.51 inches
* **Resolution**: 128 x 64 pixels
* **Color**: Light Blue
* **Driver Chip**: SSD1309
* **Interface**: SPI (default) / I2C
* **Transparency**: 55%
* **Operating Voltage**: 3.3V / 5.0V

## 2. SPI Pin Configuration (ESP32)

| OLED Pin | Description | ESP32 Connection |
|---|---|---|
| **VCC** | Power Supply (3.3V/5V) | 3V3 / 5V |
| **GND** | Ground | GND |
| **DIN (MOSI)** | Data Input | GPIO 23 |
| **CLK (SCK)** | Clock | GPIO 18 |
| **CS** | Chip Select | GPIO 5 |
| **DC** | Data / Command | GPIO 16 |
| **RST** | Reset | GPIO 17 |
