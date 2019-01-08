#include <Servo.h>

//디버그 모드
bool isDebug = true;
//가변저항
int vr1, vr2, vr3, vr4;     // A0, A1, A2, A3
//서버모터
Servo sv1, sv2, sv3, sv4;   //5번핀, 6번핀, 10번핀, 11번핀
//모터로 출력값
int mv1, mv2, mv3, mv4;     //가변저항값(0-1023)값을 0-180으로 변경

void setup() {
  // 설정
  Serial.begin(9600);
  // 핀모드 설정
  // 아날로그 입력포트라서 디폴트 되어 있음
  // 서보모터 설정
  sv1.attach(5);  sv2.attach(6);  sv3.attach(10); sv4.attach(11);
}

void loop() {
  // 가변저항 Read
  ReadVR();

  // 스케일링
  ScaleVR();
  
  // 서보모터 출력
  WriteServo();

  // 속도처리
  delay(100);
}

void ScaleVR(){
  // 가변저항 0-1023을 0-180으로 스케일링
  mv1 = map(vr1, 0, 1023, 0, 180);
  mv2 = map(vr2, 0, 1023, 0, 180);
  mv3 = map(vr3, 0, 1023, 0, 160);
  mv4 = map(vr4, 0, 1023, 0, 180);

  if (isDebug){
    Serial.print("mv1:"); Serial.print(mv1);
    Serial.print("/ mv2:"); Serial.print(mv2);
    Serial.print("/ mv3:"); Serial.print(mv3);
    Serial.print("/ mv4:"); Serial.println(mv4);
  } 
}

void WriteServo(){
  // 서보모터 출력
  sv1.write(mv1); sv2.write(mv2); sv3.write(mv3); sv4.write(mv4);
}

void ReadVR() {
  // 가변저항 읽기
  vr1 = analogRead(A0);
  vr2 = analogRead(A1);
  vr3 = analogRead(A2);
  vr4 = analogRead(A3);
}
