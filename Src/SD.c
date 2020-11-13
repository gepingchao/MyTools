#define TRUE  1
#define FALSE 0
#define bool BYTE

#include "include.h"
#include "diskio.h"
#include "SD.h"

extern SPI_HandleTypeDef hspi3; 
extern volatile unsigned char Timer1, Timer2;                    /* 10ms Ã¿¼õÉÙ¼ÆÊ±Æ÷ */

static volatile DSTATUS Stat = STA_NOINIT;              /* ´ÅÅÌ×´Ì¬Flag*/
static unsigned char CardType;                                /* SD ÀàÐÍ 0:MMC, 1:SDC, 2:Block addressing */
static unsigned char PowerFlag = 0;                           /* Power ×´Ì¬Flag */

/* SPI Chip Select */
static void SELECT(void)
{
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
}

/* SPI Chip Deselect */
static void DESELECT(void)
{
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
}

/* SPI tx*/
static void SPI_TxByte(BYTE data)
{
  while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
  HAL_SPI_Transmit(&hspi3, &data, 1, SPI_TIMEOUT);
}

/* SPI ½ÓÊÕÒ»¸ö×Ö½Ú*/
static unsigned char SPI_RxByte(void)
{
  unsigned char dummy, data;
  dummy = 0xFF;
  data = 0;
  
  while ((HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY));
  HAL_SPI_TransmitReceive(&hspi3, &dummy, &data, 1, SPI_TIMEOUT);
  
  return data;
}

static void SPI_RxBytePtr(unsigned char *buff) 
{
  *buff = SPI_RxByte();
}


static unsigned char SD_ReadyWait(void) 
{
  unsigned char res;
  
  /* 500ms ³¬Ê±¼ì²â */
  Timer2 = 50;
  SPI_RxByte();
  
  do
  {
    res = SPI_RxByte();
  } while ((res != 0xFF) && Timer2);
  
  return res;
}

static void SD_PowerOn(void) 
{
  unsigned char cmd_arg[6];
  unsigned int Count = 0x1FFF;
  
  DESELECT();
  
  for(int i = 0; i < 10; i++)
  {
    SPI_TxByte(0xFF);
  }
  
  /* SPI Chips Select */
  SELECT();
  
  cmd_arg[0] = (CMD0 | 0x40);
  cmd_arg[1] = 0;
  cmd_arg[2] = 0;
  cmd_arg[3] = 0;
  cmd_arg[4] = 0;
  cmd_arg[5] = 0x95;
  
  for (int i = 0; i < 6; i++)
  {
    SPI_TxByte(cmd_arg[i]);
  }
  
  while ((SPI_RxByte() != 0x01) && Count)
  {
    Count--;
  }
  
  DESELECT();
  SPI_TxByte(0XFF);
  
  PowerFlag = 1;
}

static void SD_PowerOff(void) 
{
  PowerFlag = 0;
}

static unsigned char SD_CheckPower(void) 
{
  /*  0=off, 1=on */
  return PowerFlag;
}

static bool SD_RxDataBlock(BYTE *buff, UINT btr) 
{
  uint8_t token;
  
  Timer1 = 10;
  
  do 
  {    
    token = SPI_RxByte();
  } while((token == 0xFF) && Timer1);
  
  if(token != 0xFE)
    return FALSE;
  
  do 
  {     
    SPI_RxBytePtr(buff++);
    SPI_RxBytePtr(buff++);
  } while(btr -= 2);
  
  SPI_RxByte(); 
  SPI_RxByte();
  
  return TRUE;
}

#if _READONLY == 0
static bool SD_TxDataBlock(const BYTE *buff, BYTE token)
{
  unsigned char resp, wc;
  unsigned char i = 0;
    
  if (SD_ReadyWait() != 0xFF)
    return FALSE;
  
  SPI_TxByte(token);      
  
  if (token != 0xFD) 
  { 
    wc = 0;
    
    do 
    { 
      SPI_TxByte(*buff++);
      SPI_TxByte(*buff++);
    } while (--wc);
    
    SPI_RxByte();     
    SPI_RxByte();
    
    while (i <= 64) 
    {			
      resp = SPI_RxByte();
      
      if ((resp & 0x1F) == 0x05) 
        break;
      
      i++;
    }
    
    while (SPI_RxByte() == 0);
  }
  
  if ((resp & 0x1F) == 0x05)
    return TRUE;
  else
    return FALSE;
}
#endif /* _READONLY */

