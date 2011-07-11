
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * base_#define.h
 *
 * 各种常量的定义
 *
 * 谭登华
 * 2011-01-15
 *
 */


#ifndef __BASE_const_H_
#define __BASE_const_H_

/* language choose 语言选择 */
#define ENGLISH_	0
#define CHINESE_	1

/* version 版本信息 */
#define VERSION "DP1.0.0.0"

/* GROUP qty 数量 */
#define setup_MAX_GROUP_QTY	8

/* 工件形状 */
#define PLATE_PART	0
#define OD_PART		1
#define ID_PART		2
/* 扫查模式 */
#define UT_SCAN		0
#define PA_SCAN		1
#define	UT1_SCAN	2
/* 电压 P113 */
#define VOLTAGE_HIGH	2
#define VOLTAGE_MID		1
#define VOLTAGE_LOW		0

/* 显示模式ABSC */
#define	DISPLAY_ALL_GROUP	0
#define DISPLAY_CURRENT_GROUP	1

/* 扫描  */
#define A_SCAN	     0
#define A_SCAN_R		0x20
#define B_SCAN	     1
#define C_SCAN	     2
#define CC_SCAN		0x22
#define CCC_SCAN	0x32
#define S_SCAN	     3			/*  */
#define S_SCAN_A		0x23	/* truedepath 的角度扫查 */
#define S_SCAN_L		0x33	/* truedepth 的线扫 */
#define A_B_SCAN     4
#define A_B_C_SCAN	 5
#define A_B_S_SCAN	 6
#define A_C_CC_SCAN	 7
#define A_S_CC_SCAN	 8
#define PA_TOFD	         9
#define Strip_Chart_AA	 10

/* c扫描的source选项 */
#define C_SCAN_A_HEIGHT		0
#define C_SCAN_B_HEIGHT		1
#define C_SCAN_THICKNESS	2
#define C_SCAN_I_GATE		3
#define C_SCAN_OFF			4

/* strip chart data 选项 */
#define STRIP_SCAN_A_HEIGHT		0
#define STRIP_SCAN_B_HEIGHT		1
#define STRIP_SCAN_THICKNESS	2
#define STRIP_SCAN_OFF			3

/* strip chart mode 选项 */
#define STRIP_SCAN_ALL_ASCAN	0
#define STRIP_SCAN_CUR_SCAN		1
#define STRIP_SCAN_ALL			2
#define STRIP_SCAN_CUR			3

/* 报警条件 */
#define ALARM_NONE		0
#define ALARM_GATEA		1
#define ALARM_GATEB		2
#define ALARM_GATEI		3
#define ALARM_NOT_GATEA	4
#define ALARM_NOT_GATEB	5
#define ALARM_NOT_GATEI	6
#define ALARM_MAX_THICKNESS	7
#define ALARM_MIN_THICKNESS	8

/* 操作AND OR */
#define OPERATOR_AND	0
#define OPERATOR_OR		1

/* 报警输出选项 */
#define OUTPUT1	0
#define OUTPUT2	1
#define OUTPUT3	2
#define ANALOG1	3
#define ANALOG2	4

#define ALARM_OFF	0
#define ALARM_ON	1

/* 报警声音 */
#define SOUND_OFF	0
#define SOUND_300	1
#define SOUND_600	2
#define SOUND_1000	3
#define SOUND_1500	4

/**/
#define OUTPUT_DATA_OFF	0
#define OUTPUT_DATA_A	1
#define OUTPUT_DATA_B	2
#define OUTPUT_DATA_THICKNESS	3

#define GRP_1	0
#define GRP_2	1
#define GRP_3	2
#define GRP_4	3
#define GRP_5	4
#define GRP_6	5
#define GRP_7	6
#define GRP_8	7


#define	NORMAL_OFF	0
#define	NORMAL_ON	1


/* Range of availage voltage (sPulseVoltagePA). 相控发射电压 */
#define setup_VOLTAGE_MIN_PA   0x100 
#define	setup_VOLTAGE_MED_PA   0x101 
#define setup_VOLTAGE_MAX_PA   0x102 

/* Range of availage voltage (sPulseVoltageUT). 常规发射电压 */
#define setup_VOLTAGE_MIN_UT   0x200 
#define setup_VOLTAGE_MED_UT   0x201 
#define setup_VOLTAGE_MAX_UT   0x202 

/* Maximum quantity of battery. 2个电池 */
#define setup_MAX_QTY_BATTERY   2 

