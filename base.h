
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * base.h
 * 定义各种基本数据结构,包括探头 楔块 工件等
 * 及各种配置文件
 *
 *
 * 谭登华
 * 2010-11-4
 * 
 *
 *
 *
 *
 * 85 X6 + 90 = 600
 * 114 X 6 + 116 = 800
 *
 */

#ifndef __BASE_H_
#define __BASE_H_

#include "base_const.h"
#include <gtk/gtk.h>

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x

#define X86_DEPTH	24
#define ARM_DEPTH	16

#if X86
#define TRUE_DEPTH	X86_DEPTH
#elif ARM
#define TRUE_DEPTH	ARM_DEPTH
#endif

extern GdkColor	color_black;
extern GdkColor	color_black1;
extern GdkColor	color_white;
extern GdkColor	color_yellow;
extern GdkColor	color_text_base;
extern GdkColor	color_button0;
extern GdkColor	color_button1;
extern GdkColor	color_button2;
extern GdkColor color_rule;


/*
 * usDelayTX:Gives the delay from the beginning of the law and the
 * pulse. This value may be from (setup_ELEM_DELAY_MIN to
 * setup_ELEM_DELAY_MAX) expressed in ns (actual steps of 5
 * ns). The value setup_ELEM_DELAY_OFF turns off the element.
 * usDelayTX:聚焦法则和发射的延迟, 以ns为单位 步进是5ns. setup_ELEM_DELAY_OFF 关闭这个阵元 
 * usIndex:Index of the element.
 * usIndex:阵元编号
 */

typedef struct setup_stElemTx {
	gushort    usIndex;
	gushort    usDelayTX;
} Doppler_stElemTx, *Doppler_stElemTx_P;

/*
usIndex:   Index of the element. This value may be from 0 to 
(setup_MAX_ELEM_RX - 1);

usDelayRx: Relative delay to form the focal law in steps of 1 ns
(the effective electronic step in 5 ns). The delay is 
relative to the start of the A-scan. A value of
setup_ELEM_DELAY_OFF indicates the element is inactive,
else this element is active and the signal measured will
contribute to the sum. This value may be from
(setup_ELEM_DELAY_MIN to setup_ELEM_DELAY_MAX) expressed
in ns (actual steps of 5 ns).

usDelayRX:聚焦法则和接收的延迟, 以ns为单位 步进是5ns. setup_ELEM_DELAY_OFF 关闭这个阵元 
sGain: The input gain of the element, it is also the first gain
of the TGC. This value may be from 0 to 80 dB by 0.1 dBstep.
*/

typedef struct setup_stElemRx {
	gushort    usIndex;
	gushort    usDelayRX;
	gshort     sGain;
} Doppler_stElemRx, *Doppler_stElemRx_P;


/*
usCondition:         Indicate how to compute the detection status given by the
                     gates for the alarm activation. Those alarms status
                     generated merge the C-scan data channel. The alarm
                     condition is formed by a mask of bits:

                     usCondition =
                       !(!GATE0 AND !GATE1 AND !GATE2 AND !GATE3 AND !GATE4)
                       | |  |       |  |       |  |       |  |       |  |
                       | |  |       |  |       |  |       |  |       |  setup_ALARM_USE_GATE4
                       | |  |       |  |       |  |       |  |       setup_ALARM_INVERT_GATE4
                       | |  |       |  |       |  |       |  setup_ALARM_USE_GATE3
                       | |  |       |  |       |  |       setup_ALARM_INVERT_GATE3
                       | |  |       |  |       |  setup_ALARM_USE_GATE2
                       | |  |       |  |       setup_ALARM_INVERT_GATE2
                       | |  |       |  +--- setup_ALARM_USE_GATE1
                       | |  |       +------ setup_ALARM_INVERT_GATE1
                       | |  +-------------- setup_ALARM_USE_GATE0
                       | +----------------- setup_ALARM_INVERT_GATE0
                       +------------------- setup_ALARM_INVERT_RESULT

                     setup_ALARM_INVERT_RESULT:
                        Indicate if the reults must be invert.

                     setup_ALARM_INVERT_GATEX:
                        Indicate if the state provide by the gate X must be
                        invert.

                     setup_ALARM_USE_GATEX:
                        Indicate if the state of gate X plays a part in the
                        alarm generation.

cCount:              Indicates the quantity of internal alarm occurrence before
                     an external alarm is generated. The possible range for
                     this value is from 1 to setup_ALARM_MAX_STAT occurences.
*/
typedef struct setup_stAlarm{
   gushort    usCondition;
   gchar      cCount;
} Doppler_stAlarm, *Doppler_Alarm_P;


