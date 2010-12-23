
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

#include <gtk/gtk.h>

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x

extern GdkColor	color_black;
extern GdkColor	color_white;
extern GdkColor	color_yellow;
extern GdkColor	color_button0;
extern GdkColor	color_button1;
extern GdkColor	color_button2;

/*探头(Probe)*/
typedef struct Probe {
	guint	Elem_qty;		/*阵元数*/
	guint	Frequency;		/*频率 0.1Mhz 为单位*/
	guint	Pitch;			/*阵元中心间距 0.1mm 为单位*/
	gchar	Name[20];		/*探头名字*/
} PROBE, *PROBE_P;

/*楔块 (Wedge)*/
typedef struct Wedge {
	guint	Angle;			/*角度*/
	guint	Height;			/*第一阵元高度*/
	gint	Primary_offset;	/*前沿 */
	guint	Volecity;		/*声速*/
	gchar	Name[20];		/*楔块名字*/
} WEDGE, *WEDGE_P;

/*工件 (Part)*/
typedef struct Part {
	guint	Geometry;		/*集合形状 FLAT/ID/OD*/
	guint	Thickness;		/*厚度*/
	guint	Diameter;		/*直径*/
	gfloat	Velocity;		/*声速 m/s */
	guint	Material;		/*材料*/
} PART, *PART_P;

/*材料 (Material)*/
typedef struct Material {
	guint	Velocity;		/*声速 */
	guint	Name[20];		/*材料名字*/
} MATERIAL, *MATERIAL_P;

/*配置信息 (CONFIG)*/
typedef	struct Config {
	guint	group;			/**/
	/*基本参数*/
	PROBE	probe;
	WEDGE	wedge;
	PART	part;
	gfloat	gain;			/* 增益 dB 单位 */
	gfloat	start;			/* 扫描延时 以 μs 为单位 */
	gfloat	range;			/* 显示范围 以 μs 为单位 */
	gfloat	wedge_delay;	/* 楔款延时 以 μs 为单位 */
	
	/*发射*/
	guchar	pulser;			/*  */
	guchar	tx_rxmode;		/*  */
	gushort	frequence;		/*  */
	guchar	voltage;		/*  */
	gushort	pulser_width;	/*  */
	guint	prf;			/*  */

	/*接收*/
	gint	receiver;		/*  */
	gint	filter;			/*  */
	gint	rectifier;		/*  */
	guchar	video_filter;	/*  */
	gint	averaging;		/*  */
	guchar	reject;		    /*  */

	/*波束 beam*/
	gint	scan_offset;		/**/
	gint	index_offset;		/**/
	gint	angle;				/**/
	gint	skew;				/**/
	guint	beam_delay;			/**/
	gint	gain_offset;

	/*高级*/
	guint	auto_height;	/**/
	gint	db_ref;			/**/
	guint	point_qty;		/**/
	gint	sum_gain;		/**/

	/*闸门报警 */
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

	gint	groupA;
	gint	groupB;
	gint	condition;

	guint	alarm_sound;

	/*测量*/
	guint	measure1;	
	guint	measure2;	
	guint	measure3;	
	guint	measure4;	

	/*显示*/
	guint	display;		/*显示模式 A B C A+B A+B+C A+S ...*/
	guint	c_scan;			/*c扫描参考*/

	guchar	ut_unit;		/*检测单位 时间2 声程1  实际深度0 .*/
	guint	color;			/**/
	

	/*选项*/
	guchar	unit;			/* 0 mm 1 inch */
	guint	bright;

	guint	date;			/*  */
	guint	time;			/*  */

	guint	count;                          /*Gate/Alarm->Output->count*/
	guint	active_delay;                   /*Gate/Alarm->Output->delay*/
	guint	holdtime;                       /*Gate/Alarm->Output->holdtime*/

	guint	VPA;                            /*Measurements->Cursors->VPA*/
	guint	cursors_scan;                   /*Measurements->Cursors->Scan*/
	guint	cursors_index;                  /*Measurements->Cursors->index*/
	guint	min_thickness;                  /*Measurements->Thickness->min*/
	guint	max_thickness;                  /*Measurements->Thickness->max*/
	guint	echo_qty;                       /*Measurements->Thickness->echo_qty*/

        guint   color_start;                    /*Display -> Color -> Start*/
        guint   color_end;                      /*Display -> Color -> end*/
        guint   sizing_curves;                  /*Display -> overlay -> sizing curves*/
        guint   overlay_gate;                   /*Display -> overlay -> gate*/
        guint   overlay_cursor;                 /*Display -> overlay -> cursor*/
        guint   overlay_overlay;                /*Display -> overlay -> overlay*/

        guint   part_thickness;                 /*Probe/Part -> Parts -> thickness*/
        guint   auto_detect;                    /*Probe/Part -> select -> auto_detect*/

        guint   connection_P;                   /*Focal_Law -> configuration -> connection P*/
        guint   element_qty;                    /*Focal_Law -> aperture -> element qty*/
        guint   first_element;                  /*Focal_Law -> aperture -> first element*/
        guint   last_element;                   /*Focal_Law -> aperture -> last element*/
        guint   element_step;                   /*Focal_Law -> aperture -> element step*/
        guint   min_angle;                      /*Focal Law -> Beam -> Min_angle*/
        guint   focus_depth;                    /*Focal Law -> Beam -> focus_depth*/
        guint   scan_speed;                     /* Scan -> Inspection -> Scan speed*/

        guint    scan_start;                    /* Scan -> Area -> Scan start*/
        guint    scan_end;                      /* Scan -> Area -> Scan end*/
        guint    scan_resolution;               /* Scan -> Area -> Scan resolution*/
        guint    index_start;                   /* Scan -> Area -> index_start*/
        guint    index_end;                     /* Scan -> Area -> index_end*/
        guint    index_resolution;              /* Scan -> Area -> index_resolution*/

        guint    format_userfield;              /* File -> Format -> UserField*/
        guint    format_probe;                  /* File -> Format -> probe*/
        guint    format_setup;                  /* File -> Format -> setup*/
        guint    format_note;                   /* File -> Format -> note*/





} CONFIG, *CONFIG_P;

