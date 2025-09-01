## Arduino LED Light Controller

_A fun project where you can change the LED display colour and view its data in real time_ <br />
_Written in Arduino c++, c++ and windows c api_
_Program written in my GDI wrapper, which is also hosted on this account_

#### Preview
<table>
  <tr>
    <td>
      There are two buttons on the ARDUINO. The first button allows you to select the colour channel to modify (Red, Green, or Blue). The second button is used to change the value of the colour channel. 0-255 range.
    </td>
    <td>
      
    </td>
  </tr>
</table>

#### Requirements

1. C++ compiler (Change compiler in the `Makefile` using the `CXX` variable)
2. Windows API/ SDK Installed
3. Arduino (TODO: Upload Arduino's code)

   > [!NOTE]
   > Remember to specify the `COM` Port that the program will attempt to communicate with the Arduino. Otherwise, the program will return a communication error.
   > This is done in `main.cpp` under the global variable `PORT`.
