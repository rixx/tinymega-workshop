#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

volatile uint8_t DIRECTION = 0;
void larson(void);

int main(void)
{
    // Setup code goes here
    clock_prescale_set(clock_div_1);
    DDRB = 255;
    PORTB = 1;

    PORTF = 1; //pull up
    DDRF = 0;

    // T/C setup code, see also data sheet section 14.10 page 121 ff
    /*TCCR1A = 0;                          //WGM{0,1,3} = 0, WGM2 = 1
    TCCR1C = 0;
                                         // 16MHz / 1024 = 15625 Ticks per second
    OCR1A = 1024;                        // 15625/2
    TIMSK1 = 1 << OCIE1A;                // enable OCIE1A
    TIFR1 = 0xff;                        //reset stuff

                                         // WGM{0,1,3} = 0, WGM2 = 1
    TCCR1B = (1 << WGM12) | (5 << CS10); // clock select CLK/1024=0b101, starts le clock

    // enable interrupts
    sei();*/

    while (1) {
        // PINF == 1, weil button erstmal nicht gedrückt
        while (PINF){};
        larson();
        while (!(PINF & 1)) {};
    }
}

/*ISR(TIMER1_COMPA_vect)
{
}*/

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
