#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

int main(void)
{
    // Setup code goes here
    DDRE = (1 << PE6);
    clock_prescale_set(clock_div_1);

    while (1) {
        // Main loop goes here
        PORTE |= _BV(PE6);
        _delay_ms(500);
        PORTE = 0;
        _delay_ms(500);
    }
}
