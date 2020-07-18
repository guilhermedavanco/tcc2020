#include <EEPROM.h> //os estados das posições são salvos na EEPROM para não perder em um ciclo de energia

#define REED 21 //input do reed switch
#define INTERVALO_MINIMO 250  //intervalo entre dois pulsos para reset
#define NUM_POS 20  //número de posições do armário
#define PIN_MOTOR 13  //pino do motor principal
#define VEL_MOTOR 127 //velocidade do motor, 0-255

int pos = 0;
long int intervaloInterrupt = 0;

void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(REED), incrementarPosicao, RISING);
  Serial.begin(115200);
  delay(10000);
  Serial.readString();
}

void loop() {
  char dados[20];
  int posicaoDesejada;
  Serial.println('s');
  delay(1000);
  if (Serial.available()) {
    Serial.readString().toCharArray(dados, 20);
    posicaoDesejada = atoi(dados);
    if (posicaoDesejada == -1) {
      receberPeca();
    }
    else {
      enviarPeca(posicaoDesejada);
    }
  }
}

void incrementarPosicao() {
  pos++;
  intervaloInterrupt = millis() - intervaloInterrupt;
  if (pos > NUM_POS || intervaloInterrupt < INTERVALO_MINIMO) {
    pos = 0;
  }
}

void moverEstrutura(int posicao) {
  while (pos != posicao) {
    analogWrite(PIN_MOTOR, VEL_MOTOR);
  }
  analogWrite(PIN_MOTOR, 0);
}

void receberPeca() {
  int posicaoVazia = pos;
  while (EEPROM.read(posicaoVazia)) {
    posicaoVazia = (posicaoVazia + 1) % NUM_POS;  //se ultrapassar NUM_POS, volta pra 0
  }
  moverEstrutura(posicaoVazia);
  EEPROM.write(pos, 1);
  char rfid[20];
  //ler rfid
  recebimentoBraco();
  char dados[20];
  sprintf(dados, "%i %s", pos, rfid);
  Serial.println('e');
  delay(50);
  Serial.println(dados);
}

void enviarPeca(int posicao) {
  moverEstrutura(posicao);
  entregaBraco();
  EEPROM.write(pos, 0);
  Serial.println('c');
}

void recebimentoBraco() {
  //?
}

void entregaBraco() {
  //?
}