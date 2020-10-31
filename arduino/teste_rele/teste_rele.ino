#define PINO_RELE 8

void setup() {
  pinMode(PINO_RELE, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(PINO_RELE, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, digitalRead(PINO_RELE));
  delay(900);
  digitalWrite(PINO_RELE, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, digitalRead(PINO_RELE));
  delay(900);
}
