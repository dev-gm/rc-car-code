#ifndef Car_hpp
#define Car_hpp
#include <Arduino.h>

class Motor {
private:
    static int enable_pin;
    const static int *input_pins;
    enum State {
        ON, // doing nothing
        OFF,
        BRAKE,
        FORWARDS,
        BACKWARDS,
    } state;
public:
    Motor(const int enable_pin, const int input_pins[2]);
    void off();
    void on();
    void brake();
    void forwards();
    void backwards();
    ~Motor();
};

class Car {
private:
	Motor *motors;
public:
    Car(const int enable_pins[2], const int input_pins[2][2]);
    void off();
    void on();
    void brake();
    void forwards();
    void backwards();
    void turn_right(int magnitude);
    void turn_left(int magnitude);
    ~Car();
};

#endif
