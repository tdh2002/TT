
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * base.h
 * 
 * 谭登华
 * 2010-11-4
 *
 */

#ifndef __BASE_H_
#define __BASE_H_

#include "base_const.h"
#include "base_config.h"
#include "spi_d.h"
#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <fakekey/fakekey.h>
#include <X11/keysym.h>

#define X86_DEPTH	24
#define ARM_DEPTH	16

#if X86
#define TRUE_DEPTH	X86_DEPTH
#elif ARM
#define TRUE_DEPTH	ARM_DEPTH
#endif

#define	MAX_DOT_QTY		163840
#define LAW_MAX_QTY		256
#define ELEM_TX_MAX_QTY	32
#define ELEM_RX_MAX_QTY	32

#define COL_24_TO_16(a) ((((a)>>8)&0xf800)|(((a)>>5)&0x07e0)|(((a)>>3)&0x1f))

/* 回波信号8位还是16位 */
typedef	guchar	DOT_TYPE;

extern GdkColor	color_black;
extern GdkColor	color_black1;
extern GdkColor	color_white;
extern GdkColor	color_yellow;
extern GdkColor	color_green;
extern GdkColor	color_blue;
extern GdkColor	color_red;
extern GdkColor	color_text_base;
extern GdkColor color_rule;

#define	COLOR_BLUE		0
#define	COLOR_GREEN		1
#define	COLOR_RED		2	
#define	COLOR_YELLOW	3
#define	COLOR_BLACK		4
#define	COLOR_WHITE		5

typedef struct _calibration
{
	//ENCODER
	gint delt_distance;
	gint measure_start;
	gint measure_end;
	gint delt_measure;
	guint resolution;

	//Ultrasound->Velocity
	gint radius1;
	gint radius2;
	gint depth1;
	gint depth2;
	gint thickness1;
	gint thickness2;
	gint time_start;
	gint time_end;

	gchar velocity_last;
	//Ultrasound->Wedge Delay
	gchar data;//保存半径，深度，厚度的值
	gchar data_measure[20];//保存检测到的实际值
	gchar wd_radius[20];
	gchar wd_depth[20];
	gchar wd_thickness[20];
	gchar wd_delay[20];
	
} CALIBRATION,*CALIBRATION_P;

