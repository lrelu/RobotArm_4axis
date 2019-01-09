#include <Servo.h>

// 디버그 모드
const bool isDebug = true;
// 딜레이 속도
const int delaySpeed = 100;
const int runSpeed = 2000;
// 자동실행모드
volatile bool isAutoRun = false;
// 가변저항
volatile int vr1, vr2, vr3, vr4;     // A0, A1, A2, A3
// 서버모터 객체
Servo sv1, sv2, sv3, sv4;   //5번핀, 6번핀, 10번핀, 11번핀
Servo sv;
// 모터로 출력값
volatile int mv1, mv2, mv3, mv4;     //가변저항값(0-1023)값을 0-180으로 변경
// 버튼 값
volatile bool pbLearn, pbRun;        //버튼값
// Pin 정의
const int pin_pbLearn = 2, pin_pbRun = 3;   //버튼 핀
const int pin_ledState = 12;                //상태 확인 핀

// 상태램프값
volatile byte state = LOW;

// 가변저항 상태 저장 값
volatile int motion[10][4];
volatile int motion_index = 0;


// 초기설정
void setup() {
  Serial.begin(9600);
  // 핀모드 설정
  pinMode(pin_pbLearn, INPUT);        // 순간의 설정된 서보모터 출력값 저장
  pinMode(pin_pbRun, INPUT);          // 실행

  // 버튼입력 인터럽트로 처리
  attachInterrupt(digitalPinToInterrupt(pin_pbLearn), run_clikced, RISING);
  attachInterrupt(digitalPinToInterrupt(pin_pbRun), learn_clicked, RISING);
  
  // 아날로그 입력포트라서 디폴트 되어 있음
  // 서보모터 설정
  sv1.attach(5);  sv2.attach(6);  sv3.attach(10); sv4.attach(11);
}

void loop() {
  // 버튼
  if (isAutoRun){
    //자동실행 모드 LED 상태 (불이 켜있음)
    state = HIGH;
    
    // 모션 실행
    run_motion();
  }else{
    //학습모드 LED 상태 (깜박깜박)
    state = !state;
    
    //로봇팔 위치 학습 모드
    // 가변저항 Read
    readVR();
  
    // 스케일링
    scaleVR();
    
    // 서보모터 출력
    writeServo(mv1, mv2, mv3, mv4);
    
    // 속도처리
    delay(delaySpeed);
  }

  // 상태확인
  turnOnOffLED();
}

// 자동실행 버튼 클릭
void run_clikced(){
  // 자동실행 모드로 전환한 후 자동 실행
  isAutoRun = !isAutoRun;

  // 디버그 모드일 경우에 저장된 모션값들 시리얼에 출력
  if (isDebug)
    view_motion();
}

// 모션 학습 버튼 클릭
void learn_clicked(){
  isAutoRun = false;

  //모션 저장
  add_motion();

  if (isDebug){
    Serial.print(vr1); Serial.print(" / ");
    Serial.print(vr2); Serial.print(" / ");
    Serial.print(vr3); Serial.print(" / ");
    Serial.print(vr4); Serial.println(" / ");
  }
}

// 모션에 저장된 위치값 실행 (순간순간 딜레이 속도 delaySpeed)
void run_motion(){
  for (int i = 0; i < motion_index; i++){
    vr1 = motion[i][0];
    vr2 = motion[i][1];
    vr3 = motion[i][2];
    vr4 = motion[i][3];
    
    writeServo(vr1, vr2, vr3, vr4);
    delay(runSpeed);
  }
}

// [디버깅용] 현재 저장된 모션값들 확인
void view_motion(){
  for (int i = 0; i < motion_index; i++){
    Serial.print(i);
    Serial.println("============================");
    Serial.print(motion[i][0]); Serial.print(" / ");
    Serial.print(motion[i][1]); Serial.print(" / ");
    Serial.print(motion[i][2]); Serial.print(" / ");
    Serial.print(motion[i][3]); Serial.println(" / ");
  }
}

// 현재 가변저항(서보모터 위치)값을 배열에 저장
void add_motion(){
  motion[motion_index][0] = vr1;
  motion[motion_index][1] = vr2;
  motion[motion_index][2] = vr3;
  motion[motion_index][3] = vr4;

  motion_index++;
  motion_index = motion_index % 10;
}

// 상태 LED 출력
void turnOnOffLED(){
  digitalWrite(pin_ledState, state);
}

// 가변저항 0-1023을 0-180으로 스케일링
void scaleVR(){
  mv1 = map(vr1, 0, 1023, 10, 160);
  mv2 = map(vr2, 0, 1023, 10, 160);
  mv3 = map(vr3, 0, 1023, 10, 160);
  mv4 = map(vr4, 0, 1023, 10, 170);
}

//서보모터 출력
void writeServo(int v1, int v2, int v3, int v4){
  sv1.write(v1); sv2.write(v2); sv3.write(v3); sv4.write(v4);
  if (isDebug){
    Serial.print(vr1); Serial.print(" / ");
    Serial.print(vr2); Serial.print(" / ");
    Serial.print(vr3); Serial.print(" / ");
    Serial.print(vr4); Serial.println(" / ");
  }
}

// 가변저항 읽기
void readVR() {
  vr1 = analogRead(A0);
  vr2 = analogRead(A1);
  vr3 = analogRead(A2);
  vr4 = analogRead(A3);
}
