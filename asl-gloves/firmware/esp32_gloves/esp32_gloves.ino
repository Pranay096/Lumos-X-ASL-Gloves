#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <MPU6050_light.h>
#include <NimBLEDevice.h>

// ══════════════════════════════════════════
//  BLE UUIDs
// ══════════════════════════════════════════
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHAR_VOICE_TO_SIGN  "12345678-1234-1234-1234-123456789ab1"
#define CHAR_SIGN_TO_VOICE  "12345678-1234-1234-1234-123456789ab2"

// ══════════════════════════════════════════
//  SERVO SETTINGS
// ══════════════════════════════════════════
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
MPU6050 mpu(Wire);

#define SERVO_FREQ   50
#define SERVOMIN     150
#define SERVOMAX     600

// ══════════════════════════════════════════
//  FLEX SENSOR PINS — GPIO26 not 36
// ══════════════════════════════════════════
const int flexPins[5] = {32, 33, 34, 35, 26};
int flexBaseline[5];
int flexBent[5];

// ══════════════════════════════════════════
//  BLE
// ══════════════════════════════════════════
NimBLECharacteristic *pVoiceChar;
NimBLECharacteristic *pSignChar;
bool deviceConnected = false;
String incomingText = "";
bool newTextAvailable = false;

// ══════════════════════════════════════════
//  ASL POSES
//  {Thumb, Index, Middle, Ring, Pinky, WristFlex, WristRotate}
// ══════════════════════════════════════════
struct ASLPose { int angles[7]; };

std::map<char, ASLPose> aslLetters = {
  {'A', {{ 90, 170, 170, 170, 170,  90,  90}}},
  {'B', {{170,   0,   0,   0,   0,  90,  90}}},
  {'C', {{ 60,  60,  60,  60,  60,  90,  90}}},
  {'D', {{170,   0, 160, 160, 160,  90,  90}}},
  {'E', {{170, 160, 160, 160, 160,  90,  90}}},
  {'F', {{ 90, 160,   0,   0,   0,  90,  90}}},
  {'G', {{ 90,  10,  80, 170, 170,  90,  45}}},
  {'H', {{ 90,  10,  10, 170, 170,  90,  45}}},
  {'I', {{170, 170, 170, 170,   0,  90,  90}}},
  {'J', {{170, 170, 170, 170,   0,  90, 180}}},
  {'K', {{ 90,  10,  20, 170, 170, 110,  90}}},
  {'L', {{  0,   0, 170, 170, 170,  90,  90}}},
  {'M', {{170, 160, 160, 160, 170,  90,  90}}},
  {'N', {{170, 160, 160, 170, 170,  90,  90}}},
  {'O', {{ 80,  80,  80,  80,  80,  90,  90}}},
  {'P', {{ 90,  10,  20, 170, 170,  45,  90}}},
  {'Q', {{ 90,  10,  80, 170, 170,  45,  90}}},
  {'R', {{170,  10,  10, 170, 170,  90,  90}}},
  {'S', {{170, 160, 160, 160, 160,  90,  90}}},
  {'T', {{170, 160, 170, 170, 170,  90,  90}}},
  {'U', {{170,  10,  10, 170, 170,  90,  90}}},
  {'V', {{170,  10,  10, 170, 170,  90,  90}}},
  {'W', {{170,  10,  10,  10, 170,  90,  90}}},
  {'X', {{170,  80, 170, 170, 170,  90,  90}}},
  {'Y', {{  0, 170, 170, 170,   0,  90,  90}}},
  {'Z', {{170,   0, 170, 170, 170,  90,  90}}},
};

// ══════════════════════════════════════════
//  SERVO HELPERS
// ══════════════════════════════════════════
int degreeToPulse(int degree) {
  return map(degree, 0, 180, SERVOMIN, SERVOMAX);
}

void neutralPose() {
  for (int i = 0; i < 5; i++)
    pwm.setPWM(i, 0, degreeToPulse(10));
  pwm.setPWM(5, 0, degreeToPulse(90));
  pwm.setPWM(6, 0, degreeToPulse(90));
  delay(200);
}

void setPose(ASLPose pose, int holdMs = 800) {
  for (int i = 0; i < 7; i++)
    pwm.setPWM(i, 0, degreeToPulse(pose.angles[i]));
  delay(holdMs);
}

void signWord(String word) {
  word.toUpperCase();
  for (int i = 0; i < word.length(); i++) {
    char c = word.charAt(i);
    if (c == ' ') {
      neutralPose();
      delay(500);
    } else if (aslLetters.count(c)) {
      setPose(aslLetters[c], 900);
      neutralPose();
    }
  }
  neutralPose();
}

