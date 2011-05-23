
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include <stdio.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define ARRAY_DEVICE	"/dev/spidev1.0"
#define NORMAL_DEVICE	"/dev/spidev3.1"

#include "spi_d.h"
#define DEBUG 1

static int fd_array, fd_normal;

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

void init_spi ()
{
	int val = 0x01;

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
/*
	write(fd, (unsigned char *)fpga_reg_p, sizeof(fpga_reg));

	write(fd1, str, 2);	
*/
	return ;
}

int write_group_data (group_data_spi *p, unsigned int group)
{

#if DEBUG
	unsigned int tmp = p->gain;
	printf ("Gain = %d\n", tmp);
#endif
	p->offset = 16 * group;
	p->addr = 0x2;
	little_to_big ((unsigned int *)(p), sizeof(group_data_spi) / 4);
	
	return write (fd_array, (unsigned char *)p, sizeof(group_data_spi));
}

void write_focal_data ( )
{
}

void write_tgc_data ()
{
}

void write_fir_data ()
{
}

void write_normal_data ()
{
}


