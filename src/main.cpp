// Includes
#include <Arduino.h>
#include <Servo.h>


// Variables
Servo servo_arm; // Set servo_arm as Servo
int pos = 90; // Default start positon
int target_pos = 38; // Replace with target position
int start_pos = 160; // Replace with start position
int peek_pos = 70; // Replace with peek position
int switchState; // Int for switch state
int attempts; // Int for attempts
int randomNumber; // Int for a random number


// Slow Move Variables
int increment = 4; // Step increment for slow_move
int del = 50; // Delay between steps
int prev_pos; // Previous pos to start slow move
int new_pos; // New position to move to


// Hi Method
void say_hi()
{
  Serial.println("Showing Hi Method!");
  servo_arm.write(peek_pos);
  delay(1500);
  servo_arm.write(start_pos);
}

// Angry Method
void angry(int angryCt = 0)
{
  Serial.println("Showing Angry Method!");
  servo_arm.write(peek_pos);
  delay(500);
  servo_arm.write(start_pos);
  delay(300);
  for(angryCt = 0; angryCt <= 5; angryCt++)
  {
    servo_arm.write(peek_pos);
    delay(100);
    servo_arm.write(start_pos);
    delay(100);
    Serial.println("AngryCt: ");
    Serial.println(angryCt);
  }
  for (angryCt = 6; angryCt >= 6 && angryCt <= 9; angryCt++)
  {
    delay(1500);
    servo_arm.write(target_pos);
    delay(1000);
    servo_arm.write(start_pos);
  }
  servo_arm.write(peek_pos);
  delay(1500);
  servo_arm.write(start_pos);
  Serial.println("Returning to loop method!");
}

// Frustrated Method
void frustrated(int frustratedCt = 0)
{
  Serial.println("Showing Frustrated Method!");
  servo_arm.write(peek_pos);
  delay(500);
  servo_arm.write(start_pos);
  delay(300);
  for(frustratedCt = 0; frustratedCt <= 3; frustratedCt++)
  {
    servo_arm.write(peek_pos);
    delay(100);
    servo_arm.write(start_pos);
    delay(100);
    Serial.println("frustratedCt: ");
    Serial.println(frustratedCt);
  }
  if (frustratedCt == 4)
  {
    delay(500);
    servo_arm.write(target_pos);
    delay(1500);
    servo_arm.write(start_pos);
    delay(1000);
    servo_arm.write(target_pos);
    delay(2000);
    servo_arm.write(start_pos);
    frustratedCt = 0;
    Serial.println("Returning to loop method!");
  }
  servo_arm.write(peek_pos);
  delay(1500);
  servo_arm.write(start_pos);
}

// Slow Move to target
void slow_move()
{
  Serial.println("Starting slow_move!");
  switchState = digitalRead(10);
  prev_pos = servo_arm.read();
  // Loop while switch is still toggled if missed.
  for (new_pos = prev_pos; switchState == LOW; new_pos -= increment)
  {
    Serial.println("Moving to new_pos:!");
    Serial.println(new_pos);
    servo_arm.write(new_pos);
    Serial.println("Delaying...");
    Serial.println(del);
    delay(del);
    switchState = digitalRead(10);
  }
  servo_arm.write(target_pos);
  delay(50);
  servo_arm.write(start_pos);
  delay(500);
  servo_arm.write(peek_pos);
  delay(1500);
  servo_arm.write(start_pos);
  Serial.println("Returning to loop method!");
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
      // Delay
      delay(100);
    }
    delay(500);
    servo_arm.write(start_pos);
    break;
  }
}

// Setup
void setup()
{
  // Initialize serial
  Serial.begin(9600);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

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
      servo_arm.write(peek_pos);
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
    while (switchState == LOW && attempts >= 4)
    {
      // Move to peek position & generate ranom number then hold.
      servo_arm.write(peek_pos);
      // Generate randon number between 1 & 5
      randomNumber = random(1,5);
      Serial.println("Random Number is: ");
      Serial.println(randomNumber);
      delay(1000);
      // Call slow_move method if randomNumber <= 3
      if (randomNumber <= 3)
      {
        Serial.println("randomNumber <= 2: Calling Slow Move Method!");
        slow_move();
        delay(50);
        attempts = 2;
        if (switchState == LOW)
        {
          Serial.println("Calling Error Method!");
          error();
        }
      }      
      // Call frustrated method if randomNumber = 4
      if (randomNumber == 4)
      {
        Serial.println("randomNumber <= 4: Calling Frustrated Method!");
        frustrated();
        delay(50);
        if (switchState == LOW)
        {
          Serial.println("Calling Error Method!");
          error();
        }
      }
      // Call angry method if randomNumber == 5
      if (randomNumber == 5)
      {
        Serial.println("randomNumber >= 5:Calling Angry Method!");
        angry();
        delay(50);
        attempts = 0;
        if (switchState == LOW)
        {
          Serial.println("Calling Error Method!");
          error();
        }
      }
      else
      {
        if (switchState == LOW)
        {
          Serial.println("Calling Error Method!");
          error();
        }
        break;
      }
      
      break;
    }
    break;
  }
  delay(5);
}