typedef struct tmp_config {
	guchar	db_reg;				/* 增益步进 */
	guchar	start_reg;			/* start ? 扫描延时步进 */
	guchar	range_reg;			/* 范围(range) 步进 */
	guchar	wedge_delay_reg;	/* 楔块延时(wedge_delay) 步进 */
	guchar	velocity_reg;		/* 声速(velocity) 步进 */

	guchar	pulser_reg;			/* 脉冲发射pulser  步进 */
	guchar	receiver_reg;		/* 接收器receiver 步进 */
	guchar	reject_reg;			/* 抑制reject  步进 */
	guchar	scanoffset_reg;		/* scan offset  步进 */
	guchar	indexoffset_reg;	/* index offset  步进 */
	guchar	gainoffset_reg;		/* gain offset  步进 */

	guchar beam_delay_reg;		/*beam delay 步进*/

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
	guchar  echo_qty_reg;                /*echo_qty步进*/

        guchar   color_start_reg;                   /*Display -> Color -> Start步进*/
        guchar   color_end_reg;                   /*Display -> Color -> end步进*/
        guchar   part_thickness_reg;                   /*Probe/Part -> parts -> thickness步进*/
        guchar   diameter_reg;                   /*Probe/Part -> parts -> diameter步进*/

        guchar   connection_P_reg;                  /*Focal_Law -> configuration -> connection P*/

        guchar   element_qty_reg;                  /*Focal_Law -> aperture -> element qty*/
        guchar   first_element_reg;                  /*Focal_Law -> aperture -> first element*/
        guchar   last_element_reg;                  /*Focal_Law -> aperture -> last_element*/
        guchar   element_step_reg;                  /*Focal_Law -> aperture -> element_step*/


        guchar   min_angle_reg;                      /*Focal Law -> Beam -> Min_angle*/
        guchar   max_angle_reg;                      /*Focal Law -> Beam -> Max_angle*/
        guchar   focus_depth_reg;                    /*Focal Law -> Beam -> focus_depth*/
        guchar   angle_step_reg;                     /*Focal_Law -> Beam -> angle_step*/

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



/*画界面结构体*/
typedef struct Draw_interface {
	guchar			pos_pos;       /* 指示当前位置信息 0 2级停留 1 2级按下 2 3级停留 3 3级按下 */
	guchar			pos;           /**/
	guchar			pos1[10];
	guchar			pos2[10][5];
	guchar			pos_last;      /*  */
	guchar			pos_last1;     /*  */
	guchar			pos_last2;     /*  */
	guchar			menu2_qty;     /*  */
	GdkColor		col;
	GtkAdjustment	*adj;
	GSList			*group;

	/*boxes*/
	GtkWidget		*vbox;			/* 整个window */
	GtkWidget		*hbox1;			/**/
	GtkWidget		*vbox11;
	GtkWidget		*hbox111;
	GtkWidget		*vbox1111[3];
	GtkWidget		*hbox112;

	
	GtkWidget		*hbox2;
	GtkWidget		*vbox21;		
	GtkWidget		*hbox211;		/*扫描数据显示*/
	GtkWidget		*vbox2111;
	GtkWidget		*hbox2112;	

	GtkWidget		*hbox212;		/*一级菜单 二级菜单显示*/

	GtkWidget		*vbox22;		/*三级菜单*/
	GtkWidget		*vbox221[6];


	/*menubar menu menuitem_main 可以不用放在这里*/
	GtkWidget	*event[20];	/*测量信息等的显示*/
	GtkWidget	*label[20];	/*测量信息等的显示*/
	GtkWidget		*view;			/*把buffer放在这个里面*/

	GtkWidget		*menubar;			/* 一级菜单 */
	GtkWidget		*menu;				/* 一级菜单 */
	GtkWidget		*menuitem_main;		/* 一级菜单 */

	GtkWidget		*menuitem[10];		/* 一级菜单选项*/

	GtkWidget		*eventbox2[5];      /* 二级菜单 一个 eventbox + 一个label */
	GtkWidget		*label2[5];         /*  */

	GtkWidget		*eventbox30[6];     /* */
	GtkWidget		*eventbox31[6];     /* 三级菜单 二个 eventbox + 一个lable + 一个data + 一个 spinbutton data和spinbutton同时只能有一个显示*/
	GtkWidget		*label3[6];         /* data 是lable   */
	GtkWidget		*data3[6];			/* */
	GtkWidget		*eventbox32[6];     /* */
//	GtkWidget		*entry3[6];
	GtkWidget		*sbutton[6];
	GtkWidget		*button;
	GtkWidget		*vscalebox;
	GtkWidget		*bigscalebox;
	GtkWidget		*vscale;

	GtkWidget		*menu3;
	GtkWidget		*menu_bar3;
	GtkWidget		*root_menu3;
	GtkWidget		*menu_item3[30];

	gint			xx;
	GtkWidget		*window;
	GtkWidget		*spinbutton;
	GtkWidget		*window1;
	GtkWidget		*window2;
	CONFIG_P		p_config;			/**/
	TMP_CONFIG_P	p_tmp_config;		/**/

	guint			mark3;


        GtkWidget                *label4[2];       /*   + - 标签   */
        GtkWidget                *labelbox[2];     /*   + - 标签box   */


} DRAW_UI, *DRAW_UI_P;


#define VOL_LOW		0
#define VOL_HIGH	1

#define MENU1_STOP    4
#define MENU2_STOP    0 
#define MENU2_PRESSED 1
#define MENU3_STOP    2
#define MENU3_PRESSED 3

#define CUR_POS (pp->pos2[pp->pos][pp->pos1[pp->pos]])

#define UNIT_MM		0
#define UNIT_INCH	1
#define UNIT_US		2
#define UNIT_NONE	3
#define UNIT_M_S	4
#define UNIT_IN_US	5
#define UNIT_DB 	6
#define UNIT_TO 	7
#define UNIT_BFH 	8
#define UNIT_MS 	9
#define UNIT_MM_S 	10
#define UNIT_TO1 	11

#endif
