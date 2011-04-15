
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
#include <webkit/webkit.h>

#define X86_DEPTH	24
#define ARM_DEPTH	16

#if X86
#define TRUE_DEPTH	X86_DEPTH
#elif ARM
#define TRUE_DEPTH	ARM_DEPTH
#endif

#define MAX_GROUP_QTY	8

#define	MAX_DOT_QTY		163840
#define LAW_MAX_QTY		256
#define ELEM_TX_MAX_QTY	32
#define ELEM_RX_MAX_QTY	32
#define GATE_MAX_QTY	3

#define COL_24_TO_16(a) ((((a)>>8)&0xf800)|(((a)>>5)&0x07e0)|(((a)>>3)&0x1f))

/* 回波信号8位还是16位 */
#if 1
typedef	guchar	DOT_TYPE;
#else
typedef	gushort	DOT_TYPE;
#endif

extern GdkColor	color_black;
extern GdkColor	color_black1;
extern GdkColor	color_white;
extern GdkColor	color_yellow;
extern GdkColor	color_green;
extern GdkColor	color_blue;
extern GdkColor	color_red;
extern GdkColor	color_text_base;
extern GdkColor color_rule;


extern GdkColor	all_col[];
extern gushort	all_col_16[];

#define	COLOR_BLACK	4
#define	COLOR_WHITE	5
#define	COLOR_YELLOW	3
#define	COLOR_GREEN		1
#define	COLOR_BLUE		0
#define	COLOR_RED	2	

typedef struct zoom_display_info {
	guchar  zoom_type;                      
	guint   start_usound;			
	guint   end_usound;			
	guint   range_usound;			
	guint   center_usound;			
	guchar   start_amplitude;		
	guchar   end_amplitude;			
	guchar   range_amplitude;		
	guchar   center_amplitude;		

} ZOOM_DISPLAY_INFO, *ZOOM_DISPLAY_INFO_P;

# if 0
/* Measurement->Cursors->Selection信息 */
typedef struct selection_info {
	gushort           per_reference;      /* 参考光标的高度 */
	gushort           per_measure;        /* 测量光标的高度 */
	gushort           u_reference;        /* 参考光标在超声轴的位置 */
	gushort           u_measure;	     /* 测量光标在超声轴的位置 */
	gushort           s_reference;	     /* 参考光标在扫描轴的位置 */	     
	gushort           s_measure;	     /* 测量光标在扫描轴的位置 */
	guint            data_link;

} SELECTION_INFO, *SELECTION_INFO_P;
#endif

/* 闸门信息 */
typedef struct gate_info {
	guchar           parameters;     /* 调节模式 0 Positions 1 Mode */
	guchar           synchro;        /* 同步 */
	guchar           measure;        /*  */
	guchar           rectifier_freq; /* 射频 */
	guchar           height;
	gint             start;
	guint            width;
} GATE_INFO, *GATE_INFO_P;

/* 报警信息 */
typedef struct alarm_info {
	guchar	groupa;   	    /* GROUP A     */
	guchar	conditiona;     /* Condition A */
	guchar	operat;         /* Operator    */
	guchar	groupb;	        /* GROUP B 	   */
	guchar	conditionb;     /* Condition B */
} ALARM_INFO, *ALARM_INFO_P;

/* 输出信息 */
typedef struct output_info {
	guchar	alarm1;   	    /* Alarm #  */
	//	guchar	alarm1_value[16];   /* Alarm #的选项是否选中，0否，1是*/
	guchar	alarm1_qty;		/* 开启alarm的个数*/
	gushort alarm1_status;	/* */
	guchar	count;    	    /* count    */
	guchar	sound;          /* sound    */
	guint	delay;			/* delay 	*/
	guint	holdtime;       /* holdtime */
} OUTPUT_INFO, *OUTPUT_INFO_P;

/* 光标信息 */
typedef struct cursors_info {
	gshort    angle;	     
	gushort    amplitude;
	gushort	   UT;
	gint      scan;
	gint      index;

} CURSORS_INFO, *CURSORS_INFO_P;

/* 输出信息 */
typedef struct analog_info {
	guchar group;			
	guchar data;
} ANALOG_INFO, *ANALOG_INFO_P;

/* 一个阵元的聚焦信息*/
typedef struct law_elem
{
    guchar	E_number;
	guchar	Amplitude;		/* 电压 单位V */
	gushort	FL_gain;		/* Focal Gain 0.1db单位 */
	gushort	R_delay;		/*  */
	gushort	T_delay;		/* 0-25560 单位 ns 65535不活动状态 */
	gushort	P_width;			/* 50-500 单位ns*/
	gushort TT_TT;
}LAW_ELEM, *LAW_ELEM_P;

