
Pranay Pelapkar <pranaypelapkar11@gmail.com>
15:00 (0 minutes ago)
to pranaytp24hcompe

import threading
import requests
import base64
from PIL import Image
from io import BytesIO
from picamera2 import Picamera2
import cv2
from flask import Flask, Response, jsonify

# ========= CONFIG =========
GEMINI_API_KEY = "YOUR API KEY"

# ========= CAMERA =========
picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(
    main={"size": (480, 360)}   # ⭐ faster + stable
))
picam2.start()

# ========= FLASK =========
app = Flask(__name__)

# ========= CAMERA STREAM =========
def generate_preview():
    while True:
        frame = picam2.capture_array()
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)

        ret, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 60])
        frame_bytes = buffer.tobytes()

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame_bytes + b'\r\n')

@app.route('/')
def video_feed():
    return Response(generate_preview(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

# ========= GEMINI =========
def gemini_request(img, prompt):

    img = img.resize((480, 360))
    buffer = BytesIO()
    img.save(buffer, format="JPEG", quality=55)
    img_b64 = base64.b64encode(buffer.getvalue()).decode()

    url = f"https://generativelanguage.googleapis.com/v1/models/gemini-2.5-flash:generateContent?key={GEMINI_API_KEY}"

    payload = {
        "contents": [{
            "parts": [
                {"text": prompt},
                {"inline_data": {"mime_type": "image/jpeg", "data": img_b64}}
            ]
        }]
    }

    try:
        r = requests.post(url, json=payload, timeout=20)
        return r.json()["candidates"][0]["content"]["parts"][0]["text"]
    except:
        return "Network slow. Try again."

# ========= SCAN =========
@app.route("/scan")
def scan():

    frame = picam2.capture_array()
    img = Image.fromarray(frame).convert("RGB")

    desc = gemini_request(
        img,
        "Describe ONLY visible obstacles and safest walking direction in MAX 2 short sentences."
    )

    return jsonify({"response": desc})

# ========= OCR =========
@app.route("/read")
def read():

    frame = picam2.capture_array()
    img = Image.fromarray(frame).convert("RGB")

    text = gemini_request(
        img,
        "Extract ONLY clearly readable printed text in one short sentence."
    )

    return jsonify({"response": text})

# ========= UI =========
@app.route("/ui")
def ui():
    return """
    <html>
    <body style="background:black;color:white;text-align:center;font-family:Arial">

    <h1>🔮 Lumos Vision</h1>

    <img src="/" width="480"><br><br>

    <button onclick="scan()" style="padding:15px;font-size:18px">Scan</button>
    <button onclick="read()" style="padding:15px;font-size:18px">Read</button>

    <p id="res"></p>

    <script>
    function speak(t){
        let s = new SpeechSynthesisUtterance(t);
        s.rate = 1;
        speechSynthesis.speak(s);
    }

    function scan(){
        fetch('/scan')
        .then(r=>r.json())
        .then(d=>{
            document.getElementById('res').innerText=d.response;
            speak(d.response);
        });
    }

    function read(){
        fetch('/read')
        .then(r=>r.json())
        .then(d=>{
            document.getElementById('res').innerText=d.response;
            speak(d.response);
        });
    }
    </script>

    </body>
    </html>
    """

# ========= MAIN =========
if __name__ == "__main__":

    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8",80))
    ip = s.getsockname()[0]
    s.close()

    print("⭐ Open Lumos at → http://" + ip + ":5000/ui")

    app.run(host="0.0.0.0", port=5000)
