# ELEC2645 Unit 3 - Magic 8 Ball 

This lab demonstrates embedded systems programming on the STM32L476 Nucleo board, focusing on:
- **LCD Screen Control** - Displaying text and graphics on the ST7789V2 SPI display
- **GPIO Input/Output** - Reading the onboard blue button and controlling the onboard LED
- **Serial Communication** - Debugging and output via UART/USB
- **Random Number Generation** - Creating an interactive Magic 8 Ball application

The most important file is [main.c](Core/Src/main.c) which contains the main application logic. 

## The Assignment

Currently the Magic 8 Ball responses are overly positive and sycophantic (agrees with everything like ChatGPT 4o) - this could get people into trouble if they were really asking it for advice! :).  Also the LCD screen goes blank after the first button press so its not clear it is still running. 

**Your Tasks**:
1. **Add more messages** to the Magic 8 Ball response set - include diverse answers (positive, negative, non-committal)
2. **Fix the blank screen issue** - investigate and correct the LCD display behavior between button presses
3. **(Bonus) Add animations** - enhance the user experience with visual effects on the LCD

**Submission**: Add your finished [main.c](Core/Src/main.c) to the Code submission area on Minerva once completed. 


## Setup Instructions

### Prerequisites

1. **Completed previous labs**
    - Blinky and LCD Test to ensure the hardware is working first!

2. **Configure the Project**
   - Open the `Unit_3_1_Magic8Ball` folder in VS Code
   - When prompted "*Would you like to configure discovered CMake project as STM32Cube project*", click **Yes**
   - Allow the STM32 extension to complete initialization
   - Select **Debug** configuration when prompted

3. **Verify Hardware Connection**
   - Connect the Nucleo board via USB
   - Check that the board appears under "STM32CUBE Devices and Boards" in the Run and Debug sidebar
   - Test with the **Blink** function to verify communication

4. **Build and Run**
   - Click **Build** in the bottom status bar to verify compilation
   - Open Run and Debug panel (`Ctrl+Shift+D`)
   - Select **"STM32Cube: STLink GDB Server"** and click Run
   - Continue past breakpoints with **F5** or the play button

### Troubleshooting
- **Board not detected**: Ensure ST Link drivers are installed and USB cable is connected
- **Build errors**: Clean the build folder and rebuild: use **Build** > **Clean all**
- **Serial communication issues**: Verify the serial port in the Serial Monitor (typically `COM3` or higher on Windows)


## Hardware Configuration

### LCD Display Connection (ST7789V2)

| LCD Pin | Signal  | Nucleo Pin | Purpose |
|---------|---------|-----------|---------|
| VDD     | Power (3.3V-5V) | VDD | Display power supply |
| GND     | Ground  | GND | Ground reference |
| MOSI    | Serial Data | PB15 | SPI Master Output Slave Input |
| SCK     | Clock   | PB13 | SPI Clock signal |
| CS      | Chip Select | PB12 | SPI Chip Select |
| DC      | Data/Command | PB11 | Command vs Data mode |
| BL      | Backlight | PB1 | Backlight control (PWM) |

### User Inputs
- **Blue Button**: Connected to the Nucleo's onboard user button (internal pull-down)
  - Used to trigger new Magic 8 Ball responses
  - GPIO port: `GPIOC`, Pin: `GPIO_PIN_13`
- **LED**: Onboard green LED for status indication
  - GPIO port: `GPIOA`, Pin: `GPIO_PIN_5`

### Serial Communication
- **UART Interface**: USB connection via ST Link debugger
- **Baud Rate**: 115200 (typical, verify in [usart.c](Core/Src/usart.c))
- **Purpose**: Debug output via `printf()` and serial monitoring


## Software Architecture

### Key Source Files

| File | Purpose |
|------|---------|
| [main.c](Core/Src/main.c) | **Main application logic** - Magic 8 Ball game loop, message storage, button handling |
| [main.h](Core/Inc/main.h) | Main header with configuration macros and function prototypes |
| [gpio.c](Core/Src/gpio.c) | GPIO peripheral initialization and setup |
| [usart.c](Core/Src/usart.c) | UART/Serial communication setup and configuration |
| [stm32l4xx_it.c](Core/Src/stm32l4xx_it.c) | Interrupt handlers (button press, systick, UART) |
| [system_stm32l4xx.c](Core/Src/system_stm32l4xx.c) | System initialization and clock setup |

### LCD Driver
The project includes an external ST7789V2 LCD driver:
- **Location**: [ST7789V2_Driver_STM32L4/](ST7789V2_Driver_STM32L4/)
- **Key Functions**:
  - `LCD_printString(x, y, text)` - Display text at specified coordinates
  - `LCD_Draw_Circle(x, y, radius, color)` - Draw a filled circle
  - `LCD_Clear()` - Clear the display
  - See the driver README for complete API documentation

### Build System
- **Build Tool**: CMake with Ninja generator
- **Configuration**: [CMakeLists.txt](CMakeLists.txt)
- **Build Output**: Compiled binary in `build/Debug/Unit_3_1_Magic8Ball.elf`


## Serial Communication & Debugging

The `printf()` function has been redirected to the UART serial port by implementing custom `_write()` in [main.c](Core/Src/main.c). This allows you to use `printf()` as normal for debugging, exactly like in Units 1 & 2, and the output automatically transmits over serial.

**Serial Configuration**:
- **Baud Rate**: 115200 bps (check [usart.c](Core/Src/usart.c) to verify)
- **Data Format**: 8 bits, 1 stop bit, no parity
- **Connection**: USB via ST Link on the Nucleo board (virtual COM port)
- **Serial Monitor**: Use VS Code Serial Monitor extension for easy viewing 



## STM32 HAL API Reference

### GPIO - General Purpose Input/Output
Digital I/O for reading/writing to pins via the Hardware Abstraction Layer:

- `HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState)` - Write digital value to pin
  - Example: `HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)` turns ON the LED
  - Example: `HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)` turns OFF the LED

- `HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)` - Read current state of a pin
  - Returns: `GPIO_PIN_SET` (HIGH/1) or `GPIO_PIN_RESET` (LOW/0)
  - Example: `if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)` detects button press

- `HAL_GPIO_TogglePin(GPIOx, GPIO_Pin)` - Toggle pin state (HIGH ↔ LOW)
  - Example: `HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)` blinks the LED

### Timing & System Functions
- `HAL_Delay(ms)` - Block execution for specified milliseconds
  - Example: `HAL_Delay(1000)` pauses for 1 second
  
- `HAL_GetTick()` - Get current system tick count (milliseconds since boot)
  - Useful for non-blocking timing and seeding random numbers
  - Example: `srand(HAL_GetTick())` seeds the random number generator
