# Lumos AI Glasses — Setup Guide

## Requirements

### Hardware
- Raspberry Pi zero 2 w (recommended) or Raspberry Pi zero w
- Raspberry Pi Camera Module v1 or v2
- ESP32 Dev Module
- Waveshare 1.51" Transparent OLED display
- Speaker or 3.5mm audio output device
- Power bank (minimum 10,000 mAh recommended for portable use)

### Software
- Raspberry Pi OS (64-bit, Bookworm or later)
- Python 3.10+
- Arduino IDE (for ESP32 firmware)

### API Keys Required
- OpenAI API key (for contextual AI assistance)
- Google Gemini API key (for scene understanding fallback)

---

## Step 1 — Clone the Repository

```bash
git clone https://github.com/yourusername/Lumos-X-ASL-Gloves.git
cd Lumos-X-ASL-Gloves/lumos-ai-glasses
```

## Step 2 — Install Python Dependencies

```bash
pip install -r requirements.txt
```

This installs: OpenCV, Ultralytics (YOLOv8), ONNX Runtime, SpeechRecognition, openai, google-generativeai, and supporting libraries.

## Step 3 — Configure API Keys

Create a `.env` file in the `lumos-ai-glasses/` directory:

```
OPENAI_API_KEY=your_openai_key_here
GEMINI_API_KEY=your_gemini_key_here
```

Do not commit this file. It is already listed in `.gitignore`.

## Step 4 — Flash the ESP32 Display Firmware

1. Open Arduino IDE
2. Install the following board support: `ESP32 by Espressif Systems`
3. Install libraries: `Adafruit SSD1306`, `Adafruit GFX`
4. Open `firmware/esp32_display/esp32_display.ino`
5. Select your ESP32 board and COM port
6. Upload the firmware

## Step 5 — Connect Hardware

- Connect the Waveshare OLED to the ESP32 via I2C (SDA → GPIO 21, SCL → GPIO 22)
- Connect the ESP32 to the Raspberry Pi via USB serial
- Mount the camera module to the Raspberry Pi CSI port

## Step 6 — Run the System

Each module can be run independently or together:

```bash
# Object detection and scene narration (for blind users)
python src/object_detection.py

# Sign language recognition
python src/sign_language_detection.py

# Speech-to-text transcription (for deaf users)
python src/speech_to_text.py
```

To run all modules simultaneously, use a process manager like `tmux` or a shell script.

---

## Troubleshooting

See [`troubleshooting.md`](troubleshooting.md) for common issues and fixes.