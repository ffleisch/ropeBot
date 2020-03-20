/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  Serial.begin( 9600);
}
int d=1000;
void loop() {
  Serial.println("start");

  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 500; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(d); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(d); 
  }
  Serial.println("switch");
  //delay(2000); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 0; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(d);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(d);
  }
 //delay(2000);
}
