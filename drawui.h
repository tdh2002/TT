
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#ifndef	__DRAW_UI_H_
#define	__DRAW_UI_H_
#include "base.h"
#include "language.h"


/* 扫查模式 */
#define UT_SCAN		0
#define PA_SCAN		1
/* 当前位置信息  */
#define MENU1_STOP    4
#define MENU2_STOP    0 
#define MENU2_PRESSED 1
#define MENU3_STOP    2
#define MENU3_PRESSED 3
/* 单位 */
#define UNIT_MM		0
#define UNIT_INCH	1
#define UNIT_US		2
#define UNIT_NULL	3
#define UNIT_M_S	4
#define UNIT_IN_US	5
#define UNIT_DB 	6
#define UNIT_TO 	7
#define UNIT_BFH 	8
#define UNIT_MS 	9
#define UNIT_MM_S 	10
#define UNIT_TO1 	11
#define UNIT_DB_MM      12
#define UNIT_S		13
#define UNIT_DEG	14
#define UNIT_STEP_MM	15
#define UNIT_RPM	16
#define UNIT_NONE	17

/* UT_UNIT */
#define UT_UNIT_SOUNDPATH	0
#define UT_UNIT_TIME		1
#define UT_UNIT_TRUE_DEPTH	2

#define AUTO_SET	0
/* 发射模式 P111 */
#define PITCH_CATCH		0
#define	PULSE_ECHO		1
#define	THROUGH_TRAN	2
#define TOFD			3
/* 电压 P113 */
#define VOLTAGE_HIGH
#define VOLTAGE_MID
#define VOLTAGE_LOW
/* 电压 */
#define VOL_LOW		0
#define VOL_HIGH	1

/* 检波 P122 */
#define RF_WAVE			0
#define POSITIVE_WAVE	1
#define NAGATIVE_WAVE	2
#define FULL_WAVE		3
/* 闸门 */
#define GATE_A	0
#define GATE_B  1
#define GATE_I  2

#define GATE_POSITION 0
#define GATE_MODE     1

#define GATE_PULSER   0
#define GATE_II       1
#define GATE_AA	      2

/* focal law 类型 定义*/
#define	ANGLE_SCAN	0
#define LINEAR_SCAN	1

/* 0纵波 1横波 */
#define LONGITUDINAL_WAVE	0
#define SHEAR_WAVE	1

/* 显示模式ABSC */
#define	DISPLAY_ALL_GROUP	0
#define DISPLAY_CURRENT_GROUP	1
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

#define GAIN_MAX	80.0
#define GAINR_OFF	0
#define GAINR_ON	1
#define AUTO_FOCAL_OFF	0
#define AUTO_FOCAL_ON	1
#define VIDEO_FILTER_OFF 0
#define VIDEO_FILTER_ON	 1

/*  */
#define UT_CHANNEL	3




/*上方数值显示信息定义 */
#define GAIN_LABEL	0
#define GAIN_VALUE	1

/* 最大测量范围  扫描延时+扫描范围 */
#define MAX_RANGE_US	9899.89

/* 弹出的对话框的类型 */
#define DIALOG_REMARK	0
#define DIALOG_PROBE	1
#define DIALOG_WEDGE	2

#define DIALOG_COLOR_PALETTE	11
#define DIALOG_FILE_MANAGE		12

