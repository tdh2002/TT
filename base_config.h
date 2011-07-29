
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

/* 闸门信息 */
typedef struct _gate_info
{
	unsigned char	parameters;		/* 调节模式 0 Positions 1 Mode */
	unsigned char	synchro;        /* 同步 */
	unsigned char	measure;        /*  */
	unsigned char	rectifier_freq; /* 射频 */
	char	height;
	char	tt[3];
	int	start;			/* 单位ns */
	unsigned int	width;
} GATE_INFO, *GATE_INFO_P;


struct _output_info;
typedef struct _output_info OUTPUT_INFO1;

/* 输出信息 */
struct _output_info 
{
	unsigned short	alarm_info;		/* 状态 表示16个alarm信号的状态 */
	unsigned char	count;    	    /**/
	unsigned char	sound;          /**/
	unsigned int	delay;			/* 单位ns */
	unsigned int	holdtime;       /* 单位ns */
};

/* 光标信息 */
typedef struct _cursors_info 
{
	short	angle;
	unsigned short	amplitude;
	unsigned short	UT;
	char	tt[2];
	int	scan;
	int	index;
} CURSORS_INFO, *CURSORS_INFO_P;

struct _simple_Probe;
typedef struct _simple_Probe st_simple_Probe;

struct _simple_Probe
{
	char	Model[20];			/* 探头名字 */
	char	Serial[20];			/* 探头名字 */
	unsigned char	PA_probe_type;		/* 探头类型 1 是Custom 3 是angle beam 5 是 Contact 6是Immersion */
	unsigned char	Elem_qty;			/* 阵元数 */
	unsigned short	Frequency;			/* 频率 */
	unsigned short	Reference_Point;	/*  */
	unsigned int	Pitch;				/* 阵元中心间距 0.001mm 为单位 范围是0.01~65.00mm UT 是Elemet_size */
};

struct _simple_Wedge;
typedef struct _simple_Wedge st_simple_Wedge;

struct _simple_Wedge
{
	char	Wave_type;		/* 1 是L 2 是 S */
	char	Model[20];		/* 共用 楔块名字 */
	char	Serial[20];		/* 共用 楔块名字 */
	char	Orientation;	/* 1 Normal 0 reversal*/
	unsigned short	Angle;			/* 共用 角度单位0.1度 */
	unsigned short	Probe_delay; 	/* UT ns为单位 */
	int	Ref_point;		/* UT 使用 */
	unsigned int	Height;			/* 单位微米 */
	unsigned int	Velocity_UT;
	unsigned int	Velocity_PA;	/* 速度 mm/s */
	int	Primary_offset;	/* 微米 */
	unsigned int	Secondary_offset;	/* 微米 */
};

/* 探头(Probe)  516 byte 如果探头是unknown 必须给一个参数 */
typedef struct _Probe
{
	/*PA 时候先读走4个字节*/
	char	A1[2];
	unsigned char	PA_probe_type;	/* 探头类型 1 是Custom 3 是angle beam 5 是 Contact 6是Immersion */
	char	A10;
	unsigned char	UT_probe_type;	/* 探头类型 1 n/a 0 converntional */
	char	A11;
	char	Model[20];		/* 探头名字 */
	char	Serial[20];		/* 探头名字 */
	unsigned char	Elem_qty;		/* 阵元数 */
	unsigned char	Freq2;			/* UT 时候 频率是 freq2 << 8 + elem_qty */
	unsigned int	Pitch;			/* 阵元中心间距 0.001mm 为单位 范围是0.01~65.00mm UT 是Elemet_size */
	unsigned int	A3;
	unsigned short A4;
	unsigned short	Frequency;		/* 频率 */
	unsigned int	A5[75];
	unsigned short A6;
	unsigned short A7;
	unsigned short A8;
	unsigned short	Reference_Point; /*  */
	unsigned int	A9[36];
} PROBE, *PROBE_P;

/*楔块 (Wedge)*/
typedef struct _Wedge
{
	unsigned char	A1[2];			/* 0x03000300 PA 0x01000100 UT*/
	char	Wave_type;		/* 1 是L 2 是 S */
	char	A11;
	char	Model[20];		/* 共用 楔块名字 */
	char	Serial[20];		/* 共用 楔块名字 */
	unsigned short	Angle;			/* 共用 角度单位0.1度 */
	unsigned short	A7;
	unsigned short	Probe_delay; 	/* UT ns为单位 */
	char	A2;
	char	A10;			/* UT 1 SW 0 LW*/
	int	Ref_point;		/* UT 使用 */
	/*这个地方 得 多读一个字节 */
	unsigned int	Height;			/* 单位微米 */
	unsigned int	Velocity_UT;
	unsigned int	A8;
	unsigned int	Velocity_PA;	/* 速度 mm/s */
	char	Orientation;	/* 1 Normal 0 reversal*/
	char	A4[3];
	int	Primary_offset;	/* 微米 */
	unsigned int	Secondary_offset;	/* 微米 */
	int	A6[107];
} WEDGE, *WEDGE_P;

