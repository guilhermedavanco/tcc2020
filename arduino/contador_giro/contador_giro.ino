#define REED 21 //input do reed switch
#define INTERVALO_MINIMO 250  //intervalo entre dois pulsos para reset
#define NUM_POS 20  //número de posições do armário

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
  if (pos > NUM_POS || intervaloInterrupt < INTERVALO_MINIMO) {
    pos = 0;
  }
}
