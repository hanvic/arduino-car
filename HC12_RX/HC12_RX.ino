// HC12_RX/HC12_RX.ino
#include <SoftwareSerial.h>
#include <Servo.h>

#define HC12_TX 2
#define HC12_RX 4
#define T6586_FI 9
#define T6586_BI 10
#define SERVO_MOTOR 5

SoftwareSerial HC12(HC12_TX, HC12_RX);
Servo myServo;

const int SERVO_DEADZONE = 8;
int lastServoAngle = 90;
unsigned long lastPrintTime = 0;

// 패킷 구조체
struct DataPacket {
  byte header;
  byte leftY;
  byte leftClick;
  byte rightX;
  byte rightClick;
};

DataPacket receivedPacket;
byte buffer[sizeof(DataPacket)];
int bufferIndex = 0;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  delay(50);
  pinMode(T6586_FI, OUTPUT);
  pinMode(T6586_BI, OUTPUT);
  
  // 서보 초기화
  myServo.attach(SERVO_MOTOR);
  myServo.write(90);  // 중앙 위치로 초기화
  lastServoAngle = 90;
  
  analogWrite(T6586_FI, 0);
  analogWrite(T6586_BI, 0);
  
  Serial.println("Servo Library Control Ready");
}

void loop1() {
    analogWrite(T6586_FI, 255);
    analogWrite(T6586_BI, 0);
    delay(10000);
    // analogWrite(T6586_FI, 0);
    // analogWrite(T6586_BI, 255);
    // delay(10000);
}
void loop() {
    
  // 바이너리 데이터 수신 및 파싱
  while (HC12.available()) {
    
    byte incomingByte = HC12.read();
    
    // 헤더 찾기
    if (bufferIndex == 0 && incomingByte == 0xAA) {
      buffer[bufferIndex++] = incomingByte;
    }
    // 나머지 데이터 수집
    else if (bufferIndex > 0 && bufferIndex < sizeof(DataPacket)) {
      buffer[bufferIndex++] = incomingByte;
      
      // 완전한 패킷 수신 완료
      if (bufferIndex == sizeof(DataPacket)) {
        memcpy(&receivedPacket, buffer, sizeof(DataPacket));
        processData();
        bufferIndex = 0; // 버퍼 리셋
      }
    }
    else {
      bufferIndex = 0; // 잘못된 데이터면 리셋
    }
  }
  
// analogWrite(T6586_FI, 255);
// analogWrite(T6586_BI, 0);
// delay(1000);
// analogWrite(T6586_FI, 0);
// analogWrite(T6586_BI, 255);
// delay(1000);
// analogWrite(T6586_FI, 255);
// analogWrite(T6586_BI, 0);
// delay(1000);
// analogWrite(T6586_FI, 0);
// analogWrite(T6586_BI, 255);
// delay(1000);
// analogWrite(T6586_FI, 255);
// analogWrite(T6586_BI, 0);
// delay(1000);
// analogWrite(T6586_FI, 0);
// analogWrite(T6586_BI, 255);
// delay(1000);
// analogWrite(T6586_FI, 0);
// analogWrite(T6586_BI, 0);
// delay(1000);
//   delay(10);
}

void processData() {
  unsigned long currentTime = millis();
  
  // 3초마다 디버깅 출력
  if (currentTime - lastPrintTime >= 3000) {
    Serial.println(receivedPacket.leftY);
    Serial.println(receivedPacket.rightX); 
    Serial.println(receivedPacket.leftClick);
    Serial.println(receivedPacket.rightClick);
    Serial.println("--------------------------------");
    lastPrintTime = currentTime;
  }
  
  // 모터 제어
  int speed = receivedPacket.leftY;
  int direction = receivedPacket.rightX;
  bool leftClick = receivedPacket.leftClick;
  bool rightClick = receivedPacket.rightClick;
  if(leftClick) {
    Serial.println("Left Click");
  }
  if(rightClick) {
    Serial.println("Right Click");
  }
  
  if(speed >= 131) {
    int mappedValueFront = map(speed, 175, 255, 50, 255);
    Serial.println("Front");
    Serial.println(speed);
    Serial.println(mappedValueFront);
    analogWrite(T6586_FI, 255);
    analogWrite(T6586_BI, 0);
    // delay(100);
  }
  else if(speed <= 123) {
    int mappedValueBack = map(speed, 0, 123, 255, 50);
    Serial.println("Back");
    Serial.println(speed);
    Serial.println(mappedValueBack);
    analogWrite(T6586_FI, 0);
    analogWrite(T6586_BI, 255);
    // delay(100);
  }
  else {
    analogWrite(T6586_FI, 0);
    analogWrite(T6586_BI, 0);
    // delay(100);
  }
  
  // 서보 제어 - Servo.h 라이브러리 사용
  int mappedDirection = map(direction, 0, 255, 0, 180);
  
  if(rightClick) {
    mappedDirection = 90;
    myServo.write(mappedDirection);
    lastServoAngle = mappedDirection;
  }else{
    // 각도 변화가 데드존을 넘을 때만 업데이트
    if(abs(mappedDirection - lastServoAngle) > SERVO_DEADZONE) {
        
        // 중앙 데드존 체크 (80~100도는 90도로 고정)
        if(mappedDirection < 80 || mappedDirection > 100) {
            // Serial.println(mappedDirection);
        myServo.write(mappedDirection);
        lastServoAngle = mappedDirection;
        
        
        } else {
            // 중앙 영역이면 90도로 고정
            if(lastServoAngle != 90) {
                // myServo.write(90);
                lastServoAngle = 90;
                
            }
        }
    }
  }
}