/**
  ******************************************************************************
  * @file    SDFatFS/source/ffext.c 
  * @author  deliang_zhu
  * @version V1.0.2
  * @date    25/12/2009
  * @brief   
  ******************************************************************************
  * @copy
  *
  */

/* Includes ------------------------------------------------------------------*/
//#include "includes.h"
#include <stdio.h>
#include <string.h>
#include "SD_Driver.h"
#include "ffext.h"



// *****************************************************************************
// Variables
// *****************************************************************************
extern MSD_CARDINFO CardInfo;


// *******************************************************************************
// Function Name  : ffext_mount
// Description    : mounted the drive and get the file system information
// *******************************************************************************
FRESULT ffext_mount (
	BYTE vol,						/* Logical drive number to be mounted/unmounted */
	FATFS *fs						/* Pointer to new file system object (NULL for unmount)*/
)
{
	FRESULT res;
	DWORD fclust;
	FATFS *pfs;

#if PRINT_INFO == TRUE

	printf("\r\n**************************************************\r\n");
	res = f_mount(vol, fs);
	if(res != FR_OK)
	{
		printf("Mount Physical Driver Failed: %d\r\n", vol);
		printf("**************************************************\r\n"); 
		return res;
	}	

	res = f_getfree("/", &fclust, &pfs);
	if(res != FR_OK)
	{
		printf("Get Physical Driver Information failed: code=%02x\r\n", res); 
		printf("**************************************************\r\n"); 
		return res;
	}

	CardInfo.SpaceTotal = (((pfs->max_clust -2) * pfs->csize) * CardInfo.BlockSize);
	CardInfo.SpaceFree = ((fclust *	pfs->csize) * CardInfo.BlockSize);

	if(pfs->fs_type == 1) 
		printf("File System Sub Type          : FAT12\r\n"); 
	else if(pfs->fs_type == 2)
		printf("File System Sub Type          : FAT16\r\n");
	else if(pfs->fs_type == 3)
		printf("File System Sub Type          : FAT32\r\n");
	
//	printf("存储容量          : %lD字节\r\n", CardInfo.SpaceTotal);		
//	printf("可用容量          : %lD字节\r\n", CardInfo.SpaceFree);
//	printf("磁盘序号          : %d\r\n", pfs->drive);
//	printf("簇总数            : %d\r\n", pfs->max_clust);	
//	printf("可用簇数量        : %d\r\n", pfs->free_clust);
//	printf("已分配簇数量      : %d\r\n", pfs->last_clust);
//	printf("扇区大小          : %d\r\n", sizeof(pfs->win));
//	printf("扇区数/簇         : %d\r\n", pfs->csize);
//	printf("文件分配表总数    : %d\r\n", pfs->n_fats);	
//	printf("扇区数/文件分配表 : %d\r\n", pfs->sects_fat);
//	printf("文件系统起始扇区  : %d\r\n", pfs->fatbase);
//	printf("根目录起始扇区    : %d\r\n", pfs->dirbase);
//	printf("数据起始扇区      : %d\r\n", pfs->database);

	
	printf("Total space (Bytes)           : %ld\r\n", CardInfo.SpaceTotal);		
	printf("Available space (Bytes)       : %ld\r\n", CardInfo.SpaceFree);

	printf("Physical drive number         : %d\r\n", pfs->drive);

	printf("Maximum cluster               : %d\r\n", pfs->max_clust);	
	printf("Number of free clusters       : %d\r\n", pfs->free_clust);
	printf("Last allocated cluster        : %d\r\n", pfs->last_clust);

	printf("Maximum sector size           : %d\r\n", sizeof(pfs->win));
	printf("Number of sectors per cluster : %d\r\n", pfs->csize);

	printf("Number of FAT copies          : %d\r\n", pfs->n_fats);		
	printf("Sectors per fat               : %d\r\n", pfs->sects_fat);

	printf("File system info sector       : %d\r\n", pfs->fsi_sector);	
	printf("Root directory start sector   : %d\r\n", pfs->dirbase);	
	printf("FAT start sector              : %d\r\n", pfs->fatbase);
	printf("Data start sector             : %d\r\n", pfs->database);

//	printf("Num of root directory entries : %d\r\n", pfs->n_rootdir);
//	printf("File system mount ID          : %d\r\n", pfs->id);
//	printf("win[] dirty flag              : %d\r\n", pfs->wflag);
//	printf("File system info dirty flag   : %d\r\n", pfs->fsi_flag);
		
	printf("**************************************************\r\n");
							
#else	
	res = f_mount(vol, fs);
	if(res != FR_OK)
	{
		return res;
	}

	res = f_getfree("/", &fclust, &pfs);
	if(res != FR_OK)
	{
		return res;
	}
#endif

	return FR_OK;	
}