typedef struct tmp_config 
{
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


	guchar   min_angle_reg;		
	guchar   max_angle_reg;	
	guchar   positions_reg;	
	guchar   offsets_reg;
	guchar   positione_reg;	
	guchar	 positionstep_reg;
	guchar	 depths_reg;
	guchar	 depthe_reg;
	guchar   offsete_reg;		
	guchar   angle_step_reg;
	guchar   beam_skew_min_angle_reg;
	guchar   beam_skew_max_angle_reg;
	guchar   beam_skew_angle_step_reg;	
	guchar   focus_depth_reg;	/* Focal Law -> Beam -> focus_depth*/


	guchar	encoder_resolution_reg;	/* Scan -> Encoder -> resolution  */
	guchar	origin_reg;
	guchar	scanspeed_reg;
	guchar	scanspeed_rpm_reg;
	guchar	indexspeed_reg;

	guchar    scan_start_reg;	/* Scan -> Area -> Scan start*/
	guchar    scan_end_reg;		/* Scan -> Area -> Scan end*/
	guchar    scan_resolution_reg;	/* Scan -> Area -> Scan resolution*/

	guchar    index_start_reg;	/* Scan -> Area -> index_start*/
	guchar    index_end_reg;	/* Scan -> Area -> index_end*/
	guchar    index_resolution_reg;	/* Scan -> Area -> index_resolution*/

	guchar	bright_reg;		/*preferences -> pref. -> bright*/
	guchar	distance_reg;		/*preferences -> pref. -> bright*/

#if 0
	DOT_TYPE	envelope_max[setup_MAX_GROUP_QTY][640];	/**/
	DOT_TYPE	envelope_min[setup_MAX_GROUP_QTY][640];	/**/
	DOT_TYPE	a_scan_data[setup_MAX_GROUP_QTY][640];	/**/
	DOT_TYPE	s_scan_data[640*400];		/**/
#endif

	LAW_BEAM	focal_law_all_beam[setup_MAX_LAW_QTY];
	LAW_ELEM	focal_law_all_elem[setup_MAX_LAW_QTY][setup_MAX_ELEM_RX_ACTIVE];	

	DOT_TYPE	scan_data[setup_MAX_GROUP_QTY][640*400];
	guint		measure_data[setup_MAX_LAW_QTY][8];
	MEASURE_DATA	measure_data_dis;

	gushort	special_col_amp[3];		/* 特殊颜色 */
	gushort	color_amp[256];			/* 调色板信息 */

	gushort	special_col_tofd[3];	/* 特殊颜色 */
	gushort	color_tofd[256];		/* 调色板信息 */

	gushort	special_col_depth[3];	/* 特殊颜色 */
	gushort	color_depth[256];		/* 调色板信息 */

	guint	t_special_col[3];		/* 临时特殊颜色 */
	guint	t_color[256];			/* 临时调色板信息 */

	guint	max_prf;
	guint	virtual_rules;			/* 一次dma数据里面有多少个重复的数据 */
	guint	total_point_qty;		/* 当前一次dma 点的总数量 */
	gushort	*fb1_addr;
	gushort	*virtual_add;

	guint	beam_num[setup_MAX_GROUP_QTY];		/* Group 显示的beam编号 */
	guint	beam_qty[setup_MAX_GROUP_QTY];		/* Group beam总数量	*/
	guint	angle_num;						/* 几个angle */
	guint	beam_skew_num;					/* 几个beam_skew */

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
	gint	ctype[16];

	gchar	velocity_data[3][20];
	gchar	*velocity_data_p[3];
	
	guint	dma_data_add;

	gint	fd_config;

	gint	range_step_min;

	group_data_spi	group_spi[setup_MAX_GROUP_QTY];
	focal_data_spi	focal_spi[setup_MAX_LAW_QTY];

	guchar	radius1_reg;
	guchar	depth1_reg;
	guchar	radius2_reg;
	guchar	depth2_reg;
	guchar	thickness1_reg;
	guchar	thickness2_reg;
	guchar	cstart_reg;
	guchar	cwidth_reg;
	guchar	cheight_reg;
	guchar	radiusa_reg;
	guchar	deptha_reg;
	guchar	thickness_reg;
	guchar	tolerance_reg;
	//guchar	first_angle_reg;
	//guchar	last_angle_reg;
	guchar	ref_amplitude_reg;
	guchar	tolerance_t_reg;
	guchar	compdb_reg;
	guchar	cangle_reg;

	gchar	freeze;

    //GdkPixbuf *pixbuf_[18];
	CALIBRATION wizard_calibration;

} TMP_CONFIG, *TMP_CONFIG_P;

/*画图区域*/
typedef struct _draw_area 
{
	GtkWidget *vbox;						/*  */
	GtkWidget *ebox;						/* 存放标题 */
	GtkWidget *label;						/* 窗口名字 */
	GtkWidget *drawing_area;                /* 波形显示区 */
	guint	width;
	guint	height;
	guint	h1_bit;
	gfloat	hmin1;
	gfloat	hmin2;
	gint	h1_unit;
	gint	h1_color;
	guint	h2_bit;
	gfloat	hmax1;
	gfloat	hmax2;
	gint	h2_unit;
	gint	h2_color;
	guint	w_bit;
	gfloat	wmin1;
	gfloat	wmax1;
	gint	w_unit;
	gint	w_color;
	gint	scan_type;
	gint	group;
	gint	mask;
	gint	hrule1_copies;		/* 将vruler1分成多少份 */
	gint	hrule2_copies;		/* 将vruler2分成多少份 */
	gint	wrule_copies;		/* 将hruler分成多少份 */
	gchar	title[64];
} DRAW_AREA, *DRAW_AREA_P;