/*
lPosition:           The position of the gate from a the reference time
                     (beginning of the A-scan). This value may be from 0
                     to the A-scan length (in steps of 10 ns).

lLength:             The length of the gate. This value may be from
                     setup_ASCAN_LEN_MIN to setup_ASCAN_LEN_MAX (the
                     A-scan length; in steps of 10 ns).

                     When drawing a gate, the left most side will be at
                     "lPosition" and the point right under that line is
                     INCLUDED in the gate. The right most side will be
                     at "lPosition+lLength" and the point under that line
                     is EXCLUDED from the gate. The reasoning is as follow
                     points in the hardware all represent 10nS while on
                     display it is rather the distance between the points
                     that represent time. The first point although it seem
                     to be a time 0, also represents 10nS so if we were to
                     include the right most point we would actually get
                     one too much time division (10nS at 100MHz).

sLevel:              The level.

cTrigModeGate:       The mode of detection; this value may be
                     setup_GATEMODE_PEAK or setup_GATEMODE_CROSSING.

cCscanDataEnabled:   Usage of gate. This value may be TRUE (used) or FALSE (not
                     used). When used, those gates will produce data
                     transmitted on socket.

cPolarity:           Polarity of the gate. This value may be
                     setup_POLARITY_POS, setup_POLARITY_NEG or
                     setup_POLARITY_BOTH.
*/
typedef struct setup_stGate {
   gint             lPosition;
   gint             lLength;
   gshort           sLevel;
   gchar            cTrigModeGate;
   gchar            cCscanDataEnabled;
   gchar            cPolarity;
} Doppler_stGate, *Doppler_Gate_P;

/*
The instrument starts the detection (A-scan measurement) when the signal
crosses the detection threshold inside the synchronization gate (when
synchronization mode is 'echo'). In 'pulser' mode, instrument starts
measurement immediately at the gate position (lPosition). The maximal delay
before the firing at the measurement is the sum of the position and the length.
This sum should be shorter than 2.62 ms (setup_SYNC_DELAY_MAX). The
measurement, the A-scan, should be short than setup_ASCAN_LEN_MAX (in sample).

lPosition:           The position (in time, steps of 10 ns) of the gate from
                     the beginning of the law (reference time).

lLength:             The length of the synchronization gate. This value may be
                     ajusted with the position to match the limit of
                     setup_SYNC_DELAY_MAX.

sLevel:              Level in samples. This value may be from 0 to 32767.

cTrigModeSynchro:    The mode of detection. This value may be
                     setup_SYNCMODE_PULSER or setup_SYNCMODE_ECHO.

cPolarity:           Polarity of the synchronization gate. This value may be
                     setup_POLARITY_POS, setup_POLARITY_NEG or
                     setup_POLARITY_BOTH.

lDelaySync:          Negative or positive delay between the detection in the
                     gate and the A-scan reading, in steps of 10 ns.

cCscanDataEnabled:   Usage of gate. This value may be TRUE (used) or FALSE (not
                     used). When used, those gates will produce data
                     transmitted on socket.
*/
typedef struct setup_stSynchro {
   gint              lPosition;
   gint              lLength;
   gshort             sLevel;
   gchar              cTrigModeSynchro;
   gchar              cPolarity;
   gint              lDelaySync;
   gchar              cCscanDataEnabled;
} Doppler_stSynchro, *Doppler_stSynchro_P;

/*
cType:               Type of encoder. There are 4 types: clock direction
                     counter (setup_ENC_CLK_DIR), Phase A-Phase B counter 
                     (setup_ENC_QUADRATURE), Counter UP (setup_ENC_UP) and
                     Counter DOWN (setup_ENC_DOWN).

cPresetSource:       The preset action; determine how the preset will act. 
                     This action may be comamnd from the associated preset line
                     (setup_PRESET_EXTERNAL_LINE), the common external preset
                     line (setup_PRESET_EXT_COMMON_LINE), a CPU action
                     (setup_PRESET_INTERNAL) or not use (setup_PRESET_NOT_USE).

lPreset:             A 32 bits signed value to preset to the encoder.

cEnable:             Encoder usage. This value may be TRUE (used) or FALSE (not
                     used). When used, this encoder will produce data
                     transmitted on socket.
*/
typedef struct setup_stEnc {
   gchar              cType;
   gchar              cPresetSource;
   gint              lPreset;
   gchar              cEnable;
} Doppler_stEnc, *Doppler_stEnc_P;

/*
This structure defines the usage of all encoder (setup_MAX_QTY_ENCODER or
setup_MAX_QTY_ENCODER_DIFF when differential usage is asked).

sAcqDivider:         When the acquisition clock source come from an encoder,
                     this value divides the encoder to produce the sampling
                     clock.

cAcqDividerMode:     The preceding divider can act on a particular direction
                     (ascending, downward or both). Value may be
                     setup_DIVIDER_MODE_UP, setup_DIVIDER_MODE_DOWN or
                     setup_DIVIDER_MODE_BOTH.
astEncoders[]:       Definition for all encoders.
*/
typedef struct setup_stEncAll {
   gshort             sAcqDivider; 
   gchar              cAcqDividerMode; 
   Doppler_stEnc      astEncoder[setup_MAX_QTY_ENCODER];
} Doppler_stEncAll, *Doppler_stEncAll_P;


