#define REED 21 //input do reed switch
#define INTERVALO_MINIMO 250  //intervalo entre dois pulsos para reset
#define NUM_POS 19  //número de posições do armário
#define PIN_MOTOR 8

int pos = 0;
long int intervaloInterrupt = 0;

void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  digitalWrite(PIN_MOTOR, HIGH);
  attachInterrupt(digitalPinToInterrupt(REED), incrementarPosicao, RISING);
  Serial.begin(9600);
}

void loop() {
  if (pos == 10) {
    digitalWrite(PIN_MOTOR, LOW);
  }
  else {
    digitalWrite(PIN_MOTOR, HIGH);
  }
  delay(250);
}

void incrementarPosicao() {
  pos++;
  pos %= NUM_POS;
  Serial.println(pos);
}
