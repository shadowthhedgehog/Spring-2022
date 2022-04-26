/**
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_ReadWrite_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use OctoSPI through
  *          the STM32L4xx HAL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup OSPI_RAM_ReadWrite_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
OSPI_HandleTypeDef OSPIPSRAMHandle;
__IO uint8_t RxCplt, TxCplt;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[BUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Delay_Calibration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  OSPIM_CfgTypeDef OSPIM_Cfg_Struct = {0};
  OSPI_RegularCmdTypeDef  sCommand = {0};
  uint8_t reg[2];

  uint32_t address = 0;
  __IO uint8_t step = 0;
  uint16_t index;



  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 110 MHz */
  SystemClock_Config();

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);

  /****************************************************************************/
  /*                                                                          */
  /* Initialize OctoSPI                                                       */
  /*                                                                          */
  /****************************************************************************/
  OSPIM_Cfg_Struct.ClkPort    = 1;
  OSPIM_Cfg_Struct.DQSPort    = 1;
  OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
  OSPIM_Cfg_Struct.IOLowPort  = HAL_OSPIM_IOPORT_1_LOW;
  OSPIM_Cfg_Struct.NCSPort    = 1;

  OSPIM_Cfg_Struct.Req2AckTime = 1;

  OSPIPSRAMHandle.Instance = OCTOSPI1;
  HAL_OSPI_DeInit(&OSPIPSRAMHandle);


  OSPIPSRAMHandle.Init.FifoThreshold         = 2;
  OSPIPSRAMHandle.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  OSPIPSRAMHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_APMEMORY;
  OSPIPSRAMHandle.Init.DeviceSize            = 23; /* 64 MBits */
  OSPIPSRAMHandle.Init.ChipSelectHighTime    = 1;
  OSPIPSRAMHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  OSPIPSRAMHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  OSPIPSRAMHandle.Init.ClockPrescaler        = 0x03;
  OSPIPSRAMHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  OSPIPSRAMHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  OSPIPSRAMHandle.Init.ChipSelectBoundary    = 4;
  OSPIPSRAMHandle.Init.DelayBlockBypass      = HAL_OSPI_DELAY_BLOCK_USED;
  OSPIPSRAMHandle.Init.MaxTran               = 0;
  __HAL_RCC_OSPIM_CLK_ENABLE();
  if (HAL_OSPIM_Config(&OSPIPSRAMHandle, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Init(&OSPIPSRAMHandle) != HAL_OK) {
    Error_Handler();
  }


  sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_8_LINES;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DQSMode            = HAL_OSPI_DQS_ENABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction        = READ_REG_CMD_SRAM;
  sCommand.Address            = 0;
  sCommand.NbData             = 2;
  sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_SRAM_READ;

  if (HAL_OSPI_Command(&OSPIPSRAMHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_OSPI_Receive(&OSPIPSRAMHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  sCommand.Instruction = WRITE_REG_CMD_SRAM;
  sCommand.DummyCycles = 0;
  MODIFY_REG(reg[0], 0x03, 0x00);

  if (HAL_OSPI_Command(&OSPIPSRAMHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_OSPI_Transmit(&OSPIPSRAMHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
   Error_Handler();
  }


  /****************************************************************************/
  /*                                                                          */
  /* RCC delay configuration register setting                                 */
  /*                                                                          */
  /****************************************************************************/
  Delay_Calibration();

  /****************************************************************************/
  /*                                                                          */
  /* Infinite write/read loop in DMA mode                                     */
  /*                                                                          */
  /****************************************************************************/
  while (1)
  {
    switch(step)
    {
      case 0:
        TxCplt = 0;

        /* Initialize Reception buffer -------------------------------------- */
        for (index = 0; index < BUFFERSIZE; index++)
        {
          aRxBuffer[index] = 0;
        }

        /* Writing Sequence ------------------------------------------------- */
        sCommand.Instruction        = WRITE_CMD_SRAM;
        sCommand.Address            = address;
        sCommand.NbData             = BUFFERSIZE;
        sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_SRAM_WRITE;

        if (HAL_OSPI_Command(&OSPIPSRAMHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        if (HAL_OSPI_Transmit_DMA(&OSPIPSRAMHandle, aTxBuffer) != HAL_OK)
        {
          Error_Handler();
        }

        step++;
        break;

      case 1:
        if(TxCplt != 0)
        {
          TxCplt = 0;
          RxCplt = 0;

          /* Reading Sequence ----------------------------------------------- */
          sCommand.Instruction        = READ_CMD_SRAM;
          sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_SRAM_READ;

          if (HAL_OSPI_Command(&OSPIPSRAMHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
          {
            Error_Handler();
          }

          if (HAL_OSPI_Receive_DMA(&OSPIPSRAMHandle, aRxBuffer) != HAL_OK)
          {
            Error_Handler();
          }
          step++;
        }
        break;

      case 2:
        if (RxCplt != 0)
        {
          RxCplt = 0;

          /* Result comparison ---------------------------------------------- */
          for (index = 0; index < BUFFERSIZE; index++)
          {
            if (aRxBuffer[index] != aTxBuffer[index])
            {
              BSP_LED_On(LED_BLUE);
              __BKPT(0);
            }
          }
          BSP_LED_Toggle(LED_GREEN);
          HAL_Delay(50);

          address += OSPI_PSRAM_INCR_SIZE;
          if(address >= OSPI_PSRAM_END_ADDR)
          {
            address = 0;
          }
          step = 0;
        }
        break;

      default :
        Error_Handler();

    }
  }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_RxCpltCallback(OSPI_HandleTypeDef *hospi)
{
  RxCplt++;
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
 void HAL_OSPI_TxCpltCallback(OSPI_HandleTypeDef *hospi)
{
  TxCplt++;
}

/**
  * @brief  Transfer Error callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_ErrorCallback(OSPI_HandleTypeDef *hospi)
{
  Error_Handler();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 110000000
  *            HCLK(Hz)                       = 110000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 55
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable voltage range 1 boost mode for frequency above 80 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 55;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED_BLUE);

  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  OctoSPI 1 delay configuration
  * @note May vary from board to be board. To be implemented by user application
  * @param  None
  * @retval None
  */
static void Delay_Calibration(void)
{
  uint32_t delay, l_index;
  OSPI_RegularCmdTypeDef  sCommand = {0};
  __IO uint32_t step;
  __IO uint32_t calibration_ongoing;

  sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_8_LINES;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DQSMode            = HAL_OSPI_DQS_ENABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  delay = 1;
  calibration_ongoing = 1;
  step = 0;

  while (calibration_ongoing)
  {
    if (step == 0)
    {
      HAL_RCCEx_OCTOSPIDelayConfig(delay, 0);
    }

    switch(step)
    {
      case 0:
        TxCplt = 0;

        /* Initialize Reception buffer -------------------------------------- */
        for (l_index = 0; l_index < BUFFERSIZE; l_index++)
        {
          aRxBuffer[l_index] = 0;
        }

        /* Writing Sequence ------------------------------------------------- */
        sCommand.Instruction        = WRITE_CMD_SRAM;
        sCommand.Address            = 0;
        sCommand.NbData             = BUFFERSIZE;
        sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_SRAM_WRITE;

        if (HAL_OSPI_Command(&OSPIPSRAMHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        if (HAL_OSPI_Transmit_DMA(&OSPIPSRAMHandle, aTxBuffer) != HAL_OK)
        {
          Error_Handler();
        }

        step = 1;
        break;

      case 1:
        if(TxCplt != 0)
        {
          TxCplt = 0;
          RxCplt = 0;

          /* Reading Sequence ----------------------------------------------- */
          sCommand.Instruction        = READ_CMD_SRAM;
          sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_SRAM_READ;

          if (HAL_OSPI_Command(&OSPIPSRAMHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
          {
            Error_Handler();
          }

          if (HAL_OSPI_Receive_DMA(&OSPIPSRAMHandle, aRxBuffer) != HAL_OK)
          {
            Error_Handler();
          }
          step = 2;
        }
        break;

      case 2:
        if (RxCplt != 0)
        {
          RxCplt = 0;

          /* Result comparison ---------------------------------------------- */
          step = 3; /* assuming passed */
          for (l_index = 0; l_index < BUFFERSIZE; l_index++)
          {
            if (aRxBuffer[l_index] != aTxBuffer[l_index])
            {
               step = 4; /* failure */
            }
          }
        }
        break;

      case 3: /* success */
          /* Proper delay found, exit calibration */
          calibration_ongoing = 0;
          break;

      case 4: /* failure */
           if (delay < 15)
          {
            /* Increase delay and relaunch iteration */
            delay++;
            step = 0;
          }
          else
          {
            /* If delay set to maximum and error still detected: can't use external PSRAM */
            Error_Handler();
          }

        break;

      default :
        Error_Handler();

    } /* switch(step) */

  } /* while (calibration_ongoing) */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
