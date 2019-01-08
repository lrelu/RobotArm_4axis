#include <Servo.h>

int vr0 = 0;

void setup() {
  // 설정
  Serial.begin(9600);
  // 핀모드 설정
  //pinMode(6, OUTPUT);
  // 테스트 - 우선 5번 포트에 연결된 모터 구동
  //analogWrite(6, 90);
  Servo sv5, sv6, sv10, sv11;
  sv5.attach(5);
  sv5.write(0);
  sv6.attach(6);
  sv6.write(0);
  sv10.attach(10);
  sv10.write(0);
  sv11.attach(11);
  sv11.write(0);
}

void loop() {
  vr0 = analogRead(A0);
  Serial.println(vr0);
  delay(100);
}
