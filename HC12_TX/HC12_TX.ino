// HC12_TX/HC12_TX.ino
#include <SoftwareSerial.h> 

#define JOYSTICK_LEFT A0
#define JOYSTICK_LEFT_CLICK 10
#define JOYSTICK_RIGHT A3
#define JOYSTICK_RIGHT_CLICK 11

SoftwareSerial HC12(5, 6);

// 패킷 구조체 (5바이트)
struct DataPacket {
  byte header;        // 0xAA (시작 바이트)
  byte leftY;         // 0-255 (10bit -> 8bit 변환)
  byte leftClick;     // 0 or 1
  byte rightX;        // 0-255 (10bit -> 8bit 변환)
  byte rightClick;    // 0 or 1
};

void setup() { 
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(JOYSTICK_LEFT_CLICK, INPUT_PULLUP);
  pinMode(JOYSTICK_RIGHT_CLICK, INPUT_PULLUP);
  pinMode(JOYSTICK_LEFT, INPUT);
  pinMode(JOYSTICK_RIGHT, INPUT);
} 

void loop() { 
  DataPacket packet;
  packet.header = 0xAA;  // 패킷 시작 식별자

  // 10비트 값을 8비트로 매핑 (0-1023 -> 0-255)
  packet.leftY = map(analogRead(JOYSTICK_LEFT), 0, 1023, 0, 255);
  packet.rightX = map(analogRead(JOYSTICK_RIGHT), 0, 1023, 0, 255);
  
  // 디지털 입력 (풀업이므로 반전)
  packet.leftClick = !digitalRead(JOYSTICK_LEFT_CLICK);
  packet.rightClick = !digitalRead(JOYSTICK_RIGHT_CLICK);
  
  // 바이너리 전송 (5바이트)
  HC12.write((byte*)&packet, sizeof(packet));
  
  delay(50); // 기존 50ms에서 20ms로 단축
}