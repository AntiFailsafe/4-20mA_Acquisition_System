# 4-20 mA Acquisition System
## Words from author
This system was developed during my time in UESTC (University of Electronic Science and Technology of China) robotic team-LIMITI for Robocon 2022 of mainland China and Team China for ABU Robocon 2022. My future research will be no longer focusing on board level circuits. As a commemoration of the past year, I open source this circuit and it is suitable for majority of 4-20mA sensors, not just DT35. I hope it will be helpful to you. Good luck!
## Introduction
This system incorporates ADS8864 and STM32F302C8T6 to processes 4 mA to 20 mA input signals using a single 24V DC supply. The total error before room temperature calibration is ±4 LSBs, namely ±0.06% FSR at the room temperature (27&deg;C).  
The sensor employed in this system is Sick’s DT35, **_but input terminal could be modified to meet a wide variety of industrial measurements._**

## Circuit description
![绘图1](https://user-images.githubusercontent.com/72595382/210067598-45a4748b-669a-48a5-9308-4ff3d9cdc3dc.svg)

The circuit is comprised of a **level shifting circuit**, an input **current-to-voltage converter**, an **ADC** stage, and an output **digital signal processing** stage. Resistor $R_{25}$ converts the 4 mA to 20 mA input signal into a voltage. The level shifting circuit's input voltage ranges from 0.48 V to 2.4 V when $R_{25} = 120 \\ \Omega$ and the input current is between 4 and 20 mA. While diode D1 is used to limit the input voltage for incorrect input of voltage instead of current, diode D2 is used to protect against an inadvertent reverse connection of the input current source.  

The U5A op amp, which is ${\frac 1  2}$ of the dual AD8606, level shifts and attenuates the voltage across R25. With 100 mV headroom to ensure linearity, the op amp's output range of 0.1 V to 2.4 V matches the input range of the ADC (0 V to 2.5 V). The necessary offset is produced using the remaining AD1582's buffered voltage reference (VREF = 2.5 V).  

The minimum output voltage (low limit of the range) is set to 100 mV to provide a safety margin while taking into account the tolerances of the parts. To provide 100 mV of headroom for the positive swing at the ADC input, the maximum limit of the output range is set at 2.4 V. As a result, the op amp's nominal output voltage range is 0.1 V to 2.4 V. The second half of the AD8606 (U5B) is used to buffer the 2.5 V voltage reference of the ADS8864 (U6) ADC.  

Due to its low offset voltage (65 μV maximum), low bias current (1 pA maximum), and low noise (12 nV/√Hz maximum), the AD8606 is selected for this application. With a 3.3 V supply, there is just 9.2 mW of power dissipation. Additionally, other opamps can be used. The replacements could be OPA2320, OPA2376, and other options, as shown in the Figure above.  

The op amp output stage is followed by a single-pole RC filter (R21/C26) to help reduce out-of-band noise. The RC filter's cutoff frequency is set to 339 kHz. To further lower the filter cutoff frequency in the occurrence of low frequency industrial noise, an extra filter (R20/C24) can be installed. In this situation, the ADS8864's sampling rate might be lowered due to the narrower signal bandwidth.  

Its ultralow power of 0.65mW at 100-kSPS led to the selection of the ADS8864 (U6) 16-bit 400-kSPS SAR ADC. SPI is compatible with its high-speed serial interface. The DSP core, STM32F302C8T6, which is the system's sampling rate limiter, has set the **_Nyquist rate to 66.6 KHz._**  

## Circuit Design
![绘图3](https://user-images.githubusercontent.com/72595382/210069716-406cd487-a300-4c61-b73d-d3fdf590fc11.svg)

The low-frequency transfer function is obtained from the superposition principle.  

$$V_{out}=I_{in} R_{25} (1+ \frac {R_{28}} {R_{19} ||R_{29}} )-V_{ref}  \frac {R_{28}} {R_{19}}$$  

where,  

$$I_{in}  = 4\\ mA\\ to\\ 20\\ mA $$ 

$$R_{19} ||R_{29}= \frac {R_{19} R_{29}} {R_{19}+R_{29}}$$  

The low-frequency gain of the circuit is:  

$$G= \frac {ΔV_{out}} {ΔI_{in}} = R_{25} (1+ \frac {R_{28}} {R_{19} ||R_{29}} )= \frac {(2.4-0.1)V} {(20-4 )mA }$$  
 
The voltage drop on $R_{25}$ must not exceed the upper limit (2.4V) of the output range,  

$$R_{25}×I_{in(max)}=R_{25}×20 mA \le 2.4V $$  

If $R_{25} = 120 \\ \Omega$, then the ratio $\frac {R_{28}} {R_{19} ||R_{29}}$ can be calculated as:  

$$\frac {R_{28}} {R_{19} ||R_{29}} = \frac {G} {R_{25}} -1=0.198 $$  

For the minimum input current, we have:  
$$OFFSET= V_{out}=0.1V=4mA \times R_{25} (1+ \frac {R_{28}} {R_{19} ||R_{29}} )-V_{ref}  \frac{R_{28}} {R_{19}} $$  

Therefore,  

$$\frac{R_{28}} {R_{19}} =0.19$$  

The closest accessible standard resistor values for $R_{19}$ and $R_{29}$ were chosen for the real circuit, with $R_{28}$ set to $1 \\ k\Omega$. The values $R_{19} = 5.11 \\ k\Omega$ and $R_{29} = 124 \\ k\Omega$ were chosen.
And since the resistors that comprise this type of circuit are principally in charge of determining its absolute correctness, gain and offset calibration are necessary to eliminate the error caused by standard value substitution and resistor tolerances.  

## Two-point Calibration

Comparing the theoretical transfer function, which is calculated through using component nominal values, and the actual circuit transfer function without calibration yields the inaccuracy prior to calibration. In order to obtain higher performance, the circuit gain needs to be tuned. Two-point calibration is an option.  

First, 4 mA is delivered to the input to begin the two-point calibration, and the ADC output code is noted as $Code_1$. The ADC output code is then recorded as $Code_2$ after 20 mA of input current is supplied. The gain factor is determined using  

$$GF=\frac {16 \\ mA} {Code_2-Code_1}$$

The input current can now be calculated corresponding to any output code, $Code_x$, using the equation:  

$$I_{in}=4 \\ mA+GF(Code_x-Code_1)$$

## Rest of the circuit

![绘图2](https://user-images.githubusercontent.com/72595382/210077031-e35f1d38-9783-420b-ae29-8802a74d4141.svg)
CAN for communications and Optocoupler to detect the threshold set on DT35.

## Photos

These are photos for V1.1. Differences between V1.1 and V1.2 are only the layouts &#x1F603;.
![IMG_20220520_133904](https://user-images.githubusercontent.com/72595382/210077206-2cd28842-3b70-4bde-b766-767e73bad8d8.jpg)
![IMG_20220520_122030](https://user-images.githubusercontent.com/72595382/210077214-dc886094-fc01-4684-adbb-165a15ee138c.jpg)
