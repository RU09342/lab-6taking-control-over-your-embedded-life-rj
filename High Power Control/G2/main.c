#include <msp430.h>

#define ADC12 BIT4          //define ADC12 as BIT4
#define LED1 BIT0           //define LED1 as BIT0
#define RXD BIT0            //define RXD as BIT0
#define TXD BIT1            //define TXD as BIT1

void TimerInit(void);      //Timer function
void ADC12Init(void);      //ADC10 function
void UARTInit(void);       //UART function
void GPIOInit(void);       //GPIO function
//void ClockInit(void);      //clock function

unsigned int in;
float voltage;
float tempC;
float tempF;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    ADC12Init();                  //ADC10 Function
    TimerInit();                  //Timer Function
  //  UARTInit();                   //UART Function
    GPIOInit();                   //GPIO Function
 //   ClockInit();                  //Clock function


    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(GIE); // LPM0, ADC10_ISR will force exit
    while(1){
    }
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    ADC12CTL0 |= ADC12SC | ADC12ENC;         //sample/ enable
}

#pragma vector = ADC12_B_VECTOR
__interrupt void ADC12_ISR(void)
{
    in = ADC12MEM0;
    voltage = in * 0.0033;        //converts ADC to voltage
    tempC= voltage/ 0.01;           //converts voltage to Temp C
    tempF=((9*tempC)/5)+32;             //Temp C to Temp F

//    while(!(UCA0IFG&UCTXIFG));      //wait for TX to clear
//    UCA0TXBUF = tempF;              //send to TX
}
void TimerInit()
{
    TA0CCTL0 = CCIE;                           // Enable interrupt
    TA0CCR0 = 4096-1;                           // PWM Period
    TA0CCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TA0CCR1 = 256;                             // TACCR1 PWM Duty Cycle
    TA0CTL = TASSEL_1 + MC_1 + ID_3;                   // ACLK, CONT MODE
}
void ADC12Init()
{

// Configure ADC12
ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
ADC12CTL1 = ADC12SHP;                   // ADCCLK = MODOSC; sampling timer
ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt
ADC12MCTL0 |= ADC12INCH_4 | ADC12VRSEL_0; // A1 ADC input select; Vref=1.2V
}
void GPIOInit()
{
    // GPIO Setup
     P1OUT &= ~BIT0;                         // Clear LED to start
     P1DIR |= BIT0;                          // P1.0 output
     P1SEL1 |= BIT1;                         // Configure P1.1 for ADC
     P1SEL0 |= BIT1;
}