/* Maximum quantity of encoder and bit definition. 2个编码器 */
#define setup_MAX_QTY_ENCODER   2 
#define setup_ENCODER_0         0x00000001 
#define setup_ENCODER_1         0x00000002 

/* Different counter types for encoder input (cType). 编码器的输入类型 */
#define setup_ENC_CLK_DIR      0 /* Clock direction counter.*/
#define setup_ENC_QUADRATURE   1 /* Phase A-Phase B counter.*/
#define setup_ENC_DOWN         2 /* Counter DOWN.*/
#define setup_ENC_UP           3 /* Counter UP.*/

/* Kind of preset source (cPresetSource). */
#define setup_PRESET_EXTERNAL_LINE     0 
#define setup_PRESET_EXT_COMMON_LINE   1 
#define setup_PRESET_NOT_USE           2 
#define setup_PRESET_INTERNAL          3 

/* Kind of divider mode (cAcqDividerMode). Divider is the number of encoder steps required to change cell.
 * 
 */
#define setup_DIVIDER_MODE_BOTH   0  // consider when encoder steps are incrementing or decrementing
#define setup_DIVIDER_MODE_UP     1  // consider only when encoder steps are incrementing
#define setup_DIVIDER_MODE_DOWN   2  // consider only when encoder steps are decrementing

/* Maximum quantity of gate.
 * 闸门数量 
 */
#define setup_MAX_GATE_SYNCHRO   1 
#define setup_MAX_GATE_MEASURE   2 
#define setup_MAX_GATE           3   /* (setup_MAX_GATE_SYNCHRO + setup_MAX_GATE_MEASURE) */

/* Bit mask for active data in read format.
 *
 */
#define setup_MASK_ENCODER_0     0x0001 
#define setup_MASK_ENCODER_1     0x0002 
#define setup_MASK_IO            0x0004 
#define setup_MASK_GATE_0        0x0001  // Sync gate
#define setup_MASK_GATE_1        0x0002 
#define setup_MASK_GATE_2        0x0004 
#define setup_MASK_SAMPLE_SIZE   0x0020 
#define setup_MASK_ASCAN         0x0080 

/* Maximum quantity of laws. 聚焦法则 */
#define setup_MAX_LAW_QTY   256 

/* Maximum input gain for receivers in tenth of dB */
#define setup_MAX_INPUT_GAIN   800 

/* Quantity of gain entry for values 0 to 80dB by steps of 0.05dB */
#define setup_GAIN_ENTRIES_QTY   1601 

/* Maximum count for the gain calibration table. */
#define setup_MAX_COUNT_GAIN_TBL   4095  

/* Maximum slope variation for gain modification in dB per micro-second */
#define setup_MAX_SLOPE   20 

/* constant around TX and RX delay (usDelayRx and usDelayTx). */
#define setup_ELEM_DELAY_OFF      65535 
#define setup_ELEM_DELAY_MIN      0 
#define setup_ELEM_DELAY_MAX      65534 
#define setup_ELEM_DELAY_MAX_TX   51230 
#define setup_ELEM_DELAY_MAX_RX   51210 

/* Minimum and maximum pulse width (in ns  see sPulserWidth). 脉冲宽度 */
#define setup_PULSE_WIDTH_MIN   25 
#define setup_PULSE_WIDTH_MAX   1000 

/* Different kind of pulser (sPulserMode). PP is for embedded pulser and 
receiver. PR is for separate pulser and receiver.收发分离 或者收发一体  */
#define setup_PULSER_PP   0 
#define setup_PULSER_PR   1 

/* Kind of input signal rectification (cInputRectification). 检波 */
#define setup_RECT_NONE   0 
#define setup_RECT_POS    1 
#define setup_RECT_NEG    2 
#define setup_RECT_BOTH   3 

/* Maximum quantity of detected probes. 最多几个探头 */
#define setup_MAX_QTY_PROBE  4 

/* Maximum quantity of element per instrument in Phased Array technology. 最多收发的阵元 */
#define setup_MAX_ELEM_RX   128 
#define setup_MAX_ELEM_TX   128 

/* Maximum quantity of active element per law in Phased Array technology. PA 一次最多激发的阵元数 */
#define setup_MAX_ELEM_RX_ACTIVE   32 
#define setup_MAX_ELEM_TX_ACTIVE   32 

/* Maximum quantity of element per instrument in conventional Ultrasound Technology. UT 一次最多激发的阵元数 */
#define setup_MAX_ELEM_RX_UT   2 
#define setup_MAX_ELEM_TX_UT   2 

/* constant need for arbitrary level definition. */
#define setup_MAX_NODE_QTY   16 

