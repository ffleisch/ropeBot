#include <AS5600.h>
AS5600 encoder;

long revolutionsL = 0, revolutionsR = 0; // number of revolutions the encoder has made
float positionL = 0, positionR = 0; // the calculated value the encoder is at
float positionLold = 0, positionRold = 0; // the calculated value the encoder is at
float outputL, outputR;         // raw value from AS5600
long lastoutputL, lastoutputR;       // last output from AS5600
int dirL = 1, dirR = -1;


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
        speed=map(speed,0,-1023,-500,-1023);  
      }else{
        speed=map(speed,0,1023,100,1023);
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
    float kp=2;
    float ki=.1;
    float kd=.05;
    float iSumMax=1023;
    float getVal(float nd){
      float sum=nd*kp;
      isum+=nd*ki;
      sum+=isum;
      sum+=(lV-nd)*kd;
      isum=max(min(isum,iSumMax),-iSumMax);
      return(sum);
    }
    PID(float kp,float ki,float kd,float iSumMax){
      this->kp =kp;  
      this->ki =ki; 
      this->kd =kd;
      this->iSumMax=iSumMax;
    };
};

class Filter{
  private:
    float state=0;
  public:
    float str=0;
    float getVal(float inp){
      state=state*str+inp*(1-str);
      return state;
    }  
};


void setup() {
  Serial.begin(9600);
  Serial.println("I solemnly swear i am up to no good");
  selectChannel(0);
  outputL = encoder.getPosition();
  lastoutputL = outputL;
  positionL = outputL;
  selectChannel(1);
  outputR = encoder.getPosition();
  lastoutputR = outputR;
  positionR = outputR;
}

void readPos() {
  selectChannel(0);
  
  outputL = dirL * encoder.getPosition();         // get the raw value of the encoder
  if ((lastoutputL - outputL) > 2047 )        // check if a full rotation has been made
    revolutionsL++;
  if ((lastoutputL - outputL) < -2047 )
    revolutionsL--;
  positionLold=positionL;
  positionL = revolutionsL * 4096 + outputL;   // calculate the position the the encoder is at based off of the number of revolutions
  lastoutputL = outputL;                      // save the last raw value for the next loop

  selectChannel(1);
  outputR = dirR * encoder.getPosition();         // get the raw value of the encoder
  if ((lastoutputR - outputR) > 2047 )        // check if a full rotation has been made
    revolutionsR++;
  if ((lastoutputR - outputR) < -2047 )
    revolutionsR--;
  positionRold=positionR;

  positionR = revolutionsR * 4096 + outputR;   // calculate the position the the encoder is at based off of the number of revolutions
  lastoutputR = outputR;                      // save the last raw value for the next loop
}



int count = 0;

int sa = 0;
int saOld=0;
int sb= 0;
int sbOld=0;

PID pidLPos(2,.1,.05,100);
PID pidLVel(1,0,0,1023);

PID pidRPos(.6,0,0,100);

PID pidRVel(1,0,0,500);


Filter filtL;
Filter filtR,filtVelR;

float velSR=0;
float velSL=0;
int resetTest=false;

void loop() {
  delay(10);
  readPos();
  
  //float velSL = max(min(int((positionL-sp) * kp), 1023), -1023);

  float velR=filtR.getVal(positionR-positionRold);
  float velRBase=filtVelR.getVal(sb-sbOld);
  
  
  //float velSR=pidRVel.getVal(pidRPos.getVal(positionR-sb)-velR); //;max(min(int((positionR-sp2) * kp), 1023), -1023);
  velSR+=pidRVel.getVal(velR-velRBase);
  
  //velSL+=pidLPos.getVal(positionL-sa);

  velSR=velSR>1023?1023:(velSR<-1023?-1023:velSR);
  //velSR*=0.99;
/*
  if (millis() % (6000) > 4500) {
    
    velSR=-count%1023;
    resetTest=true;
  } else {
    if(resetTest==true){
      resetTest=false;
      Serial.printf("%d %f endPos\n",count,positionR);
      count+=20;
    }
    velSR=pidR.getVal(positionR-sb) ;
  }
    //*/
  motorL.setSpeed(int(velSL));
  motorR.setSpeed(int(velSR));

  sbOld=sb;
  
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
  sb=cos(millis()/5000.0)*1000;

//*/

  
  //Serial.printf("ch0: %f ch1: %f  %d %d %f\n", positionL, positionR,sa,sb,velR);

  Serial.printf("%f %f %f\n",velR,velRBase);

}
