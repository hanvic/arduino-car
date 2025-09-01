// HC12_TX/HC12_TX.ino
#include <SoftwareSerial.h> 

#define HC12_TX 2
#define HC12_RX 3
#define HC12_SET 4

#define JOYSTICK_LEFT A0
#define JOYSTICK_LEFT_CLICK 10
#define JOYSTICK_RIGHT A3
#define JOYSTICK_RIGHT_CLICK 11

SoftwareSerial HC12(HC12_TX, HC12_RX);

// 패킷 구조체 (5바이트)
struct DataPacket {
  byte header;        // 0xAA (시작 바이트)
  byte leftY;         // 0-255 (10bit -> 8bit 변환)
  byte leftClick;     // 0 or 1
  byte rightX;        // 0-255 (10bit -> 8bit 변환)
  byte rightClick;    // 0 or 1
};

void setMaxPower() {
  Serial.println("Setting HC12 to maximum power...");
  
  // 1. SET 핀을 LOW로 (AT 모드 진입)
  pinMode(HC12_SET, OUTPUT);  // SET 핀을 7번으로 가정
  digitalWrite(HC12_SET, LOW);
  delay(100);
  
  // 2. 최대 파워 설정 (FU4)
  /*
  AT+FU1 = 1200bps   (최대 거리)
  AT+FU2 = 2400bps
  AT+FU3 = 4800bps
  AT+FU4 = 9600bps   (기본값, 적당한 거리)
  AT+FU5 = 19200bps
  AT+FU6 = 38400bps
  AT+FU7 = 57600bps
  AT+FU8 = 115200bps (최소 거리)
*/
  HC12.print("AT+FU4");
  delay(100);
  
  // 3. 최대 전송 파워 설정 (P8)
  /* 
  AT+P1 = -1dBm   (최소)
  AT+P2 = 2dBm
  AT+P3 = 5dBm
  AT+P3 = 5dBm
  AT+P4 = 8dBm
  AT+P5 = 11dBm
  AT+P6 = 14dBm
  AT+P7 = 17dBm
  AT+P8 = 20dBm   (최대)
  */
  HC12.print("AT+P8");
  delay(100);
  
  // 4. 설정 확인
  HC12.print("AT+RX");
  delay(100);
  
  // 5. SET 핀을 HIGH로 (통신 모드)
  digitalWrite(HC12_SET, HIGH);
  delay(100);
  
  Serial.println("HC12 power set to maximum!");
}

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