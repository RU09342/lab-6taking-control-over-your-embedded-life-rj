//Jessica Wozniak & Ryan Hare
//Lab 5 Sensors: ADC10 MSP430G2553- Temperature Sensor
//Created: 11/7/17
//Last updated: 11/19/17

#include <msp430.h> 

void UARTInit(void);
void GPIOInit(void);

int i = 0;

int main(void)
{
      WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    GPIOInit();
    while(1)
    {

        for (i = 0; i < 255; i ++)
        {
            P3OUT = i;
        }
        for (i = 255; i > 0 ; i --)
        {
            P3OUT = i;
        }
    }
}
void GPIOInit()
{
    P3DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
}
