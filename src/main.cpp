// Includes
#include <Arduino.h>
#include <Servo.h>


// Variables
Servo servo_arm;
int pos = 90;
int target_pos = 38; // replace with target pos.
int start_pos = 160; // start pos.
int switchState;
int attempts = 0;


// Hi Method
void say_hi()
{
  Serial.println("Showing Hi Method!");
  servo_arm.write(60);
  delay(2000);
  servo_arm.write(start_pos);
  delay(50);
}


// Angry Method
void angry(int angryCt = 0)
{
  Serial.println("Showing Angry Method!");
  servo_arm.write(55);
  delay(500);
  servo_arm.write(start_pos);
  delay(300);
  for(angryCt = 0; angryCt <= 5; angryCt++)
  {
    servo_arm.write(60);
    delay(100);
    servo_arm.write(start_pos);
    delay(100);
    Serial.println("AngryCt: ");
    Serial.println(angryCt);
  }
  if (angryCt == 6)
  {
    delay(2000);
    servo_arm.write(target_pos);
    delay(250);
    servo_arm.write(start_pos);
    angryCt = 0;
    Serial.println("Returning to loop method!");
  }
}


// Error Method
void error()
{
  // Read switchState var
  switchState = digitalRead(10);
  // Loop while switch is still toggled if missed.
  while (switchState == LOW)
  {
    // ERROR HANDELING: If miss, move servo to target_pos deg and incriment by -2 as long as pos is > 36 & < 160 & switch state is LOW. 
    for (pos = target_pos; pos > 36 && pos < start_pos && switchState == LOW; pos -= 2)
    {
      // Read switchState 
      switchState = digitalRead(10);
      // Print to console
      Serial.println("Missed! Trying a little closer this time...");
      // Move servo to new pos
      servo_arm.write(pos);
      // Print new pos to console
      Serial.println("New Position: ");
      Serial.println(pos);
      Serial.println("Current Switch State: ");
      Serial.println(switchState);
      // Delay 1 second
      delay(100);
    }
    break;
  }
}


// Setup
void setup()
{
  // Initialize serial
  Serial.begin(9600);
  // Initialize servo
  servo_arm.attach(9);
  // Set pinMode
  pinMode(10, INPUT);
  digitalWrite(10, HIGH);
  // Move servo to start pos (160 deg)
  servo_arm.write(start_pos);
  // Print action to console
  Serial.println("Moving to start pos!");
  // Say hi?
  Serial.println("Calling Hi Method!");
  say_hi();
  delay(50);
  // Print starting loop to console
  Serial.println("Starting Loop...");
}

// Main Program:
void loop()
{ 
  // Read switchState var
  switchState = digitalRead(10);
  // If switchState is LOW (connected to GND)
  while (switchState == LOW)
  {
    Serial.println("Attempts: ");
    Serial.println(attempts);
    // Increment attempts var
    attempts++;
    
    while (switchState == LOW && attempts <= 3)
    {
      // Print to console
      Serial.println("Switch toggled! Attempting to turn off!");
      delay(50);
      // Move servo to target deg. (Known hit @ 38 deg)
      servo_arm.write(70);
      delay(750);
      servo_arm.write(target_pos); 
      delay(500);
      // If missed call Error Method!
      switchState = digitalRead(10);
      if (switchState == LOW)
      {
        Serial.println("Calling Error Method!");
        error();
      }
      // Reset servo to start pos
      Serial.println("Resetting arm!");
      servo_arm.write(start_pos);
      // Clear console
      Serial.flush();
      break;
    }
    if (attempts == 4 && switchState == LOW)
    {
      // Call angry method
      Serial.println("Calling Angry Method!");
      angry();
      delay(50);
      attempts = 0;
    }
    break;
  }
  delay(5);
}