/*
This structure gives information of a focal law. The field order follow the
natural sequence order (pulser, receiver, numerization, process, etc.).

sLawNumber:          the number of the law, this value may be from 0 to
                     setup_MAX_LAW_QTY.
					 
sLawFiringOrder:     Firing ordering of the law.

cEnable:             TRUE: the law is enable. FALSE, the law is disable.

cPATech:             TRUE: the law is PA technology (use the first HV power
                     supply); FALSE: the law is UT technology (use the second 
                     HV poser supply).

lDuration:           Complete duration of the law. This time (in step of 10 ns)
                     includes the synchronization gate, the delay time, the
                     A-scan digitalization, the measurement time and electronic
                     update time. This value controls the recurrence firing of
                     the law. When averaging is in use, this time is multiply
                     by the average value. The instrument will round and adjust
                     this value. Put an equal or greater value to the acoustic
                     attenuation to void strange echo.

sPulserWidth:        The duration of the US generator pulse width. This value
                     may be from setup_PULSE_WIDTH_MIN to setup_PULSE_WIDTH_MAX
                     (in ns and the precision is of 5 ns).

cReceiverArray:      Defines the kind of pulser. This value may be
                     setup_PULSER_PP or setup_PULSER_PR.

astTransmitter<>:    Structure that gives the details about the usage (usage
                     and phase) of every pulser. For UT technology, this field
                     allows selection of the unique transmitter. The maximum of
                     active element in a law is setup_MAX_ELEM_TX_ACTIVE.

lAcqDelay:           This delay (number of 24 bits in front of the law) holds
                     the initial sequencing before acquisition (law formation).
                     This value is embedded in lDuration and expressed in step
                     of 10 ns.

usMaxDelayRx:        This returned delay (ignored as input) gives the maximum
                     relative delay to form the focal law (based on all active
                     element anabled). This value may be from 0 to
                     setup_ELEM_DELAY_MAX_RX / 10 (in step of 10 ns).

cInputFilter:        Select the input filter (or not) to apply on the receiving
                     signal (RF). The effective input filter is a automatic
                     combination of many kind of filter (analog and digital).
                     See constants setup_IN_FILTER_* for more details.

astReceiver<>:       Structure that gives the details about the usage (usage
                     and phase) of every receiver. For UT technology, this field
                     allows selection of the unique receiver.

cTGCEnabled:         TRUE: the Time Gain Compensation is
                     enabled. The TGC, which is the same for every element,
                     starts after the element specific delay.

stTGC:               Structure that gives the arbitrary Time Gain Compensation
                     (TGC) level for the signal incoming from every active RX
                     elements. This arbitrary level may be from 0 to 800 tenth
                     of DB.	Important: the first point must be 0dB, 0ns (0,0).

stSynchro:           Defines the position and length of the synchronization
                     gate (to take the A-scan).

lAscanDuration:      A-scan mesurement time. This value may be from
                     setup_ASCAN_LEN_MIN to setup_ASCAN_LEN_MAX steps of 10ns.

cSamplingRate:       The sampling rate of the signal. See list of #define
                     setup_SAMP_RATE_*.

cAveragingType:      The kind of averaging. This value may be setup_AVER_NONE
                     or setup_AVER_NORMAL.

sAveraging:          The number on average to compute. This value may from 2,
                     4, 8 or 16.

cInputRectification: The kind of input signal rectification. See constant
                     setup_RECT_* for available kind.

cSmoothingFilter:    Select the low pass filter (or not) to apply on the
                     receiving signal (after rectification) to smooth the
                     signal. See defines setup_FILTER_SM for more details.

cCompressionType:    The compression type to use on the A-scan. Valid values
                     are: setup_COMPRESSION_NONE, setup_COMPRESSION_POS,
                     setup_COMPRESSION_NEG, setup_COMPRESSION_BOTH.

ucCompressionRatio:  The compression ratio to use on the A-scan. This value
                     may be between 0 and 255 (meaning 1 to 256). The ratio
                     this information: for example, with a ratio of 3, we
                     transmit over Ethernet only one sample (the maximum) per
                     3 samples from the A-scan. When the compression is
                     setup_COMPRESSION_BOTH we transmit the positive and
                     negative maximum. For the same example of 3, we transmit
                     two samples (positive and negative max) per 3 samples from
                     A-scan.

cSampleSize:         TRUE: the A-scan in the data have a full scale 16 bits.
                     FALSE: full scale 8 bits.

cAscanDataEnabled:   TRUE: transfer the A-scan to the data queue for
                     transmission over Ethernet.

astGate[]:           Defines the position and length of the
                     setup_MAX_GATE_MEASURE measurement gates.

stAlarm:             Structure that defines the conditional alarm based on
                     C-scan.
*/
typedef struct setup_stLaw {
   gshort             sLawNumber;
   gshort             sLawFiringOrder;
   gchar              cEnable;
   gchar              cPATech;
   gint              lDuration;
   gshort             sPulserWidth;
   gchar              cReceiverArray;
   Doppler_stElemTx    astTransmitter[setup_MAX_ELEM_TX_ACTIVE];
   gint              lAcqDelay;
   gushort    usMaxDelayRx;
   gchar              cInputFilter;
   Doppler_stElemRx    astReceiver[setup_MAX_ELEM_RX_ACTIVE];
   gchar              cTGCEnabled;
   gshort     sTGC;
   Doppler_stSynchro   stSynchro;
   gint              lAscanDuration;
   gchar              cSamplingRate;
   gchar              cAveragingType;
   gshort             sAveraging;
   gchar              cInputRectification;
   gchar              cSmoothingFilter;
   gchar              cCompressionType;
   guchar     ucCompressionRatio;
   gchar              cSampleSize;
   gchar              cAscanDataEnabled;
   Doppler_stGate      astGate[setup_MAX_GATE_MEASURE];
   Doppler_stAlarm     stAlarm;
} Doppler_stLaw, *Doppler_stLaw_P;