/* 一条beam的聚焦信息 */
typedef struct law_beam
{
/*    gchar Version[32];	*/
/*    gint  N_laws;			*/
    
    guchar N_ActiveElements;	/* 同时激发的阵元数 */
	guchar cycle;
	guchar sumgain;				/*  */
	guchar mode;				/* 0 T/R 1 Pulse-echo*/
	guchar filter;				/* 0 no filter 1 0.5-5 2 2-10 3 5-15 */
	guchar T_first;
	guchar R_first;
	guchar TTT_TTT;				/* 占位置 */
	gushort frequency;			/* 频率0.001Mhz 为单位 */
	gshort R_angle;
	gushort S_angle;
	gushort	TTT__TTT;			/* 占位置 */
	guint Scan_offset;			/* 单位 0.001 mm*/
	guint Index_offset;			/* 单位 0.001 mm*/
	gint G_delay;				/* 单位ns Wedge Delay + Law Delay */
	gint beam_delay;			/* 单位ns Law Delay */
	gint F_depth;				/* 单位 微粒 */
	guint M_velocity;			/* 单位 m/s */
}LAW_BEAM, *LAW_BEAM_P;

typedef struct law_focal
{
	gchar	version[16];
	gshort	beam_qty;		/* 这个focal有几个beam */
	LAW_BEAM_P	*law_beam_p;
	LAW_ELEM_P	*law_elem_p;
} LAW_FOCAL, *LAW_FOCAL_P;


/*聚焦法则信息(law_info) 28byte*/
typedef struct law_info
{
	guchar	Focal_type;		/* 聚焦类型 */
	guchar	Tx_connect;		/* pulser该法则的探头发射第一个阵元其连接口的编号 */
	guchar	Rx_connect;		/* receiver */
	guchar	Elem_qty;		/* 聚焦阵元数 */
	guchar	First_tx_elem;		/* 法则使用的第一个发射阵元 收发分离时候 tx rx 不一样 */
	guchar	First_rx_elem;		/* 法则使用的第一个接收阵元 */
	guchar	Last_tx_elem;		/* 法则使用的最后一个发射阵元 */
	guchar	Last_rx_elem;		/* 法则使用的最后一个接收阵元 */
	guchar	Elem_step;		/* 线扫时候可以设置阵元间隔 */
	guchar	Wave_type;		/* 纵波 或者 横波 */
	gshort	Angle_start;	/* PA 时候是开始角度 UT时候记录探头角度 */
	gshort	Angle_end;		/* 扇扫时候可以设置的角度 0.01度为单位 */
	gushort	Angle_step;		/* 扇扫时候可以设置的角度步进 */
	guint	Focus_depth;		/* 扇扫时候为声程 线扫是深度 0.001mm为单位 */
	gushort	law_index_start;	/* 聚焦法则索引 计算出来的 */
	gushort law_index_end;		/*  */
} LAW_INFO, *LAW_INFO_P;

/*  */
typedef struct element_law
{
	guchar	e_number;
} ELEMENT_LAW, *ELEMENT_LAW_P;

/* 探头(Probe)  516 byte 如果探头是unknown 必须给一个参数 */
typedef struct Probe {
	/*PA 时候先读走4个字节*/
	gchar	A1[2];
	guchar	PA_probe_type;	/* 探头类型 1 是Custom 3 是angle beam 5 是 Contact 6是Immersion */
	gchar	A10;
	guchar	UT_probe_type;	/* 探头类型 1 n/a 0 converntional */
	gchar	A11;
	gchar	Model[20];		/* 探头名字 */
	gchar	Serial[20];		/* 探头名字 */
	guchar	Elem_qty;		/* 阵元数 */
	guchar	Freq2;			/* UT 时候 频率是 freq2 << 8 + elem_qty */
	guint	Pitch;			/* 阵元中心间距 0.001mm 为单位 范围是0.01~65.00mm UT 是Elemet_size */
	guint	A3;
	gushort A4;
	gushort	Frequency;				/* 频率 */
	guint	A5[75];
	gushort A6;
	gushort A7;
	gushort A8;
	gushort	Reference_Point; /*  */
	guint	A9[36];
} PROBE, *PROBE_P;

/*楔块 (Wedge)*/
typedef struct Wedge {
	guchar A1[2]; /* 0x03000300 PA 0x01000100 UT*/
	gchar	Wave_type;		/* 1 是L 2 是 S */
	gchar	A11;
	gchar Model[20]; /* 共用 楔块名字 */
	gchar Serial[20]; /* 共用 楔块名字 */
	gushort Angle; /* 共用 角度单位0.1度 */
	gushort A7;
	gushort Probe_delay; /* UT ns为单位 */
	gchar A2;
	gchar A10; /* UT 1 SW 0 LW*/
	gint Ref_point; /* UT 使用 */
	/*这个地方 得 多读一个字节 */
	guint Height; /* 单位微米 */
	guint Velocity_UT;
	guint A8;
	guint Velocity_PA; /* 速度 mm/s */
	gchar Orientation; /* 1 Normal 0 reversal*/
	gchar A4[3];
	gint Primary_offset; /* 微米 */
	guint Secondary_offset; /* 微米 */
	gint A6[107];
} WEDGE, *WEDGE_P;

