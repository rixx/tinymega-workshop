#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>

int main(void)
{
    // Setup code goes here
    clock_prescale_set(clock_div_1);
    DDRE = (1 << PE6);

    // T/C setup code, see also data sheet section 14.10 page 121 ff
    TCCR1A = 0;                          //WGM{0,1,3} = 0, WGM2 = 1
    TCCR1C = 0;
                                         // 16MHz / 1024 = 15625 Ticks per second
    OCR1A = 7812;                        // 15625/2
    TIMSK1 = 1 << OCIE1A;                // enable OCIE1A
    TIFR1 = 0xff;                        //reset stuff

                                         // WGM{0,1,3} = 0, WGM2 = 1
    TCCR1B = (1 << WGM12) | (5 << CS10); // clock select CLK/1024=0b101, starts le clock

    // enable interrupts
    sei();

    while (1) {
        /* Main loop goes here
        PORTE |= _BV(PE6);
        _delay_ms(500);
        PORTE = 0;
        _delay_ms(500);*/
    }
}

ISR(TIMER1_COMPA_vect)
{
    // alt: PINE = 1 << PE6; // 1 zyklus statt 3
    PORTE ^= _BV(PE6);
}

