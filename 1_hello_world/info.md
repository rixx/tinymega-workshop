#Intro

tristate: input only, and only when no pull-up is there (obv)

    [0,1]:0
    [2,5]:1

GPIO ports BCDEF + 0..7 -- BUT not all ports have all bits available!

Dinge auf einem Port können synchron umgeschaltet werden, b/c memory mapping
Each port has three registers: PORTx (ausgabe, pull-up [default 0]), PINx (lesen), DDRx (config)
nach reset: alles input ohne pullup

merke: AVR-Konstanten sind immer shift-werte. _BV macht bitmaske mit ner 1

--------------------------

    #include <avr/io.h>

        {
            // After reset, all pins default to input, no pull-up
            PORTB = (1 << PB3);       // Enable pull-up on B3
            DDRB = (1 << PB2);        // Set B2 to output, all others to input
            if (!(PINB & _BV(PB3)))   // If something pulls B3 down...
                PORTB |= _BV(PB2);    // Set B2 to high, keep rest as is
        }

------------------------

    #define F_CPU 16000000UL  // CPU clock is 16 MHz
    #include <util/delay.h>

        {
            _delay_ms(1000);  // waaaait a sec...
            _delay_us(10);    // and a bit more
        }

-------------------------

clock divider at /8; fix by doing

    #include <avr/power.h>
    clock_prescale_set(clock_div_1);


