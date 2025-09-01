// VVVVV  HC-12 'VAR-side' 가변저항 회로 코드   VVVVV
// RasINO IoT : https://rasino.tistory.com/
#include <SoftwareSerial.h>
#define DEFAULT_LED 13
#define JOISTICK_X A0
#define JOISTICK_X_SWITCH A1
#define JOISTICK_Y A3
#define JOISTICK_Y_SWITCH A4

SoftwareSerial HC12(2, 3); // HC-12 TX Pin, HC-12 RX Pin

int Var = 0;

void setup()
{
  Serial.begin(9600); // Computer ↔ 시리얼포트(아두이노) 통신속도
  HC12.begin(9600);   //             시리얼포트(아두이노) ↔ HC12   통신속도
  pinMode(DEFAULT_LED, OUTPUT);
  Serial.println("This is 'VAR controller' HC-12 Module");
}

void loop()
{
  if (analogRead(JOISTICK_X_SWITCH) == 0) // 조이스틱 스위치가 눌리면
  {
    Serial.println("Joystick Switch is Pressed");
    digitalWrite(DEFAULT_LED, HIGH);
  }else{
    Serial.println("Joystick Switch is Released");
    digitalWrite(DEFAULT_LED, LOW);
  }

  if (analogRead(JOISTICK_Y_SWITCH) == 0) // 조이스틱 스위치가 눌리면
  {
    Serial.println("Joystick Switch is Pressed");
    digitalWrite(DEFAULT_LED, HIGH);
    delay(100);
    digitalWrite(DEFAULT_LED, LOW);
    delay(100);
    digitalWrite(DEFAULT_LED, HIGH);
    delay(100);
    digitalWrite(DEFAULT_LED, LOW);
    delay(100);
    digitalWrite(DEFAULT_LED, HIGH);
  }else{
    Serial.println("Joystick Switch is Released");
    digitalWrite(DEFAULT_LED, LOW);
  }
  Var = map(analogRead(JOISTICK_X), 0, 1023, 0, 255);
  Serial.println(Var);
  HC12.write(Var); // 주의! : 'print()'가 아닌, 'write()'를 사용합니다
  // ※ 'print()'는 숫자의 경우 ASCII값으로 변환하여 전송하기 때문에,
  // 숫자값 그대로 전송하는 'write()'를 사용합니다
  delay(50); //TX,RX 딜레이 값이 동일해야 제대로 동작한다. delay가 다를 경우 송수신기에서 매핑 및 전처리되는 데이터가 상이하여 제대로 제어되지 않는 것으로 판단된다. 또한 delay 값이 짧을 수록 조향이 빠르게 좌우로 변경되어도 신호가 끊기지 않는다.
}