/*
sPulseVoltagePA:     Voltage for the instrument. Gives the high voltage for PA
                     technology power supply. This value may be 
                     setup_VOLTAGE_MIN_PA, setup_VOLTAGE_MED_PA or 
                     setup_VOLTAGE_MAX_PA.

sPulseVoltageUT:     Voltage for the instrument. Gives the high voltage for UT
                     technology power supply. This value may be 
                     setup_VOLTAGE_MIN_UT, setup_VOLTAGE_MED_UT or 
                     setup_VOLTAGE_MAX_UT.

cAcqClockSource:     The synchronization (acquisition) source type. Choices
                     are: setup_SYNC_INTERNAL, setup_SYNC_EXTERNAL,
                     setup_SYNC_COUNTER1 and setup_SYNC_COUNTER2,
                     setup_SYNC_APS_TCPIP, setup_SYNC_APS_UDP_LITTLE_ENDIAN
                     and setup_SYNC_APS_UDP_BIG_ENDIAN.                        
                                             
bClockEnabled:       Specifies if the input line ENA is used to control
                     (enable or disable) the acquisition clock. This value may
                     be TRUE (controlled) or FALSE (not controlled).
*/
typedef struct setup_stGeneral {
   gshort             sPulseVoltagePA;
   gshort             sPulseVoltageUT;
   gchar              cAcqClockSource;
   gchar              bClockEnabled;
} Doppler_stGeneral, *Doppler_stGeneral_P;


/*
stGeneral:           The general definition. 

stMiscellaneous:     Miscellanaous definition.

stEnc:               Encoder definition.

astLaw<>:            Dynamic array of laws.
*/
typedef struct setup_stAll {
   Doppler_stGeneral   stGeneral;
//   Doppler_stMisc      stMiscellaneous;
   Doppler_stEncAll    stEnc;
   Doppler_stLaw       astLaw[setup_MAX_LAW_QTY];
} Doppler_stAll, *Doppler_stAll_P;

/* 一个阵元的聚焦信息*/
typedef struct law_elem
{

} LAW_ELEM, *LAW_ELEM_P;

/* 一条beam的聚焦信息 */
typedef struct law_beam
{

} LAW_BEAM, *LAW_BEAM_P;


/*聚焦法则信息(law_info)*/
typedef struct law_info
{
	guchar	Focal_type;			/* 聚焦类型 */
	guchar	ELem_qty;			/* 聚焦阵元数 */
	guchar	Elem_tx_start;		/* 该法则阵元发射开始的编号 */
	guchar	Elem_tx_end;		/* 该法则阵元发射开始的编号 线扫时候需要设置 扇扫直接算好 */
	guchar	Elem_rx_start;		/* 该法则阵元接收开始的编号 */
	guchar	Elem_rx_end;		/* 该法则阵元接收开始的编号 线扫时候需要设置 扇扫直接算好 */
	guchar	ELem_step;			/* 线扫时候可以设置阵元间隔 */
	guchar	Velocity_type;		/* 从波 或者 横波 */
	gshort	Angle_start;
	gshort	Angle_end;			/* 扇扫时候可以设置的角度 */
	guint	focus_depth;		/* 扇扫时候为声程 线扫是深度 */
	gushort	law_index_start;	/* 聚焦法则索引 计算出来的 */
	gushort law_index_end;		/*  */
} LAW_INFO, *LAW_INFO_P;;

typedef struct element_law
{
	guchar	e_number;

} ELEMENT_LAW, *ELEMENT_LAW_P;

/*探头(Probe)*/
typedef struct Probe {
	guint	Elem_qty;		/* 阵元数 */
	guint	Frequency;		/* 频率 0.1Mhz 为单位 */
	guint	Pitch;			/* 阵元中心间距 0.1mm 为单位 */
	gchar	Name[20];		/* 探头名字 */
} PROBE, *PROBE_P;

/*楔块 (Wedge)*/
typedef struct Wedge {
	guint	Angle;			/* 角度 */
	guint	Height;			/* 第一阵元高度 */
	gint	Primary_offset;	/* 前沿 */
	guint	Vel0city;		/* 声速 */
	gchar	Name[20];		/* 楔块名字 */
} WEDGE, *WEDGE_P;

/*材料 (Material)*/
typedef struct Material {
	guint	Velocity_LW;	/* 声速 单位 0.01m/s 纵波 快点 */
	guint	Velocity_SW;	/* 声速 单位 0.01m/s 横波 慢点 */
	guchar	Name[20];		/* 材料名字 */
} MATERIAL, *MATERIAL_P;

/*工件 (Part)*/
typedef struct Part {
	guint	Geometry;		/* 几何形状 FLAT/ID/OD/BALL */
	guint	Thickness;		/* 厚度 */
	guint	Diameter;		/* 直径 */
	guint	Material;		/* 材料 */
} PART, *PART_P;

/* 组信息 */
typedef struct Group {
	guint	wedge_delay;	/* 楔款延时 单位 ns */
	guint	range;			/* 显示范围 单位 ns */
	gint	start;			/* 扫描延时 单位 ns */
	gushort	gain;			/* 实际增益 单位 0.01dB */
	gushort	gainr;			/* 参考增益 单位 0.01dB */
	guint	velocity;		/* 实际声速 单位 0.01m/s  */
	guchar	db_ref;			/* 参考增益开关 0 off 1 on */
	/*发射*/
	guchar	pulser;			/* 1~ 128 - elem_qty(聚焦阵元数最大为32) + 1 
							   指定发射阵元 与机器配置相关我们是128阵元最大,
							   Probe 的Auto Program 选择On 以后不可以调节 */
	LAW_INFO	law_info;	/* 聚焦法则的信息  */
	PROBE	probe;
	WEDGE	wedge;


} GROUP, *GROUP_P;

