#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
  char c[100];
  Serial.begin(9600);
  for (int i = 0; i < 100; i++){
      EEPROM.write(i,0);
      sprintf(c,"%d:",i);
      Serial.print(c);
      Serial.println(EEPROM.read(i));
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
