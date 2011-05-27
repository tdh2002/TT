
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
unsigned int	offset:16;		/* bit:0-15 地址的偏移 */
unsigned int	TT:12;		/* bit:16-27 保留*/
unsigned int	addr:4;		/* bit:28-31 片选 Group取值0010 */

/* s_group_reg (0) */
unsigned int	freq_band:4;			/* bit:0-3 频带选择 */
unsigned int	video_filter:1;			/* bit:4   视频滤波 */
unsigned int	rectifier:2;			/* bit:5-6 检波		*/
unsigned int	compress_rato:12;		/* bit:7-18  压缩比	*/
unsigned int	TT9:3;					/*  */
unsigned int	gain:10;				/* bit:22-31 Gain	*/

/* s_group_reg (1) */
unsigned int	tcg_point_qty:8;			/* bit:0-7 TCG点个数 */
unsigned int	tcg_en:1;				/* bit:8   TCG使能	 */
unsigned int	UT2:1;					/* bit:9  UT2		 */	
unsigned int	UT1:1;					/* bit:10 UT1		 */
unsigned int	PA:1;					/* bit:11 PA	 	 */
unsigned int	sample_start:20;		/* bit:12-31 采样起点*/

/* s_group_reg (2) */
unsigned int	sum_gain:12;			/* bit:0-11 Sum gain */
unsigned int	sample_range:20;		/* bit:12-31 capture end 采样范围 */

/* s_group_reg (3) */
unsigned int	beam_qty:13;			/* bit:0-12 聚焦法则数量 */
unsigned int	sample_offset:19;		/* bit:13-31 采样延时 */

/* s_group_reg (4) */
unsigned int	rx_time:20;				/* bit:0-19 rx_time */
unsigned int	TT1:2;
unsigned int	gain1:10;				/* bit:22-31 gain1 */

/* s_group_reg (5) */
unsigned int	idel_time:27;			/* bit:0-26 idel time  */
unsigned int	TT2:5;		

/* s_group_reg (6) */
unsigned int	gate_a_height:12;		/* bit:0-11 闸门A 高度 */
unsigned int	gate_a_start:20;		/* bit:12-31 闸门A 开始 */

/* s_group_reg (7) */
unsigned int	gate_a_logic:8;			/* bit:0-7 闸门A 逻辑 */
unsigned int	gate_a_end:24;			/* bit:12-31 闸门A 结束 */

/* s_group_reg (8) */
unsigned int	gate_b_height:12;		/* bit:0-11 闸门B 高度 */
unsigned int	gate_b_start:20;		/* bit:12-31 闸门B 开始 */

/* s_group_reg (9) */
unsigned int	gate_b_logic:8;			/* bit:0-7 闸门B 逻辑 */
unsigned int	gate_b_end:24;			/* bit:12-31 闸门B 结束 */

/* s_group_reg (10) */
unsigned int	gate_i_height:12;		/* bit:0-11 闸门C 高度 */
unsigned int	gate_i_start:20;		/* bit:12-31 闸门C 开始 */

/* s_group_reg (11) */
unsigned int	gate_i_logic:8;			/* bit:0-7 闸门C 逻辑 */
unsigned int	gate_i_end:24;			/* bit:12-31 闸门C 结束 */

/* s_group_reg (11) */
unsigned int	voltage:8;			/* bit:0-7 发射电压 */
unsigned int	damping:2;			/* bit:8-9 阻尼 */
unsigned int	TT8:10;				/**/
unsigned int	reject:12;			/* bit:20-31 闸门C 结束 */

/* s_group_ref (13-15) */
unsigned int	TT3[3];

} group_data_spi;


extern void init_spi ();
extern int write_group_data (group_data_spi *p, unsigned int group);
#endif
