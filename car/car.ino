#include <SoftwareSerial.h>
#include "./Car.hpp"
#include <stdlib.h>
#include <string.h>

SoftwareSerial BTSerial (9, 10);

enum State {
    NONE = 0,
    OFF = 1,
    FORWARDS = 2,
    BACKWARDS = 3,
    SLOW_TURN_LEFT = 4,
    SHARP_TURN_LEFT = 5,
    SLOW_TURN_RIGHT = 6,
    SHARP_TURN_RIGHT = 7,
};

State recieve_state() {
    BTSerial.println("Enter state:");
    while (!BTSerial.available()) {
        delay(30); // TOO SHORT?
    }
    char *state = (char *) memset(malloc(3), 3, '\0');
    for (
        char i = 0, ch = BTSerial.read();
        ch != '\0' && ch != -1 && i < 3;
        ch = BTSerial.read(), ++i
    ) state[i] = ch;
    BTSerial.println(atoi(state));
    return (State) atoi(state);
}

const uint8_t MOTOR_PINS[2][2] = {
    { 2, 3 },
    { 4, 5 },
};

Motor motors[2] = {
    Motor (MOTOR_PINS[0]),
    Motor (MOTOR_PINS[1]),
};

Car car (motors);

void setup() {
    Serial.begin(9600);
    BTSerial.begin(9600);
}

void loop() {
    switch (recieve_state()) {
        case OFF:
            car.off();
            break;
        case FORWARDS:
            car.forwards();
            break;
        case BACKWARDS:
            car.backwards();
            break;
        case SLOW_TURN_LEFT:
            car.turn_left(false);
            break;
        case SHARP_TURN_LEFT:
            car.turn_left(true);
            break;
        case SLOW_TURN_RIGHT:
            car.turn_right(false);
            break;
        case SHARP_TURN_RIGHT:
            car.turn_right(true);
            break;
        default:
            break;
    }
}
