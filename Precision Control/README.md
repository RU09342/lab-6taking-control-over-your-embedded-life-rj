# Lab 6: Precision Control
* Jessica Wozniak & Ryan Hare
* Created: 11/1/17
* Last updated: 11/20/17

## PWM Part 2


### Schematic
![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/Schematic_LPF.PNG)

### Hardware

![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/20171201_174819.jpg)

The circuit for this PWM features a low pass filter. This low pass filter works with the varied duty cycle of the PWM signal to convert the digital signal to an analog signal.
Once the signal has been converted to analog, it will be approximately a sinusoidal function. As the maximum duty cycle is decreased, the magnitude of the analog signal will decrease.
### Software
The software features a PWM signal with a duty cycle that repeatedly varies between 0% and 100%. The for loops that perform this function are shown below.
```C
  while(1){

      for (i = 0; i < 255; i ++)
      {
          TA0CCR1= i;
      }
      for (i = 255; i > 0 ; i --)
      {
          TA0CCR1 = i;
      }
  }
```
This function works with the low pass filter to generate the analog sinusoidal signal that the circuit outputs.

## R2R DAC
The values of the resistors in a R2R Ladder DAC allow an input to be taken in and divided through the bits accordingly. The resistor values used 
in our R2R ladder were 10k and 5.1K.   The first set of resistors will form a volatge divider, making the output of that section of the R2R Vcc/2. 
The Vcc/2 is then send through another voltage divider, making the output of that section Vcc/4, etc. One important note about R2R Ladders: 
BIT0 is actually the MSB (most significant bit). 


### Schematic


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/R2R_Ladder.PNG)

### Hardware 


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/R2R_Ladder.jpg)

### Software
In the code written below, essentially each bit of the R2R Ladder is being turned on or off, depending on the value being inputted by the counter.
When 1 is sent, BIT0 is on, when 255 is sent, BIT0 - BIT7 are all on. The corresponding voltage is then outputted to the oscilliscope. For this 8-bit
resolution DAC, each bit is equal to 0.01289V. When just BIT0 is on, the outputted voltage should be 0.01289V, whereas when all bits are on, the 
outputted voltage should be 3.3V.


On the MSP430F5529, Port 3 is the only port that has all pins (P3.0-P3.7) available for output. The following line of code initializes Port 3.
```C
 P3DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
```
The 8 pins (P3.1 - P3.7) correspond to an 8 bit binary number. To show the DAC was working properly, the following code sets up a binary counter that 
counts from 0 (0x00) up to 255 (0xFF) and back down to 0 again.  
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
When the output of the circuit was recorded on the oscilliscope, a triangle wave was formed.


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/r2r_ladder_right.png)


When you zoom in closely on the oscilliscope, you can see the "staircase" formed by each of the 255 voltages on the output of the DAC. 


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/r2r_ladder_zoomed_2.png)


FFT


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/fft_r2r.png)


### Loading Effects R2R
After trying various resistor sizes as a load on the R2R ladder, it seems that when the load is bigger, it has less of an effect on the circuit,
whereas when the load is small, there is a larger effect on the circuit. 
* 100
The first load that was tested was a 100 ohm resistor. As stated above, the voltage was at 3.3V with no load, the voltage shown below is 170mV. 


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/100%20ohm.png)

* 5k
Next to be tested was a 5k resistor. The voltage was at 2.21V. Even though the effect on the cicuit is not as drastic as the previous load, there is still a 
change in volatge compared to the original. 


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/5k%20ohm.png)

* 100k
As you can see, with 100k resistor as a load the voltage is almost identical to the original circuit.


![Atl Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/100k%20ohm.png)

## Bill of Materials
Materials    | Quantity | Price per unit |
-------------|----------|----------------|
10k resistor |     9    |      0.10      |
5.1k resistor|     7    |      0.29      |

