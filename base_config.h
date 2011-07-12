
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

/* display信息 */
typedef struct _display_info
{
	guchar	display_group;		/*  0 All  1 current */

} DISPLAY_INFO, *DISPLAY_INFO_P;

/* 闸门信息 */
typedef struct _gate_info
{
	guchar	parameters;		/* 调节模式 0 Positions 1 Mode */
	guchar	synchro;        /* 同步 */
	guchar	measure;        /*  */
	guchar	rectifier_freq; /* 射频 */
	gchar	height;
	gchar	tt[3];
	gint	start;			/* 单位ns */
	guint	width;
} GATE_INFO, *GATE_INFO_P;

/* 输出信息 */
typedef struct _output_info 
{
	gushort	alarm_info;		/* 状态 表示16个alarm信号的状态 */
	guchar	count;    	    /**/
	guchar	sound;          /**/
	guint	delay;			/* 单位ns */
	guint	holdtime;       /* 单位ns */
} OUTPUT_INFO1, *OUTPUT_INFO_P1;

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

/* 工件 (Part) 12字节 已经全部移走只有CONFIG结构体用到了它 等待隐藏 */
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
	gshort	First_tx_elem;		/* 法则使用的第一个发射阵元 收发分离时候 tx rx 不一样 */
	gshort	First_rx_elem;		/* 法则使用的第一个接收阵元 */
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
	guchar	groupId;			/* 当前group 0-7 */
	guchar	groupQty;			/* 共有几个group  1-8 */
	guchar	voltage_pa;			/* PA UT 的电压 */
	guchar	voltage_ut;	
	guchar	language;			/* 语言 */
	guchar	reject;		        /* 抑制 */
/*显示*/
	guchar	display_pos;		/* 显示模式 A B C A+B A+B+C A+S ... */
	guchar	display_group;		/* 显示所有group还是 当前 */
	guchar	c_scan_pos;			/* c扫描的source*/
	guchar	data1;				/* Strip Chart A 的DATA */
	guchar	data2;				/*  */
	guchar	dis_mode;			/* Strip Chart A 时候的模式 */
	guint	dis_range;			/* Strip Chart A 时候的显示范围 */

	guchar	alarm_pos;          /* 当前选择报警信息 0~15 */
	guchar	output_pos;			/* 选择输出0~4 */
	guchar	analog1[2];			/* Analog 2个 0-2位表示group 3-4位是data */
	gushort	alarm_info[16];		/* 存储alarm 配置的信息 */
	OUTPUT_INFO1 output1[3];	/* 输出信息 output[0],output[1],output[2] */

	GROUP	group[setup_MAX_GROUP_QTY];			/* */
//	MEASURE_DATA	measure_data;
	st_PART	part;				/* 被检测工件	*/
	/*选项*/
	guchar	bright;				/* 亮度 0~100 */

	gushort	year;
	guchar	month;
	guchar	day;
	guchar	hour;
	guchar	minitue;
	guchar	second;
	guchar	unit;				/* 0 mm 1 inch */

	guchar	list;				/* Measurements->Reading->list filed  测量的list选项 */
	guchar	field[4];			/* 4个测量值选择 */

	guchar	display_table;		/* Measurements->Table->display table 测量数据保存显示  ON or OFF */
	guchar	entry_image;		/* Measurements->Table->entry image   ON or OFF */
	guint	cur_entry;			/* Measurements->Table->Select Entry 增加一组测量数据(entry)  */
	guint	entry_qty;			/* Measurements->Table->Select Entry 删除一组测量数组 */

	guint	min_thickness;		/* Measurements->Thickness->min */
	guint	max_thickness;		/* Measurements->Thickness->max */
	guchar	echo_qty;			/* Measurements->Thickness->echo_qty */

	guchar	grid;				/* Display->overlay->grid 网格选项 */
	guint	on_off_status;		
	/* 保存各种ON OFF 菜单的值
	0位 Display->overlay->sizing curves 曲线的显示开关
	1位 Display->overlay->gate 闸门的显示开关 
	2位 Display->overlay->cursor  标尺显示开关 
	3位 Display->overlay->overlay 什么的显示开关? 
	4位 Display->Properties->Bscan.optimum B扫描最优开关 
	5位 Display->Properties->Cscan.ratio1:1 C扫描开关 
	6位 Display->Properties->Sscan.interpolation S扫描插值开关 
	7位 Display->Properties->Stripchart.orientation Strip chart 方向 0垂直 1水平 
	8位 探头自动检测开关 
	9位	选择探头或者是自定义探头
	10位 probe/part characterize FFT 开关
	11位 聚集法则 auto_program 开关
	*/	
	guchar	dis_prop_scan;		/* Display->Properties->Scan  6个 */
	guchar	fft_color;			/* Display->Properties->fft_color */
	guint	bcompress;			/* Display->Properties->compress */

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
extern gchar	*get_material_name (CONFIG *p);
extern guint	get_material_lw (CONFIG *p);
extern guint	get_material_sw (CONFIG *p);

