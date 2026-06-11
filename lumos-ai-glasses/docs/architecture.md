# Lumos AI Glasses — Architecture

## System Overview

Lumos is built around a Raspberry Pi as the central processing unit, with a camera for input, an ESP32-driven transparent OLED for visual output, and audio for spoken output. Cloud AI APIs handle the tasks that are too heavy for on-device inference at current hardware specs.

```
Camera Input
    ↓
Raspberry Pi (Central Processor)
    ├── object_detection.py     → YOLOv8 → Audio narration
    ├── sign_language_detection.py → ONNX model → Text output
    └── speech_to_text.py      → SpeechRecognition → OLED display
         ↓
    ESP32 (via USB Serial)
         ↓
    Waveshare Transparent OLED
```

## Modules

### Object Detection (`src/object_detection.py`)

- Captures frames from the camera using OpenCV
- Runs YOLOv8 nano (`models/yolov8n.pt`) for real-time object detection
- Detected objects and their positions are passed to the OpenAI/Gemini API for natural language scene description
- Audio output is generated using `pyttsx3` (offline TTS) or a cloud TTS service
- Target latency: under 2 seconds from frame capture to audio narration

### Sign Language Detection (`src/sign_language_detection.py`)

- Captures hand landmarks using MediaPipe from the camera feed
- Landmark data is fed into a trained ONNX model (`models/asl_model.onnx`) for gesture classification
- Recognised signs are assembled into words and sentences
- Output is displayed as text and optionally spoken aloud

### Speech to Text (`src/speech_to_text.py`)

- Captures microphone audio using `SpeechRecognition`
- Transcribes speech using Google Speech Recognition (online) or Vosk (offline fallback)
- Sends transcribed text to the ESP32 over USB serial
- ESP32 renders the text on the Waveshare transparent OLED display

### ESP32 Display Firmware (`firmware/esp32_display/esp32_display.ino`)

- Listens on USB serial for incoming text strings
- Renders text on the Waveshare 1.51" transparent OLED using I2C
- Handles text wrapping and display refresh
- Communicates back to Raspberry Pi for status acknowledgement

## Data Flow by User Mode

### Blind User Mode
```
Camera → OpenCV frame → YOLOv8 detection → OpenAI scene description → pyttsx3 audio
```

### Deaf User Mode
```
Microphone → SpeechRecognition → transcribed text → Serial → ESP32 → OLED display
```

### Speech-Impaired User Mode
```
Camera → MediaPipe landmarks → ONNX classifier → recognised sign → text/audio output
```

## AI and Model Details

| Component | Model / Service | Where it runs |
|---|---|---|
| Object detection | YOLOv8n | On-device (Raspberry Pi) |
| Sign classification | Custom ONNX model | On-device (Raspberry Pi) |
| Scene understanding | OpenAI GPT-4o / Gemini | Cloud (API call) |
| Speech transcription | Google Speech / Vosk | Cloud / on-device |
| Text to speech | pyttsx3 / cloud TTS | On-device / cloud |

## Known Architecture Limitations

- Scene understanding and contextual AI responses require an internet connection. Offline mode falls back to object labels only (no descriptive narration).
- YOLOv8 inference on Raspberry Pi 4 runs at approximately 5–10 FPS. Real-time performance requires the nano model; larger models are too slow for this hardware.
- The transparent OLED display area is limited — long transcriptions are truncated or scrolled.