/*画界面结构体*/
typedef struct Draw_interface 
{
	guchar			pos_pos;       /* 指示当前位置信息 0 2级停留 1 2级按下 2 3级停留 3 3级按下 */
	guchar			pos;           /* 0,1,2,...,9  一级菜单位置*/
	guchar			pos1[10];           /* 0，1，2，3，4 二级菜单位置*/
	guchar			pos2[10][5];        /* 0，1，2，3，4，5 三级菜单位置*/
	guchar			pos_last;           /* 一级菜单位置上一位置 */
	guchar			pos_last1;          /* 二级菜单位置上一位置  */
	guchar			pos_last2;          /* 三级菜单位置上一位置  */
	guchar			menu2_qty;          /*  */

	gchar			*file_path;	/* 帮助文档的路径 */
	WebKitWebView	*web_view;
	WebKitWebView	*web_view_tmp;
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
	//GtkWidget		*entry;
	GtkWidget		*label3[6];         /* data 是lable   */
	GtkWidget		*data3[6];			/* */
	GtkWidget		*eventbox32[6];                 /* */
	GtkWidget		*sbutton[6];
	/* waiting for delete*/
	GtkWidget		*menu3;
	GtkWidget		*menu33[6];
	GtkWidget		*menu_bar3;
	GtkWidget		*root_menu3;
	GtkWidget		*menu_item3[64];
	GtkWidget		*menu_dialog[20];

	GtkWidget		*window;
	GtkWidget		*win_keyboard;
	Display			*disp;
	FakeKey			*fk;


	GtkWidget		*dialog;
	/* scale 快速调节数值 */
	GtkWidget		*button_add;
	GtkWidget		*button_sub;
	GtkWidget		*vscalebox;
	GtkWidget		*scale_drawarea;
	GtkWidget		*vscale;

	/* 画图区域 */
	const gchar **con0_p;
	const gchar ***con1_p;
	const gchar ****con2_p;
	const gchar **units;
	const gchar **menu_content;

	const gchar **list;
	const gchar **list1;
	const gchar **field1;
	const gchar **field;
	const gchar **field_unit;

	CONFIG		*p_config;			/**/
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
	guchar			mark_pop_change;      /*  */
	guchar			markreturn;
	guchar			mark2;
	guchar			mark3;

	guchar	bscan_mark;
	guchar	cscan_mark;
	guchar	ccscan_mark;
	guchar	cccscan_mark;
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
	guchar  menu3_start;	/* 正数第一个可以选的三级菜单的位置*/
	guchar  menu3_end;	/* 倒数第一个可以选的三级菜单的位置*/

	gpointer probe_dialog;

	guint	dialog_pos;		/* 弹出dialog的类型 */
	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *sw11;
	GtkWidget *sw1;		/* 第二个scroll 探头 楔块 聚焦法则 setup 等需要2个sw */
	int		fd_key;		/* 串口设备 */
	int		fd_key1;		/* 串口设备 */

	guchar  iter_qty;       /* treeview 中 iter 的个数 */
	guchar  iter_pos;	/* treeview 中 iter 的位置 */

	guchar	start_qty;  /* wizard 中 group -> start 被点击的次数 */
	guchar	operation_pos;
	guchar	wgroup_pos;
	guchar	fstart_qty;  /* wizard 中 focal law -> start 被点击的次数 */
	guchar	cstart_qty;
	guchar  ctype_pos;
	guchar	cmode_pos;
	guchar	scode_pos;
	guchar	echotype_pos;
	gint	radius1;
	gint	radius2;
	gint	depth1;
	gint	depth2;
	gint	thickness1;
	gint	thickness2;
	gint	radiusa;
	gint	deptha;
	gint	thickness;
	gint	tolerance;
	gshort	first_angle;
	gshort	last_angle;
	gushort	ref_amplitude;
	guint	tolerance_t;
	gushort	comp_gain;
	guchar	weldtype_pos;
	guchar	cangle;

	gint	distance;

	gfloat	swmin;
	gfloat	swmax;
} DRAW_UI, *DRAW_UI_P;

typedef struct _MY_SIGNAL_TMP 
{
	GtkTreeModel *source_model;
	GtkTreeSelection *source_selection;
	GtkWidget *source_list;

	GtkTreeModel *target_model;
	GtkTreeSelection *target_selection;
	GtkWidget *target_list;
} MY_SIGNAL,*MY_SIGNAL_P;

