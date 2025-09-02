# Arduino LED Light Controller

_A fun project where you can change the LED display colour and view its data in real time_ <br />
_Written in Arduino c++, c++ and windows c api_
_Program written in my GDI wrapper, which is also hosted on this account_

## Preview
<table>
  <tr>
    <td>
      There are two buttons on the ARDUINO. The first button allows you to select the colour channel to modify (Red, Green, or Blue). The second button is used to change the value of the colour channel. 0-255 range.
    </td>
    <td width="70%">
        <video src="https://github.com/user-attachments/assets/ae709ea1-3250-4f2f-bbcd-9115d77ac6b0"/>
    </td>
  </tr>
</table>

## Requirements

### Software
1. C++ compiler (Change compiler in the `Makefile` using the `CXX` variable)
2. Windows API
3. Arduino IDE (TODO: Upload Arduino's code)

### Hardware 
_The hardware set up will be included in the next section_
1. Arduino Uno (Or any PWM supported Arduino)
2. RGB Light
3. Buttons x 2
4. 10K Ohm Resistors x 2 
5. Wires, lots and lots of wires!

## Setting up project

### Compiling the Arduino's Code

> [!TIP]
> View the hardware schematic [here](https://www.tinkercad.com/things/3vjTRE9jWru-arduino-led?sharecode=U9ETlC6QbMGK1n0t5VS8EiUdgKE64ssl48DPOJ-cmzk)

Wire the components according to the comment in `Arduino/main.o`. Or modify the global variables to your desired pins. Compile and upload the code into the Arduino.

### Compiling the User Software
Specify the `COM` Port that the program will attempt to communicate with the Arduino. Otherwise, the program may return a communication error. This is done in `main.cpp` under the global variable `PORT`. Compile the program. 

Plug in the Arduino **BEFORE** running the program.

<div align="center">
    <img width="35%" alt="image" src="https://github.com/user-attachments/assets/dd4efd55-b260-425d-a4d8-a4a908964f4c" />
    <img width="35%" alt="image" src="https://github.com/user-attachments/assets/d5c8bb6b-11eb-4ffe-b256-d2ad802f3a5f" />
    <img width="35%" alt="image" src="https://github.com/user-attachments/assets/4ae7d588-642b-4c0b-b9ce-6a6b5f77e463" />
    <img width="35%" alt="Screenshot 2025-09-01 160355" src="https://github.com/user-attachments/assets/08964aaf-a72e-4cd1-921b-1156b48fef28" />
</div>
