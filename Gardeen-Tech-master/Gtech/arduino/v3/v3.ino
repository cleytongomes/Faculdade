//MEMORIA ARDUINO
#include <EEPROM.h>
#include <Thermistor.h>

//Atuadores
int ledAzul = 2;
int ledVermelho = 3;
int regador = 12;

//informativos sensor / estado
int infoTemp = 6;
int infoIlu = 5;
int infoUmi = 13;

int infoBom = 8;
int infoBaixo = 7;
int infoAlto = 11;

//Sensores
int sensorUmidade = A0;
int sensorLdr = A1;
Thermistor temp(3);

//Dados do instante
int umidade = 0;
int taxaIluminacao = 0;
int temperatura = 0;

int horasSolAtual = 0;
int estadoPlantaUmidade;
int estadoPlantaLuz;
int estadoPlantaTemperatura = 0;

//Dados da minha especie
int horasSol = 4;//1-5
int umidadeDesejada = 3;//1-5;
int temperaturaDesejada = 2;//1-4

void setup()
{
  //leds de crescimento
  pinMode(ledVermelho,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  
  //leds informativos
  pinMode(infoTemp,OUTPUT);
  pinMode(infoIlu,OUTPUT);
  pinMode(infoUmi,OUTPUT);
  pinMode(infoBom,OUTPUT);
  pinMode(infoBaixo,OUTPUT);
  pinMode(infoAlto,OUTPUT);
  
  //velocidade de comunicacao
  Serial.begin(9600);
  
  horasSolAtual = EEPROM.read(0);
}
//**********************************principal loop**********************************
void loop()
{
  tempo();
  leOsSensores();
  comUsb();
  analisa();
  decide(); 
  apresentaResultados(5);
}

//**********************************calcula o estado da planta e toma decisao**********************************
void analisa(){
  if(umidade > ((umidadeDesejada-1)*20) && umidade < ((umidadeDesejada)*20)){
    estadoPlantaUmidade = 2;
  }else if(umidade < ((umidadeDesejada-1)*20)){
    estadoPlantaUmidade = 2;
  }else{
    estadoPlantaUmidade = 3;
  }
  
  if(horasSolAtual > ((horasSol-1)*5) && horasSolAtual < (horasSol)*5){
    estadoPlantaLuz = 2;
  }else if(horasSolAtual < (horasSol-1)*5){
    estadoPlantaLuz = 1;
  }else if(horasSolAtual > ((horasSol)*5)){
    estadoPlantaLuz = 3;
  }
  
  if(temperatura > ((temperaturaDesejada-1)*10) && temperatura<(temperaturaDesejada*10)){
    estadoPlantaTemperatura = 2;
  }else if(temperatura<((temperaturaDesejada-1)*10)){
    estadoPlantaTemperatura = 1;
  }else if(temperatura>(temperaturaDesejada*10)){
    estadoPlantaTemperatura = 3;
  }
}


void decide(){
  if(estadoPlantaLuz == 1){
    analogWrite(ledAzul, 215);
    analogWrite(ledVermelho, 200);
  }else if(estadoPlantaLuz == 2){
    analogWrite(ledAzul, 0);
    analogWrite(ledVermelho, 0);
  }else if(estadoPlantaLuz == 3){
    analogWrite(ledAzul, 0);
    analogWrite(ledVermelho, 0);
  }
  
  
  if(estadoPlantaUmidade == 1){
    digitalWrite(regador,HIGH);
  }else if(estadoPlantaUmidade == 2){
    digitalWrite(regador,LOW);
  }else if(estadoPlantaUmidade == 3){
    digitalWrite(regador,LOW);
  }
  
  //Atuaçao que pode ser testada depois
  //led vermelho aquecer a planta
  /*if(estadoPlantaTemperatura == 1){
    analogWrite(ledVermelho,50);
  }else if(estadoPlantaTemperatura == 2){
    analogWrite(ledVermelho,0);
  }else if(estadoPlantaTemperatura == 3){
    analogWrite(ledVermelho,0);
  }*/
}

int indice = 0;
int balancoTimeFunction2 = 0;
void apresenta(){
  if(indice == 1){
    //mostra temperatura
    mostraTemperatura();
  }else if(indice == 2){
    mostraIluminacao();
  }else if(indice == 3){
    mostraUmidade();
  }
  
  indice++;
  if(indice > 3){
    indice = 1;
  }
}

//*****************************Funçes de controle de tempo***********************

int segundo = 0;
int minuto = 0;
int hora = 0;
int dia = 0;
long int balanco = 0;

int apresentaResultados(int exeSegundos){
  if(segundo%exeSegundos == 0 && balancoTimeFunction2 != segundo  ){
    balancoTimeFunction2 = segundo;
    apresenta();
  }
  return 0;
}


void tempo(){
  segundo = (millis()-balanco)/1000;
  if(segundo == 60){
    segundo = 0;
    balanco = millis();
    minuto++;
    if(minuto == 60){
      minuto = 0;
      hora++;
      if(hora == 24){
        dia++;
        horasSolAtual = 0;
        hora = 0;
      }
    }
  }
  
  timeFunction(59);
}

int minutosSol=0;
int balancoTimeFunction = 0;
int timeFunction(int exeSegundos){
  if(segundo%exeSegundos == 0 && balancoTimeFunction != segundo  ){
    balancoTimeFunction = segundo;
    if(taxaIluminacao>60){
      minutosSol++;
      if(minutosSol/2==60){
        minutosSol = 0;
        horasSolAtual++;
        EEPROM.write(0, horasSolAtual);
      }
    }
      return 1;
  }
  return 0;
}

//**********************************Le os sensores************************************************
void leOsSensores(){
  umidade = 55;//map(analogRead(A0),0,1023,0,100);
  taxaIluminacao = 70;//map(analogRead(A1),0,1023,0,100);
  temperatura = 15;//temp.getTemp();
}
//**********************************Pega e manda dados para o pc**********************************
void comUsb(){
  if (Serial.available() >= 0){
    char a = Serial.read();
    if(a == 'R'){
      Serial.println(umidade);
      Serial.println(taxaIluminacao);
      Serial.println(temperatura);
      Serial.println(estadoPlantaUmidade);
      Serial.println((horasSol*5)-horasSolAtual);
      Serial.println(estadoPlantaTemperatura);
      Serial.println(horasSolAtual);
    }
  }
}


//**********************************Atuadores passivos de infomaçao**********************************
void mostraTemperatura(){
  digitalWrite(infoTemp,HIGH);
  digitalWrite(infoIlu,LOW);
  digitalWrite(infoUmi,LOW);
  apresenta(estadoPlantaTemperatura);
}
void mostraUmidade(){
  digitalWrite(infoTemp,LOW);
  digitalWrite(infoIlu,LOW);
  digitalWrite(infoUmi,HIGH);
  apresenta(estadoPlantaUmidade);
}

void mostraIluminacao(){
  digitalWrite(infoTemp,LOW);
  digitalWrite(infoIlu,HIGH);
  digitalWrite(infoUmi,LOW);
  apresenta(estadoPlantaLuz);
}

void apresenta(int range){
  if(range == 1){
    baixo();
  }else if(range == 2){
    bom();
  }else if(range == 3){
    alto();
  }
}

void baixo(){
  digitalWrite(infoBom,LOW);
  digitalWrite(infoBaixo,HIGH);
  digitalWrite(infoAlto,LOW);
}

void bom(){
  digitalWrite(infoBom,HIGH);
  digitalWrite(infoBaixo,LOW);
  digitalWrite(infoAlto,LOW);  
}

void alto(){
  digitalWrite(infoBom,LOW);
  digitalWrite(infoBaixo,LOW);
  digitalWrite(infoAlto,HIGH);
}
