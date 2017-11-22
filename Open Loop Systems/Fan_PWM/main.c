
//Jessica Wozniak & Ryan Hare
//Lab_6: Open Loop Control

//Input is RBG
#include <msp430.h> 

#define OUTPUT BIT4 //Pin 1.4 is the TB0CCR1 output pin.

unsigned int count = 0;

int size;
unsigned int timeCount = 0;

void PinInit(void);
void TimerBInit(void);
void UARTInit(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //Disable HIGH Z mode

    PinInit(); //Initialize output pins.
    TimerBInit(); //Initialize Timer B.
    UARTInit(); //Initialize UART

    __enable_interrupt(); //Enable interrupts.

    __bis_SR_register(LPM0 + GIE); // Enter LPM0, interrupts enabled
    __no_operation(); // For debugger
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            while(!(UCA0IFG&UCTXIFG));

                TB0CCR1 = 255-UCA0RXBUF;      //duty cycle for FAN
                __no_operation();
                break;

        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;

        default:
                break;
    }
}

void PinInit(void) {
    //For pin 1.4, P1DIR = 1, P1SEL0 = 1, P1SEL1 = 0.
    P1DIR |= OUTPUT; //Pin 1.4
    P1SEL1 &= ~OUTPUT;
    P1SEL0 |= OUTPUT;
}

void TimerBInit(void) {
    TB0CCTL1 = OUTMOD_3; //Set OUTMOD_3 (set/reset) for CCR1
    //Set initial values for CCR1
    TB0CCR1 = 255;
    TB0CCR0 = 200; //Set CCR0 for a ~1kHz clock.
    TB0CTL = TBSSEL_2 + MC_1; //Enable Timer B0 with SMCLK and up mode.
}

void UARTInit(void){

        CSCTL0_H = CSKEY_H;                         // Unlock CS registers
        CSCTL1 = DCOFSEL_3 | DCORSEL;               // Set DCO to 8MHz
        CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;       // Set all dividers
        CSCTL0_H = 0;                               // Lock CS registers

        P2SEL0 &= ~(BIT0 | BIT1);                   //Configure pin 2.0 to RXD
        P2SEL1 |= BIT0+BIT1;                        //Configure pin 2.1 to TXD

        // Configure USCI_A0 for UART mode
        UCA0CTLW0 = UCSWRST;                        // Put eUSCI in reset
        UCA0CTLW0 |= UCSSEL__SMCLK;                 // CLK = SMCLK
        UCA0BRW = 52;                               // 8000000/16/9600
        UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
        UCA0CTLW0 &= ~UCSWRST;                      // Initialize eUSCI
        UCA0IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt
}
