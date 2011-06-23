#ifndef __FF_EXT_H__
#define __FF_EXT_H__


#include "ff.h"
#include "integer.h"

FRESULT ffext_mount (
	BYTE vol,			/* Logical drive number to be mounted/unmounted */
	FATFS *fs			/* Pointer to new file system object (NULL for unmount)*/
);

FRESULT ffext_write (	
	const XCHAR *path,	/* Pointer to the file name */
	const void *buff,	/* Pointer to the data to be written */
	UINT nobtw,			/* Number of bytes to write */
	BYTE mode			/* Access mode and file open mode flags */	
);

FRESULT ffext_read (	
	const XCHAR *path,	/* Pointer to the file name */
	void *buff,			/* Pointer to the data to be written */
	UINT nobtr,			/* Number of bytes to read */
	BYTE mode			/* Access mode and file open mode flags */	
);

FRESULT ffext_scanfiles (
	char *path			/* Pointer to the file name */	
);

#endif
