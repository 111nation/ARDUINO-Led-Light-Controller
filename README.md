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
    <td width="70%">
        <video src="https://github.com/user-attachments/assets/ae709ea1-3250-4f2f-bbcd-9115d77ac6b0"/>
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

![20250901_160207](https://github.com/user-attachments/assets/893633f9-3d76-47fc-840f-35e9a3cf50e6)
![20250901_160207](https://github.com/user-attachments/assets/dd4efd55-b260-425d-a4d8-a4a908964f4c)

![20250901_160149](https://github.com/user-attachments/assets/ef5f0cf9-e775-476a-b56e-bd0f80fc1836)
<img width="1920" height="1080" alt="Screenshot 2025-09-01 160355" src="https://github.com/user-attachments/assets/08964aaf-a72e-4cd1-921b-1156b48fef28" />