struct _Encoder;
typedef struct _Encoder st_Enc;

struct _Encoder {
	char Polarity;
	char Type;
	char Preset;
	char Enable;
	unsigned int Resolution;
	unsigned int Origin;
};

struct _Part;
typedef struct _Part st_PART;

/* 工件 (Part) 12字节 已经全部移走只有CONFIG结构体用到了它 等待隐藏 */
struct _Part
{
	unsigned char	Geometry;		/* 几何形状 FLAT/ID/OD/BALL */
	unsigned char	Material_pos;	/* 材料 */
	unsigned char	tt[2];			/* 保留 */
	unsigned int	Thickness;		/* 厚度 */
	unsigned int	Diameter;		/* 直径 */
};

/* 一个阵元的聚焦信息*/
typedef struct _law_elem
{
	unsigned char	E_number;
	unsigned char	Amplitude;		/* 电压 单位V */
	unsigned short	FL_gain;		/* Focal Gain 0.1db单位 */
	unsigned short	R_delay;		/*  */
	unsigned short	T_delay;		/* 0-25560 单位 ns 65535不活动状态 */
	unsigned short	P_width;		/* 50-500 单位ns*/
	unsigned char	tt[2];
}LAW_ELEM, *LAW_ELEM_P;

/* 一条beam的聚焦信息 */
typedef struct _law_beam
{
	/*    char Version[32];	*/
	/*    int  N_laws;			*/

	unsigned char	N_ActiveElements;	/* 同时激发的阵元数 */
	unsigned char	cycle;
	unsigned char	sumgain;			/*  */
	unsigned char	mode;				/* 0 T/R 1 Pulse-echo*/
	unsigned char	filter;				/* 0 no filter 1 0.5-5 2 2-10 3 5-15 */
	unsigned char	T_first;
	unsigned char	R_first;
	char	tt;					/* 占位置 */
	unsigned short	frequency;			/* 频率0.001Mhz 为单位 */
	short	R_angle;
	unsigned short	S_angle;
	char	tt1[2];				/* 占位置 */
	unsigned int	Scan_offset;		/* 单位 0.001 mm*/
	unsigned int	Index_offset;		/* 单位 0.001 mm*/
	int	G_delay;			/* 单位ns Wedge Delay + Law Delay */
	int	beam_delay;			/* 单位ns Law Delay */
	int	F_depth;			/* 单位 微粒 */
	unsigned int	M_velocity;			/* 单位 m/s */
}LAW_BEAM, *LAW_BEAM_P;

typedef struct _law_focal
{
	char		version[16];
	short		beam_qty;		/* 这个focal有几个beam */
	LAW_BEAM_P	*law_beam_p;
	LAW_ELEM_P	*law_elem_p;
} LAW_FOCAL, *LAW_FOCAL_P;

/* 聚焦法则信息(law_info) */
typedef struct _law_info
{
	unsigned char	Focal_type;			/* 聚焦类型 */
	unsigned char	Focal_point_type;	/* 聚焦类型 */
	unsigned char	Tx_connect;			/* pulser该法则的探头发射第一个阵元其连接口的编号 */
	unsigned char	Rx_connect;			/* receiver */
	unsigned char	Elem_qty;			/* 聚焦阵元数 */
	short	        First_tx_elem;		/* 法则使用的第一个发射阵元 收发分离时候 tx rx 不一样 */
	short	        First_rx_elem;		/* 法则使用的第一个接收阵元 */
	unsigned char	Last_tx_elem;		/* 法则使用的最后一个发射阵元 */
	unsigned char	Last_rx_elem;		/* 法则使用的最后一个接收阵元 */
	unsigned char	Elem_step;			/* 线扫时候可以设置阵元间隔 */
	unsigned char	Wave_type;			/* 纵波 或者 横波 */
	unsigned char	res1;
	short	        Angle_min;			/* PA 时候是开始角度 UT时候记录探头角度 */
	short	        Angle_max;			/* 扇扫时候可以设置的角度 0.01度为单位 */
	unsigned short	Angle_step;			/* 扇扫时候可以设置的角度步进 */
	short	        Angle_beam_skew_min;		
	short	        Angle_beam_skew_max;		
	unsigned short	Angle_beam_skew_step;	/* 2D的时候beam skew angle */
	unsigned short	law_index_start;		/* 聚焦法则索引 计算出来的 */
	unsigned short  law_index_end;			/*  */
	unsigned int	Focus_depth;			/* 扇扫时候为声程 线扫是深度 0.001mm为单位 */
	unsigned int	Position_start;
	unsigned int	Position_end;
	unsigned int	Position_step;
	unsigned int	Offset_start;
	unsigned int	Depth_start;
	unsigned int	Offset_end;
	unsigned int	Depth_end;
} LAW_INFO, *LAW_INFO_P;

