
//Jessica Wozniak & Ryan Hare
//Lab_6: Open Loop Control

//Input is RBG
#include <msp430.h> 

#define OUTPUT BIT5 //Pin 1.5 is the TB0CCR1 output pin.
#define ADC12 BIT4;         //define ADC12 to P1.4
#define LED1 BIT0           //define LED1 as BIT0
#define RXD BIT0            //define RXD as BIT0
#define TXD BIT1            //define TXD as BIT1

void TimerBInit(void);      //TIMERB initialization function
//void ADCInit(void);         //ADC initialization function
void GPIOInit(void);
//void TimerAInit(void);
void UARTInit(void);
void PinInit(void);

unsigned int in;
float voltage;
float tempC;
float tempF;


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
    PM5CTL0 &= ~LOCKLPM5; //Disable HIGH Z mode

    TimerBInit();           //initialize TIMERB
 //   TimerAInit();           //initialize TIMERB
 //   ADCInit();              //initialize ADC
    PinInit();
    GPIOInit();             //Initialize GPIO
    UARTInit();

    // Initialize the shared reference module
    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
 //   while(REFCTL0 & REFGENBUSY);            // If ref generator busy, WAIT
 //  REFCTL0 |= REFVSEL_0 + REFON;           // Enable internal 1.2V reference



 //   while(!(REFCTL0 & REFGENRDY));          // Wait for reference generator

    __enable_interrupt(); //Enable interrupts.

    __bis_SR_register(LPM0 + GIE); // Enter LPM0, interrupts enabled
    __no_operation(); // For debugger
 //   while(1)
//    {
 //       __delay_cycles(10000);
 //   }
}
//ADC ISR
/*#pragma vector=ADC12_B_VECTOR
__interrupt void ADC12ISR (void)
{
    in = ADC12MEM0;
    voltage = in * 0.000293;        //converts ADC to voltage
    tempC= voltage/ 0.01;           //converts voltage to Temp C
    tempF=((9*tempC)/5)+32;             //Temp C to Temp F

 //  while(!(UCA0IFG&UCTXIFG));           //waits for tx to be clear
  //  UCA0TXBUF = tempF;
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    ADC12CTL0 |= ADC12SC | ADC12ENC;
    __delay_cycles(10000);
}

void TimerAInit(void) {
    TA0CCTL0 = CCIE;
//    TA0CCTL1 = OUTMOD_3; //Set OUTMOD_3 (set/reset) for CCR1, CCR2
//    TA0CCR1 = 9000; //Red
    TA0CCR0 = 32000; //Set CCR0 for a ~1kHz clock.
    TA0CTL = TASSEL_2 + MC_1 + ID_3;
}
// Initialize ADC12_A
void ADCInit(void)
{
    ADC12CTL0 = ADC12SHT0_2 + ADC12ON;      // Set sample time
    ADC12CTL1 = ADC12SHP;                   // Enable sample timer
    ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
    ADC12MCTL0 = ADC12INCH_4 | ADC12VRSEL_4;// Vref = AVCC, Input
    ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt
    P1OUT = BIT0;

}
*/

void GPIOInit()
{
    P1OUT &= ~BIT0;                         // Clear LED to start
    P1DIR |= BIT0;                          // P1.0 output
    P1SEL1 |= ADC12;                         // Configure P1.4 for ADC
    P1SEL0 |= ADC12;
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            while(!(UCA0IFG&UCTXIFG));
            {
                TB0CCR1 = 255-UCA0RXBUF;      //duty cycle for RED
                    break;
            }


            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;

        default:
                break;

    }
}

void PinInit(void) {
    //For pin 1.4, 1.5, and 3.4, P1DIR = 1, P1SEL0 = 1, P1SEL1 = 0.
    P1DIR |= OUTPUT; //Pin 1.5
    P1SEL1 &= ~OUTPUT;
    P1SEL0 |= OUTPUT;

}

void TimerBInit(void) {
<<<<<<< HEAD
    TB0CCTL1 = OUTMOD_3; //Set OUTMOD_3 (set/reset) for CCR1, CCR2, CCR3

    //Set initial values for CCR1, CCR2, CCR3
    TB0CCR1 = 200; //Red

    TB0CCR0 = 255-1; //Set CCR0 for a ~1kHz clock.

=======
    TB0CCTL1 = OUTMOD_3; //Set OUTMOD_3 (set/reset) for CCR1
    //Set initial values for CCR1
    TB0CCR1 = 255;
    TB0CCR0 = 200; //Set CCR0 for a ~1kHz clock.
>>>>>>> 9820f253bbf2612044c988aeab6c70103e898f2b
    TB0CTL = TBSSEL_2 + MC_1; //Enable Timer B0 with SMCLK and up mode.
}
void UARTInit(void){

        CSCTL0_H = CSKEY_H;                         // Unlock CS registers
        CSCTL1 = DCOFSEL_3 | DCORSEL;               // Set DCO to 8MHz
        CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;       // Set all dividers
        CSCTL0_H = 0;                               // Lock CS registers

        P2SEL0 &= ~(BIT0 | BIT1);
        P2SEL1 |= BIT0+BIT1;

        // Configure USCI_A0 for UART mode
        UCA0CTLW0 = UCSWRST;                        // Put eUSCI in reset
        UCA0CTLW0 |= UCSSEL__SMCLK;                 // CLK = SMCLK
        UCA0BRW = 52;                               // 8000000/16/9600
        UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
        UCA0CTLW0 &= ~UCSWRST;                      // Initialize eUSCI
        UCA0IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt
}
