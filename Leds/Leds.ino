void setup() {
   Serial.begin(9600);
   pinMode(10, INPUT); // Configuração para detecção de derivações LO +
   pinMode(11, INPUT); // Configuração para detecção de leads off LO -
   pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
}
 
void loop() {
 
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
     Serial.println('!');
  } else {
    // envia o valor da entrada analógica 0:
    Serial.println(analogRead(A0));
  }
  if (analogRead (A0) < 400){
    digitalWrite(12, HIGH);
  } else if (analogRead (A0) > 400){
    digitalWrite(13, HIGH);
  
  }
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  // Espere um pouco para evitar que os dados seriais saturem
  delay(1);
}