/* part operations 工件的操作 */
extern guchar	get_part_geometry (CONFIG *p);
extern void		set_part_geometry (CONFIG *p, gpointer data);
extern guchar	get_part_material (CONFIG *p);
extern void		set_part_material (CONFIG *p, gpointer data);
extern guint	get_part_thickness (CONFIG *p);
extern void		set_part_thickness (CONFIG *p, gpointer data);
extern guint	get_part_diameter (CONFIG *p);
extern void		set_part_diameter (CONFIG *p, gpointer data);

/* 配置信息的操作 */
extern guchar	get_current_group (CONFIG *p);
extern void		set_current_group (CONFIG *p, guchar data);
extern guchar	get_group_qty (CONFIG *p);
extern void		set_group_qty (CONFIG *p, guchar data);
extern guchar	get_voltage (CONFIG *p, guint group_num);
extern void		set_voltage (CONFIG *p, guint group_num, guchar data);
extern guchar	get_language (CONFIG *p);
extern void		set_language (CONFIG *p, guchar data);
extern guchar	get_reject (CONFIG *p);
extern void		set_reject (CONFIG *p, guchar data);
extern guchar	get_display_pos (CONFIG *p);
extern void		set_display_pos (CONFIG *p, guchar data);
extern guchar	get_display_group (CONFIG *p);
extern void		set_display_group (CONFIG *p, guchar data);
extern guchar	get_cscan_source (CONFIG *p, guint pos);
extern void		set_cscan_source (CONFIG *p, guchar data, guint pos);
extern guchar	get_stripscan_data1 (CONFIG *p);
extern guchar	get_stripscan_data2 (CONFIG *p);
extern guchar	get_stripscan_mode (CONFIG *p);
extern guint	get_stripscan_disrange (CONFIG *p);
extern void		set_stripscan_data1 (CONFIG *p, guchar data);
extern void		set_stripscan_data2 (CONFIG *p, guchar data);
extern void		set_stripscan_mode (CONFIG *p, guchar data);
extern void		set_stripscan_disrange (CONFIG *p, guint data);
/* ALARM */
extern guchar	get_alarm_pos (CONFIG *p);
extern guchar	get_alarm_groupa (CONFIG *p);
extern guchar	get_alarm_groupb (CONFIG *p);
extern guchar	get_alarm_conditiona (CONFIG *p);
extern guchar	get_alarm_conditionb (CONFIG *p);
extern guchar	get_alarm_operator (CONFIG *p);
extern void		set_alarm_pos (CONFIG *p, guchar data);
extern void		set_alarm_groupa (CONFIG *p, guchar data);
extern void		set_alarm_groupb (CONFIG *p, guchar data);
extern void		set_alarm_conditiona (CONFIG *p, guchar data);
extern void		set_alarm_conditionb (CONFIG *p, guchar data);
extern void		set_alarm_operator (CONFIG *p, guchar data);
/* OUTPUT */
extern guchar	get_output_pos (CONFIG *p);
extern gushort	get_output_alarm (CONFIG *p);
extern gushort	get_1output_alarm_pos (CONFIG *p);
extern guchar	get_output_alarm_qty (CONFIG *p);
extern guchar	get_output_alarm_pos (CONFIG *p, guint pos);
extern void		set_output_pos (CONFIG *p, guchar data);
extern void		set_output_alarm (CONFIG *p, guchar data, guint pos);
extern guchar	get_output_count (CONFIG *p);
extern guchar	get_output_sound (CONFIG *p);
extern void		set_output_count (CONFIG *p, guchar data);
extern void		set_output_sound (CONFIG *p, guchar data);
extern guint	get_output_delay (CONFIG *p);
extern guint	get_output_holdtime (CONFIG *p);
extern void		set_output_delay (CONFIG *p, guint data);
extern void		set_output_holdtime (CONFIG *p, guint data);
extern guchar	get_output_group (CONFIG *p);
extern guchar	get_output_data (CONFIG *p);
extern void		set_output_group (CONFIG *p, guchar data);
extern void		set_output_data (CONFIG *p, guchar data);

