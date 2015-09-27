#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

void shift(void);
void setup(void);
void dark(void);
void dark_led(void);
void bright_led(void);
void bright(uint16_t);
void darkness(uint8_t);

volatile uint8_t DIRECTION = 0;
volatile uint16_t LARSON = 1;
uint8_t dark_max = 255;


int main(void)
{
    setup();
    cli();
    while (1) {
        for (uint8_t i = 1; i <= dark_max; i++) {
            darkness(i);
        }
        for (uint8_t i = dark_max - 1; i > 0; i--) {
            darkness(i);
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    shift();
}

void shift(void) {
    if ((LARSON ==1) || (LARSON == (1 << 15))) {
        DIRECTION = !DIRECTION;
    }

    if (DIRECTION) {
        LARSON = LARSON << 1;
    } else {
        LARSON = LARSON >> 1;
    }
}

void bright(uint16_t larson) {
    SPCR |= (1<<MSTR);
    SPDR = (uint8_t)((larson & 0xff00)>>8);
    while (!(SPSR & (1<<SPIF)));

    SPCR |= (1<<MSTR);
    SPDR = (uint8_t)(larson & 0x00ff);
    while (!(SPSR & (1<<SPIF)));
    PORTB |= _BV(PB0);
    PORTB &= ~_BV(PB0);
}

void dark(void) {
    SPCR |= (1<<MSTR);
    SPDR = 0x00;
    while (!(SPSR & (1<<SPIF)));

    SPCR |= (1<<MSTR);
    SPDR = 0x00;
    while (!(SPSR & (1<<SPIF)));
    PORTB |= _BV(PB0);
    PORTB &= ~_BV(PB0);
}

void bright_led(void) {
    PORTE |= _BV(PE6);
}

void dark_led(void) {
    PORTE &= ~_BV(PE6);
}

void darkness(uint8_t factor) {
    for (uint8_t j = 0; j < 16; j++) {
        bright_led();
        for (uint8_t i = 0; i< (dark_max-factor); i++)
            _delay_us(50);

        dark_led();
        for (uint8_t i = 0; i< factor; i++)
            _delay_us(50);
    }
}
void setup()
{
    // Setup code goes here
    clock_prescale_set(clock_div_1);
    DDRB = 7;
    DDRE |= _BV(PE6);
    dark_led();

    // T/C setup code, see also data sheet section 14.10 page 121 ff
    // We will use Timer 1
                                         // until where do we count until interrupting?
                                         // 16MHz / 1024 = 15625 Ticks per second
    OCR1A = 8000;                        // 15625/2 for 0.5 seconds

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

    PRR0 &= ~(1 << PRSPI);                           // enable SPI by writing 0 to the Power Reduction SPI bit (p179)
    DDRB = (1<<DDB0)|(1<<DDB1)|(1<<DDB2);            // set SCK (PB1) and MOSI(PB2) to output, all others to input
    SPCR = (1 << SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0); // enable SPI. add SPIE to enable the SPI interrupt

    // enable interrupts
    //sei();
}
