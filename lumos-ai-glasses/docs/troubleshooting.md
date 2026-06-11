# Lumos AI Glasses — Troubleshooting

## Camera Not Detected

**Symptom:** `cv2.VideoCapture(0)` returns no frames, or the script exits immediately with a camera error.

**Fixes:**
- Run `vcgencmd get_camera` on the Raspberry Pi. Output should show `supported=1 detected=1`. If not, the camera is not connected or not enabled.
- Enable the camera interface: `sudo raspi-config` → Interface Options → Camera → Enable.
- Try `libcamera-hello` in the terminal to confirm the camera works at the OS level before running Python scripts.
- If using a USB webcam instead of the CSI module, change the device index: `cv2.VideoCapture(1)` or `cv2.VideoCapture(2)`.

---

## YOLOv8 Inference Is Very Slow

**Symptom:** Object detection runs at 1–2 FPS or lower, making real-time use impractical.

**Fixes:**
- Make sure you are using the nano model (`yolov8n.pt`), not a larger variant. Larger models (s, m, l, x) are too slow on Raspberry Pi hardware.
- Reduce input resolution in `object_detection.py`. Processing at 320×240 instead of 640×480 roughly doubles FPS.
- Close all other processes running on the Raspberry Pi.
- Consider exporting the model to NCNN format for faster ARM inference: `yolo export model=yolov8n.pt format=ncnn`.

---

## OpenAI / Gemini API Errors

**Symptom:** `AuthenticationError`, `RateLimitError`, or `ConnectionError` when running scene description.

**Fixes:**
- Check that your `.env` file exists in `lumos-ai-glasses/` and contains valid keys.
- Confirm the Raspberry Pi has an active internet connection: `ping google.com`.
- If you hit rate limits, add a delay between API calls in the script (`time.sleep(2)`).
- If internet is unavailable, the system falls back to announcing detected object labels only. This is expected behaviour.

---

## ESP32 OLED Not Displaying Text

**Symptom:** The transparent OLED shows nothing, or shows garbled characters.

**Fixes:**
- Confirm the I2C wiring: SDA → GPIO 21, SCL → GPIO 22 on the ESP32.
- Run an I2C scanner sketch on the ESP32 to confirm the display is detected at address `0x3C`.
- Check the serial connection between Raspberry Pi and ESP32. The correct baud rate is `115200`. Mismatched baud rates produce garbled output.
- Make sure the ESP32 firmware was uploaded successfully — check the Arduino IDE serial monitor for boot messages.

---

## SpeechRecognition Not Working

**Symptom:** `speech_to_text.py` throws errors or transcription is always empty.

**Fixes:**
- Confirm a microphone is connected and recognised: `arecord -l` should list your audio device.
- Set the correct device index in the script if multiple audio devices are present.
- The default recogniser uses Google's online API. If offline, switch to Vosk: install `pip install vosk` and download a Vosk model from [alphacephei.com/vosk/models](https://alphacephei.com/vosk/models).
- Test the microphone independently: `arecord -d 5 test.wav && aplay test.wav`.

---

## Sign Language Detection Is Inaccurate

**Symptom:** Signs are misclassified or not recognised at all.

**Fixes:**
- Ensure lighting is even and the hand is clearly visible against the background. Dark backgrounds with good frontal lighting work best.
- The ONNX model (`asl_model.onnx`) was trained on a specific dataset. Performance varies with hand size, skin tone, and signing style. Fine-tuning on your own data will improve accuracy.
- Check that MediaPipe is detecting hand landmarks — add a debug visualisation mode to `sign_language_detection.py` to draw landmarks on the frame.
- Ensure the camera frame rate is sufficient for gesture capture. At very low FPS, fast signs will be missed.

---

## Python Dependency Errors

**Symptom:** `ModuleNotFoundError` or version conflicts when running scripts.

**Fixes:**
- Use a virtual environment to isolate dependencies:
  ```bash
  python3 -m venv venv
  source venv/bin/activate
  pip install -r requirements.txt
  ```
- If `onnxruntime` fails to install on ARM, use the ARM-specific build:
  ```bash
  pip install onnxruntime  # for Pi 4 64-bit OS
  ```