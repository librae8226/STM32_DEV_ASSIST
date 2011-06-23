/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
//#include "includes.h"
#include "os_cpu.h"
#include "SD_Driver.h"
#include "diskio.h"


extern MSD_CARDINFO CardInfo;

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv					/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;

	if(drv)
	{
		return STA_NOINIT;
	}

	stat = MSD_Init();
	if(stat == STA_NODISK)
	{		
		return STA_NODISK;
	}
	else if(stat != 0)
	{
		return STA_NOINIT;
	}

	if(MSD_GetCardInfo(&CardInfo))
	{
		return STA_NOINIT;
	}

	return 0; 
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv					/* Physical drive nmuber (0..) */
)
{
	if(drv)
	{
		return STA_NOINIT;
	}
#if CHECK_INSERT == TRUE
	if(!_card_insert())
	{
		return STA_NODISK;
	}
#endif

	return 0;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,					/* Physical drive nmuber (0..) */
	BYTE *buff,				/* Data buffer to store read data */
	DWORD sector,			/* Sector address (LBA) */
	BYTE count				/* Number of sectors to read (1..255) */
)
{
	BYTE res;

	if(drv || count == 0)
	{
		return RES_PARERR;
	}

#if CHECK_INSERT == TRUE
	if(!_card_insert())
	{
		return RES_NOTRDY; 			// Not ready
	}
#endif
	
	if(count == 1)            // Single block read    
	{                                                
		res = MSD_ReadSingleBlock(sector, buff);      
	}                                                
	else                    	// Multi block read      
	{                                                
		res = MSD_ReadMultiBlock(sector, buff, count);
	}
	
	if(res == 0x00)
	{
		return RES_OK;
	}

	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,					/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,			/* Sector address (LBA) */
	BYTE count				/* Number of sectors to write (1..255) */
)
{
	BYTE res;

	if(drv || count == 0)
	{
		return RES_PARERR;
	}

#if CHECK_INSERT == TRUE
	if(!_card_insert())
	{
		return RES_NOTRDY; 			// Not ready
	}
#endif
	
	if(count == 1)						// Single block write
	{
		res = MSD_WriteSingleBlock(sector, buff);
	}
	else 											// Multi block write
	{
		res = MSD_WriteMultiBlock(sector, buff, count);
	}
	
	if(res == 0)
	{
		return RES_OK;
	}

	return RES_ERROR;
}
#endif /* _READONLY */


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,					/* Physical drive nmuber (0..) */
	BYTE ctrl,				/* Control code */
	void *buff				/* Buffer to send/receive control data */
)
{
	u32 retry;

	if(drv)
	{
		return RES_PARERR;
	}

  // FATFS only disponse 3 control codes: CTRL_SYNC, GET_SECTOR_COUNT, GET_BLOCK_SIZ
  switch(ctrl)
  {
  case CTRL_SYNC:
    _card_enable();
		retry = 0;
		while(_spi_read_write(DUMMY_BYTE) != 0xFF)
		{	
			// Timeout return
			if(retry++ == 0x20000)
			{
		    _card_disable();
		    return RES_ERROR;
			}
		}
		_card_disable();
    return RES_OK;
      
  case GET_BLOCK_SIZE:
    *(WORD*)buff = CardInfo.BlockSize;
    return RES_OK;

  case GET_SECTOR_COUNT:
    *(DWORD*)buff = CardInfo.Capacity;
    return RES_OK;
  }

  return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{
  return 0;
}