/*测量数据 (MEASURE_DATA)*/
typedef struct _Measure_data
{
	//目前用到的
	unsigned int	a_height;//%A
	unsigned int	b_height;//%B
	unsigned int	a_position;//A^
	unsigned int	b_position;//B^
	unsigned int    i_position;//I/
	unsigned int	a_ref_position;//RA^
	unsigned int	b_ref_position;//RB^
	unsigned int	a_probe_front;//PA^
	unsigned int	b_probe_front;//PB^
	unsigned int	a_depth;//DA^
	unsigned int	b_depth;//DB^
	unsigned int	a_sound_path;//SA^
	unsigned int	b_sound_path;//SB^
	unsigned int	a_volu_position_i;//ViA
	unsigned int	b_volu_position_i;//ViB
	unsigned int	a_volu_position_s;//VsA
	unsigned int	b_volu_position_s;//VsB
	unsigned int	a_leg;//LA
	unsigned int	b_leg;//LB
    
	//其他待添加
} MEASURE_DATA, *MEASURE_DATA_P;

struct _Group;
typedef struct _Group GROUP;

/* 组信息 */
struct _Group 
{
	/* 基本设置 */
	unsigned int	wedge_delay;	/* 楔款延时 单位 ns */
	unsigned int	range;			/* 显示范围 单位 ns */
	int	start;						/* 扫描延时 单位 ns */
	unsigned short	gain;			/* 实际增益 单位 0.01dB */
	unsigned short	gainr;			/* 参考增益 单位 0.01dB */
	unsigned int	velocity;		/* 实际声速 单位 0.01m/s  */
	unsigned char	db_ref;			/* 参考增益开关 0 off 1 on */
	/* 发射接收 */
	unsigned char	pulser;			/* 1~ 128 - elem_qty(聚焦阵元数最大为32) + 1 
							   指定发射阵元 与机器配置相关我们是128阵元最大,
							   Probe 的Auto Program 选择On 以后不可以调节 值与connect P 一样 */
	unsigned char  receiver;		/* 接收阵元 必须是 PR 模式才能调节 */
	unsigned char	filter;			/* 滤波 */
	unsigned char	rectifier;		/* 检波  */
	unsigned char	averaging;		/* 平均 */
	unsigned char	video_filter;	/* 视频滤波*/

	unsigned char  tx_rxmode;		/* 收发模式 */
	unsigned char	freq_pos;		/* 频率选择模式是指定还是自己输入 */
	unsigned char	pw_pos;			/* 脉冲宽度选择模式 */
	unsigned char	prf_pos;		/* 脉冲宽度选择模式 */
	unsigned short	frequency;		/* 以0.001Mhz 也就是Khz 1MHz~20MHz 为单位 当探头学选unknown 时候才可以调节 */
	unsigned short	pulser_width;	/* 30~500ns 2.5ns为步进*/

	unsigned int	prf;			/* 重复频率 1-20000Hz 取值为10~200000 */

	unsigned char	point_qty_pos;	/* 点个数 位置 */
	unsigned char	sum_gain_pos;	/**/
	unsigned char  gate_pos;		/* A , B , I 当前修改的是哪个闸门 */
	unsigned short	point_qty;		/* 点个数 */
	unsigned short	sum_gain;
	unsigned short	gain_offset;

	GATE_INFO    gate[setup_MAX_GATE];	/* gate[0] A , gate[1] B , gate[2] I */

	unsigned char	mode_pos;		/*Gate/Alarm->Sizing Curves->Mode*/
	unsigned char	curve_pos;		/*Gate/Alarm->Sizing Curves->Curve*/
	unsigned short ref_ampl;
	unsigned short ref_ampl_offset;
	unsigned short curve_step;
	unsigned short ref_gain;
	unsigned int   position;
	unsigned char  point_pos;
	unsigned int   amplitude;
	unsigned short mat_atten;
	unsigned int   delay;
	unsigned short tcg_gain;

	unsigned char	ut_unit;		/*检测单位 时间2 声程1  实际深度0 .*/

	unsigned char	group_mode;     /* 组工作模式  0 UT or 1 PA*/
	LAW_INFO	law_info;	/* 聚焦法则的信息  */
	PROBE	probe;
	WEDGE	wedge;

