/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
// defines pins numbers
const int stepPinR = 3; 
const int dirPinR = 4; 


const int stepPinL = 5; 
const int dirPinL = 6; 


void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPinL,OUTPUT); 
  pinMode(dirPinL,OUTPUT);
  
  pinMode(stepPinR,OUTPUT); 
  pinMode(dirPinR,OUTPUT);
  Serial.begin( 9600);
}
int d=800;
int num=0;
int dist=32;
void loop() {
  Serial.println("start");

  digitalWrite(dirPinL,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < num; x++) {
    digitalWrite(stepPinL,HIGH); 
    delayMicroseconds(d); 
    digitalWrite(stepPinL,LOW); 
    delayMicroseconds(d); 
  }
  Serial.println("switch");
  delay(200); // One second delay

  digitalWrite(dirPinR,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < num; x++) {
    digitalWrite(stepPinR,HIGH); 
    delayMicroseconds(d); 
    digitalWrite(stepPinR,LOW); 
    delayMicroseconds(d); 
  }
  Serial.println("switch");
  delay(200); // One second delay

  num+=dist;
  
  digitalWrite(dirPinL,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < num; x++) {
    digitalWrite(stepPinL,HIGH);
    delayMicroseconds(d);
    digitalWrite(stepPinL,LOW);
    delayMicroseconds(d);
  }
  delay(200);
  digitalWrite(dirPinR,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < num ; x++) {
    digitalWrite(stepPinR,HIGH);
    delayMicroseconds(d);
    digitalWrite(stepPinR,LOW);
    delayMicroseconds(d);
  }
  delay(200);
  num+=dist;
}
