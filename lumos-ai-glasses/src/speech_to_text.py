import speech_recognition as sr

class SpeechTranscriber:
    def __init__(self):
        self.recognizer = sr.Recognizer()
        self.microphone = sr.Microphone()
        print("Speech Transcriber initialized.")

    def transcribe_live(self, callback):
        with self.microphone as source:
            print("Calibrating microphone ambient noise...")
            self.recognizer.adjust_for_ambient_noise(source)
            print("Ready! Speak clearly into the microphone.")
            
            while True:
                try:
                    audio = self.recognizer.listen(source, timeout=5, phrase_time_limit=10)
                    print("Processing speech...")
                    text = self.recognizer.recognize_google(audio)
                    callback(text)
                except sr.WaitTimeoutError:
                    continue
                except sr.UnknownValueError:
                    print("Could not understand audio.")
                except sr.RequestError as e:
                    print(f"Speech recognition service error: {e}")
                    break

def print_transcript(text):
    print(f"Transcribed Text: {text}")

if __name__ == "__main__":
    transcriber = SpeechTranscriber()
    try:
        transcriber.transcribe_live(print_transcript)
    except KeyboardInterrupt:
        print("\nExited speech transcribing.")