/* 各个menu 弹出菜单的偏移量 */
#define TX_RX_MODE	0
#define FREQUENCY	(TX_RX_MODE + 8)
#define VOLTAGE		(FREQUENCY + 14)
#define PULSER_WIDTH	(VOLTAGE + 8)
#define PRF		(PULSER_WIDTH + 2)
#define FILTER		(PRF + 4)
#define RECTIFIER	(FILTER + 16)
#define AVERAGING	(RECTIFIER + 4)
#define OFF_ON		(AVERAGING + 5)
#define POINT_QTY	(OFF_ON + 2)
#define SUM_GAIN	(POINT_QTY + 5)
#define GATE_POS	(SUM_GAIN + 2)
#define PARAMETERS	(GATE_POS + 3)
#define SYNCHRO 	(PARAMETERS + 2)
#define MEASURE 	(SYNCHRO + 3)
#define RECTIFIER_FREQ 	(MEASURE + 2)
#define ALARM_POS 	(RECTIFIER_FREQ + 3)
#define ALARM_ON_POS 	(ALARM_POS + 16)
#define GROUPA	 	(ALARM_ON_POS + 16)
#define CONDITIONA 	(GROUPA + 8)
#define OPERAT 		(CONDITIONA + 9)
#define OUTPUT_POS	(OPERAT + 2)
#define ALARM1		(OUTPUT_POS+10)
#define DATA		(ALARM1+37)
#define SOUND		(DATA+4)
#define MODE_POS	(SOUND+10)
#define CURVE_POS	(MODE_POS + 2)
#define LIST		(CURVE_POS + 4 )
#define FIELD		(LIST + 2)
#define SELECTION	(FIELD + 61)
#define DATA_LINK	(SELECTION+9)
#define SOURCE		(DATA_LINK+4)
#define DISPLAY		(SOURCE + 9)
#define DISPL		(DISPLAY + 11)
#define GROUP		(DISPL +11)
#define C_SCAN1		(GROUP +2)
#define DATA2		(C_SCAN1 + 5)
#define DIS_MODE	(DATA2 + 4)
#define UTUNIT		(DIS_MODE + 4)
#define GRID		(UTUNIT + 3)
#define ZOOM_DISPLAY	(GRID + 6)
#define TYPE		(ZOOM_DISPLAY + 6)
#define COL_SELECT	(TYPE + 6 )
#define COL_MODE	(COL_SELECT + 3)
#define PROP_SCAN	(COL_MODE + 2)
#define PROP_COLOR	(PROP_SCAN + 6)
#define ENVELOPE	(PROP_COLOR + 6)
#define PROP_SOURCE	(ENVELOPE + 2)
#define PROP_APP	(PROP_SOURCE + 4)
#define PROP_OVERLAY	(PROP_APP + 4)
#define FFT_COLOR	(PROP_OVERLAY + 3)
#define ORIENTATION	(FFT_COLOR + 4)
#define GROUP_P		(ORIENTATION + 2)
#define GROUP_MODE	(GROUP_P + 10)
#define GROUP_MODE_P	(GROUP_MODE + 2)
#define PROB_SELECT	(GROUP_MODE_P + 2)
#define PROB_SKEW	(PROB_SELECT + 2 )
#define GEOMETRY	(PROB_SKEW + 5)
#define MATERIAL	(GEOMETRY + 3)
#define LAW_CONFIG	(MATERIAL + 7)
#define L_CONFIG	(LAW_CONFIG + 4)
#define WAVE_TYPE	(L_CONFIG + 4)
#define W_TYPE		(WAVE_TYPE + 3)
#define ENCODER		(W_TYPE + 3)
#define POLARITY	(ENCODER + 2)
#define E_TYPE		(POLARITY + 2)
#define I_TYPE		(E_TYPE + 9)
#define I_SCAN		(I_TYPE + 3)
#define I_INDEX		(I_SCAN + 3)
#define START_MODE	(I_INDEX + 5)
#define STORAGE		(START_MODE + 3)
#define INSPEC_DATA	(STORAGE + 4)
#define F_STORAGE	(INSPEC_DATA + 2)
#define F_STORAGE_P	(F_STORAGE + 2)
#define SAVE_MODE	(F_STORAGE_P + 2)
#define TEMPLA		(SAVE_MODE + 4)
#define TEMPLA_P	(TEMPLA + 1)
#define PAPER_SIZE	(TEMPLA_P + 1)
#define VIEW		(PAPER_SIZE + 4)
#define F_SELECT	(VIEW + 3)
#define U_LABEL		(F_SELECT + 10)
#define U_CONTENT	(U_LABEL + 10)
#define P_UNITS		(U_CONTENT + 10)
#define SCHEME		(P_UNITS + 2 )
#define GATE_M		(SCHEME + 2)
#define SELECT_K	(GATE_M + 2)
#define ASSIGN_K	(SELECT_K + 9)
#define ASSIGN_K_P	(ASSIGN_K + 16)
#define MOUSE		(ASSIGN_K_P + 5)

