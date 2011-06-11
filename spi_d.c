
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include <stdio.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "drawui.h"
#define ARRAY_DEVICE	"/dev/spidev1.0"
#define NORMAL_DEVICE	"/dev/spidev3.1"
#define GPIO_DEVICE		"/dev/tt"

#define GPIO61_LOW	0x6000
#define GPIO61_HIGH	0x6001
#define GPIO43_LOW	0x6002
#define GPIO43_HIGH	0x6003


#include "spi_d.h"
#define DEBUG 0

static int fd_array, fd_normal;
static int fd_gpio;

/* n为siezof(p)/4 */
static void little_to_big(unsigned int *p, int n)
{
	unsigned int i = 0;
	unsigned int *pt = p;
	for (i = 0; i < n; ++i, ++pt)
	{
		*pt = ((*pt) <<24) | ((*pt<<8) & 0x00ff0000)|((*pt>>8) & 0x0000ff00) | ((*pt) >>24);
	}
	return ;
}

/*  */
DWORD  channel_select(unsigned int n)
{ 
	int i;
	int temp[33];
	int L=0,H=0;
	DWORD tx_select;
	if((n > 97) || (n <1))
		exit(1);

	for(i=n; i<n + 32; ++i)
	{
		if(i%32  == 0)
		{
			L = 32;
			H = i/32 -1;
		}
		else
		{
			L = i%32;
			H = i/32;
		}

		if(H == 1) 
			temp[L] = 2;
		else if(H == 2)
			temp[L] = 1;
		else
			temp[L] = H;  
	}    


	tx_select.RX_TX_SEL_H =  (temp[1]<<30) | (temp[2]<<28)  | (temp[3]<<26)  | (temp[4]<<24)  |
		(temp[5]<<22) | (temp[6]<<20)  | (temp[7]<<18)  | (temp[8]<<16)  |
		(temp[9]<<14) | (temp[10]<<12) | (temp[11]<<10) | (temp[12]<<8 ) |
		(temp[13]<<6) | (temp[14]<<4)  | (temp[15]<<2)  | temp[16];

	tx_select.RX_TX_SEL_L =  (temp[17]<<30) | (temp[18]<<28) | (temp[19]<<26) | (temp[24]<<24) |
		(temp[21]<<22) | (temp[22]<<20) | (temp[23]<<18) | (temp[24]<<16) |
		(temp[25]<<14) | (temp[26]<<12) | (temp[27]<<10) | (temp[28]<<8 ) |
		(temp[29]<<6)  | (temp[30]<<4)  | (temp[31]<<2)  | temp[32];

	return tx_select;
}

void init_spi ()
{
	int val = 0x01;

	if ((fd_gpio = open (GPIO_DEVICE, O_RDWR)) == -1)
	{
		perror(GPIO_DEVICE);
		return ;
	}


	/*spi0 3设备 初始化skyworks*/
	if ((fd_normal = open(NORMAL_DEVICE, O_RDWR)) == -1) 
	{
		perror(NORMAL_DEVICE);
		return ;
	}

	if ((fd_array = open(ARRAY_DEVICE, O_RDWR)) == -1) 
	{
		perror(ARRAY_DEVICE);
		return ;
	}
	if (ioctl(fd_normal, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error2\n");
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd_normal, SPI_IOC_WR_MODE, &val) == -1) 
	{
		perror("ioctl spi write error3\n");
		return ;
	}
	if (ioctl(fd_normal, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error4\n");
		return ;
	}
	if (ioctl(fd_array, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error2\n");
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd_array, SPI_IOC_WR_MODE, &val) == -1) 
	{
		perror("ioctl spi write error3\n");
		return ;
	}
	if (ioctl(fd_array, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error4\n");
		return ;
	}

	ioctl (fd_gpio, GPIO61_HIGH, &val);
/*
	write(fd, (unsigned char *)fpga_reg_p, sizeof(fpga_reg));

	write(fd1, str, 2);	
*/
	return ;
}

int write_group_data (group_data_spi *p, unsigned int group)
{
	group_data_spi new, *p1;
	int i;
	memcpy (&new, p, sizeof (group_data_spi));
	p1 = &new;

#if DEBUG
/*	unsigned int *tmp = (unsigned *)(p);*/
	unsigned int tmp = p->gain;
	printf ("Gain= %d\n", tmp);
	tmp = p->freq_band;
	printf ("Freq_band= %d\n", tmp);
	tmp = p->rectifier;
	printf ("Rectifier= %d\n", tmp);
	tmp = p->video_filter;
	printf ("Video_filter= %d\n", tmp);
	tmp = p->sum_gain;
	printf ("sun gain= %d\n", tmp);
	tmp = p->reject;
	printf ("reject= %d\n", tmp);
	tmp = p->voltage;
	printf ("voltage= %d\n", tmp);
#endif

	p->offset = 16 * group;
	p->addr = 0x2;
	little_to_big ((unsigned int *)(p1), sizeof(group_data_spi) / 4);
	
	ioctl (fd_gpio, GPIO43_LOW, &i);
	i = write (fd_array, (unsigned char *)(p1), sizeof(group_data_spi));
	ioctl (fd_gpio, GPIO43_HIGH, &i);
	return 0;
}

int write_focal_data (focal_data_spi *p, unsigned int beam_num)
{
	focal_data_spi new, *p1;
	int i;
	memcpy (&new, p, sizeof (focal_data_spi));
	p1 = &new;

#if DEBUG
	/*
	unsigned int tmp = p->gain;
	printf ("Gain= %d\n", tmp);
	tmp = p->freq_band;
	*/
#endif

	p->offset = 64 * beam_num;
	p->addr = 0x1;
	little_to_big ((unsigned int *)(p1), sizeof(focal_data_spi) / 4);

	ioctl (fd_gpio, GPIO43_LOW, &i);
	i = write (fd_array, (unsigned char *)(p1), sizeof(focal_data_spi));
	ioctl (fd_gpio, GPIO43_HIGH, &i);

	return 0;
}

void write_tgc_data ()
{
}

void write_fir_data ()
{
}

void write_normal_data (group_data_spi *p)
{
	group_data_spi new, *p1;
	int i;
	memcpy (&new, p, sizeof (group_data_spi));
	p1 = &new;

#if DEBUG
/*	unsigned int *tmp = (unsigned *)(p);*/
	unsigned int tmp = p->gain;
	printf ("Gain= %d\n", tmp);
	tmp = p->freq_band;
	printf ("Freq_band= %d\n", tmp);
	tmp = p->rectifier;
	printf ("Rectifier= %d\n", tmp);
	tmp = p->video_filter;
	printf ("Video_filter= %d\n", tmp);
	tmp = p->sum_gain;
	printf ("sun gain= %d\n", tmp);
	tmp = p->reject;
	printf ("reject= %d\n", tmp);
	tmp = p->voltage;
	printf ("voltage= %d\n", tmp);
#endif

	p->offset = 0;
	p->addr = 0x2;
	little_to_big ((unsigned int *)(p1), sizeof(group_data_spi) / 4);
	
	ioctl (fd_gpio, GPIO43_LOW, &i);
	i = write (fd_normal, (unsigned char *)(p1), sizeof(group_data_spi));
	ioctl (fd_gpio, GPIO43_HIGH, &i);
	return ;
}


