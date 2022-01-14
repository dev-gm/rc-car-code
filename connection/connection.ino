#include <SoftwareSerial.h>
 
SoftwareSerial BT (10, 11);
int ledpin = 13;
int BluetoothData;
 
void setup() {
  BT.begin(9600);
  Serial.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(ledpin,OUTPUT);
}
 
void loop() {
  	if (BT.available()) {
    	BluetoothData = BT.read();
    	if(BluetoothData == '1') {   
      		digitalWrite(ledpin,1);
      		Serial.println("LED  On D13 ON ! ");
    	}
    	if (BluetoothData == '0') {
      		digitalWrite(ledpin,0);
      		Serial.println("LED  On D13 Off ! ");
    	}
  	}
	delay(100);
}