typedef struct __my_ip_get 
{
    unsigned char system_b1;
    unsigned char system_b2;
    unsigned char system_b3;
    unsigned char system_b4;
}_my_ip_get,*_my_ip_get_p;

typedef struct __my_ip_set
{
    GtkWidget *entry[4];
}_my_ip_set,*_my_ip_set_p;

//typedef __my_ip_get __my_mask_get;
typedef _my_ip_get _my_mask_get;
typedef _my_ip_get_p _my_mask_get_p;

//typedef __my_ip_set __my_mask_set;
typedef _my_ip_set _my_mask_set;
typedef _my_ip_set_p _my_mask_set_p;

typedef struct __my_time_set
{
    GtkWidget *entry[3];
}_my_time_set,*_my_time_set_p;

typedef struct __my_time_get 
{
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
}_my_time_get,* _my_time_get_p;

typedef struct __my_date_set
{
    GtkWidget *entry[3];
}_my_date_set,* _my_date_set_p;

typedef struct __my_date_get
{
    unsigned int year;
    unsigned int mon;
    unsigned int mday;
}_my_date_get,* _my_date_get_p;

typedef struct __year_mon_day_change
{
    GtkAdjustment *year;
    GtkAdjustment *mon;
    GtkAdjustment *day;
}_year_mon_day_change,* _year_mon_day_change_p;

typedef struct __save_file_name_struct
{
    GtkWidget *list;
	GtkListStore *store;
    GtkWidget *file_name;
}_save_file_name_struct,* _save_file_name_struct_p;

typedef struct __group_wizard
{
	enum 
	{
		add = 1,
		modify
	}operation;

}_group_wizard, *_group_wizard_p;

typedef struct __dir_path_label
{
	char *file_type;
    GtkWidget *source_label;
	GtkWidget *target_label;
}_dir_path_label, *_dir_path_label_p;

typedef struct __load_menu3_7
{
    char *tmp_char[7];
	GtkWidget *tmp_label[7];
	GtkWidget *tmp_hbox[7];
	GtkWidget *tmp_entry[7];
	int tmp_pos;
	int *value;
}_load_menu3_7, *_load_menu3_7_p;

typedef struct __DEFINE_PROBE
{
	int *pos_tmp;
	GtkWidget *entry_tmp;
	PROBE_P probe_p_tmp;
} _DEFINE_PROBE, *_DEFINE_PROBE_P;

#define CUR_POS (pp->pos2[pp->pos][pp->pos1[pp->pos]])      /* 0,1,2,3,4,5 */
#define CUR_POS_P(p) (p->pos2[p->pos][p->pos1[p->pos]])      /* 0,1,2,3,4,5 */

#define CCFG(a)	(pp->p_config->a)

#define GROUP_VAL_POS(a, b)	(CCFG(group[a].b))				/* a表示哪个group b是返回哪个值 */
#define GROUP_VAL(a)  (CCFG(group[CCFG(groupId)].a))

#define LAW_VAL_POS(a, b)  (GROUP_VAL_POS(a,law_info).b)
#define LAW_VAL(a)  (GROUP_VAL(law_info).a)
//#define BEAM_INFO(a,b)  (CFG(focal_law_all_beam[a].b))         /* a表示第几个波束， b表示当前波束下的某一个特性的值 */
#define BEAM_INFO(a,b)  (TMP(focal_law_all_beam[a].b))         /* a表示第几个波束， b表示当前波束下的某一个特性的值 */

#define GROUP_GATE_POS(a)   GROUP_VAL(gate[GROUP_VAL(gate_pos)].a)

#define GROUP_CURSORS_POS(a) GROUP_VAL(cursors_info[GROUP_VAL(selection)-5].a)

#define TMP(a)  (pp->p_tmp_config->a)
#define TMP_CBA(a) (TMP(wizard_calibration).a)
#define	MENU_STATUS	(pp->pos_pos) 

#endif

