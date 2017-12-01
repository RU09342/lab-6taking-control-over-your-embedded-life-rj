# Lab 6: Open Loop Systems
* Jessica Wozniak & Ryan Hare
* Created: 11/1/17
* Last updated: 11/20/17

### Overview
The open loop system will receive a desired tempC over UART and calculate the proper duty cycle to maintain that temperature. 
### Voltage Regulator
The 5V regulator (LM7805C) had a 20V input and a 33 ohm power resistor connected Vout to ground. 

### Fan Control
The fan was powered by 12V DC with a current limit of 0.6A. Our fan had four pins, but only 2 were used: Vcc and GND.
The fan was controlled by a duty cycle that was hard coded depending on desired tempC. At 0x00 the fan was fully off, and at 0xFF the fan was fully on. 
Since a certain current/ voltage was needed to initially turn the fan on, we found that a duty cycle below "0x1A (26)" was not reachable.  

### Temperature Reading
A LM35 temperature sensor was used to read the temperature of the 5V regulator. 

### Schematic 
![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/Schematic.PNG)

### System Modeling
Since it wasn't possible to do a PWM below 10%, instead of plotting the temp C vs PWM in 5C steps, we plotted PWM 0- 100% and recorded the 
corresponding tempC.  

|  Duty Cycle  |  Temp C |
|--------------|---------|
|   0          |   100   |
|  10          |    54   |
|  20          |	43   |
|  30          |    39   |
|  40          |    38   |
|  50          |    37   |
|  60          |    36   |
|  70          |    34   |
|  80          |    33   |
|  90          |    32   |
|  100         |    32   |


![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/System_Modeling_chart.PNG)

Without the ability to have a PWM under 10% it was impossible to maintain a temperature over 54 easily. So for the purpose of this lab 
we decided to take out the first two data points. A new plot is shown below.


![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/System_Modeling_No_over_50.PNG)


Using the new plot shown above, the equation (y = -0.0763x + 48.61) can be used to find what duty cycle should be set for a desired temperature: 
where x is the duty cycle and y is tempCDes. For example, when tempCDes is 37, the duty cycle would be about 170. The duty cycle was calculated 
in the UART interrupt. 
```C
        tempCDes  = UCA0RXBUF;             // send RX to tempCDes
		
        if (tempCDes > 32 && tempCDes <=54)
        {
            PWM = ((tempCDes-48.61)/-0.0763)
        }
        else if (tempCDes < 32)
        {
            PWM = 255;  
        }
        else if (tempCDes > 54)
        {
            PWM = 25;  
        }
	   
	   TB0CCR1 = PWM;
      
```

Although this method worked pretty well; we then decided to make the program more accurate by splitting up the equation to calculate duty cycle 
by for a range of temperatures. Using excel, we found new equations for 
* tempCDes <= 32       
* 32 < tempCDes <= 34  
![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/34%20and%2032.PNG)          
* 34 < tempCDes <= 36   
![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/36%20and%2034.PNG)      
* 36 < tempCDes <= 39  
![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/39%20and%2036.PNG)    
* 39 < tempCDes <= 43 
![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/43%20and%2039.PNG) 
* 43 < tempCDes <= 54    
![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/54%20and%2043.PNG)
* tempCDes > 54
![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-rj/blob/master/Photos/over%2054.PNG)

The new equations were then implemented through if else statements within the UART interrupt.
```C 
        tempCDes  = UCA0RXBUF;             // send RX to tempCDes
		
        if (tempCDes <= 32)
        {
            PWM = 0xFF;
        }
        else if (tempCDes > 32 && tempCDes <=34)
        {
            PWM = ((tempCDes-36.87)/-0.0917)
        }
        else if (tempCDes > 34 && tempCDes <=36)
        {
            PWM = ((tempCDes-48.24)/-0.08)
        else if (tempCDes > 36 && tempCDes <=39)
        {
            PWM = ((tempCDes-42.03)/-0.0394)
        }
        else if (tempCDes > 39 && tempCDes <=43)
        {
            PWM = ((tempCDes-50.85)/-0.1538)
        }
        else if (tempCDes > 43 && tempCDes <=54)
        {
            PWM = ((tempCDes-57.90)/-0.2759)
        }
        else if (tempCDes > 54)
        {
            PWM = 25;
        }

        TB0CCR1 = PWM;
```

### Why the 5994?
The main reason we chose to use the MSP430FR5994 is because it has 2 timers. One timer (TimerB) was needed for the PWM of the fan and the other
 timer (TimerA) was needed for the ADC coversion/ enable. Also as mentioned above, UART was used to send a desired temperature. Using code from a 
 previous milestone "Stranger Things Light Wall," the UART was easily manipulated to also work for this lab.