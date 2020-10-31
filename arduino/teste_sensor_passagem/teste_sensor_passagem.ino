#define PINO_SENSOR 9

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PINO_SENSOR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(PINO_SENSOR));
  delay(100);
}