/* 按键是由那个window发出 */
#define MENU_POP		0
#define MENU_DIS		1 /*disappear*/
#define MENU_CHANGE		2
#define MENU_UP			3
#define MENU_DOWN		4
#define MENU_ENTER		5

#define MENU3_TURN 	3
#define MENU3_ENTER	4
#define MENU3_UP	5
#define MENU3_DOWN	6
#define MENU3_DIS	7
#define MENU3_SHOW	8

#define HELP_N		0
#define HELP_Y		1

#define	MAIN_MENU_PRESS	1
#define	MENU31_PRESS	2
#define	MENU32_PRESS	3
#define	MENU33_PRESS	4
#define	MENU34_PRESS	5
#define	MENU35_PRESS	6
#define	MENU36_PRESS	7

/* 发送给硬件数据类型 */
#define BEAM_DELAY_DSP	0x1000	/* 0x1000~0x10ff 最多256个聚焦法则 */
#define	RANGE_DSP		0x2000	/*  */	

#define ELEM_T_START 
#define ELEM_R_START 

#define GATEA_DSP		0x3000
#define GATEB_DSP		0x3001
#define GATEI_DSP		0x3002
#define TX_RX_MODE_DSP		0x3003
#define FREQUENCY_DSP		0x3004
#define	VOLTAGE_DSP		0x3005
#define PW_DSP			0x3006
#define PRF_DSP			0x3007
#define FILTER_DSP		0x3008
#define RECTIFIER_DSP		0x3009
#define AVERAGING_DSP		0x300a
#define VIDEO_FILTER_DSP	0x300b
#define POINT_QTY_DSP		0x300c
#define SUM_GAIN_DSP		0x300d
#define GATE_POS_DSP		0x300e
#define PARAMETERS_DSP		0x300f
#define START_DSP		0x3010
#define SYNCHRO_DSP		0x3011
#define WIDTH_DSP		0x3012
#define MEASURE_DSP		0x3013
#define HEIGHT_DSP		0x3014
#define RECTIFIER_FREQ_DSP	0x3015
#define ALARM_POS_DSP		0x3016
#define GROUPA_DSP		0x3017
#define CONDITIONA_DSP		0x3018
#define OPERAT_DSP		0x3019
#define GROUPB_DSP		0x301a
#define CONDITIONB_DSP		0x301b
#define OUTPUT_POS_DSP		0x301c
#define ALARM1_DSP		0x301d
#define COUNT_DSP		0x301e
#define DATA_DSP		0x301f
#define SOUND_DSP		0x3020
#define DELAY_DSP		0x3021
#define HOLDTIME_DSP		0x3022
#define MODE_POS_DSP		0x3023
#define CURVE_POS_DSP		0x3024
#define POINT_POS_DSP		0x3025

extern gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
extern gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);
extern GtkWidgetClass *widget_window_class;

extern DRAW_UI_P pp;					

//extern guint address[];
extern guint get_beam_qty();
extern guint get_freq ();
extern guint get_pw ();
extern guint get_prf ();
extern guint get_filter ();

extern void tt_label_show_string (GtkWidget *label, const gchar *s1, const gchar *s2,
		const gchar *s3, const gchar *color,  guint font_size);
extern void tt_label_show_float (GtkWidget *label, const gchar *s1, gfloat value, guint digit, 
		const gchar *s3, const gchar *color,  guint font_size);

