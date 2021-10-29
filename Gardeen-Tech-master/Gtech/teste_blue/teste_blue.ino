
void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);  
}

// the loop routine runs over and over again forever:
void loop() {
  if(Serial.available() >= 0){
    String a = Serial.readString();
    if(a=="on"){
       digitalWrite(13,HIGH);
    }else if (a=="off"){
      digitalWrite(13,LOW);
    }
  }
  
  Serial.println("25");
  delay(100);
}