/*配置信息 (CONFIG)*/
typedef	struct Config {
	guchar	groupId;			/* 当前group */
	guchar	groupQty;			/* 共有几个group  0 1 2 3 4 5 6 7 */
	GROUP	group[4];
	/* 基本参数 */
	PART	part;				/* 被检测工件... */
	
	/* 所有聚焦法则的信息在这里 */
	LAW_BEAM	focal_law_all[setup_MAX_LAW_QTY][setup_MAX_ELEM_RX_ACTIVE];	

	/*发射*/
	guchar	tx_rxmode;		/*  */
	gushort	frequence;		/*  */
	guchar	voltage_cfg;		/*  */
	guint	pulser_width;	        /*  */
	gushort	prf;			/*  */

	/*接收*/
	guchar	receiver;		/*  */
	guchar	filter;			/* 滤波 */
	guchar	rectifier;		/*  */
	guchar	video_filter;	        /*  */
	guchar	averaging;		/*  */
	guchar	reject;		        /*  */

	/*波束 beam*/
	gint	scan_offset;		/**/
	gint	index_offset;		/**/
	gint	angle;				/**/
	gint	skew;				/**/
	guint	beam_delay;			/**/
	gint	gain_offset;

	/*高级*/
	guint	auto_height;	/**/
	gushort	point_qty;		/**/
	gushort	sum_gain;		/**/

	/*闸门报警 */
	gchar   gate;
	gchar   parameter;
	gchar   synchro;
	gchar   measure;
	gchar   rf;
	guint	agate_mode;		
	gint	agate_start;	
	guint	agate_width;	
	guint	agate_height;	

	guint	bgate_mode;	
	gint	bgate_start;	
	guint	bgate_width;	
	guint	bgate_height;	

	guint	cgate_mode;		
	gint	cgate_start;	
	guint	cgate_width;	
	guint	cgate_height;	

	guchar	groupA;
	guchar	conditionA;
	guchar	oprt;
	guchar	groupB;
	guchar	conditionB;

	guint	alarm_sound;

	/*测量*/
	guint	measure1;	
	guint	measure2;	
	guint	measure3;	
	guint	measure4;	

	/*显示*/
	guchar	display;		/*显示模式 A B C A+B A+B+C A+S ...*/
	guint	c_scan;			/*c扫描参考*/

	guchar	ut_unit;		/*检测单位 时间2 声程1  实际深度0 .*/
	guchar	color;			/**/


	/*选项*/
	guchar	unit;			/* 0 mm 1 inch */
	guint	bright;

	guint	date;			/*  */
	guint	time;			/*  */

	guchar	type;                            /*Wizard -> calibration -> type */
	guchar	calibration_mode;                /*Wizard -> calibration -> Mode */

	guchar	alarm;                           /*Gate/Alarm->Alarm->Alarm*/
	guchar	output;                          /*Gate/Alarm->Output->Output*/
	guchar	alarm1;                          /*Gate/Alarm->Output->Alarm #*/
	guint	count;                           /*Gate/Alarm->Output->count*/
	guchar	sound;                           /*Gate/Alarm->Output->sound*/
	guint	active_delay;                    /*Gate/Alarm->Output->delay*/
	guint	holdtime;                        /*Gate/Alarm->Output->holdtime*/
	guchar	mode;                            /*Gate/Alarm->Sizing Curves->Mode*/
	guchar	curve;                           /*Gate/Alarm->Sizing Curves->Curve*/

	guchar	list;                            /*Measurements->Reading->list*/
	guchar	field1;                            /*Measurements->Reading->Field1*/
	guchar	field2;                            /*Measurements->Reading->Field2*/
	guchar	field3;                            /*Measurements->Reading->Field3*/
	guchar	field4;                            /*Measurements->Reading->Field4*/
	guchar	select;                            /*Measurements->Cursors->select*/
	guint	VPA;                            /*Measurements->Cursors->VPA*/
	guint	cursors_scan;                   /*Measurements->Cursors->Scan*/
	guint	cursors_index;                  /*Measurements->Cursors->index*/
	guchar	display_table;                  /*Measurements->Table->display table*/
	guchar	entry_image;                   /*Measurements->Table->entry image*/
	guchar	entry_qty;                     /*Measurements->Table->Select Entry*/

	guchar	source;                         /*Measurements->Thickness->source*/
	guint	min_thickness;                  /*Measurements->Thickness->min*/
	guint	max_thickness;                  /*Measurements->Thickness->max*/
	guint	echo_qty;                       /*Measurements->Thickness->echo_qty*/

	//	guchar  display;                        /* Display -> Selection -> display*/
	guchar  grid;                           /* Display -> Overlay -> grid */
	guchar  zoom_display;                   /* Display -> zoom -> display */
	guchar  color_select;                   /* Display -> Color -> select*/
	guint   color_start;                    /* Display -> Color -> Start*/
	guchar  color_contrast;                 /* Display -> Color -> contrast */
	guint   color_end;                      /* Display -> Color -> end */
	guchar  brightness;                     /* Display -> Color -> brightness */
	gushort  min;                            /* Display -> Color -> min */
	gushort  max;                            /* Display -> Color -> max */
	guchar  color_mode;                     /* Display -> Color -> Mode */
	guint   sizing_curves;                  /* Display -> overlay -> sizing curves*/
	guint   overlay_gate;                   /* Display -> overlay -> gate*/
	guint   overlay_cursor;                 /* Display -> overlay -> cursor*/
	guint   overlay_overlay;                /* Display -> overlay -> overlay*/
	guchar  prop_scan;                      /* Display -> Properties -> Scan */
	guchar  envelope;                       /* Display -> Properties -> envelope */
	guchar  prop_source;                    /* Display -> Properties -> Source */
	guchar  prop_app;                       /* Display -> Properties -> appearance */
	guchar  prop_overlay;                   /* Display -> Properties -> overlay */
	guint   compress;                       /* Display -> Properties -> compress */
	guchar  optimum;                        /* Display -> Properties -> optimum */
	guchar  ratio;                          /* Display -> Properties -> Ratio 1:1 */
	guchar  interpolation;                  /* Display -> Properties -> Interpolation */
	guchar  fft_color;                      /* Display -> Properties -> fft_color */
	guchar  orientation;                      /* Display -> Properties -> orientation*/

	guint   part_thickness;                 /*Probe/Part -> Parts -> thickness*/
	guchar  auto_detect;                    /* 自动检测探头连接状态 开启时候不能调节 收发起止位置 */
	guchar  group_mode;                     /*Probe/Part -> select -> Group Mode*/
	guchar  probe_select;                   /*Probe/Part -> select -> select*/
	guchar  fft;                            /*Probe/Part -> characterize -> FFT*/

	/* 聚焦法则 */
	guchar  law_config;                     /* 聚焦模式 扇扫 线扫etc */
	guchar  element_qty;					/* */
	guchar	connection_P;                   /* 设置收的接口 1-128 */
	guchar  connection_R;                   /* 设置发的接口 1-128 */
	guchar  first_element;                  /* 第一个阵元 */
	guchar  last_element;                   /* 最后一个阵元 (线扫时候可以设置) */
	guchar  element_step;                   /* 阵元间隔 (线扫时候可以设置) */
	guchar  wave_type;                      /* 0纵波 与 1横波 */
	guint   min_angle;                      /*Focal Law -> Beam -> Min_angle*/
	guchar  auto_program;                   /*Focal Law -> Laws -> Auto Program*/
	guint   focus_depth;                    /*Focal Law -> Beam -> focus_depth*/

	guint    scan_speed;                     /* Scan -> Inspection -> Scan speed*/
	guchar   scan_type;                     /* Scan -> Inspection -> Type*/
	guchar   inspec_scan;                   /* Scan -> Inspection -> Scan*/
	guint    scan_start;                    /* Scan -> Area -> Scan start*/
	guint    scan_end;                      /* Scan -> Area -> Scan end*/
	guint    scan_resolution;               /* Scan -> Area -> Scan resolution*/
	guint    index_start;                   /* Scan -> Area -> index_start*/
	guint    index_end;                     /* Scan -> Area -> index_end*/
	guint    index_resolution;              /* Scan -> Area -> index_resolution*/
	guchar   start_mode;                    /* Scan -> Start -> start mode*/
	guchar   pause;                         /* Scan -> Start -> pause*/
	guchar   storage;                       /* Scan -> Data  -> storage*/
	guchar   inspec_data;                   /* Scan -> Data  -> Inspec.data*/

	guchar   file_storage;                  /* File -> File -> Storage*/
	guchar   save_mode;                     /* File -> File -> save mode */
	guchar   paper_size;                    /* File -> Report -> paper size */
	guint    format_userfield;              /* File -> Format -> UserField*/
	guint    format_probe;                  /* File -> Format -> probe*/
	guint    format_setup;                  /* File -> Format -> setup*/
	guint    format_note;                   /* File -> Format -> note*/
	guchar    view;                         /* File -> Format -> view*/
	guchar    file_select;                  /* File -> User Field -> file_select*/
	guchar    enable;                       /* File -> User Field -> enable*/
	guchar    scheme;                       /* Preferences -> Pref. -> Scheme */
	guchar    gate_mode;                    /* Preferences -> Pref. -> Gate Mode */
	guchar    select_key;                    /* Preferences -> Pref. -> Select Key */
	guchar    assign_key;                    /* Preferences -> Pref. -> Assign Key */
	guchar    startup_mode;                    /* Preferences -> Service -> Startup Mode*/
	guchar    mouse;                       /* Preferences -> Options -> mouse*/
	guchar    ezview;                       /* Preferences -> Options -> mouse*/
	guchar    remote_desktop;                       /* Preferences -> Options -> mouse*/






} CONFIG, *CONFIG_P;