extern void change_language();										/* 选择语言 */
extern void draw_2_menu(gint pa);									/* 更新二级菜单 */
extern void draw_3_menu(gint pa, gpointer p);					    /* 更新三级菜单 */
extern void init_ui(DRAW_UI_P p);									/* 初始化界面, */
extern void draw_area_all();

extern void send_dsp_data (guint data_type, guint value);
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
extern void data_120 (GtkSpinButton *spinbutton, gpointer data);	/* 120 receiver 脉冲接收器 */
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
extern void data_2211 (GtkMenuItem *menuitem, gpointer data);         /* Output -> group */
extern void data_222 (GtkSpinButton *spinbutton, gpointer data);	/*222 Output -> count */
extern void data_2221 (GtkMenuItem *menuitem, gpointer data);         /* Output -> data */
extern void data_223 (GtkMenuItem *menuitem, gpointer data);          /* 223 sound */
extern void data_224 (GtkSpinButton *spinbutton, gpointer data);	/*224*/
extern void data_225 (GtkSpinButton *spinbutton, gpointer data);	/*225*/
extern void data_230 (GtkMenuItem *menuitem, gpointer data);         /* Gate/Alarm -> Sizing Curves -> Mode 230 */
extern void data_231 (GtkMenuItem *menuitem, gpointer data);         /* Gate/Alarm -> Sizing Curves -> Curve 231 */
extern void data_2311 (GtkMenuItem *menuitem, gpointer data);
extern void data_2312 (GtkSpinButton *spinbutton, gpointer data);
extern void data_232 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2321 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2322 (GtkSpinButton *spinbutton, gpointer data);
extern void data_233 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2331 (GtkSpinButton *spinbutton, gpointer data);
extern void data_2332 (GtkSpinButton *spinbutton, gpointer data);
extern void data_234 (GtkSpinButton *spinbutton, gpointer data);
extern void data_235 (GtkSpinButton *spinbutton, gpointer data);

extern void data_300 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> list 300 */
extern void data_302 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field 302 */
extern void data_303 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field2 303 */
extern void data_304 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field3 304 */
extern void data_305 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> Reading -> Field4 305 */

extern void data_310 (GtkMenuItem *menuitem, gpointer data);         /* Measurements -> cursors -> selection 310 */
extern void data_311 (GtkSpinButton *spinbutton, gpointer data);	/*311*/
extern void data_3111 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3112 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3113 (GtkSpinButton *spinbutton, gpointer data);	
extern void data_312 (GtkSpinButton *spinbutton, gpointer data);	/*312*/
extern void data_3121 (GtkSpinButton *spinbutton, gpointer data);	/*312*/
extern void data_3122 (GtkSpinButton *spinbutton, gpointer data);
extern void data_313 (GtkSpinButton *spinbutton, gpointer data);	/*313*/
extern void data_3131 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3132 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3133 (GtkSpinButton *spinbutton, gpointer data);
extern void data_314 (GtkSpinButton *spinbutton, gpointer data);	/*314*/
extern void data_3141 (GtkSpinButton *spinbutton, gpointer data);
extern void data_3142 (GtkSpinButton *spinbutton, gpointer data);
extern void data_315 (GtkMenuItem *menuitem, gpointer data);
extern void data_3151 (GtkSpinButton *spinbutton, gpointer data);
extern void data_324 (GtkSpinButton *spinbutton, gpointer data);        /*324*/
extern void data_330 (GtkMenuItem *menuitem, gpointer data);          /* Measurements -> Thickness -> source 330 */
extern void data_331 (GtkSpinButton *spinbutton, gpointer data);	/*331*/
extern void data_332 (GtkSpinButton *spinbutton, gpointer data);	/*332*/
extern void data_333 (GtkSpinButton *spinbutton, gpointer data);	/*333*/

