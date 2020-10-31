#define REED 2//input do reed switch
#define NUM_POS 7  //número de posições do armário
#define PIN_MOTOR 8

int pos = 0;
unsigned long ultimoInterrupt = 0;
int posDesejada = 0;
int posAnterior = 0;

void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(REED, INPUT);
  digitalWrite(PIN_MOTOR, LOW);
  attachInterrupt(digitalPinToInterrupt(REED), incrementarPosicao, RISING);
  Serial.begin(9600);
}

void loop() {
  char c;
  char dados[8];
  int x;
  if (Serial.available()) {
    x = 0;
    do {
      c = Serial.read();
      dados[x] = c;
      x++;
      delay(1);
    } while (c >= 48 && c <= 57);
    Serial.println();
    dados[x - 1] = '\0';
    posDesejada = atoi(dados);
    Serial.print("Indo para posicao ");
    Serial.println(dados);
  }
  if (pos == posDesejada) {
    digitalWrite(PIN_MOTOR, LOW);
  }
//  else if (pos != posAnterior) {
//    digitalWrite(PIN_MOTOR, LOW);
//    delay(500);
//  }
  else {
    digitalWrite(PIN_MOTOR, HIGH);
  }
  posAnterior = pos;
  delay(10);
  // Serial.println(digitalRead(REED));
}

void incrementarPosicao() {
  if (millis() - ultimoInterrupt  > 50) {
    if (pos != posDesejada) {
      pos++;
      pos = pos % NUM_POS;
      Serial.println(pos);
      ultimoInterrupt = millis();
    }
  }
}