typedef struct tmp_config {
	guchar	db_reg;				/* 增益步进 */
	guchar	start_reg;			/* start ? 扫描延时步进 */
	guchar	range_reg;			/* 范围(range) 步进 */
	guchar	wedge_delay_reg;	/* 楔块延时(wedge_delay) 步进 */
	guchar	velocity_reg;		/* 声速(velocity) 步进 */

	guchar	pulser_reg;			/* 脉冲发射pulser  步进 */
	guchar	frequence_reg;		/* 频率 frequence 步进 */
	guchar	pulser_width_reg;       /*  脉宽 PW 调节步进 */
	guchar	prf_reg;                /*  重复频率 PRF 调节步进 */

	guchar	receiver_reg;		/* 接收器receiver 步进 */
	guchar	filter_reg;		/* 滤波 filter 步进 */
	guchar	reject_reg;			/* 抑制reject  步进 */
	guchar	scanoffset_reg;		/* scan offset  步进 */
	guchar	indexoffset_reg;	/* index offset  步进 */
	guchar	gainoffset_reg;		/* gain offset  步进 */

	guchar beam_delay_reg;		/*beam delay 步进*/

	guchar point_qty_reg;           /*Points Qty 步进*/
	guchar	sum_gain_reg;           /*Sum Gain 步进*/
	guchar	skew_reg;

	guchar	agate_start_reg;        /*agate_start步进*/
	guchar	agate_width_reg;        /*agate_width步进*/
	guchar	agate_height_reg;       /*agate_height步进*/
	guchar	count_reg;              /*count步进*/
	guchar	active_delay_reg;       /* active_delay  步进 */
	guchar	holdtime_reg;		/* holdtime  步进 */
	guchar	VPA_reg;		/* VPA  步进 */
	guchar	cursors_scan_reg;		/* cursors_scan  步进 */
	guchar  cursors_index_reg;           /*cursors_index步进*/
	guchar  min_thickness_reg;           /*min_thickness步进*/
	guchar  max_thickness_reg;           /*max_thickness步进*/
	guchar  entry_reg;                   /*entry步进*/
	guchar  echo_qty_reg;                /*echo_qty步进*/

	guchar   color_contrast_reg;                 /* Display -> Color -> contrast  */
	guchar   brightness_reg;                     /* Display -> Color -> brightness*/
	guchar   color_start_reg;                    /* Display -> Color -> Start步进  */
	guchar   color_end_reg;                      /* Display -> Color -> end步进    */
	guchar   min_reg;                            /* Display -> Color -> min       */
	guchar   max_reg;                            /* Display -> Color -> max       */
	guchar   compress_reg;                       /* Display -> Properties -> compress */

	guchar   part_thickness_reg;                 /* Probe/Part -> parts -> thickness步进*/
	guchar   diameter_reg;                       /* Probe/Part -> parts -> diameter步进*/

	guchar   connection_P_reg;                   /* Focal_Law -> configuration -> connection P*/
	guchar   connection_R_reg;                   /*Focal_Law -> configuration -> connection R*/

	guchar   element_qty_reg;                    /* Focal_Law -> aperture -> element qty*/
	guchar   first_element_reg;                  /* Focal_Law -> aperture -> first element*/
	guchar   last_element_reg;                   /* Focal_Law -> aperture -> last_element*/
	guchar   element_step_reg;                   /* Focal_Law -> aperture -> element_step*/


	guchar   min_angle_reg;                      /* Focal Law -> Beam -> Min_angle*/
	guchar   max_angle_reg;                      /* Focal Law -> Beam -> Max_angle*/
	guchar   focus_depth_reg;                    /* Focal Law -> Beam -> focus_depth*/
	guchar   angle_step_reg;                     /* Focal_Law -> Beam -> angle_step*/

	guchar    scan_speed_reg;                      /* Scan -> Inspection -> Scan speed*/

	guchar    scan_start_reg;                      /* Scan -> Area -> Scan start*/
	guchar    scan_end_reg;                        /* Scan -> Area -> Scan end*/
	guchar    scan_resolution_reg;                 /* Scan -> Area -> Scan resolution*/

	guchar    index_start_reg;                      /* Scan -> Area -> index_start*/
	guchar    index_end_reg;                            /* Scan -> Area -> index_end*/
	guchar    index_resolution_reg;                     /* Scan -> Area -> index_resolution*/

	guchar    bright_reg;                          /*preferences -> pref. -> bright*/


	gint	fd_config;

} TMP_CONFIG, *TMP_CONFIG_P;

