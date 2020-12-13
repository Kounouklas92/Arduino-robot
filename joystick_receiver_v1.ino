
#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (40, 53); 
struct package
{
  int X=512;
  int Y=512;
};

byte addresses[][6] = {"0"}; 
int OUT1 = 5;
int OUT2 = 6;
int OUT3 = 9;
int OUT4 = 10;


typedef struct package Package;
Package data;

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  analogWrite(OUT1, 0);
  analogWrite(OUT2, 0);
  analogWrite(OUT3, 0);
  analogWrite(OUT4, 0);
}


void loop()  
{
  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("X:");
    Serial.print(data.X);
    Serial.print("      Y");
    Serial.println(data.Y);
    int X = data.X;
    int Y = data.Y;
    int foward = map(X,524,1024,0,255);
    int backward = map(X,500,0,0,255);
    int right = map(Y,524,1024,0,255);
    int left = map(Y,500,0,0,255);
    if(X > 524 && Y < 524 && Y > 500){
      analogWrite(OUT3, foward);
      analogWrite(OUT4, 0);
      analogWrite(OUT1, foward);
      analogWrite(OUT2, 0);
    }else if(X < 500 && Y < 524 && Y > 500){
      analogWrite(OUT4, backward);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, backward);
      analogWrite(OUT1, 0);
    }else if(X < 524 && X > 500 && Y < 524 && Y > 500){
      analogWrite(OUT4, 0);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, 0);
      analogWrite(OUT1, 0);
    }else if(X < 524 && X > 500 && Y > 524){
      analogWrite(OUT4, 0);
      analogWrite(OUT3, left);
      analogWrite(OUT2, left);
      analogWrite(OUT1, 0);
    }else if(X < 524 && X > 500 && Y < 500){
      analogWrite(OUT4, right);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, 0);
      analogWrite(OUT1, right);
    }else if(X > 524 && Y > 524){
      analogWrite(OUT3, foward);
      analogWrite(OUT4, 0);
      analogWrite(OUT1, foward-right);
      analogWrite(OUT2, 0);
    }else if(X > 524 && Y < 500){
      analogWrite(OUT3, foward-left);
      analogWrite(OUT4, 0);
      analogWrite(OUT1, foward);
      analogWrite(OUT2, 0);
    }else if(X < 500 && Y > 524){
      analogWrite(OUT4, backward);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, backward-right);
      analogWrite(OUT1, 0);
    }else if(X < 500 && Y < 500){
      analogWrite(OUT4, backward-left);
      analogWrite(OUT3, 0);
      analogWrite(OUT2, backward);
      analogWrite(OUT1, 0);
    }
    
  }

}
