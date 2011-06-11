
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * 定义各种基本数据结构
 * 有探头 楔块 工件 闸门 GROUP 配置 聚焦法则的阵元和波束 
 * 
 * 
 * 2011-04-24
 * 谭登华
 */

#ifndef __BASE_CONFIG_H_
#define __BASE_CONFIG_H_

#include "base_const.h"
#include <gtk/gtk.h>

typedef struct _zoom_display_info
{
	guchar	zoom_type;
	gchar	tt[3];
	guint	start_usound;
	guint	end_usound;
	guint	range_usound;
	guint	center_usound;
	guchar	start_amplitude;
	guchar	end_amplitude;
	guchar	range_amplitude;
	guchar	center_amplitude;
} ZOOM_DISPLAY_INFO, *ZOOM_DISPLAY_INFO_P;

/* 闸门信息 */
typedef struct _gate_info
{
	guchar	parameters;		/* 调节模式 0 Positions 1 Mode */
	guchar	synchro;        /* 同步 */
	guchar	measure;        /*  */
	guchar	rectifier_freq; /* 射频 */
	gchar	height;
	gchar	tt[3];
	gint	start;
	guint	width;
} GATE_INFO, *GATE_INFO_P;

/* 报警信息 */
typedef struct _alarm_info 
{
	guchar	groupa;   	    /* GROUP A     */
	guchar	conditiona;     /* Condition A */
	guchar	operat;         /* Operator    */
	guchar	groupb;	        /* GROUP B 	   */
	guchar	conditionb;     /* Condition B */
	guchar	tt[3];
} ALARM_INFO, *ALARM_INFO_P;

/* 输出信息 */
typedef struct _output_info 
{
	guchar	alarm1;   	    /* Alarm # 编号  */
	guchar	alarm1_qty;		/* 开启alarm的个数*/
	gushort	alarm1_status;	/* 状态 */
	guchar	count;    	    /* count    */
	guchar	sound;          /* sound    */
	gchar	tt[2];
	guint	delay;			/* delay 	*/
	guint	holdtime;       /* holdtime */
} OUTPUT_INFO, *OUTPUT_INFO_P;

/* 光标信息 */
typedef struct _cursors_info 
{
	gshort	angle;
	gushort	amplitude;
	gushort	UT;
	gchar	tt[2];
	gint	scan;
	gint	index;
} CURSORS_INFO, *CURSORS_INFO_P;

/* 输出信息 */
typedef struct _analog_info 
{
	guchar	group;	/* GROUP */
	guchar	data;	/* DATA */
	gchar	tt[2];
} ANALOG_INFO, *ANALOG_INFO_P;

/* 探头(Probe)  516 byte 如果探头是unknown 必须给一个参数 */
typedef struct _Probe
{
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
	gushort	Frequency;		/* 频率 */
	guint	A5[75];
	gushort A6;
	gushort A7;
	gushort A8;
	gushort	Reference_Point; /*  */
	guint	A9[36];
} PROBE, *PROBE_P;

/*楔块 (Wedge)*/
typedef struct _Wedge
{
	guchar	A1[2];			/* 0x03000300 PA 0x01000100 UT*/
	gchar	Wave_type;		/* 1 是L 2 是 S */
	gchar	A11;
	gchar	Model[20];		/* 共用 楔块名字 */
	gchar	Serial[20];		/* 共用 楔块名字 */
	gushort	Angle;			/* 共用 角度单位0.1度 */
	gushort	A7;
	gushort	Probe_delay; 	/* UT ns为单位 */
	gchar	A2;
	gchar	A10;			/* UT 1 SW 0 LW*/
	gint	Ref_point;		/* UT 使用 */
	/*这个地方 得 多读一个字节 */
	guint	Height;			/* 单位微米 */
	guint	Velocity_UT;
	guint	A8;
	guint	Velocity_PA;	/* 速度 mm/s */
	gchar	Orientation;	/* 1 Normal 0 reversal*/
	gchar	A4[3];
	gint	Primary_offset;	/* 微米 */
	guint	Secondary_offset;	/* 微米 */
	gint	A6[107];
} WEDGE, *WEDGE_P;


struct _Part;
typedef struct _Part st_PART;

/* 工件 (Part) */
struct _Part
{
	guchar	Geometry;		/* 几何形状 FLAT/ID/OD/BALL */
	guchar	Material_pos;	/* 材料 */
	guchar	tt[2];			/* 保留 */
	guint	Thickness;		/* 厚度 */
	guint	Diameter;		/* 直径 */
};

/* 一个阵元的聚焦信息*/
typedef struct _law_elem
{
	guchar	E_number;
	guchar	Amplitude;		/* 电压 单位V */
	gushort	FL_gain;		/* Focal Gain 0.1db单位 */
	gushort	R_delay;		/*  */
	gushort	T_delay;		/* 0-25560 单位 ns 65535不活动状态 */
	gushort	P_width;		/* 50-500 单位ns*/
	guchar	tt[2];
}LAW_ELEM, *LAW_ELEM_P;

