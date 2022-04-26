/**
  ******************************************************************************
  * @file    audio_recorder_app.c
  * @author  MCD Application Team   
  * @brief   Audio recorder application functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "audio_recorder_app.h"


/** @addtogroup AUDIO_RECORDER_MODULE
  * @{
  */

/** @defgroup AUDIO_RECORDER_APPLICATION
  * @brief audio recorder application routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FIL                    wav_file;
static osMessageQId           AudioEvent = 0;
static osThreadId             AudioThreadId = 0;
static WAV_InfoTypedef        AudioInfo;
static uint8_t                pHeaderBuff[44];

/* Check fifo overflow to avoid SD saturation */
#define DATA_REQUESTED 1
#define DATA_AVAILABLE 2

//#define DEBUG_AUDIO_APP

extern FATFS mSDDISK_FatFs;         /* File system object for MSD disk logical drive */
extern char mSDDISK_Drive[4];       /* MSD Host logical drive number */

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);
static uint32_t WavProcess_EncInit(uint8_t *pHeader);
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAV_InfoTypedef* pAudioInfoStruct);
static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAV_InfoTypedef* pAudioInfoStruct);

static void AUDIO_TransferComplete_CallBack(void);
static void AUDIO_HalfTransfer_CallBack(void);
static void AUDIO_Error_CallBack(void);
static void BSP_AUDIO_IN_TransferComplete_CallBack(void);
static void BSP_AUDIO_IN_HalfTransfer_CallBack(void);

extern void _cbNotifyStateChange (void) ;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_Init(void)
{
  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudio_Thread, Audio_Thread, osPriorityRealtime, 0, 512);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);  
  haudio.in.state  = AUDIO_RECORDER_IDLE;
  return AUDIO_RECORDER_ERROR_NONE;
}
   
