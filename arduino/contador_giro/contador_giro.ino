#define REED 21
#define INTERVALO_MINIMO 250

int pos = 0;
long int intervaloInterrupt = 0;

void setup() {
  attachInterrupt(digitalPinToInterrupt(REED), incrementarPosicao, RISING);
  Serial.begin(9600);
}

void loop() {
  Serial.println(pos);
  delay(250);
}

void incrementarPosicao() {
  pos++;
  intervaloInterrupt = millis() - intervaloInterrupt;
  if (pos > 20 || intervaloInterrupt < INTERVALO_MINIMO) {
    pos = 0;
  }
}
