busy waiting sucks.

#T/C modules
we have 4 timer/counter modules, counting up at (divided) clock freq, which you have to start yourself
use compare registers to trigger actions
see page 101 in the [data sheet](http://www.atmel.com/Images/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)
slowest divided clock frequency: 8MHz/1024=7.8K in einer halben Sekunde, *2= 15khz

#IRQs
interrupts are asynchronous events triggered by hardware, can be enabled/disabled globally: `sei()`, `cli()` (SEt and CLear)
sources: timer overflows/compares, pin toggles …

#Interrupt Handlers
(ISR, Interrupt Service Routine)

    #include <avr/interrupt.h>

    ISR(TIMER0_COMPA_vect)
    {
        // Code goes here
    } 


#RULES:

* shared global variables (for main loop) to be declared `volatile`
