#include <Servo.h>
Servo myservoX;  // create servo object to control a servo
Servo myservoY;  // create servo object to control a servo

#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
int den=7;
int x, y;
int gocservo_x, gocservo_y;

int tempPin = A0; // Analog pin connected to the thermistor
int greenPin = 5; 
int yellowPin = 6; 
int redPin = 7; 


float Vo;
float R1 = 10000;
float logR2, R2, T, temp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float tempC;

void setup() 
{
  Serial.begin(9600);

  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  pinMode(tempPin, INPUT);

  pinMode(den,OUTPUT);
  digitalWrite(den, LOW);

  myservoX.attach(4);   // connect servo at X axis(roll) to pin 9  
  myservoY.attach(3);  // connect servo at Y axis(pitch) to pin 10
  myservoX.write(90);   //set starting position of both servo motors at 90 degree
  myservoY.write(90);
}
float get_temp() {
  Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / Vo - 1.0); // R2= R1*(Vin/Vout - 1)
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  return T - 273.15; //độ C
}
void loop() 
{
  mpu6050.update();
  x = mpu6050.getAngleX();
  y = mpu6050.getAngleY();

  gocservo_x = 90-x;
  gocservo_y = 90-y;
  
  myservoX.write(gocservo_x);
  myservoY.write(gocservo_y);
  
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());

  tempC = get_temp(); // Convert the temperature value to Celsius

  if (tempC <= 20) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, LOW);
  } else if (tempC <= 50 && tempC>20 ) {
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(redPin, LOW);
  } else if (tempC > 50) {
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, HIGH);
  }
}