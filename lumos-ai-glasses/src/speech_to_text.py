import socket
import speech_recognition as sr

ESP32_IP = "192.168.0.8"  #add your esp32 ip here
PORT = 1234

r = sr.Recognizer()
mic = sr.Microphone()

print("Speak...")

while True:
    with mic as source:
        r.adjust_for_ambient_noise(source)
        audio = r.listen(source)

    try:
        text = r.recognize_google(audio)
        print("You said:", text)

        s = socket.socket()
        s.connect((ESP32_IP, PORT))
        s.send((text + "\n").encode())
        s.close()

    except Exception as e:
        print("Error:", e)
