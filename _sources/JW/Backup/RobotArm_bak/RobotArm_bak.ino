#include <EEPROM.h>
#include <Servo.h> 

const int Pot1 = A0;
const int Pot2 = A1;
const int Pot3 = A2;
const int Pot4 = A3;

const int SPin1 = 11;
const int SPin2 = 10;
const int SPin3 = 9;
const int SPin4 = 6;

const int Switch_Record = 4;
const int Switch_Run = 3;

int state = 0;  // 0 : Record   / 1 : Run
int prev_p1 = 0, prev_p2 = 0, prev_p3 = 0, prev_p4 = 0;

int prev_record = 0;
int prev_run = 0;

int pos_arr[4][50]; // 위치 저장 배열
int pos_num = 0;    // 저장 수
int pos_cnt = 0;    // 딜레이를 
int pos_step = 0;

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
}

void loop()
{
    int p1 = map(analogRead(Pot1), 1, 1023, 5, 160);
    int p2 = map(analogRead(Pot2), 1, 1023, 5, 160);
    int p3 = map(analogRead(Pot3), 1, 1023, 5, 160);
    int p4 = map(analogRead(Pot4), 1, 1023, 5, 160);
    
    //        char tbf[200];
    //        sprintf(tbf, "pot1 = %10d  / pot2 = %10d   / pot3 = %10d   / pot4 = %10d ", p1, p2, p3, p4);
    //        Serial.println(tbf);     
    
    int pres_record = digitalRead(Switch_Record);
    int pres_run = digitalRead(Switch_Run);

    // 기록버튼
    if(prev_record == 0 && pres_record == 1)
    {
      if(state == 0 && pos_num < 20)
      {
        pos_arr[0][pos_num] = p1;
        pos_arr[1][pos_num] = p2;
        pos_arr[2][pos_num] = p3;
        pos_arr[3][pos_num] = p4;
        pos_num++;
      }
    }

    // 시작버튼
    if(prev_run == 0 && pres_run == 1)
    {
      if(state == 1)
      {
        state = 0;
        pos_num = 0;
        pos_step = 0;
        pos_cnt = 0;
      }
      
      else
      {
        // 저장된게 없으면 실행하지않음
        if(pos_num > 0)
        {
          state = 1;
          pos_step = 0;
          pos_cnt = 0;
        }
      }
    }

    // 기록설정중
    if (state == 0)
    {
        if (prev_p1 != p1)
        {
            if (abs(prev_p1 - p1) > 1)
            {
                servo1.write(p1);
                prev_p1 = p1;
            }
        }

        if (prev_p2 != p2)
        {
            if (abs(prev_p2 - p2) > 1)
            {
                servo2.write(p2);
                prev_p2 = p2;
            }
        }

        if (prev_p3 != p3)
        {
            if (abs(prev_p3 - p3) > 1)
            {
                servo3.write(p3);
                prev_p3 = p3;
            }
        }

        if (prev_p4 != p4)
        {
            if (abs(prev_p4 - p4) > 1)
            {
                servo4.write(p4);
                prev_p4 = p4;
            }
        }
    }

    // 구동중
     else if (state == 1)
    {
        if(pos_cnt >= 100)
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
    
    prev_record = pres_record;
    prev_run = pres_run;
        
    delay(20);
}
