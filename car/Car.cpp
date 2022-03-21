#include "./Car.hpp"
#include <string.h>
#include <stdlib.h>

Motor::Motor(const uint8_t input_pins[2]) {
    this->input_pins = input_pins;
    for (int i = 0; i < 2; ++i)
        pinMode(this->input_pins[i], OUTPUT);
}
void Motor::off() {
    char *str = (char *) memset(malloc(10), 10, '\0');
    sprintf(str, "%d, %d - ", this->input_pins[0], this->input_pins[1]);
    BTSerial.print(str);
    BTSerial.println("OFF");
    for (int i = 0; i < 2; ++i)
        digitalWrite(this->input_pins[i], LOW);
}
void Motor::forwards() {
    char *str = (char *) memset(malloc(10), 10, '\0');
    sprintf(str, "%d, %d - ", this->input_pins[0], this->input_pins[1]);
    BTSerial.print(str);
    BTSerial.println("FORWARDS");
    digitalWrite(this->input_pins[0], LOW);
    digitalWrite(this->input_pins[1], HIGH);
}
void Motor::backwards() {
    char *str = (char *) memset(malloc(10), 10, '\0');
    sprintf(str, "%d, %d - ", this->input_pins[0], this->input_pins[1]);
    BTSerial.print(str);
    BTSerial.println("BACKWARDS");
    digitalWrite(this->input_pins[0], HIGH);
    digitalWrite(this->input_pins[1], LOW);
}
Motor::~Motor() {
    off();
}

Car::Car(Motor motors[2]) {
  this->motors = motors;
}
void Car::off() {
    BTSerial.println("MAIN - OFF");
    for (int i = 0; i < 2; ++i)
        this->motors[i].off();
}
void Car::forwards() {
    BTSerial.println("MAIN - FORWARDS");
    for (int i = 0; i < 2; ++i)
        this->motors[i].forwards();
}
void Car::backwards() {
    BTSerial.println("MAIN - BACKWARDS");
    for (int i = 0; i < 2; ++i)
        this->motors[i].backwards();
}
void Car::turn_right(bool sharp) {
    BTSerial.println("MAIN - TURN RIGHT");
    this->motors[0].forwards();
    if (sharp)
        this->motors[1].backwards();
    else
        this->motors[1].off();
}
void Car::turn_left(bool sharp) {
    BTSerial.println("MAIN - TURN LEFT");
    this->motors[1].forwards();
    if (sharp)
        this->motors[0].backwards();
    else
        this->motors[0].off();
    this->motors[1].forwards();
}
Car::~Car() {
    off();
}
