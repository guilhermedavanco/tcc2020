#define REED 20//input do reed switch
#define INTERVALO_MINIMO 250  //intervalo entre dois pulsos para reset
#define NUM_POS 19  //número de posições do armário
#define PIN_MOTOR 8

int pos = 0;
unsigned long ultimoInterrupt = 0;
int posDesejada = 0;

void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  digitalWrite(PIN_MOTOR, LOW);
  attachInterrupt(digitalPinToInterrupt(REED), incrementarPosicao, RISING);
  Serial.begin(9600);
}

void loop() {
  char c;
  char dados[8];
  int x = 0;
  if (Serial.available()) {
    do {
      c = Serial1.read();
      dados[x] = c;
      x++;
      delay(1);
    } while (c != '\n');
    dados[x - 1] = '\0';
    Serial.println(dados);
    posDesejada = atoi(dados);
  }
  if (pos == posDesejada) {
    digitalWrite(PIN_MOTOR, LOW);
  }
  else {
    digitalWrite(PIN_MOTOR, HIGH);
  }
  delay(250);
}

void incrementarPosicao() {
  if (millis() - ultimoInterrupt  > 250) {
    pos++;
    pos = pos % NUM_POS;
    Serial.println(pos);
    ultimoInterrupt = millis();
  }
}
