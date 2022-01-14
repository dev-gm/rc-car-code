#include "Car.hpp"

Motor::Motor(const int enable_pin, const int input_pins[2]) {
    this->enable_pin = enable_pin;
    pinMode(enable_pin, OUTPUT);
    this->input_pins = input_pins;
    for (int i = 0; i < 2; ++i)
        pinMode(enable_pin[i], OUTPUT);
    state = OFF;
}
void Motor::off() {
    digitalWrite(enable_pin, LOW);
    state = OFF;
}
void Motor::on() {
    digitalWrite(enable_pin, HIGH);
    state = ON;
}
void Motor::brake() {
    for (int i = 0; i < 2; ++i)
        digitalWrite(input_pins[i], HIGH);
    state = Motor::State::BRAKE;
}
void Motor::forwards() {
    digitalWrite(input_pins[0], HIGH);
    digitalWrite(input_pins[1], LOW);
    state = Motor::State::FORWARDS;
}
void Motor::backwards() {
    digitalWrite(input_pins[0], LOW);
    digitalWrite(input_pins[1], HIGH);
    state = Motor::State::BACKWARDS;
}
Motor::~Motor() {
    off();
}

Car::Car(const int enable_pins[2], const int input_pins[2][2]) {
    Motor motor1 (enable_pins[0], input_pins[0]);
    Motor motor2 (enable_pins[1], input_pins[1]);
	motors = new Motor[2] { motor1, motor2 };
}
void Car::off() {
        for (int i = 0; i < 2; ++i)
            motors[i].off();
    }
void Car::brake() {
    for (int i = 0; i < 2; ++i)
        motors[i].brake();
}
void Car::forwards() {
    for (int i = 0; i < 2; ++i)
        motors[i].forwards();
}
void Car::backwards() {
    for (int i = 0; i < 2; ++i)
        motors[i].backwards();
}
void Car::turn_right(int magnitude) {
    motors[0].forwards();
    if (magnitude)
        motors[1].backwards();
    else
        motors[1].off();
}
void Car::turn_left(int magnitude) {
    if (magnitude)
        motors[0].backwards();
    else
        motors[0].off();
    motors[1].forwards();
}
Car::~Car() {
    off();
}
