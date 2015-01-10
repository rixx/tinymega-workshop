#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

volatile uint8_t DIRECTION = 0;
volatile uint16_t LARSON = 1;
void shift(void);

int main(void)
{
    // Setup code goes here
    clock_prescale_set(clock_div_1);
    DDRB = 7;

    // T/C setup code, see also data sheet section 14.10 page 121 ff
    TCCR1A = 0;                          //WGM{0,1,3} = 0, WGM2 = 1
    TCCR1C = 0;
                                         // 16MHz / 1024 = 15625 Ticks per second
    OCR1A = 1024;                        // 15625/2
    TIMSK1 = 1 << OCIE1A;                // enable OCIE1A
    TIFR1 = 0xff;                        //reset stuff

                                         // WGM{0,1,3} = 0, WGM2 = 1
    TCCR1B = (1 << WGM12) | (5 << CS10); // clock select CLK/1024=0b101, starts le clock

    // enable interrupts
    sei();

    while (1) {
    }
}

ISR(TIMER1_COMPA_vect)
{
    shift();
}

/*void larson(void) {
    if ((PINB == 1) || (PINB == 128)) {
        DIRECTION = !DIRECTION;
    }

    if (DIRECTION) {
        PORTB = PORTB << 1;
    } else {
        PORTB = PORTB >> 1;
    }
}*/

void shift(void) {
    uint8_t i, pin;

    if ((LARSON ==1) || (LARSON == (1 << 15))) {
        DIRECTION = !DIRECTION;
    }

    if (DIRECTION) {
        LARSON = LARSON << 1;
    } else {
        LARSON = LARSON >> 1;
    }

    for (i = 0; i < 16; i++) {
       pin = !(!(LARSON & (1 << i)));
       PORTB = (pin << PB0);

       PORTB |= (1 << PB1);
       PORTB &= ~(1 << PB1);

    }

    PORTB |= (1 << PB2);
    PORTB &= ~(1 << PB2);

}
