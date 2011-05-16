
#include <stdio.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define DAC_DEVICE "/dev/spidev0.1"
#define FPGA_DEVICE "/dev/spidev3.1"
#define ADC_DEVICE "/dev/spidev3.0"

#include "twm_spi.h"

//---此处理器为小端模式----------------------
#define write_spi   0        //1b  //选择聚焦命令1b
#define read_spi    1       //9b  //聚焦次数9b

int init_spi(int argc, char *argv[])
{
	int val = 0x01;
	int fd ,fd1;
	int i=0;
	int j = 0;
	unsigned char reg;
	unsigned char data ;
	int32_reg fpga_reg;
	int32_reg *fpga_reg_p = &fpga_reg;
	ad_reg  ad_spi_reg;
	ad_reg *ad_spi_reg_p = &ad_spi_reg;

	
	
	
  unsigned char *str= (unsigned char *)ad_spi_reg_p;

	//---------configure--fpga_spi-----------------------------------------
//	initial_fpga_reg(fpga_reg_p);	

	
	/*spi0 3设备 初始化skyworks*/
	if ((fd = open(FPGA_DEVICE, O_RDWR)) == -1) 
		{
		perror(FPGA_DEVICE);
		return ;
	 }
	 
	 	if ((fd1 = open(ADC_DEVICE, O_RDWR)) == -1) 
		{
		perror(ADC_DEVICE);
		return ;
	 }
//-----------fpga__spi------------------------------------------
	if (ioctl(fd, SPI_IOC_RD_MODE, &val) == -1) 
		{
		perror("ioctl spi read error2\n");
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd, SPI_IOC_WR_MODE, &val) == -1) 
		{
		perror("ioctl spi write error3\n");
		return ;
	}
	if (ioctl(fd, SPI_IOC_RD_MODE, &val) == -1) 
		{
		perror("ioctl spi read error4\n");

		return ;
	}
//-----ad_spi----------------------------------------------------

	if (ioctl(fd1, SPI_IOC_RD_MODE, &val) == -1) 
		{
		perror("ioctl spi read error2\n");
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd1, SPI_IOC_WR_MODE, &val) == -1) 
		{
		perror("ioctl spi write error3\n");
		return ;
	}
	if (ioctl(fd1, SPI_IOC_RD_MODE, &val) == -1) 
		{
		perror("ioctl spi read error4\n");

		return ;
	}
	
		


//  little_big((unsigned int *)fpga_reg_p,sizeof(fpga_reg)/4);//

	write(fd, (unsigned char *)fpga_reg_p, sizeof(fpga_reg));

//while(1)
{
	//close(fd);
//---------configure--write ad_spi-----------------------------------------
//  initial_ad_reg(ad_spi_reg_p,reg, data,write_spi);
  
  
	write(fd1, str, 2);	
#if 0
  //--read-spi----------------------------
  initial_ad_reg(ad_spi_reg_p, read_spi);
	write(fd1, str, 18);	
#endif	
}
	return 0;
}