extern void data_400 (GtkMenuItem *menuitem, gpointer data);            /*400*/
extern void data_401 (GtkMenuItem *menuitem, gpointer data);            /*401*/
extern void data_4011 (GtkMenuItem *menuitem, gpointer data);
extern void data_4012 (GtkMenuItem *menuitem, gpointer data);
extern void data_4013 (GtkMenuItem *menuitem, gpointer data);
extern void data_402 (GtkMenuItem *menuitem, gpointer data);
extern void data_4021 (GtkMenuItem *menuitem, gpointer data);
extern void data_403 (GtkMenuItem *menuitem, gpointer data);
extern void data_404 (GtkSpinButton *spinbutton, gpointer data);
extern void data_405 (GtkSpinButton *spinbutton, gpointer data);
extern void data_410 (GtkMenuItem *menuitem, gpointer data);            /*410*/
extern void data_411 (GtkMenuItem *menuitem, gpointer data);            /*411*/
extern void data_420 (GtkMenuItem *menuitem, gpointer data);            /*420*/
extern void data_421 (GtkMenuItem *menuitem, gpointer data); 
extern void data_422 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4221 (GtkSpinButton *spinbutton, gpointer data);
extern void data_423 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4231 (GtkSpinButton *spinbutton, gpointer data);
extern void data_424 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4241 (GtkSpinButton *spinbutton, gpointer data);
extern void data_425 (GtkSpinButton *spinbutton, gpointer data);
extern void data_4251 (GtkSpinButton *spinbutton, gpointer data);
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

extern void data_500 (GtkMenuItem *menuitem, gpointer data);
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
extern void data_621 (GtkSpinButton *spinbutton, gpointer data);
extern void data_622 (GtkSpinButton *spinbutton, gpointer data);
extern void data_623 (GtkSpinButton *spinbutton, gpointer data);	/*623*/

extern void data_700 (GtkMenuItem *menuitem, gpointer data);
extern void data_701 (GtkMenuItem *menuitem, gpointer data);
extern void data_702 (GtkMenuItem *menuitem, gpointer data);
extern void data_703 (GtkSpinButton *spinbutton, gpointer data);
extern void data_704 (GtkSpinButton *spinbutton, gpointer data);
extern void data_710 (GtkMenuItem *menuitem, gpointer data);
extern void data_711 (GtkMenuItem *menuitem, gpointer data);
extern void data_712 (GtkMenuItem *menuitem, gpointer data);
extern void data_713 (GtkSpinButton *spinbutton, gpointer data);	/*713*/
extern void data_714 (GtkSpinButton *spinbutton, gpointer data);
extern void data_715 (GtkSpinButton *spinbutton, gpointer data);

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
extern void data_810 (GtkMenuItem *menuitem, gpointer data);
extern void data_812 (GtkMenuItem *menuitem, gpointer data);
extern void data_824 (GtkMenuItem *menuitem, gpointer data);
extern void data_830 (GtkMenuItem *menuitem, gpointer data);

extern void data_900 (GtkMenuItem *menuitem, gpointer data);
extern void data_901 (GtkSpinButton *spinbutton, gpointer data);	/*901*/
extern void data_904 (GtkMenuItem *menuitem, gpointer data);
extern void data_903 (GtkMenuItem *menuitem, gpointer data);
extern void data_912 (GtkMenuItem *menuitem, gpointer data);
extern void data_913 (GtkMenuItem *menuitem, gpointer data);
extern void data_9131 (GtkMenuItem *menuitem, gpointer data);
//extern void data_923 (GtkMenuItem *menuitem, gpointer data);
extern void data_930 (GtkMenuItem *menuitem, gpointer data);

extern gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

extern gboolean key_press_handler (GtkWidget* pWidget, GdkEventKey* pEvent, gpointer data);
extern void main_menu_pop();
extern void show_help(guint i);
extern void menu3_pop();

extern void draw_warning(guint btn_qty, const gchar *warn_info);

extern void draw3_pressed1(gfloat step, guint digit);

extern void da_call_remark (GtkDialog *dialog, gint response_id, gpointer user_data);

extern void dialog_destroy(GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern void add_to_list(GtkWidget *list, const gchar *str, guint count);
#endif
