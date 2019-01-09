#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  char buf[100];
  sprintf(buf,"num = %d",EEPROM.read(0));
  Serial.println(buf);
  
  for(int i = 0 ; i < 50; i++)
  {
    sprintf(buf,"%4d번째 || servo1 = %3d / servo2 = %3d / servo3 = %3d / servo4 = %3d",i+1, EEPROM.read(i*4+1), EEPROM.read(i*4+2), EEPROM.read(i*4+3), EEPROM.read(i*4+4));
    Serial.println(buf);
  }
}

void loop() {
  delay(10000);
}
