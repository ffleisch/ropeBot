
void interpret_next_step(){

  #if defined(ARDUINO_AVR_NANO)
    byte b=Serial.read();
    b=b&15;
    b=b<<3;
    //printBinary(b);
  
    //set dir pins
    PORTD =  B01010000&b;
    delayMicroseconds(2);
    PORTD |= B00101000&b;
    delayMicroseconds(2);
    //delayMicroseconds(800);
    //delay(100);
    PORTD=0;
  #elif defined(ARDUINO_ARCH_ESP8266)
    int b=Serial.read();
    b=b&15;
    b=b<<12;
    //printBinary(b);
  
    //set dir pins
    GPOS =  (B1010<<12)&b;
    delayMicroseconds(10);
    GPOS= (B0101<<12)&b;
    delayMicroseconds(10);
    //delayMicroseconds(800);
    //delay(100);
    GPOC=b;
  #else
    #error Unsupported board selection.
  #endif
  

}



void setup() {
  Serial.begin(115200);
  Serial.println("HELLO :D");
  //Serial.setTimeout(10);


  #if defined(ARDUINO_AVR_NANO)
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
  #elif defined(ARDUINO_ARCH_ESP8266)
    pinMode(12,OUTPUT);
    pinMode(13,OUTPUT);
    pinMode(14,OUTPUT);
    pinMode(15,OUTPUT);
  #else
  #error Unsupported board selection.
  #endif
}
long last_step_time=0;
int intervall=10000;

void loop() {
  // put your main code here, to run repeatedly:
  //int t=micros();
  int n_av=Serial.available();
  
  //if(n_av>0){
    //while(Serial.available()>0){
    //  interpret_next_step();
    //}
  //}
  if((micros()-last_step_time)>intervall && n_av>0){
    Serial.print(micros()-last_step_time);
    Serial.print(" ");
    last_step_time=micros();
    interpret_next_step();
    Serial.println(n_av);
  }
  //int t2=micros()-t;
  //Serial.println(t2);
}
