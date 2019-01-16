#include <Servo.h> 
#include <EEPROM.h>

const byte Pot1 = A0;
const byte Pot2 = A1;
const byte Pot3 = A2;
const byte Pot4 = A3;

const byte SPin1 = 11;
const byte SPin2 = 10;
const byte SPin3 = 9;
const byte SPin4 = 6;

const byte Switch_Record = 4;
const byte Switch_Run = 3;

const byte running_speed = 100; // 1당 10ms

byte state = 0;  // 0 : Record   / 1 : Run

byte prev_s1_angle = 0, prev_s2_angle = 0, prev_s3_angle = 0, prev_s4_angle = 0;
byte s1_angle, s2_angle, s3_angle, s4_angle ;

bool prev_record;
bool prev_run;

bool pres_record;
bool pres_run;

byte pos_arr[4][50]; // 위치 저장 배열
byte pos_num = 0;    // 저장 수
byte pos_cnt = 0;    // 딜레이를 낮추고 카운트를 이용해서 시간제어
byte pos_step = 0;   // 데이터 로드 인덱스

Servo servo1, servo2, servo3, servo4;

void setup()
{
    Serial.begin(115200);
    servo1.attach(SPin1);
    servo2.attach(SPin2);
    servo3.attach(SPin3);
    servo4.attach(SPin4);

    pinMode(Switch_Record, INPUT);
    pinMode(Switch_Run, INPUT);
    
    Chk_eeprom();
}

void loop()
{
    Read_signal();  // 가변저항값 읽어서 매핑, 버튼 현재값 읽기

    Record_Button_Chk();  // 기록 버튼 처리

    Run_Button_Chk(); // 시작버튼 처리

    if (state == 0) 
      Recording(); // 기록설정 모드
    
    else if (state == 1) 
      Running(); // 구동 모드
    
    prev_record = pres_record;
    prev_run = pres_run;
        
    delay(10);
}

// ====================== 함수============================

void Chk_eeprom()
{
  // EEPROM 사용처
  // 0 : 저장 데이터 수
  // 1~511 : 4단위로 저장 위치
  
  pos_num = EEPROM.read(0);

  // 아두이노의 eeprom 처음 사용시 255값이 저장되어있기때문에 0으로 전부 초기화시켜줌
  if(pos_num > 50)
  {
    for (int i = 0 ; i < EEPROM.length() ; i++) 
      EEPROM.write(i, 0);
  }
  
  // 데이터 있는경우
  if(pos_num > 0)
  {
    for(int i = 0; i <  pos_num ; i++)
    {
      pos_arr[0][i] = EEPROM.read((i*4) + 1);
      pos_arr[1][i] = EEPROM.read((i*4) + 2);
      pos_arr[2][i] = EEPROM.read((i*4) + 3);
      pos_arr[3][i] = EEPROM.read((i*4) + 4);
    }
    state = 1;

    servo1.write(pos_arr[0][0]);
    servo2.write(pos_arr[1][0]);
    servo3.write(pos_arr[2][0]);
    servo4.write(pos_arr[3][0]);
  }
  Serial.print("pos_num = ");
  Serial.println(pos_num);
}

// 가변저항값 읽어서 매핑, 버튼 현재값 읽기
void Read_signal()
{
    s1_angle = map(analogRead(Pot1), 1, 1023, 5, 160);
    s2_angle = map(analogRead(Pot2), 1, 1023, 5, 160);
    s3_angle = map(analogRead(Pot3), 1, 1023, 5, 160);
    s4_angle = map(analogRead(Pot4), 1, 1023, 5, 160);
    
    pres_record = digitalRead(Switch_Record);
    pres_run = digitalRead(Switch_Run);
}

// 기록 버튼 처리
void Record_Button_Chk(){
  if(prev_record == 0 && pres_record == 1)
  {
    if(state == 0 && pos_num < 50)
    {
      pos_arr[0][pos_num] = s1_angle;
      pos_arr[1][pos_num] = s2_angle;
      pos_arr[2][pos_num] = s3_angle;
      pos_arr[3][pos_num] = s4_angle;
      pos_num++;
      Serial.print("pos_num = ");
      Serial.println(pos_num);
    }
  }
}

// 시작, 중지버튼 처리
void Run_Button_Chk()
{
    if(prev_run == 0 && pres_run == 1)
    {
      if(state == 1)
      {
        state = 0;
        pos_num = 0;
        pos_step = 0;
        pos_cnt = 0;

        EEPROM.write(0,0);
        servo1.write(s1_angle);
        servo2.write(s2_angle);
        servo3.write(s3_angle);
        servo4.write(s4_angle);
        
        Serial.print("stop , num = ");
        Serial.println(pos_num);
      }
      
      else
      {
        // 저장된게 있을때만 실행
        if(pos_num > 0)
        {
          state = 1;
          pos_step = 0;
          pos_cnt = 0;

          EEPROM.write(0,pos_num);
          for(int i = 0; i <  pos_num ; i++)
          {
            EEPROM.write((i*4) + 1, pos_arr[0][i]);
            EEPROM.write((i*4) + 2, pos_arr[1][i]);
            EEPROM.write((i*4) + 3, pos_arr[2][i]);
            EEPROM.write((i*4) + 4, pos_arr[3][i]);
          }
         Serial.print("start , num = ");
         Serial.println(pos_num);
        }
        
        else
        {
         Serial.print("not start , num = ");
         Serial.println(pos_num);
        }
        
      }
    }
}

// 기록 모드
void Recording()
{
  if (prev_s1_angle != s1_angle)
  {
      if (abs(prev_s1_angle - s1_angle) > 1)
      {
          servo1.write(s1_angle);
          prev_s1_angle = s1_angle;
      }
  }

  if (prev_s2_angle != s2_angle)
  {
      if (abs(prev_s2_angle - s2_angle) > 1)
      {
          servo2.write(s2_angle);
          prev_s2_angle = s2_angle;
      }
  }

  if (prev_s3_angle != s3_angle)
  {
      if (abs(prev_s3_angle - s3_angle) > 1)
      {
          servo3.write(s3_angle);
          prev_s3_angle = s3_angle;
      }
  }

  if (prev_s4_angle != s4_angle)
  {
      if (abs(prev_s4_angle - s4_angle) > 1)
      {
          servo4.write(s4_angle);
          prev_s4_angle = s4_angle;
      }
  }  
}

// 구동 모드
void Running()
{
  if(pos_cnt >= running_speed) 
  {
    servo1.write(pos_arr[0][pos_step]);
    servo2.write(pos_arr[1][pos_step]);
    servo3.write(pos_arr[2][pos_step]);
    servo4.write(pos_arr[3][pos_step]);
    
    pos_step++;
    
    if(pos_step >= pos_num)
    {
      pos_step = 0;
    }
    
    pos_cnt = -1;
  }
  
  pos_cnt++;
}
