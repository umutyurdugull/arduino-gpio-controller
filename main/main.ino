
#include <stdint.h>
#include <stddef.h>

// DDR: pin input/output ayari
// PORT: output degeri veya input pull-up kontrolu

#define SET_MASK(REG, MASK)    ((REG) |= (MASK))
#define CLEAR_MASK(REG, MASK)  ((REG) &= ~(MASK))
#define TOGGLE_MASK(REG, MASK) ((REG) ^= (MASK))
#define READ_MASK(REG, MASK)   (((REG) & (MASK)) != 0)

typedef struct {
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    uint8_t bitmask;
} PinMap;

PinMap getPinMap(uint8_t pin) {
    PinMap p = {NULL, NULL, 0};

    // Arduino Uno: D0-D7 -> PORTD, bit 0-7
    if (pin <= 7) {
        p.ddr = &DDRD;
        p.port = &PORTD;
        p.bitmask = (uint8_t)(1U << pin);
    }
    // Arduino Uno: D8-D13 -> PORTB, bit 0-5
    else if (pin <= 13) {
        p.ddr = &DDRB;
        p.port = &PORTB;
        p.bitmask = (uint8_t)(1U << (pin - 8));
    }

    return p;
}

void myPinMode(uint8_t pin, uint8_t mode) {
    PinMap p = getPinMap(pin);

    if (p.ddr == NULL) return;

    if (mode == 1) {  // OUTPUT
        SET_MASK(*(p.ddr), p.bitmask);
    } else {          // INPUT
        CLEAR_MASK(*(p.ddr), p.bitmask);
    }
}

void myDigitalWrite(uint8_t pin, uint8_t val) {
    PinMap p = getPinMap(pin);

    if (p.port == NULL) return;

    if (val == 1) {   // HIGH
        SET_MASK(*(p.port), p.bitmask);
    } else {          // LOW
        CLEAR_MASK(*(p.port), p.bitmask);
    }
}

void setup() {
    myPinMode(7, 1);
}

void loop() {
    myDigitalWrite(7, 1);
    delay(500);

    myDigitalWrite(7, 0);
    delay(500);
}
