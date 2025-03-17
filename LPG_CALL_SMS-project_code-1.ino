
#include <Servo.h>
#define gas_sensor  A0 //MQ-2 Sensor A0 pin
#define buzzer      8  //Buzzer red wire
#define fan         10 //CPU Fan red wire

#include <SoftwareSerial.h>
SoftwareSerial sim900A(2, 3);//arduino pin 2 connected to TX pin of GSM module
                             //arduino pin 3 connected to RX pin of GSM module 
                             //arduino GND pin connected to GND pin of GSM module
int sensor_value;
Servo servo;
String phoneNumber = "+91XXXXXXXXXX"; //change mobile number here to receive call/sms
String message = "               ALERT!!    \nGAS LEAKAGE DETECTED \nREGULATOR TURNED OFF"; //change content if required

void setup() {
  Serial.begin(9600);
  sim900A.begin(9600);
  delay(1000);
  sim900A.println("AT+CMGF=1");
  delay(1000);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  servo.attach(9); //servo motor signal pin attached to arduino pin 9
  servo.write(12); //regulator on
}

void loop() {
  sensor_value = analogRead(gas_sensor);
  Serial.println(sensor_value);
  if (sensor_value > 650) // change this value according to sensitivity of gas
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(fan, HIGH);
    servo.write(140);
    delay(1000);
    servo.detach();
    sendSMS(phoneNumber, message);
    makeCall(phoneNumber);
    servo.attach(9);
    servo.write(12);
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(fan, LOW);
    servo.write(12);
  }
  delay(50);
}




void sendSMS(String phoneNumber, String message) {
  sim900A.print("AT+CMGS=\"" + phoneNumber + "\"\r");
  delay(1000);
  sim900A.print(message);
  delay(1000);
  sim900A.print((char)26);
  delay(1000);

    delay(3000); 
}


void makeCall(String phoneNumber) {
  sim900A.println("ATD" + phoneNumber + ";");
  delay(20000); 
  sim900A.println("ATH");
  delay(1000); 
}