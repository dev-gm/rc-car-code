#ifndef CAR_HPP
#define CAR_HPP
#include <Arduino.h>
#include <SoftwareSerial.h>

class Motor {
private:
    const uint8_t *input_pins;
public:
    Motor(const uint8_t input_pins[2]);
    void off();
    void forwards();
    void backwards();
    ~Motor();
};

class Car {
private:
	  Motor *motors;
public:
    Car(Motor motors[2]);
    void off();
    void forwards();
    void backwards();
    void turn_right(bool sharp);
    void turn_left(bool sharp);
    void turn_right_backwards(bool sharp);
    void turn_left_backwards(bool sharp);
    ~Car();
};

extern SoftwareSerial BTSerial;

#endif