/*材料 (Material) 28byte */
typedef struct Material {
	guint	Velocity_LW;		/* 声速 单位 0.01m/s 纵波 快点 */
	guint	Velocity_SW;		/* 声速 单位 0.01m/s 横波 慢点 */
	guchar	Name[20];		/* 材料名字 */
} MATERIAL, *MATERIAL_P;

/*工件 (Part)*/
typedef struct Part {
	guchar	Geometry_pos;		/* 几何形状 FLAT/ID/OD/BALL */
	guint	Thickness;		/* 厚度 */
	guint	Diameter;		/* 直径 */
	guint	Material;		/* 材料 */
} PART, *PART_P;

/* 组信息 */
typedef struct Group {
	/* 基本设置 */
	guint	wedge_delay;	/* 楔款延时 单位 ns */
	guint	range;			/* 显示范围 单位 ns */
	gint	start;			/* 扫描延时 单位 ns */
	gushort	gain;			/* 实际增益 单位 0.01dB */
	gushort	gainr;			/* 参考增益 单位 0.01dB */
	guint	velocity;		/* 实际声速 单位 0.01m/s  */
	guchar	db_ref;			/* 参考增益开关 0 off 1 on */
	/* 发射接收 */
	guchar	pulser;			/* 1~ 128 - elem_qty(聚焦阵元数最大为32) + 1 
							   指定发射阵元 与机器配置相关我们是128阵元最大,
							   Probe 的Auto Program 选择On 以后不可以调节 值与connect P 一样 */
	guchar  receiver;		/* 接收阵元 必须是 PR 模式才能调节 */
	guchar	filter;			/* 滤波 */
	guchar	rectifier;		/* 检波  */
	guchar	averaging;		/* 平均 */
	guchar	video_filter;	/* 视频滤波*/

	guchar  tx_rxmode;		/* 收发模式 */
	guchar	freq_pos;		/* 频率选择模式是指定还是自己输入 */
	guchar	pw_pos;			/* 脉冲宽度选择模式 */
	guchar	prf_pos;		/* 脉冲宽度选择模式 */
	gushort	frequency;		/* 以0.001Mhz 也就是Khz 1MHz~20MHz 为单位 当探头学选unknown 时候才可以调节 */
	gushort	pulser_width;	/* 30~500ns 2.5ns为步进*/

	guint	prf;			/* 重复频率 1-20000Hz 取值为10~200000 */

	guchar	point_qty_pos;	/* 点个数 位置 */
	guchar	sum_gain_pos;	/**/
	guchar  gate_pos;		/* A , B , I 当前修改的是哪个闸门 */
	gushort	point_qty;		/* 点个数 */
	gushort	sum_gain;
	gushort	gain_offset;

	GATE_INFO    gate[GATE_MAX_QTY];	/* gate[0] A , gate[1] B , gate[2] I */

	guchar	mode_pos;					/*Gate/Alarm->Sizing Curves->Mode*/
	guchar	curve_pos;					/*Gate/Alarm->Sizing Curves->Curve*/
	gushort ref_ampl;
	gushort ref_ampl_offset;
	gushort curve_step;
	gushort ref_gain;
	guint   position;
	guchar  point_pos;
	guint   amplitude;
	gushort mat_atten;
	guint   delay;
	gushort tcg_gain;

	guchar	ut_unit;		/*检测单位 时间2 声程1  实际深度0 .*/

	guchar		group_mode;     /* 组工作模式  0 UT or 1 PA*/
	LAW_INFO	law_info;	/* 聚焦法则的信息  */
	PROBE	probe;
	WEDGE	wedge;

	guchar	selection;
	gushort	per_reference;       /* 参考光标的高度 */
	gushort	per_measure;         /* 测量光标的高度 */
	guint	u_reference;         /* 参考光标在超声轴的位置 */
	guint	u_measure;	     /* 测量光标在超声轴的位置 */
	gint	s_reference;	     /* 参考光标在扫描轴的位置 */	     
	gint	s_measure;	     /* 测量光标在扫描轴的位置 */
	guchar	data_link;
	gint	i_reference;         /* 参考光标在指数轴的位置 */
	gint	i_measure;	     /* 测量光标在指数轴的位置 */
	gushort	s_refmeas;
	gushort	cursors_angle;
	CURSORS_INFO	cursors_info[4];
	guchar	source;

	guchar	 col_select_pos;        /* 0 Amplitude  1 TOFD  2 Depth 共3个*/
	guchar   col_start;
	guchar   col_end;                    
	guchar   col_contrast;                                     
	guchar   col_brightness;                     
	guint    col_min;                          
	guint    col_max;                           
	guchar   col_mode; 

	/* Display Ascan的设置 */
	guchar	ascan_color;		/**/
	guchar	ascan_envelope;		/* 0 None 1 Infinite */
	guchar	ascan_source;		/**/
	guchar	ascan_appearance;	/**/
	guchar	ascan_overlay;		/**/

//	COL_SELECT_INFO	col_select[3]; /* Amplitude TOFD Depth 3个*/

	/*波束 beam*/
	gint	scan_offset;		/**/
	gint	index_offset;		/**/
	gint	angle;				/**/
	gushort	skew;				/**/
	guchar  skew_pos;

	gint	agate_start;
	guint	agate_width;

	guchar	last_element;                   /* 最后一个阵元 (线扫时候可以设置) */
	guchar	element_step;                   /* 阵元间隔 (线扫时候可以设置) */
} GROUP, *GROUP_P;

