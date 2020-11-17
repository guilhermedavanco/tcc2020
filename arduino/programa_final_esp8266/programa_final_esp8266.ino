#include <ESP8266WiFi.h>  //Biblioteca de conexão ao WiFi do ESP8266
#include <MySQL_Connection.h> //Bibliotecas de conexão ao MySQL
#include <MySQL_Cursor.h>

//Definições para conexão ao banco de dados
IPAddress server_addr(192, 168, 15, 6); // IP of the MySQL *server* here
char user[] = "arduino_user"; // MySQL user login username
char password[] = "secret"; // MySQL user login password

//Definições para conexão à rede WiFi
char ssid[] = "VIVOFIBRA-6248"; // your SSID
char pass[] = "8DBD1913B2"; // your SSID Password

//Criação de variáveis necessárias para conexão à rede e ao banco de dados
WiFiClient client;
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

//Variáveis utilizadas para guardar ids necessários para consultas ao banco de dados
int id,item_id;

//Configuração inicial do ESP8266
void setup() {

  //Inicialização da porta serial
  Serial.begin(115200);

  //Inicialização da conexão WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) { //Garante que o WiFi esteja conectado
    delay(500);
  }

  //Inicialização da conexão ao banco de dados
  while (!conn.connect(server_addr, 3306, user, password)) {
    delay(100);
  }
}

//Execução principal após configuração
void loop() {
  yield();  //Deixa que o ESP8266 realize funções necessárias para manutenção da conexão à rede
  char comando; //Variável usada para guardar o comando recebido do Arduino
  if (Serial.available()) { //Varre a porta serial em busca de um novo comando
    comando = Serial.read();
    switch (comando) {  //Verifica o comando recebido
      case 's': //Caso o comando seja "s", o ESP8266 buscará qual a próxima movimentação pendente para execução e enviará a posição necessária para o Arduino
        SQLSelect();
        break;
      case 'c': //Caso o comando seja "c", o ESP8266 atualizará o banco de dados para conclusão de uma movimentação de saída
        SQLUpdateSaida();
        break;
      case 'e': //Caso o comando seja "e", o ESP8266 atualizará o banco de dados para conclusão de uma movimentação de entrada
        SQLUpdateEntrada();
        break;
    }
  }
  delay(250);
}

//Função de busca da próxima movimentação pendente
void SQLSelect() {
  
  /*Variável que armazena a consulta ao banco de dados necessária para identificação da movimentação pendente.
  Como as informações necessárias estão em duas tabelas diferentes, cuja conexão se dá pelo item que está sendo
  movimentado, é necessário utilizar o comando "INNER JOIN" para uní-las*/
  char QUERY[] = "SELECT fluxo.ID, fluxo.direcao, item.posicao,item.id FROM db_tcc.organizador_item as item"
                 " INNER JOIN db_tcc.organizador_fluxo as fluxo"
                 " ON item.id=fluxo.item_id"
                 " WHERE fluxo.realizado=false"
                 " ORDER BY fluxo.id DESC LIMIT 1;";
  cursor = new MySQL_Cursor(&conn); //Criação de cursor para realização da consulta
  row_values *row = NULL; //Variável que armazena os valores recebidos da consulta
  if (conn.connected()) { //Verifica se a conexão está disponível
    cursor->execute(QUERY); //Executa a query no MySQL
    delay(250);

    //Variáveis para recepção dos dados
    column_names *cols = cursor->get_columns();
    row = cursor->get_next_row();

    if (row != NULL) {  //Lê os dados de cada linha encontrada
      
      //Armazena o id e o id do item em variáveis globais
      id = atol(row->values[0]);
      item_id = atol(row->values[3]);

      //Envia a posição da peça para o Arduino. No caso de uma entrada de peça, esse valor será "-1"
      Serial.println(row->values[2]);
    }

    //Limpa as linhas remanescentes
    while (row != NULL) {
      row = cursor->get_next_row();
    }
  }
  delete cursor;  //Limpa o cursor criado para liberar memória
}

