



#define STEP_ARR_SIZE 1024

byte steps[STEP_ARR_SIZE];

int current_step=0;
int valid_step=0;

void printBinary(byte b) {
  for (int i = 7; i >= 0; i-- )
  {
    Serial.print((b >> i) & 0X01);//shift and select first bit
  }
  Serial.println();
}

void interpret_next_step(){
  byte b=steps[current_step];
  b=b&15;
  b=b<<3;
  //printBinary(b);
  delayMicroseconds(10);
  //delay(20);
  PORTD=b;
  delayMicroseconds(800);
  //delay(100);
  PORTD&=B01010000;
  
  current_step++;
  current_step%=STEP_ARR_SIZE;
}




void setup() {
  Serial.begin(38400);
  Serial.println("HELLO :D");
  //Serial.setTimeout(10);

  for(int i=0;i<STEP_ARR_SIZE;i++){
    steps[i]=3;    
  }
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  
  // put your setup code here, to run once:

}


int write_before(int start_step, int space,int num_av){
  /*Serial.print(start_step);
  Serial.print(" ");
  Serial.print(space);
  Serial.print(" ");
  Serial.print(num_av);
  Serial.print("\n");*/
  if(space<num_av){
    
    valid_step+=Serial.readBytes(steps+start_step,space);
    //Serial.println("full");
    return 0;
  }else{
    
    valid_step+=Serial.readBytes(steps+start_step,num_av);
    return 1;
  }
}


//int show_char=33;
void loop() {
  // put your main code here, to run repeatedly:

 
  if(current_step!=valid_step){
    interpret_next_step();
    /*steps[current_step]=show_char;
    current_step++;
    if(current_step==STEP_ARR_SIZE){
      show_char+=1;  
    }
    current_step%=STEP_ARR_SIZE;
    */
  }
  /* Serial.print("Vs ");
  Serial.print(valid_step);
  Serial.print(" Cs ");
  Serial.print(current_step);
  Serial.print(" Av ");
  Serial.print(num_av);
  Serial.print(" Buffer ");
    for(int i=0;i<STEP_ARR_SIZE;i++){
    Serial.print((char)steps[i]);
  }*/
  //Serial.println();
  int num_av=Serial.available();

  if(num_av>30){
      /*Serial.println("before");
      for(int i=0;i<STEP_ARR_SIZE;i++){
        Serial.print((char)steps[i]);
      }
      Serial.println();*/      

      
      int diff=current_step-valid_step-1;
      
      int res=0;
      if(diff>0){
        res=write_before(valid_step+1,diff-1,num_av);
      }else{
          if(valid_step+1+num_av<=STEP_ARR_SIZE){
            valid_step+=Serial.readBytes(steps+valid_step+1,num_av);
            valid_step%=STEP_ARR_SIZE;
          }else{
            int space=STEP_ARR_SIZE-valid_step-1;
            valid_step+=Serial.readBytes(steps+valid_step+1,space);
            
            if(current_step>1){
              res=write_before(0,current_step-1,num_av-space);
            }
            valid_step%=STEP_ARR_SIZE;
          
          }
     
      }
      //num_av=Serial.available();
      Serial.print(num_av);
      Serial.print(" ");
      Serial.print((STEP_ARR_SIZE+diff)%STEP_ARR_SIZE);
      Serial.print(" Vs ");

      Serial.print(valid_step);
      Serial.print(" Cs ");
      Serial.println(current_step);
      /*if(res==0){
        Serial.print("0");
      }else{
        Serial.print("1");
      }*/
        /*Serial.println("after");
        for(int i=0;i<STEP_ARR_SIZE;i++){
          Serial.print((char)steps[i]);
        }*/
        

      //if(num_av>(STEP_ARR_SIZE-valid_step)){
        
      //}

      
        
    
  }
  //delay(1000);
}
