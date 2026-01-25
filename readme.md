# ELEC2645 Unit 3 - Magic 8 Ball 

This lab shows the basics of using the LCD screen and the onboard blue button and LED on the Nucleo board. We also write to the serial port 

The most important file is `main.c` which is found in `Core/Src/main.c` 

## The assignment

First get the code running and make sure you understand how it is all working. 

Something is wrong with this Magic 8 Ball - it sounds far too sycophantic like ChatGPT (4o in particular). It just agrees with everything you say and praises you! There is also a black screen after the first button press.

For this lab can you:
1. Add more messages to the Magic 8 ball
2. Fix the blank screen in between button presses
3. (Bonus) Add some more fancy animations!

When you are finished, add your finished code (just main.c) to the Code submission area on Minerva. 


## Setup
Please follow the prelab setup on the Minerva page, but a brief reminder here:
- Install [ST Link drivers](https://www.st.com/en/development-tools/stsw-link009.html) (if using your own PC)
- Install the [STM32CubeIDE for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=stmicroelectronics.stm32-vscode-extension) a bundle of 15 extensions
- Open the Unit3_Magic8Ball folder  and in the bottom corner when asked "*Would you like to configure discovered CMake project as STM32Cube project*" say "**Yes**" you want to open this folder as an STM32 Project. Let the extension finish the setup
- If prompted choose the "Debug" configuration
- You can click "Build" at the bottom of the screen to check it compiles
- Check the board is connected under "STM32CUBE Devices and Boards" in the "Run and Debug" page. You can tell it to Blink, and if you have not yet done so, upgrade the firmware. 
- Then Click "Run and Debug" and select "STM32Cube: STLink GDB Server"
- The code will now run, and you can continue past the breakpoints by hitting F5 or clicking the green arrow
- (optional) add the [Serial Monitor](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-serial-monitor) extension as it is easier to use than the one bundled with the STM32CubeIDE

## The LCD 

Make sure you have followed the instructions on the Minerva page to hook up the LCD screen to the Nucleo through the breadboard. This is the same as the LCD Demo code


| LCD | MCU |
|-----|-----|
| 3V-5V | VDD |
| GND | GND |
| MOSI | B15 |
| SCK | B13 |
| CS  | B12 |
| DC  | B11 |
| BL  | B1  |

The functions we have used here are:

- `LCD_printString()`
- `LCD_Draw_Circle()`


## GPIO - General Purpose Input Output

These are the basic digital input and output functions described by the Hardware Abstraction Layer (HAL) for reading and writing values to pins. 

- `HAL_GPIO_WritePin(port, pin, state)` - Write a digital value (0 or 1) to a GPIO pin
- `HAL_GPIO_ReadPin(port, pin)` - Read the current state of a GPIO pin (returns `GPIO_PIN_SET` or `GPIO_PIN_RESET`)
- `HAL_GPIO_TogglePin(port, pin)` - Toggle a GPIO pin (if it was HIGH, set it LOW, and vice versa)


## Serial

Here we have redirected the printf function to the serial port, in the `_write` definition at the top of main.c. This way we can use `printf` as normal like in Unit 1&2 and it automatically sends it over serial. 



## Other HAL functions

- `HAL_Delay(ms)` - Delay execution for a specified number of milliseconds
- `HAL_UART_Transmit(huart, data, len, timeout)` - Send data over UART serial port (this is what printf uses internally via the `_write` function)
- `HAL_GetTick()` - Get the current system tick count (useful for timing and seeding random numbers)