/* 设置获取亮度 */
extern guchar	get_bright (CONFIG *p);			
extern void		set_bright (CONFIG *p, guchar data);
/* Reading list field 设置当前读取的数值 */
extern guchar	get_reading_list (CONFIG *p);			
extern void		set_reading_list (CONFIG *p, guchar data);
extern guchar	get_reading_field1 (CONFIG *p);			
extern void		set_reading_field1 (CONFIG *p, guchar data);
extern guchar	get_reading_field2 (CONFIG *p);			
extern void		set_reading_field2 (CONFIG *p, guchar data);
extern guchar	get_reading_field3 (CONFIG *p);			
extern void		set_reading_field3 (CONFIG *p, guchar data);
extern guchar	get_reading_field4 (CONFIG *p);			
extern void		set_reading_field4 (CONFIG *p, guchar data);
/* 单位 */
extern guchar	get_unit (CONFIG *p);			
extern void		set_unit (CONFIG *p, guchar data);
/*  */
extern guchar	get_display_table (CONFIG *p);			
extern void		set_display_table (CONFIG *p, guchar data);
extern guchar	get_entry_image (CONFIG *p);			
extern void		set_entry_image (CONFIG *p, guchar data);
extern guint	get_cur_entry (CONFIG *p);			
extern void		set_cur_entry (CONFIG *p, guint data);
extern guint	get_entry_qty (CONFIG *p);			
extern void		set_entry_qty (CONFIG *p, guint data);
extern guint	get_min_thickness (CONFIG *p);			
extern void		set_min_thickness (CONFIG *p, guint data);
extern guint	get_max_thickness (CONFIG *p);			
extern void		set_max_thickness (CONFIG *p, guint data);
extern guchar	get_echo_qty (CONFIG *p);			
extern void		set_echo_qty (CONFIG *p, guchar data);

/* Display Overlay */
extern guchar	get_overlay_grid (CONFIG *p);			
extern void		set_overlay_grid (CONFIG *p, guchar data);
extern gboolean get_overlay_sizing_curves (CONFIG *p);
extern void		set_overlay_sizing_curves (CONFIG *p, gboolean data);
extern gboolean get_overlay_gate (CONFIG *p);
extern void		set_overlay_gate (CONFIG *p, gboolean data);
extern gboolean get_overlay_cursor (CONFIG *p);
extern void		set_overlay_cursor (CONFIG *p, gboolean data);
extern gboolean get_overlay_overlay (CONFIG *p);
extern void		set_overlay_overlay (CONFIG *p, gboolean data);
/* Display Properties */
extern guchar	get_dis_prop_scan (CONFIG *p);			
extern void		set_dis_prop_scan (CONFIG *p, guchar data);
extern guint	get_b_compress (CONFIG *p);			
extern void		set_b_compress (CONFIG *p, guint data);
extern gboolean get_dis_prop_boptimum (CONFIG *p);
extern void		set_dis_prop_boptimum (CONFIG *p, gboolean data);
extern gboolean get_dis_prop_cratio (CONFIG *p);
extern void		set_dis_prop_cratio (CONFIG *p, gboolean data);
extern gboolean get_dis_prop_sinterpolation (CONFIG *p);
extern void		set_dis_prop_sinterpolation (CONFIG *p, gboolean data);
extern gboolean get_dis_prop_strip_orientation (CONFIG *p);
extern void		set_dis_prop_strip_orientation (CONFIG *p, gboolean data);
extern guchar	get_fft_color (CONFIG *p);			
extern void		set_fft_color (CONFIG *p, guchar data);

/* 自动检测探头 */
extern gboolean get_auto_detect (CONFIG *p);
extern void		set_auto_detect (CONFIG *p, gboolean data);

/* 选择探头或者是自定义探头 */
extern gboolean get_probe_select (CONFIG *p);
extern void		set_probe_select (CONFIG *p, gboolean data);

/* 选择探头FFT开关 */
extern gboolean get_probe_fft (CONFIG *p);
extern void		set_probe_fft (CONFIG *p, gboolean data);

/* 选择聚焦法则自动计算开关 */
extern gboolean get_auto_focal (CONFIG *p);
extern void		set_auto_focal (CONFIG *p, gboolean data);

/* group操作 */
extern void		grpcpy (CONFIG *p, guint src, guint dst);

/***
#define	VAL_GRP_ID	0x0001
extern gint	*get_cfg_val (CONFIG *p, gint val_type, gint grp);
***/

#endif
