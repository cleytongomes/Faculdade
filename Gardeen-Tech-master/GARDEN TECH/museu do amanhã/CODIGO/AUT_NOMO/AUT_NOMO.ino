#include <EEPROM.h>
#include <Thermistor.h>

//ATUADORES
int ledAzul = 2;
int ledVermelho = 5;
int regador = 4;

//SENSORES ANALOGICOS
#define P1 A0
#define ldr A2
#define nivelB A4
#define nivelA A5
Thermistor temperatura(A3);

//INFORMATIVOS
int ledInfoVermelho = 8;
int ledInfoVerde = 9;
int ledInfoAmarelo = 10;

//dados inerentes
int tempMax = 35;
int tempMin = 10;
int umidadeMaxDoSensor = 700;
int umidadeMax = 80;
int umidadeMin = 60;
int brilhoLuzLed = 800;
int autorizacaoParaRegar = 0;
int estadoPlantas = 1; 

void setup() {
 pinMode(ledAzul,OUTPUT);
 pinMode(ledVermelho,OUTPUT);
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
  
  //passando os valores das entradas analogicas para as variaveis 
  int temp = temperatura.getTemp();
  int fonteLuz = calculaFonte(analogRead(ldr));
  int iluminacao = calculaIluminacao(analogRead(ldr));
  int umidadeV1 = calculaUmidade(analogRead(P1));
  int nivel = calculaNivel(analogRead(nivelA), analogRead(nivelB));
  
  //segurança para evitar queimar a bomba
  autorizacaoParaRegar = seguranca(nivel, temp, umidadeV1);
  
  //controle autonomo
  autonomo(autorizacaoParaRegar, temp, iluminacao, fonteLuz, umidadeV1, nivel); 
  
  //controle pelo pc
  controlePhp(temp ,iluminacao, fonteLuz, umidadeV1, nivel);
  
}//fim da plantaçao

void autonomo(int autorizacaoParaRegar, int temp, int iluminacao, int fonteLuz, int umidadeV1, int nivel){
  
  //irrigaçao autonoma
  if(autorizacaoParaRegar == 1){
    //umidade baixa planta1
    if(umidadeV1 < umidadeMin)
      digitalWrite(regador,HIGH);
    else{ 
       if(umidadeV1 >= umidadeMax) 
        digitalWrite(regador,LOW);
    }
  }else{
    digitalWrite(regador,LOW);
  }//fim de irrigaçao e do bloco de permissao
  
  //iluminaçao artificial
  if(fonteLuz == 0)
     digitalWrite(ledAzul,LOW);
  else{
     digitalWrite(ledAzul,HIGH);
  }
}

void controlePhp(int temp, int iluminacao, int fonteLuz, int umidadeV1, int nivel){
  if (Serial.available() >= 0) {
      char lSerial = Serial.read();
      
      //Controle pelo pc e php
      //Ler os estados e passar para o PHP
      if(lSerial == 'R'){
        Serial.println(fonteLuz);
        Serial.println(iluminacao);
        Serial.println(temp);
        Serial.println(umidadeV1);
        Serial.println(digitalRead(regador));
        Serial.println(nivel);
        Serial.println(estadoPlantas);
      }
  }
}


int calculaNivel(int alto, int baixo){
  if(alto > 10){
    return 1; //Nivel Alto
  }
  else if(baixo > 10){
    return 2; //Nivel Medio
  }else{
    return 3; //Nivel Crtico
  }
}

int calculaUmidade(int valor){
  return ((valor * 100)/umidadeMaxDoSensor); 
}

int calculaFonte(int quantidadeDeLuz){
  int guarda;
  if(quantidadeDeLuz > brilhoLuzLed){
    guarda = 0; //luz do sol
  }else{
    guarda = 1; //luz solar
  }
  return guarda;
}

int calculaIluminacao(int quantidadeDeLuz){
  //porcentagem de luz no ambiente em relaço ao potencial do led
  int guarda = ((100*quantidadeDeLuz)/brilhoLuzLed);
  return guarda;
}

int seguranca(int nivel, int temp, int umidadeV1){
  //reservatorio critico
  if(nivel == 3){
    estadoPlantas = 3;
    alerta();
    return 0;
  }
  //umidade baixa
  else if(umidadeV1 < (umidadeMin - 5)){
    estadoPlantas = 4;
    alerta();
    return 1;
  }
  
  //umidade alta
  else if(umidadeV1 > (umidadeMax + 6)){
    estadoPlantas = 6;
    alerta();
    return 1;
  }
  
  //temperatura alta
  else if(temp > tempMax){
    estadoPlantas = 9;
    alerta();
    return 1;
  }
  //temperatura baixa
  else if(temp < tempMin){
    estadoPlantas = 11;
    alerta();
    return 1;
  }
  
  else if(analogRead(ldr)>100){
    estadoPlantas = 8;
    alerta();
    return 1;
  }
  else if(nivel == 2){
    estadoPlantas = 2;
    atencao();
    return 1;
  }
  
  //tudo OK
  else{
    estadoPlantas = 1;
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


