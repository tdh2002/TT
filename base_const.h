
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
/* 重复频率 */
#define	PRF_AUTO_MAX	0
#define	PRF_AUTO_MAX_2	1
#define	PRF_OPTIMUM		2
#define	PRF_USERDEF		3

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
#define S_SCAN_A 		0x23	/* truedepath 的角度扫查 */
#define S_SCAN_L		0x33	/* truedepth 的线扫 */
#define A_B_SCAN     4
#define A_B_C_SCAN	 5
#define A_B_S_SCAN	 6
#define A_C_CC_SCAN	 7
#define A_S_CC_SCAN	 8
#define PA_TOFD	         9
#define Strip_Chart_AA	 10
#define WEDGE_DELAY      11
#define SENSITIVITY      12
#define TCG              13
#define CODER            14

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


#define FILTER_NONE	0
#define FILTER_AUTO	1

#define	NORMAL_OFF	0
#define	NORMAL_ON	1

/* DISPPLAY properties scan 选项*/
#define DIS_PROP_SCAN_A		0
#define DIS_PROP_SCAN_B		1
#define DIS_PROP_SCAN_C		2
#define DIS_PROP_SCAN_S		3
#define DIS_PROP_SCAN_FFT	4
#define DIS_PROP_SCAN_STRIP	5

/* Display properties fft color */
#define FFT_YELLOW	0
#define FFT_BLUE	1
#define FFT_WHITE	2
#define FFT_BLACK	3

/*  */
#define CHOOSE_PROBE 0
#define DEFINE_PROBE 1

/* 编码器的设置*/
#define ENCODER_1	0
#define ENCODER_2	1 
/* encoder polarity */
#define ENC_NORMAL	0
#define ENC_INVERSE	1
/* encoder type */
#define ENC_CLOCK_DIR	0
#define ENC_QUAD		1
#define ENC_UP			2
#define ENC_DOWN		3
#define ENC_CLOCK_DIR_UP	4
#define ENC_CLOCK_DIR_DOWN	5
#define ENC_QUAD_UP		6
#define ENC_QUAD_DOWN	7
#define ENC_CLICKER		8

/* inspection 设置 */
#define TYPE_ONE_LINE	0 
#define TYPE_RASTER		1
#define TYPE_HELICOIDAL	2 

#define	SCAN_TIME		0
#define	SCAN_ENCODER1	1
#define	SCAN_ENCODER2	2

#define INDEX_AUTO		0
#define INDEX_ENCODER1	1
#define INDEX_ENCODER2	2
#define INDEX_TOP_TURN	3
#define INDEX_OFF		4

#define RESET_ALL		0
#define RESET_ENCODER	1
#define RESET_DATA		2

#define STORAGE_LAST	0
#define STORAGE_A_PERCENT		1
#define STORAGE_MAX_THICKNESS	2
#define STORAGE_MIN_THICKNESS	2

#define INSPEC_DATA_ALL_A_C		0
#define INSPEC_DATA_ALL_C		1

/* File 设置 */
#define STORAGE_CARD	0
#define STORAGE_INTERNAL_MEM	1
#define STORAGE_USB1	2
#define STORAGE_USB2	3

#define SAVE_MODE_INSPEC_DATA	0
#define SAVE_MODE_INSPEC_TABLE	1
#define SAVE_MODE_SCREEN	2
#define SAVE_MODE_REPORT	3

#define REPORT_COMPLETE	0

#define PAPER_LETTER	0
#define PAPER_LEGAL		1
#define PAPER_A4		2
#define PAPER_B4		3


#define FILE_VIEW_TABLE	0
#define FILE_VIEW_CURRENT_LAYOUT	1
#define	FILE_VIEW_OFF	2

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


//各种response消息
#define RESPONSE_FILE_OPEN	1
#define RESPONSE_CLOSE	2 
#define RESPONSE_CONFIG_SAVE	3
#define RESPONSE_CONFIG_FILE_FOCAL	4
#define RESPONSE_SAVE_AND_CLOSE	5
#define RESPONSE_CHANGE_FOCAL	6
#define RESPONSE_SELECT_AND_CLOSE	7
#define RESPONSE_LAW_READ	8
#define RESPONSE_LAW_SAVE	9
#define RESPONSE_SELECT_ALL	10
#define RESPONSE_COPY_FILE	11
#define RESPONSE_MOVE_FILE	12
#define RESPONSE_DELECT_FILE	13
#define RESPONSE_RENAME_FILE	14

/*帮助文档路径*/
#define HELP_WIZARD_PATH 		 "http://127.0.0.1/source/system/Help/Contextual/Help_Wizard_Menu.html"
#define HELP_UT_SETTING_PATH	 "http://127.0.0.1/source/system/Help/Contextual/Help_UT_Settings_Menu.html"
#define HELP_GATE_ALARM_PATH	 "http://127.0.0.1/source/system/Help/Contextual/Help_GateAlarm_Menu.html"
#define HELP_MEASUREMENTS_PATH	 "http://127.0.0.1/source/system/Help/Contextual/Help_Measurements_Menu.html"
#define HELP_DISPLAY_PATH		 "http://127.0.0.1/source/system/Help/Contextual/Help_Display_Menu.html"
#define HELP_PROBE_PART_PATH	 "http://127.0.0.1/source/system/Help/Contextual/Help_ProbePart_Menu.html"		
#define HELP_FOCAL_LAW_PATH 	 "http://127.0.0.1/source/system/Help/Contextual/Help_Focal_Law_Menu.html"
#define HELP_SCAN_PATH 			 "http://127.0.0.1/source/system/Help/Contextual/Help_Scan_Menu.html"
#define HELP_FILE_PATH 			 "http://127.0.0.1/source/system/Help/Contextual/Help_File_Menu.html"
#define HELP_PREFERENCES_PATH  	 "http://127.0.0.1/source/system/Help/Contextual/Help_Preferences_Menu.html"


/*帮助文档 标签信息*/
#define LABEL_00   "#512875"
#define LABEL_01   "#641413"
#define LABEL_02   "#314552"

#define LABEL_10   "#346475"
#define LABEL_11   "#315062"
#define LABEL_12   "#315092"
#define LABEL_13   "#315156"
#define LABEL_14   "#315170"

#define LABEL_20   "#383759"
#define LABEL_21   "#315385"
#define LABEL_22   "#315446"
#define LABEL_23   "#315504"

#define LABEL_30   "#315694"
#define LABEL_31   "#522686"
#define LABEL_32   "#316136"
#define LABEL_33   "#319518"
#define LABEL_34   "#582690"

#define LABEL_40   "#HelpID_10434"
#define LABEL_41   "#382525"
#define LABEL_42   "#316930"
#define LABEL_43   "#316990"

#define LABEL_50   "#584040"
#define LABEL_51   "#584371"
#define LABEL_52   "#584524"
#define LABEL_53   "#584552"

#define LABEL_60   "#643961"
#define LABEL_61   "#644073"
#define LABEL_62   0
#define LABEL_63   "#644028"
#define LABEL_64   "#644119"

#define LABEL_70   "#584719"
#define LABEL_71   "#584779"
#define LABEL_72   "#584845"
#define LABEL_73   "#584879"
#define LABEL_74   "#584906"

#define LABEL_80   "#151491"
#define LABEL_81   "#151558"
#define LABEL_82   "#306577"
#define LABEL_83   "#151607"
#define LABEL_84   "#151625"

#define LABEL_90   "#151729"
#define LABEL_91   "#321439"
#define LABEL_92   "#525460"
#define LABEL_93   "#586053"
//#define LABEL_94   

#endif
