#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

IPAddress server_addr(192, 168, 15, 6); // IP of the MySQL *server* here
char user[] = "arduino_user"; // MySQL user login username
char password[] = "secret"; // MySQL user login password
char ssid[] = "VIVOFIBRA-6248"; // your SSID
char pass[] = "8DBD1913B2"; // your SSID Password

WiFiClient client;
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

int id;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  //Conexão WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Conexão SQL
  while (!conn.connect(server_addr, 3306, user, password)) {
    delay(100);
  }
}

void loop() {
  yield();
  char comando;
  if (Serial.available()) {
    comando = Serial.read();
    switch (comando) {
      case 's':
        SQLSelect();
        break;
      case 'c':
        SQLUpdateSaida();
        break;
      case 'e':
        SQLUpdateEntrada();
        break;
    }
  }
  delay(250);
}

void SQLSelect() {
  char QUERY[] = "SELECT * FROM db_tcc.organizador_item ORDER BY id DESC LIMIT 1;";
  cursor = new MySQL_Cursor(&conn);
  row_values *row = NULL;
  if (conn.connected()) {
    cursor->execute(QUERY);
    delay(250);
    column_names *cols = cursor->get_columns();
    row = cursor->get_next_row();
    if (row != NULL) {
      id = atol(row->values[0]);
      Serial.println(row->values[1]);
    }
    while(row!=NULL){
        row = cursor->get_next_row();
    }
  }
  
  delete cursor;
}

void SQLUpdateSaida() {
  char QUERY_UPDATE[] = "UPDATE nome_da_tabela SET executado = true WHERE id = %d;";
  char query[128];
  sprintf(query, QUERY_UPDATE, id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query);
  delete cursor;
}

void SQLUpdateEntrada() {
  while (!Serial.available()) {
    yield();
    delay(10);
  }
  char dados[20];
  Serial.readString().toCharArray(dados, 20);
  char posicao[5];
  int i = 0, j = 0;
  while (dados[i] != ' '){
    posicao[j] = dados[i];
    i++;
    j++;
  }
  char rfid[20];
  i++;
  j = 0;
  while (dados[i] != ' '){
    rfid[j] = dados[i];
    i++;
    j++;
  }
  char QUERY_UPDATE[] = "UPDATE nome_da_tabela SET posicao = %s, rfid = %s, executado = true WHERE id = %d;";
  char query[128];
  sprintf(query, QUERY_UPDATE, posicao, rfid, id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query);
  delete cursor;
}
