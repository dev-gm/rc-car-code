#include <SoftwareSerial.h>

#define RXD 6
#define TXD 9
#define ECHO 10
#define TRIG 11
#define ENA 3
#define ENB 5
#define IN1 2 // IN1
#define IN2 4 // IN2
#define IN3 7 // IN3
#define IN4 8 // IN4

SoftwareSerial BT (TXD, RXD);

void setup() {
	BT.begin(9600);
	pinMode(ENA, OUTPUT);
	pinMode(ENB, OUTPUT);
	pinMode(ECHO, INPUT);
	pinMode(TRIG, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
}

long monitor_sensor() {
	static long duration;
	digitalWrite(TRIG, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);
	duration = pulseIn(ECHO, HIGH);
	return duration / 29 / 2;
}

int get_input() {
	if (BT.available()) {
		char out[100];
		int i = 0;
		char ch;
		while ((ch = BT.read()) != '\n' && ch > 0)
			out[i++] = ch;
		out[i] = '\0';
		return atoi(out);
	}
	return -1;
}

void loop() {
	switch (get_input()) {
		case -1:
			return;
			break;
		case 0: // stop
			BT.println("STOP");
			analogWrite(ENA, 0);
			analogWrite(ENB, 0);
			break;
		case 1: // f
			BT.println("FORWARDS");
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, LOW);
			break;
		case 2: // b
			BT.println("BACKWARDS");
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
			break;
		case 3: // left f
			BT.println("LEFT FORWARDS");
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, LOW);
			break;
		case 4: // right f
			BT.println("RIGHT FORWARDS");
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
			break;
		case 5: // left b
			BT.println("LEFT BACKWARDS");
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
			break;
		case 6: // right b
			BT.println("RIGHT BACKWARDS");
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
			break;
		case 7:
			BT.println("CHANGE MAGNITUDE");
			int input = get_input();
			BT.print("BY ");
			BT.println(input);
			analogWrite(ENA, input);
			analogWrite(ENB, input);
			break;
	}
	BT.print("Ultrasonic Sensor: ");
	BT.println(monitor_sensor());
	delay(750);
}
