

#define sensor A0 // Sharp IR GP2Y0A41SK0F 





#include <Servo.h>
float volts;

const int motorR1 = 6;
const int motorR2 = 5;
const int motorL1 = 11;
const int motorL2 = 10;

const int servo = 9;

void stop1();
void forward();
void backward();
void right();
void left();
Servo myservo;          


unsigned int rightDistance;
unsigned int leftDistance;
int distance ;
int pos; // for servo, 90 deg looking forward
int incomingData; 
char mode = 'm'; // m for manual, a for automatic
int vSpeed = 200; 
void setup(){
  //Define inputs and outputs
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  
  myservo.attach(9); 
  myservo.write(pos); 
  
  
  Serial.begin(9600);
}



void loop(){

  if(Serial.available() > 0){     
       // read the incoming byte:
       incomingData = Serial.read();
       if (incomingData == 'A'){mode = 'a'; }
       if (incomingData == 'M'){mode = 'm'; 
          stop1();
          pos=90;;
          myservo.write(pos); 
       }  
    }

  //Change speed 
    if (incomingData == '0'){
      vSpeed=0;}
    else if (incomingData == '1'){
      vSpeed=100;}
    else if (incomingData == '2'){
      vSpeed=180;}
    else if (incomingData == '3'){
      vSpeed=200;}
    else if (incomingData == '4'){
      vSpeed=255;}
    
    //Manual mode
    if (mode == 'm'){
      if (incomingData=='F'){
        forward();
    }
      else if (incomingData=='B'){
        backward();
      }
      else if (incomingData=='R'){
        right();
      }
      else if (incomingData=='L'){
        left();
      }
      else if (incomingData=='S'){
        stop1();
      }
    }
    //Automatic mode
    else if (mode == 'a'){
  
    // distance sensor
    volts = analogRead(sensor)*0.0048828125;  
  distance = 13*pow(volts, -1);
      
    
    if (distance <= 10){
      Serial.println(distance);
      stop1();
      
      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        myservo.write(pos);               // tell servo to go to position in variable 'pos'
          delay(20);                         // waits 20ms for the servo to reach the position
        }
        //Read new distance from the right side
        volts = analogRead(sensor)*0.0048828125;  
        distance = 13*pow(volts, -1);// 
        leftDistance = distance;
        delay(100);
        //Now look left
        for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
          myservo.write(pos);               // tell servo to go to position in variable 'pos'
          delay(20);                         // waits 20ms for the servo to reach the position
        }
        //Read new distance from the left side
        volts = analogRead(sensor)*0.0048828125;  
        distance = 13*pow(volts, -1);
        rightDistance = distance;
        delay(100);
        pos = 90; // look forward again
        myservo.write(pos);
      
        //Finally compare left and right distances and make the best turn decision
        if (leftDistance > rightDistance){
          left();
          delay(500); 
        }
        else if (leftDistance < rightDistance){
          right();
          delay(500);
        }
        else{ //distances is equal
          backward();
          delay(1000);// Go back for 1 sec 
          left();   // and turn left
          delay(500);
        }
    }else{
      
       forward();
    }
    
    
   
}

}
//Movement functions

void forward(){
  analogWrite(motorR1, vSpeed);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, vSpeed);
  analogWrite(motorL2, 0);
}

void backward(){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, vSpeed);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, vSpeed);
}

void right(){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, vSpeed);
  analogWrite(motorL1, vSpeed);
  analogWrite(motorL2, 0);
}

void left(){
  analogWrite(motorR1, vSpeed);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, vSpeed);
}

void stop1(){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
}
