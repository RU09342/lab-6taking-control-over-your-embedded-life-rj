# Lab 6: Precision Control
### Jessica Wozniak & Ryan Hare
### Created: 11/1/17
### Last updated: 11/18/17

Some applications require large amounts of voltage or current, so switching techniques must be used in order to provide the desired output. 
Other cases however require a finer control over the voltage or current going into them (some even require a control over resistance). So far you 
have looked at PWM to control the brightness of an LED, is there a way to use this to output a specified voltage or current, or even a specific waveform?

## PWM Part 2
Since you already have the code to perform PWM, then really, the software side of this part of the lab is fairly easy. You need to design a system 
which can take in a PWM duty cycle over something like UART (or you could have your system read in the position of a potentiometer), and produce that 
signal on a GPIO. The interesting part comes in when I want the output of your system to be an Analog voltage. In this case, a PWM with a 50% duty cycle 
should produce roughly Vcc/2 volts. This part of the lab should be done with the MSP430F5529 and the physical circuit should be constructed of an active 
Low-Pass Filter.

## R2R DAC
An digital-to-analog converter 
### Schematic
![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/R2R_Ladder.PNG)
### Software
On the MSP430F5529, Port 3 is the only port that has all pins (0-7) avaible for output. The following line of code initializes Port 3.
```C
 P3DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
```
To show the proper output was being produced, the following code sets up a counter that counts up to 255 and back down again.  
```C
 while(1){

        for (i = 0; i < 255; i ++)
        {
            P3OUT = i;
        }
        for (i = 255; i > 0 ; i --)
        {
            P3OUT = i;
        }
```
This forms a triangle wave.
![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/r2r_Triangle.png)
When you zoom in closely on the oscilliscope, you can see the "staircase" formed by each voltage on the output of the DAC. 
![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/r2r_zoomed_in.png)

## Loading Effects
Obviously you are going to be making this type of circuitry to drive something. This introduces the idea of loading effect, wherein your circuit 
will perform differently based on what is going to be attached to it. For each of these implementations, try placing a variety of resistors from 
100 ohms up to see what happens to your output signal and comment on what is happening.

## Deliverables
Along with what was asked in each of the parts above, for each implementation, you need to generate at least one triangle wave from your microntroller. 
This can be done by simply incrementing and decrementing values that are being sent to your circuit. You need to measure the output of each one of these 
along with taking the FFT on the scope of each one. The span on the FFT will need to go from 1kHz to about 50kHz if possible. You then need to compare 
the integrity of each signal by analyzing the difference in frequency components.

## Bill of Materials
Materials    | Quantity | Price per unit |
-------------|----------|----------------|
10k resistor |     9    |      0.10      |
5.1k resistor|     7    |      0.29      |

(https://www.digikey.com/classic/Ordering/AddPart.aspx)