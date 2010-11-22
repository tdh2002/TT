
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
 */

#ifndef __BASE_H_
#define __BASE_H_

#include <gtk/gtk.h>

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x

extern GdkColor	color_black;
extern GdkColor	color_button0;
extern GdkColor	color_button1;

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
	guint	Volecity;		/*声速*/
	guint	Material;		/*材料*/
} PART, *PART_P;

/*材料 (Material)*/
typedef struct Material {
	guint	Volecity;		/*声速*/
	guint	Name[20];		/*材料名字*/
} MATERIAL, *MATERIAL_P;

/*配置信息 (CONFIG)*/
typedef	struct Config {
	guint	group;			/**/
	/*基本参数*/
	PROBE	probe;
	WEDGE	wedge;
	PART	part;
	gint	gain;			/*增益 0.1dB 单位*/
	guint	range;			/*显示范围 以0.01mm为单位*/
	gint	start;			/*扫描延时 以0.01mm为单位*/
	gint	wedge_delay;	/*楔款延时 以0.01mm为单位*/
	
	/*发射*/
	gint	pulser;
	gint	tx_rxmode;
	gint	frequence;
	gint	voltage;
	gint	pw;
	gint	prf;

	/*接收*/
	gint	receiver;
	gint	filter;
	gint	rectifier;
	gint	video_filter;
	gint	averaging;
	gint	recject;

	/*波束 beam*/
	gint	scan_offset;
	gint	index_offset;
	gint	angle;
	guint	skew;
	gint	beam_delay;
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

	guchar	ut_unit;		/*单位 mm(0) or inch(1) or ..*/
	guint	color;			/**/
	

	/*选项*/
	guint	unit;
	guint	bright;

	guint	date;			/**/
	guint	time;			/**/


} CONFIG, *CONFIG_P;

typedef struct tmp_config {
	guchar	db_reg;			/*增益步进*/
	guchar	start_reg;		/*start ? 扫描延时步进*/

	gint	fd_config;

} TMP_CONFIG, *TMP_CONFIG_P;

/*测量数据 (MEASURE_DATA)*/
typedef struct Measure_data {



} MEASURE_DATA, *MEASURE_DATA_P;



/*画界面结构体*/
typedef struct Draw_interface {
	guint			pos;
	guint			pos_qty;
	guint			pos1[10];
	guint			pos2[10][5];
	GdkColor		col;
	GtkAdjustment	*adj;
	GSList			*group;

	/*boxes*/
	GtkWidget		*vbox;			/*整个window*/
	GtkWidget		*hbox1;			/**/
	GtkWidget		*vbox11;
	GtkWidget		*hbox111;
	GtkWidget		*hbox1111[3];
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
	GtkTextBuffer	*buffer[20];	/*测量信息等的显示*/
	GtkWidget		*view;			/*把buffer放在这个里面*/

	GtkWidget		*menubar;			/* 一级菜单 */
	GtkWidget		*menu;				/* 一级菜单 */
	GtkWidget		*menuitem_main;		/* 一级菜单 */

	GtkWidget		*menuitem[10];		/* 一级菜单选项*/

	GtkWidget		*button2[6];		/* 二级菜单*/

	GtkWidget		*button3[6];		/* 三级菜单*/
	GtkWidget		*data[6];			/* 三级菜单内容*/

	gint			xx;
	GtkWidget		*window;
	CONFIG_P		p_config;			/**/
	TMP_CONFIG_P	p_tmp_config;		/**/

} DRAW_UI, *DRAW_UI_P;


#endif
