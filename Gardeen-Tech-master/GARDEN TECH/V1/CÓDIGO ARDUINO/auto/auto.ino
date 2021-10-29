#include <Thermistor.h>

//ATUADORES
int led = 2;
int cooler = 3;
int regadorA = 4;
int regadorB = 5;

//SENSORES ANALOGICOS
#define P1 A0
#define P2 A1
#define ldr A2
#define nivelB A4
#define nivelA A5
Thermistor temperatura(A3);

//configs globais
int tempMax = 35;
int tempMin = 10;
int umidadeMax = 80;
int umidadeMin = 60;
int brilhoLuzLed = 200;
int fimPlantacao = 1;

//Leds de Controle
int ledVermelho = 8;
int ledVerde = 9;

int executa = 0;

String estadoPlantas;

void setup() {
 //Atuadores
 pinMode(led,OUTPUT);
 pinMode(cooler,OUTPUT);
 pinMode(regadorA,OUTPUT);
 pinMode(regadorB,OUTPUT);
 //Sensores
 pinMode(P1,INPUT);
 pinMode(P2,INPUT);
 pinMode(ldr,INPUT);
 pinMode(nivelB,INPUT);
 pinMode(nivelA,INPUT);
 //Leds de controle
 pinMode(ledVermelho,OUTPUT);
 pinMode(ledVerde,OUTPUT);

 //definindo velocidade de comunicaçao
 Serial.begin(9600);
}

void loop(){
  while(executa != 1){  
    int permission=0;

    //passando os valores das entradas analogicas para as variaveis
    double temp = temperatura.getTemp();
     
    //calcula iluminacao
    int iluminacao;
    if(analogRead(ldr) > brilhoLuzLed){
      iluminacao = 0;
    }
    else{
      iluminacao = 1;
    }
    //calcula umidade
    int umidadeV1 = calculaUmidade(analogRead(P1));
    int umidadeV2 = calculaUmidade(analogRead(P2));
    
    //calcula Nivel
    int nivel = calculaNivel();
    
    //controle do sistema para evitar falha
    permission = seguranca(temp,iluminacao, umidadeV1, umidadeV2, nivel);
    
    //controle pelo pc
    controlePhp(temp,iluminacao, umidadeV1, umidadeV2, nivel);
  
    //modo autonomo 
    autoMatico(temp,iluminacao, umidadeV1, umidadeV2, permission);
  }
  exit;
}
int calculaUmidade(int valor){
  return (valor/8); 
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

int seguranca(int temp, int iluminacao, int umidadeV1, int umidadeV2, int nivel){
  //reservatorio baixo
  if(nivel == 3){
    estadoPlantas = "3";
    alerta();
    return 0;
  }
  //umidade baixa
  else if(umidadeV1 < (umidadeMin - 5)){
    estadoPlantas = "4";
    alerta();
    return 1;
  }
  else if(umidadeV2 < (umidadeMin - 5)){
    estadoPlantas = "5";
    alerta();
    return 1;
  }
  
  //umidade alta
  else if(umidadeV1 > (umidadeMax + 6)){
    estadoPlantas = "6";
    alerta();
    return 1;
  }
  else if(umidadeV2 > (umidadeMax + 6)){
    estadoPlantas = "7";
    alerta();
    return 1;
  }
  //temperatura alta
  else if(temp > tempMax){
    estadoPlantas = "9";
    alerta();
    return 1;
  }
  //temperatura baixa
  else if(temp < tempMin){
    estadoPlantas = "11";
    alerta();
    return 1;
  }
  //Liga o led verde se tudo estiver ok
  else if(analogRead(ldr)>100){
    estadoPlantas = "8";
    alerta();
    return 1;
  }
  else if(nivel == 2){
    estadoPlantas = "2";
    return 1;
  }
  else{
    estadoPlantas = "1";
    funcionamentoCorreto();
    return 1;
  }
  
}

//avisa o usuario pela interface de leds
void alerta(){
  digitalWrite(ledVermelho,HIGH);
  digitalWrite(ledVerde,LOW);
  }

//avisa o usuario pela interface de leds
void funcionamentoCorreto(){
  digitalWrite(ledVerde,HIGH);
  digitalWrite(ledVermelho,LOW);
}

//controle autonomo
void autoMatico(int temp, int iluminacao, int umidadeV1, int umidadeV2, int permission){  
  //regar
  if(permission == 1){
    //umidade baixa planta1
    if(umidadeV1 < umidadeMin)
      digitalWrite(regadorA,HIGH);
    else{
      if(umidadeV1 >= umidadeMax) 
        digitalWrite(regadorA,LOW);
    }
    
    //umidade baixa planta2
    if(umidadeV2 < umidadeMin)
      digitalWrite(regadorB,HIGH);
    else{
      if(umidadeV2 >= umidadeMax) 
        digitalWrite(regadorB,LOW);
    }
  }//fim de irrigaçao e do bloco de permissao
  
  //Compara a luz solar com a luz dos leds
  if(iluminacao == 0)
     digitalWrite(led,LOW);
  else{
     digitalWrite(led,HIGH);
  }
    
  //ligar cooler para temperatura
  if(temp >= (tempMax - 5)){
     digitalWrite(cooler,HIGH);
  }else{
    digitalWrite(cooler,LOW);
  }
}


//controle pelo pc
void controlePhp(int temp, int iluminacao, int umidadeV1, int umidadeV2, int nivel){
  if (Serial.available() >= 0) {
      char lSerial = Serial.read();
      
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
        Serial.println(estadoPlantas);
      }
      //fim de plantaçao
      else if(lSerial == 'F'){
        executa = 1;
     }
  }
}


