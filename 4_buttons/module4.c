#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

void setup(void);

volatile uint8_t DIRECTION = 0;
void larson(void);

int main(void)
{

    setup();

    while (1) {
        while (PINF){};

        larson();

        uint8_t bounce = 1;
        while (bounce) {
            bounce = bounce << 1;
            bounce |= (!(PINF & 1));
            _delay_ms(5);
        }
    }
}

void larson(void) {
    if ((PINB == 1) || (PINB == 128)) {
        DIRECTION = !DIRECTION;
    }

    if (DIRECTION) {
        PORTB = PORTB << 1;
    } else {
        PORTB = PORTB >> 1;
    }
}

void setup()
{
    // Setup code goes here
    clock_prescale_set(clock_div_1);
    DDRB = 0xff;
    DDRF = 0x00;

    PORTB = 1;
    PORTF = 1; //pull up
    DDRF = 0;
}
