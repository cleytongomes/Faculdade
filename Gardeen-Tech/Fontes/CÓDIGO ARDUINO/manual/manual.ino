#include <Thermistor.h>

//ATUADORES
int led = 2;
int cooler = 3;
int regadorA = 4;
int regadorB = 5;

//SENSORES DIGITAIS

int ledVermelho = 8;
int ledVerde = 9;

int brilhoLuzLed = 200;

//SENSORES ANALOGICOS
#define P1 A0
#define P2 A1
#define ldr A2
#define nivelB A4
#define nivelA A5

Thermistor temperatura(A3);

String estado;

void setup() {
 pinMode(led,OUTPUT);
 pinMode(cooler,OUTPUT);
 pinMode(regadorA,OUTPUT);
 pinMode(regadorB,OUTPUT);
 
 pinMode(P1,INPUT);
 pinMode(P2,INPUT);
 pinMode(ldr,INPUT);
 pinMode(nivelB,INPUT);
 pinMode(nivelA,INPUT);
 
 pinMode(ledVermelho,OUTPUT);
 pinMode(ledVerde,OUTPUT);
 
 Serial.begin(9600);
}

void loop(){
  int permission = 0;
  
  //passando os valores das entradas analogicas para as variaveis 
  double temp = temperatura.getTemp();
  int iluminacao;
  if(analogRead(ldr) > brilhoLuzLed){
     iluminacao = 0;
  }else{
     iluminacao = 1;
  }
  int umidadeV1 = calculaUmidade(analogRead(P1));
  int umidadeV2 = calculaUmidade(analogRead(P2));
  int nivel = calculaNivel();
  
  
  
  //controle do sistema para evitar falha
  permission = seguranca(nivel);
  
  //controle pelo pc
  controlePhp(temp,iluminacao, umidadeV1, umidadeV2, nivel);
  
}//fim da plantaçao

int calculaUmidade(int valor){
  return (valor/10); 
}

int calculaNivel(){
  if(analogRead(nivelA) > 10){
    return 1;
  }
  else if(analogRead(nivelB) > 10){
    return 2;
  }else{
    return 3;
  }
}

int seguranca(int nivel){
  //reservatorio baixo
  if(nivel == 3){
    estado = "reservatrio em nivel critico";
    alerta();
    return 0;
  }
  else{
    funcionamentoCorreto();
    return 1;
  }
}

//avisa o usuario pela interface de leds
void alerta(){
  digitalWrite(ledVermelho,HIGH);
  digitalWrite(ledVerde,LOW);
}

void funcionamentoCorreto(){
  digitalWrite(ledVerde,HIGH);
  digitalWrite(ledVermelho,LOW);
}

void controlePhp(int temp, int iluminacao, int umidadeV1, int umidadeV2, int nivel){
  if (Serial.available() >= 0) {
      char lSerial = Serial.read();
      
      //Controle pelo pc e php
      //Ler os estados e passar para o PHP
      if(lSerial == 'R'){
        Serial.println(iluminacao);
        Serial.println(temp);
        Serial.println(umidadeV1);
        Serial.println(umidadeV2);
        Serial.println(digitalRead(regadorA));
        Serial.println(digitalRead(regadorB));
        Serial.println(digitalRead(cooler));
        Serial.println(nivel);
        Serial.println(estado);
      }
      
      //fim de plantaçao
      else if(lSerial == 'F'){
       digitalWrite(led,LOW);
       digitalWrite(cooler,LOW);
       digitalWrite(regadorA,LOW);
       digitalWrite(regadorB,LOW);
      }
      
      //controle do led
      else if(lSerial == 'A'){
        digitalWrite(led,HIGH);
      }
      else if(lSerial == 'a'){
        digitalWrite(led,LOW);
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
        digitalWrite(regadorA,HIGH);
      }
      else if(lSerial == 'c'){
        digitalWrite(regadorA,LOW);
      }
      
      //controle do regador do vaso2
      else if(lSerial == 'D'){
        digitalWrite(regadorB,HIGH);
      }
      else if(lSerial == 'd'){
        digitalWrite(regadorB,LOW);
      }//fim do controle em modo manual
    } 
}
