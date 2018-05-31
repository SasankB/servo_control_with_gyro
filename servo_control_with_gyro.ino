#include <Servo.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

int m1 = 2;// white clockwise
int m2 = 3;//white anti
int m3 = 44; // red clockwise
int m4 = 45;// red anti
int grnd = 51; 
Servo esc1, esc2, esc3, esc4;
float throttle, throttle_discard;
float prev_thr = 0;
int a = 0;
int16_t ax, ay, az;
int16_t gx, gy, gz;
MPU6050 accelgyro;

void setup() {
  // Initialize comm with IMU
  
  pinMode(grnd, OUTPUT);
  digitalWrite(grnd, LOW);
  
  Wire.begin();
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  // put your setup code here, to run once:
  esc1.attach(m1, 1000, 2000);
  esc2.attach(m2, 1000, 2000);
  esc3.attach(m3, 1000, 2000);
  esc4.attach(m4, 1000, 2000); 
  Serial.begin(9600);
  
}

void loop() {
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  // put your main code here, to run repeatedly:
  throttle_discard = getFloatFromSerialMonitor();
  a++;
  //   if (a>8000)
  //  {throttle_discard = 10;}
  if (throttle_discard != 0)
  {
    prev_thr = throttle_discard;
  }
  esc1.writeMicroseconds(  map((int)prev_thr , 0,100, 1000, 2000));
  Serial.println(prev_thr);
  esc2.writeMicroseconds(  map((int)prev_thr , 0,100, 1000, 2000));
  esc3.writeMicroseconds(  map((int)prev_thr , 0,100, 1000, 2000)); 
  esc4.writeMicroseconds(  map((int)prev_thr , 0,100, 1000, 2000));     
}

float getFloatFromSerialMonitor(){
  char inData[20];  
  float f=0;    
  int x=0;  
  if (x<1){  
  String str;   
  if (Serial.available()) {
    delay(100);
    int i=0;
    while (Serial.available() > 0) {
     char  inByte = Serial.read();
      str=str+inByte;
      inData[i]=inByte;
      i+=1;
      x=2;
    }
    f = atof(inData);
    memset(inData, 0, sizeof(inData));  
    throttle = f;
  }
  }//END WHILE X<1  
   return f; 
}