/*Função de atualização do bando de dados para uma movimentação de saída de peça.
Ao receber um sinal de conclusão vindo do Arduino, será feita a atualização das tabelas
"Fluxo" e "Itens" do banco de dados de forma a marcar o fluxo como concluído e a
peça correspondente como indisponível no banco de dados*/
void SQLUpdateSaida() {
  
  //Criação e execução de query de atualização do fluxo.
  char QUERY_UPDATE[] = "UPDATE db_tcc.organizador_fluxo SET realizado = true WHERE id = %d;";  //%d é substituido pelo id da peça identificado na função SELECT
  char query[128];
  sprintf(query, QUERY_UPDATE, id); //Criação da string com a query a ser realizada
  cursor = new MySQL_Cursor(&conn); //criação de cursor para execução da query
  cursor->execute(query); //Execução da query
  delete cursor;  //Limpa o cursor criado para liberar memória
  delay(250);

  //Criação e execução de query de atualização de item.
  char QUERY_UPDATE2[] = "UPDATE db_tcc.organizador_item SET posicao = -1, disponibilidade=false WHERE id = %d;";
  char query2[128];
  sprintf(query2, QUERY_UPDATE2, item_id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query2);
  delete cursor;
}

/*Função de atualização do banco de dados para uma movimentação de entrada de peça.
Ao receber um sinal vindo do Arduino, o ESP8266 lerá a entrada serial buscando a informação
da posição final da peça e UID da tag da peça. O processo dessa função é mais complexo:
é necessário primeiro buscar o UID no banco de dados de forma a identificar a peça,
para então atualizar o banco de dados com as informações corretas*/
void SQLUpdateEntrada() {

  //Aguarda identificação de mensagem na serial
  while (!Serial.available()) {
    yield();
    delay(10);
  }

  //Criação das variáveis utilizadas
  char dados[30];
  char c;
  int x = 0;

  //Começo do processo de leitura da serial.
  //Lê todos os caracteres presentes na porta serial, até o fim da mensagem
  do {
    c = Serial.read();
    dados[x] = c;
    x++;
    delay(1);
  } while (x<30 && Serial.available());

  dados[x] = '\0';  //Marca o fim da mensagem
  
  char posicao[5];  //Armazena a posição da peça
  int i = 0, j = 0,posicao_int;
  while (dados[i] != ' ') { //Busca pelo caractere separador de dados, nesse caso o caractere "espaço", para identificar onde termina a informação da posição e começa a informação do UID
    posicao[j] = dados[i];
    i++;
    j++;
  }
  posicao_int=atol(posicao);  //Converte a posição de string para int
  
  char rfid[20];  //Armazena o UID da tag
  i++;
  j = 0;
  while (dados[i] != '\0') {  //Armazena o resto da string na variável rfid
    rfid[j] = dados[i];
    i++;
    j++;
  }
  rfid[j] = '\0'; //Marca o fim da mensagem
  //Final do processo de leitura da serial

  //Identificação do item a partir da tag RFID. Funciona de forma semelhante à função SQLSelect()
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
      item_id = atol(row->values[0]); //Salva o id da peça correta na variável global item_id
    }
    while (row != NULL) {
      row = cursor->get_next_row();
    }
  }

  //Com a peça já identificada, é feito o update das tabelas fluxo e item com as informações recebidas de posição
  char QUERY_UPDATE[] = "UPDATE db_tcc.organizador_fluxo SET item_id = %d, realizado = true WHERE id = %d;";  //Define qual peça foi movimentada e marca o fluxo como realizado
  char query[128];
  sprintf(query, QUERY_UPDATE, item_id, id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query);
  delete cursor;
  
  delay(250);
  
  char QUERY_UPDATE2[] = "UPDATE db_tcc.organizador_item SET posicao = %d,disponibilidade=true WHERE id = %d;"; //Marca a peça recebida como disponível
  char query2[128];
  sprintf(query2, QUERY_UPDATE2,posicao_int, item_id);
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(query2);
  delete cursor;
}