/*配置信息 (CONFIG)*/
typedef	struct Config {
	guchar	groupId;			/* 当前group */
	guchar	groupQty;			/* 共有几个group  0 1 2 3 4 5 6 7 */
	guchar  group_pos;
	guchar	voltage_pa;			/*  */
	guchar	voltage_ut;	
	guchar	language;			/* 语言 */
	GROUP	group[MAX_GROUP_QTY];			/* 前3个都接前面的128的接口 */

	PART	part;				/* 被检测工件... */
	/* 所有聚焦法则的信息在这里 */
	LAW_FOCAL	focal_law_all_info[MAX_GROUP_QTY];
	LAW_BEAM	focal_law_all_beam[setup_MAX_LAW_QTY];
	LAW_ELEM	focal_law_all_elem[setup_MAX_LAW_QTY][setup_MAX_ELEM_RX_ACTIVE];	
	/*  */
	guchar	reject;		        /* 抑制 */
	guchar	auto_height;		/* 自动增益高度*/

	guchar	alarm_pos;          /* 报警信息 0~15 */
	ALARM_INFO	alarm[16];		/* alarm[0], alarm[1], ..., alarm[15] */

	guchar	output_pos;		/* 0~5 */
	OUTPUT_INFO	output[3];	/* 输出信息 output[0],output[1],output[2] */
	ANALOG_INFO	analog[2];	/* Analog 2 个 */
	//guchar alarm_on[16];  		  /*alarm#中的选项后面是否加[On] 0否 1是*/

	/*显示*/
	guchar	display;		/*显示模式 A B C A+B A+B+C A+S ...*/
	guchar	c_scan1;			/*c扫描参考*/
	guchar	c_scan2;
	guchar	c_scan11;
	guchar	data1;
	guchar	data2;
	guchar	dis_mode;
	guint	dis_range;
	guint	avg_scan_speed;
	/*选项*/
	guchar	unit;			/* 0 mm 1 inch */
	guint	bright;

	guint	date;			/*  */
	guint	time;			/*  */

	guchar	type;                            /*Wizard -> calibration -> type */
	guchar	calibration_mode;                /*Wizard -> calibration -> Mode */

	//guchar	alarm;                           /*Gate/Alarm->Alarm->Alarm*/
	guchar	list;                            /*Measurements->Reading->list*/
	guchar	field1;                            /*Measurements->Reading->Field1*/
	guchar	field2;                            /*Measurements->Reading->Field2*/
	guchar	field3;                            /*Measurements->Reading->Field3*/
	guchar	field4;                            /*Measurements->Reading->Field4*/

	//	guchar	selection;                            /*Measurements->Cursors->selection*/
	//	SELECTION_INFO   select;			
	guint	VPA;                            /*Measurements->Cursors->VPA*/
	guint	cursors_scan;                   /*Measurements->Cursors->Scan*/
	guint	cursors_index;                  /*Measurements->Cursors->index*/
	guchar	display_table;                  /*Measurements->Table->display table*/
	guchar	entry_image;                   /*Measurements->Table->entry image*/
	guchar	entry_qty;                     /*Measurements->Table->Select Entry*/

	//	guchar	source;                         /*Measurements->Thickness->source*/
	guint	min_thickness;                  /*Measurements->Thickness->min*/
	guint	max_thickness;                  /*Measurements->Thickness->max*/
	guchar	echo_qty;                       /*Measurements->Thickness->echo_qty*/

	guchar	display_group;

	guchar	grid;                           /* Display -> Overlay -> grid */
	guchar	sizing_curves;                  /* Display -> overlay -> sizing curves*/
	guchar	overlay_gate;                   /* Display -> overlay -> gate*/
	guchar	overlay_cursor;                 /* Display -> overlay -> cursor*/
	guchar	overlay_overlay;                /* Display -> overlay -> overlay*/

	ZOOM_DISPLAY_INFO	zoom_display[6];     /* Display -> zoom -> display   A、B、S..Scan*/
	guchar	zoom_display_pos;               /* A-Scan  B-Scan  S-Scan  ... Off 共6个*/

	guchar	prop_scan;                      /* Display -> Properties -> Scan  6个 */

	guint	compress;                       /* Display -> Properties -> compress */
	guchar	optimum;                        /* Display -> Properties -> optimum */
	guchar	ratio;                          /* Display -> Properties -> Ratio 1:1 */
	guchar	interpolation;                  /* Display -> Properties -> Interpolation */
	guchar	fft_color;                      /* Display -> Properties -> fft_color */
	guchar	orientation;                      /* Display -> Properties -> orientation*/

	//guint   part_thickness;                 /*Probe/Part -> Parts -> thickness*/
	guchar	auto_detect;                    /* 自动检测探头连接状态 开启时候不能调节 收发起止位置 */

	guchar	probe_select;                   /*Probe/Part -> select -> select*/
	guchar	fft;                            /*Probe/Part -> characterize -> FFT*/

	guchar	auto_program;                   /* Off   On*/

	guchar	encoder;
	guchar	polarity;
	guchar	e_type;
	guint	encoder_resolution;
	gint	origin;
	guchar	i_type;
	guchar	i_scan;
	guchar	i_index;
	guint	scanspeed;
	guint	scanspeed_rpm;
	guint	indexspeed;

	gint	scan_start;                    /* Scan -> Area -> Scan start*/
	gint	scan_end;                      /* Scan -> Area -> Scan end*/
	guint	scan_resolution;               /* Scan -> Area -> Scan resolution*/
	gint	index_start;                   /* Scan -> Area -> index_start*/
	gint	index_end;                     /* Scan -> Area -> index_end*/
	guint	index_resolution;              /* Scan -> Area -> index_resolution*/
	guchar	start_mode;                    /* Scan -> Start -> start mode*/
	guchar	pause;                         /* Scan -> Start -> pause*/
	guchar	storage;                       /* Scan -> Data  -> storage*/
	guchar	inspec_data;                   /* Scan -> Data  -> Inspec.data*/

	guchar	file_storage;                  /* File -> File -> Storage*/
	guchar	save_mode;                     /* File -> File -> save mode */
	guchar	templa;
	guchar	paper_size;                    /* File -> Report -> paper size */
	guchar	format_userfield;              /* File -> Format -> UserField*/
	guchar	format_probe;                  /* File -> Format -> probe*/
	guchar	format_setup;                  /* File -> Format -> setup*/
	guchar	format_note;                   /* File -> Format -> note*/
	guchar	view;                         /* File -> Format -> view*/
	guchar	file_select;                  /* File -> User Field -> file_select*/
	guchar	enable;                       /* File -> User Field -> enable*/

	guchar	scheme;                       /* Preferences -> Pref. -> Scheme */
	guchar	gate_mode;                    /* Preferences -> Pref. -> Gate Mode */
	guchar	select_key;                    /* Preferences -> Pref. -> Select Key */
	guchar	assign_key;                    /* Preferences -> Pref. -> Assign Key */
	guchar	assign_key_p;
	guchar	mouse;                       /* Preferences -> Options -> mouse*/
	//guchar    ezview;                       /* Preferences -> Options -> mouse*/
	guchar	remote_desktop;                       /* Preferences -> Options -> mouse*/

	guchar	remark_info[256];
} CONFIG, *CONFIG_P;

