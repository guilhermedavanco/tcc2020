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

int id,item_id;

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
  char QUERY[] = "SELECT fluxo.ID, fluxo.direcao, item.posicao,item.id FROM db_tcc.organizador_item as item"
                 " INNER JOIN db_tcc.organizador_fluxo as fluxo"
                 " ON item.id=fluxo.item_id"
                 " WHERE fluxo.realizado=false"
                 " ORDER BY fluxo.id DESC LIMIT 1;";
  cursor = new MySQL_Cursor(&conn);
  row_values *row = NULL;
  if (conn.connected()) {
    cursor->execute(QUERY);
    delay(250);
    column_names *cols = cursor->get_columns();
    row = cursor->get_next_row();
    if (row != NULL) {
      id = atol(row->values[0]);
      item_id = atol(row->values[3]);
      Serial.println(row->values[2]);
    }
    while (row != NULL) {
      row = cursor->get_next_row();
    }
  }

  delete cursor;
}

void SQLUpdateSaida() {
  char QUERY_UPDATE[] = "UPDATE db_tcc.organizador_fluxo SET realizado = true WHERE id = %d;";
  char query[128];
  sprintf(query, QUERY_UPDATE, id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query);
  delete cursor;
  delay(250);
  char QUERY_UPDATE2[] = "UPDATE db_tcc.organizador_item SET posicao = -1,disponibilidade=false WHERE id = %d;";
  char query2[128];
  sprintf(query2, QUERY_UPDATE2, item_id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query2);
  delete cursor;
}

void SQLUpdateEntrada() {
  while (!Serial.available()) {
    yield();
    delay(10);
  }
  char dados[30];
  char c;
  int x = 0;
  do {
    c = Serial.read();
    dados[x] = c;
    x++;
    delay(1);
  } while (x<30 && Serial.available());
  dados[x] = '\0';
  char posicao[5];
  int i = 0, j = 0,posicao_int;
  while (dados[i] != ' ') {
    posicao[j] = dados[i];
    i++;
    j++;
  }
  posicao_int=atol(posicao);
  char rfid[20];
  i++;
  j = 0;
  while (dados[i] != '\0') {
    rfid[j] = dados[i];
    i++;
    j++;
  }
  rfid[j] = '\0';
  char QUERY3[] = "SELECT id FROM db_tcc.organizador_item WHERE rfid = '%s';";
  char query3[128];
  sprintf(query3, QUERY3, rfid);
  cursor = new MySQL_Cursor(&conn);
  row_values *row = NULL;
  if (conn.connected()) {
    cursor->execute(query3);
    delay(250);
    column_names *cols = cursor->get_columns();
    row = cursor->get_next_row();
    if (row != NULL) {
      item_id = atol(row->values[0]);
    }
    while (row != NULL) {
      row = cursor->get_next_row();
    }
  }
  char QUERY_UPDATE[] = "UPDATE db_tcc.organizador_fluxo SET item_id = %d, realizado = true WHERE id = %d;";
  char query[128];
  sprintf(query, QUERY_UPDATE, item_id, id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query);
  delete cursor;
  
  delay(250);
  
  char QUERY_UPDATE2[] = "UPDATE db_tcc.organizador_item SET posicao = %d,disponibilidade=true WHERE id = %d;";
  char query2[128];
  sprintf(query2, QUERY_UPDATE2,posicao_int, item_id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query2);
  delete cursor;
}