/* 一条beam的聚焦信息 */
typedef struct _law_beam
{
	/*    gchar Version[32];	*/
	/*    gint  N_laws;			*/

	guchar	N_ActiveElements;	/* 同时激发的阵元数 */
	guchar	cycle;
	guchar	sumgain;			/*  */
	guchar	mode;				/* 0 T/R 1 Pulse-echo*/
	guchar	filter;				/* 0 no filter 1 0.5-5 2 2-10 3 5-15 */
	guchar	T_first;
	guchar	R_first;
	gchar	tt;					/* 占位置 */
	gushort	frequency;			/* 频率0.001Mhz 为单位 */
	gshort	R_angle;
	gushort	S_angle;
	gchar	tt1[2];				/* 占位置 */
	guint	Scan_offset;		/* 单位 0.001 mm*/
	guint	Index_offset;		/* 单位 0.001 mm*/
	gint	G_delay;			/* 单位ns Wedge Delay + Law Delay */
	gint	beam_delay;			/* 单位ns Law Delay */
	gint	F_depth;			/* 单位 微粒 */
	guint	M_velocity;			/* 单位 m/s */
}LAW_BEAM, *LAW_BEAM_P;

typedef struct _law_focal
{
	gchar		version[16];
	gshort		beam_qty;		/* 这个focal有几个beam */
	LAW_BEAM_P	*law_beam_p;
	LAW_ELEM_P	*law_elem_p;
} LAW_FOCAL, *LAW_FOCAL_P;

/* 聚焦法则信息(law_info) */
typedef struct _law_info
{
	guchar	Focal_type;			/* 聚焦类型 */
	guchar	Focal_point_type;	/* 聚焦类型 */
	guchar	Tx_connect;			/* pulser该法则的探头发射第一个阵元其连接口的编号 */
	guchar	Rx_connect;			/* receiver */
	guchar	Elem_qty;			/* 聚焦阵元数 */
	guchar	First_tx_elem;		/* 法则使用的第一个发射阵元 收发分离时候 tx rx 不一样 */
	guchar	First_rx_elem;		/* 法则使用的第一个接收阵元 */
	guchar	Last_tx_elem;		/* 法则使用的最后一个发射阵元 */
	guchar	Last_rx_elem;		/* 法则使用的最后一个接收阵元 */
	guchar	Elem_step;			/* 线扫时候可以设置阵元间隔 */
	guchar	Wave_type;			/* 纵波 或者 横波 */
	guchar	res1;
	gshort	Angle_min;			/* PA 时候是开始角度 UT时候记录探头角度 */
	gshort	Angle_max;			/* 扇扫时候可以设置的角度 0.01度为单位 */
	gushort	Angle_step;			/* 扇扫时候可以设置的角度步进 */
	gshort	Angle_beam_skew_min;		
	gshort	Angle_beam_skew_max;		
	gushort	Angle_beam_skew_step;	/* 2D的时候beam skew angle */
	gushort	law_index_start;		/* 聚焦法则索引 计算出来的 */
	gushort law_index_end;			/*  */
	guint	Focus_depth;			/* 扇扫时候为声程 线扫是深度 0.001mm为单位 */
	guint	Position_start;
	guint	Position_end;
	guint	Position_step;
	guint	Offset_start;
	guint	Depth_start;
	guint	Offset_end;
	guint	Depth_end;
} LAW_INFO, *LAW_INFO_P;

/*测量数据 (MEASURE_DATA)*/
typedef struct _Measure_data
{
	guint	a_height;
	guint	AdBA;
	guint	AdBr;
	guint	b_height;
	guint	BdBB;
	guint	BdBr;
	guint	a_position;
	guint	b_position;
	guint	i_position;
	guint	iw_position;
	guint	TA;
	guint	ML;
	guint	TT[47];
	gint	SA;
} MEASURE_DATA, *MEASURE_DATA_P;