/* Different cutoff frequency choice for the input filter (cInputFilter). Doppler */ 
//#define setup_IN_FILTER_LP_?MHz        ? 
//#define setup_IN_FILTER_LP_...
//#define setup_IN_FILTER_HP_?MHz        ? 
//#define setup_IN_FILTER_HP_...
//#define setup_IN_FILTER_BP_?MHz_?MHz   ? 
//#define setup_IN_FILTER_BP_...
#define setup_IN_FILTER_0_5_MHz_15_MHz	0x10   /* doppler 有2个输入滤波 */
#define	setup_IN_FILTER_1_MHz_10_MHz	0x11   /**/
#define setup_IN_FILTER_NONE			0x12 

/* Different averaging mode (cAveragingType). */
#define setup_AVER_NONE     0 
#define setup_AVER_NORMAL   1 

/* Different cutoff frequency choices available on low-pass filter for smoothing
(see cSmoothingFilter). */
#define setup_FILTER_SM_NONE     0 
//#define setup_FILTER_SM_?MHz   1 
//#define setup_FILTER_SM_...

/* Minimum and maximum acquisition rate (lAcqRAte). */
#define setup_ACQRATE_MIN_TENTH_HZ   1 
#define setup_ACQRATE_MAX_TENTH_HZ   500000 

/* Different types of synchronization sources (cAcqClockSource). 同步源 */
#define setup_SYNC_INTERNAL   0 
#define setup_SYNC_EXTERNAL   1 
#define setup_SYNC_COUNTER1   2 
#define setup_SYNC_COUNTER2   3 

/* List of sampling rate (Digitizing Frequency): 我们现在只有 100MHZ */
#define setup_SAMP_RATE_100_MHZ     0 
#define setup_SAMP_RATE_50_MHZ      1 
#define setup_SAMP_RATE_25_MHZ      2 
#define setup_SAMP_RATE_20_MHZ      3 
#define setup_SAMP_RATE_12_5_MHZ    4 
#define setup_SAMP_RATE_10_MHZ      5 
#define setup_SAMP_RATE_6_25_MHZ    6 
#define setup_SAMP_RATE_5_MHZ       7 
#define setup_SAMP_RATE_3_125_MHZ   8 
#define setup_SAMP_RATE_MAX         9 

/* Maximum delay before acquisition (lAcqDelay in ns). Doppler */
#define setup_ACQ_DELAY_MIN   51200 
#define setup_ACQ_DELAY_MAX   51200 

/* Maximum delay before A-scan measurement in echo synchronization (in ns). Doppler */
#define setup_SYNC_DELAY_MAX   51200 

/* Maximum A-scan length (in sample)  8K   (8 * 1024) A扫长度 MIN 10 MAX 8192 */
#define setup_ASCAN_LEN_MIN   10 
#define setup_ASCAN_LEN_MAX   8192 

/* Type of detection (cTrigModeGate). 普通模式下  波峰 或者波前  */
#define setup_GATEMODE_PEAK       0 
#define setup_GATEMODE_CROSSING   1 

/* Type of polarity for a gate (cPolarity). 闸门逻辑选项 */
#define setup_POLARITY_POS    1 
#define setup_POLARITY_NEG    2 
#define setup_POLARITY_BOTH   3 

/* Type of detection (cTrigModeSynchro). 同步模式 */
#define setup_SYNCMODE_PULSER   0 
#define setup_SYNCMODE_ECHO     1 

/* Type of compression. (cCompressionType) 压缩 */
#define setup_COMPRESSION_NONE   0 
#define setup_COMPRESSION_POS    1 
#define setup_COMPRESSION_NEG    2 
#define setup_COMPRESSION_BOTH   3 

/* Definition for logical operator in alarm generation. */
#define setup_ALARM_INVERT_RESULT   0x8000 
#define setup_ALARM_USE_GATE0       0x0001 
#define setup_ALARM_INVERT_GATE0    0x0002 
#define setup_ALARM_USE_GATE1       0x0004 
#define setup_ALARM_INVERT_GATE1    0x0008 
#define setup_ALARM_USE_GATE2       0x0010 
#define setup_ALARM_INVERT_GATE2    0x0020 
#define setup_ALARM_USE_GATE3       0x0040 
#define setup_ALARM_INVERT_GATE3    0x0080 
#define setup_ALARM_USE_GATE4       0x0100 
#define setup_ALARM_INVERT_GATE4    0x0200 

/* Definition for the maximum quantity of internal alarm occurrence before an
	external alarm is generated.
	
	*/
#define setup_ALARM_MAX_STAT	32  

#endif