/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIO_RECORDER_StateTypdef  AUDIO_RECORDER_GetState(void)
{
  return haudio.in.state;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AUDIO_RECORDER_GetVolume(void)
{
  return haudio.in.volume;
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency used to record the audio stream.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_StartRec(uint32_t headSetSelected)
{
  uint32_t byteswritten = 0;
  uint32_t device;
  
  if (headSetSelected)
  {
    device = INPUT_DEVICE_ANALOG_MIC;
    haudio.in.freq = HEADSET_MIC_FREQ;
    haudio.in.nbChannel = HEADSET_MIC_NBR_CHANNEL;
    haudio.in.volume = HEADSET_MIC_VOLUME;  

  }
  else
  {
    device = INPUT_DEVICE_DIGITAL_MIC;
    haudio.in.freq = DIGITAL_MIC_FREQ;
    haudio.in.nbChannel = DIGITAL_MIC_NBR_CHANNEL;
    haudio.in.volume = DIGITAL_MIC_VOLUME;  
  }
  
  /* Initialize header file */
  WavProcess_EncInit(pHeaderBuff);

  /* Write header file */
  if(f_write(&wav_file, pHeaderBuff, 44, (void*)&byteswritten) == FR_OK)
  {
    if(byteswritten != 0)
    {      
      if(BSP_AUDIO_IN_InitEx(device, haudio.in.freq, AUDIO_REC_BIT_RESOLUTION, haudio.in.nbChannel) == AUDIO_ERROR)
      {
        Error_Handler();
      }
      
      /* Register audio BSP drivers callbacks */
      BSP_AUDIO_IN_RegisterCallbacks(AUDIO_Error_CallBack,
                                     BSP_AUDIO_IN_HalfTransfer_CallBack, 
                                     BSP_AUDIO_IN_TransferComplete_CallBack);
      
      if(BSP_AUDIO_IN_Record((uint16_t*)&haudio.buff[0], AUDIO_IN_BUFFER_SIZE_HALF_WORD) == AUDIO_ERROR)
      {
        Error_Handler();
      }
      
      if(haudio.in.state == AUDIO_RECORDER_SUSPENDED)
      {
        osThreadResume(AudioThreadId);
      }
      haudio.in.state = AUDIO_RECORDER_RECORDING;
      
      
      haudio.in.fptr = byteswritten;
      return AUDIO_RECORDER_ERROR_NONE;
    }
  }
  return AUDIO_RECORDER_ERROR_IO;
  
}

/**
  * @brief  Get the wav file information.
  * @param  file: wav file.
  * @param  info: pointer to wav file structure
  * @retval Audio state.
  */
WAV_InfoTypedef* AUDIO_RECORDER_GetFileInfo(void)
{
  return &AudioInfo;
}


/**
  * @brief  Erase audio file from storage unit
  * @param  None
  * @retval None
  */
void AUDIO_RECORDER_RemoveAudioFile(char const *fname)
{
  if (f_unlink(fname) != 0)
  {
    Error_Handler();
  }
}

/**
  * @brief  Select wav file.
  * @param  file: wav file.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_SelectFile(char* file, uint8_t mode)
{
  int numOfReadBytes;
   
  AUDIO_RECORDER_ErrorTypdef ret = AUDIO_RECORDER_ERROR_IO;
  
  f_mkdir("0:/Record");
  if( f_open(&wav_file, file, mode) == FR_OK) 
  {
    if (mode & FA_READ)
    {
      if(f_read(&wav_file, &AudioInfo, sizeof(WAV_InfoTypedef), (void *)&numOfReadBytes) == FR_OK)
      {
        ret = AUDIO_RECORDER_ERROR_NONE;
      }
      else
      {
        Error_Handler();
      }
    }
  }
  else
  {
    Error_Handler();
  }
  return ret;  
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency used to play the audio stream.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_StartPlay(void)
{
  uint32_t numOfReadBytes;
  uint32_t mode = BSP_AUDIO_OUT_STEREOMODE;

  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_OUT_VOLUME, haudio.in.freq) == AUDIO_ERROR)
  {
    Error_Handler();
  }

  /* Register audio BSP drivers callbacks */
  BSP_AUDIO_OUT_RegisterCallbacks(AUDIO_Error_CallBack,
                                  AUDIO_HalfTransfer_CallBack, 
                                  AUDIO_TransferComplete_CallBack);
 
  
  /* Fill whole buffer @ first time */
  if(f_read(&wav_file, 
            &haudio.buff[0], 
            AUDIO_OUT_BUFFER_SIZE, 
            (void *)&numOfReadBytes) == FR_OK)
  { 
    if(numOfReadBytes != 0)
    {
      if (BSP_AUDIO_OUT_Pause())
      {
        Error_Handler();
      }        

      if (haudio.in.nbChannel == 1)
      {
        mode = BSP_AUDIO_OUT_MONOMODE;
      }
      else if (haudio.in.nbChannel == 2)
      {
        mode = BSP_AUDIO_OUT_STEREOMODE;
      }
      else
      {
        Error_Handler();
      }
      
      BSP_AUDIO_OUT_ChangeAudioConfig(mode | BSP_AUDIO_OUT_CIRCULARMODE);
      
      if(haudio.in.state == AUDIO_RECORDER_SUSPENDED)
      {
        osThreadResume(AudioThreadId);
      }
      haudio.in.state = AUDIO_RECORDER_PLAYING;

      if(BSP_AUDIO_OUT_Play((uint16_t*)&haudio.buff[0], AUDIO_OUT_BUFFER_SIZE_HALF_WORD) == AUDIO_ERROR)
      {
        Error_Handler();
      }

      return AUDIO_RECORDER_ERROR_NONE;
    }
  }
  else
  {
    Error_Handler();
  }

  return AUDIO_RECORDER_ERROR_IO;
  
}

/**
  * @brief  Audio player DeInit
  * @param  None.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_DeInit(void)
{
  
  if(haudio.in.state == AUDIO_RECORDER_RECORDING)
  {
    BSP_AUDIO_IN_Stop();
    BSP_AUDIO_IN_DeInit();
    f_close(&wav_file); 
  }
  
  if(haudio.in.state == AUDIO_RECORDER_PLAYING)
  {  
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
    BSP_AUDIO_OUT_DeInit();  
    f_close(&wav_file); 
  }
  
  if(AudioEvent != 0)
  {
    vQueueDelete(AudioEvent); 
    AudioEvent = 0;
  }
  
  if(AudioThreadId != 0)
  {
    osThreadTerminate(AudioThreadId);
    AudioThreadId = 0;
  }
  return AUDIO_RECORDER_ERROR_NONE;
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_StopRec(void)
{
  uint32_t byteswritten = 0;
  AUDIO_RECORDER_ErrorTypdef audio_error = AUDIO_RECORDER_ERROR_IO;
  
  if(AUDIO_RECORDER_GetState() == AUDIO_RECORDER_RECORD_PAUSE)
  {
    AUDIO_RECORDER_PauseResume();  
  }
  
  if(BSP_AUDIO_IN_Stop() == AUDIO_ERROR)
  {
    Error_Handler();
  }
  
  if(BSP_AUDIO_IN_DeInit() == AUDIO_ERROR)
  {
    Error_Handler();
  }
  
//  /* Configure SD IT pin */
//  BSP_SD_Init();    
//  
//  /* Configure SD IT pin */
//  BSP_SD_ITConfig();  
  
  haudio.in.state = AUDIO_RECORDER_IDLE;      
  if(f_lseek(&wav_file, 0) == FR_OK)
  {
    /* Update the wav file header save it into wav file */
    WavProcess_HeaderUpdate(pHeaderBuff, &AudioInfo);
    
    if(f_write(&wav_file, pHeaderBuff, sizeof(WAV_InfoTypedef), (void*)&byteswritten) == FR_OK)
    {   
      audio_error = AUDIO_RECORDER_ERROR_NONE;
    }
  }
  
  haudio.in.state = AUDIO_RECORDER_SUSPENDED;
  
  f_close(&wav_file);
  
  _cbNotifyStateChange();
  
  if(AudioThreadId != 0)
  {  
    osThreadSuspend(AudioThreadId); 
  }
  return audio_error;
}


