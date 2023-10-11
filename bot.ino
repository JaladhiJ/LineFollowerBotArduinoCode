#include <Servo.h>

// Pin connections for infrared sensors
const int leftSensorPin = A0;
const int rightSensorPin = A1;

// Motor control pins
const int leftMotorPin = 3;
const int rightMotorPin = 5;
const int left_plus = 2;
const int left_minus = 4;
const int right_plus = 7;
const int right_minus = 8;

// Servo control pins
const int microServoPin = 10;
const int macroServoPin = 9;

// Threshold values for sensor readings
const int sensorThreshold = 400;

// Speed value for motors
const int speed = 255;

// Servo objects
Servo microServo;
Servo macroServo;

// Rotation count for dumping mechanism
int rotationCount = 0;

void setup() {
// Set sensor pins as inputs
pinMode(leftSensorPin, INPUT);
pinMode(rightSensorPin, INPUT);
pinMode(left_plus, OUTPUT);
pinMode(right_plus, OUTPUT);
pinMode(left_minus, OUTPUT);
pinMode(right_minus, OUTPUT);

// Set motor pins as outputs
pinMode(leftMotorPin, OUTPUT);
pinMode(rightMotorPin, OUTPUT);

// Attach the servos to their respective pins
microServo.attach(microServoPin);
macroServo.attach(macroServoPin);

// Initialize servo positions
microServo.write(0);
macroServo.writeMicroseconds(1500);
}

void loop() {
// Read sensor values
int leftSensorValue = analogRead(leftSensorPin);
int rightSensorValue = analogRead(rightSensorPin);

// Check if the robot is on the line
bool onLine = (leftSensorValue > sensorThreshold) && (rightSensorValue >
sensorThreshold);

// Follow the line
if (onLine) {
// Move forward
moveForward(speed);
} else {
// Check which sensor has detected the line
bool leftLineDetected = (leftSensorValue < sensorThreshold) && (rightSensorValue >
sensorThreshold);
bool rightLineDetected = (rightSensorValue < sensorThreshold) && (leftSensorValue >
sensorThreshold);

// Adjust robot movement based on the detected line
if (leftLineDetected) {
turnLeft(speed);
//delay(500);
//stopMoving();
} else if (rightLineDetected) {
turnRight(speed);
//delay(500);
//stopMoving();
} else {
// Stop if the line is lost
stopMoving();
}
}

// Dumping mechanism activation
if (rotationCount == 0 && millis() > 3000) {
activateDumpingMechanism();
}
}

// Move the robot forward
void moveForward(int speed) {
analogWrite(leftMotorPin, speed);
analogWrite(rightMotorPin, speed);
digitalWrite(left_plus, LOW);
digitalWrite(left_minus, HIGH);
digitalWrite(right_plus, LOW);
digitalWrite(right_minus, HIGH);
}

// Turn the robot left
void turnLeft(int speed) {
analogWrite(leftMotorPin, speed);
analogWrite(rightMotorPin, speed);
digitalWrite(left_plus, HIGH);
digitalWrite(left_minus, LOW);
digitalWrite(right_plus, LOW);
digitalWrite(right_minus, HIGH-20);
}

// Turn the robot right
void turnRight(int speed) {
analogWrite(leftMotorPin, speed);
analogWrite(rightMotorPin, speed);

digitalWrite(left_plus, LOW);
digitalWrite(left_minus, HIGH);
digitalWrite(right_plus, HIGH-40 );
digitalWrite(right_minus, LOW);
}

// Stop the robot
void stopMoving() {
digitalWrite(leftMotorPin, LOW);
digitalWrite(rightMotorPin, LOW);
}

// Activate the dumping mechanism
void activateDumpingMechanism() {
// Rotate the micro servo to 90 degrees
microServo.write(90);
delay(2000);

// Rotate the macro servo for 5 rounds
macroServo.write(0); // Set servo to maximum speed in one direction

// Wait for a short delay to allow for one rotation
delay(5000);

// Stop the servo
macroServo.write(90);
// Rotate the micro servo back to 0 degrees
microServo.write(0);

// Increment the rotation count

rotationCount++;

// Exit the loop after 5 rotations
if (rotationCount >= 5) {
microServo.write(0); // Move micro servo to initial position
macroServo.detach(); // Detach macro servo
while (true) {
continue; // Loop indefinitely
}
}
}