typedef struct tmp_config {
	guchar	db_reg;				/* 增益 gain 步进		*/
	guchar	start_reg;			/* 显示延时 start步进	*/
	guchar	range_reg;			/* 范围(range) 步进		*/
	guchar	wedge_delay_reg;	/* 楔块延时(wedge_delay) 步进 */
	guchar	velocity_reg;		/* 声速(velocity) 步进	*/

	guchar	pulser_reg;			/* 脉冲发射pulser  步进	*/
	guchar	frequency_reg;		/* 频率 frequency 步进	*/
	guchar	pulser_width_reg;	/* 脉宽 PW 步进			*/
	guchar	prf_reg;			/* 重复频率 PRF 步进	*/

	guchar	receiver_reg;		/* 接收器receiver 步进	*/
	guchar	filter_reg;			/* 滤波 filter 步进		*/
	guchar	reject_reg;			/* 抑制reject  步进		*/
	guchar	scanoffset_reg;		/* scan offset  步进	*/
	guchar	indexoffset_reg;	/* index offset  步进	*/
	guchar	gain_offset_reg;	/* gain offset  步进	*/

	guchar  beam_delay_reg;		/*beam delay 步进*/

	guchar  point_qty_reg;		/*Points Qty 步进*/
	guchar	sum_gain_reg;		/*Sum Gain 步进*/
	guchar	angle_reg;
	guchar	skew_reg;

	guchar	gate_width_reg;		/* */

	guchar	agate_start_reg;    /*agate_start步进*/
	guchar	agate_width_reg;    /*agate_width步进*/
	guchar	agate_height_reg;	/*agate_height步进*/
	guchar	count_reg;			/*count步进*/
	guchar	active_delay_reg;	/* active_delay  步进 */
	guchar	holdtime_reg;		/* holdtime  步进 */
	guchar	VPA_reg;			/* VPA  步进 */
	guchar  cursors_amplitude_reg;
	guchar  cursors_ut_reg;
	guchar	cursors_scan_reg;	/* cursors_scan  步进 */
	guchar  cursors_index_reg;	/*cursors_index步进*/
	guchar  min_thickness_reg;	/*min_thickness步进*/
	guchar  max_thickness_reg;	/*max_thickness步进*/
	guchar  entry_reg;			/*entry步进*/
	guchar  echo_qty_reg;		/*echo_qty步进*/

	guchar  ref_ampl_reg;		/*232 Gate/Alarm->Sizing Curves */
	guchar  ref_ampl_offset_reg;
	guchar  curve_step_reg;
	guchar  ref_gain_reg;
	guchar  position_reg;
	guchar  amplitude_reg;
	guchar  mat_atten_reg;
	guchar  delay_reg;
	guchar  tcg_gain_reg;

	guchar  per_reference_reg;	 /* 311 Measurements-> cursors -> %(r) */
	guchar  per_measure_reg;
	guchar  u_reference_reg;
	guchar  u_measure_reg;
	guchar  s_reference_reg;
	guchar  s_measure_reg;
	guchar  i_reference_reg;
	guchar  i_measure_reg;
	guchar  s_refmeas_reg;
	guchar  cursors_angle_reg;

	guchar   dis_range_reg;
	guchar   avg_scan_speed_reg;
	guchar   start_usound_reg;	/* Display -> zoom -> Start USound */
	guchar   end_usound_reg;	/* Display -> zoom -> End USound */
	guchar   range_usound_reg;	/* Display -> zoom -> Range USound */
	guchar   center_usound_reg;
	guchar   start_amplitude_reg;	/* Display -> zoom -> Start Amplitude */
	guchar   end_amplitude_reg;	/* Display -> zoom -> End Amplitude */
	guchar   range_amplitude_reg;	/* Display -> zoom -> Range Amplitude */
	guchar   center_amplitude_reg;

	guchar   color_contrast_reg;	/* Display -> Color -> contrast  */
	guchar   brightness_reg;	/* Display -> Color -> brightness*/
	guchar   color_start_reg;	/* Display -> Color -> Start步进  */
	guchar   color_end_reg;		/* Display -> Color -> end步进    */
	guchar   min_reg;			/* Display -> Color -> min       */
	guchar   max_reg;			/* Display -> Color -> max       */
	guchar   compress_reg;		/* Display -> Properties -> compress */

	guchar   part_thickness_reg;	/* Probe/Part -> parts -> thickness步进*/
	guchar   diameter_reg;		/* Probe/Part -> parts -> diameter步进*/

	guchar   connection_P_reg;	/* Focal_Law -> configuration -> connection P*/
	guchar   connection_R_reg;	/*Focal_Law -> configuration -> connection R*/

	guchar   element_qty_reg;	/* Focal_Law -> aperture -> element qty*/
	guchar   first_element_reg;	/* Focal_Law -> aperture -> first element*/
	guchar   last_element_reg;	/* Focal_Law -> aperture -> last_element*/
	guchar   element_step_reg;	/* Focal_Law -> aperture -> element_step*/


	guchar   min_angle_reg;		/* Focal Law -> Beam -> Min_angle*/
	guchar   max_angle_reg;		/* Focal Law -> Beam -> Max_angle*/
	guchar   focus_depth_reg;	/* Focal Law -> Beam -> focus_depth*/
	guchar   angle_step_reg;	/* Focal_Law -> Beam -> angle_step*/


	guchar	  encoder_resolution_reg;	/* Scan -> Encoder -> resolution  */
	guchar	  origin_reg;
	//	guchar    scan_speed_reg;                      /* Scan -> Inspection -> Scan speed*/
	guchar   scanspeed_reg;
	guchar   scanspeed_rpm_reg;
	guchar   indexspeed_reg;

	guchar    scan_start_reg;	/* Scan -> Area -> Scan start*/
	guchar    scan_end_reg;		/* Scan -> Area -> Scan end*/
	guchar    scan_resolution_reg;	/* Scan -> Area -> Scan resolution*/

	guchar    index_start_reg;	/* Scan -> Area -> index_start*/
	guchar    index_end_reg;	/* Scan -> Area -> index_end*/
	guchar    index_resolution_reg;	/* Scan -> Area -> index_resolution*/

	guchar	bright_reg;		/*preferences -> pref. -> bright*/

	DOT_TYPE	envelope_max[MAX_GROUP_QTY][640];	/**/
	DOT_TYPE	envelope_min[MAX_GROUP_QTY][640];	/**/
	DOT_TYPE	a_scan_data[MAX_GROUP_QTY][640];	/**/
	DOT_TYPE	s_scan_data[640*400];		/**/

	DOT_TYPE	scan_data[MAX_GROUP_QTY][640*400];

	gushort	special_col_amp[3];	/* 特殊颜色 */
	gushort	color_amp[256];		/* 调色板信息 */

	gushort	special_col_tofd[3];	/* 特殊颜色 */
	gushort	color_tofd[256];		/* 调色板信息 */

	gushort	special_col_depth[3];	/* 特殊颜色 */
	gushort	color_depth[256];		/* 调色板信息 */

	guint	t_special_col[3];/* 临时特殊颜色 */
	guint	t_color[256];		/* 临时调色板信息 */

	guint	max_prf;
	gushort	*fb1_addr;
	gushort	*virtual_add;

	guint	beam_num[MAX_GROUP_QTY];		/* Group显示的beam编号 */
	guint	beam_qty[MAX_GROUP_QTY];		/* Group beam总数量	*/

	/* 各种扫描窗口的大小, 位置 */
	guint	a_scan_dot_qty;		/*  */
	guint	a_scan_width;
	guint	a_scan_height;
	guint	b_scan_width;
	guint	b_scan_height;
	guint	s_scan_width;
	guint	s_scan_height;
	guint	c_scan_width;
	guint	c_scan_height;

	guchar	scan_type[16];	/* 16个窗口显示的类型 */
	guchar	scan_group[16];	/* 16个窗口对应的group */
	guint	scan_xpos[16];
	guint	scan_ypos[16];

	gchar	*pDraw;			/*  */
	guchar	*pAngleZoom;	/*  */
	guchar	*pDrawRate;		/*  */
	gint	*pDataNo;		/*  */

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
	guchar	scan_type;
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

	gchar			*file_path;	/* 帮助文档的路径 */
	WebKitWebView*		web_view;
	WebKitWebView*		web_view_tmp;
	GtkAdjustment	*adj;
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
	GtkWidget		*vboxtable;             /* 各个画图区域            */

	GtkWidget		*vbox_area[4];
	GtkWidget		*hbox_area[4];

	GtkWidget		*hbox212;		/*一级菜单 二级菜单显示*/

	GtkWidget		*vbox22;		/*三级菜单*/
	GtkWidget		*vbox221[6];

	/*menubar menu menuitem_main 可以不用放在这里*/
	GtkWidget		*event[20];	       /*测量信息等的显示*/
	GtkWidget		*label[20];	       /*测量信息等的显示*/

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
	GtkWidget		*sbutton[6];
	/* waiting for delete*/
	GtkWidget		*menu3;
	GtkWidget		*menu33[6];
	GtkWidget		*menu_bar3;
	GtkWidget		*root_menu3;
	GtkWidget		*menu_item3[30];

	GtkWidget		*window;

	GtkWidget		*dialog;
	/* scale 快速调节数值 */
	GtkWidget		*button_add;
	GtkWidget		*button_sub;
	GtkWidget		*vscalebox;
	GtkWidget		*scale_drawarea;
	GtkWidget		*vscale;

	/* 画图区域 */
//	GtkWidget	*table;
//	GtkWidget	*vrule1;
//	GtkWidget	*vrule2;
//	GtkWidget	*hrule;
//	GtkWidget	*title;
//	GtkWidget	*drawlabel;

	CONFIG_P		p_config;			/**/
	TMP_CONFIG_P	p_tmp_config;		/**/
	guint		p_beam_data;
	gpointer	p_a_scan_data[4];
	gpointer	p_b_scan_data[4];
	gpointer	p_c_scan_data[4];
	gpointer	p_s_scan_data[4];
	guint		scan_count;

	gulong		signal_id;

	GtkWidget	*label_probe;	/* dialog 最下面一排信息   无 probe wedge之分 */
	GtkTreeSelection *selection; 
	GtkTreeSelection *selection1; 
	gchar	p_type[12];
	guchar	tag;		/* 大类选择状态 */

	guchar			main_menu_pop_status; /* 0 down 1 up */
	guchar			mark_pop_change;    /*  */
	guchar			markreturn;
	guchar			mark3;

	guchar	bscan_mark;
	guchar	sscan_mark;
	guchar	refresh_mark;


/*	guint	scan_dispaly_qty;*/	/* 显示几个窗口 */

	gint x_pos;
	gint y_pos;  /* 弹出窗口的xy坐标 */

	guchar  help_yn; /*是否弹出help窗口 0 否，1 是*/	

	guchar  menu3_qty;	/* 三级菜单弹出选项的个数 */
	guchar  menu3_poppos;	/* 三级菜单弹出选项选中的是第几个 */
	guchar  menu3_pos;	/* 当前按下的是第几个三级菜单 0 1 ... 5 */

	guchar  menu3_amount;	/* 三级菜单的个数 */
	guchar  menu3_s;	/* 正数第一个可以选的三级菜单选项的位置*/
	guchar  menu3_e;	/* 倒数第一个可以选的三级菜单选项的位置*/

	gpointer probe_dialog;

	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *sw1;		/* 第二个scroll 探头 楔块 聚焦法则 setup 等需要2个sw */

	guchar  iter_qty;       /* treeview 中 iter 的个数 */
	guchar  iter_pos;	/* treeview 中 iter 的位置 */

} DRAW_UI, *DRAW_UI_P;