	unsigned char	selection;
	unsigned short	per_reference;	/* 参考光标的高度 */
	unsigned short	per_measure;	/* 测量光标的高度 */
	unsigned int	u_reference;	/* 参考光标在超声轴的位置 */
	unsigned int	u_measure;		/* 测量光标在超声轴的位置 */
	int	s_reference;	/* 参考光标在扫描轴的位置 */	     
	int	s_measure;		/* 测量光标在扫描轴的位置 */
	unsigned char	data_link;
	int	i_reference;	/* 参考光标在指数轴的位置 */
	int	i_measure;		/* 测量光标在指数轴的位置 */
	unsigned short	s_refmeas;
	unsigned short	cursors_angle;
	CURSORS_INFO	cursors_info[4];
	unsigned char	source;

	unsigned char	col_select_pos;	/* 0 Amplitude  1 TOFD  2 Depth 共3个*/
	unsigned char	col_start;
	unsigned char	col_end;                    
	unsigned char	col_contrast;                                     
	unsigned char	col_brightness;                     
	unsigned int	col_min;                          
	unsigned int	col_max;                           
	unsigned char	col_mode; 

	/* Display Ascan的设置 */
	unsigned char	ascan_color;		/**/
	unsigned char	ascan_envelope;		/* 0 None 1 Infinite */
	unsigned char	ascan_source;		/**/
	unsigned char	ascan_appearance;	/**/
	unsigned char	ascan_overlay;		/**/

	/*	COL_SELECT_INFO	col_select[3];*/ /* Amplitude TOFD Depth 3个*/

	/*波束 beam*/
	int	scan_offset;		/**/
	int	index_offset;		/**/
	int	angle;				/**/
	unsigned short	skew;				/**/
	unsigned char  skew_pos;

	int	agate_start;
	unsigned int	agate_width;

};

struct _Config;
typedef struct _Config CONFIG;

/*配置信息 (CONFIG)*/
struct _Config 
{
	unsigned char	groupId;			/* 当前group 0-7 */
	unsigned char	groupQty;			/* 共有几个group  1-8 */
	unsigned char	voltage_pa;			/* PA UT 的电压 */
	unsigned char	voltage_ut;	
	unsigned char	language;			/* 语言 */
	unsigned char	reject;		        /* 抑制 */
	unsigned char	display_pos;		/* 显示模式 A B C A+B A+B+C A+S ... */
	unsigned char	display_group;		/* 显示所有group还是 当前 */
	unsigned char	c_scan_pos;			/* c扫描的source*/
	unsigned char	data1;				/* Strip Chart A 的DATA */
	unsigned char	data2;				/*  */
	unsigned char	dis_mode;			/* Strip Chart A 时候的模式 */
	unsigned int	dis_range;			/* Strip Chart A 时候的显示范围 */
	unsigned char	bright;				/* 亮度 0~100 */
	unsigned char	alarm_pos;          /* 当前选择报警信息 0~15 */
	unsigned char	output_pos;			/* 选择输出0~4 */
	unsigned char	analog1[2];			/* Analog 2个 0-2位表示group 3-4位是data */
	unsigned short	alarm_info[16];		/* 存储alarm 配置的信息 */
	unsigned char	list;				/* Measurements->Reading->list filed  测量的list选项 */
	unsigned char	field[4];			/* 4个测量值选择 */
	OUTPUT_INFO1    output1[3];	/* 输出信息 output[0],output[1],output[2] */
	GROUP	        group[setup_MAX_GROUP_QTY];			/* */
	MEASURE_DATA	measure_data[setup_MAX_LAW_QTY];//保存多个聚焦法则的测量数据
	st_PART	part;				/* 被检测工件	*/
	unsigned short	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	minitue;
	unsigned char	second;
	unsigned char	unit;				/* 0 mm 1 inch */
	unsigned int	cur_entry;			/* Measurements->Table->Select Entry 增加一组测量数据(entry)  */
	unsigned int	entry_qty;			/* Measurements->Table->Select Entry 删除一组测量数组 */
	unsigned int	min_thickness;		/* Measurements->Thickness->min */
	unsigned int	max_thickness;		/* Measurements->Thickness->max */
	unsigned char	echo_qty;			/* Measurements->Thickness->echo_qty */
	unsigned char	grid;				/* Display->overlay->grid 网格选项 */
	unsigned char	dis_prop_scan;		/* Display->Properties->Scan  6个 */
	unsigned char	fft_color;			/* Display->Properties->fft_color */
	unsigned int	on_off_status;		
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
	12位 encoder 编码器当前选择 0表示编码器1 1表示编码器2
	13位 空
	14-15位 Scan->Start->start mode 00 reset all  01 reset encoder 10 reset data
	16位 Scan->start->pause 0 关 1 开
	17-18位 Scan->Data->Storage 00 01 10 11
	19位 Scan->Data->Inspec Data 0 1
	20-21 File->File->Storage 保存在什么地方  00 01 10 11
	22-23 File->File->Save Mode 保存什么数据 00 01 10 11
	24位 File->Report->Template
	25位 File->Format->User field
	26位 File->Format->Probe
	27位 File->Format->Setup
	28位 File->Format->Note
	29-30位 File->Format->View 00 01 10 11
	*/	
	unsigned int	on_off_status1;		
	/*
	0位 Measurements->Table->display table 测量数据保存显示  ON or OFF 
	1位 Measurements->Table->entry image   ON or OFF 
	*/
	unsigned int	bcompress;			/* Display->Properties->compress */
	st_Enc	encoder1[setup_MAX_QTY_ENCODER];
	unsigned char	report_paper_size;	/* File->Report->paper size */
	unsigned char	inspection_type;	/* Scan->Inspection->Type On-line  Raster  Helicoidal */
	unsigned char	inspection_scan;	/* Scan->Inspection->Scan Time  Encoder1,2  */
	unsigned char	inspection_index;	/* Scan->Inspection->Index auto encoder1,2 topturn off  */
	unsigned int	inspection_scanspeed;	/* Scan->Inspection->Index scanspeed */
	unsigned int	inspection_rpmscanspeed;/* Scan->Inspection->Index rpm scanspeed */
	unsigned int	inspection_indexspeed;	/* Scan->Inspection->Index rpm scanspeed */
	int	inspection_scan_start;		/* Scan->Area->Scan start*/
	int	inspection_scan_end;		/* Scan->Area->Scan end*/
	int	inspection_scan_resolution;	/* Scan->Area->Scan resolution*/
	int	inspection_index_start;		/* Scan->Area->Index start*/
	int	inspection_index_end;		/* Scan->Area->Index end*/
	int	inspection_index_resolution;/* Scan->Area->Index resolution*/
	unsigned int	userfield_select_enable;	/* File->User Field->select userfield 使能状态 */
	char	user_label[10][25];
	char	user_label_content[10][25];
	char	edit_notes_info[256];
	char	edit_header_info[256];
	char	file_name_info[50];
	unsigned char	probe_type;
	unsigned char	userfield_select;			/* File->User Field->select 当前选择userfield编号 */
};

