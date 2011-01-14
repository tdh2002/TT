
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

typedef struct focal_law 
{
	guchar	active_elem;
	guchar	t;	
}FOCAL_LAW, *FOCAL_LAW_P;;

typedef struct element_law
{
	guchar	e_number;

}ELEMENT_LAW, *ELEMENT_LAW_P;

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
	guint	Geometry;		/* 几何形状 FLAT/ID/OD/BALL */
	guint	Thickness;		/* 厚度 */
	guint	Diameter;		/* 直径 */
	guint	Velocity_LW;		/* 声速 单位 0.01m/s  */
	guint	Velocity_SW;		/* 声速 单位 0.01m/s  */
	guint	Material;		/* 材料 */
} PART, *PART_P;

/*材料 (Material)*/
typedef struct Material {
	guint	Velocity;		/* 声速 */
	guint	Name[20];		/* 材料名字 */
} MATERIAL, *MATERIAL_P;

/*配置信息 (CONFIG)*/
typedef	struct Config {
	guint	group;			/**/
	/*基本参数*/
	PROBE	probe[2];
	WEDGE	wedge[2];
	PART	part;
	gushort	gain;			/* 实际增益 单位 0.01dB */
	gushort	gainr;			/* 参考增益 单位 0.01dB */
	gint	start;			/* 扫描延时 单位 0.001μs */
	guint	range;			/* 显示范围 单位 0.001μs */
	guint	wedge_delay;	/* 楔款延时 单位 0.001μs */

	/*发射*/
	guchar	db_ref;			/* 参考增益开关 0 off 1 on */
	guchar	pulser;			/*  */
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
	guint   auto_detect;                    /*Probe/Part -> select -> auto_detect*/
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
#define TMP(a)  (pp->p_tmp_config->a)

#define VERSION "DP1.0.0.0"

#endif
