// ELEC2645 Unit 3 Magic 8-Ball

#include "main.h"
#include "usart.h"
#include "gpio.h"


// AUTO-GENERATED STM32 FUNCTION PROTOTYPES - DO NOT EDIT
void SystemClock_Config(void);



#include "LCD.h" // Include the LCD driver header file
#include <stdio.h> // For printf
#include <stdlib.h> // For rand() 


// Magic 8-Ball messages
const char *messages[][3] = {
    {"You are", "so", "clever!"},
    {"Wow, what", "a great", "insight!"},
    {"It's not", "just code,", "it's art!"},
    {"That's", "brilliant", "thinking!"},
    {"You're", "crushing", "it!"},
    {"I'm in", "awe of", "your skills"},
    {"This is", "next-level", "genius!"},
    {"Absolutely", "remarkable", "work!"},
    {"You've", "nailed", "it!"},
    {"What an", "innovative", "approach!"}
};

#define NUM_MESSAGES (sizeof(messages) / sizeof(messages[0]))




// Retarget printf to UART (i.e. serial console)
// by default printf does nothing, but we can override the _write function which is 
// what printf ultimately calls to output characters
// this way we can write to serial console using the familar printf function
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}


// Function to display a Magic 8-Ball message on the LCD
// idx is the index of the message to display (0-NUM_MESSAGES-1)
void Display8BallMessage(ST7789V2_cfg_t* cfg, int idx);






/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  HAL_Init();   // You will get a breakpoint here by default!



  /* Configure the system clock */
  SystemClock_Config();

srand(HAL_GetTick()); // Seed the random number generator with the current tick value

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  // LCD configuration structure.
  // Here we define which SPI peripheral to use, and which GPIO pins are connected to which LCD signals.
  // You shouldnt need to change this unless you change the connections
  ST7789V2_cfg_t cfg0 = {
    .setup_done = 0,
    .spi = SPI2,
    .RST = {.port = GPIOB, .pin = GPIO_PIN_2},
    .BL = {.port = GPIOB, .pin = GPIO_PIN_1},
    .DC = {.port = GPIOB, .pin = GPIO_PIN_11},
    .CS = {.port = GPIOB, .pin = GPIO_PIN_12},
    .MOSI = {.port = GPIOB, .pin = GPIO_PIN_15},
    .SCLK = {.port = GPIOB, .pin = GPIO_PIN_13},
    .dma = {.instance = DMA1, .channel = DMA1_Channel5}
  };

  // Initialize LCD - note that this also powers on the display and backlight
  // and also if the display was previously powered on, it will probably show the last image
  // that was displayed before reset
  LCD_init(&cfg0);

  // Using printf to send data over UART to the serial console instead. See the _write function above
  // this makes it much easier!
  printf("Starting Magic 8 Ball demo...\n");

  // Flash the on-board LED a few times so we know it's running
  for (int iFlash = 0; iFlash < 6; iFlash++) {
    // HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); LD2_GPIO_Port and LD2_Pin are defined in main.h,
    // they are the on-board LED on the Nucleo board. Which are connected to PA5 (pin 5 of GPIO port A)
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(250); // delay 250 milliseconds
  }

  // make screen black (0 in palette set in LCD.h)
  LCD_Fill_Buffer(0);



  // Display welcome message
  LCD_printString("Push", 20, 50, 1, 4);
  LCD_printString("Blue", 20, 90, 4, 4);
  LCD_printString("Button   ", 20, 130, 1, 4);

  // in order to see the changes we have made to the screen buffer, we need to
  // refresh the display to copy the data in the buffer to the screen
  LCD_Refresh(&cfg0);



  while (1)
  {


    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
      // Button is pressed (active low)
      printf("Button pressed! Generating Magic 8-Ball response...\n");

      // Find the random number in the range [min, max]
        int min = 0;
        int max = NUM_MESSAGES - 1;
        int rd_num = rand() % (max - min + 1) + min;

    Display8BallMessage(&cfg0, rd_num);

    HAL_Delay(10); // Debounce delay
  }
}
}




void Display8BallMessage(ST7789V2_cfg_t* cfg, int idx)
{
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);

  uint16_t txt_x =40;
  uint16_t txt_y =70;
  uint16_t txt_gap = 40;

  LCD_Fill_Buffer(0);
  LCD_Draw_Circle(120, 120, 120, 12, 1);
  LCD_Draw_Circle(120, 120, 115, 8, 1);

  LCD_printString("The Magic", txt_x, txt_y, 15, 3);
  LCD_printString("  8-Ball", txt_x, txt_y + txt_gap, 15, 3);
  LCD_printString("  says   ", txt_x, txt_y + 2 * txt_gap, 15, 3);
  LCD_Refresh(cfg);
  HAL_Delay(300);

  LCD_printString("  says.  ", txt_x, txt_y + 2 * txt_gap, 15, 3);
  LCD_Refresh(cfg);
  HAL_Delay(300);

  LCD_printString("  says..", txt_x, txt_y + 2 * txt_gap, 15, 3);
  LCD_Refresh(cfg);
  HAL_Delay(300);

  LCD_printString("  says...", txt_x, txt_y + 2 * txt_gap, 15, 3);
  LCD_Refresh(cfg);
  HAL_Delay(300);

  LCD_Fill_Buffer(0);


  txt_x =20;
  txt_y =70;
  txt_gap = 50;
   

  LCD_printString(messages[idx][0], txt_x, txt_y, 14, 3);
  LCD_printString(messages[idx][1], txt_x, txt_y + txt_gap, 14, 3);
  LCD_printString(messages[idx][2], txt_x, txt_y + 2 * txt_gap, 14, 3);
  LCD_Refresh(cfg);
  HAL_Delay(3000);


  LCD_Fill_Buffer(0);
  LCD_Refresh(cfg);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
}




// ==== AUTO-GENERATED STM32 FUNCTIONS ====
// This is auto generated by STM32CubeMX, luckily we can just leave it here we dont need to understand it
//
// DO NOT EDIT UNLESS YOU KNOW WHAT YOU ARE DOING! 


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

// ==== AUTO-GENERATED STM32 FUNCTIONS END ====