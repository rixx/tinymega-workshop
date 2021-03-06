#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

void shift(void);
void setup(void);

volatile uint8_t DIRECTION = 0;
volatile uint16_t LARSON = 1;

int main(void)
{
    setup();

    while (1) {
    }
}

ISR(TIMER1_COMPA_vect)
{
    shift();
}

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

void setup()
{
    // Setup code goes here
    clock_prescale_set(clock_div_1);
    DDRB = 7;

    // T/C setup code, see also data sheet section 14.10 page 121 ff
    // We will use Timer 1
                                         // until where do we count until interrupting?
                                         // 16MHz / 1024 = 15625 Ticks per second
    OCR1A = 1024;                        // 15625/2 for 0.5 seconds

                                         // which interrupt do we use? 
    TIMSK1 = 1 << OCIE1A;                // enable OCIE1A (output compare interrupt enable timer interrupt 1a)

    TIFR1 = 0xff;                        // reset all pending interrupt requests to be safe

    // Timer/Counter control register A,B,C for Timer 1
    // WGM{0,1,3} = 0, WGM2 = 1 for CTC mode
    // we set TCCR1B last, because this will start the clock, see below
    TCCR1A = 0;                          // WGM11=0, WGM10=0
    TCCR1C = 0;                          // ignored
                                         // Clock Select 1 = 5 (clk/1024)
    TCCR1B = (1 << WGM12) | (5 << CS10); // WGM2=1, Clock Select 1 = 5 (clk/1024), p134, s14.10.4

    // enable interrupts
    sei();
}
