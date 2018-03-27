
#include<Wire.h>
#include<SoftwareSerial.h>
SoftwareSerial gsm(4,5);
String outMessage = "Fall occured";
String destinationNumber = "+918848257619";

const int MPU_addr=0x68;  // I2C address of the MPU-6050
float AcX;
float Xpos,dx;
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  gsm.begin(9600); 
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  Xpos=Xpos*0.9+AcX*0.1;
   dx=abs(AcX-Xpos);
   if(dx>15000)
   {
        gsm.print("AT+CMGF=1\r");
        delay(1000);
        gsm.println("AT + CMGS = \"" + destinationNumber +"\"");
        delay(1000);
        gsm.print(outMessage);
        delay(1000);
        gsm.write((char)26); //ctrl+z
        delay(1000);
        gsm.println("AT+CLTS=1");
        gsm.println("AT+CCLK?");

   }
  delay(333);
  
}
