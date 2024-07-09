#include <SoftwareSerial.h>
#include <TinyGPS++.h>


const int sensorPin = A0;     
const int motorEnablePin = 9;  
const int motorInput1 = 8;     
const int motorInput2 = 7;     


const int alcoholThreshold = 200;  


SoftwareSerial gsmSerial(10, 11);  
SoftwareSerial gpsSerial(4, 3);    

TinyGPSPlus gps;

void setup() {
 
  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  
 
  gsmSerial.begin(9600);
  delay(1000);
  gsmSerial.println("AT");
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); 
  delay(1000);
  
  
  gpsSerial.begin(9600);
}

void loop() {
 
  int alcoholValue = analogRead(sensorPin);

 
  if (alcoholValue > alcoholThreshold) {
 
    digitalWrite(motorEnablePin, LOW);
    digitalWrite(motorInput1, LOW);
    digitalWrite(motorInput2, LOW);
    
   
    if (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
      if (gps.location.isValid()) {
        
        sendSMS("Danger: Alcohol detected! Location: Lat " + String(gps.location.lat(), 6) +
                ", Long " + String(gps.location.lng(), 6));
      }
    }
  } else {
    
    digitalWrite(motorEnablePin, HIGH);
    digitalWrite(motorInput1, HIGH);
    digitalWrite(motorInput2, LOW);
  }

  
}


void sendSMS(String message) {
  gsmSerial.println("AT+CMGS=\"+1234567890\""); 
  delay(1000);
  gsmSerial.println(message);
  delay(1000);
  gsmSerial.println((char)26);  
  delay(1000);
}