typedef struct _MY_SIGNAL_TMP {
	GtkTreeModel *source_model;
	GtkTreeSelection *source_selection;
	GtkWidget *source_list;

	GtkTreeModel *target_model;
	GtkTreeSelection *target_selection;
	GtkWidget *target_list;
} MY_SIGNAL,*MY_SIGNAL_P;


/* 315 Data Link 止 20110212*/
/* 522\523 与 A闸门时的 202\203 相同，未实现 */
/* 524 525 可编辑状态 未实现 */
/* 805 */

/* Wizard 中各Start , Next。。。等的实现 */
/* 201 220 231  所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 212 与 215 的关系  */
/* 310  所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 322 323 控制324 的值加减，  325 340弹出一个窗口 */
/* 400 420 所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 430 440 已完成 */
/* 501 502 所选菜单项不同，后面跟着弹出的菜单便不同 */
/* 503 504 弹出一个窗口 */
/* 524 525 可编辑状态  */
/* 612 613 何时为sensitive , 何时为 unsensitive 状态*/
/* 620 621 622 何时为sensitive , 何时为 unsensitive 状态*/
/* 710 所选菜单项不同，后面跟着弹出的菜单便不同 */


/* probe  wedge  弹出窗口的按键处理 */
///* 72*  mm   in    */
/* 31***   313 mm   in */
/* 三级菜单弹出选项哪些哪时为黑 */