/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_StopPlay(void)
{
  haudio.in.state = AUDIO_RECORDER_SUSPENDED;

  if(BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW) == AUDIO_ERROR)
  {
//    Error_Handler();
  }

  BSP_AUDIO_OUT_DeInit();
//  {
////    Error_Handler();
//  }

  haudio.in.state = AUDIO_RECORDER_SUSPENDED;

  if (f_close(&wav_file) != 0)
  {
//    Error_Handler();
  }
      
  _cbNotifyStateChange(); 
  
  if(AudioThreadId != 0)
  {  
    osThreadSuspend(AudioThreadId); 
  } 
  return AUDIO_RECORDER_ERROR_NONE;
}

/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_PauseResume(void)
{
  if(haudio.in.state == AUDIO_RECORDER_PLAYING)
  {  
    osThreadSuspend(AudioThreadId);     

    if(BSP_AUDIO_OUT_Pause() == AUDIO_ERROR)
    {
      Error_Handler();
    }
    
    haudio.in.state = AUDIO_RECORDER_PLAY_PAUSE;
  }
  else if(haudio.in.state == AUDIO_RECORDER_RECORDING)
  {
    osThreadSuspend(AudioThreadId);     
    if(BSP_AUDIO_IN_Pause() == AUDIO_ERROR)
    {
      Error_Handler();
    }
    
    haudio.in.state = AUDIO_RECORDER_RECORD_PAUSE;    
  }
  
  else if(haudio.in.state == AUDIO_RECORDER_PLAY_PAUSE)
  { 
    osThreadResume(AudioThreadId);  
    if(BSP_AUDIO_OUT_Resume() == AUDIO_ERROR)
    {
      Error_Handler();
    }
    
    haudio.in.state = AUDIO_RECORDER_PLAYING;
  }
  else if(haudio.in.state == AUDIO_RECORDER_RECORD_PAUSE)
  {
    osThreadResume(AudioThreadId);  
    if(BSP_AUDIO_IN_Resume() == AUDIO_ERROR)
    {
      Error_Handler();
    }
    
    haudio.in.state = AUDIO_RECORDER_RECORDING;    
  }  
  return AUDIO_RECORDER_ERROR_NONE;
}

