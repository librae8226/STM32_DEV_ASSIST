/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : gps.h
* Author             : Librae
* Last Modified Date : 08/19/2010
* Description        : Provide GPS related functions and variables.

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __GPS_H__
#define __GPS_H__

/* Includes ---------------------------------------------------------------- */
/* Exported types ---------------------------------------------------------- */

typedef struct type_GPS_dat
{
	unsigned char	status;
	unsigned char	lat_ew;
	unsigned char	latitude[11];
	unsigned char	long_ns;
	unsigned char	longitude[12];
	unsigned char	altitude[6];
	unsigned char	date[7];
	unsigned char	time[11];
	unsigned char	satellites[3];
	unsigned char	satellites_inview[3];
	unsigned char	satellites_id[3];
	unsigned char	speed;
	//int		direction;
	float			hdop;
}t_GPS_data;

/* Exported constants ------------------------------------------------------ */
/* Exported macro ---------------------------------------------------------- */
/* Exported variables ------------------------------------------------------ */
extern unsigned char GPS_keyword[5];
extern unsigned char GPS_data_buff[80];
extern unsigned char *p_GPS_data_buff;
extern unsigned char *p_GPS_keyword;

extern t_GPS_data GPS_data;

extern BOOLEAN flag_GPS_data_updated;
extern BOOLEAN flag_GPS_data_processed;

/* Exported functions ------------------------------------------------------ */

void GPS_Test(void);

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
