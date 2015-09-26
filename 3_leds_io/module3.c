#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

void setup(void);

volatile uint8_t DIRECTION = 0;

int main(void)
{
    setup();

    while (1) {
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (PINB == _BV(PB0) || PINB == _BV(PB7)) {
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
    clock_prescale_set(clock_div_1);
    DDRB = 0xff;
    PORTB = 1;

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

    sei();                               // enable interrupts
}