/* fetch material info of current config 获取当前配置中材料的信息 */
extern char	*get_material_name (CONFIG *p);
extern unsigned int	get_material_lw (CONFIG *p);
extern unsigned int	get_material_sw (CONFIG *p);

/* part operations 工件的操作 */
extern unsigned char	get_part_geometry (CONFIG *p);
extern void		set_part_geometry (CONFIG *p, void* data);
extern unsigned char	get_part_material (CONFIG *p);
extern void		set_part_material (CONFIG *p, void* data);
extern unsigned int	get_part_thickness (CONFIG *p);
extern void		set_part_thickness (CONFIG *p, void* data);
extern unsigned int	get_part_diameter (CONFIG *p);
extern void		set_part_diameter (CONFIG *p, void* data);

/* 配置信息的操作start */
extern unsigned char	get_current_group (CONFIG *p);
extern void		set_current_group (CONFIG *p, unsigned char data);
extern unsigned char	get_group_qty (CONFIG *p);
extern void		set_group_qty (CONFIG *p, unsigned char data);
extern unsigned char	get_voltage (CONFIG *p, unsigned int group_num);
extern void		set_voltage (CONFIG *p, unsigned int group_num, unsigned char data);
extern unsigned char	get_language (CONFIG *p);
extern void		set_language (CONFIG *p, unsigned char data);
extern unsigned char	get_reject (CONFIG *p);
extern void		set_reject (CONFIG *p, unsigned char data);
extern unsigned char	get_display_pos (CONFIG *p);
extern void		set_display_pos (CONFIG *p, unsigned char data);
extern unsigned char	get_display_group (CONFIG *p);
extern void		set_display_group (CONFIG *p, unsigned char data);
extern unsigned char	get_cscan_source (CONFIG *p, unsigned int pos);
extern void		set_cscan_source (CONFIG *p, unsigned char data, unsigned int pos);
extern unsigned char	get_stripscan_data1 (CONFIG *p);
extern unsigned char	get_stripscan_data2 (CONFIG *p);
extern unsigned char	get_stripscan_mode (CONFIG *p);
extern unsigned int	get_stripscan_disrange (CONFIG *p);
extern void		set_stripscan_data1 (CONFIG *p, unsigned char data);
extern void		set_stripscan_data2 (CONFIG *p, unsigned char data);
extern void		set_stripscan_mode (CONFIG *p, unsigned char data);
extern void		set_stripscan_disrange (CONFIG *p, unsigned int data);
/* ALARM */
extern unsigned char	get_alarm_pos (CONFIG *p);
extern unsigned char	get_alarm_groupa (CONFIG *p);
extern unsigned char	get_alarm_groupb (CONFIG *p);
extern unsigned char	get_alarm_conditiona (CONFIG *p);
extern unsigned char	get_alarm_conditionb (CONFIG *p);
extern unsigned char	get_alarm_operator (CONFIG *p);
extern void		set_alarm_pos (CONFIG *p, unsigned char data);
extern void		set_alarm_groupa (CONFIG *p, unsigned char data);
extern void		set_alarm_groupb (CONFIG *p, unsigned char data);
extern void		set_alarm_conditiona (CONFIG *p, unsigned char data);
extern void		set_alarm_conditionb (CONFIG *p, unsigned char data);
extern void		set_alarm_operator (CONFIG *p, unsigned char data);
/* OUTPUT */
extern unsigned char	get_output_pos (CONFIG *p);
extern unsigned short	get_output_alarm (CONFIG *p);
extern unsigned short	get_1output_alarm_pos (CONFIG *p);
extern unsigned char	get_output_alarm_qty (CONFIG *p);
extern unsigned char	get_output_alarm_pos (CONFIG *p, unsigned int pos);
extern void		set_output_pos (CONFIG *p, unsigned char data);
extern void		set_output_alarm (CONFIG *p, unsigned char data, unsigned int pos);
extern unsigned char	get_output_count (CONFIG *p);
extern unsigned char	get_output_sound (CONFIG *p);
extern void		set_output_count (CONFIG *p, unsigned char data);
extern void		set_output_sound (CONFIG *p, unsigned char data);
extern unsigned int	get_output_delay (CONFIG *p);
extern unsigned int	get_output_holdtime (CONFIG *p);
extern void		set_output_delay (CONFIG *p, unsigned int data);
extern void		set_output_holdtime (CONFIG *p, unsigned int data);
extern unsigned char	get_output_group (CONFIG *p);
extern unsigned char	get_output_data (CONFIG *p);
extern void		set_output_group (CONFIG *p, unsigned char data);
extern void		set_output_data (CONFIG *p, unsigned char data);

