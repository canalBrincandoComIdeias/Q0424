/*
      Q0424
      AUTOR:   BrincandoComIdeias
      LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
      COMPRE:  https://www.arducore.com.br/
      SKETCH:  EXEMPLO PCB PARA INICIANTES
      APP:     https://youtu.be/8Ke7u3otWms
      DATA:    28/01/2019
*/
// INCLUSÃO DE BIBLIOTECA
#include <SoftwareSerial.h>

// DEFINIÇÃO DE PINO
#define pinSH_CP 9   //Pino Clock
#define pinST_CP 10  //Pino Latch
#define pinDS    11  //Pino Data
#define qtdeCI   1   //Quantidade de placas em série

#define DEBUG // COMENTAR PARA IGNORAR PRINT P/ DEBUG
//#define TESTE // COMENTAR PARA IGNORAR A FUNÇÃO TESTE

// INSTANCIANDO OBJETO
SoftwareSerial bluetooth(6,7); // RX (TX DO HC-06) , TX (RX DO HC-06)

// DECLARAÇÃO DE FUNÇÕES
void gravarEstados();

// DECLARAÇÃO DE VARIÁVEIS
const int MAX = qtdeCI * 8;
bool gravar;
bool estadoRele[MAX];
char comandoBT;
int  numeroRele;

unsigned long testeDelay;
byte testeIndex = 0;

String comandoRecebido;

void setup() {

  // INICIANDO COMUNICAÇÕES SERIAL
  Serial.begin(9600);
  bluetooth.begin(9600);

  // DEFINIÇÃO DE MODO DO PINO
  pinMode(pinSH_CP, OUTPUT);
  pinMode(pinST_CP, OUTPUT);
  pinMode(pinDS   , OUTPUT);

  // ESCREVE ESTADO DO LATCH E CLOCK PARA EVITAR GRAVAÇÃO
  digitalWrite(pinST_CP,HIGH);
  digitalWrite(pinSH_CP,LOW);

  // ESCREVE O VALOR LOW PARA TODOS OS RELES
  for (byte i = 0; i < MAX; i++) {
    estadoRele[i] = LOW;
  }
  
  gravar = true;
    
  #ifdef DEBUG
    Serial.println("FIM SETUP");
  #endif
}

void loop() {
  #ifdef DEBUG
    //Serial.println("INICIO LOOP");
  #endif

  if( gravar == true){
    gravarEstados();
  }

  if(bluetooth.available()){
    comandoBT = bluetooth.read();
    comandoRecebido += comandoBT;
   
    if(comandoBT == ';'){
      numeroRele = int(comandoRecebido.charAt(1)) - 48;
      if (numeroRele == 0) numeroRele = 10;
      numeroRele--;
      
      #ifdef DEBUG
        Serial.println(numeroRele);
      #endif

      if ( numeroRele < MAX ) {
         estadoRele[numeroRele] = (comandoRecebido.substring(2) == "true;");
         gravar = true;
      }
      

      comandoRecebido = "";
    }
  }
  
  #ifdef TESTE
    if(millis() - testeDelay > 400){
      estadoRele[testeIndex] = !estadoRele[testeIndex];
      gravar = true;
      testeIndex++; 
      testeDelay=millis();
      if(testeIndex >= 8) testeIndex = 0;
    }
  #endif

}

void gravarEstados(){
  // PREPARA PARA INICIAR A TRANSMISSÃO
  digitalWrite(pinST_CP,LOW);
  digitalWrite(pinSH_CP, LOW);
  digitalWrite(pinDS, LOW);  

  // LAÇO PARA ENVIO DE DADOS
  for(byte i=0 ; i < MAX; i++){
    digitalWrite(pinSH_CP,LOW);
    digitalWrite(pinDS,estadoRele[i]);
    digitalWrite(pinSH_CP,HIGH);
    digitalWrite(pinDS, LOW);
  }
  gravar = false;
  digitalWrite(pinST_CP, HIGH);
  #ifdef DEBUG
    //Serial.println("Gravado");
  #endif
}