// *******************************************************************************
// Function Name  : ffext_write
// Description    : 
// *******************************************************************************
FRESULT ffext_write (	
	const XCHAR *path,	/* Pointer to the file name */
	const void *buff,		/* Pointer to the data to be written */
	UINT nobtw,					/* Number of bytes to write */
	BYTE mode						/* Access mode and file open mode flags */	
)
{
	FRESULT res;
	FIL f;
	UINT bytes_ret;

	res = f_open(&f, path, mode);
	if(res == FR_OK)
	{
		f_lseek(&f, f.fsize);
		res = f_write(&f, buff, nobtw, &bytes_ret);

#if PRINT_INFO == TRUE
		if(res == FR_OK)
		{
			printf("Write File Successed: %s, %d:%d\r\n", path, nobtw, bytes_ret);
		}
		else
		{
	 		printf("Wrtie File Failed: %s, error code=%02x\r\n", path, res);
		}
#endif

		f_close(&f);
		return res;
	}

#if PRINT_INFO == TRUE
	 	printf("Open File Failed: %s, error code=%02x\r\n", path, res);
#endif

	f_close(&f);
	return res;
}

// *******************************************************************************
// Function Name  : ffext_read
// Description    : 
// *******************************************************************************
FRESULT ffext_read (	
	const XCHAR *path,	/* Pointer to the file name */
	void *buff,					/* Pointer to the data to be written */
	UINT nobtr,					/* Number of bytes to read */
	BYTE mode						/* Access mode and file open mode flags */	
)
{
	FRESULT res;
	FIL f;
	UINT bytes_ret;

 	res = f_open(&f, path, mode);
	if(res == FR_OK)
	{
		res = f_read(&f, buff, nobtr, &bytes_ret);

#if PRINT_INFO == TRUE
		if(res == FR_OK)
		{
			printf("Read File Successed: %s, %d:%d\r\n", path, nobtr, bytes_ret);
		}
		else
		{
	 		printf("Read File Failed: %s, error code=%02x\r\n", path, res);
		}
#endif

		f_close(&f);
		return res;
	}

#if PRINT_INFO == TRUE
	 	printf("Open File Failed: %s, error code=%02x\r\n", path, res);
#endif

	f_close(&f);
	return res;
}
 

// *******************************************************************************
// Function Name  : ffext_scanfiles
// Description    : 
// *******************************************************************************
FRESULT ffext_scanfiles (
	char *path				/* Pointer to the file name */	
)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  int i;
  char *fn;

#if _USE_LFN
  static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
  fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
#endif

  res = f_opendir(&dir, path);
  if (res == FR_OK) 
	{
    i = strlen(path);
    for (;;) 
		{
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
			{
				break;
			}

      if (fno.fname[0] == '.') 
			{
				continue;
			}

#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif

      if (fno.fattrib & AM_DIR)
			{
        sprintf(path+i, "/%s", fn);
        res = ffext_scanfiles(path);
        if (res != FR_OK)
				{
					break;
				}

        path[i] = 0;
	    }
			else
			{
        printf("%s/%s\r\n", path, fn);
      }
    }
  }

  return res;
}
