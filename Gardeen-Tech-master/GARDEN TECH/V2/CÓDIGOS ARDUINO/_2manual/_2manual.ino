#include <dht.h>
#include <math.h>
#define dht_dpin A1 //Pino DATA do Sensor ligado na porta Analogica A1

dht DHT; //Inicializa o sensor

//ATUADORES
int ledAzul = 2;
int ledVermelho = 5;
int cooler = 3;
int regador = 4;

//SENSORES DIGITAIS
int nivelB = 6;
int nivelA = 7;

int ledInfoVermelho = 8;
int ledInfoVerde = 9;
int ledInfoAmarelo = 10;

int brilhoLuzLed = 200;

//SENSORES ANALOGICOS
#define P1 A0
#define ldr A2

String estado = "0";

void setup() {
 pinMode(ledAzul,OUTPUT);
 pinMode(ledVermelho,OUTPUT);
 pinMode(cooler,OUTPUT);
 pinMode(regador,OUTPUT);
 //Sensores
 pinMode(P1,INPUT);
 pinMode(ldr,INPUT);
 pinMode(nivelB,INPUT);
 pinMode(nivelA,INPUT);
 //Leds de controle
 pinMode(ledInfoVermelho,OUTPUT);
 pinMode(ledInfoVerde,OUTPUT);
 pinMode(ledInfoAmarelo,OUTPUT);

 //definindo velocidade de comunicaçao
 Serial.begin(9600);
}

void loop(){
  int permission = 0;
  
  //passando os valores das entradas analogicas para as variaveis 
  DHT.read11(dht_dpin); //Lê as informações do sensor
  double umidadeAr = (DHT.humidity);
  int temp = (DHT.temperature);
  
  int iluminacao;
  if(analogRead(ldr) > brilhoLuzLed){
     iluminacao = 0;
  }else{
     iluminacao = 1;
  }
  double lux = calculaLux();
  int umidadeV1 = calculaUmidade(analogRead(P1));
  int nivel = calculaNivel();
  
  //controle do sistema para evitar falha
  permission = seguranca(nivel);
  
  //controle pelo pc
  controlePhp(temp,iluminacao, umidadeV1, nivel, umidadeAr, lux);
  
}//fim da plantaçao

double calculaLux(){
  double voltage = 2.0625 * (analogRead(ldr) / 2048.0);
  double resistance = (33.0) / voltage - 10.0;
  double illuminance = 255.84 * pow(resistance, -10/9);
  return illuminance;
}

int calculaUmidade(int valor){
  return (valor/3); 
}

int calculaNivel(){
  if(analogRead(A6) > 10){
    return 1;
  }
  else if(analogRead(A7) > 10 ){
    return 2;
  }else{
    return 3;
  }
}

int seguranca(int nivel){
  //reservatorio baixo
  if(nivel == 3){
    estado = "5";
    alerta();
    return 0;
  }
  if(nivel == 2){
    estado = "6";
    atencao();
    return 1;
  }
  else{
    funcionamentoCorreto();
    return 1;
  }
}

//avisa o usuario pela interface de leds
void alerta(){
  digitalWrite(ledInfoVermelho,HIGH);
  digitalWrite(ledInfoVerde,LOW);
  digitalWrite(ledInfoAmarelo,LOW);
}

void funcionamentoCorreto(){
  digitalWrite(ledInfoVerde,HIGH);
  digitalWrite(ledInfoVermelho,LOW);
  digitalWrite(ledInfoAmarelo,LOW);
}

void atencao(){
  digitalWrite(ledInfoAmarelo,HIGH);
  digitalWrite(ledInfoVermelho,LOW);
  digitalWrite(ledInfoVerde,LOW);
}

void controlePhp(int temp, int iluminacao, int umidadeV1, int nivel, int umidadeAr, int lux){
  if (Serial.available() >= 0) {
      char lSerial = Serial.read();
      
      //Controle pelo pc e php
      //Ler os estados e passar para o PHP
      if(lSerial == 'R'){
        Serial.print("Iluminaçao: ");
        Serial.println(iluminacao);
        Serial.print("Lux: ");
        Serial.println(lux);
        Serial.print("Temperatura: ");
        Serial.println(temp);
        Serial.print("UmidadeV1: ");
        Serial.println(umidadeV1);
        Serial.print("UmidadeAr: ");
        Serial.println(umidadeAr);
        Serial.print("Regador: ");
        Serial.println(digitalRead(regador));
        Serial.print("Cooler: ");
        Serial.println(digitalRead(cooler));
        Serial.print("Nivel: ");
        Serial.println(nivel);
        Serial.print("Estado: ");
        Serial.println(estado);
      }
      
      //fim de plantaçao
      else if(lSerial == 'F'){
       digitalWrite(ledAzul,LOW);
       digitalWrite(cooler,LOW);
       digitalWrite(regador,LOW);
       digitalWrite(ledVermelho,LOW);
      }
      
      //controle do led
      else if(lSerial == 'A'){
        digitalWrite(ledAzul,HIGH);
      }
      else if(lSerial == 'a'){
        digitalWrite(ledAzul,LOW);
      }
      
      //Controler do cooler 
      else if(lSerial == 'B'){
        digitalWrite(cooler,HIGH);
      }
      else if(lSerial == 'b'){
        digitalWrite(cooler,LOW);
      }
      
      //controle do regador do vaso1 
      else if(lSerial == 'C'){
        digitalWrite(regador,HIGH);
      }
      else if(lSerial == 'c'){
        digitalWrite(regador,LOW);
      }
      
      //controle do regador do vaso2
      else if(lSerial == 'D'){
        digitalWrite(ledVermelho,HIGH);
      }
      else if(lSerial == 'd'){
        digitalWrite(ledVermelho,LOW);
      }//fim do controle em modo manual
    } 
}