#define CUR_POS (pp->pos2[pp->pos][pp->pos1[pp->pos]])      /* 0,1,2,3,4,5 */
#define CFG(a)	(pp->p_config->a)
/*#define GROUP_VAL(a)  (pp->p_config->group[pp->p_config->groupId].a)*/	/* 原型 */
#define GROUP_VAL_POS(a, b)	(CFG(group[a].b))				/* a表示哪个group b是返回哪个值 */
#define GROUP_VAL(a)  (CFG(group[CFG(groupId)].a))
#define LAW_VAL_POS(a, b)  (GROUP_VAL_POS(a,law_info).b)
#define LAW_VAL(a)  (GROUP_VAL(law_info).a)
#define BEAM_INFO(a,b)  (CFG(focal_law_all_beam[a].b))         /* a表示第几个波束， b表示当前波束下的某一个特性的值 */

#define GROUP_GATE_POS(a)   GROUP_VAL(gate[GROUP_VAL(gate_pos)].a)
#define CFG_ALARM_POS(a)  CFG(alarm[CFG(alarm_pos)].a)
#define CFG_OUTPUT_POS(a) CFG(output[CFG(output_pos)].a)
#define CFG_ANALOG_POS(a) CFG(analog[CFG(output_pos - 3)].a)
#define GROUP_CURSORS_POS(a) GROUP_VAL(cursors_info[GROUP_VAL(selection)-5].a)

#define CFG_ZOOM_POS(a)  CFG(zoom_display[CFG(zoom_display_pos)].a)
//#define GROUP_COL_SELECT(a)  GROUP_VAL(col_select[GROUP_VAL(col_select_pos)].a)

#define TMP(a)  (pp->p_tmp_config->a)
#define	MENU_STATUS	(pp->pos_pos) 

#define VERSION "DP1.0.0.0"

#define ENGLISH_	0
#define CHINESE_	1

#endif
