#include <dht.h>
#define dht_dpin A1 //Pino DATA do Sensor ligado na porta Analogica A1

dht DHT; //Inicializa o sensor

//ATUADORES
int ledAzul = 2;
int ledVermelho = 5;
int cooler = 3;
int regador = 4;

//SENSORES ANALOGICOS
#define P1 A0
#define ldr A2
#define nivelA A7
#define nivelB A6

//Leds Informativos
int ledInfoVermelho = 8;
int ledInfoVerde = 9;
int ledInfoAmarelo = 10;

//dados inerentes
int tempMax = 35;
int tempMin = 10;
int umidadeMaxDoSensor = 700;
int umidadeMax = 80;
int umidadeMin = 60;
int brilhoLuzLed = 200;
int autorizacaoParaRegar = 0;
int estadoPlantas = 1; 

void setup() {
 //Atuadores
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
    //passando os valores das entradas analogicas para as variaveis
    DHT.read11(dht_dpin); //Lê as informações do sensor
    int temp = (DHT.temperature);
    int fonteLuz = calculaFonte(analogRead(ldr));
    int iluminacao = calculaIluminacao(analogRead(ldr));
    int umidadeV1 = calculaUmidade(analogRead(P1));
    int nivel = calculaNivel(analogRead(nivelA), analogRead(nivelB));
    double umidadeAr = (DHT.humidity);
    
    //segurança para evitar queimar a bomba
    autorizacaoParaRegar = seguranca(temp, fonteLuz, umidadeV1, nivel);
  
    //modo autonomo 
    autonomo(temp,fonteLuz, umidadeV1, autorizacaoParaRegar);
    
    //controle pelo pc
    controlePhp(temp, iluminacao, umidadeV1, nivel, umidadeAr, fonteLuz);
}

//controle autonomo
void autonomo(int temp, int fonteLuz, int umidadeV1, int permission){  
  //regar
  if(permission == 1){
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
  
  //Compara a luz solar com a luz dos leds
  if(fonteLuz == 0)
     digitalWrite(ledAzul,LOW);
  else{
     digitalWrite(ledAzul,HIGH);
  }
    
  //ligar cooler para temperatura
  if(temp >= (tempMax - 5)){
     digitalWrite(cooler,HIGH);
  }else{
    digitalWrite(cooler,LOW);
  }
}

void controlePhp(int temp, int iluminacao, int umidadeV1, int nivel, int umidadeAr, int fonteLuz){
  if (Serial.available() >= 0) {
      char lSerial = Serial.read();
      
      //Ler os estados e passar para o PHP
      if(lSerial == 'R'){
        Serial.println(fonteLuz);
        Serial.println(temp);
        Serial.println(umidadeV1);
        Serial.println(umidadeAr);
        Serial.println(digitalRead(regador));
        Serial.println(digitalRead(cooler));
        Serial.println(nivel);
        Serial.println(estadoPlantas);
        Serial.println(iluminacao);
      }
  }
}

int calculaUmidade(int valor){
  return ((valor * 100)/umidadeMaxDoSensor); 
}

int calculaNivel(int alto, int baixo){
  if(alto > 10){
    return 1;
  }
  else if(baixo > 10 ){
    return 2;
  }else{
    return 3;
  }
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

int seguranca(int temp, int iluminacao, int umidadeV1, int nivel){
  //reservatorio baixo
  if(nivel == 3){
    estadoPlantas = 2;
    alerta();
    return 0;
  }
  //umidade baixa
  else if(umidadeV1 < (umidadeMin - 4)){
    estadoPlantas = 4;
    alerta();
    return 1;
  }
  
  //umidade alta
  else if(umidadeV1 > (umidadeMax + 4)){
    estadoPlantas = 5;
    alerta();
    return 1;
  }
  
  //temperatura alta
  else if(temp > tempMax){
    estadoPlantas = 6;
    alerta();
    return 1;
  }
  //temperatura baixa
  else if(temp < tempMin){
    estadoPlantas = 7;
    alerta();
    return 1;
  }
  //Liga o led verde se tudo estiver ok
  if(nivel == 2){
    estadoPlantas = 2;
    atencao();
    return 1;
  }
  else{
    estadoPlantas = 1;
    funcionamentoCorreto();
    return 1;
  }
  
}

//avisa o usuario pela interface de leds
void alerta(){
  digitalWrite(ledInfoVerde,LOW);
  digitalWrite(ledInfoVermelho,HIGH);
  digitalWrite(ledInfoAmarelo,LOW);
}

//avisa o usuario pela interface de leds
void funcionamentoCorreto(){
  digitalWrite(ledInfoVerde,HIGH);
  digitalWrite(ledInfoVermelho,LOW);
  digitalWrite(ledInfoAmarelo,LOW);
}

void atencao(){
  digitalWrite(ledInfoVerde,LOW);
  digitalWrite(ledInfoVermelho,LOW);
  digitalWrite(ledInfoAmarelo,HIGH);
}






