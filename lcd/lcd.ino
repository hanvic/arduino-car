/*
 * DFRobot LCD Keypad Shield 테스트 코드 (라이브러리 없이)
 * 
 * 이 코드는 DFRobot LCD Keypad Shield의 기본 기능을 테스트합니다:
 * - LCD 표시 기능 (직접 핀 제어)
 * - 5개 버튼 (UP, DOWN, LEFT, RIGHT, SELECT) 입력 처리
 * - 간단한 메뉴 시스템
 * 
 * 연결:
 * - LCD Keypad Shield를 Arduino Uno에 직접 장착
 * - 별도 배선 불필요
 * 
 * 핀 할당:
 * - LCD: 8, 9, 4, 5, 6, 7
 * - 버튼: A0 (아날로그 핀)
 */

// LCD 핀 정의
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

// 버튼 핀 정의
const int buttonPin = A0;

// 버튼 값 정의 (아날로그 값) - 실제 값으로 수정 필요
const int BUTTON_NONE = 1023;
const int BUTTON_RIGHT = 0;
const int BUTTON_UP = 131;
const int BUTTON_DOWN = 308;
const int BUTTON_LEFT = 481;
const int BUTTON_SELECT = 720;

// 메뉴 상태 변수
int currentMenu = 0;
int menuItem = 0;
const int MENU_COUNT = 3;

// 메뉴 항목들
const char* menuItems[] = {
  "Button Test",
  "Counter Demo", 
  "Simple Text"
};

// 카운터 변수
int counter = 0;

void setup() {
  // 시리얼 통신 초기화
  Serial.begin(9600);
  Serial.println("DFRobot LCD Keypad Shield 테스트 시작");
  Serial.println("버튼 매핑 디버깅 모드");
  Serial.println("================================");
  
  // LCD 핀들을 출력으로 설정
  pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);
  
  // LCD 초기화
  lcdInit();
  
  // 시작 메시지 표시
  lcdClear();
  lcdSetCursor(0, 0);
  lcdPrint("Button Debug");
  lcdSetCursor(0, 1);
  lcdPrint("Press buttons!");
  
  Serial.println("버튼을 눌러서 아날로그 값을 확인하세요:");
  Serial.println("버튼을 누르면 아날로그 값이 출력됩니다.");
  Serial.println("================================");
}

void loop() {
  // 버튼 입력 확인
  int buttonValue = analogRead(buttonPin);
  
  // 버튼이 눌렸을 때만 출력 (값이 1023이 아닐 때)
  if (buttonValue < 1000) {
    Serial.print("버튼 아날로그 값: ");
    Serial.println(buttonValue);
    
    // LCD에도 표시
    lcdClear();
    lcdSetCursor(0, 0);
    lcdPrint("Button Value:");
    lcdSetCursor(0, 1);
    lcdPrint(buttonValue);
    
    delay(500); // 0.5초 대기
  }
  
  // 1초마다 현재 상태 출력
  static unsigned long lastReport = 0;
  if (millis() - lastReport > 1000) {
    Serial.print("현재 아날로그 값: ");
    Serial.println(buttonValue);
    lastReport = millis();
  }
}

// LCD 초기화 함수
void lcdInit() {
  delay(50);
  
  // 4비트 모드로 초기화
  digitalWrite(rs, LOW);
  digitalWrite(en, LOW);
  
  // Function Set: 4-bit mode, 2 lines, 5x8 font
  lcdWrite4Bits(0x03);
  delay(5);
  lcdWrite4Bits(0x03);
  delay(5);
  lcdWrite4Bits(0x03);
  delay(5);
  lcdWrite4Bits(0x02);
  
  // Function Set: 4-bit mode, 2 lines, 5x8 font
  lcdCommand(0x28);
  
  // Display Control: Display ON, Cursor OFF, Blink OFF
  lcdCommand(0x0C);
  
  // Entry Mode Set: Increment cursor, No display shift
  lcdCommand(0x06);
  
  // Clear Display
  lcdCommand(0x01);
  delay(2);
}

// LCD 명령어 전송
void lcdCommand(byte command) {
  digitalWrite(rs, LOW);
  lcdWrite4Bits(command >> 4);
  lcdWrite4Bits(command);
}

// LCD 데이터 전송
void lcdWrite(byte data) {
  digitalWrite(rs, HIGH);
  lcdWrite4Bits(data >> 4);
  lcdWrite4Bits(data);
}

// 4비트 데이터 전송
void lcdWrite4Bits(byte data) {
  digitalWrite(d4, (data >> 0) & 1);
  digitalWrite(d5, (data >> 1) & 1);
  digitalWrite(d6, (data >> 2) & 1);
  digitalWrite(d7, (data >> 3) & 1);
  
  digitalWrite(en, HIGH);
  delayMicroseconds(1);
  digitalWrite(en, LOW);
  delayMicroseconds(50);
}

// LCD 클리어
void lcdClear() {
  lcdCommand(0x01);
  delay(2);
}

// LCD 커서 설정
void lcdSetCursor(int col, int row) {
  int address = row * 0x40 + col;
  lcdCommand(0x80 | address);
}

// LCD에 텍스트 출력
void lcdPrint(const char* text) {
  for (int i = 0; text[i] != '\0'; i++) {
    lcdWrite(text[i]);
  }
}

// LCD에 숫자 출력
void lcdPrint(int number) {
  char buffer[10];
  sprintf(buffer, "%d", number);
  lcdPrint(buffer);
}
