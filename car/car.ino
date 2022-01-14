#include <SoftwareSerial.h>
#include "Car.hpp"

enum State {
    ON,
    OFF,
    BRAKE,
    FORWARDS,
    BACKWARDS,
    SLOW_TURN_LEFT,
    SHARP_TURN_LEFT,
    SLOW_TURN_RIGHT,
    SHARP_TURN_RIGHT,
};

State recieve_state() {
    while (!BTSerial.available()) {
        delay(15); // TOO SHORT?
    }
    return (State) *BTSerial.read();
}

const int MOTOR_ENABLE_PINS[2] = { 5, 6 };

const int MOTOR_INPUT_PINS[2][2] = {
    { 1, 2 },
    { 3, 4 },
};

Car car (MOTOR_ENABLE_PINS, MOTOR_INPUT_PINS);

const int BT_PORTS[2] = { 7, 8 };

SoftwareSerial BTSerial (BT_PORTS[0], BT_PORTS[1]);

void setup() {
    Serial.begin(9600);
    BTSerial.begin(9600);
    car.on();
}

void loop() {
    switch (recieve_state()) {
        case ON:
            car.on();
            break;
        case OFF:
            car.off();
            break;
        case BRAKE:
            car.brake();
            break;
        case FORWARDS:
            car.forwards();
            break;
        case BACKWARDS:
            car.backwards();
            break;
        case SLOW_TURN_LEFT:
            car.turn_left(0);
            break;
        case SHARP_TURN_LEFT:
            car.turn_left(1);
            break;
        case SLOW_TURN_RIGHT:
            car.turn_left(0);
            break;
        case SHARP_TURN_RIGHT:
            car.turn_left(1);
            break;
    }
}
