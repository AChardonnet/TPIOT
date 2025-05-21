#define tds 35

void setup() {
  Serial.begin(115200);
  pinMode(tds, INPUT);
  Serial.println("Setup done");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(tds));
  delay(1000); 
}
