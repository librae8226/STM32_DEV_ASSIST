/************************ (C) COPYLEFT 2010 Leafgrass *************************
* File Name          : SD_Driver.h
* Author             : Librae
* Last Modified Date : 11/10/2010
* Description        : This file provides SD SPI driver.

******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __SD_DRIVER_H__
#define __SD_DRIVER_H__

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types -----------------------------------------------------------*/
enum _CS_HOLD
{
	HOLD = 0,
	RELEASE = 1,
};

typedef struct       /* Card Specific Data */
{
  vu8  CSDStruct;            /* CSD structure */
  vu8  SysSpecVersion;       /* System specification version */
  vu8  Reserved1;            /* Reserved */
  vu8  TAAC;                 /* Data read access-time 1 */
  vu8  NSAC;                 /* Data read access-time 2 in CLK cycles */
  vu8  MaxBusClkFrec;        /* Max. bus clock frequency */
  vu16 CardComdClasses;      /* Card command classes */
  vu8  RdBlockLen;           /* Max. read data block length */
  vu8  PartBlockRead;        /* Partial blocks for read allowed */
  vu8  WrBlockMisalign;      /* Write block misalignment */
  vu8  RdBlockMisalign;      /* Read block misalignment */
  vu8  DSRImpl;              /* DSR implemented */
  vu8  Reserved2;            /* Reserved */
  vu32 DeviceSize;           /* Device Size */
  vu8  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
  vu8  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
  vu8  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
  vu8  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
  vu8  DeviceSizeMul;        /* Device size multiplier */
  vu8  EraseGrSize;          /* Erase group size */
  vu8  EraseGrMul;           /* Erase group size multiplier */
  vu8  WrProtectGrSize;      /* Write protect group size */
  vu8  WrProtectGrEnable;    /* Write protect group enable */
  vu8  ManDeflECC;           /* Manufacturer default ECC */
  vu8  WrSpeedFact;          /* Write speed factor */
  vu8  MaxWrBlockLen;        /* Max. write data block length */
  vu8  WriteBlockPaPartial;  /* Partial blocks for write allowed */
  vu8  Reserved3;            /* Reserded */
  vu8  ContentProtectAppli;  /* Content protection application */
  vu8  FileFormatGrouop;     /* File format group */
  vu8  CopyFlag;             /* Copy flag (OTP) */
  vu8  PermWrProtect;        /* Permanent write protection */
  vu8  TempWrProtect;        /* Temporary write protection */
  vu8  FileFormat;           /* File Format */
  vu8  ECC;                  /* ECC code */
  vu8  CSD_CRC;              /* CSD CRC */
  vu8  Reserved4;            /* always 1*/
}
MSD_CSD;

typedef struct							/*Card Identification Data*/
{
  vu8  ManufacturerID;       /* ManufacturerID */
  vu16 OEM_AppliID;          /* OEM/Application ID */
  vu32 ProdName1;            /* Product Name part1 */
  vu8  ProdName2;            /* Product Name part2*/
  vu8  ProdRev;              /* Product Revision */
  vu32 ProdSN;               /* Product Serial Number */
  vu8  Reserved1;            /* Reserved1 */
  vu16 ManufactDate;         /* Manufacturing Date */
  vu8  CID_CRC;              /* CID CRC */
  vu8  Reserved2;            /* always 1 */
}
MSD_CID;

typedef struct
{
  MSD_CSD CSD;
  MSD_CID CID;
  u32 Capacity;          /* Card Capacity */
  u32 BlockSize;         /* Card Block Size */
  u16 RCA;
  u8 CardType;
	u32 SpaceTotal;      		/* Total space size in file system */
	u32 SpaceFree;      		/* Free space size in file system */
}
MSD_CARDINFO, *PMSD_CARDINFO;

/* Exported constants -------------------------------------------------------*/

#define CARDTYPE_MMC		0x00
#define CARDTYPE_SDV1		0x01
#define CARDTYPE_SDV2		0x02
#define CARDTYPE_SDV2HC		0x04
#define DUMMY_BYTE			0xFF 
#define MSD_BLOCKSIZE		_MAX_SS

/* ------ SD/MMC command list - SPI mode ------*/
#define CMD0    0       // Reset
#define CMD1    1       // Send Operator Condition - SEND_OP_COND
#define CMD8    8       // Send Interface Condition - SEND_IF_COND
#define CMD9    9       // Read CSD
#define CMD10   10      // Read CID
#define CMD12   12      // Stop data transmit
#define CMD16   16      // Set block size, should return 0x00 
#define CMD17   17      // Read single block
#define CMD18   18      // Read multi block
#define ACMD23  23      // Prepare erase N-blokcs before multi block write
#define CMD24   24      // Write single block
#define CMD25   25      // Write multi block
#define ACMD41  41      // should return 0x00
#define CMD55   55      // should return 0x01
#define CMD58   58      // Read OCR
#define CMD59   59      // CRC disable/enbale, should return 0x00

/* Exported macro -----------------------------------------------------------*/
#define PRINT_INFO TRUE
//#define CHECK_INSERT TRUE

/* Exported variables ------------------------------------------------------ */

/* Exported functions ------------------------------------------------------ */
// Application Programming Interface 
u8 MSD_Init(void);
u8 MSD_GetCardInfo(PMSD_CARDINFO cardinfo);
u8 MSD_ReadSingleBlock(u32 sector, u8 *buffer);
u8 MSD_ReadMultiBlock(u32 sector, u8 *buffer, u32 NbrOfSector);
u8 MSD_WriteSingleBlock(u32 sector, uc8 *buffer);
u8 MSD_WriteMultiBlock(u32 sector, uc8 *buffer, u32 NbrOfSector);

// Low level functions
__inline u8 _spi_read_write(u8 data);
u8 _send_command(u8 cmd, u32 arg, u8 crc);
u8 _send_command_hold(u8 cmd, u32 arg, u8 crc);
u8 _read_buffer(u8 *buff, u16 len, u8 release);

// Physical level functions
void MSD_SPIConfig(void);
void MSD_SPIHighSpeed(u8 b_high);

// Physical level marcos
#define _card_enable()		GPIOA->BRR = GPIO_Pin_4
#define _card_disable()		GPIOA->BSRR = GPIO_Pin_4
//#define _card_power_on()	GPIOC->BRR = GPIO_Pin_10
//#define _card_power_off()	GPIOC->BSRR = GPIO_Pin_10

#if CHECK_INSERT == TRUE
#define _card_insert()		((GPIOA->IDR & GPIO_Pin_2) == Bit_RESET)
#endif 

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
