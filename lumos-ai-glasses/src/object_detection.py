import cv2
from ultralytics import YOLO

class ObjectDetector:
    def __init__(self, model_path="models/yolov8n.pt"):
        print(f"Loading YOLOv8 model from {model_path}...")
        self.model = YOLO(model_path)

    def detect(self, frame):
        # Run inference
        results = self.model(frame)
        
        # Parse results and extract labels
        detections = []
        for result in results:
            for box in result.boxes:
                class_id = int(box.cls[0])
                label = self.model.names[class_id]
                confidence = float(box.conf[0])
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                detections.append({
                    "label": label,
                    "confidence": confidence,
                    "bbox": (x1, y1, x2, y2)
                })
        return detections

if __name__ == "__main__":
    detector = ObjectDetector()
    cap = cv2.VideoCapture(0)
    
    print("Starting Object Detection Stream (press 'q' to exit)...")
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
            
        detections = detector.detect(frame)
        for det in detections:
            x1, y1, x2, y2 = det["bbox"]
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, f"{det['label']} {det['confidence']:.2f}", 
                        (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
            
        cv2.imshow("Object Detection", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
            
    cap.release()
    cv2.destroyAllWindows()