/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIO_RECORDER_ErrorTypdef  AUDIO_RECORDER_Resume(void)
{

  if(haudio.in.state == AUDIO_RECORDER_PLAY_PAUSE)
  { 
    osThreadResume(AudioThreadId);  
    if(BSP_AUDIO_OUT_Resume() == AUDIO_ERROR)
    {
      Error_Handler();
    }
    
    haudio.in.state = AUDIO_RECORDER_PLAYING;
  }
  else if(haudio.in.state == AUDIO_RECORDER_RECORD_PAUSE)
  {
    osThreadResume(AudioThreadId);  
    if(BSP_AUDIO_IN_Resume() == AUDIO_ERROR)
    {
      Error_Handler();
    }
    
    haudio.in.state = AUDIO_RECORDER_RECORDING;    
  }
  return AUDIO_RECORDER_ERROR_NONE;
}


/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
static void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
#ifdef DEBUG_AUDIO_APP
  if(osMessagePut(AudioEvent, REC_BUFFER_OFFSET_FULL, 0) != osOK){
    /* Fifo overflow !!! */
    Error_Handler();
  }
#else
  osMessagePut(AudioEvent, REC_BUFFER_OFFSET_FULL, 0);
#endif


}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{ 
#ifdef DEBUG_AUDIO_APP
  if (osMessagePut(AudioEvent, REC_BUFFER_OFFSET_HALF, 0) != osOK)
  {
    /* Fifo overflow !!! */
    Error_Handler();
  }
#else
  osMessagePut(AudioEvent, REC_BUFFER_OFFSET_HALF, 0);
#endif
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_CallBack(void)
{
  haudio.in.state = AUDIO_RECORDER_ERROR;
}


/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_TransferComplete_CallBack(void)
{
  if(haudio.in.state == AUDIO_RECORDER_PLAYING)
  {
    if (osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_FULL, 0) != osOK)
    {
      /* Fifo underflow !!! */
//      Error_Handler();
    }
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_HalfTransfer_CallBack(void)
{ 
  if(haudio.in.state == AUDIO_RECORDER_PLAYING)
  {
    if (osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_HALF, 0) != osOK)
    {
      /* Fifo underflow !!! */
//      Error_Handler();
    }
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_Error_CallBack(void)
{
  haudio.in.state = AUDIO_RECORDER_ERROR;
}

/**
  * @brief  Audio task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Audio_Thread(void const * argument)
{
  uint32_t numOfReadBytes, numOfWrittenBytes;    
  osEvent event;  
  for(;;)
  {
    event = osMessageGet(AudioEvent, osWaitForever);
    
    if( event.status == osEventMessage )
    {
      if(haudio.in.state == AUDIO_RECORDER_PLAYING)
      {
        switch(event.value.v)
        {
        case PLAY_BUFFER_OFFSET_HALF:
          if(f_read(&wav_file, 
                    &haudio.buff[0], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            {  
              AUDIO_RECORDER_StopPlay();
            } 
            
          }
          else
          {
            AUDIO_RECORDER_StopPlay(); 
          }
          break;  
          
        case PLAY_BUFFER_OFFSET_FULL:
          if(f_read(&wav_file, 
                    &haudio.buff[AUDIO_OUT_BUFFER_SIZE/2], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            { 
              AUDIO_RECORDER_StopPlay();  
            } 
          }
          else
          {
            AUDIO_RECORDER_StopPlay();          
          }
          break;   
          
        default:
          break;
        }
      }
      /* Recording .... */
      if(haudio.in.state == AUDIO_RECORDER_RECORDING)
      {
        switch(event.value.v)
        {
          
        case REC_BUFFER_OFFSET_HALF:
          if(f_write(&wav_file, (uint8_t*)(haudio.buff), 
                     AUDIO_IN_BUFFER_SIZE/2, 
                     (void*)&numOfWrittenBytes) == FR_OK)
          { 
            if(numOfWrittenBytes == 0)
            { 
              AUDIO_RECORDER_StopRec();
            } 
          }
          else
          {
            AUDIO_RECORDER_StopRec();
          }
          haudio.in.fptr += numOfWrittenBytes;
          break; 
          
          
        case REC_BUFFER_OFFSET_FULL:
          if(f_write(&wav_file, (uint8_t*)(haudio.buff + AUDIO_IN_BUFFER_SIZE/2), 
                     AUDIO_IN_BUFFER_SIZE/2, 
                     (void*)&numOfWrittenBytes) == FR_OK)
          { 
            if(numOfWrittenBytes == 0)
            { 
              AUDIO_RECORDER_StopRec();
            } 
          }
          else
          {
            AUDIO_RECORDER_StopRec();
          }
          haudio.in.fptr += numOfWrittenBytes;
          break; 
          
          
        default:
          break;
        }
      }
      
    }

  }
}

