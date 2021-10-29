//Inclui as bibliotecas
#include <EEPROM.h>
#include <Thermistor.h>
#include <DS1307.h>

DS1307 rtc(A4, A5);

//Atuadores
int ledAzul = 2;
int ledVermelho = 3;
int regador = 12;

//Led informativo
int ledFuncionamento = 2;

//Sensores
int sensorUmidade = A0;
int sensorLdr = A1;
Thermistor temp(2);

//Dados do instante
int umidade = 0;
int taxaIluminacao = 0;
int temperatura = 0;

//Estado da planta
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
  //Aciona o relogio
  rtc.halt(false);
   
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  rtc.setDOW(FRIDAY);      //Define o dia da semana
  rtc.setTime(10, 5, 30);     //Define o horario
  rtc.setDate(24, 2, 17);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  
  //leds de crescimento
  pinMode(ledVermelho,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  pinMode(regador,OUTPUT);
  
  //leds informativos
  pinMode(ledFuncionamento,OUTPUT);
  
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
  temporizador(3); 
  iluminacaoSol(5);
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


//*****************************Funçes de controle de tempo***********************


void tempo(){
  //codigo rtc;
  //Serial.print("Hora : ");
  //Serial.print(rtc.getTimeStr());
  //Serial.print(" ");
  //Serial.print("Data : ");
  //Serial.print(rtc.getDateStr(FORMAT_SHORT));
  //Serial.print(" ");
  //Serial.println(rtc.getDOWStr(FORMAT_SHORT));
  //talvez aqui tenha que ter um delay
}

int balanco = 0;
void temporizador(int second){
  int segundo = (millis()/1000);
  if((segundo-balanco) == second){
    balanco = segundo;
    Serial.print(estadoPlantaUmidade);
    Serial.print(estadoPlantaLuz);
    Serial.println(estadoPlantaTemperatura);
  }
}

int balancoMin = 0;
int minutosSol = 0;
int iluminacaoSol(int mint){
  int minuts = String(rtc.getTimeStr()).toInt();
  if((minuts-balancoMin) == mint){
    balancoMin = minuts;
    minutosSol += 5;
    
    if(minutosSol>60){
      horasSolAtual++;
      minutosSol = 0;
      EEPROM.write(0, horasSolAtual);
    }
  }
}



//**********************************Le os sensores************************************************
void leOsSensores(){
  umidade = 55;//map(analogRead(A0),0,1023,0,100);
  taxaIluminacao = 70;//map(analogRead(A1),0,1023,0,100);
  temperatura = 15;//temp.getTemp();
}
//**********************************Pega e manda dados para o pc**********************************
void comUsb(){
  if (Serial.available() > 0){
    String dados = Serial.readString();
    //iluxtmpyumiz
    horasSol = dados.substring(0,1).toInt();
    temperaturaDesejada = dados.substring(1,2).toInt();
    umidadeDesejada = dados.substring(2).toInt();
  }
}
