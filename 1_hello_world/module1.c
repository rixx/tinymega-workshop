#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

void setup(void);

int main(void){

    setup();

    while (1) {
        PORTE |= (1 << PE6);
        _delay_ms(500);
        PORTE &= ~_BV(PE6);                 // _BV is a bit mask
        _delay_ms(500);
    }

    // never return from main
}

void setup(){
    DDRE = (1 << PE6);                      // set PE6 to output mode
    clock_prescale_set(clock_div_1);        // set clock prescale to 1 (default: 8)
}
