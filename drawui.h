
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#ifndef	__DRAW_UI_H_
#define	__DRAW_UI_H_
#include "base.h"
#include "language.h"

#define MENU1_STOP    4
#define MENU2_STOP    0 
#define MENU2_PRESSED 1
#define MENU3_STOP    2
#define MENU3_PRESSED 3
/* 单位 */
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

/* UT_UNIT */
#define UT_UNIT_TRUE_DEPTH	0
#define UT_UNIT_SOUNDPATH	1
#define UT_UNIT_TIME		2

/* 发射模式 */
#define PITCH_CATCH	0
#define	PULSE_ECHO	1
#define	THROUGH_TRAN	2

/* focal law 类型 定义*/
#define LINEAR_SCAN	0
#define	ANGLE_SCAN	1

/* 各种数值定义 */
#define A_SCAN	     0
#define B_SCAN	     1
#define C_SCAN	     2
#define S_SCAN	     3
#define A_B_SCAN     4
#define A_B_C_SCAN	 5
#define A_B_S_SCAN	 6
#define A_C_CC_SCAN	 7
#define A_S_CC_SCAN	 8
#define PA_TOFD	         9
#define Strip_Chart_AA	 10
#define GAINR_OFF	0
#define GAINR_ON	1

#define VOL_LOW		0
#define VOL_HIGH	1

/*上方数值显示信息定义 */
#define GAIN_LABEL	0
#define GAIN_VALUE	1


/* 最大测量范围  扫描延时+扫描范围 */
#define MAX_RANGE_US	9899.89


/* 各个menu 弹出菜单的偏移量 */
#define TX_RX_MODE	0
#define FREQUENCE	(TX_RX_MODE + 8)
#define VOLTAGE		(FREQUENCE + 14)



/* 发送给硬件数据类型 */
#define BEAM_DELAY_DSP	0x1000	/* 0x1000~0x10ff 最多256个聚焦法则 */
#define	RANGE_DSP		0x2000	/*  */	

#define ELEM_T_START 
#define ELEM_R_START 

#define GATEA_DSP		0x3000
#define GATEB_DSP		0x3001
#define GATEI_DSP		0x3002

extern gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
extern gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);
extern GtkWidgetClass *widget_window_class;

extern DRAW_UI_P pp;					

extern guint address[];
extern gfloat get_freq ();
extern void tt_label_show_string (GtkWidget *label, const gchar *s1, const gchar *s2,
		const gchar *s3, const gchar *color,  guint font_size);
extern void tt_label_show_float (GtkWidget *label, const gchar *s1, gfloat value, guint digit, 
		const gchar *s3, const gchar *color,  guint font_size);

extern void change_language();										/* 选择语言 */
extern void draw_2_menu(gint pa);									/* 更新二级菜单 */
extern void draw_3_menu(gint pa, gpointer p);					    /* 更新三级菜单 */
extern void init_ui(DRAW_UI_P p);									/* 初始化界面, */
extern void draw_area_all();