/*测量数据 (MEASURE_DATA)*/
typedef struct Measure_data {



} MEASURE_DATA, *MEASURE_DATA_P;


/*画图区域*/
typedef struct draw_area {
	GtkWidget *vbox;			/*  */
	GtkWidget *hbox;
	GtkWidget *table;			/* 3个刻度尺 1个画图区 */
	GtkWidget *ebox;			/* 存放标题 */
	GtkWidget *label;			/* 窗口名字 */
	GtkWidget *hruler;
	GtkWidget *vruler1;
	GtkWidget *vruler2;
	GtkWidget *drawing_area;             /*波形显示区*/
	GtkWidget *drawarea_colorbar;        /*颜色条*/
} DRAW_AREA, *DRAW_AREA_P;




/*画界面结构体*/
typedef struct Draw_interface {
	guchar			pos_pos;       /* 指示当前位置信息 0 2级停留 1 2级按下 2 3级停留 3 3级按下 */
	guchar			pos;           /* 0,1,2,...,9  一级菜单位置*/
	guchar			pos1[10];           /* 0，1，2，3，4 二级菜单位置*/
	guchar			pos2[10][5];        /* 0，1，2，3，4，5 三级菜单位置*/
	guchar			pos_last;           /* 一级菜单位置上一位置 */
	guchar			pos_last1;          /* 二级菜单位置上一位置  */
	guchar			pos_last2;          /* 三级菜单位置上一位置  */
	guchar			menu2_qty;          /*  */
	GdkColor		col;
	GtkAdjustment	        *adj;
	GSList			*group;

	GtkWidget		*drawing_area;
	DRAW_AREA		draw_area[16];
	/*boxes*/
	GtkWidget		*vbox;			/* 整个window */
	GtkWidget		*hbox1;			/* 上方数据显示 */
	GtkWidget		*vbox11;
	GtkWidget		*vbox12;
	GtkWidget		*hbox121;
	GtkWidget		*hbox111;
	GtkWidget		*vbox1111[3];
	GtkWidget		*hbox112;


	GtkWidget		*hbox2;                 /* button 和 画图区域     */
	GtkWidget		*vbox21;		/* 一、二级菜单， 及画图区域 */
	GtkWidget		*hbox211;		/* 画图区域 及 vscalebox  */
	GtkWidget               *vboxtable;             /* 各个画图区域            */

	GtkWidget	*vbox_area[4];
	GtkWidget	*hbox_area[4];

	//	GtkWidget		*vbox2111;
	//	GtkWidget		*hbox2112;	

	GtkWidget		*hbox212;		/*一级菜单 二级菜单显示*/

	GtkWidget		*vbox22;		/*三级菜单*/
	GtkWidget		*vbox221[6];


	/*menubar menu menuitem_main 可以不用放在这里*/
	GtkWidget	        *event[20];	       /*测量信息等的显示*/
	GtkWidget	        *label[20];	       /*测量信息等的显示*/

	GtkWidget		*menubar;			/* 一级菜单 */
	GtkWidget		*menu;				/* 一级菜单 */
	GtkWidget		*menuitem_main;		        /* 一级菜单 */

	GtkWidget		*menuitem[10];		        /* 一级菜单选项*/

	GtkWidget		*eventbox2[5];                  /* 二级菜单 一个 eventbox + 一个label */
	GtkWidget		*label2[5];                     /*  */

	GtkWidget		*eventbox30[6];                 /* */
	GtkWidget		*eventbox31[6];     /* 三级菜单 二个 eventbox + 一个lable + 一个data + 一个 spinbutton data和spinbutton同时只能有一个显示*/
	GtkWidget		*label3[6];         /* data 是lable   */
	GtkWidget		*data3[6];			/* */
	GtkWidget		*eventbox32[6];                 /* */
	//	GtkWidget		*entry3[6];
	GtkWidget		*sbutton[6];
	GtkWidget		*button;
	/* waiting for delete*/
	GtkWidget		*menu3;
	GtkWidget		*menu_bar3;
	GtkWidget		*root_menu3;
	GtkWidget		*menu_item3[30];

	gint			xx;
	GtkWidget		*window;
	GtkWidget		*spinbutton;

	GtkWidget		*dialog;
	GtkWidget		*popbox;
	GtkWidget		*popbutton[30];
	/* scale 快速调节数值 */
	GtkWidget		*button_add;
	GtkWidget		*button_sub;
	GtkWidget		*vscalebox;
	GtkWidget		*scale_drawarea;
	GtkWidget		*vscale;


	/* 画图区域 */
	GtkWidget               *table;
	GtkWidget               *vrule1;
	GtkWidget               *vrule2;
	GtkWidget               *hrule;
	GtkWidget               *title;
	GtkWidget               *drawlabel;

	GtkWidget		*window2;
	CONFIG_P		p_config;			/**/
	TMP_CONFIG_P	        p_tmp_config;		/**/
	gulong			signal_id;


	guchar			mark_pop_change;    /**/
	guchar			markreturn;
	guchar			mark3;
} DRAW_UI, *DRAW_UI_P;