static BYTE SD_SendCmd(BYTE cmd, DWORD arg) 
{
  unsigned char crc, res;
  
  if (SD_ReadyWait() != 0xFF)
    return 0xFF;
  
  SPI_TxByte(cmd); 			/* Command */
  SPI_TxByte((BYTE) (arg >> 24)); 	/* Argument[31..24] */
  SPI_TxByte((BYTE) (arg >> 16)); 	/* Argument[23..16] */
  SPI_TxByte((BYTE) (arg >> 8)); 	/* Argument[15..8] */
  SPI_TxByte((BYTE) arg); 		/* Argument[7..0] */
  
  crc = 0;  
  if (cmd == CMD0)
    crc = 0x95; /* CRC for CMD0(0) */
  
  if (cmd == CMD8)
    crc = 0x87; /* CRC for CMD8(0x1AA) */
  
  SPI_TxByte(crc);
  
  if (cmd == CMD12)
    SPI_RxByte();
  
  unsigned char n = 10; 
  do
  {
    res = SPI_RxByte();
  } while ((res & 0x80) && --n);
  
  return res;
}

/*-----------------------------------------------------------------------
-----------------------------------------------------------------------*/

DSTATUS SD_disk_initialize(BYTE drv) 
{
  unsigned char n, type, ocr[4];
  
  if(drv)
    return STA_NOINIT;  
  
  if(Stat & STA_NODISK)
    return Stat;        
  
  SD_PowerOn();         
  
  SELECT();             
  
  type = 0;
  
  if (SD_SendCmd(CMD0, 0) == 1) 
  { 
    Timer1 = 100;
    
    if (SD_SendCmd(CMD8, 0x1AA) == 1) 
    { 
      for (n = 0; n < 4; n++)
      {
        ocr[n] = SPI_RxByte();
      }
      
      if (ocr[2] == 0x01 && ocr[3] == 0xAA) 
      { 
        do {
          if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 1UL << 30) == 0)
            break; /* ACMD41 with HCS bit */
        } while (Timer1);
        
        if (Timer1 && SD_SendCmd(CMD58, 0) == 0) 
        { 
          /* Check CCS bit */
          for (n = 0; n < 4; n++)
          {
            ocr[n] = SPI_RxByte();
          }
          
          type = (ocr[0] & 0x40) ? 6 : 2;
        }
      }
    } 
    else 
    { 
      /* SDC Ver1 or MMC */
      type = (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) <= 1) ? 2 : 1; /* SDC : MMC */
      
      do {
        if (type == 2) 
        {
          if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) == 0)
            break; /* ACMD41 */
        } 
        else 
        {
          if (SD_SendCmd(CMD1, 0) == 0)
            break; /* CMD1 */
        }
      } while (Timer1);
      
      if (!Timer1 || SD_SendCmd(CMD16, 512) != 0) 
      {
        type = 0;
      }
    }
  }
  
  CardType = type;
  
  DESELECT();
  
  SPI_RxByte(); 
  
  if (type) 
  {
    /* Clear STA_NOINIT */
    Stat &= ~STA_NOINIT; 
  }
  else
  {
    /* Initialization failed */
    SD_PowerOff();
  }
  
  return Stat;
}

DSTATUS SD_disk_status(BYTE drv) 
{
  if (drv)
    return STA_NOINIT; 
  
  return Stat;
}

DRESULT SD_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count) 
{
  if (pdrv || !count)
    return RES_PARERR;
  
  if (Stat & STA_NOINIT)
    return RES_NOTRDY;
  
  if (!(CardType & 4))
    sector *= 512;      /* ì§€ì • sectorë¥¼ Byte addressing ë‹¨ìœ„ë¡œ ë³€ê²½ */
  
  SELECT();
  
  if (count == 1) 
  { 
    if ((SD_SendCmd(CMD17, sector) == 0) && SD_RxDataBlock(buff, 512))
      count = 0;
  } 
  else 
  { 
    if (SD_SendCmd(CMD18, sector) == 0) 
    {       
      do {
        if (!SD_RxDataBlock(buff, 512))
          break;
        
        buff += 512;
      } while (--count);
      
      SD_SendCmd(CMD12, 0); 
    }
  }
  
  DESELECT();
  SPI_RxByte(); /* Idle ìƒíƒœ(Release DO) */
  
  return count ? RES_ERROR : RES_OK;
}