extern gboolean (*data_fun[6])(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
extern gboolean (*eventbox2_fun[5])(GtkWidget *widget, GdkEventButton *event, gpointer data);

extern void data_022 (GtkMenuItem *menuitem, gpointer data);
extern void data_023 (GtkMenuItem *menuitem, gpointer data);
extern void data_100 (GtkSpinButton *spinbutton, gpointer data);	/* 100 gain增益 */
extern void data_101 (GtkSpinButton *spinbutton, gpointer data);	/* 101 start 扫描延时 */
extern void data_102 (GtkSpinButton *spinbutton, gpointer data);	/* 102 range 范围 */
extern void data_103 (GtkSpinButton *spinbutton, gpointer data);	/* 103 wedget delay 楔款延时 */
extern void data_104 (GtkSpinButton *spinbutton, gpointer data);	/* 104 velocity 声速 */

extern void data_110 (GtkSpinButton *spinbutton, gpointer data);	/* 110 pulser 脉冲发射器 */
extern void data_111 (GtkMenuItem *menuitem, gpointer data);            /* 111 Tx/Rx Mode 收发模式 */
extern void data_1121 (GtkSpinButton *spinbutton, gpointer data);       /* 112 freq 频率 */
extern void data_112 (GtkMenuItem *menuitem, gpointer data);            /* 112 Freq 频率 */
extern void data_113 (GtkMenuItem *menuitem, gpointer data);            /* 113 Voltage */
extern void data_1141 (GtkSpinButton *spinbutton, gpointer data);       /* 114 PW 脉宽*/
extern void data_114 (GtkMenuItem *menuitem, gpointer data);            /* 114 PW 脉宽 */
extern void data_1151 (GtkSpinButton *spinbutton, gpointer data);       /* 115 PRF  重复频率*/
extern void data_115 (GtkMenuItem *menuitem, gpointer data);            /* 115 PRF  重复频率 */

extern void data_121 (GtkMenuItem *menuitem, gpointer data);            /* 121 Filter */
extern void data_122 (GtkMenuItem *menuitem, gpointer data);            /* 122 Rectifier */
extern void data_124 (GtkMenuItem *menuitem, gpointer data);            /* 124 Averaging */
extern void data_125 (GtkSpinButton *spinbutton, gpointer data);	/* 125 */

extern void data_130 (GtkSpinButton *spinbutton, gpointer data);	/* 130 */
extern void data_131 (GtkSpinButton *spinbutton, gpointer data);	/* 131 */
extern void data_132 (GtkSpinButton *spinbutton, gpointer data);	/* 132 */
extern void data_134 (GtkSpinButton *spinbutton, gpointer data);	/* 134 */
extern void data_135 (GtkSpinButton *spinbutton, gpointer data);	/* 135 */

extern void data_1431(GtkSpinButton *spinbutton, gpointer data);        /* 143 Points Qty */
extern void data_143 (GtkMenuItem *menuitem, gpointer data);            /* 143 Points Qty */
extern void data_1451(GtkSpinButton *spinbutton, gpointer data);        /* 145 Sum Gain */
extern void data_145 (GtkMenuItem *menuitem, gpointer data);            /* 145 Sum Gain */

extern void data_200 (GtkMenuItem *menuitem, gpointer data);        /*  200 Gate */
extern void data_201 (GtkMenuItem *menuitem, gpointer data);        /*  201 parameter */
extern void data_202 (GtkSpinButton *spinbutton, gpointer data);	/* 202 */
extern void data_2021 (GtkMenuItem *menuitem, gpointer data);         /*  2021 synchro */
extern void data_203 (GtkSpinButton *spinbutton, gpointer data);	/* 203 */
extern void data_2031 (GtkMenuItem *menuitem, gpointer data);         /*  2031 measure */
extern void data_204 (GtkSpinButton *spinbutton, gpointer data);	/* 204 */
extern void data_2041 (GtkMenuItem *menuitem, gpointer data);         /*  2041 rf */

extern void data_210 (GtkMenuItem *menuitem, gpointer data);         /*  210 Alarm */
extern void data_211 (GtkMenuItem *menuitem, gpointer data);         /*  211 Group A */
extern void data_212 (GtkMenuItem *menuitem, gpointer data);         /*  212 condition A */
extern void data_213 (GtkMenuItem *menuitem, gpointer data);         /*  213 Operator */
extern void data_214 (GtkMenuItem *menuitem, gpointer data);         /*  214 Group B */
extern void data_215 (GtkMenuItem *menuitem, gpointer data);         /*  215 condition B */

extern void data_220 (GtkMenuItem *menuitem, gpointer data);          /* 220 Output */
extern void data_221 (GtkMenuItem *menuitem, gpointer data);         /* Output -> alarm # */
extern void data_222 (GtkSpinButton *spinbutton, gpointer data);	/*222*/
extern void data_223 (GtkMenuItem *menuitem, gpointer data);          /* 223 sound */
extern void data_224 (GtkSpinButton *spinbutton, gpointer data);	/*224*/
extern void data_225 (GtkSpinButton *spinbutton, gpointer data);	/*225*/
extern void data_230 (GtkMenuItem *menuitem, gpointer data);         /* Gate/Alarm -> Sizing Curves -> Mode 230 */
extern void data_231 (GtkMenuItem *menuitem, gpointer data);         /* Gate/Alarm -> Sizing Curves -> Curve 231 */

extern void data_300 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> list 300 */
extern void data_302 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field 302 */
extern void data_303 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field2 303 */
extern void data_304 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field3 304 */
extern void data_305 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field4 305 */

extern void data_310 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> cursors -> selection 310 */
extern void data_311 (GtkSpinButton *spinbutton, gpointer data);	/*311*/
extern void data_313 (GtkSpinButton *spinbutton, gpointer data);	/*313*/
extern void data_324 (GtkSpinButton *spinbutton, gpointer data);        /*324*/
extern void data_330 (GtkMenuItem *menuitem, gpointer data);          /* Measurements -> Thickness -> source 330 */
extern void data_331 (GtkSpinButton *spinbutton, gpointer data);	/*331*/
extern void data_332 (GtkSpinButton *spinbutton, gpointer data);	/*332*/
extern void data_333 (GtkSpinButton *spinbutton, gpointer data);	/*333*/

extern void data_400 (GtkMenuItem *menuitem, gpointer data);            /*400*/
extern void data_410 (GtkMenuItem *menuitem, gpointer data);            /*410*/
extern void data_411 (GtkMenuItem *menuitem, gpointer data);            /*411*/
extern void data_420 (GtkMenuItem *menuitem, gpointer data);            /*420*/
extern void data_430 (GtkMenuItem *menuitem, gpointer data);            /*430*/
extern void data_431 (GtkSpinButton *spinbutton, gpointer data);	/*431*/
extern void data_4311 (GtkSpinButton *spinbutton, gpointer data);       /*4311*/
extern void data_4312 (GtkSpinButton *spinbutton, gpointer data);       /*4311*/
extern void data_432 (GtkSpinButton *spinbutton, gpointer data);	/*432*/
extern void data_4321 (GtkSpinButton *spinbutton, gpointer data);	/*432*/
extern void data_4322 (GtkSpinButton *spinbutton, gpointer data);	/*432*/
extern void data_434 (GtkMenuItem *menuitem, gpointer data);            /*434*/
extern void data_440 (GtkMenuItem *menuitem, gpointer data);            /*440*/
extern void data_441 (GtkMenuItem *menuitem, gpointer data);            /*441*/
extern void data_4411 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4414 (GtkMenuItem *menuitem, gpointer data);
extern void data_4415 (GtkMenuItem *menuitem, gpointer data);
extern void data_442 (GtkMenuItem *menuitem, gpointer data);            /*442*/
extern void data_443 (GtkMenuItem *menuitem, gpointer data);            /*443*/
extern void data_444 (GtkMenuItem *menuitem, gpointer data);            /*444*/
extern void data_445 (GtkMenuItem *menuitem, gpointer data);            /*445*/

extern void data_501 (GtkMenuItem *menuitem, gpointer data);
extern void data_502 (GtkMenuItem *menuitem, gpointer data);
extern void data_510 (GtkSpinButton *spinbutton, gpointer data);	/*510*/
extern void data_511 (GtkSpinButton *spinbutton, gpointer data);	/*511*/
extern void data_5121(GtkSpinButton *spinbutton, gpointer data);        /* 512 Skew (deg) */
extern void data_512 (GtkMenuItem *menuitem, gpointer data);            /* 512 Skew (deg) */

extern void data_521 (GtkSpinButton *spinbutton, gpointer data);	/*521*/
extern void data_522 (GtkSpinButton *spinbutton, gpointer data);	/*522*/
extern void data_523 (GtkSpinButton *spinbutton, gpointer data);	/*523*/

extern void data_530 (GtkMenuItem *menuitem, gpointer data);
extern void data_531 (GtkSpinButton *spinbutton, gpointer data);	/*531*/
extern void data_532 (GtkSpinButton *spinbutton, gpointer data);
extern void data_533 (GtkMenuItem *menuitem, gpointer data);

extern void data_600 (GtkMenuItem *menuitem, gpointer data);
extern void data_601 (GtkSpinButton *spinbutton, gpointer data);	/*601*/

extern void data_610 (GtkSpinButton *spinbutton, gpointer data);	/*610*/
extern void data_611 (GtkSpinButton *spinbutton, gpointer data);	/*611*/
extern void data_612 (GtkSpinButton *spinbutton, gpointer data);	/*612*/
extern void data_613 (GtkSpinButton *spinbutton, gpointer data);	/*613*/
extern void data_614 (GtkMenuItem *menuitem, gpointer data);

extern void data_620 (GtkSpinButton *spinbutton, gpointer data);	/*620*/
extern void data_623 (GtkSpinButton *spinbutton, gpointer data);	/*623*/

extern void data_710 (GtkMenuItem *menuitem, gpointer data);
extern void data_711 (GtkMenuItem *menuitem, gpointer data);
extern void data_713 (GtkSpinButton *spinbutton, gpointer data);	/*713*/

extern void data_720 (GtkSpinButton *spinbutton, gpointer data);	/*720*/
extern void data_721 (GtkSpinButton *spinbutton, gpointer data);	/*721*/
extern void data_722 (GtkSpinButton *spinbutton, gpointer data);	/*722*/
extern void data_723 (GtkSpinButton *spinbutton, gpointer data);	/*723*/
extern void data_724 (GtkSpinButton *spinbutton, gpointer data);	/*724*/
extern void data_725 (GtkSpinButton *spinbutton, gpointer data);	/*725*/
extern void data_730 (GtkMenuItem *menuitem, gpointer data);
extern void data_740 (GtkMenuItem *menuitem, gpointer data);
extern void data_741 (GtkMenuItem *menuitem, gpointer data);

extern void data_800 (GtkMenuItem *menuitem, gpointer data);
extern void data_804 (GtkMenuItem *menuitem, gpointer data);
extern void data_812 (GtkMenuItem *menuitem, gpointer data);
extern void data_824 (GtkMenuItem *menuitem, gpointer data);
extern void data_830 (GtkMenuItem *menuitem, gpointer data);

extern void data_900 (GtkMenuItem *menuitem, gpointer data);
extern void data_901 (GtkSpinButton *spinbutton, gpointer data);	/*901*/
extern void data_903 (GtkMenuItem *menuitem, gpointer data);
extern void data_904 (GtkMenuItem *menuitem, gpointer data);
extern void data_912 (GtkMenuItem *menuitem, gpointer data);
extern void data_913 (GtkMenuItem *menuitem, gpointer data);
extern void data_924 (GtkMenuItem *menuitem, gpointer data);
extern void data_930 (GtkMenuItem *menuitem, gpointer data);

extern gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

extern gboolean key_press_handler (GtkWidget* pWidget, GdkEventKey* pEvent, gpointer data);


extern void draw3_pressed1(gfloat step, guint digit);

#endif