/* 设置获取亮度 */
extern unsigned char	get_bright (CONFIG *p);			
extern void		set_bright (CONFIG *p, unsigned char data);
/* Reading list field 设置当前读取的数值 */
extern unsigned char	get_reading_list (CONFIG *p);			
extern void		set_reading_list (CONFIG *p, unsigned char data);
extern unsigned char	get_reading_field1 (CONFIG *p);			
extern void		set_reading_field1 (CONFIG *p, unsigned char data);
extern unsigned char	get_reading_field2 (CONFIG *p);			
extern void		set_reading_field2 (CONFIG *p, unsigned char data);
extern unsigned char	get_reading_field3 (CONFIG *p);			
extern void		set_reading_field3 (CONFIG *p, unsigned char data);
extern unsigned char	get_reading_field4 (CONFIG *p);			
extern void		set_reading_field4 (CONFIG *p, unsigned char data);
/* 单位 */
extern unsigned char	get_unit (CONFIG *p);			
extern void		set_unit (CONFIG *p, unsigned char data);
/*  */
extern unsigned char	get_display_table (CONFIG *p);			
extern void		set_display_table (CONFIG *p, unsigned char data);
extern unsigned char	get_entry_image (CONFIG *p);			
extern void		set_entry_image (CONFIG *p, unsigned char data);
extern unsigned int	get_cur_entry (CONFIG *p);			
extern void		set_cur_entry (CONFIG *p, unsigned int data);
extern unsigned int	get_entry_qty (CONFIG *p);			
extern void		set_entry_qty (CONFIG *p, unsigned int data);
extern unsigned int	get_min_thickness (CONFIG *p);			
extern void		set_min_thickness (CONFIG *p, unsigned int data);
extern unsigned int	get_max_thickness (CONFIG *p);			
extern void		set_max_thickness (CONFIG *p, unsigned int data);
extern unsigned char	get_echo_qty (CONFIG *p);			
extern void		set_echo_qty (CONFIG *p, unsigned char data);