/* guchar 0~256   gushort 0~65536  guint 0~2^32 */

/* Wizard 中各Start , Next。。。等的实现 */
/* UT Settings -> Beam 下几个菜单unsensitive */
/* UT Settings -> Advanced -> Set 80% , Set Ref. 的实现 */
/* 201 220 231  所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 212 与 215 的关系  */
/* 310  所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 322 323 控制324 的值加减，  325 340弹出一个窗口 */
/* 400 420 所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 430 440已完成 */
/* 501 502 所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 503 504 弹出一个窗口 */
/* 524 525 可编辑状态  */
/* 612 613 何时为sensitive , 何时为 unsensitive 状态*/
/* 620 621 622 何时为sensitive , 何时为 unsensitive 状态*/
/* 710 所选菜单项不同，后面跟着弹出的菜单便不同 */


#define CUR_POS (pp->pos2[pp->pos][pp->pos1[pp->pos]])      /*0,1,2,3,4,5*/
#define CFG(a)	(pp->p_config->a)
/*#define GROUP_VAL(a)  (pp->p_config->group[pp->p_config->groupId].a)*/	/* 原型 */
#define GROUP_VAL(a)  (CFG(group[CFG(groupId)].a))
#define LAW_VAL(a)  (GROUP_VAL(law_info).a)

#define TMP(a)  (pp->p_tmp_config->a)

#define VERSION "DP1.0.0.0"

#endif
