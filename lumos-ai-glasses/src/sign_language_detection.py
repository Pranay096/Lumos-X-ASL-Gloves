import cv2
import numpy as np
import onnxruntime as ort

class ASLSignDetector:
    def __init__(self, model_path="models/asl_model.onnx"):
        print(f"Loading ASL ONNX model from {model_path}...")
        self.session = ort.InferenceSession(model_path)
        self.input_name = self.session.get_inputs()[0].name
        self.classes = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
                        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
                        'space', 'del', 'nothing']

    def preprocess(self, frame):
        # Convert frame to gray or resize depending on input training details
        # Assuming 224x224 RGB image normalized
        img = cv2.resize(frame, (224, 224))
        img = img.astype(np.float32) / 255.0
        img = np.transpose(img, (2, 0, 1))  # HWC to CHW
        img = np.expand_dims(img, axis=0)   # Add batch dimension
        return img

    def predict(self, frame):
        input_data = self.preprocess(frame)
        outputs = self.session.run(None, {self.input_name: input_data})
        predictions = outputs[0]
        class_idx = np.argmax(predictions[0])
        confidence = float(predictions[0][class_idx])
        return self.classes[class_idx], confidence

if __name__ == "__main__":
    detector = ASLSignDetector()
    cap = cv2.VideoCapture(0)
    
    print("Starting ASL Sign Language Stream (press 'q' to exit)...")
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
            
        sign, confidence = detector.predict(frame)
        if confidence > 0.7:
            cv2.putText(frame, f"Sign: {sign} ({confidence:.2%})", 
                        (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
            
        cv2.imshow("ASL Sign Language Detection", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
            
    cap.release()
    cv2.destroyAllWindows()