/* Display Overlay */
extern unsigned char	get_overlay_grid (CONFIG *p);			
extern void		set_overlay_grid (CONFIG *p, unsigned char data);
extern int get_overlay_sizing_curves (CONFIG *p);
extern void		set_overlay_sizing_curves (CONFIG *p, int data);
extern int get_overlay_gate (CONFIG *p);
extern void		set_overlay_gate (CONFIG *p, int data);
extern int get_overlay_cursor (CONFIG *p);
extern void		set_overlay_cursor (CONFIG *p, int data);
extern int get_overlay_overlay (CONFIG *p);
extern void		set_overlay_overlay (CONFIG *p, int data);
/* Display Properties */
extern unsigned char	get_dis_prop_scan (CONFIG *p);			
extern void		set_dis_prop_scan (CONFIG *p, unsigned char data);
extern unsigned int	get_b_compress (CONFIG *p);			
extern void		set_b_compress (CONFIG *p, unsigned int data);
extern int get_dis_prop_boptimum (CONFIG *p);
extern void		set_dis_prop_boptimum (CONFIG *p, int data);
extern int get_dis_prop_cratio (CONFIG *p);
extern void		set_dis_prop_cratio (CONFIG *p, int data);
extern int get_dis_prop_sinterpolation (CONFIG *p);
extern void		set_dis_prop_sinterpolation (CONFIG *p, int data);
extern int get_dis_prop_strip_orientation (CONFIG *p);
extern void		set_dis_prop_strip_orientation (CONFIG *p, int data);
extern unsigned char	get_fft_color (CONFIG *p);			
extern void		set_fft_color (CONFIG *p, unsigned char data);

/* 自动检测探头 */
extern int get_auto_detect (CONFIG *p);
extern void		set_auto_detect (CONFIG *p, int data);

/* 选择探头或者是自定义探头 */
extern int get_probe_select (CONFIG *p);
extern void		set_probe_select (CONFIG *p, int data);

/* 选择探头FFT开关 */
extern int get_probe_fft (CONFIG *p);
extern void		set_probe_fft (CONFIG *p, int data);

/* 选择聚焦法则自动计算开关 */
extern int get_auto_focal (CONFIG *p);
extern void		set_auto_focal (CONFIG *p, int data);

/* 选择当前encoder 1或者2 */
extern int get_cur_encoder (CONFIG *p);
extern void		set_cur_encoder (CONFIG *p, int data);
/* 设置encoder属性 */
extern unsigned char	get_enc_polarity (CONFIG *p, int enc_id);
extern void		set_enc_polarity (CONFIG *p, unsigned char data, int enc_id);
extern unsigned char	get_enc_type (CONFIG *p, int enc_id);
extern void		set_enc_type (CONFIG *p, unsigned char data, int enc_id);
extern unsigned int	get_enc_resolution (CONFIG *p, int enc_id);
extern void		set_enc_resolution (CONFIG *p, unsigned int data, int enc_id);
extern unsigned int	get_enc_origin (CONFIG *p, int enc_id);
extern void		set_enc_origin (CONFIG *p, unsigned int data, int enc_id);
/* 设置 scan inspection 属性 */
extern unsigned char	get_inspec_type (CONFIG *p);			
extern void		set_inspec_type (CONFIG *p, unsigned char data);
extern unsigned char	get_inspec_source (CONFIG *p);			
extern void		set_inspec_source (CONFIG *p, unsigned char data);
extern unsigned char	get_inspec_index (CONFIG *p);			
extern void		set_inspec_index (CONFIG *p, unsigned char data);
extern unsigned int	get_inspec_speed (CONFIG *p);
extern void		set_inspec_speed (CONFIG *p, unsigned int data);
extern unsigned int	get_inspec_rpmspeed (CONFIG *p);
extern void		set_inspec_rpmspeed (CONFIG *p, unsigned int data);
extern unsigned int	get_inspec_indexspeed (CONFIG *p);
extern void		set_inspec_indexspeed (CONFIG *p, unsigned int data);
/* 设置 scan area 属性 */
extern int		get_area_scanstart (CONFIG *p);
extern void		set_area_scanstart (CONFIG *p, int data);
extern int		get_area_scanend (CONFIG *p);
extern void		set_area_scanend (CONFIG *p, int data);
extern int		get_area_scanresolution (CONFIG *p);
extern void		set_area_scanresolution (CONFIG *p, int data);
extern int		get_area_indexstart (CONFIG *p);
extern void		set_area_indexstart (CONFIG *p, int data);
extern int		get_area_indexend (CONFIG *p);
extern void		set_area_indexend (CONFIG *p, int data);
extern int		get_area_indexresolution (CONFIG *p);
extern void		set_area_indexresolution (CONFIG *p, int data);
/* 设置 scan start */
extern unsigned char	get_start_mode (CONFIG *p);
extern void		set_start_mode (CONFIG *p, unsigned char data);
extern int get_start_pause (CONFIG *p);
extern void		set_start_pause (CONFIG *p, int data);
/* 设置 scan Data */
extern unsigned char	get_data_storage (CONFIG *p);
extern void		set_data_storage (CONFIG *p, unsigned char data);
extern int get_data_inspec_data (CONFIG *p);
extern void		set_data_inspec_data (CONFIG *p, int data);

