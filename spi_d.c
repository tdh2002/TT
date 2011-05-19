
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include <stdio.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define ARRAY_DEVICE	"/dev/spidev0.1"
#define NORMAL_DEVICE	"/dev/spidev3.1"

#include "spi_d.h"

#define write_spi   0
#define read_spi    1       

static int fd_adc, fd_fpga;

void init_spi ()
{
	int val = 0x01;

	/*spi0 3设备 初始化skyworks*/
	if ((fd_fpga = open(NORMAL_DEVICE, O_RDWR)) == -1) 
	{
		perror(NORMAL_DEVICE);
		return ;
	}

	if ((fd_adc = open(ARRAY_DEVICE, O_RDWR)) == -1) 
	{
		perror(ARRAY_DEVICE);
		return ;
	}
	if (ioctl(fd_fpga, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error2\n");
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd_fpga, SPI_IOC_WR_MODE, &val) == -1) 
	{
		perror("ioctl spi write error3\n");
		return ;
	}
	if (ioctl(fd_fpga, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error4\n");
		return ;
	}
	if (ioctl(fd_adc, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error2\n");
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd_adc, SPI_IOC_WR_MODE, &val) == -1) 
	{
		perror("ioctl spi write error3\n");
		return ;
	}
	if (ioctl(fd_adc, SPI_IOC_RD_MODE, &val) == -1) 
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

void write_group_data (int offset, int val)
{
	/*
	write (fd, (unsigned char *)fpga_reg_p, sizeof(fpga_reg));
	*/
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


