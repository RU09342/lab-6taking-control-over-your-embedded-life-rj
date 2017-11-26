# Lab 6: Open Loop Systems
#### Jessica Wozniak & Ryan Hare
#### Created: 11/1/17
#### Last updated: 11/20/17

### Voltage Regulator

### Fan Control
The fan was controlled by a HEX value that was sent over UART. 
### Temperature Reading

### System Modeling

### Open Loop Control System


### Why the 5994?
The main reason we chose to use the MSP430FR5994 is because it has 2 timers. One timer (TimerB) was needed for the PWM of the fan and the other
 timer (TimerA) was needed for the ADC coversion/ enable. Also as mentioned above, UART was used to control the PWM of the fan. Using code from a previous
 milestone "Stranger Things Light Wall," the PWM control method for the RGB LED was easily manipulated to become the PWM of the fan. 