/* 设置 file->file */
extern unsigned char	get_file_storage (CONFIG *p);
extern void		set_file_storage (CONFIG *p, unsigned char data);
extern unsigned char	get_file_save_mode (CONFIG *p);
extern void		set_file_save_mode (CONFIG *p, unsigned char data);

/* 设置 file->report */
extern int	get_report_template (CONFIG *p);
extern void		set_report_template (CONFIG *p, int data);
extern unsigned char	get_report_paper_size (CONFIG *p);
extern void		set_report_paper_size (CONFIG *p, unsigned char data);

/* 设置 file->format */
extern int	get_report_format_userfield (CONFIG *p);
extern void		set_report_format_userfield (CONFIG *p, int data);
extern int	get_report_format_probe (CONFIG *p);
extern void		set_report_format_probe (CONFIG *p, int data);
extern int	get_report_format_setup (CONFIG *p);
extern void		set_report_format_setup (CONFIG *p, int data);
extern int	get_report_format_note (CONFIG *p);
extern void		set_report_format_note (CONFIG *p, int data);
extern unsigned char	get_report_format_view (CONFIG *p);
extern void		set_report_format_view (CONFIG *p, unsigned char data);

/* 设置 file->userfield */
extern unsigned char	get_report_userfield_select (CONFIG *p);
extern void		set_report_userfield_select (CONFIG *p, unsigned char data);
extern int	get_report_userfield_enable (CONFIG *p, int select_pos);
extern void		set_report_userfield_enable (CONFIG *p, int data, int select_pos);
extern const char	*get_report_userfield_label (CONFIG *p, int select_pos);
extern void		set_report_userfield_label (CONFIG *p, const char *label, int select_pos);
extern const char	*get_report_userfield_content (CONFIG *p, int select_pos);
extern void		set_report_userfield_content (CONFIG *p, const char *content, int select_pos);

/* 保存配置信息 */
extern const char	*get_edit_notes_info (CONFIG *p);
extern void		set_edit_notes_info (CONFIG *p, const char *content);
extern const char	*get_header_info (CONFIG *p);
extern void		set_header_info (CONFIG *p, const char *content);
extern const char	*get_file_name_info (CONFIG *p);
extern void		set_file_name_info (CONFIG *p, const char *content);
/* gll probe_type */
extern unsigned char	get_probe_type (CONFIG *p);
extern void		set_probe_type (CONFIG *p, unsigned char data);
/* 配置信息的操作end */



/* Group 参数的保存读取 */
extern unsigned int get_group_wedge_delay (CONFIG *p, int group_id);
extern void set_group_wedge_delay (CONFIG *p, int group_id, unsigned int data);
extern unsigned int get_group_range (CONFIG *p, int group_id);
extern void set_group_range (CONFIG *p, int group_id, unsigned int data);
extern int get_group_start (CONFIG *p, int group_id);
extern void set_group_start (CONFIG *p, int group_id, int data);
extern unsigned short get_group_gain (CONFIG *p, int group_id);
extern void set_group_gain (CONFIG *p, int group_id, unsigned short data);
extern unsigned short get_group_gainr (CONFIG *p, int group_id);
extern void set_group_gainr (CONFIG *p, int group_id, unsigned short data);
extern unsigned int get_group_velocity (CONFIG *p, int group_id);
extern void set_group_velocity (CONFIG *p, int group_id, unsigned int data);
extern int get_group_db_ref (CONFIG *p, int group_id);
extern void set_group_db_ref (CONFIG *p, int group_id, int data);

#if 0
	/* 基本设置 */
	unsigned int	wedge_delay;	/* 楔款延时 单位 ns */
	unsigned int	range;			/* 显示范围 单位 ns */
	int	start;			/* 扫描延时 单位 ns */
	unsigned short	gain;			/* 实际增益 单位 0.01dB */
	unsigned short	gainr;			/* 参考增益 单位 0.01dB */
	unsigned int	velocity;		/* 实际声速 单位 0.01m/s  */
	unsigned char	db_ref;			/* 参考增益开关 0 off 1 on */
#endif


/* group操作 */
extern void		grpcpy (CONFIG *p, unsigned int src, unsigned int dst);

/***
#define	VAL_GRP_ID	0x0001
extern int	*get_cfg_val (CONFIG *p, int val_type, int grp);
***/

#endif
