#include <Arduino.h>

#include <Servo.h> 
Servo servo_1; 
int pos = 0; 
int switchState; 
void setup() 
{ 
servo_1.attach(9); 
pinMode(10, INPUT); 
digitalWrite(10, HIGH); 
servo_1.write(160); 
} 
void loop(){ 
switchState = digitalRead(10); 
if(switchState==LOW) 
{ 
delay(50); 
servo_1.write(38); 
delay(300); 
for(pos = 38; pos < 160; pos+=4){ 
servo_1.write(pos); 
delay(5); 
} 
} 
delay(5); 
}