/* 组信息 */
typedef struct _Group 
{
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

	GATE_INFO    gate[setup_MAX_GATE];	/* gate[0] A , gate[1] B , gate[2] I */

	guchar	mode_pos;		/*Gate/Alarm->Sizing Curves->Mode*/
	guchar	curve_pos;		/*Gate/Alarm->Sizing Curves->Curve*/
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

	guchar	group_mode;     /* 组工作模式  0 UT or 1 PA*/
	LAW_INFO	law_info;	/* 聚焦法则的信息  */
	PROBE	probe;
	WEDGE	wedge;

	guchar	selection;
	gushort	per_reference;	/* 参考光标的高度 */
	gushort	per_measure;	/* 测量光标的高度 */
	guint	u_reference;	/* 参考光标在超声轴的位置 */
	guint	u_measure;		/* 测量光标在超声轴的位置 */
	gint	s_reference;	/* 参考光标在扫描轴的位置 */	     
	gint	s_measure;		/* 测量光标在扫描轴的位置 */
	guchar	data_link;
	gint	i_reference;	/* 参考光标在指数轴的位置 */
	gint	i_measure;		/* 测量光标在指数轴的位置 */
	gushort	s_refmeas;
	gushort	cursors_angle;
	CURSORS_INFO	cursors_info[4];
	guchar	source;

	guchar	col_select_pos;	/* 0 Amplitude  1 TOFD  2 Depth 共3个*/
	guchar	col_start;
	guchar	col_end;                    
	guchar	col_contrast;                                     
	guchar	col_brightness;                     
	guint	col_min;                          
	guint	col_max;                           
	guchar	col_mode; 

	/* Display Ascan的设置 */
	guchar	ascan_color;		/**/
	guchar	ascan_envelope;		/* 0 None 1 Infinite */
	guchar	ascan_source;		/**/
	guchar	ascan_appearance;	/**/
	guchar	ascan_overlay;		/**/

	/*	COL_SELECT_INFO	col_select[3];*/ /* Amplitude TOFD Depth 3个*/

	/*波束 beam*/
	gint	scan_offset;		/**/
	gint	index_offset;		/**/
	gint	angle;				/**/
	gushort	skew;				/**/
	guchar  skew_pos;

	gint	agate_start;
	guint	agate_width;

} GROUP, *GROUP_P;

/*配置信息 (CONFIG)*/
typedef	struct _Config 
{
	guchar	groupId;			/* 当前group */
	guchar	groupQty;			/* 共有几个group  0 1 2 3 4 5 6 7 */
	guchar	voltage_pa;			/* PA UT 的电压 */
	guchar	voltage_ut;	
	guchar	language;			/* 语言 */
	guchar	reject;		        /* 抑制 */
	guchar	auto_height;		/* 自动增益高度*/

	/*显示*/
	guchar	display;			/* 显示模式 A B C A+B A+B+C A+S ... */
	guchar	display_group;		/* 显示所有group还是 当前 */
	guchar	c_scan1;			/* c扫描参考1 */
	guchar	c_scan2;			/* c扫描参考2 */
	guchar	c_scan11;			/* ASCscan 时候的c扫描参考*/
	guchar	data1;				/* Strip Chart A 的DATA */
	guchar	data2;				/*  */
	guchar	dis_mode;			/* Strip Chart A 时候的模式 */
	guint	dis_range;			/* Strip Chart A 时候的显示范围 */
	guint	avg_scan_speed;		/* 这个值只能显示不用加在这里的 delete */

	guint	prf_virtual;

	guchar	alarm_pos;          /* 当前选择 报警信息 0~15 */
	guchar	output_pos;			/* 0~5 */

	ALARM_INFO	alarm[16];		/* alarm[0], alarm[1], ..., alarm[15] */

	OUTPUT_INFO	output[3];		/* 输出信息 output[0],output[1],output[2] */
	ANALOG_INFO	analog[2];		/* Analog 2 个 */

	GROUP	group[setup_MAX_GROUP_QTY];			/* */
	MEASURE_DATA	measure_data;

	st_PART	part;				/* 被检测工件	*/
	/* 所有聚焦法则的信息在这里 */
	LAW_FOCAL	focal_law_all_info[setup_MAX_GROUP_QTY];
//	LAW_BEAM	focal_law_all_beam[setup_MAX_LAW_QTY];
//	LAW_ELEM	focal_law_all_elem[setup_MAX_LAW_QTY][setup_MAX_ELEM_RX_ACTIVE];	

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

	guint	VPA;                            /*Measurements->Cursors->VPA*/
	guint	cursors_scan;                   /*Measurements->Cursors->Scan*/
	guint	cursors_index;                  /*Measurements->Cursors->index*/
	guchar	display_table;                  /*Measurements->Table->display table*/
	guchar	entry_image;                   /*Measurements->Table->entry image*/
	guchar	entry_qty;                     /*Measurements->Table->Select Entry*/

	guint	min_thickness;                  /*Measurements->Thickness->min*/
	guint	max_thickness;                  /*Measurements->Thickness->max*/
	guchar	echo_qty;                       /*Measurements->Thickness->echo_qty*/


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
	guchar	remote_desktop;                       /* Preferences -> Options -> mouse*/

	guchar	edit_notes_info[256];
	guchar	edit_header_info[256];
} CONFIG, *CONFIG_P;

/* fetch material info of current config 获取当前配置中材料的信息 */
extern gint	 parse_material_info (CONFIG *p);
extern void  print_material_info (CONFIG *p);
extern gchar *get_material_name (CONFIG *p);
extern guint get_material_lw (CONFIG *p);
extern guint get_material_sw (CONFIG *p);

/* part operations 工件的操作 */
extern guint get_part_geometry (CONFIG *p);
extern void set_part_geometry (CONFIG *p, gpointer data);
extern guint get_part_material (CONFIG *p);
extern void set_part_material (CONFIG *p, gpointer data);
extern guint get_part_thickness (CONFIG *p);
extern void set_part_thickness (CONFIG *p, gpointer data);
extern guint get_part_diameter (CONFIG *p);
extern void set_part_diameter (CONFIG *p, gpointer data);



#endif
