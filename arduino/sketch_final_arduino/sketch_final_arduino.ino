#include <EEPROM.h> //os estados das posições são salvos na EEPROM para não perder em um ciclo de energia
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

#define REED 21 //input do reed switch
#define INTERVALO_MINIMO 250  //intervalo entre dois pulsos para reset
#define NUM_POS 20  //número de posições do armário
#define PIN_MOTOR 13  //pino do motor principal
#define VEL_MOTOR 127 //velocidade do motor, 0-255

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
int pos = 0;
long int intervaloInterrupt = 0;

void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(REED), incrementarPosicao, RISING);
  Serial.begin(115200);
  Serial1.begin(115200);

  //Inicialização do leitor RFID
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
  }
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  //nfc.setPassiveActivationRetries(0xFF);
  // configure board to read RFID tags
  nfc.SAMConfig();

  delay(10000);
  if (Serial1.available())
    Serial1.readString();
}

void loop() {
  char dados[30];
  char c;
  int x = 0;
  int posicaoDesejada;
  Serial1.println('s');
  Serial.println('s');
  delay(1000);
  if (Serial1.available()) {
    do {
      c = Serial1.read();
      dados[x] = c;
      x++;
      delay(1);
    } while (c != '\n');
    dados[x - 1] = '\0';
    Serial.println(dados);
    //posicaoDesejada = atoi(dados);
    //Serial.println(posicaoDesejada);
    //    if (posicaoDesejada == -1) {
    //      receberPeca();
    //    }
    //    else {
    //      enviarPeca(posicaoDesejada);
    //    }
  }
}

void incrementarPosicao() {
  pos++;
  intervaloInterrupt = millis() - intervaloInterrupt;
  if (pos > NUM_POS || intervaloInterrupt < INTERVALO_MINIMO) {
    pos = 0;
  }
  Serial.println(pos);
}

void moverEstrutura(int posicao) {
  while (pos != posicao) {
    analogWrite(PIN_MOTOR, VEL_MOTOR);
  }
  analogWrite(PIN_MOTOR, 0);
}

void receberPeca() {
  Serial.println("Receber peça");
  int posicaoVazia = pos;
  while (EEPROM.read(posicaoVazia)) {
    posicaoVazia = (posicaoVazia + 1) % NUM_POS;  //se ultrapassar NUM_POS, volta pra 0
  }
  moverEstrutura(posicaoVazia);
  EEPROM.write(pos, 1);

  //Leitura RFID
  char rfid[20] = "FF FF FF FF";
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("UID Value: ");
    sprintf(rfid, "");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      Serial.print(" 0x");
      sprintf(rfid, "%s %d", rfid, uid[i]);
      Serial.print(uid[i], HEX);
    }
    Serial.println("");
    // Wait 1 second before continuing
    delay(1000);
  }
  //Fim da leitura do RFID

  recebimentoBraco();
  char dados[30];
  sprintf(dados, "%i %s", pos, rfid);
  Serial1.println('e');
  delay(100);
  Serial1.println(dados);
}

void enviarPeca(int posicao) {
  Serial.println("Enviar peça");
  moverEstrutura(posicao);
  entregaBraco();
  EEPROM.write(pos, 0);
  Serial1.println('c');
}

void recebimentoBraco() {
  //?
}

void entregaBraco() {
  //?
}
