#include <AS5600.h>
AS5600 encoder;

long revolutions1 = 0, revolutions2 = 0; // number of revolutions the encoder has made
float position1 = 0, position2 = 0; // the calculated value the encoder is at
float output1, output2;         // raw value from AS5600
long lastOutput1, lastOutput2;       // last output from AS5600
int dir1 = 1, dir2 = -1;


void selectChannel(uint8_t channel)
{
  if ( channel >= 0 && channel < 8 ) {
    Wire.beginTransmission(0x70);
    byte b = 1;
    for (int i = 0; i < channel; i++) {
      b = b << 1;
    }
    Wire.write(b);
    Wire.endTransmission();
  } else {
    Serial.print("TCA9546A ERROR - Wrong channel selected: ");
    Serial.print(channel);
    Serial.print(" (available channels 0,1,2 and 3)");
  }
}

class Motor {
  private:
    int dir = 1;
    void setDirPins() {
      if (dir * vDir > 0) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
      } else {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
      }
    }
  public:
    int pinEn, pin1, pin2;
    int vDir;
    Motor(int pinEn, int pin1, int pin2, int vDir) {
      this->pinEn = pinEn;
      this->pin1 = pin1;
      this->pin2 = pin2;
      pinMode(pinEn, OUTPUT);
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      this->vDir = vDir;
      analogWrite(pinEn, 0);
      setDirPins();
    }
    void setSpeed(int speed) {
      if(speed<0){
        speed=map(speed,0,-1023,-550,-1023);  
      }else{
        speed=map(speed,0,1023,100,850);
        }
      
      speed=max(min(speed, 1023), -1023);
      if ((speed ^ dir) < 0) {
        setDirPins();
        dir = copysign(1, speed);
      }
      analogWrite(pinEn, abs(speed));
    }
};

Motor motorR(D3, D5, D6, 1);
Motor motorL(D4, D7, D8, 1);

class PID{
  private:
    float isum=0;
    float lV=0;
  public:
    float kp=1.2;
    float ki=.5;
    float kd=0;
    float iSumMax=1023;
    float getVal(float nd){
      float sum=nd*kp;
      isum+=nd*ki;
      sum+=isum;
      sum+=(lV-nd)*kd;
      isum=max(min(isum,iSumMax),-iSumMax);
      return(sum);
    }
    
};

void setup() {
  Serial.begin(9600);
  Serial.println("I solemnly swear i am up to no good");
  selectChannel(0);
  output1 = encoder.getPosition();
  lastOutput1 = output1;
  position1 = output1;
  selectChannel(1);
  output2 = encoder.getPosition();
  lastOutput2 = output2;
  position2 = output2;
}

void readPos() {
  selectChannel(0);

  output1 = dir1 * encoder.getPosition();         // get the raw value of the encoder
  if ((lastOutput1 - output1) > 2047 )        // check if a full rotation has been made
    revolutions1++;
  if ((lastOutput1 - output1) < -2047 )
    revolutions1--;
  position1 = revolutions1 * 4096 + output1;   // calculate the position the the encoder is at based off of the number of revolutions
  lastOutput1 = output1;                      // save the last raw value for the next loop

  selectChannel(1);
  output2 = dir2 * encoder.getPosition();         // get the raw value of the encoder
  if ((lastOutput2 - output2) > 2047 )        // check if a full rotation has been made
    revolutions2++;
  if ((lastOutput2 - output2) < -2047 )
    revolutions2--;
  position2 = revolutions2 * 4096 + output2;   // calculate the position the the encoder is at based off of the number of revolutions
  lastOutput2 = output2;                      // save the last raw value for the next loop
}



int count = 0;

int sa = 0;
int sb= 0;

PID pidL;
PID pidR;
int resetTest=false;
void loop() {
  delay(100);
  readPos();
  //float velL = max(min(int((position1-sp) * kp), 1023), -1023);
  float velR=pidR.getVal(position2-sb); //;max(min(int((position2-sp2) * kp), 1023), -1023);
  
  float velL =pidL.getVal(position1-sa);
/*
  if (millis() % (6000) > 4500) {
    
    velR=count%1023;
    resetTest=true;
  } else {
    if(resetTest==true){
      resetTest=false;
      Serial.printf("%d %f endPos\n",count,position2);
      count+=20;
    }
    velR=pidR.getVal(position2-sb) ;
  }
    //*/
  motorL.setSpeed(int(velL));
  motorR.setSpeed(int(velR));

  int timeLen=5000;
  if (millis() % (2*timeLen) > timeLen) {
    sa = 1000;
  } else {
    sa = 5000;
  }
  sa=sin(millis()/6000.0)*1500;
  if ((millis()+timeLen/2) % (2*timeLen) > timeLen) {
    sb = 1000;
  } else {
    sb = 5000;
  }
    sb=cos(millis()/6000.0)*1500;
//*/

  
  Serial.printf("ch0: %f ch1: %f  %d %d\n", position1, position2,sa,sb);


}
