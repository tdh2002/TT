
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#ifndef __SPI_D_H_
#define __SPI_D_H_

#define FOCAL_DATA_ADD	0x10000000
#define GROUP_DATA_ADD	0x20000000
#define TGC_DATA_ADD	0x40000000
#define FIR_DATA_ADD	0x80000000

typedef struct _Group_data
{
/* s_group_reg (-1) 地址 */
unsigned short	offset;		/* bit:0-15 地址的偏移 */
unsigned short	TT:12;		/* bit:16-27 保留*/
unsigned char	addr:4;		/* bit:28-31 片选 Group取值0010 */

/* s_group_reg (0) */
unsigned char	freq_band:4;			/* bit:0-3 频带选择 */
unsigned char	video_filter:1;			/* bit:4   视频滤波 */
unsigned char	rectifier:2;			/* bit:5-6 检波		*/
unsigned short	compress_rato:12;		/* bit:7-18  压缩比	*/
unsigned short	gain:13;				/* bit:19-31 Gain	*/

/* s_group_reg (1) */
unsigned char	tcg_point_qty;			/* bit:0-7 TCG点个数 */
unsigned char	tcg_en:1;				/* bit:8   TCG使能	 */
unsigned char	UT2:1;					/* bit:9  UT2		 */	
unsigned char	UT1:1;					/* bit:10 UT1		 */
unsigned char	PA:1;					/* bit:11 PA	 	 */
unsigned int	sample_start:20;		/* bit:12-31 采样起点*/

/* s_group_reg (2) */
unsigned short	sum_gain:12;			/* bit:0-11 Sum gain */
unsigned int	sample_range:20;		/* bit:12-31 capture end 采样范围 */

/* s_group_reg (3) */
unsigned short	beam_qty:13;			/* bit:0-12 聚焦法则数量 */
unsigned int	sample_offset:19;		/* bit:13-31 采样延时 */

/* s_group_reg (4) */
unsigned int	rx_time:20;				/* bit:0-19 rx_time */
unsigned char	TT1:2;
unsigned short	gain1:10;				/* bit:22-31 gain1 */

/* s_config_reg (5) */
unsigned int	idel_time:27;			/* bit:0-26 idel time  */
unsigned char	TT2:5;		

/* s_config_reg (6) */
unsigned short	gate_a_high:12;			/* bit:0-11 闸门A 高度 */
unsigned int	gate_a_start:24;		/* bit:12-31 闸门A 开始 */

#if 0
//s_config_reg (7)
unsigned int gate_c_high  :12;   //bit(11--0)Gate A set Bit( 11..0 )
unsigned int gate_c_start :20;   //bit(31--12)A high  bit 31 ..12  A start

//s_config_reg (8)
unsigned int gate_c_end_res :12; //bit(11 --0)保留
unsigned int gate_c_end :20;     //bit(31--12)Gate A logic  bit 31 ..12  A end

//s_config_reg (9)	
unsigned int idle_time :27;      //bit(26--0)控制idle时间 
unsigned int TCG_point :5;       //bit(31--27)TCG点个数

//s_config_reg(10)
unsigned int tx_time : 13;       //bit(12 -- 0)控制发送时间  
unsigned int tx_time_res : 3;    //bit(15--13)保留	
unsigned int sum_gain  :16;      //bit(31--16)Sum gain 

//s_config_reg(11)
unsigned int rx_time : 20;       //bit(19 -- 0)控制接收时间
unsigned int rx_time_res : 12;   //bit(31--18)保留

//s_config_reg(12-13)
DWORD RX_SEL;// 串并发送总共64bit
//s_config_reg(14-15)
DWORD TX_SEL;// 串并接收总共64bit

	
//s_config_reg(16--47)
unsigned int DDRIO_D1_START :14;  //bit(13--0)发送第1路DDRIO的输出延时与脉冲宽度的开始
unsigned int DDRIO_D1_RS :2;      //bit(15--14)保留	
unsigned int DDRIO_D1_END :14;    //bit(29--16)发送第1路DDRIO的输出延时与脉冲宽度的结束
unsigned int rx_en : 1;           //bit(30)接收使能
unsigned int tx_en : 1;           //bit(31)发送使能

unsigned int DDRIO_D2_START :14;  //bit(13--0)
unsigned int DDRIO_D2_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D2_END   :14;  //bit(29--16)	
unsigned int DDRIO_D2_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D3_START :14;  //bit(13--0)
unsigned int DDRIO_D3_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D3_END   :14;  //bit(29--16)	
unsigned int DDRIO_D3_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D4_START :14;  //bit(13--0)
unsigned int DDRIO_D4_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D4_END   :14;  //bit(29--16)	
unsigned int DDRIO_D4_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D5_START :14;  //bit(13--0)
unsigned int DDRIO_D5_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D5_END   :14;  //bit(29--16)	
unsigned int DDRIO_D5_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D6_START :14;  //bit(13--0)
unsigned int DDRIO_D6_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D6_END   :14;  //bit(29--16)	
unsigned int DDRIO_D6_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D7_START :14;  //bit(13--0)
unsigned int DDRIO_D7_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D7_END   :14;  //bit(29--16)	
unsigned int DDRIO_D7_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D8_START :14;  //bit(13--0)
unsigned int DDRIO_D8_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D8_END   :14;  //bit(29--16)	
unsigned int DDRIO_D8_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D9_START :14;  //bit(13--0)
unsigned int DDRIO_D9_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D9_END   :14;  //bit(29--16)	
unsigned int DDRIO_D9_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D10_START :14;  //bit(13--0)
unsigned int DDRIO_D10_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D10_END   :14;  //bit(29--16)	
unsigned int DDRIO_D10_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D11_START :14;  //bit(13--0)
unsigned int DDRIO_D11_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D11_END   :14;  //bit(29--16)	
unsigned int DDRIO_D11_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D12_START :14;  //bit(13--0)
unsigned int DDRIO_D12_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D12_END   :14;  //bit(29--16)	
unsigned int DDRIO_D12_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D13_START :14;  //bit(13--0)
unsigned int DDRIO_D13_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D13_END   :14;  //bit(29--16)	
unsigned int DDRIO_D13_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D14_START :14;  //bit(13--0)
unsigned int DDRIO_D14_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D14_END   :14;  //bit(29--16)	
unsigned int DDRIO_D14_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D15_START :14;  //bit(13--0)
unsigned int DDRIO_D15_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D15_END   :14;  //bit(29--16)	
unsigned int DDRIO_D15_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D16_START :14;  //bit(13--0)
unsigned int DDRIO_D16_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D16_END   :14;  //bit(29--16)	
unsigned int DDRIO_D16_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D17_START :14;  //bit(13--0)
unsigned int DDRIO_D17_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D17_END   :14;  //bit(29--16)	
unsigned int DDRIO_D17_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D18_START :14;  //bit(13--0)
unsigned int DDRIO_D18_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D18_END   :14;  //bit(29--16)	
unsigned int DDRIO_D18_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D19_START :14;  //bit(13--0)
unsigned int DDRIO_D19_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D19_END   :14;  //bit(29--16)	
unsigned int DDRIO_D19_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D20_START :14;  //bit(13--0)
unsigned int DDRIO_D20_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D20_END   :14;  //bit(29--16)	
unsigned int DDRIO_D20_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D21_START :14;  //bit(13--0)
unsigned int DDRIO_D21_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D21_END   :14;  //bit(29--16)	
unsigned int DDRIO_D21_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D22_START :14;  //bit(13--0)
unsigned int DDRIO_D22_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D22_END   :14;  //bit(29--16)	
unsigned int DDRIO_D22_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D23_START :14;  //bit(13--0)
unsigned int DDRIO_D23_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D23_END   :14;  //bit(29--16)	
unsigned int DDRIO_D23_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D24_START :14;  //bit(13--0)
unsigned int DDRIO_D24_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D24_END   :14;  //bit(29--16)	
unsigned int DDRIO_D24_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D25_START :14;  //bit(13--0)
unsigned int DDRIO_D25_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D25_END   :14;  //bit(29--16)	
unsigned int DDRIO_D25_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D26_START :14;  //bit(13--0)
unsigned int DDRIO_D26_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D26_END   :14;  //bit(29--16)	
unsigned int DDRIO_D26_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D27_START :14;  //bit(13--0)
unsigned int DDRIO_D27_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D27_END   :14;  //bit(29--16)	
unsigned int DDRIO_D27_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D28_START :14;  //bit(13--0)
unsigned int DDRIO_D28_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D28_END   :14;  //bit(29--16)	
unsigned int DDRIO_D28_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D29_START :14;  //bit(13--0)
unsigned int DDRIO_D29_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D29_END   :14;  //bit(29--16)	
unsigned int DDRIO_D29_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D30_START :14;  //bit(13--0)
unsigned int DDRIO_D30_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D30_END   :14;  //bit(29--16)	
unsigned int DDRIO_D30_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D31_START :14;  //bit(13--0)
unsigned int DDRIO_D31_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D31_END   :14;  //bit(29--16)	
unsigned int DDRIO_D31_RS0   :2;   //bit(31--30)

unsigned int DDRIO_D32_START :14;  //bit(13--0)
unsigned int DDRIO_D32_RS1   :2;   //bit(15--14)
unsigned int DDRIO_D32_END   :14;  //bit(29--16)	
unsigned int DDRIO_D32_RS0   :2;   //bit(31--30)
                     
//	s_config_reg(48--63)
unsigned int FOCUS1_LAW_DELAY      :12;    //bit(11--0)控制第1路接收波形的延时相位
unsigned int FOCUS1_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS17_LAW_DELAY     :12;   //(27--16) 控制第17路接收波形的延时相位
unsigned int FOCUS1_LAW_DELAY_RS0  :4;     //bit(31--28)	
	
unsigned int FOCUS2_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS2_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS18_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS2_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS3_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS3_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS19_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS3_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS4_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS4_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS20_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS4_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS5_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS5_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS21_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS5_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS6_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS6_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS22_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS6_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS7_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS7_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS23_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS7_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS8_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS8_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS24_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS8_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS9_LAW_DELAY      :12;    //bit(11--0)	
unsigned int FOCUS9_LAW_DELAY_RS1  :4;     //bit(15--12)
unsigned int FOCUS25_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS9_LAW_DELAY_RS0  :4;     //bit(31--28)

unsigned int FOCUS10_LAW_DELAY     :12;    //bit(11--0)	
unsigned int FOCUS10_LAW_DELAY_RS1 :4;     //bit(15--12)
unsigned int FOCUS26_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS10_LAW_DELAY_RS0 :4;     //bit(31--28)

unsigned int FOCUS11_LAW_DELAY     :12;    //bit(11--0)	
unsigned int FOCUS11_LAW_DELAY_RS1 :4;     //bit(15--12)
unsigned int FOCUS27_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS11_LAW_DELAY_RS0 :4;     //bit(31--28)

unsigned int FOCUS12_LAW_DELAY     :12;    //bit(11--0)	
unsigned int FOCUS12_LAW_DELAY_RS1 :4;     //bit(15--12)
unsigned int FOCUS28_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS12_LAW_DELAY_RS0 :4;     //bit(31--28)

unsigned int FOCUS13_LAW_DELAY     :12;    //bit(11--0)	
unsigned int FOCUS13_LAW_DELAY_RS1 :4;     //bit(15--12)
unsigned int FOCUS29_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS13_LAW_DELAY_RS0 :4;     //bit(31--28)

unsigned int FOCUS14_LAW_DELAY     :12;    //bit(11--0)	
unsigned int FOCUS14_LAW_DELAY_RS1 :4;     //bit(15--12)
unsigned int FOCUS30_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS14_LAW_DELAY_RS0 :4;     //bit(31--28)

unsigned int FOCUS15_LAW_DELAY     :12;    //bit(11--0)	
unsigned int FOCUS15_LAW_DELAY_RS1 :4;     //bit(15--12)
unsigned int FOCUS31_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS15_LAW_DELAY_RS0 :4;     //bit(31--28)


unsigned int FOCUS16_LAW_DELAY     :12;    //bit(11--0)	
unsigned int FOCUS16_LAW_DELAY_RS1 :4;     //bit(14--12)
//unsigned int RX_CHANNEL16_EN       :1;     //bit(15)
unsigned int FOCUS32_LAW_DELAY     :12;    //bit(27--16)
unsigned int FOCUS16_LAW_DELAY_RS0 :4;     //bit(30--28)
//unsigned int RX_CHANNEL32_EN       :1;     //bit(31)
#endif

} int32_reg;

#endif
