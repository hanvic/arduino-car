// ### HC-12 'LED-side' 가변저항(LED) 회로 코드   ###
// RasINO IoT : https://rasino.tistory.com/

#include <SoftwareSerial.h>
#include <Servo.h>
#define HC12_TX 3
#define HC12_RX 4

#define T6586_FI 10
#define T6586_BI 9

#define SERVO_MOTOR 6

SoftwareSerial HC12(HC12_TX, HC12_RX); // HC-12 TX Pin, HC-12 RX Pin
Servo myServo; // 서보 모터 객체 생성

void setup()
{
  Serial.begin(9600); // computer :양방향_화살표: 시리얼포트(아두이노) 통신속도
  HC12.begin(9600);   // 시리얼포트(아두이노) :양방향_화살표: HC12   통신속도
  delay(50);
  Serial.println("setup start");
  

  pinMode(T6586_FI,OUTPUT);
  pinMode(T6586_BI,OUTPUT);
  
  myServo.attach(SERVO_MOTOR);
  myServo.write(90); // 중앙 위치로 초기화
  
  digitalWrite(T6586_FI,LOW);
  digitalWrite(T6586_BI,LOW);
  Serial.println("setup end");
  Serial.println("Waiting for HC12 data...");
  
  // 핀 설정 확인
  Serial.print("T6586_FI pin: ");
  Serial.println(T6586_FI);
  Serial.print("T6586_BI pin: ");
  Serial.println(T6586_BI);
  
  HC12.println("start");
}

void loop()
{
  // HC12에서 데이터가 있는지 확인
  if(HC12.available()){
    Serial.print("Data received from HC12: ");
    
    String receivedData = HC12.readStringUntil('\n');
    receivedData.trim();
    int values[4]; // 최대 10개 값 저장
    int valueCount = 0;

    int startPos = 0;
    int commaPos = receivedData.indexOf(',');

    while (commaPos != -1 && valueCount < 4) {
        String valueStr = receivedData.substring(startPos, commaPos);
        valueStr.trim();
        if (valueStr.length() > 0) {
            values[valueCount] = valueStr.toInt();
            valueCount++;
        }
        startPos = commaPos + 1;
        commaPos = receivedData.indexOf(',', startPos);
    }
    if (startPos < receivedData.length() && valueCount < 4) {
        String valueStr = receivedData.substring(startPos);
        valueStr.trim();
        if (valueStr.length() > 0) {
            values[valueCount] = valueStr.toInt();
            valueCount++;
        }
    }

    HC12.println(receivedData);
    
    int speed = values[0];
    int left_button = values[1];
    int direction = values[2];
    int right_button = values[3];
    Serial.print(receivedData);
    // Serial.print(" [speed: ");
    // Serial.print(speed);
    // Serial.print(" left_button: ");
    // Serial.print(left_button);
    // Serial.print(" direction: ");
    // Serial.print(direction);
    // Serial.print(" right_button: ");
    // Serial.print(right_button);
    // Serial.println("]");

    // 수신된 데이터에 따른 모터 제어
    //최대는 168
    if(speed >= 525) // 한방향으로 모터 회전 (87~168 범위)
    {
      // 87~168 범위를 0~255로 매핑
      int mappedValueFront = map(speed, 521, 1023, 0, 255);
      analogWrite(T6586_FI, mappedValueFront);
      analogWrite(T6586_BI, 0);
      // Serial.print(" Motor forward: ");
      // Serial.print(speed);
      // Serial.print(" -> ");
      // Serial.print(mappedValueFront);
      // Serial.print(" (FI pin: ");
      // Serial.print(T6586_FI);
      // Serial.println(")");
      
    }
    else if(speed <= 515) // 반대방향으로 모터 회전 (0~83 범위)
    {
      // 0~83 범위를 255~0으로 매핑 (83->0, 0->255)
      int mappedValueBack = map(speed, 0, 520, 255, 0);
      analogWrite(T6586_FI, 0);
      analogWrite(T6586_BI, mappedValueBack);
      // Serial.print(" Motor backward: ");
      // Serial.print(speed);
      // Serial.print(" -> ");
      // Serial.print(mappedValueBack);
      // Serial.print(" (BI pin: ");
      // Serial.print(T6586_BI);
      // Serial.println(")");
    }
    else // 84~86 범위는 모터 정지
    {
      analogWrite(T6586_FI, 0);
      analogWrite(T6586_BI, 0);
      // Serial.print("Motor stop: ");
      // Serial.print(speed);
    }

    int mappedValueDirection = map(direction, 0, 1023, 0, 180);
    if(mappedValueDirection < 89 || 91 < mappedValueDirection){
      Serial.print(" direction: ");
      Serial.println(mappedValueDirection);
      myServo.write(mappedValueDirection);
    }else{
      Serial.print(" direction: ");
      Serial.println(mappedValueDirection);
    }
    
  // }

  
  // Serial.println("---");
  
  }

  
  

  //서보 컨트롤
  if(Serial.available()) {
    Serial.print("Data received from serial: ");
    String input = Serial.readString();
    input.trim(); // 앞뒤 공백 제거
    
    // 숫자인지 확인
    if(input.length() > 0) {
      int angle = input.toInt();
      
      // 각도 범위 확인 (0-180)
      if(angle >= 0 && angle <= 180) {
        myServo.write(angle);
        Serial.print("Servo moved to: ");
        Serial.print(angle);
        Serial.println(" degrees");
      } else {
        Serial.println("Invalid angle! Please enter a value between 0-180");
      }
    }
  }
  delay(50); // 로그 출력 후 짧은 딜레이
}
