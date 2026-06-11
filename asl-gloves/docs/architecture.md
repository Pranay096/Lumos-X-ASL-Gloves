# ASL Glove Architecture

## 1. Hardware Diagram
```mermaid
graph LR
    subgraph Glove Sensors
        F1[Thumb Flex]
        F2[Index Flex]
        F3[Middle Flex]
        F4[Ring Flex]
        F5[Pinky Flex]
        IMU[MPU6050 IMU]
    end
    subgraph Processing Board
        ESP32[ESP32 SoC]
        VD[Voltage Divider Circuit]
    end
    
    F1 & F2 & F3 & F4 & F5 --> VD
    VD -->|Analog Volts| ESP32
    IMU -->|I2C Data| ESP32
    ESP32 -->|Bluetooth/BLE| MobileApp[MIT App Inventor Mobile App]
```

## 2. Communication Protocol
The glove transmits data packets at 20Hz (every 50ms).
Each packet is sent as a plain-text comma-separated value (CSV) string ended by a newline character (`\n`):
`Thumb,Index,Middle,Ring,Pinky,AccelX,AccelY,AccelZ`

* **Flex sensor values**: Range `0` to `4095` (12-bit ADC).
* **Accelerometer values**: Range `-32768` to `32767`.