/**
  * @brief  Return Elapsed recording Time
  * @param  None
  * @retval Elapsed Time in millisecond
  */

uint32_t AUDIO_RECORDER_GetElapsedTime(void)
{
  uint32_t duration;
  
  duration = haudio.in.fptr / AudioInfo.ByteRate;    
  return duration;
  
}

/**
  * @brief  Return remain playing time
  * @param  None
  * @retval Elapsed Time in millisecond
  */

uint32_t AUDIO_RECORDER_GetPlayedTime(void)
{
  uint32_t duration;
  
  duration = (wav_file.fptr) / AudioInfo.ByteRate;    
  return duration;
  
}


/**
  * @brief  Return remain playing time
  * @param  None
  * @retval Elapsed Time in millisecond
  */

uint32_t AUDIO_RECORDER_GetTotalTime(void)
{
  uint32_t duration;
  
  duration = f_size(&wav_file) / AudioInfo.ByteRate;    
  return duration;
  
}
/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Encoder initialization.
  * @param  Freq: Sampling frequency.
  * @param  pHeader: Pointer to the WAV file header to be written.  
  * @retval 0 if success, !0 else.
  */
static uint32_t WavProcess_EncInit(uint8_t *pHeader)
{  
  /* Initialize the encoder structure */
  AudioInfo.SampleRate = haudio.in.freq; /* Audio sampling frequency */
  AudioInfo.NbrChannels = haudio.in.nbChannel; /* Number of channels: 1:Mono or 2:Stereo */
  AudioInfo.BitPerSample = AUDIO_REC_BIT_RESOLUTION; /* Number of bits per sample (16, 24 or 32) */
  AudioInfo.FileSize = 0x001D4C00; /* Total length of useful audio data (payload) */
  AudioInfo.SubChunk1Size = 44; /* The file header chunk size */
  AudioInfo.ByteRate = (AudioInfo.SampleRate * \
                        (AudioInfo.BitPerSample/8) * \
                         AudioInfo.NbrChannels);     /* Number of bytes per second  (sample rate * block align)  */
  AudioInfo.BlockAlign = AudioInfo.NbrChannels * \
                         (AudioInfo.BitPerSample/8); /* channels * bits/sample / 8 */
  
  /* Parse the wav file header and extract required information */
  if(WavProcess_HeaderInit(pHeader, &AudioInfo))
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Initialize the wave header file
  * @param  pHeader: Header Buffer to be filled
  * @param  pAudioInfoStruct: Pointer to the wave structure to be filled.
  * @retval 0 if passed, !0 if failed.
  */
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAV_InfoTypedef* pAudioInfoStruct)
{
  /* Write chunkID, must be 'RIFF'  ------------------------------------------*/
  pHeader[0] = 'R';
  pHeader[1] = 'I';
  pHeader[2] = 'F';
  pHeader[3] = 'F';
  
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the 
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = 0x00;
  pHeader[5] = 0x4C;
  pHeader[6] = 0x1D;
  pHeader[7] = 0x00;
  /* Write the file format, must be 'WAVE' -----------------------------------*/
  pHeader[8]  = 'W';
  pHeader[9]  = 'A';
  pHeader[10] = 'V';
  pHeader[11] = 'E';
  
  /* Write the format chunk, must be'fmt ' -----------------------------------*/
  pHeader[12]  = 'f';
  pHeader[13]  = 'm';
  pHeader[14]  = 't';
  pHeader[15]  = ' ';
  
  /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
  pHeader[16]  = 0x10;
  pHeader[17]  = 0x00;
  pHeader[18]  = 0x00;
  pHeader[19]  = 0x00;
  
  /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
  pHeader[20]  = 0x01;
  pHeader[21]  = 0x00;
  
  /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
  pHeader[22]  = pAudioInfoStruct->NbrChannels;
  pHeader[23]  = 0x00;
  
  /* Write the Sample Rate in Hz ---------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  pHeader[24]  = (uint8_t)((pAudioInfoStruct->SampleRate & 0xFF));
  pHeader[25]  = (uint8_t)((pAudioInfoStruct->SampleRate >> 8) & 0xFF);
  pHeader[26]  = (uint8_t)((pAudioInfoStruct->SampleRate >> 16) & 0xFF);
  pHeader[27]  = (uint8_t)((pAudioInfoStruct->SampleRate >> 24) & 0xFF);
  
  /* Write the Byte Rate -----------------------------------------------------*/
  pHeader[28]  = (uint8_t)((pAudioInfoStruct->ByteRate & 0xFF));
  pHeader[29]  = (uint8_t)((pAudioInfoStruct->ByteRate >> 8) & 0xFF);
  pHeader[30]  = (uint8_t)((pAudioInfoStruct->ByteRate >> 16) & 0xFF);
  pHeader[31]  = (uint8_t)((pAudioInfoStruct->ByteRate >> 24) & 0xFF);
  
  /* Write the block alignment -----------------------------------------------*/
  pHeader[32]  = pAudioInfoStruct->BlockAlign;
  pHeader[33]  = 0x00;
  
  /* Write the number of bits per sample -------------------------------------*/
  pHeader[34]  = pAudioInfoStruct->BitPerSample;
  pHeader[35]  = 0x00;
  
  /* Write the Data chunk, must be 'data' ------------------------------------*/
  pHeader[36]  = 'd';
  pHeader[37]  = 'a';
  pHeader[38]  = 't';
  pHeader[39]  = 'a';
  
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  pHeader[40]  = 0x00;
  pHeader[41]  = 0x4C;
  pHeader[42]  = 0x1D;
  pHeader[43]  = 0x00;
  
  /* Return 0 if all operations are OK */
  return 0;
}

/**
  * @brief  Initialize the wave header file
  * @param  pHeader: Header Buffer to be filled
  * @param  pAudioInfoStruct: Pointer to the wave structure to be filled.
  * @retval 0 if passed, !0 if failed.
  */
static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAV_InfoTypedef* pAudioInfoStruct)
{
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the 
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = (uint8_t)(haudio.in.fptr);
  pHeader[5] = (uint8_t)(haudio.in.fptr >> 8);
  pHeader[6] = (uint8_t)(haudio.in.fptr >> 16);
  pHeader[7] = (uint8_t)(haudio.in.fptr >> 24);
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  haudio.in.fptr -=44;
  pHeader[40] = (uint8_t)(haudio.in.fptr); 
  pHeader[41] = (uint8_t)(haudio.in.fptr >> 8);
  pHeader[42] = (uint8_t)(haudio.in.fptr >> 16);
  pHeader[43] = (uint8_t)(haudio.in.fptr >> 24); 
  
  /* Return 0 if all operations are OK */
  return 0;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
