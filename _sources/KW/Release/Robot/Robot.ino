#include <TimerOne.h>

void setup() {
  Serial.begin(9600);

  //pinMode(7, OUTPUT);
  //digitalWrite(7, HIGH);

  // 타이머초기화
  Timer1.initialize();
  Timer1.pwm(9, 0);
  
  Timer1.setPeriod(1000000);    //1Hz
  Timer1.setPwmDuty(9, 511);    //0~1023
  
  Serial.println("셋업완료");
}
void loop() {

  
}
