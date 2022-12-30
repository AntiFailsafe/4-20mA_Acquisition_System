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
And since the resistors that comprise this type of circuit are principally in charge of determining its absolute accuracy, gain and offset calibration are necessary to eliminate the error caused by standard value substitution and resistor tolerances.  

## Two-point Calibration

In order to obtain higher performance, the circuit gain needs to be tuned. Two-point calibration is an option.  

First, 4 mA is delivered to the input to begin the two-point calibration, and the ADC output code is noted as $Code_1$. The ADC output code is then recorded as $Code_2$ after 20 mA of input current is supplied. The gain factor is determined using  

$$GF=\frac {16 \\ mA} {Code_2-Code_1}$$

The input current can now be calculated corresponding to any output code, $Code_x$, using the equation:  

$$I_{in}=4 \\ mA+GF(Code_x-Code_1)$$

## Rest of the circuit

![绘图2](https://user-images.githubusercontent.com/72595382/210077031-e35f1d38-9783-420b-ae29-8802a74d4141.svg)
CAN for communications and Optocoupler to detect the threshold set on DT35.

## Photos

These are photos for V1.1. Differences between V1.1 and V1.2 are only the layouts &#x1F603;.

![IMG_20220520_133904_1](https://user-images.githubusercontent.com/72595382/210082009-d8d61bdc-3e1a-4594-a972-d0a4ce33ccd3.jpg)
![IMG_20220520_122030](https://user-images.githubusercontent.com/72595382/210077214-dc886094-fc01-4684-adbb-165a15ee138c.jpg)

# 中文 4-20 mA 数据采集系统
## 作者的话

该系统是我在电子科技大学机器人团队LIMITI（2022年全国大学生机器人大赛——Robocon）和ABU Robocon 2022中国代表队期间开发的。我未来的研究方向将不再专注于板级电路。作为对过去一年的纪念，我开源了这个电路，它适用于大多数4-20mA传感器，而不仅仅是DT35。我希望这对大家有所帮助，祝好运。

## 介绍

该系统采用ADS8864和STM32F302C8T6，使用单个24V直流电源处理4 mA至20 mA的输入信号。室温校准前的总误差为±4 LSB，即室温（27℃）下能够达到±0.06%FSR的误差。  
该系统中使用的传感器是Sick的DT35，但输入端子可以修改以满足各种不同的工业传感器接口（只要是4-20mA便可）。

## 电路说明
![绘图1](https://user-images.githubusercontent.com/72595382/210067598-45a4748b-669a-48a5-9308-4ff3d9cdc3dc.svg)

该电路由电平转换电路、输入电流-电压转换器、ADC级和输出数字信号处理级组成。电阻 $R_{25}$ 将4 mA至20 mA的输入信号转换为电压。当 $R_{25} = 120 \\ \Omega$ 且输入电流在4至20 mA之间时，电平转换电路的输入电压范围为0.48 V至2.4 V。二极管D1用于限制电压以防止输入为电压而不是电流的情况，二极管D2用于输入电流源防反接。

U5A运算放大器是双AD8606的其中一个，其转换电平并衰减 $R_{25}$ 两端的电压。运算放大器的输出范围为0.1 V至2.4 V，具有100 mV的余量以确保线性，与ADC的输入范围（0 V至2.5 V）相匹配。使用剩下的一个AD1582来跟随缓冲参考电压（VREF=2.5 V）。

最小输出电压（范围下限）设置为100mV，以提供安全余量，同时考虑到元件的公差。为了在ADC输入端为正摆幅提供100 mV的余量，输出范围的最大值限制为2.4 V。因此，运算放大器的标称输出电压范围为0.1 V至2.4 V。AD8606（U5B）的后半部分用于缓冲ADS8864（U6）ADC的2.5 V参考电压。

由于其低偏置电压（最大值65μV）、低偏置电流（最大值1 pA）和低噪声（最大值12 nV/√Hz），选择AD8606用于此应用。使用3.3V电源时，功耗仅为9.2mW。此外，也可以使用其他运算放大器。替代品可以是OPA2320、OPA2376和其他选项，如上图所示。

运算放大器输出级之后是单极点RC滤波器（R21/C26），以降低带外噪声。RC滤波器的截止频率设置为339kHz。为了应对低频工业噪声，可以进一步降低滤波器截止频率，即级联额外的滤波器（R20/C24）。在这种情况下，由于信号带宽较窄，ADS8864的采样率可以降低更大范围。

其在100kSPS下0.65mW的超低功率使得ADS8864（U6）16位400kSPS SAR ADC成为本次设计的选择，同时SPI与其高速串行接口兼容。受STM32F302C8T6处理速度受限，奈奎斯特频率设置为66.6KHz。

## 电路设计
![绘图3](https://user-images.githubusercontent.com/72595382/210069716-406cd487-a300-4c61-b73d-d3fdf590fc11.svg)

低频传递函数由叠加原理得到：

$$V_{out}=I_{in} R_{25} (1+ \frac {R_{28}} {R_{19} ||R_{29}} )-V_{ref}  \frac {R_{28}} {R_{19}}$$  

其中,  

$$I_{in}  = 4\\ mA\\ to\\ 20\\ mA $$ 

$$R_{19} ||R_{29}= \frac {R_{19} R_{29}} {R_{19}+R_{29}}$$  

电路的低频增益为:  

$$G= \frac {ΔV_{out}} {ΔI_{in}} = R_{25} (1+ \frac {R_{28}} {R_{19} ||R_{29}} )= \frac {(2.4-0.1)V} {(20-4 )mA }$$  
 
同时 $R_{25}$ 上的电压降不得超过输出范围的上限（2.4V）  

$$R_{25}×I_{in(max)}=R_{25}×20 mA \le 2.4V $$  

若 $R_{25} = 120 \\ \Omega$， $\frac {R_{28}} {R_{19} ||R_{29}}$ 可计算为:  

$$\frac {R_{28}} {R_{19} ||R_{29}} = \frac {G} {R_{25}} -1=0.198 $$  

对于最小输入电流，我们有:  
$$OFFSET= V_{out}=0.1V=4mA \times R_{25} (1+ \frac {R_{28}} {R_{19} ||R_{29}} )-V_{ref}  \frac{R_{28}} {R_{19}} $$  

因此,  

$$\frac{R_{28}} {R_{19}} =0.19$$  

将 $R_{28}$ 设置为 $1 \\ k\Omega$，根据标准电阻值，选择 $R_{19} = 5.11 \\ k\Omega$ 和 $R_{29} = 124 \\ k\Omega$ 作为实际电路的元件值。

而且，由于电路的主要误差来源于电阻，因此必须进行增益和偏移校准，以尽可能消除标准值与之间理论值的误差以及电阻公差引起的误差。

## 两点校准

为了获得更高的性能，需要调整电路增益。可选择两点校准。

首先，向输入端输送4 mA以开始两点校准，ADC输出代码记为 $Code_1$。在提供20mA输入电流后，ADC输出代码被记录为 $Code_2$。增益系数为：
$$GF=\frac {16 \\ mA} {Code_2-Code_1}$$

现在可以使用以下公式计算与任何输出代码 $Code_x$ 相对应的输入电流：

$$I_{in}=4 \\ mA+GF(Code_x-Code_1)$$

## 剩下的电路

![绘图2](https://user-images.githubusercontent.com/72595382/210077031-e35f1d38-9783-420b-ae29-8802a74d4141.svg)
CAN用于通信，光电耦合器用于检测DT35设置的距离阈值。

## 照片

这些是V1.1的照片。V1.1和V1.2之间的区别仅在于PCB layout&#x1F603;.

![IMG_20220520_133904_1](https://user-images.githubusercontent.com/72595382/210082009-d8d61bdc-3e1a-4594-a972-d0a4ce33ccd3.jpg)
![IMG_20220520_122030](https://user-images.githubusercontent.com/72595382/210077214-dc886094-fc01-4684-adbb-165a15ee138c.jpg)