#if _READONLY == 0
DRESULT SD_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count) 
{
  if (pdrv || !count)
    return RES_PARERR;
  
  if (Stat & STA_NOINIT)
    return RES_NOTRDY;
  
  if (Stat & STA_PROTECT)
    return RES_WRPRT;
  
  if (!(CardType & 4))
    sector *= 512; 
  
  SELECT();
  
  if (count == 1) 
  { 
    if ((SD_SendCmd(CMD24, sector) == 0) && SD_TxDataBlock(buff, 0xFE))
      count = 0;
  } 
  else 
  { 
    if (CardType & 2) 
    {
      SD_SendCmd(CMD55, 0);
      SD_SendCmd(CMD23, count); /* ACMD23 */
    }
    
    if (SD_SendCmd(CMD25, sector) == 0) 
    {       
      do {
        if(!SD_TxDataBlock(buff, 0xFC))
          break;
        
        buff += 512;
      } while (--count);
      
      if(!SD_TxDataBlock(0, 0xFD))
      {        
        count = 1;
      }
    }
  }
  
  DESELECT();
  SPI_RxByte();
  
  return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

DRESULT SD_disk_ioctl(BYTE drv, BYTE ctrl, void *buff) 
{
  DRESULT res;
  BYTE n, csd[16], *ptr = buff;
  WORD csize;
  
  if (drv)
    return RES_PARERR;
  
  res = RES_ERROR;
  
  if (ctrl == CTRL_POWER) 
  {
    switch (*ptr) 
    {
    case 0:
      if (SD_CheckPower())
        SD_PowerOff();          /* Power Off */
      res = RES_OK;
      break;
    case 1:
      SD_PowerOn();             /* Power On */
      res = RES_OK;
      break;
    case 2:
      *(ptr + 1) = (BYTE) SD_CheckPower();
      res = RES_OK;             /* Power Check */
      break;
    default:
      res = RES_PARERR;
    }
  } 
  else 
  {
    if (Stat & STA_NOINIT)
      return RES_NOTRDY;
    
    SELECT();
    
    switch (ctrl) 
    {
    case GET_SECTOR_COUNT: 
      if ((SD_SendCmd(CMD9, 0) == 0) && SD_RxDataBlock(csd, 16)) 
      {
        if ((csd[0] >> 6) == 1) 
        { 
          /* SDC ver 2.00 */
          csize = csd[9] + ((WORD) csd[8] << 8) + 1;
          *(DWORD*) buff = (DWORD) csize << 10;
        } 
        else 
        { 
          /* MMC or SDC ver 1.XX */
          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
          csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6] & 3) << 10) + 1;
          *(DWORD*) buff = (DWORD) csize << (n - 9);
        }
        
        res = RES_OK;
      }
      break;
      
    case GET_SECTOR_SIZE: 
      *(WORD*) buff = 512;
      res = RES_OK;
      break;
      
    case CTRL_SYNC: 
      if (SD_ReadyWait() == 0xFF)
        res = RES_OK;
      break;
      
    case MMC_GET_CSD: 
      if (SD_SendCmd(CMD9, 0) == 0 && SD_RxDataBlock(ptr, 16))
        res = RES_OK;
      break;
      
    case MMC_GET_CID: 
      if (SD_SendCmd(CMD10, 0) == 0 && SD_RxDataBlock(ptr, 16))
        res = RES_OK;
      break;
      
    case MMC_GET_OCR: 
      if (SD_SendCmd(CMD58, 0) == 0) 
      {         
        for (n = 0; n < 4; n++)
        {
          *ptr++ = SPI_RxByte();
        }
        
        res = RES_OK;
      }     
      
    default:
      res = RES_PARERR;
    }
    
    DESELECT();
    SPI_RxByte();
  }
  
  return res;
}

unsigned int num = 0;

void SD_Test(void)
{ 
	static unsigned int tick = 0;
	unsigned short num;
	FIL fil;        /* File object */
	static char line[1000] = {0}; /* Line buffer */
	FRESULT fr; 
	unsigned int write_num = 0;
	for(num = 0 ;num< 950; num++)
		{
			line[num] = '*';
		}
	
	sprintf((line+950),"hello stm32 %d last opeart time %d\r\n ",num++,tick);
	
	tick = HAL_GetTick();
	fr = f_open(&fil, "hello.txt", FA_WRITE|FA_OPEN_ALWAYS);
	f_lseek(&fil, fil.fsize);
	
	fr = f_write(&fil , line, 1000, &write_num);
	fr = f_close(&fil);
	tick = HAL_GetTick() - tick;
	
	if(fr)
		{
			fr++;
		}
	return;
}