// ══════════════════════════════════════════
//  FLEX SENSOR CALIBRATION — Fixed
// ══════════════════════════════════════════
void calibrateFlexSensors() {
  Serial.println("Calibrating — open hand flat...");
  delay(2000);
  for (int i = 0; i < 5; i++) {
    long sum = 0;
    for (int s = 0; s < 20; s++) {
      sum += analogRead(flexPins[i]);
      delay(10);
    }
    flexBaseline[i] = sum / 20;
    flexBent[i] = flexBaseline[i] * 0.85; // 15% drop = bent
    Serial.printf("Finger %d baseline: %d bent: %d\n",
                  i, flexBaseline[i], flexBent[i]);
  }
  Serial.println("Calibration done!");
}

bool fingerBent(int finger) {
  return analogRead(flexPins[finger]) < flexBent[finger];
}

// ══════════════════════════════════════════
//  SIGN RECOGNITION — Expanded
// ══════════════════════════════════════════
char recognizeSign() {
  bool b[5];
  for (int i = 0; i < 5; i++) b[i] = fingerBent(i);
  // b[0]=Thumb b[1]=Index b[2]=Middle b[3]=Ring b[4]=Pinky

  // All bent = A
  if ( b[0]&& b[1]&& b[2]&& b[3]&& b[4]) return 'A';
  // All open = B
  if (!b[0]&&!b[1]&&!b[2]&&!b[3]&&!b[4]) return 'B';
  // Thumb + index open = L
  if (!b[0]&&!b[1]&& b[2]&& b[3]&& b[4]) return 'L';
  // Thumb + pinky open = Y
  if (!b[0]&& b[1]&& b[2]&& b[3]&&!b[4]) return 'Y';
  // Only index up = D
  if ( b[0]&&!b[1]&& b[2]&& b[3]&& b[4]) return 'D';
  // Only pinky up = I
  if ( b[0]&& b[1]&& b[2]&& b[3]&&!b[4]) return 'I';
  // Index + middle up = V
  if ( b[0]&&!b[1]&&!b[2]&& b[3]&& b[4]) return 'V';
  // Index + middle + ring up = W
  if ( b[0]&&!b[1]&&!b[2]&&!b[3]&& b[4]) return 'W';
  // Two fingers sideways = H
  if ( b[0]&&!b[1]&&!b[2]&& b[3]&& b[4]) return 'H';
  // Thumb open only = A variant
  if (!b[0]&& b[1]&& b[2]&& b[3]&& b[4]) return 'O';
  // All curled tight = S
  if ( b[0]&& b[1]&& b[2]&& b[3]&& b[4]) return 'S';

  return '?';
}

// ══════════════════════════════════════════
//  BLE CALLBACKS
// ══════════════════════════════════════════
class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Phone connected!");
  }
  void onDisconnect(NimBLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Disconnected — restarting advertising...");
    NimBLEDevice::startAdvertising();
  }
};

class VoiceCharCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pChar) {
    incomingText = String(pChar->getValue().c_str());
    newTextAvailable = true;
    Serial.print("Received: ");
    Serial.println(incomingText);
  }
};

// ══════════════════════════════════════════
//  SETUP
// ══════════════════════════════════════════
void setup() {
  Serial.begin(115200);

  // I2C with correct pins
  Wire.begin(21, 22);

  // PCA9685
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  neutralPose();
  Serial.println("PCA9685 ready");

  // MPU6050
  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("MPU6050 ERROR — check wiring!");
  } else {
    Serial.println("MPU6050 ready");
    mpu.calcOffsets();
  }

  // Flex sensors
  calibrateFlexSensors();

  // BLE — NimBLE
  NimBLEDevice::init("SignGlove");
  NimBLEServer *pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  NimBLEService *pService = pServer->createService(SERVICE_UUID);

  // Voice → Sign
  pVoiceChar = pService->createCharacteristic(
    CHAR_VOICE_TO_SIGN,
    NIMBLE_PROPERTY::WRITE
  );
  pVoiceChar->setCallbacks(new VoiceCharCallbacks());

  // Sign → Voice
  pSignChar = pService->createCharacteristic(
    CHAR_SIGN_TO_VOICE,
    NIMBLE_PROPERTY::NOTIFY
  );

  pService->start();
  NimBLEDevice::startAdvertising();
  Serial.println("BLE ready — SignGlove");
}

// ══════════════════════════════════════════
//  LOOP
// ══════════════════════════════════════════
unsigned long lastSignCheck = 0;
char lastSentSign = '?';

void loop() {
  // Mode 1 — Voice to Sign
  if (newTextAvailable) {
    newTextAvailable = false;
    Serial.print("Signing: ");
    Serial.println(incomingText);
    signWord(incomingText);
  }

  // Mode 2 — Sign to Voice
  if (millis() - lastSignCheck > 300) {
    lastSignCheck = millis();
    char detected = recognizeSign();
    if (detected != '?' && detected != lastSentSign) {
      lastSentSign = detected;
      Serial.print("Sign detected: ");
      Serial.println(detected);
      if (deviceConnected) {
        pSignChar->setValue(std::string(1, detected));
        pSignChar->notify();
      }
    }
  }

  // MPU6050 update — was missing in original
  mpu.update();
}