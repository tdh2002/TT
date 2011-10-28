
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include "focallaw.h"		/* 计算聚焦法则的头文件 */
#include "base_config.h"
#include "spi_d.h"
#include "file_op.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>
#include <pthread.h>

#define MAX_DMA_FREQUENCY    400


pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

extern void focal_law(gpointer data);/*回调函数*/
extern void draw_area_calibration();
extern void switch_area();
extern void draw_field_value ();
extern void send_group_spi (guint grp);
extern void send_focal_spi (guint grp);

static int handler_key(guint keyval, gpointer data);
static int thread_set_DB_eighty_percent(gpointer data);

extern const gchar ****con2_p;
extern int fd_array ;

extern int REFRESH_REQUEST ;
extern int *DMA_MARK    ;

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

gboolean key_press_handler (GtkWidget* pWidget,
		GdkEventKey* pEvent, gpointer data);
void send_spi_data (gint group);
guint get_beam_qty();
guint get_skew();
guint get_freq ();
guint get_pw ();
guint get_prf ();
guint get_max_prf();
guint get_filter ();
guint get_max_point_qty();

/*add by hefan */
void update_gate_info();
void generate_focallaw(int grp);
gfloat cba_encoder();
gfloat cba_ultrasound_velocity();
void cba_ultrasound_wedgedelay();
void cba_ultrasound_sensitivity();
void cba_ultrasound_TCG();
void esc_calibration();
void draw_encoder_value(gpointer data);

float tttmp;

//把group向导的设置参数保存起来
_group_wizard g_group_wizard_struct;

/* 输入数字时候的其他快捷键 */
static guint key_fast_map[] =
{
	GDK_Return, GDK_Escape, GDK_F1, GDK_F2, GDK_F3, GDK_F4, GDK_F5, GDK_F6, 
	GDK_F7, GDK_F8, GDK_F9, GDK_F10, GDK_F11, GDK_F12, GDK_Super_L
};

gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

void data_0021 (GtkMenuItem *menuitem, gpointer data);
void data_0022 (GtkMenuItem *menuitem, gpointer data);
void data_022 (GtkMenuItem *menuitem, gpointer data);
void data_0221 (GtkMenuItem *menuitem, gpointer data);
void data_0222 (GtkSpinButton *spinbutton, gpointer data);
void data_0223 (GtkSpinButton *spinbutton, gpointer data);
void data_0224 (GtkSpinButton *spinbutton, gpointer data);
void data_0225 (GtkMenuItem *menuitem, gpointer data);
void data_0226 (GtkSpinButton *spinbutton, gpointer data);
void data_0227 (GtkSpinButton *spinbutton, gpointer data);
void data_023 (GtkMenuItem *menuitem, gpointer data); 
void data_0231 (GtkMenuItem *menuitem, gpointer data); 
void data_0232 (GtkSpinButton *spinbutton, gpointer data);
void data_0233 (GtkSpinButton *spinbutton, gpointer data);
void data_0234 (GtkSpinButton *spinbutton, gpointer data);
void data_0235 (GtkSpinButton *spinbutton, gpointer data);
void data_0236 (GtkSpinButton *spinbutton, gpointer data);
void data_0237 (GtkSpinButton *spinbutton, gpointer data);
void data_0238 (GtkSpinButton *spinbutton, gpointer data);
void data_0239 (GtkSpinButton *spinbutton, gpointer data);
void data_024 (GtkSpinButton *spinbutton, gpointer data);
void data_0241 (GtkSpinButton *spinbutton, gpointer data);
void data_0242 (GtkSpinButton *spinbutton, gpointer data);
void data_0243 (GtkSpinButton *spinbutton, gpointer data);
void data_032 (GtkSpinButton *spinbutton, gpointer data);
void data_0321 (GtkSpinButton *spinbutton, gpointer data);
void data_033 (GtkMenuItem *menuitem, gpointer data);
void data_0331 (GtkSpinButton *spinbutton, gpointer data);
void data_0332 (GtkSpinButton *spinbutton, gpointer data);
void data_034 (GtkSpinButton *spinbutton, gpointer data);

void data_100 (GtkSpinButton *spinbutton, gpointer data);
void data_101 (GtkSpinButton *spinbutton, gpointer data);
void data_102 (GtkSpinButton *spinbutton, gpointer data);
void data_103 (GtkSpinButton *spinbutton, gpointer data);
void data_104 (GtkSpinButton *spinbutton, gpointer data);

void data_110 (GtkSpinButton *spinbutton, gpointer data);
void data_120 (GtkSpinButton *spinbutton, gpointer data);
void data_111 (GtkMenuItem *menuitem, gpointer data);            /* 111 Tx/Rx Mode 收发模式 */
void data_1121(GtkSpinButton *spinbutton, gpointer data);        /* 112 freq 频率 */
void data_112 (GtkMenuItem *menuitem, gpointer data);            /* 112 Freq 频率 */
void data_113 (GtkMenuItem *menuitem, gpointer data);            /* 113 voltage  */
void data_1141(GtkSpinButton *spinbutton, gpointer data);        /* 114 PW 脉宽*/
void data_114 (GtkMenuItem *menuitem, gpointer data);            /* 114 PW 脉宽 */
void data_1151(GtkSpinButton *spinbutton, gpointer data);        /* 115 PRF  重复频率*/
void data_115 (GtkMenuItem *menuitem, gpointer data);            /* 115 PRF  重复频率 */

void data_1201 (GtkMenuItem *menuitem, gpointer data); 
void data_121 (GtkMenuItem *menuitem, gpointer data);        /* 121 Filter */
void data_122 (GtkMenuItem *menuitem, gpointer data);        /* 122 Rectifier */
void data_124 (GtkMenuItem *menuitem, gpointer data);        /* 124 Averaging */
void data_125 (GtkSpinButton *spinbutton, gpointer data); 

void data_130 (GtkSpinButton *spinbutton, gpointer data);
void data_131 (GtkSpinButton *spinbutton, gpointer data);
void data_132 (GtkSpinButton *spinbutton, gpointer data);
void data_134 (GtkSpinButton *spinbutton, gpointer data);
void data_135 (GtkSpinButton *spinbutton, gpointer data);

void data_140 (GtkSpinButton *spinbutton, gpointer data);

void data_1431(GtkSpinButton *spinbutton, gpointer data);        /* 143 Points Qty */
void data_143 (GtkMenuItem *menuitem, gpointer data);            /* 143 Points Qty */
void data_1451(GtkSpinButton *spinbutton, gpointer data);        /* 145 Sum Gain */
void data_145 (GtkMenuItem *menuitem, gpointer data);            /* 145 Sum Gain */

void data_200 (GtkMenuItem *menuitem, gpointer data);         /*  200 Gate */
void data_201 (GtkMenuItem *menuitem, gpointer data);         /*  201 parameter */
void data_202 (GtkSpinButton *spinbutton, gpointer data);
void data_2021 (GtkMenuItem *menuitem, gpointer data);         /*  2021 synchro */
void data_203 (GtkSpinButton *spinbutton, gpointer data);
void data_2031 (GtkMenuItem *menuitem, gpointer data);         /*  2031 measure */
void data_204 (GtkSpinButton *spinbutton, gpointer data);
void data_2041 (GtkMenuItem *menuitem, gpointer data);         /*  2041 rf */

void data_210 (GtkMenuItem *menuitem, gpointer data);         /*  210 Alarm */
void data_211 (GtkMenuItem *menuitem, gpointer data);         /*  211 Group A */
void data_212 (GtkMenuItem *menuitem, gpointer data);         /*  212 condition A */
void data_213 (GtkMenuItem *menuitem, gpointer data);         /*  213 Operator */
void data_214 (GtkMenuItem *menuitem, gpointer data);         /*  214 Group B */
void data_215 (GtkMenuItem *menuitem, gpointer data);         /*  215 condition B */

void data_220 (GtkMenuItem *menuitem, gpointer data);          /* 220 Output */
void data_221 (GtkMenuItem *menuitem, gpointer data);         /* Output->alarm # */
void data_2211 (GtkMenuItem *menuitem, gpointer data);         /* Output->group */
void data_222 (GtkSpinButton *spinbutton, gpointer data);	/* Output->count */
void data_2221 (GtkMenuItem *menuitem, gpointer data);		/* Output->data */
void data_223 (GtkMenuItem *menuitem, gpointer data);          /* 223 sound */
void data_224 (GtkSpinButton *spinbutton, gpointer data);
void data_225 (GtkSpinButton *spinbutton, gpointer data);
void data_230 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm->Sizing Curves->Mode 230 */
void data_231 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm->Sizing Curves->Curve 231 */
void data_2311 (GtkMenuItem *menuitem, gpointer data);
void data_2312 (GtkSpinButton *spinbutton, gpointer data);
void data_232 (GtkSpinButton *spinbutton, gpointer data);
void data_2321 (GtkSpinButton *spinbutton, gpointer data);
void data_2322 (GtkSpinButton *spinbutton, gpointer data);
void data_233 (GtkSpinButton *spinbutton, gpointer data);
void data_2331 (GtkSpinButton *spinbutton, gpointer data);
void data_2332 (GtkSpinButton *spinbutton, gpointer data);
void data_234 (GtkSpinButton *spinbutton, gpointer data);
void data_235 (GtkSpinButton *spinbutton, gpointer data);
void data_234_add_point(DRAW_UI_P p);
void data_235_del_point(DRAW_UI_P p);


void data_300 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->list 300 */
void data_302 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field1 302 */
void data_303 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field2 303 */
void data_304 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field3 304 */
void data_305 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field4 305 */

void data_310 (GtkMenuItem *menuitem, gpointer data); /* Measurements->cursors->selection 310 */
void data_311 (GtkSpinButton *spinbutton, gpointer data);
void data_3111 (GtkSpinButton *spinbutton, gpointer data);
void data_3112 (GtkSpinButton *spinbutton, gpointer data);
void data_3113 (GtkSpinButton *spinbutton, gpointer data);
void data_312 (GtkSpinButton *spinbutton, gpointer data);
void data_3121 (GtkSpinButton *spinbutton, gpointer data);
void data_3122 (GtkSpinButton *spinbutton, gpointer data);
void data_313 (GtkSpinButton *spinbutton, gpointer data);
void data_3131 (GtkSpinButton *spinbutton, gpointer data);
void data_3132 (GtkSpinButton *spinbutton, gpointer data);
void data_3133 (GtkSpinButton *spinbutton, gpointer data);
void data_314 (GtkSpinButton *spinbutton, gpointer data);
void data_3141 (GtkSpinButton *spinbutton, gpointer data);
void data_3142 (GtkSpinButton *spinbutton, gpointer data);
void data_315 (GtkMenuItem *menuitem, gpointer data);
void data_3151 (GtkSpinButton *spinbutton, gpointer data); 
void data_324 (GtkSpinButton *spinbutton, gpointer data);

void data_330 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Thickness->source 330 */
void data_331 (GtkSpinButton *spinbutton, gpointer data);
void data_332 (GtkSpinButton *spinbutton, gpointer data);
void data_333 (GtkSpinButton *spinbutton, gpointer data);

void data_400 (GtkMenuItem *menuitem, gpointer data);
void data_401 (GtkMenuItem *menuitem, gpointer data);
void data_4011 (GtkMenuItem *menuitem, gpointer data);
void data_4012 (GtkMenuItem *menuitem, gpointer data);
void data_4013 (GtkMenuItem *menuitem, gpointer data);
void data_402 (GtkMenuItem *menuitem, gpointer data);
void data_4021 (GtkMenuItem *menuitem, gpointer data);
void data_403 (GtkMenuItem *menuitem, gpointer data);
void data_404 (GtkSpinButton *spinbutton, gpointer data);
void data_405 (GtkSpinButton *spinbutton, gpointer data);
void data_410 (GtkMenuItem *menuitem, gpointer data);
void data_411 (GtkMenuItem *menuitem, gpointer data);
void data_420 (GtkMenuItem *menuitem, gpointer data);
void data_421 (GtkMenuItem *menuitem, gpointer data);
void data_422 (GtkSpinButton *spinbutton, gpointer data);
void data_4221 (GtkSpinButton *spinbutton, gpointer data);
void data_423 (GtkSpinButton *spinbutton, gpointer data);
void data_4231 (GtkSpinButton *spinbutton, gpointer data);
void data_424 (GtkSpinButton *spinbutton, gpointer data);
void data_4241 (GtkSpinButton *spinbutton, gpointer data);
void data_425 (GtkSpinButton *spinbutton, gpointer data);
void data_4251 (GtkSpinButton *spinbutton, gpointer data);
void data_430 (GtkMenuItem *menuitem, gpointer data);
void data_431 (GtkSpinButton *spinbutton, gpointer data);
void data_4311 (GtkSpinButton *spinbutton, gpointer data);
void data_4312 (GtkSpinButton *spinbutton, gpointer data);
void data_432 (GtkSpinButton *spinbutton, gpointer data);
void data_4321 (GtkSpinButton *spinbutton, gpointer data);
void data_4322 (GtkSpinButton *spinbutton, gpointer data);
void data_440 (GtkMenuItem *menuitem, gpointer data);
void data_434 (GtkMenuItem *menuitem, gpointer data);
void data_441 (GtkMenuItem *menuitem, gpointer data);
void data_4411 (GtkSpinButton *spinbutton, gpointer data);
void data_4414 (GtkMenuItem *menuitem, gpointer data);
void data_4415 (GtkMenuItem *menuitem, gpointer data);
void data_442 (GtkMenuItem *menuitem, gpointer data);
void data_443 (GtkMenuItem *menuitem, gpointer data);
void data_444 (GtkMenuItem *menuitem, gpointer data);
void data_445 (GtkMenuItem *menuitem, gpointer data);

void data_500 (GtkMenuItem *menuitem, gpointer data);
void data_501 (GtkMenuItem *menuitem, gpointer data);
void data_502 (GtkMenuItem *menuitem, gpointer data);
void data_510 (GtkSpinButton *spinbutton, gpointer data);
void data_511 (GtkSpinButton *spinbutton, gpointer data);
void data_5121(GtkSpinButton *spinbutton, gpointer data);        /* 512 Skew (deg) */
void data_512 (GtkMenuItem *menuitem, gpointer data);            /* 512 Skew (deg) */


void data_521 (GtkSpinButton *spinbutton, gpointer data);
void data_522 (GtkSpinButton *spinbutton, gpointer data);
void data_523 (GtkSpinButton *spinbutton, gpointer data);

void data_530 (GtkMenuItem *menuitem, gpointer data);
void data_531 (GtkSpinButton *spinbutton, gpointer data);
void data_532 (GtkSpinButton *spinbutton, gpointer data);
void data_533 (GtkMenuItem *menuitem, gpointer data);
void data_534 (GtkMenuItem *menuitem, gpointer data);

void data_600 (GtkMenuItem *menuitem, gpointer data);
void data_601 (GtkSpinButton *spinbutton, gpointer data);

void data_610 (GtkSpinButton *spinbutton, gpointer data);
void data_611 (GtkSpinButton *spinbutton, gpointer data);
void data_612 (GtkSpinButton *spinbutton, gpointer data);
void data_613 (GtkSpinButton *spinbutton, gpointer data);
void data_614 (GtkSpinButton *spinbutton, gpointer data);

void data_620 (GtkMenuItem *menuitem, gpointer data);
void data_621 (GtkSpinButton *spinbutton, gpointer data);
void data_6211 (GtkSpinButton *spinbutton, gpointer data);
void data_622 (GtkSpinButton *spinbutton, gpointer data);
void data_6221 (GtkSpinButton *spinbutton, gpointer data);
void data_623 (GtkSpinButton *spinbutton, gpointer data);
void data_6231 (GtkSpinButton *spinbutton, gpointer data);
void data_624 (GtkSpinButton *spinbutton, gpointer data);
void data_630 (GtkSpinButton *spinbutton, gpointer data);
void data_631 (GtkSpinButton *spinbutton, gpointer data);
void data_632 (GtkSpinButton *spinbutton, gpointer data);
void data_633 (GtkSpinButton *spinbutton, gpointer data);
void data_634 (GtkMenuItem *menuitem, gpointer data);

void data_700 (GtkMenuItem *menuitem, gpointer data);
void data_701 (GtkMenuItem *menuitem, gpointer data);
void data_702 (GtkMenuItem *menuitem, gpointer data);
void data_703 (GtkSpinButton *spinbutton, gpointer data);
void data_704 (GtkSpinButton *spinbutton, gpointer data);
void data_710 (GtkMenuItem *menuitem, gpointer data);
void data_711 (GtkMenuItem *menuitem, gpointer data);
void data_712 (GtkMenuItem *menuitem, gpointer data);
void data_713 (GtkSpinButton *spinbutton, gpointer data);
void data_714 (GtkSpinButton *spinbutton, gpointer data);
void data_715 (GtkSpinButton *spinbutton, gpointer data);
void data_720 (GtkSpinButton *spinbutton, gpointer data);
void data_721 (GtkSpinButton *spinbutton, gpointer data);
void data_722 (GtkSpinButton *spinbutton, gpointer data);
void data_723 (GtkSpinButton *spinbutton, gpointer data);
void data_724 (GtkSpinButton *spinbutton, gpointer data);
void data_725 (GtkSpinButton *spinbutton, gpointer data);
void data_730 (GtkMenuItem *menuitem, gpointer data);
void data_740 (GtkMenuItem *menuitem, gpointer data);
void data_741 (GtkMenuItem *menuitem, gpointer data);

void data_800 (GtkMenuItem *menuitem, gpointer data);
void data_804 (GtkMenuItem *menuitem, gpointer data);
void data_810 (GtkMenuItem *menuitem, gpointer data);
void data_812 (GtkMenuItem *menuitem, gpointer data);
void data_824 (GtkMenuItem *menuitem, gpointer data);
void data_830 (GtkMenuItem *menuitem, gpointer data);

void data_900 (GtkMenuItem *menuitem, gpointer data);
void data_901 (GtkSpinButton *spinbutton, gpointer data);
void data_904 (GtkMenuItem *menuitem, gpointer data);
void data_903 (GtkMenuItem *menuitem, gpointer data);
void data_912 (GtkMenuItem *menuitem, gpointer data);
void data_913 (GtkMenuItem *menuitem, gpointer data);
void data_9131 (GtkMenuItem *menuitem, gpointer data);
//void data_923 (GtkMenuItem *menuitem, gpointer data);
void data_930 (GtkMenuItem *menuitem, gpointer data);

gboolean eventbox2_function0 (GtkWidget *widget, GdkEventButton *event,	gpointer data);

/*   */
static inline void data_process(guchar *data, guint pa);

void request_refresh()
{
#if ARM
	DMA_MARK[0] = 2 ;
	REFRESH_REQUEST = 1;
#endif
}


static void setup_para(PARAMETER_P p, guint group)
{
	//	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, group);
	/* 探头 */
	p->probe_p->D1_D2 = 0;			/* 0 1d 1 2d*/
	p->probe_p->Pitch_Catch = 
		//		!GROUP_VAL_POS(group, tx_rxmode);	/* 0 PE 1 PT 与我的相反 */
		!get_group_val (p_grp, GROUP_TX_RX_MODE);	/* 0 PE 1 PT 与我的相反 */
	p->probe_p->transmi_trans_longi = 0;	/* 1 SW 横波慢 0 LW 纵波快 */
	p->probe_p->recev_trans_longi	= 0;	/* */
	p->probe_p->pb_skew_angle	= GROUP_VAL_POS(group, skew) / 100.0;	/* ffff*/
	p->probe_p->pb_frequency	=
		get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0;	/* */
	p->probe_p->ele_num_pri = GROUP_VAL_POS(group, probe.Elem_qty);	/* */
	p->probe_p->ele_num_sec = 1;	/* 2D 时候就不为1了 */
	p->probe_p->pri_axis_pitch = GROUP_VAL_POS(group, probe.Pitch) / 1000.0;	/* */
	p->probe_p->sec_axis_pitch = 0;	/* 2D时候有效 */
	p->probe_p->pri_ele_size = 1;	/* ffff*/
	p->probe_p->sec_ele_size = 0;	/* 2D 时候有效 */

	p->probe_p->pb_skew_angle_r = 0;	/*ffff */
	p->probe_p->pb_frequency = 5.0;	/*ffff */
	p->probe_p->ele_num_pri_r = GROUP_VAL_POS(group, probe.Elem_qty);	/* */
	p->probe_p->ele_num_sec_r = 1;	/* */
	p->probe_p->pri_axis_pitch_r = GROUP_VAL_POS(group, probe.Pitch) / 1000.0;	/* */
	p->probe_p->sec_axis_pitch_r = 0;	/* */
	p->probe_p->pri_ele_size_r = 1;	/*ffff */
	p->probe_p->sec_ele_size_r = 0;	/*ffff */

	/* 楔块 */
	p->wedge_p->wg_wedge_angle = GROUP_VAL_POS (group, wedge.Angle) / 10.0; /* 楔块角 度 */
	p->wedge_p->wg_roof_angle	= 0;	/* 顶角 度 还没有找到对应的 */
	p->wedge_p->wg_lon_vel		= GROUP_VAL_POS (group, wedge.Velocity_PA) / 1000.0;		/*纵波声速m/s*/
	p->wedge_p->wg_trans_vel	= 0;
	p->wedge_p->wg_density= 7.8;/* 密度 */
	p->wedge_p->wg_heigh_fir	= GROUP_VAL_POS (group, wedge.Height) / 1000.0;	/*第一阵元高度mm*/
	p->wedge_p->wg_pri_elem_offset_fir = fabs(GROUP_VAL_POS (group, wedge.Primary_offset) / 1000.0);/*11111111111第一主轴阵元偏移mm*/
	p->wedge_p->wg_sec_elem_offset_fir = fabs(GROUP_VAL_POS (group, wedge.Secondary_offset) / 1000.0);/*11111111111第一次轴阵元偏移mm*/
	p->wedge_p->wg_pri_axis_reference = 0;/*主轴楔块参考位置mm*/
	p->wedge_p->wg_sec_axis_reference = 0;/*次轴楔块参考位置mm*/
	p->wedge_p->wg_length = 1;/*楔块长度mm*/
	p->wedge_p->wg_width = 1;/*楔块宽度mm*/
	p->wedge_p->wg_height = 1;/*楔块高mm*/
	p->wedge_p->wg_separation = 0;

	p->wedge_r->wg_wedge_angle = GROUP_VAL_POS(group, wedge.Angle) / 10.0; /* 楔块角 度 */
	p->wedge_r->wg_roof_angle=0;/* 顶角 度*/
	p->wedge_r->wg_lon_vel = 3230;/*纵波声速m/s*/
	p->wedge_r->wg_trans_vel = 3230;/*横波声速m/s*/
	p->wedge_r->wg_density= 7.8;/* 密度 */
	p->wedge_r->wg_heigh_fir	= GROUP_VAL_POS(group, wedge.Height) / 1000.0;	/*第一阵元高度mm*/
	p->wedge_r->wg_pri_elem_offset_fir = GROUP_VAL_POS (group, wedge.Primary_offset) / 1000.0;/*11111111111第一主轴阵元偏移mm*/
	p->wedge_r->wg_sec_elem_offset_fir = GROUP_VAL_POS (group, wedge.Secondary_offset) / 1000.0;/*11111111111第一次轴阵元偏移mm*/
	p->wedge_r->wg_pri_axis_reference = 0;/*主轴楔块参考位置mm*/
	p->wedge_r->wg_sec_axis_reference = 0;/*次轴楔块参考位置mm*/
	p->wedge_r->wg_length = 1;/*楔块长度mm*/
	p->wedge_r->wg_width = 1;/*楔块宽度mm*/
	p->wedge_r->wg_height = 1;/*楔块高度mm*/
	p->wedge_r->wg_separation = 0;

	/* BEAM */
	p->beam_angle->beam_pri_steer_angle_start = 0 ;
	p->beam_angle->beam_pri_steer_angle_stop = 0;
	p->beam_angle->beam_pri_steer_angle_resolution = 0;
	//
	p->beam_angle->beam_sec_steer_angle_start = 0;
	p->beam_angle->beam_sec_steer_angle_stop = 0;
	p->beam_angle->beam_sec_steer_angle_resolution = 0;
	//
	p->beam_angle->beam_refrac_angle_start = LAW_VAL_POS (group, Angle_min) / 100.0;  
	p->beam_angle->beam_refrac_angle_stop = LAW_VAL_POS (group, Angle_max) / 100.0;
	p->beam_angle->beam_refrac_angle_resolution = LAW_VAL_POS (group, Angle_step) / 100.0; 
	// 
	p->beam_angle->beam_skew_angle_start = 0;  
	p->beam_angle->beam_skew_angle_stop = 0;
	p->beam_angle->beam_skew_angle_resolution = 0;

	//与P600 Law Type 对应起来
	p->beam_angle->beam_type = LAW_VAL_POS(group,Focal_type);//何凡修改
	p->beam_angle->beam_angle_sel = 0;//只有Refracted angle一种情况 何凡修改

	/* 样本 */
	p->specimen->speci_longitudinal_wave	= get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100;
	p->specimen->speci_transverse_wave		= get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100;
	//
	//    p->specimen->speci_length_flat = 0;
	//    p->specimen->speci_height_flat = 0;
	//    p->specimen->speci_width_flat = 0;  
	//
	p->specimen->speci_inside_cylindrical = 0;
	p->specimen->speci_outside_cylindrical = 0;
	//    p->specimen->speci_length_cylindrical = 0; 
	p->specimen->flat_cylindrical = 0;
	//
	p->specimen->Inspection_od_id[0] = 0;  
	p->specimen->Inspection_od_id[1] = 1;  

	/* 聚焦点 */
	p->focal_point->focal_focus_type = LAW_VAL(Focal_point_type);	/* 0 half path 1 TURE DEPTH */
	//  p->focal_point->focal_focus_point_start = LAW_VAL_POS (group, Focus_depth) / 1000.0;	/* type =0 是 声程 type =1 是深度 */   
	p->focal_point->focal_focus_point_start = LAW_VAL(Position_start)/ 1000.0;	/* type =0 是 声程 type =1 是深度 */
	p->focal_point->focal_focus_point_stop = LAW_VAL(Position_end)/ 1000.0; 
	p->focal_point->focal_focus_point_resolution = LAW_VAL(Position_step)/ 1000.0;
	p->focal_point->offset_start = LAW_VAL(Position_start)/1000.0 ;//true depth 何凡添加
	p->focal_point->offset_end = 0;//
	p->focal_point->depth_start = 0;//
	p->focal_point->depth_end = 0;//

	p->element_sel->pri_axis_ape = LAW_VAL_POS (group, Elem_qty);
	p->element_sel->sec_axis_ape = 1;
	p->element_sel->primary_axis_s = LAW_VAL_POS (group, First_tx_elem);
	p->element_sel->primary_axis_e = LAW_VAL_POS (group, Last_tx_elem)-LAW_VAL_POS (group, Elem_qty)+1;//
	p->element_sel->primary_axis_r = LAW_VAL_POS (group, Elem_step);//

	p->location->rotation_x = 0;
	p->location->rotation_y = 0;
	p->location->rotation_z = 90;
}

static void save_cal_law(gint offset, gint group, PARAMETER_P p)
{
	gint i, j,k;
	gint ElementStart;
	gint ElementStop ;
	//	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, group);
	unsigned int tmp_max_beam_delay;


	for (i = 0; i < TMP(beam_qty[group]); i++)
	{
		//***************************************
		TMP(field_distance[i]) = (gfloat)(p->field_distance[i]);//每束 中心正元到出射点的距离 单位mm
		pp->G_delay[i] = (gint)p->G_delay[i];////保存每一个beam的延时  方便用于显示
//		printf("focal_beam_delay = %d \n", pp->G_delay[i]);
		BEAM_INFO(i+offset,beam_delay) = pp->G_delay[i];//modified by hefan
		//printf("beam_delay[%d]=%d \n", i, pp->G_delay[i]);
		//***************************************
		if(LAW_VAL_POS(group, Focal_type) == 1)//Linear
		{
			ElementStart = LAW_VAL_POS (group, First_tx_elem) + i*LAW_VAL_POS (group, Elem_step) - 1 ;
			ElementStop  = ElementStart + LAW_VAL_POS (group, Elem_qty) ;
		}
		else
		{
			ElementStart = LAW_VAL_POS (group, First_tx_elem) - 1 ;
			ElementStop  = ElementStart + LAW_VAL_POS (group, Elem_qty) ;
		}
		TMP(focal_law_all_beam[offset + i]).N_ActiveElements	= LAW_VAL_POS (group, Elem_qty);
		TMP(focal_law_all_beam[offset + i]).frequency		= get_group_val (p_grp, GROUP_TX_RX_MODE);
		TMP(focal_law_all_beam[offset + i]).cycle			= 1; 
		TMP(focal_law_all_beam[offset + i]).sumgain			= GROUP_VAL_POS (group, sum_gain) / 100;
		TMP(focal_law_all_beam[offset + i]).mode			= 
			!get_group_val (p_grp, GROUP_TX_RX_MODE);	/* 0 PE 1 PT 与我的相反 */
		TMP(focal_law_all_beam[offset + i]).filter			= 	
			get_group_val (get_group_by_id (pp->p_config, group), GROUP_FILTER);
		TMP(focal_law_all_beam[offset + i]).R_angle			= (LAW_VAL_POS (group, Angle_min) + 
				LAW_VAL_POS(group, Angle_step) * i) / 100.0;
		TMP(focal_law_all_beam[offset + i]).S_angle			= GROUP_VAL_POS (group, skew) / 100.0; 
		TMP(focal_law_all_beam[offset + i]).T_first			= LAW_VAL_POS (group, First_tx_elem);
		TMP(focal_law_all_beam[offset + i]).R_first			= LAW_VAL_POS (group, First_rx_elem);
		TMP(focal_law_all_beam[offset + i]).Scan_offset		= GROUP_VAL_POS (group, scan_offset) * 100;
		TMP(focal_law_all_beam[offset + i]).Index_offset	= GROUP_VAL_POS (group, index_offset) * 100;
		TMP(focal_law_all_beam[offset + i]).G_delay			= 
			get_group_val (get_group_by_id (pp->p_config, group), GROUP_WEDGE_DELAY)
			+	GROUP_VAL_POS (group, wedge.Probe_delay) + BEAM_INFO(i + offset,beam_delay);//modified by hefan 
		TMP(focal_law_all_beam[offset + i]).F_depth			= LAW_VAL_POS (group, Focus_depth);
		TMP(focal_law_all_beam[offset + i]).M_velocity		= get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100;

		for(k=ElementStart,j = 0; k< ElementStop; k++,j++)//,j < TMP(focal_law_all_beam[offset + i]).N_ActiveElements
		{ 
			TMP(focal_law_all_elem[offset + i][j]).E_number = j + 1;
			TMP(focal_law_all_elem[offset + i][j]).FL_gain	= get_group_val (get_group_by_id (pp->p_config, group), GROUP_GAIN) / 100;
			TMP(focal_law_all_elem[offset + i][j]).T_delay	= p->timedelay[i][k];
			TMP(focal_law_all_elem[offset + i][j]).R_delay	= p->timedelay[i][k];
			TMP(focal_law_all_elem[offset + i][j]).Amplitude = get_voltage (pp->p_config, get_current_group(pp->p_config)); 
			TMP(focal_law_all_elem[offset + i][j]).P_width	= 
				get_group_val (p_grp, GROUP_PW_VAL) / 100;
		} 
	} 

	tmp_max_beam_delay  = pp->G_delay[0];
	for (i = 1; i < TMP(beam_qty[group]); i++)
	{
		if(tmp_max_beam_delay < pp->G_delay[i])
			tmp_max_beam_delay = pp->G_delay[i] ;
	}
	TMP(max_beam_delay[group]) = tmp_max_beam_delay / 10 ;
}

void cal_focal_law (guint group)
{
	gint offset, k;
	PARAMETER_P p;

	p = (PARAMETER_P)g_malloc0(sizeof(PARAMETER));

	PROBEF        probe_p;
	WEDGEF        wedge_p;
	WEDGEF        wedge_r;
	BEAM_ANGLE   beam_angle;
	SPECIMEN     specimen;
	FOCAL_POINT  focal_point;
	ELEMENT_SEL  element_sel;
	LOCATION     location;

	p->probe_p	= &probe_p;
	p->wedge_p	= &wedge_p;
	p->wedge_r	= &wedge_r;
	p->beam_angle = &beam_angle;
	p->specimen	= &specimen;
	p->focal_point = &focal_point;
	p->element_sel = &element_sel;
	p->location = &location;
	p->k = 0 ;	            

	setup_para(p, group);
	focal_law(p);

	/* 把聚集法则信息保存起来 */
	for (offset = 0, k = 0 ; k < group; k++)
		offset += TMP(beam_qty[k]);

	save_cal_law (offset, group, p);

	g_free (p);
	return ;
}

/* 取整函数 */
gint rounding(gint src1, gint src2, gint round)
{
	if (src1 == src2)
		return src2;
	else if (src1 < src2)
		return (((src2 - src1 + round / 2) / round) * round + src1);
	else if (src1 > src2)
		return (src1 - ((src1 - src2 + round / 2) / round) * round);
	else 
		return 0;
}

guint	get_beam_qty()
{
	guint i, beam_qty = 0;
	for (i = 0; i < get_group_qty(pp->p_config); i++)
		beam_qty += TMP(beam_qty[i]);
	return beam_qty;
}

guint get_sum_gain ()
{
	if (GROUP_VAL(sum_gain_pos))
		return GROUP_VAL(sum_gain);
	else 
	{
		if (LAW_VAL(Elem_qty) == 1)
			GROUP_VAL(sum_gain) = 4800;
		else
			GROUP_VAL(sum_gain) = (46.1 - 20 * log10(LAW_VAL(Elem_qty) / 2)) * 100;
	}
	return GROUP_VAL(sum_gain);
}

guint get_max_point_qty()
{
	guint i, point_qty = 0, max_point_qty;
	for (i = 0; i < setup_MAX_GROUP_QTY; i++)
		point_qty += TMP(beam_qty[i]) * (GROUP_VAL_POS (i, point_qty) + 32);
	max_point_qty = 192000 - point_qty + TMP(beam_qty[get_current_group(pp->p_config)]) * (GROUP_VAL (point_qty) + 128);
	return MIN(((max_point_qty / TMP(beam_qty[get_current_group(pp->p_config)])) - 32), 8192);
}


int get_current_proper_point_qty ()
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if(get_group_val (p_grp, GROUP_RANGE) <= 6400)
	{
		GROUP_VAL(point_qty) = get_group_val (p_grp, GROUP_RANGE) / 10 ;
	}
	else
	{
		double float_compress_rate ;
		int    int_compress_rate   ;
		float_compress_rate = get_group_val (p_grp, GROUP_RANGE) / 6400.0 ;
		int_compress_rate   = (get_group_val (p_grp, GROUP_RANGE) % 6400) ? (int) (float_compress_rate + 1) : (int)float_compress_rate ;
		GROUP_VAL(point_qty) = get_group_val (p_grp, GROUP_RANGE) / (int_compress_rate * 10 );
	}

	set_group_val (p_grp, GROUP_RANGE,
			rounding(0, get_group_val (p_grp, GROUP_RANGE), GROUP_VAL(point_qty) * 10));
	return GROUP_VAL(point_qty) ;
}
/* 取得压缩点的数量 */
guint get_point_qty ()
{
	if (GROUP_VAL(point_qty_pos) == 4)
		return GROUP_VAL(point_qty);
	else 
	{
		switch (GROUP_VAL(point_qty_pos))
		{
			case 0:get_current_proper_point_qty();break;
			case 1:GROUP_VAL(point_qty) = MIN(get_max_point_qty(), 160);break;
			case 2:GROUP_VAL(point_qty) = MIN(get_max_point_qty(), 320);break;
			case 3:GROUP_VAL(point_qty) = MIN(get_max_point_qty(), 640);break;
			default:break;
		}
	}
	return GROUP_VAL(point_qty);
}

guint get_max_prf()
{
	unsigned int result;
	int i          ;
	int pulse_width;
	int point_qty  ;
	int wedge_delay;
	int probe_element_num;
	int velocity  ;
	int beam_sum  ;
	int group_num ;
	int max_element_power;
	int total_power      ;
	int voltage ;
	int rx_time[8] ;
	int max_rx_time;
	int single_beam_time;
	double prf_limit[3];
	// get group number
	group_num = get_group_qty (pp->p_config) ;
	// pulse width for every group is the same
	pulse_width = get_group_val (get_group_by_id (pp->p_config, 0), GROUP_PW_VAL) / 100;
	// velocity in specimen , for every group is the same
	velocity = get_group_val(get_group_by_id (pp->p_config, 0), GROUP_VELOCITY ) / 100;
	// wedge delay , mostly is to be zero
	wedge_delay = get_group_val(get_group_by_id (pp->p_config, 0) , GROUP_WEDGE_DELAY);
	//
	if(get_voltage (pp->p_config, 0 )==0)
		voltage = 50 ;
	else if (get_voltage (pp->p_config, 0 )==1)
		voltage = 100;
	// get sum beam number
	for (beam_sum = 0 , i = 0; i < group_num; i++)
		beam_sum += TMP(beam_qty[i]);
	// get total wave data size
	for (point_qty = 0 , i = 0; i < group_num; i++)
		point_qty += TMP(beam_qty[i]) * (GROUP_VAL_POS (i, point_qty) + 32);
	// get max element number
	for(probe_element_num = LAW_VAL_POS(0, Elem_qty),i = 1; i< group_num; i++)
		if(LAW_VAL_POS(i, Elem_qty)>probe_element_num)  probe_element_num = LAW_VAL_POS(i, Elem_qty);
	//  max single element power
	for(i = 0; i< group_num; i++)
	{
		// these a problem here for the max beam delay is calculated after generate focal law
		// where the group_spi structure should be initialized first
		rx_time[i] =  TMP(group_spi[i]).sample_range + TMP(max_beam_delay[i]);
		//printf("TMP(group_spi[i]).sample_range %d TMP(max_beam_delay[i]) %d \n", TMP(group_spi[i]).sample_range , TMP(max_beam_delay[i]));
	}
	for(max_rx_time = rx_time[0], i = 1; i< group_num; i++)
	{
		if(max_rx_time < rx_time[i]) max_rx_time = rx_time[i] ;
	}
	single_beam_time = 4 * max_rx_time                        ;   // unit -- 10nm
	prf_limit[0] = 100000000.0 / single_beam_time             ;
	prf_limit[0] = prf_limit[0] / beam_sum                    ;
	//printf("prf_limit[0] %f \n", prf_limit[0]);
	max_element_power = voltage * voltage * pulse_width / 50  ;
	total_power = max_element_power * probe_element_num       ;
	prf_limit[1] = 125000000.0 / max_element_power            ;
	if(probe_element_num  > 16) prf_limit[1] = 2000000000.0 / total_power ;
	prf_limit[1] = prf_limit[1] / beam_sum                    ;

	prf_limit[2] = MAX_DMA_FREQUENCY * (int)(MIN(192 * 1024.0 / point_qty , 1024 / beam_sum )) / 10;

	prf_limit[0] = MIN(prf_limit[0], prf_limit[1]);
	prf_limit[0] = MIN(prf_limit[0], prf_limit[2]);
	result = (int) prf_limit[0]                   ;

#if 0
	printf("****************\n");
	printf("TMP(beam_qty[0]) %d \n", TMP(beam_qty[0]));
	printf("GROUP_VAL_POS (0, point_qty) %d \n", GROUP_VAL_POS (0, point_qty));
	printf("group_num %d\n", group_num)       ;
	printf("probe_element_num %d \n", probe_element_num);
	printf("pulse_width %d\n", pulse_width)   ;
	printf("velocity  %d\n", velocity)        ;
	printf("wedge_delay  %d\n",wedge_delay)   ;
	printf("voltage %d\n", voltage)           ;
	printf("beam_sum %d\n", beam_sum)         ;
	printf("point_qty %d\n", point_qty)       ;
	printf("max_rx_time %d \n", max_rx_time)  ;
	printf("max_element_power %d\n", max_element_power);
	printf("prf_limit[1] %f \n", prf_limit[1]);
	printf("prf_limit[2] %f \n", prf_limit[2]);
	printf("result %d\n", result)             ;
	printf("******************\n")            ;
#endif

	return result * 10;
}

/* 计算prf,并且附加限制 限制计算 */
guint get_prf ()
{
	guint	prf_temp = get_max_prf();
	gint	grp = get_current_group (pp->p_config);
	GROUP	*p_grp = get_group_by_id (pp->p_config, grp);

	if (get_group_val (p_grp, GROUP_PRF_POS) == 3)
	{
		if (get_group_val (p_grp, GROUP_PRF_VAL) > prf_temp )
			set_group_val (p_grp, GROUP_PRF_VAL, prf_temp);
		if(get_group_val (p_grp, GROUP_PRF_VAL) <= MAX_DMA_FREQUENCY )
		{
			pp->p_config->virtual_focallaw = 1 ;
		}
		else
		{
			pp->p_config->virtual_focallaw = prf_temp % MAX_DMA_FREQUENCY ? (prf_temp/MAX_DMA_FREQUENCY + 1 ): (prf_temp / MAX_DMA_FREQUENCY) ;
		}
	}
	else 
	{
		switch (get_group_val (p_grp, GROUP_PRF_POS))
		{
			case 0:
				set_group_val (p_grp, GROUP_PRF_VAL, prf_temp);
				pp->p_config->virtual_focallaw = prf_temp % MAX_DMA_FREQUENCY ? (prf_temp/MAX_DMA_FREQUENCY + 1 ): (prf_temp / MAX_DMA_FREQUENCY) ;
				//printf("pp->p_config->virtual_focal_law %d \n", pp->p_config->virtual_focal_law );
				break;
			case 1:
				prf_temp = prf_temp / 2 ;
				set_group_val (p_grp, GROUP_PRF_VAL, prf_temp );
				pp->p_config->virtual_focallaw = prf_temp % MAX_DMA_FREQUENCY ? (prf_temp/MAX_DMA_FREQUENCY + 1 ): (prf_temp / MAX_DMA_FREQUENCY) ;
				//printf("pp->p_config->virtual_focal_law %d \n", pp->p_config->virtual_focal_law );
				break;
			case 2:
				set_group_val (p_grp, GROUP_PRF_VAL,
						(prf_temp > 600 ) ? 600 : prf_temp );
				if(prf_temp > 600 )
					prf_temp = 600 ;
				pp->p_config->virtual_focallaw = prf_temp % MAX_DMA_FREQUENCY ? (prf_temp/MAX_DMA_FREQUENCY + 1 ): (prf_temp / MAX_DMA_FREQUENCY) ;
				break;
			default:break;
		}
	}

	gchar *markup;
	markup=g_markup_printf_escaped(
			"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",get_group_val (p_grp, GROUP_PRF_VAL) / 10, (get_group_val (p_grp, GROUP_PRF_VAL)*get_beam_qty() / 10) * 1);
	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
	g_free(markup);

	return get_group_val (p_grp, GROUP_PRF_VAL);
}


/* 计算脉冲宽度 */
guint get_pw ()
{
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if (get_group_val (p_grp, GROUP_PW_POS))
		return (guint)((get_group_val (p_grp, GROUP_PW_VAL)) / 100.0);
	else
	{
		return (guint)(( 5000*10000 / get_group_val (p_grp, GROUP_FREQ_VAL)) / 100);/// 250) * 250; /* 改变脉冲宽度 */
	}
}

/* 计算滤波 0 1 None 和 Auto 时候怎么计算 */
guint get_filter ()
{
	int tmp = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS);
	switch (tmp) 
	{
		case 0:
			set_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER, 0);
			break;
		case 1:
			set_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER, 0);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			set_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER, tmp - 1);
			break;
		default:break;
	}
	return (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER));
}

guint get_freq ()
{
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	switch (get_group_val (p_grp, GROUP_FREQ_POS))
	{
		case 0:return 1000;break;
		case 1:return 1500;break;
		case 2:return 2000;break;
		case 3:return 2250;break;
		case 4:return 3500;break;
		case 5:return 4000;break;
		case 6:return 5000;break;
		case 7:return 7500;break;
		case 8:return 10000;break;
		case 9:return 12000;break;
		case 10:return 15000;break;
		case 11:return 20000;break;
		default:break;
	}
	return get_group_val (p_grp, GROUP_FREQ_VAL);
}

/* 返回skew */
guint get_skew ()
{
	switch (GROUP_VAL(skew_pos))
	{
		case 0:return 0;break;
		case 1:return 9000;break;
		case 2:return 18000;break;
		case 3:return 27000;break;
		default:break;
	}
	return GROUP_VAL(skew);
}

gint cal_menu3_qty()
{
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[pp->pos])
			{
				case 0:return 2;break;
				case 1:return 2;break;
				case 2:return 2;break;
				default:break;
			}
			break;
		case 1:
			break;
		default:break;

	}
	return 0;
}

/* 设置LABEL特别显示的函数 字符串 */
void tt_label_show_string (GtkWidget *label, const gchar *s1, const gchar *s2, 
		const gchar *s3, const gchar *color,  guint font_size)
{
	gchar *markup;

	markup = g_markup_printf_escaped(
			"<span foreground=\'%s\' font_desc=\'%d\'>%s%s%s</span>", color, font_size, s1, s2, s3);
	gtk_label_set_markup (GTK_LABEL (label), markup);

	g_free (markup);
}

/* 设置LABEL特别显示的函数 字符串与数值 */
void tt_label_show_float (GtkWidget *label, const gchar *s1, gfloat value, guint digit, 
		const gchar *s3, const gchar *color,  guint font_size)
{
	gchar *markup;

	markup = g_markup_printf_escaped(
			"<span foreground=\'%s\' font_desc=\'%d\'>%s%0.*f%s</span>", color, font_size, s1, digit,  value, s3);
	gtk_label_set_markup (GTK_LABEL (label), markup);

	g_free (markup);
}

/*二级菜单按钮的回调函数*/
void b2_fun0(DRAW_UI_P p, guint pos)
{
	if(gtk_widget_get_visible(p->eventbox2[pos]))
	{
		if (p->pos1[p->pos] != pos)
		{
			p->pos_last1 = p->pos1[p->pos];
			p->pos1[p->pos] = pos;
			p->pos_pos = MENU3_STOP;
			p->pos_last2 = 0xff;
			draw_menu2(0);
			draw_menu3(0, NULL);
		}
		if(pp->help_yn)
		{
			switch(p->pos)
			{
				case 0:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_00);
							break;
						case 1:
							show_help(HELP_Y, LABEL_01);
							break;
						case 2:
							show_help(HELP_Y, LABEL_02);
							break;
						case 3:
							show_help(HELP_Y, LABEL_03);
							break;
						case 4:
							//show_help(HELP_Y, LABEL_04);
							break;
						default :break;
					}
					break;

				case 1:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_10);
							break;
						case 1:
							show_help(HELP_Y, LABEL_11);
							break;
						case 2:
							show_help(HELP_Y, LABEL_12);
							break;
						case 3:
							show_help(HELP_Y, LABEL_13);
							break;
						case 4:
							show_help(HELP_Y, LABEL_14);
							break;
						default :break;
					}
					break;

				case 2:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_20);
							break;
						case 1:
							show_help(HELP_Y, LABEL_21);
							break;
						case 2:
							show_help(HELP_Y, LABEL_22);
							break;
						case 3:
							show_help(HELP_Y, LABEL_23);
							break;
						case 4:
							//show_help(HELP_Y, "");
							break;
						default :break;
					}
					break;

				case 3:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_30);
							break;
						case 1:
							show_help(HELP_Y, LABEL_31);
							break;
						case 2:
							show_help(HELP_Y, LABEL_32);
							break;
						case 3:
							show_help(HELP_Y, LABEL_33);
							break;
						case 4:
							show_help(HELP_Y, LABEL_34);
							break;
						default :break;
					}
					break;

				case 4:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_40);
							break;
						case 1:
							show_help(HELP_Y, LABEL_41);
							break;
						case 2:
							show_help(HELP_Y, LABEL_42);
							break;
						case 3:
							show_help(HELP_Y, LABEL_43);
							break;
						case 4:
							//show_help(HELP_Y, LABEL_44);
							break;
						default :break;
					}
					break;
				case 5:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_50);
							break;
						case 1:
							show_help(HELP_Y, LABEL_51);
							break;
						case 2:
							show_help(HELP_Y, LABEL_52);
							break;
						case 3:
							show_help(HELP_Y, LABEL_53);
							break;
						case 4:
							//show_help(HELP_Y, LABEL_54);
							break;
						default :break;
					}
					break;
				case 6:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_60);
							break;
						case 1:
							show_help(HELP_Y, LABEL_61);
							break;
						case 2:
							show_help(HELP_Y, LABEL_62);
							break;
						case 3:
							show_help(HELP_Y, LABEL_63);
							break;
						case 4:
							show_help(HELP_Y, LABEL_64);
							break;
						default :break;
					}
					break;
				case 7:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_70);
							break;
						case 1:
							show_help(HELP_Y, LABEL_71);
							break;
						case 2:
							show_help(HELP_Y, LABEL_72);
							break;
						case 3:
							show_help(HELP_Y, LABEL_73);
							break;
						case 4:
							show_help(HELP_Y, LABEL_74);
							break;
						default :break;
					}
					break;
				case 8:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_80);
							break;
						case 1:
							show_help(HELP_Y, LABEL_81);
							break;
						case 2:
							show_help(HELP_Y, LABEL_82);
							break;
						case 3:
							show_help(HELP_Y, LABEL_83);
							break;
						case 4:
							show_help(HELP_Y, LABEL_84);
							break;
						default :break;
					}
					break;
				case 9:
					switch(p->pos1[p->pos])
					{
						case 0:
							show_help(HELP_Y, LABEL_90);
							break;
						case 1:
							show_help(HELP_Y, LABEL_91);
							break;
						case 2:
							show_help(HELP_Y, LABEL_92);
							break;
						case 3:
							show_help(HELP_Y, LABEL_93);
							break;
						case 4:
							//show_help(HELP_Y, LABEL_94);
							break;
						default :break;
					}
					break;
				default: break;
			}

		}

		if((p->pos == 0) && (p->pos1[p->pos] == 2))//Calibration
		{
			//	gtk_widget_set_sensitive(p->eventbox2[0],FALSE);
			//	gtk_widget_set_sensitive(p->eventbox2[1],FALSE);
			//	gtk_widget_set_sensitive(p->eventbox2[2],FALSE);
			//	gtk_widget_set_sensitive(p->menubar,FALSE);
			//	pp->clb_flag = 1;
			pp->cstart_qty = 1;
			//	draw_menu3(0, NULL);
		}
	}
}

gboolean eventbox2_function0 (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	guint p = GPOINTER_TO_UINT (data);
	b2_fun0(pp, p);
	return TRUE;
}

static inline void data_process(guchar* data, guint pa)
{
	//(*data > 0) ? (*data)-- : (*data = pa);
	(*data < pa) ? (*data)++ : (*data = 0);
	return ;
}

/*6个数值区域共有的处理函数 */
/* 第一个数值按键 快捷键是F12  */
void b3_fun0(gpointer pt)
{
	DRAW_UI_P p = pp;

	/* 之前的位置 */
	p->pos_last2 = p->pos2[p->pos][p->pos1[p->pos]];
	p->pos2[p->pos][p->pos1[p->pos]] = 0;

	/* 单击处理 */
	switch (p->pos)
	{
		case 0: /* 向导 Wizard */
			switch (p->pos1[0])
			{
				case 0:	/* P000 什么向导 */
					((p->start_qty) > 1) ? (p->start_qty) -- : ((p->start_qty) = 9); break;
				case 1:
					((p->fstart_qty) > 1) ? (p->fstart_qty) -- : ((p->fstart_qty) = 6);break;
				case 2:
					switch(p->ctype_pos)
					{
						case 0:
							((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 4);
							break;
						case 1:
							switch(p->cmode_pos)
							{
								case 0:
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 6);
									break;
								case 1:
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 5);
									break;
								case 2:
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 6);
									break;
								case 3:
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 5);
									break;
								default:break;
							}
							break;
						case 2:
							((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 5);
							break;
						default:break;
					}
					//	if((p->cstart_qty) == 1)
					//			esc_calibration();
					break;
				case 3:((p->wstart_qty) > 1) ? (p->wstart_qty) -- : ((p->wstart_qty) = 4);break;
				default:break;
			}
			break;
		case 1: /* UT Settings*/
			switch (p->pos1[1])
			{
				case 4:
					data_140 (NULL, (gpointer)p);
					break; /* P140 自动80%  */
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 2: 
					set_display_table (pp->p_config, !get_display_table(pp->p_config)); /* P320 */
					break; 
				default:break;
			}
			break;
		case 4:
			switch(p->pos1[4])
			{
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 2: 
					set_probe_fft (pp->p_config, !get_probe_fft(pp->p_config)); /* P520 */
					break; 
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 4: 
					set_auto_focal (pp->p_config, !get_auto_focal(pp->p_config)); /* P640 */
					break; 
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 2: 
					set_report_format_userfield (pp->p_config,!get_report_format_userfield (pp->p_config)); /* P820 */
					break; 
				default:break;
			}
			break;
		default:break;
	}

	/* 处理微调 */
	if ((p->pos_last2 == p->pos2[p->pos][p->pos1[p->pos]]) && 
			(p->pos_pos == MENU3_PRESSED))
	{
		switch (p->pos) 
		{
			case 0: /* Wizard */
				break;
			case 1: /* UT Settings */
				switch (p->pos1[1])
				{
					case 0: data_process (&(p->p_tmp_config->db_reg), 4);			break; /* 100增益 5种步进 */
					case 1: data_process (&(p->p_tmp_config->pulser_reg), 2);		break; /* 110pulser发射 3种步进 */
					case 2: data_process (&(p->p_tmp_config->receiver_reg), 2);		break; /* 120receiver接收 3种步进 */
					case 3: data_process (&(p->p_tmp_config->scanoffset_reg), 2);	break; /* 130 */
					case 4: break; /* 140 自动80%  */
					default:break;
				}
				break;
			case 2: /* Gate/Alarm */
				switch (p->pos1[2])
				{
					case 0: /* 弹出一个菜单选择  */			break; /* 200闸门3种选择  */
					case 1: /* 弹出一个菜单选择  */			break; /* 210 Alarm  */
					case 2: /* 弹出一个菜单选择  */			break; /* 220 Alarm  */
					case 3: break; /* 230 */
					default:break;
				}
				break;
			case 3: /* Display Table */
				switch (p->pos1[3])
				{
					case 0: /* 弹出一个菜单选择  */			break; /* 300 */
					case 1: /* 弹出一个菜单选择  */			break; /* 310  */
					case 2: break; /* 320 display table  */
					case 3: break; /* 330 */
					default:break;
				}
				break;
			case 4:
				break;
			case 5:
				switch (p->pos1[5])
				{
					case 0: break; /*   */
					case 1: data_process (&(p->p_tmp_config->scanoffset_reg), 2);  break; /*510 */
					case 2: break; /*   */
					case 3: break; 
					default:break;
				}
				break;
			case 6:
				switch (p->pos1[6])
				{
					case 0: break; /*   */
					case 1: data_process (&(p->p_tmp_config->min_angle_reg), 2);  break;   /*610  */
					case 2: break; /*620 */
					case 3: data_process (&(p->p_tmp_config->element_qty_reg), 2);break; /*630*/ 
					default:break;
				}
				break;
			case 7:
				switch (p->pos1[7])
				{
					case 0: break; /*   */
					case 1: break; /*710 */
					case 2: data_process (&(p->p_tmp_config->scan_start_reg), 2);  break;   /*720  */
					case 3: break; 
					default:break;
				}
				break;
			case 8:
				switch (p->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*820  */
					case 3: break; 
					default:break;
				}
				break;
			default:break;
		}
		//		g_print ("b0 preslkjaflsdjf\n");
	}
	else
	{
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[0]));
	}
	p->pos_pos = MENU3_PRESSED;
	draw_menu2 (0);
	draw_menu3 (0, p);                          /**/

	return ;
}

void b3_fun1(gpointer p)
{
	gint i;
	guint tmp_data;
	gchar* markup;
	int err;
	pthread_t encoder;

	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 1;
	/*	pp->pos_pos = MENU3_PRESSED;*/

	gint offset,k,step;
	gfloat s, vel, wedge_delay;
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	gint index = offset + TMP(beam_num[grp]);
	gint thread_count = 10;
	gint count = 0;
	wedge_delay = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_WEDGE_DELAY) / 1000.0;
	//	gint BeamNo = pp->p_tmp_config->beam_num[grp];

	GtkWidget* dialog = NULL;
	GtkWindow *win = GTK_WINDOW (pp->window);

	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}

	/* 一次点击处理 */
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0://Group
					((pp->start_qty) < 9) ? (pp->start_qty) ++ : ((pp->start_qty) = 1); break;
				case 1://Focal Law
					((pp->fstart_qty) < 6) ? (pp->fstart_qty) ++ : ((pp->fstart_qty) = 1); 
					if((pp->fstart_qty) == 5)
					{
						//生成聚焦法则
						generate_focallaw(grp);
						send_group_spi (grp);
						send_focal_spi (grp);
					}					
					break;
				case 2://Calibration
					switch(pp->ctype_pos)
					{
						case 0://Encoder
							((pp->cstart_qty) < 4) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
							if((pp->cstart_qty) == 2)
							{
								err = pthread_create(&encoder, NULL, (void*)draw_encoder_value, pp);
								if(err != 0)
									perror("can't create thread: encoder");
							}
							if((pp->cstart_qty) == 3)
							{
								//先让编码器的起点值与origin一致
								TMP_CBA(measure_start) = TMP(measure_data[index][4]);

								markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f mm</span>",
										get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0);
								gtk_label_set_markup (GTK_LABEL (pp->label[7]), markup); 

								while(thread_count)
								{
									thread_count-- ;
									tmp_data = TMP(measure_data[index][4]);
									//	if(tmp_data != TMP_CBA(measure_start))
									//					break;
								}
								//printf("\ntmp_data = %d\n",tmp_data);
								if(tmp_data != TMP_CBA(measure_start))
								{
									pthread_mutex_lock(&qlock);
									pthread_cond_signal(&qready);
									pthread_mutex_unlock(&qlock);
								}
								else
									TMP_CBA(measure_end) = TMP_CBA(measure_start);
							}
							else if((pp->cstart_qty) == 4)
							{
								//调用校准函数cba_encoder()								
								set_enc_resolution (pp->p_config, cba_encoder()*1000,
										get_cur_encoder (pp->p_config));
							}
							else if((pp->cstart_qty) == 1)//Accpet
							{
								pp->p_config->encoder1[get_cur_encoder (pp->p_config)].Resolution = TMP_CBA(resolution);	
								esc_calibration();
							}
							break;
						case 1://Ultrasound
							switch(pp->cmode_pos)
							{
								case 0://Velocity
									((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
									if((pp->cstart_qty) == 2)
									{
										//点击start进入A-
										draw_area_calibration();
									}	
									else if((pp->cstart_qty) == 4)
									{
										//获取闸门1信息
										switch(pp->echotype_pos)
										{
											case 0://Radius
												TMP_CBA(radius1) = pp->radius1 / 1000;
												break;
											case 1://Depth
												TMP_CBA(depth1) =  pp->depth1 / 1000;
												break;
											case 2://Thickness
												TMP_CBA(thickness1) = pp->thickness1 / 1000;
												break;
										}
										TMP_CBA(time_start) = ((TMP(measure_data[index][1])) & 0xfffff);
									}
									else if((pp->cstart_qty) == 5)
									{
										//获取闸门2信息
										switch(pp->echotype_pos)
										{
											case 0://Radius
												TMP_CBA(radius2) = pp->radius2 / 1000;
												break;
											case 1://Depth
												TMP_CBA(depth2) = pp->depth2 / 1000;
												break;
											case 2://Thickness
												TMP_CBA(thickness2) = pp->thickness2 / 1000;
												break;
										}
										TMP_CBA(time_end) = ((TMP(measure_data[index][1])) & 0xfffff);
									}
									else if((pp->cstart_qty) == 6)
									{
										pp->vel = cba_ultrasound_velocity();
									} 
									else if((pp->cstart_qty) == 1)
									{
										if((pp->vel > 635) && (pp->vel<1000000))
										{
											//在此调用声速校准函数->此处校准之后的声速用于Wedge Delay校准
											set_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), 
													GROUP_VELOCITY, 100*pp->vel);
											pp->flag = 1;//当该标志为1时才能进行下面的wedge Delay
											esc_calibration();		
										}
										else
										{
											dialog = gtk_message_dialog_new( win,
													GTK_DIALOG_DESTROY_WITH_PARENT,
													GTK_MESSAGE_ERROR,
													GTK_BUTTONS_CLOSE,
													"velocity is not reasonable \n");
											gtk_dialog_run(GTK_DIALOG(dialog));
											gtk_widget_destroy(dialog);
											pp->cstart_qty = 6 ;
										}
									}
									break;
								case 1://Wedge Delay
									if(!pp->flag)
									{
										dialog = gtk_message_dialog_new( win,
												GTK_DIALOG_DESTROY_WITH_PARENT,
												GTK_MESSAGE_ERROR,
												GTK_BUTTONS_CLOSE,
												"you must calibration velocity first \n");
										gtk_dialog_run(GTK_DIALOG(dialog));
										gtk_widget_destroy(dialog);
									}
									else
									{
										((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
										if((pp->cstart_qty) == 2)
										{
											draw_area_calibration();
											vel = get_group_val (p_grp, GROUP_VELOCITY) / 100000.0 ;
											if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
											{
												if (UNIT_MM == get_unit(pp->p_config))
												{
													pp->gate_start_clb = GROUP_GATE_POS(start)* vel;
													pp->gate_width_clb = GROUP_GATE_POS(width)* vel;
												}
												else  /* 英寸 */
												{
													pp->gate_start_clb = GROUP_GATE_POS(start)* vel * 0.03937;
													pp->gate_width_clb = GROUP_GATE_POS(width)* vel * 0.03937;
												}
											}
											else /* 显示方式为时间 */
											{
												pp->gate_start_clb = GROUP_GATE_POS(start); 
												pp->gate_width_clb = GROUP_GATE_POS(width); 
											}
										}
										else if((pp->cstart_qty) == 1)//Accept
										{
											for (i = 0; i < step; i++)
											{
												vel = (get_group_val (p_grp, GROUP_VELOCITY) / 100.0);// m/s
												s   = (TMP(clb_wedge_data[i]) - wedge_delay - pp->G_delay[i] - get_pw()) * vel / 2000000;//mm
												if(s<0) s = 0;
												if( fabs(TMP_CBA(distance) - s) >  (pp->tolerance / 100.0) )
												{
													if(!dialog)
													{
														dialog = gtk_message_dialog_new( win,
																GTK_DIALOG_DESTROY_WITH_PARENT,
																GTK_MESSAGE_ERROR,
																GTK_BUTTONS_CLOSE,
																"At least one context has no amplitude peak in \ngate A. Wedge delay calibration cannot be performed. ");
														gtk_dialog_run(GTK_DIALOG(dialog));
														gtk_widget_destroy(dialog);
														pp->cstart_qty = 5;
														count = 1;
													}
												}
											}
											if(!count)
											{
												for (i = 0; i < step; i++)
												{
													BEAM_INFO(offset + i, beam_delay) = pp->G_delay[i];
//													printf("clb_beam_delay = %d \n", pp->G_delay[i]);
													TMP(focal_law_all_beam[offset + i]).G_delay			= 
															get_group_val (get_group_by_id (pp->p_config, grp), GROUP_WEDGE_DELAY)
															+ GROUP_VAL_POS (grp, wedge.Probe_delay) + BEAM_INFO(i + offset, beam_delay);//modified by hefan 
												}	
												send_focal_spi(get_current_group(pp->p_config));
												gtk_widget_queue_draw (pp->vboxtable);
												//pp->flag = 0;
												esc_calibration();
											}
										}
									}
									break;
								case 2://Sensitivity
									((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
									if((pp->cstart_qty) == 2)
									{
										//点击start进入A-
										draw_area_calibration();
									}	
									else if((pp->cstart_qty) == 1)//最后一步
									{
										//Accept 所做的事情就是把校准之后值显示在控件上
										for (i = 0; i < step; i++)
										{
											GROUP_VAL(gain_offset[i+offset]) =  pp->tmp_gain_off[ i+offset]; 
										}
										//校准完之后清除包络线
										for (i = 0; i < step; i++)
										{
											TMP(clb_max_data[i]) = TMP(clb_real_data[i]);//第一次需初始化
										}
										esc_calibration();		
									}
									break;
								case 3://TCG
									((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
									break;
								default:break;
							}
							break;
						case 2://Code
							((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
							break;
						default:break;
					}
					break;
				case 3:((pp->wstart_qty) < 4) ? (pp->wstart_qty) ++ : ((pp->wstart_qty) = 1);break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 4: 
					set_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR,
							get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START));
					pp->pos_pos = MENU3_STOP;
					if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
						markup = g_markup_printf_escaped (
								"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
								((int)(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN)) - (int)(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR))) / 100.0, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) / 100.0);
					else
						markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",	get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) / 100.0 );
					gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);

					g_free(markup);
					break; /* Set Ref P141 设置参考增益值*/
				default:break;
			}
			break;

		case 3:
			switch (pp->pos1[3])
			{
				case 2: 
					set_entry_image (pp->p_config, !get_entry_image (pp->p_config));/* P321 */
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 3:
					switch(get_dis_prop_scan(pp->p_config)) 
					{
						case 0:break;
						case 1:break;
						case 2:set_dis_prop_cratio (pp->p_config, !get_dis_prop_cratio (pp->p_config));break;
						case 3:set_dis_prop_sinterpolation (pp->p_config, !get_dis_prop_sinterpolation (pp->p_config));break;
						default:break;
					}
					break;  
					/* P431 */
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 3: 
					set_start_pause (pp->p_config, !get_start_pause (pp->p_config)); /* P731 */
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					set_report_format_probe (pp->p_config, !get_report_format_probe (pp->p_config));/* P821 */
					break;
				case 3: 
					set_report_userfield_enable (pp->p_config, 
							!get_report_userfield_enable (pp->p_config, get_report_userfield_select (pp->p_config)),
							get_report_userfield_select (pp->p_config));
					break;
				default:break;
			}
			break;
		default:break;
	}


	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED) )
	{
		switch (pp->pos) 
		{
			case 0:
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->start_reg), 2); break; /* 101start扫描延时 3种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break; /*111 收发模式Tx/Rx Mode  */
					case 2: /* 弹出一个选择菜单,选择 */ break; /*121 滤波Filter  */
					case 3: data_process (&(pp->p_tmp_config->indexoffset_reg), 2);	break; /* 131 */
					case 4: /* Set Ref. */ break; /*141 Set Ref.参考增益  */
					default:break;
				}
				break;
			case 2: 
				switch (pp->pos1[2])
				{
					case 0: break; 
					case 1: break;
					case 2: break;/* 221 Alarm#*/ 
					case 3: data_process(&(pp->p_tmp_config->mat_atten_reg), 3);break;  /*231 Mat.Attenuatior*/
					case 4: break; 
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break; 
					case 1:
							switch(GROUP_VAL(selection))  
							{
								case 0:
									data_process(&(TMP(per_reference_reg)), 2);break;
								case 1:
								case 2:
								case 3:
									data_process(&(TMP(s_reference_reg)), 3);break;
								case 4:
								case 5:
								case 6:
								case 7:
								case 8:
									data_process(&(TMP(cursors_angle_reg)), 1);break;
								default:break;
							}
							break;	/*p311*/
					case 2: break; /* 321 entry image*/ 
					case 3: data_process(&(pp->p_tmp_config->min_thickness_reg), 3); break;  /*331 */
					case 4: /* Set Ref. */ break; 
					default:break;
				}
				break;
			case 4: 
				switch (pp->pos1[4])
				{
					case 0: break; 
					case 1: break;
					case 2:
							switch(GROUP_VAL(col_select_pos)) 
							{
								case 0:data_process(&(TMP(color_start_reg)), 1 );break;
								case 1:data_process(&(TMP(color_contrast_reg)), 1 );break;
								case 2:data_process(&(TMP(min_reg)), 2);break;
							}
							break; 

					case 3:
							switch(get_dis_prop_scan(pp->p_config)) 
							{
								case 0:break;
								case 1:data_process(&(TMP(compress_reg)), 2 );break;
								case 2:break;
								case 3:break;
							}
							break; 
					default:break;
				}
				break;

			case 5:
				switch (pp->pos1[5])
				{
					case 0: break; 
					case 1: data_process (&(pp->p_tmp_config->indexoffset_reg), 2);  break; /*511 */
					case 2: data_process (&(pp->p_tmp_config->db_reg), 4);  break;     /*521*/
					case 3: data_process (&(pp->p_tmp_config->part_thickness_reg), 3);  break;     /*531*/
					case 4: break; 
					default:break;
				}
				break;

			case 6:
				switch (pp->pos1[6])
				{
					case 0: data_process (&(pp->p_tmp_config->connection_P_reg), 3);  break;  /*601 */
					case 1: data_process (&(pp->p_tmp_config->max_angle_reg), 2);  break;     /*611 */
					case 2:
							if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
								data_process (&(pp->p_tmp_config->positions_reg), 2);
							else if((LAW_VAL(Focal_point_type)==PROJECTION_P)||(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
								data_process (&(pp->p_tmp_config->offsets_reg), 2);
							break; /*621 */
					case 3: data_process (&(TMP(first_element_reg)), 2); break; 
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0: break; /*    */
					case 1: break; /*710 */
					case 2: data_process (&(pp->p_tmp_config->scan_end_reg), 2);  break;   /*721  */
					case 3: break; /*731*/ 
					default:break;
				}
				break;
			case 8:
				switch (pp->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*821  */
					case 3: break;          /*831  */ 
					default:break;
				}
				break;
			case 9:
				switch (pp->pos1[9])
				{
					case 0: data_process (&(pp->p_tmp_config->bright_reg), 2);  break; /*901  */
					case 1: break; /*710 */
					case 2: break;   /*721  */
					case 3: //pp->p_config->ezview = !pp->p_config->ezview;break;  /*931*/
					default:break;
				}
				break;
			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[1]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/
	return ;
}

void b3_fun2(gpointer p)
{
	gchar *markup;
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 2;

	switch (pp->pos)
	{
		case 1:
			switch (pp->pos1[1])
			{
				case 4: 
					set_group_db_ref (pp->p_config, get_current_group(pp->p_config), 
							!get_group_db_ref (pp->p_config, get_current_group(pp->p_config)));
					pp->pos_pos = MENU3_STOP;
					if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
					{
						tt_label_show_string (pp->label[GAIN_LABEL], con2_p[1][0][7], "\n", "(dB)", "white", 10);
						markup = g_markup_printf_escaped (
								"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
								(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR)) / 100.0, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) / 100.0);
					}
					else
					{
						tt_label_show_string (pp->label[GAIN_LABEL], con2_p[1][0][0], "\n", "(dB)", "white", 10);
						markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
								get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) / 100.0 );
					}
					gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);
					g_free(markup);
					break; /* dB Ref P142 开关 */
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					set_overlay_sizing_curves(pp->p_config, !get_overlay_sizing_curves(pp->p_config));
					break; /* p412 */
				case 3: 
					set_dis_prop_boptimum (pp->p_config, !get_dis_prop_boptimum (pp->p_config));
					break; /* p432 */
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					set_report_format_setup (pp->p_config,  !get_report_format_setup (pp->p_config));/* P822 */
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 3:break;
				default:break;
			}
			break;
		default:break;
	}
	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED)) 
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 002 */
						if ( pp->start_qty == 4 )
							data_process(&(TMP(connection_P_reg)), 2);
						if ( pp->start_qty == 7 )
							data_process (&(pp->p_tmp_config->scanoffset_reg), 2);
						break;
					case 1: /* 012 */
						if ( pp->fstart_qty == 3 )
							data_process(&(TMP(element_qty_reg)), 2);
						else if ( pp->fstart_qty == 4 )
							data_process(&(TMP(min_angle_reg)), 2);
						break;
					case 2:	/* 022 */
						if ( pp->cstart_qty == 2 )
						{
							if(pp->ctype_pos == 0)
								data_process(&(TMP(origin_reg)), 2);
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
								data_process(&(TMP(cangle_reg)), 1);
							else if((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
								data_process(&(TMP(ref_amplitude_reg)), 2);
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 3))
								data_process(&(TMP(ref_ampl_reg)), 2);
						}
						else if ( pp->cstart_qty == 3 )
						{
							if(pp->ctype_pos == 0)
								data_process(&(TMP(distance_reg)), 2);
							else if(pp->ctype_pos == 1)
								data_process(&(TMP(first_element_reg)),2);
							else if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
								data_process(&(TMP(cstart_reg)), 3);
						}
						else if ( pp->cstart_qty == 4 )
						{
							if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
								data_process(&(TMP(db_reg)), 4);
							else
								data_process(&(TMP(cstart_reg)), 3);
						}
						else if ( pp->cstart_qty == 5 )
						{
							if((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
								data_process(&(TMP(compdb_reg)), 1);
							else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 3)) )
								data_process(&(TMP(db_reg)), 4);
						}
						else if(pp->cstart_qty == 6)
						{
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
								data_process (&(TMP(db_reg)), 4);
						}
						break;
					case 3:
						if(pp->wstart_qty == 3)
							data_process (&(TMP(weland_height_reg)), 2);
						else if(pp->wstart_qty == 4)
							data_process (&(TMP(fizone_height_reg)), 2);


						break;/*p032*/
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->range_reg), 2); break; /* 102range范围 3种步进 */
					case 1:
						   if (pp->mark_pop_change)
							   data_process(&(TMP(frequency_reg)), 2); /* 112 频率范围 3种步进 */
						   break; /* 112 频率 Freq.  */
					case 2: /* 弹出一个选择菜单,选择 */ break; /* 122 检波 Recitify  */
					case 3:  /* 132 角度 PA 不能更改 UT 可以修改 */  data_process(&(TMP(angle_reg)), 2); break; 
					case 4: break;/* dB Ref P142 开关 */
					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0:data_process(&(pp->p_tmp_config->agate_start_reg), 3); break; /* 202 agate start*/
					case 1:
						   /* 弹出一个选择菜单,选择 */                         break; 
					case 2:data_process(&(pp->p_tmp_config->count_reg), 1); break; /* 222 output count*/
					case 3:
						   if(GROUP_VAL(mode_pos) == 1)
						   {
							   if( GROUP_VAL(curve_pos) == 1|| GROUP_VAL(curve_pos)==3)
								   data_process(&(pp->p_tmp_config->position_reg),3);
							   else if(GROUP_VAL(curve_pos) == 2)
								   data_process(&(pp->p_tmp_config->delay_reg), 3);
						   }
						   else
						   {
							   if(GROUP_VAL(curve_pos) == 1|| GROUP_VAL(curve_pos)==2)
								   data_process(&(pp->p_tmp_config->ref_ampl_reg), 2);
						   }
						   break; /*232 Ref.Amplitude*/

					case 4: break;
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break; 
					case 1:/*312*/
							switch(GROUP_VAL(selection))  
							{
								case 0:
									data_process(&(TMP(per_measure_reg)), 2);break;
								case 1:
								case 2:
								case 3:
									data_process(&(TMP(s_measure_reg)), 3);break;
								case 4:
									data_process(&(TMP(u_reference_reg)), 3);break;
								case 5:
								case 6:
								case 7:
									data_process(&(TMP(cursors_amplitude_reg)), 2);break;
								case 8:
									data_process(&(TMP(cursors_scan_reg)), 3);break;
								default:break;
							}
							break;

					case 2: break;
					case 3: data_process(&(pp->p_tmp_config->max_thickness_reg), 3); break; /*332 */
					case 4: break;
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0: break; 
					case 1: break;  /*412*/
					case 2: 
							if (GROUP_VAL(col_select_pos) == 0)
								data_process(&(TMP(color_end_reg)), 1);  /*422 */
							else if (GROUP_VAL(col_select_pos) == 1)
								data_process(&(TMP(brightness_reg)), 1);
							else if (GROUP_VAL(col_select_pos) == 2)
								data_process(&(TMP(max_reg)), 2);
							break;  /*432 */
					case 3: break;
					default:break;
				}
				break;
			case 5:
				switch (pp->pos1[5])
				{
					case 0: break; 
					case 1: data_process(&(TMP(skew_reg)), 2); break;  /*512*/ 
					case 2: data_process(&(TMP(agate_start_reg)), 3); break;  /*522*/
					case 3: data_process(&(TMP(diameter_reg)), 3); break;  /*532*/
					case 4: break;
					default:break;
				}
				break;
			case 6:
				switch (pp->pos1[6])
				{
					case 0: data_process(&(TMP(connection_R_reg)), 2); break;	/*602*/
					case 1: data_process(&(TMP(angle_step_reg)), 2); break;		/*612*/
					case 2:
							if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
								data_process (&(pp->p_tmp_config->positione_reg), 2);
							else if((LAW_VAL(Focal_point_type)==PROJECTION_P)||(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
								data_process (&(pp->p_tmp_config->offsete_reg), 2);
							break;	/*622 */ 
					case 3: data_process(&(TMP(last_element_reg)), 2); break;  /*632*/
					case 4: break;
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0: break; /*    */
					case 1: break; /*712 */
					case 2: data_process (&(TMP(scan_resolution_reg)), 2);  break;   /*722  */
					case 3: break; 
					default:break;
				}
				break;
			case 8:
				switch (pp->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*822  */
					case 3: break; 
					default:break;
				}
				break;


			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[2]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/
	return ;
}

void b3_fun3(gpointer p)
{
	gint i,clb_step;
	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		clb_step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		clb_step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}
	gint	grp = get_current_group (pp->p_config);
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 3;

	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 2:
					switch(pp->ctype_pos)
					{
						case 0:
							break;
						case 1:
							switch(pp->cmode_pos)
							{
								case 0:
									break;
								case 1:
									if (pp->cstart_qty == 5)//Clear Calibrate
									{
										for (i = 0; i < clb_step; i++)
										{
											TMP(clb_wedge_data[i]) = 0;//(TMP(measure_data[i][1]) & 0xfffff) * 10;
										}
									}
									break;
								case 2:
									if(pp->cstart_qty == 6)
									{
										for (i = 0; i < clb_step; i++)
										{
											//							TMP(clb_real_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;
											TMP(clb_max_data[i]) = TMP(clb_real_data[i]);//第一次需初始化
										}
									}
									break;
								case 3:
									break;
							}
							break;
						case 2:
							break;
					}
			}
		case 1:
			switch (pp->pos1[1])
			{
				case 2: 
					set_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER, 
							!get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER));
					TMP(group_spi[grp]).video_filter	= 
						get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER);
					g_print ("videofilter=%d\n",
							get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER));
					send_spi_data (grp);
					/* 视频滤波 P123 */
					break; 
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					set_overlay_gate(pp->p_config, !get_overlay_gate(pp->p_config));
					break; /* p413 */
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 4:
					/* 计算聚焦法则 P643 */
					generate_focallaw(get_current_group (pp->p_config));
					send_group_spi (get_current_group (pp->p_config));
					send_focal_spi (get_current_group (pp->p_config));
					break;  
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					set_report_format_note (pp->p_config, !get_report_format_note (pp->p_config));/* P823 */
					break;
				default:break;
			}
			break;
		default:break;
	}

	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 003 */
						if ( pp->start_qty == 7 )
							data_process (&(TMP(indexoffset_reg)), 2);
						break;
					case 1: /* 013 */
						if ( pp->fstart_qty == 3 )
							data_process(&(TMP(first_element_reg)), 2);
						else if ( pp->fstart_qty == 4 )
							data_process (&(TMP(max_angle_reg)), 2);
						break;
					case 2:	/* 023 */
						switch(pp->cstart_qty)
						{
							case 2:
								if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
								{
									if(pp->echotype_pos == 0)
									{
										data_process (&(TMP(radiusa_reg)), 3);
									}
									else if(pp->echotype_pos == 1)
									{
										data_process (&(TMP(deptha_reg)), 3);
									}
									else
									{
										data_process (&(TMP(thickness1_reg)), 3);
									}
								}
								else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
									data_process (&(TMP(tolerance_t_reg)), 2);

								else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
								{
									data_process (&(TMP(db_reg)), 4);
								}

								break;
							case 3:
								if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
								{
									if(pp->echotype_pos == 0)
									{
										data_process (&(TMP(radius1_reg)), 3);
									}
									else if(pp->echotype_pos == 1)
									{
										data_process (&(TMP(depth1_reg)), 3);
									}
									else
									{
										data_process (&(TMP(thickness1_reg)), 3);
									}
								}
								else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
								{
									if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
										data_process (&(TMP(last_angle_reg)), 2);
									else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
										data_process(&(TMP(last_element_reg)),2);
								}
								else if ((pp->ctype_pos == 2) && (pp->scode_pos == 1))
								{
									data_process (&(TMP(cwidth_reg)), 3);
								}
								break;
							case 4:
								data_process(&(TMP(cwidth_reg)), 3);
								break;
							default: break;
						}
						break;
					case 3:
						if ( pp->wstart_qty == 3 )
							data_process (&(TMP(weland_offset_reg)), 2);
						else if(pp->wstart_qty == 4)
							data_process (&(TMP(fizone_angle_reg)), 2);
						break;/*p033*/
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->wedge_delay_reg), 2); break;   /* 103wedge delay 楔款延时 3种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break; 
					case 2: 
														break;   /* 123 视频滤波 on or off  */
					case 3:break; /* 133 Skew (deg)  */
					case 4:
						   if (pp->mark_pop_change)
							   data_process(&(pp->p_tmp_config->point_qty_reg), 2); /* 143 Points Qty. 3种步进 */
						   break; /* 143 Points Qty.  */

					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0:data_process(&(pp->p_tmp_config->gate_width_reg), 3); break; /* 203 agate width 3种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break;
					case 2: /* 视频滤波 */ break; /* 123 视频滤波 on or off  */
					case 3: 
										   if( GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2|| GROUP_VAL(curve_pos)==3 )
										   {
											   if((GROUP_VAL(mode_pos)==0 )&&(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2))
												   data_process(&(pp->p_tmp_config->ref_ampl_offset_reg), 4); /* 233 ref.ampl.offset  */
											   else if((GROUP_VAL(mode_pos)==1 )&&(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2))
												   data_process(&(pp->p_tmp_config->amplitude_reg), 3);
											   else if((GROUP_VAL(mode_pos)==1 )&&(GROUP_VAL(curve_pos)==3))
												   data_process(&(pp->p_tmp_config->tcg_gain_reg), 4);
										   }
										   break; /*233 Ref.Amplitude*/

					case 4: /* 弹出一个选择菜单,选择 */ break;
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break;
					case 1: /* 313 */
							switch(GROUP_VAL(selection))  
							{
								case 0:
								case 1:
									data_process(&(TMP(u_reference_reg)), 3);break;
								case 4:
									data_process(&(TMP(u_measure_reg)), 3);break;
								case 2:
									data_process(&(TMP(i_reference_reg)), 3);break;
								case 3:
									data_process(&(TMP(s_refmeas_reg)), 3);break;
								case 5:
								case 6:
								case 7:
									data_process(&(TMP(cursors_ut_reg)), 3);break;
								case 8:
									data_process(&(TMP(cursors_index_reg)), 3);break;
								default:break;
							}
							break;

					case 2: /* 视频滤波 */ break; 
					case 3: data_process(&(TMP(echo_qty_reg)), 1); break;  /*333 */
					case 4: /* 弹出一个选择菜单,选择 */ break; 
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0: break;
					case 1: break; /*413*/
					case 2: break; /*423*/
					case 3: break;  
					case 4: break; 
					default:break;
				}
				break;

			case 5:
				switch (pp->pos1[5])
				{
					case 0: break;
					case 1: break;
					case 2: data_process(&(pp->p_tmp_config->gate_width_reg), 3);  break; /* 523 */
					case 3: break;  
					case 4: break; 
					default:break;
				}
				break;

			case 6:
				switch (pp->pos1[6])
				{
					case 0: break;
					case 1: data_process(&TMP(beam_skew_min_angle_reg), 2);  break; /*613 */
					case 2: 
							if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
								data_process (&(pp->p_tmp_config->positionstep_reg), 2);
							else if((LAW_VAL(Focal_point_type)==PROJECTION_P)||(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
								data_process (&(pp->p_tmp_config->depths_reg), 2);
							break;	/*623 */
					case 3:  data_process(&TMP(element_step_reg), 2);break;  
					case 4: break; 
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0: data_process(&(pp->p_tmp_config->encoder_resolution_reg), 2);break;
					case 1: data_process(&(pp->p_tmp_config->scanspeed_reg), 2);  break; 
					case 2: data_process(&(pp->p_tmp_config->index_start_reg), 2);  break; 
					case 3: break;  
					case 4: break; 
					default:break;
				}
				break;
			case 8:
				switch (pp->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*823  */
					case 3: break; 
					default:break;
				}
				break;
			case 9:
				switch (pp->pos1[9])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*923  */
					case 3: break;   /*933  */ 
					default:break;
				}
				break;

			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[3]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/
	return ;
}

void b3_fun4(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 4;
	gint offset,k,step;
	gint grp = get_current_group(pp->p_config);//当前group
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	//	gint index = offset + TMP(beam_num[grp]);	
	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}

	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:	/* p004 */
					if ( pp->start_qty == 5 )
						set_auto_detect (pp->p_config, !get_auto_detect(pp->p_config));
					break;
				case 1:break;
				case 2:/*p024*/
					   //在此调用校准函数
					   switch(pp->cmode_pos)
					   {
						   case 0:
							   break;
						   case 1://wedge delay
							   if (pp->cstart_qty == 5)//Calibrate
								   cba_ultrasound_wedgedelay();
							   break;
						   case 2://sensitivity
							   if (pp->cstart_qty == 6)//Calibrate
							   {
								   cba_ultrasound_sensitivity();
							   }
							   break;
						   case 3:
							   break;
					   }
					   break;
				case 3:break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					set_overlay_cursor (pp->p_config, !get_overlay_cursor (pp->p_config));
					gtk_widget_queue_draw (pp->vboxtable);
					break; /* p414 */
				default:break;
			}
			break;
		default:break;
	}

	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 004 */
						if ( pp->start_qty == 7 )
							data_process(&(TMP(skew_reg)), 2); 
						break;
					case 1: /* 014 */
						if ( pp->fstart_qty == 3 )
							data_process(&(TMP(last_element_reg)), 2);
						else if ( pp->fstart_qty == 4 )
							data_process(&(TMP(angle_step_reg)), 2);
						break;
					case 2: /* 024 */
						if(pp->cstart_qty == 2)
						{
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{
								data_process (&(TMP(tolerance_reg)), 3);
							}
						}

						else if(pp->cstart_qty == 3)
						{
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								if(pp->echotype_pos == 0)
								{
									data_process (&(TMP(radius2_reg)), 3);
								}
								else if(pp->echotype_pos == 1)
								{
									data_process (&(TMP(depth2_reg)), 3);
								}
								else
								{
									data_process (&(TMP(thickness2_reg)), 3);
								}
							}
							else if(pp->ctype_pos == 2)
							{
								data_process (&(TMP(cheight_reg)), 1);
							}
						}
						else if(pp->cstart_qty == 4)
						{
							if(pp->ctype_pos == 1)
								data_process (&(TMP(cheight_reg)), 1);
							else if(pp->ctype_pos == 2)
								data_process (&(TMP(thickness_reg)), 3);
						}
						else if(pp->cstart_qty == 5)
						{
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
								data_process (&(TMP(db_reg)), 4);
						}

						break;
					case 3: data_process (&(TMP(fizone_radius_reg)), 2);break;
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->velocity_reg), 3); break; /* 104velocity声速  3种步进 */
					case 1:
						   if (pp->mark_pop_change)
							   data_process(&(pp->p_tmp_config->pulser_width_reg), 2); /* 114 脉宽范围 3种步进 */
						   break; /* 114 脉冲宽度 PW   */
					case 2: /* 弹出一个选择菜单,选择 */ break; /* 124 平均 averaging  */
					case 3:data_process(&(pp->p_tmp_config->beam_delay_reg), 2); break; /* 134 beamdelay 波束延时 */
					case 4: /*  */ break; /* 144 Scale Factor 多少点压缩一点 只能看  */
					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0:data_process(&(pp->p_tmp_config->agate_height_reg), 1); break; /* 204 threshold  2种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break;
					case 2: data_process(&(pp->p_tmp_config->active_delay_reg), 3); break; /*224 Delay*/
					case 3: data_process(&(pp->p_tmp_config->curve_step_reg), 2);  break; /* 234  */
					case 4: /*  */ break; /* 244 Scale Factor 多少点压缩一点 只能看  */
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break;
					case 1:  /* 314 */ 
							switch(GROUP_VAL(selection))  
							{
								case 0:
								case 1:
									data_process(&(TMP(u_measure_reg)), 3);break;
								case 4:
									data_process(&(TMP(i_reference_reg)), 3);break;
								case 2:
									data_process(&(TMP(i_measure_reg)), 3);break;
								case 5:
								case 6:
								case 7:
									data_process(&(TMP(cursors_scan_reg)), 3);break;
								default:break;
							}
							break;

					case 2: data_process(&(pp->p_tmp_config->entry_reg), 1); break;  /*324 */
					case 3: break;  /*334 */
					case 4: break; 
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0:
						if( get_display_pos(pp->p_config)==10) 
							data_process(&(TMP(dis_range_reg)), 2);
						break; /*404*/
					case 1:break; /*414*/
					case 2:break; /*424*/
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			case 6:
				switch (pp->pos1[6])
				{
					case 0:break;
					case 1: data_process(&TMP(beam_skew_max_angle_reg), 2);  break; /*614 */
					case 2: data_process(&TMP(depthe_reg), 2);  break;
					case 3:break; 
					case 4:break; 
					default:break;
				}
			case 7:
				switch (pp->pos1[7])
				{
					case 0:data_process(&(pp->p_tmp_config->origin_reg), 2);break; /* 704 */
					case 1:data_process(&(pp->p_tmp_config->scanspeed_rpm_reg), 2);break;  /* 714 */
					case 2:data_process(&(pp->p_tmp_config->index_end_reg), 2);  break; /* 724 */
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;

			default:break;

		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[4]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/

	return ;
}

void b3_fun5(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 5;
	gint clb_step;
	gint offset,k,i;
	gint grp = get_current_group(pp->p_config);//当前group
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);

	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		clb_step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		clb_step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}

	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 2:
					switch(pp->ctype_pos)
					{
						case 0:
							if((pp->cstart_qty) == 4)
							{
								(pp->cstart_qty) = 2;
							}
							break;
						case 1:
							switch(pp->cmode_pos)
							{
								case 0:
									if((pp->cstart_qty) == 6)
									{
										(pp->cstart_qty) = 2;
										pp->vel = 0;
									}
									break;
								case 1:
									if(pp->cstart_qty == 5)//Restart
									{
										pp->cstart_qty = 2;
										for (i = 0; i < clb_step; i++)
										{
											TMP(clb_real_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;
											TMP(clb_max_data[i]) = TMP(clb_real_data[i]);//第一次需初始化
										}
									}
									break;
								case 2:
									if (pp->cstart_qty == 1)//Clear Calibrate
									{
										for (i = 0; i < clb_step; i++)
										{
											TMP(clb_real_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;
											TMP(clb_max_data[i]) = TMP(clb_real_data[i]);//第一次需初始化
											TMP(clb_his_max_data) = 0;
											GROUP_VAL(gain_offset[i+offset]) =  0;
											pp->tmp_gain_off[i+offset] = 0; 
										}
									}
									else if(pp->cstart_qty == 5)//Clear env
									{
										for (i = 0; i < clb_step; i++)
										{
											TMP(clb_real_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;
											TMP(clb_max_data[i]) = TMP(clb_real_data[i]);//第一次需初始化
										}
									}
									else if(pp->cstart_qty == 6)//Restart
									{
										pp->cstart_qty = 2;
										for (i = 0; i < clb_step; i++)
										{
											TMP(clb_real_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;
											TMP(clb_max_data[i]) = TMP(clb_real_data[i]);//第一次需初始化
										}
									}
									break;
								case 3:
									break;
							}
							break;
						case 2:
							break;
						case 3:
							break;
					}
					break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					set_overlay_overlay (pp->p_config, !get_overlay_overlay (pp->p_config));
					gtk_widget_queue_draw (pp->vboxtable);
					break; /* p415 */
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0: 
					set_auto_detect (pp->p_config, !get_auto_detect(pp->p_config));
					break; /* p505 */
				default:break;
			}
			break;
		default:break;
	}

	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 005 */
						break;
					case 1: /* 015 */
						if ( pp->fstart_qty == 3 )
							data_process(&(TMP(element_step_reg)), 2);
						break;
					case 2: /* 025 */
						if(pp->cstart_qty == 4)
						{
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{
								if(pp->echotype_pos == 0)
								{
									data_process (&(TMP(radiusa_reg)), 3);
								}
								else if(pp->echotype_pos == 1)
								{
									data_process (&(TMP(deptha_reg)), 3);
								}
								else
								{
									data_process (&(TMP(thickness1_reg)), 3);
								}
							}

							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								if(pp->echotype_pos == 0)
								{
									data_process (&(TMP(radius1_reg)), 3);
								}
								else if(pp->echotype_pos == 1)
								{
									data_process (&(TMP(depth1_reg)), 3);
								}
								else
								{
									data_process (&(TMP(thickness1_reg)), 3);
								}
							}
						}

						break;
					case 3:break;
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0: break; /* 105 空 */
					case 1: 
							if (pp->mark_pop_change)
								data_process(&(pp->p_tmp_config->prf_reg), 2); /* 115 PRF重复频率 3种步进 */
							break; /* 115 PRF重复频率  */

					case 2:data_process(&(pp->p_tmp_config->reject_reg), 2); break; /* 125 Reject 抑制  */
					case 3:data_process(&(pp->p_tmp_config->gain_offset_reg), 4); break; /* 135 Gain Offset  */
					case 4: 
						   if (pp->mark_pop_change)
							   data_process(&(pp->p_tmp_config->sum_gain_reg), 4); /* 145 Sum Gain  5种步进 */
						   break; /* 145 Sum Gain   */

					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0: break; 
					case 1: break;
					case 2: data_process(&(pp->p_tmp_config->holdtime_reg), 3); break; 
					case 3: data_process(&(pp->p_tmp_config->ref_gain_reg), 4); break; 
					case 4: break; 
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break; 
					case 1: /* P315 */ 
							switch(GROUP_VAL(selection))  
							{
								case 4:
									data_process(&(TMP(i_measure_reg)), 3);break;
								case 5:
								case 6:
								case 7:
									data_process(&(TMP(cursors_index_reg)), 3);break;
								default:break;
							}
							break;
					case 2: break; 
					case 3: break; 
					case 4: break; 
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0:break; /*405*/
					case 1:break; /*415*/
					case 2:break; /*425*/
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			case 5:
				switch (pp->pos1[5])
				{
					case 0:break;  /*505*/
					case 1:break; /*515*/
					case 2:break;
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			case 6:
				switch (pp->pos1[6])
				{
					case 0:break;
					case 1: data_process(&TMP(beam_skew_angle_step_reg), 2);  break; /*615*/
					case 2:break;
					case 3:break; 
					case 4:break; 
					default:break;
				}
			case 7:
				switch (pp->pos1[7])
				{
					case 0:break; 
					case 1:data_process(&(pp->p_tmp_config->indexspeed_reg), 2);break;  /* 715 */
					case 2:data_process(&(pp->p_tmp_config->index_resolution_reg), 2);  break; /*725*/
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[5]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/

	return ;
}





static gint keypress_event_main_bak(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	return window_keypress_event_orig (widget, event); 
}

static gint keypress_event_main(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("main press\n");
	return handler_key(event->keyval, NULL);
}

static gint keypress_event_main_spinbutton(GtkWidget *widget, GdkEventKey *event)	
{
	g_print("main spinbutton press\n");
	guint i, keyval = event->keyval;

	for ( i = sizeof (key_fast_map) / sizeof (guint) ; i; i--)
	{
		if (keyval == key_fast_map[i - 1])
		{
			return handler_key(keyval, NULL);
		}
	}
	if (!i)
	{
		g_print("main spinbutton press 1234567890\n");
		return window_keypress_event_orig (widget, event); 
	}

	return 0;
}

static gint keypress_event_main_entry(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("main entry press\n");
	return 0;
}

static gint keypress_event_dialog(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("dialog press\n");
	guint keyval = event->keyval;

	switch (keyval)
	{
		case GDK_Return:
			gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_OK);
			return 0;
			break;
		case GDK_Escape:
			gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);
			return 0;
			break;
		case GDK_F12:
			if((pp->pos==6)&&(pp->pos1[6]==4)&&((pp->pos2[6][4]==1)||(pp->pos2[6][4]==2))&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_OK);

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CHANGE_FOCAL);

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CHANGE_FOCAL);			

			if((pp->pos==6)&&(pp->pos1[pp->pos]==4)&&(pp->pos2[6][4]==2)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LAW_SAVE);			

			if((pp->pos==6)&&(pp->pos1[pp->pos]==4)&&(pp->pos2[6][4]==1)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LAW_READ);			
			return 0;
			break;
		case GDK_F11:
			if((pp->pos==6)&&(pp->pos1[6]==4)&&((pp->pos2[6][4]==1)||(pp->pos2[6][4]==2))&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);

			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);

			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==2)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);
			}

			return 0;
			break;
		case GDK_Super_L:
			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_FILE_OPEN);
			}

			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==2)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CONFIG_SAVE);
			}

			return 0;
			break;

		case GDK_F2:

			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==2)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CONFIG_FILE_FOCAL);
			}

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SELECT_ALL);
			}

			return 0;
			break;

		case GDK_F3:

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_COPY_FILE);
			}

			return 0;
			break;

		case GDK_F4:

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_MOVE_FILE);
			}

			return 0;
			break;

		case GDK_F6:

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==0)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SAVE_AND_CLOSE);
			}

			if((pp->pos==8)&&(pp->pos1[pp->pos]==1)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==3)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SAVE_AND_CLOSE);
			}

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_OK );

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget),GTK_RESPONSE_OK );	

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_RENAME_FILE);
			}

			return 0;
			break;

		case GDK_F1:

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==0)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CLOSE);
			}

			if((pp->pos==8)&&(pp->pos1[pp->pos]==1)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==3)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CLOSE);
			}

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL );

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget),GTK_RESPONSE_CANCEL );	

			return 0;
			break;

		case GDK_F5:

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SELECT_AND_CLOSE);

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SELECT_AND_CLOSE);	

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_DELECT_FILE);
			}

			return 0;
			break;
		default:break;
	}

	return dialog_keypress_event_orig (widget, event); 
	return 0;
}

static gint keypress_event_dialog_spinbutton(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("dialog spinbutton press\n");

	return 0;
}

static gint keypress_event_dialog_entry(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("dialog entry press\n");
	return 0;
}

static gint keypress_event_warning(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("warning press\n");
	return 0;
}

/* 修改窗口的按键处理程序 */
void change_keypress_event(gint window_type)
{
	switch (window_type)
	{
		case KEYPRESS_MAIN:
			widget_window_class->key_press_event = keypress_event_main;
			break;
		case KEYPRESS_MAIN_SPINBUTTON:
			widget_window_class->key_press_event = keypress_event_main_spinbutton;
			break;
		case KEYPRESS_MAIN_ENTRY:
			widget_window_class->key_press_event = keypress_event_main_entry;
			break;
		case KEYPRESS_DIALOG:
			widget_window_class->key_press_event = keypress_event_main_bak;
			dialog_window_class->key_press_event = keypress_event_dialog;
			break;
		case KEYPRESS_DIALOG_SPINBUTTON:
			dialog_window_class->key_press_event = keypress_event_dialog_spinbutton;
			break;
		case KEYPRESS_DIALOG_ENTRY:
			dialog_window_class->key_press_event = keypress_event_dialog_entry;
			break;
		case KEYPRESS_WARNING:
			widget_window_class->key_press_event = keypress_event_warning;
			break;
		case KEYPRESS_MENUITEM:
			break;
		default:break;
	}
}

static int handler_key(guint keyval, gpointer data)
{
	guchar tmp = pp->pos_pos;
	guchar tmp1 = pp->mark_pop_change;
	guint	data1 = (GPOINTER_TO_UINT (data));
	guint offset,k;
	guint group = get_current_group(pp->p_config);

	//	double current_angle ;
	//	double max_angle     ;
	int BeamNo ;
	float current_angle;

	switch (keyval) 
	{
		case GDK_KP_0:	/* 选中 P310 cursors 这个位置 */
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				pp->pos = 3;
				pp->pos1[pp->pos] = 1;
				CUR_POS = 0;
				pp->pos_pos = MENU3_PRESSED;
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			break;

		case GDK_KP_1:	/* 冻结 */
			if(!pp->help_yn)
			{
				TMP(freeze) = !TMP(freeze);
				if(TMP(freeze))
					//gtk_widget_modify_fg(pp->event[18], GTK_STATE_NORMAL, &color_black);
					update_widget_bg(pp->event[18], /*backpic[12]*/ 16);
				else
					update_widget_bg(pp->event[18], /*backpic[12]*/ 15);
			}
			break;

		case GDK_KP_2:	/* 选中 Calibration 这个位置 */
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				pp->pos = 0;
				pp->pos1[pp->pos] = 2;
				//CUR_POS = 0;
				pp->pos_pos = MENU2_PRESSED;
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			break;

		case GDK_KP_3:	/* 选中 P100 Gain 这个位置 */
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
				{
					menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
				}
				pp->pos = 1;
				pp->pos1[pp->pos] = 0;
				CUR_POS = 0;
				pp->pos_pos = MENU3_PRESSED;
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			break;

		case GDK_KP_4:	/* 选中 P801 Open 这个位置 */
			if(!pp->help_yn)
			{
				pp->pos = 8;
				pp->pos1[pp->pos] = 0;
				CUR_POS = 1;
				pp->pos_pos = MENU3_PRESSED;
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			break;

		case GDK_KP_5:	/* 选中 P200 Gate 这个位置 */
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				pp->pos = 2;
				pp->pos1[pp->pos] = 0;
				CUR_POS = 0;
				pp->pos_pos = MENU3_PRESSED;
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			break;

		case GDK_KP_6:	/* 开始／停止 */			
			break;

		case GDK_KP_7:	/* 保存 */			
			break;

		case GDK_KP_8:	/* 选中 P140 Set 80% 这个位置 */
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				pp->pos = 1;
				pp->pos1[pp->pos] = 4;
				CUR_POS = 0;
				pp->pos_pos = MENU3_PRESSED;
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			data_140 (NULL, (gpointer)pp);
			break;

		case GDK_KP_9:	/* 选中 P400 Display 这个位置 */
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				pp->pos = 4;
				pp->pos1[pp->pos] = 0;
				CUR_POS = 0;
				pp->pos_pos = MENU3_PRESSED;
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			break;

		case GDK_KP_Subtract:  /* +/- */
			if(!pp->help_yn)
			{
				if(!pp->clb_flag)
				{
					(TMP(beam_num[group]) < (TMP(beam_qty[group]) - 1))	?
						(TMP(beam_num[group]) += 1)	: (TMP(beam_num[group]) = 0);

					for (offset = 0, k = 0 ; k < group; k++)
						offset += TMP(beam_qty[k]);
					BEAM_INFO(offset + TMP(beam_num[group]),beam_delay) = pp->G_delay[ TMP(beam_num[group]) ];
					GROUP_VAL(gain_offset[TMP(beam_num[group]) + offset]) = 
						pp->tmp_gain_off[TMP(beam_num[group]) + offset];

					BeamNo = pp->p_tmp_config->beam_num[group];
					update_gate_info();
					send_focal_spi (group);
					send_group_spi (group);
					if(LAW_VAL(Focal_type) == 0)
					{
						current_angle = LAW_VAL(Angle_min)/100.0 + BeamNo * LAW_VAL(Angle_step)/100.0 ;
					}
					else
					{
						current_angle = LAW_VAL(Angle_min)/100.0 ;
					}
					TMP(current_angle[group]) = current_angle * G_PI / 180.0 ;

					draw_menu3(0, NULL);
					if(!pp->clb_flag)
						draw_area_all();
					else
						draw_area_calibration();
//					TMP(focal_law_all_beam[offset + BeamNo]).G_delay			= 
//						get_group_val (get_group_by_id (pp->p_config, group), GROUP_WEDGE_DELAY)
//						+	GROUP_VAL_POS (group, wedge.Probe_delay) + BEAM_INFO(BeamNo + offset, beam_delay);//modified by hefan 
//					gtk_widget_queue_draw (pp->vboxtable);
				}
			}
			break;

		case GDK_KP_Divide:	/* 擦除 */			
			break;

		case GDK_Super_L:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_CHANGE);/*被选中的主菜单轮流切换*/
			}
			else/*当主菜单条处于收回状态时*/
			{
				menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
				main_menu_pop(MENU_POP);/*弹出主菜单条并选中当前选项*/
				if ((tmp != pp->pos_pos) || (tmp1 != pp->mark_pop_change))
				{
					draw_menu2(0);
					draw_menu3(0, NULL);
				}
			}
			break;

		case GDK_Escape:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
			{
				menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
			}
			/*if(PROBE_DIALOG_KEYPRESS == data1)
			  {
			  pp->response_id = GTK_RESPONSE_CANCEL;
			  }*/
			else/*当主菜单条处于收回状态时*/
			{
				if ( pp->mark_pop_change == 1)
					pp->mark_pop_change = 0;
				switch (pp->pos_pos)
				{
					case MENU2_STOP:
						break;
					case MENU2_PRESSED:
						pp->pos_pos = MENU2_STOP;
						break;
					case MENU3_STOP:
						pp->pos_pos = MENU2_STOP;
						break;
					case MENU3_PRESSED:
						gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[CUR_POS]), tttmp);
						pp->pos_pos = MENU3_STOP;
						break;
					default:break;
				}
				if ((tmp != pp->pos_pos) || (tmp1 != pp->mark_pop_change))
				{
					draw_menu2(0);
					draw_menu3(0, NULL);
				}
			}

			if((pp->pos==0)&&(pp->pos1[pp->pos]==0))
			{
				if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
				{
					menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
				}
				else
				{
					pp->pos1[pp->pos]=0;
					pp->start_qty = 0;
					gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
					gtk_widget_set_sensitive(pp->menubar,TRUE);
					if(GROUP_VAL(group_mode)==PA_SCAN)
						gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
					else
						gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);

					draw_menu3(0, NULL);
				}
			}
			else if((pp->pos==0)&&(pp->pos1[pp->pos]==1))
			{
				if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
				{
					menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
				}
				else
				{
					pp->pos1[pp->pos]=1;
					pp->fstart_qty = 0;
					gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
					gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
					gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
					gtk_widget_set_sensitive(pp->menubar,TRUE);

					draw_menu3(0, NULL);
				}
			}
			else if((pp->pos==0)&&(pp->pos1[pp->pos]==3))
			{
				if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
				{
					menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
				}
				else
				{
					pp->pos1[pp->pos]=3;
					pp->wstart_qty = 1;
					gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
					gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
					gtk_widget_set_sensitive(pp->menubar,TRUE);
					if(GROUP_VAL(group_mode)==PA_SCAN)
						gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
					else
						gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);

					draw_menu3(0, NULL);
				}
			}
			if(pp->clb_flag)
			{
				esc_calibration();
				draw_menu3(0, NULL);
			}
			break;
		case GDK_Return:	/*回车键*/

			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_ENTER);/*选中当前主菜单，并收回主菜单条*/
			}
			else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
			{
				menu3_pop(MENU3_ENTER);/*选中当前三级菜单，并收回三级菜单条*/
			}
			else/*当主菜单条 三级菜单条 都处于收回状态时*/
			{
				switch (pp->pos_pos)
				{
					case MENU2_STOP:
						pp->pos_pos = MENU2_PRESSED;
						pp->pos_pos = MENU3_STOP;
						switch (pp->pos1[pp->pos])
						{
							case 0:
								b2_fun0(pp,0);
								break;
							case 1:
								b2_fun0(pp,1);
								break;
							case 2:
								b2_fun0(pp,2);
								break;
							case 3:
								b2_fun0(pp,3);
								break;
							case 4:
								b2_fun0(pp,4);
								break;
							default:break;
						}
						break;
					case MENU2_PRESSED:
						pp->pos_pos = MENU3_STOP;
						break;
					case MENU3_STOP:
						if(!pp->help_yn)
						{
							if(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS]))
								pp->pos_pos = MENU3_PRESSED;
							else
								pp->pos_pos = MENU3_STOP;
							/* 按下的动作在这里实现 */
						}
						break;
					case MENU3_PRESSED:
						if(!pp->help_yn)
						{
							pp->pos_pos = MENU3_STOP;
							switch (CUR_POS)
							{
							    request_refresh() ;
								case 0:
									b3_fun0(NULL);
									break;
								case 1:
									b3_fun1(NULL);
									break;
								case 2:
									b3_fun2(NULL);
									break;
								case 3:
									b3_fun3(NULL);
									break;
								case 4:
									b3_fun4(NULL);
									break;
								case 5:
									b3_fun5(NULL);
									break;
								default:break;
							}
							pp->pos_pos = MENU3_STOP;
						}
						break;
					default:break;
				}
				if ((tmp != pp->pos_pos) || (tmp1 != pp->mark_pop_change))
				{
					draw_menu2(0);
					draw_menu3(0, NULL);
					tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[CUR_POS]));
				}
			}

			break;

		case GDK_F1:
			if(pp->help_yn == 1)
				show_help(HELP_N, 0);/*隐藏帮助窗口*/
			else if(pp->help_yn == 0)
				show_help(HELP_Y, 0);/*弹出帮助窗口*/
			pp->help_yn = !pp->help_yn;
			break;
		case GDK_F2:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[0]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos == MENU2_PRESSED)
					;
				else 
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,0);
			}			
			break;
		case GDK_F3:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[1]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos == MENU2_PRESSED)
					;
				else 
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,1);
			}		
			break;
		case GDK_F4:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[2]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos == MENU2_PRESSED)
					;
				else 
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,2);
			}		
			break;
		case GDK_F5:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[3]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos == MENU2_PRESSED)
					;
				else 
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,3);
			}			
			break;
		case GDK_F6:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[4]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos == MENU2_PRESSED)
					;
				else 
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,4);
			}			
			break;
		case GDK_F7:
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				if(gtk_widget_get_sensitive(pp->eventbox30[5]))
				{
					if (MENU36_PRESS == data1)
						menu3_pop(MENU3_TURN);
					else
					{
						menu3_pop(MENU3_HIDE); 
						b3_fun5(NULL);
					}
				}
			}			
			break;
		case GDK_F8:
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				if(gtk_widget_get_sensitive(pp->eventbox30[4]))
				{
					if (MENU35_PRESS == data1)
						menu3_pop(MENU3_TURN);
					else
					{
						menu3_pop(MENU3_HIDE); 
						b3_fun4(NULL);
					}
				}
			}			
			break;
		case GDK_F9:
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				if(gtk_widget_get_sensitive(pp->eventbox30[3]))
				{
					if (MENU34_PRESS == data1)
						menu3_pop(MENU3_TURN);
					else
					{
						menu3_pop(MENU3_HIDE); 
						b3_fun3(NULL);
					}
				}
			}			
			break;
		case GDK_F10:
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				if(gtk_widget_get_sensitive(pp->eventbox30[2]))
				{
					if (MENU33_PRESS == data1)	
						menu3_pop(MENU3_TURN);
					else
					{
						menu3_pop(MENU3_HIDE); 
						b3_fun2(NULL);
					}	
				}
			}		
			break;
		case GDK_F11:
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				if(gtk_widget_get_sensitive(pp->eventbox30[1]))
				{
					if (MENU32_PRESS == data1)
						menu3_pop(MENU3_TURN);
					else
					{
						menu3_pop(MENU3_HIDE); 
						b3_fun1(NULL);
					}
				}	
			}		
			break;
		case GDK_F12:
			if(!pp->help_yn)
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				if(gtk_widget_get_sensitive(pp->eventbox30[0]))
				{
					if (MENU31_PRESS == data1)
						menu3_pop(MENU3_TURN);
					else
					{
						menu3_pop(MENU3_HIDE); 
						b3_fun0(pp);
					}
				}
			}
			break;

		case GDK_Left:
		case GDK_Up:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_UP);/*被选中的主菜单向上切换*/
			}
			else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
			{
				menu3_pop(MENU3_UP);/*被选中的三级菜单向上切换选项*/
			}
			else/*当主菜单条处于收回状态 且 没有三级菜单弹出选项 时*/
			{
				switch (pp->pos_pos)
				{
					case MENU2_STOP:
						pp->pos_last1 = pp->pos1[pp->pos];
						pp->pos1[pp->pos] > 0 ? pp->pos1[pp->pos]-- :  (pp->pos1[pp->pos] = (pp->menu2_qty - 1));
						draw_menu2(0);
						draw_menu3(1, NULL);
						break;
					case MENU2_PRESSED:
						break;


					case MENU3_STOP:	/*向上轮流切换三级菜单*/
						if(pp->menu3_amount != 0)  /*当三级菜单不全为黑时*/
						{
							pp->menu3_start = 0;
							while(!(gtk_widget_get_sensitive(pp->eventbox30[pp->menu3_start])))
								pp->menu3_start++;	/*找出第一个可选的三级菜单位置*/

							if(CUR_POS > pp->menu3_start)
							{
								CUR_POS--;
								while(!(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS])))
									CUR_POS--;
							}

							else if(CUR_POS == pp->menu3_start)
							{
								CUR_POS = pp->menu3_amount-1;
							}
							draw_menu3(1, NULL);
						}
						else if(pp->menu3_amount == 0)	/*当三级菜单全部为黑掉时*/
							break;
						break;

					case MENU3_PRESSED:
						break;
				}
			}
			break;
		case GDK_Right:
		case GDK_Down:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_DOWN);/*被选中的主菜单向下切换*/
			}
			else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
			{
				menu3_pop(MENU3_TURN);/*被选中的三级菜单向下切换*/
			}
			else/*当主菜单条处于收回状态时*/
			{
				switch (pp->pos_pos)
				{
					case MENU2_STOP:
						pp->pos_last1 = pp->pos1[pp->pos];
						pp->pos1[pp->pos] < (pp->menu2_qty - 1) ? pp->pos1[pp->pos]++ :  (pp->pos1[pp->pos] = 0);
						draw_menu2(0);
						draw_menu3(1, NULL);
						break;
					case MENU2_PRESSED:
						break;
					case MENU3_STOP:	/*向下轮流切换三级菜单*/
						if(pp->menu3_amount != 0)  /*当三级菜单不全为黑时*/
						{
							pp->menu3_start = 0;
							while(!(gtk_widget_get_sensitive(pp->eventbox30[pp->menu3_start])))
								pp->menu3_start++;	/*找出第一个可选的三级菜单位置*/

							if(CUR_POS < pp->menu3_amount - 1)
							{
								CUR_POS++;
								while(!(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS])))
									CUR_POS++;
							}

							else if(CUR_POS == pp->menu3_amount - 1)
							{
								CUR_POS = pp->menu3_start;
							}
							draw_menu3(1, NULL);
						}
						else if (pp->menu3_amount == 0)	/*当三级菜单全部为黑掉时*/
							break;
						break;

					case MENU3_PRESSED:
						break;
				}
			}
			break;
		default:break;
	}
	return 0;
}

/* 快捷键处理函数 */
gboolean key_press_handler (GtkWidget* pWidget,	GdkEventKey* pEvent, gpointer data)
{
	if (pEvent->type == GDK_KEY_PRESS)
	{
		handler_key(pEvent->keyval, data);
	}
	return TRUE;
}

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data)
{
	handler_key(keyval, data);
	return 0;
}

gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer data)
{
	guint num = GPOINTER_TO_UINT (data);
	gpointer p = pp;
	printf("\n num = %d \n", num);
	if(!pp->help_yn)
	{
		switch (num)
		{
			case 0:b3_fun0(p);break;
			case 1:b3_fun1(p);break;
			case 2:b3_fun2(p);break;
			case 3:b3_fun3(p);break;
			case 4:b3_fun4(p);break;
			case 5:b3_fun5(p);break;
			default:break;
		}
	}
	return TRUE;
}

void data_0021 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Group->Operation */
{
	pp->operation_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0022 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Group->Group */
{
	pp->wgroup_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0023 (GtkMenuItem *menuitem, gpointer data) /* */
{
	g_tmp_group_struct.group_mode = (gchar) (GPOINTER_TO_UINT (data));
	//if(!g_tmp_group_struct.group_mode) /*group mode 选择UT时，focal law 不可用*/
	//gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
	//else
	//gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_00241 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	set_group_val (&g_tmp_group_struct, GROUP_PULSER, (int) (gtk_spin_button_get_value (spinbutton)));
	if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)
		set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_00242 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode P111 */
{
	set_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE,  (int) (GPOINTER_TO_UINT (data)));
	if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)
		set_group_val (&g_tmp_group_struct, 
				GROUP_RECEIVER, get_group_val (&g_tmp_group_struct, GROUP_PULSER));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0025 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Select->Select 502 */
{
	set_probe_select (pp->p_config, (gboolean) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0027 (GtkSpinButton *spinbutton, gpointer data) /*scanoffset */
{
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		g_tmp_group_struct.scan_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	}
	else
	{
		g_tmp_group_struct.scan_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
	}

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_032 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_weland_height (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_weland_height (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
}
void data_0321 (GtkSpinButton *spinbutton, gpointer data) /*fill zone height*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_fizone_height (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_fizone_height (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
}

void data_033 (GtkMenuItem *menuitem, gpointer data) /* wizard weld->symmetry */
{
	set_part_symmetry (pp->p_config, data);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0331 (GtkSpinButton *spinbutton, gpointer data) /*weld land offset*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_weland_offset (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_weland_offset (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
}
void data_0332 (GtkSpinButton *spinbutton, gpointer data) /*fill zone angle*/
{
	set_fizone_angle (pp->p_config, (GUINT_TO_POINTER
				((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
}

void data_034 (GtkSpinButton *spinbutton, gpointer data) /*fill zone radius*/
{
	set_fizone_radius (pp->p_config, (GUINT_TO_POINTER
				((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
}

void data_00341 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_00342 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	set_group_val (&g_tmp_group_struct, GROUP_PULSER, (int) (gtk_spin_button_get_value (spinbutton)));
	if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)
		set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));

	/*发送给硬件*/
}

void data_0037 (GtkSpinButton *spinbutton, gpointer data) /*indexoffset */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		g_tmp_group_struct.index_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
		g_tmp_group_struct.index_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
}

void data_0044 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_00471 (GtkSpinButton *spinbutton, gpointer data) /* Skew (deg) */
{
	g_tmp_group_struct.skew =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_00472 (GtkMenuItem *menuitem, gpointer data) /* Skew (deg) */
{
	gushort temp = GPOINTER_TO_UINT (data);
	g_tmp_group_struct.skew_pos = (guchar) (GPOINTER_TO_UINT (data));

	switch (g_tmp_group_struct.skew_pos)
	{
		case 0:g_tmp_group_struct.skew = 0;break;
		case 1:g_tmp_group_struct.skew = 9000;break;
		case 2:g_tmp_group_struct.skew =18000;break;
		case 3:g_tmp_group_struct.skew =27000;break;
		default:break;
	}

	if (temp != 4)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
	}
}

void data_022 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Type */
{
	pp->ctype_pos = (guchar) (GPOINTER_TO_UINT (data));
	if(!pp->ctype_pos)
		pp->clb_flag = 0;
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0221 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Echo Type */
{
	pp->echotype_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}
#if 0
void data_0222 (GtkSpinButton *spinbutton, gpointer data) /* wizard  First Angle  */
{
	pp->first_angle =  (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}
#endif
void data_0223 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Ref.Amplitude  */
{
	pp->ref_amplitude =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}
void data_0224 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Comp Gain  */
{
	pp->comp_gain =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}
void data_0225 (GtkMenuItem *menuitem, gpointer data) /* wizard  Weld Type  */
{
	pp->weldtype_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_0226 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Comp Gain  */
{
	pp->cangle =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_0227 (GtkSpinButton *spinbutton, gpointer data) /* distance */
{
	pp->distance =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_023 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Mode */
{
	pp->cmode_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0231 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Select code */
{
	pp->scode_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0232 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Radius 1 */
{
	pp->radius1 =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_0233 (GtkSpinButton *spinbutton, gpointer data) /* wizard  depth 1 */
{
	pp->depth1 =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_0234 (GtkSpinButton *spinbutton, gpointer data) /* wizard  thickness 1 */
{
	pp->thickness1 =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	TMP_CBA(distance) = pp->thickness1 / 1000;
}

void data_0235 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Radius A */
{
	pp->radiusa =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	TMP_CBA(distance) = pp->radiusa / 1000;
}
void data_0236 (GtkSpinButton *spinbutton, gpointer data) /* wizard  depth A */
{
	pp->deptha =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	TMP_CBA(distance) = pp->deptha / 1000;
}

void data_0237 (GtkSpinButton *spinbutton, gpointer data) /* wizard  thickness */
{
	pp->thickness =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_0238 (GtkSpinButton *spinbutton, gpointer data) /* wizard  thickness A */
{
	pp->last_angle =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_0239 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Tolerance*/
{
	pp->tolerance_t =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}



void data_024 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Radius 2 */
{
	pp->radius2 =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);

	/*发送给硬件*/
}

void data_0241 (GtkSpinButton *spinbutton, gpointer data) /* wizard  depth 2 */
{
	pp->depth2 =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);

	/*发送给硬件*/
}

void data_0242 (GtkSpinButton *spinbutton, gpointer data) /* wizard  thickness 2 */
{
	pp->thickness2 =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_0243 (GtkSpinButton *spinbutton, gpointer data) /* wizard  tolerance  */
{
	pp->tolerance =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_100 (GtkSpinButton *spinbutton, gpointer data) /* 增益Gain P100 */
{
	gchar *markup;
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	gint gain = ((gint)(gtk_spin_button_get_value(spinbutton) * 100) + 5) / 10 * 10;

	if (get_group_db_ref (pp->p_config, grp))
		set_group_val (p_grp, GROUP_GAIN,
				gain + get_group_val (p_grp, GROUP_GAINR));
	else
		set_group_val (p_grp, GROUP_GAIN,	gain);

	if (get_group_db_ref (pp->p_config, grp))
		markup = g_markup_printf_escaped (
				"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
				(get_group_val (p_grp, GROUP_GAIN) -
				 get_group_val (p_grp, GROUP_GAINR)) / 100.0,
				get_group_val (p_grp, GROUP_GAINR) / 100.0);
	else
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
				get_group_val (p_grp, GROUP_GAIN) / 100.0 );
	gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);

	g_free(markup);
	TMP(group_spi[grp]).gain = get_group_val (p_grp, GROUP_GAIN) / 10;


	group_data_spi new, *p1;
	memcpy (&new, &TMP(group_spi[grp]), sizeof (group_data_spi));
	p1 = &new;
	p1->offset = 16 * grp;
	p1->addr = 0x2;
	little_to_big ((unsigned int *)(p1),2);

#if ARM
	/*	ioctl (fd_gpio, GPIO43_LOW, &i);*/ /* 发送group参数不复位 */
	write (fd_array, (unsigned char *)(p1), 8);
	//write_group_data (&TMP(group_spi[grp]), grp);
#endif

}

void data_101 (GtkSpinButton *spinbutton, gpointer data) /*Start 扫描延时 P101 */
{
	request_refresh() ;

	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	gint temp_prf;

	double current_angle ;
	double max_angle     ;
	int BeamNo = pp->p_tmp_config->beam_num[grp];
	if(LAW_VAL(Focal_type) == 0)
	{
		current_angle = LAW_VAL(Angle_min)/100.0 + BeamNo * LAW_VAL(Angle_step)/100.0 ;
		max_angle = MAX(abs(LAW_VAL(Angle_min)), abs(LAW_VAL(Angle_max))) * G_PI / 180.0 ;
	}
	else
	{
		current_angle = LAW_VAL(Angle_min)/100.0 ;
		max_angle = LAW_VAL(Angle_min) * G_PI / 180.0 ;
	}
	if(GROUP_VAL(group_mode) == 1) //PA
		current_angle = current_angle * G_PI / 180.0 ;
	else
		current_angle = 0 ;

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			set_group_val (p_grp, GROUP_START,
					(gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
						(cos(TMP(current_angle[grp])) * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)));
		else  /* 英寸 */
			set_group_val (p_grp, GROUP_START,
					(gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
						(cos(TMP(current_angle[grp])) * 0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)));
	}
	else /* 显示方式为时间 */
		set_group_val (p_grp, GROUP_START,
				(gint) (gtk_spin_button_get_value (spinbutton) * 1000.0)); 

	set_group_val (p_grp, GROUP_START,
			((get_group_val (p_grp, GROUP_START) + 5) / 10 ) * 10);

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();

	TMP(group_spi[grp]).sample_start	= (get_group_val (p_grp, GROUP_START) +
			get_group_val (p_grp, GROUP_WEDGE_DELAY)) / 10;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		get_group_val (p_grp, GROUP_RANGE) / 10;


	if(GROUP_VAL(group_mode) == 1) //PA
	{
		TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range  + TMP(max_beam_delay[grp]) + TMP(group_spi[grp]).compress_rato;
		temp_prf = TMP(beam_qty[grp]) * GROUP_VAL_POS(grp, prf1);
		TMP(group_spi[grp]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	}
	else  //UT
	{
		TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range + TMP(group_spi[grp]).compress_rato;
		temp_prf = TMP(beam_qty[grp]) * GROUP_VAL_POS(grp, prf1);
		TMP(group_spi[grp]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	}
}

void data_102 (GtkSpinButton *spinbutton, gpointer data) /*Range 范围 P102 */
{
	request_refresh() ;

	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	gint temp_prf;

	double current_angle ;
	double max_angle     ;
	int BeamNo = pp->p_tmp_config->beam_num[grp];
	if(LAW_VAL(Focal_type) == 0)
	{
		current_angle = LAW_VAL(Angle_min)/100.0 + BeamNo * LAW_VAL(Angle_step)/100.0 ;
		max_angle = MAX(abs(LAW_VAL(Angle_min)), abs(LAW_VAL(Angle_max))) * G_PI / 180.0 ;
	}
	else
	{
		current_angle = LAW_VAL(Angle_min)/100.0 ;
		max_angle = LAW_VAL(Angle_min) * G_PI / 180.0 ;
	}
	if(GROUP_VAL(group_mode) == 1) //PA
		current_angle = current_angle * G_PI / 180.0 ;
	else
		current_angle = 0 ;

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			set_group_val (p_grp, GROUP_RANGE,
					(gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
						(cos(TMP(current_angle[grp])) * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)));
		else  /* 英寸 */
			set_group_val (p_grp, GROUP_RANGE,
					(gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
						( cos(TMP(current_angle[grp])) * 0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)));
	}
	else /* 显示方式为时间 */
		set_group_val (p_grp, GROUP_RANGE,
				gtk_spin_button_get_value (spinbutton) * 1000); 


	if(GROUP_VAL(point_qty_pos) == 0) // point quantity is auto --- adjust the point quantity
	{
		if(get_group_val (p_grp, GROUP_RANGE) <= 6400)
		{
			GROUP_VAL(point_qty) = get_group_val (p_grp, GROUP_RANGE) / 10 ;
		}
		else
		{
			double float_compress_rate ;
			int    int_compress_rate   ;
			float_compress_rate = get_group_val (p_grp, GROUP_RANGE) / 6400.0 ;
            int_compress_rate   = (get_group_val (p_grp, GROUP_RANGE) % 6400) ? (int) (float_compress_rate + 1) : (int)float_compress_rate ;
            GROUP_VAL(point_qty) = get_group_val (p_grp, GROUP_RANGE) / (int_compress_rate * 10 );
		}
	}

	set_group_val (p_grp, GROUP_RANGE,
			rounding(0, get_group_val (p_grp, GROUP_RANGE), GROUP_VAL(point_qty) * 10));

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	TMP(group_spi[grp]).compress_rato	= 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) > 1 ? 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) : 1;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		get_group_val (p_grp, GROUP_RANGE) / 10;		

	if(GROUP_VAL(group_mode) == 1) //PA
	{
		TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range  + TMP(max_beam_delay[grp]) + TMP(group_spi[grp]).compress_rato;
		temp_prf = TMP(beam_qty[grp]) * GROUP_VAL_POS(grp, prf1);
		TMP(group_spi[grp]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	}
	else  //UT
	{
		TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range + TMP(group_spi[grp]).compress_rato;
		temp_prf = TMP(beam_qty[grp]) * GROUP_VAL_POS(grp, prf1);
		TMP(group_spi[grp]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	}

}

void data_103 (GtkSpinButton *spinbutton, gpointer data) /*楔块延时  P103 */
{
	//gint tt[4];
	request_refresh() ;

	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	gint temp_prf;

	double current_angle ;
	double max_angle     ;
	int BeamNo = pp->p_tmp_config->beam_num[grp];
	if(LAW_VAL(Focal_type) == 0)
	{
		current_angle = LAW_VAL(Angle_min)/100.0 + BeamNo * LAW_VAL(Angle_step)/100.0 ;
		max_angle = MAX(abs(LAW_VAL(Angle_min)), abs(LAW_VAL(Angle_max))) * G_PI / 180.0 ;
	}
	else
	{
		current_angle = LAW_VAL(Angle_min)/100.0 ;
		max_angle = LAW_VAL(Angle_min) * G_PI / 180.0 ;
	}
	current_angle = current_angle * G_PI / 180.0 ;

	set_group_val (p_grp, GROUP_WEDGE_DELAY, 
			(gint) (gtk_spin_button_get_value (spinbutton) * 1000.0));

	TMP(group_spi[grp]).sample_start	= (get_group_val (p_grp, GROUP_START) +
			get_group_val (p_grp, GROUP_WEDGE_DELAY)) / 10 ;//+ TMP(max_beam_delay[grp]);
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		get_group_val (p_grp, GROUP_RANGE) / 10;		
	//tt[0] = (GROUP_VAL_POS(grp, gate[0].start) +	GROUP_VAL_POS (grp, gate[0].width));
	//tt[1] = (GROUP_VAL_POS(grp, gate[1].start) +	GROUP_VAL_POS (grp, gate[1].width));
	//tt[2] = (GROUP_VAL_POS(grp, gate[2].start) +	GROUP_VAL_POS (grp, gate[2].width));

	//tt[3] = MAX(tt[0], (MAX(tt[1],tt[2]))) / 10;
	//tt[3] = (gint)(tt[3]/cos(max_angle));

	TMP(group_spi[grp]).rx_time		= TMP(group_spi[grp]).sample_range  + TMP(max_beam_delay[grp]) + TMP(group_spi[grp]).compress_rato;
	temp_prf = TMP(beam_qty[grp]) * get_group_val (p_grp, GROUP_PRF_VAL);
	TMP(group_spi[grp]).idel_time	= 
		100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	/*		100000000 / (get_group_val (p_grp, GROUP_PRF_VAL) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;*/
	//send_spi_data (grp);
	/*发送给硬件*/
}

void data_104 (GtkSpinButton *spinbutton, gpointer data) /*声速 P104 */
{
	request_refresh() ;

	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if (UNIT_MM == get_unit(pp->p_config))
		set_group_val (p_grp, GROUP_VELOCITY,
				(gtk_spin_button_get_value (spinbutton) * 100));
	else   /* 英寸/微秒 */
		set_group_val (p_grp, GROUP_VELOCITY,
				(gint) (gtk_spin_button_get_value (spinbutton) * 25400 * 100));

	//double RANGE = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) / 1000.0 ;
	//double VELOCITY = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) / 100.0 ;
	//double range =  get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE)* get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) ;
	//double range = RANGE * VELOCITY / 2000.0 ;
	//printf("\n*****\n");
	//printf("range = %f\n",range );
	//printf("get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) = %d \n", get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE));
	//printf("get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) = %d \n", get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY));

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	/*发送给硬件*/
}

void data_110 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	//	set_group_val (&pp->p_config->group[get_current_group(pp->p_config)],
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_val (p_grp,
			GROUP_PULSER, (int) (gtk_spin_button_get_value (spinbutton)));
	if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)
		set_group_val (&pp->p_config->group[grp],
				GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_120 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	gint grp = get_current_group(pp->p_config);
	set_group_val (&pp->p_config->group[grp],
			GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_111 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode P111 */
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_val (p_grp, GROUP_TX_RX_MODE, (int)(GPOINTER_TO_UINT (data)));
	if (get_group_val (p_grp, GROUP_TX_RX_MODE) == PULSE_ECHO )
	{
		set_group_val (&pp->p_config->group[grp], GROUP_RECEIVER, 
				get_group_val (&pp->p_config->group[grp], GROUP_PULSER));
	}
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_1121 (GtkSpinButton *spinbutton, gpointer data) /* 频率 Freq 数值改变 */
{
	request_refresh() ;

	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_val (p_grp, GROUP_FREQ_VAL, (int) ((gtk_spin_button_get_value (spinbutton)) * 1000.0));
	if (!get_group_val (p_grp, GROUP_PW_POS))
		set_group_val (p_grp, GROUP_PW_VAL, 
				((int)(( 5000*10000 / get_group_val (p_grp, GROUP_FREQ_VAL))) / 250)  * 250) ; /* 改变脉冲宽度 */
	/* 发送给硬件 */
}

void data_112 (GtkMenuItem *menuitem, gpointer data) /* 频率 Freq P112 */
{
	guint temp = GPOINTER_TO_UINT (data);
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_val (p_grp, GROUP_FREQ_POS, (int) (GPOINTER_TO_UINT (data)));
	set_group_val (p_grp, GROUP_FREQ_VAL, get_freq());

	if (!get_group_val (p_grp, GROUP_PW_POS))
		set_group_val (p_grp, GROUP_PW_VAL, 
				((int)(( 5000*10000 / get_group_val (p_grp, GROUP_FREQ_VAL))) / 250)  * 250) ; /* 改变脉冲宽度 */
	if (temp != 12)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[2]), get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[2]));
	}
	/* 发送给硬件 */
}

void data_113 (GtkMenuItem *menuitem, gpointer data)  /* Voltage  P113 */
{
	request_refresh() ;

	gint grp = get_current_group (pp->p_config);
	set_voltage (pp->p_config, grp, (guchar)(GPOINTER_TO_UINT (data)));

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	TMP(group_spi[grp]).voltage = get_voltage (pp->p_config, grp);	
	//send_spi_data (grp);
	/* 发送给硬件 */
}

void data_1141 (GtkSpinButton *spinbutton, gpointer data) /* PW  P114 */
{
	request_refresh() ;
	gint temp =  (gint) (gtk_spin_button_get_value (spinbutton) * 100);
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	temp = (temp / 250) * 250;
	set_group_val (p_grp, GROUP_PW_VAL, temp);
	//send_focal_spi (get_current_group(pp->p_config));
}

void data_114 (GtkMenuItem *menuitem, gpointer data) /* PW */
{
	request_refresh() ;
	guint temp = GPOINTER_TO_UINT (data);
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	set_group_val (p_grp, GROUP_PW_POS, (int) temp);
	set_group_val (p_grp, GROUP_PW_VAL, get_pw()*100);

	//g_print ("pw_pos=%d\n ", get_group_val (p_grp, GROUP_PW_POS));
	if (temp == AUTO_SET)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[4]), 
				get_group_val (p_grp, GROUP_PW_VAL) / 100.0);

	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[4]));
	}
	//send_focal_spi (get_current_group(pp->p_config));
	/* 发送给硬件 */

}

void data_1151 (GtkSpinButton *spinbutton, gpointer data) /* PRF P115 */
{

	gchar *markup;
	gint grp = get_current_group(pp->p_config);
	gint temp_prf;
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	set_group_val (p_grp, GROUP_PRF_VAL, 
			(gint) (gtk_spin_button_get_value (spinbutton) * 10));
	markup=g_markup_printf_escaped(
			"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",get_group_val (p_grp, GROUP_PRF_VAL) / 10, (get_group_val (p_grp, GROUP_PRF_VAL)*get_beam_qty() / 10) * 1);
	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
	g_free(markup);

	markup = g_markup_printf_escaped (
			"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",(gfloat)(get_group_val (p_grp, GROUP_PRF_VAL) / 10.0));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;
	g_free(markup);
	//if (get_group_val (p_grp, GROUP_PRF_VAL) >= 400)
	//	set_group_val (p_grp, GROUP_PRF_VAL, 400);
	temp_prf = TMP(beam_qty[grp]) * get_group_val (p_grp, GROUP_PRF_VAL);
	TMP(group_spi[grp]).idel_time	= 
		100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	/*		100000000 / (get_group_val (p_grp, GROUP_PRF_VAL) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;*/


	write_group_data (&TMP(group_spi[grp]), grp);
	/* 发送给硬件 */
}

void data_115 (GtkMenuItem *menuitem, gpointer data) /* PRF */
{
	gchar *markup;
	guint temp = GPOINTER_TO_UINT (data);
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	gint temp_prf;
	set_group_val (p_grp, GROUP_PRF_POS, temp);
	set_group_val (p_grp, GROUP_PRF_VAL, get_prf());			/*  */

	markup=g_markup_printf_escaped(
			"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",get_group_val (p_grp, GROUP_PRF_VAL) / 10, (get_group_val (p_grp, GROUP_PRF_VAL)*get_beam_qty() / 10) * 1);
	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
	g_free(markup);

	markup = g_markup_printf_escaped (
			"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",(gfloat)(get_group_val (p_grp, GROUP_PRF_VAL) / 10.0));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;
	g_free(markup);


	if (temp != 3)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[5]), get_prf() / 10.0);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[5]));
	}

	//if (get_group_val (p_grp, GROUP_PRF_VAL)  >= 400)
	//	set_group_val (p_grp, GROUP_PRF_VAL, 400);
	temp_prf = TMP(beam_qty[grp]) * get_group_val (p_grp, GROUP_PRF_VAL);
	TMP(group_spi[grp]).idel_time		= 
		/*		100000000 / (get_group_val (p_grp, GROUP_PRF_VAL) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;*/
		100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	/* 如何 */
	write_group_data (&TMP(group_spi[grp]), grp);

}

void data_1201 (GtkMenuItem *menuitem, gpointer data) /* damping p120 */
{
	//get_damping_pos (pp->p_config);
	set_damping_pos (pp->p_config, (void*)(data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

/* filter 滤波 P121 */
void data_121 (GtkMenuItem *menuitem, gpointer data)  
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_val (p_grp, GROUP_FILTER_POS, (int) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	get_filter();
	/* 发送给硬件 */
	if (get_group_val (p_grp, GROUP_FILTER_POS) == 0)
	{
		TMP(group_spi[grp]).freq_band	= 0;
	}
	else if (get_group_val (p_grp, GROUP_FILTER_POS) == 1)
	{
		if (get_group_val (p_grp, GROUP_FREQ_VAL) < 1250)
			TMP(group_spi[grp]).freq_band	= 1;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 1750)
			TMP(group_spi[grp]).freq_band	= 2;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 2125)
			TMP(group_spi[grp]).freq_band	= 3;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 3125)
			TMP(group_spi[grp]).freq_band	= 4;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 4500)
			TMP(group_spi[grp]).freq_band	= 5;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 6250)
			TMP(group_spi[grp]).freq_band	= 6;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 8750)
			TMP(group_spi[grp]).freq_band	= 7;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 11000)
			TMP(group_spi[grp]).freq_band	= 8;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 13500)
			TMP(group_spi[grp]).freq_band	= 9;
		else if (get_group_val (p_grp, GROUP_FREQ_VAL) < 17500)
			TMP(group_spi[grp]).freq_band	= 10;
		else 
			TMP(group_spi[grp]).freq_band	= 11;
	}
	else
		TMP(group_spi[grp]).freq_band	= 
			get_group_val (p_grp, GROUP_FILTER);
	write_group_data (&TMP(group_spi[grp]), grp);
}

#define RF_PATH	"source/system/Sample/Palette/ONDT_RFTOFD.pal"
void data_122 (GtkMenuItem *menuitem, gpointer data)  /* Rectifier 检波 P122 */
{
	gint i;
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_val (p_grp, GROUP_RECTIFIER,
			(GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	if(!GPOINTER_TO_UINT (data))
	{
		read_palette_file (RF_PATH, TMP(t_special_col), TMP(t_color));  /*   */
		if (TMP(t_special_col[0]) != 0x12345678)
		{
			TMP(special_col_amp[0]) = COL_24_TO_16(TMP(t_special_col)[0]);
			TMP(special_col_amp[1]) = COL_24_TO_16(TMP(t_special_col)[1]);
			TMP(special_col_amp[2]) = COL_24_TO_16(TMP(t_special_col)[2]);
		}
		for (i = 0; i < 256; i ++)
			TMP(color_amp[i]) = COL_24_TO_16(TMP(t_color)[i]);
	}

	/* 发送给硬件 */
	TMP(group_spi[grp]).rectifier =
		get_group_val (p_grp, GROUP_RECTIFIER);
	send_spi_data (grp);
}

void data_124 (GtkMenuItem *menuitem, gpointer data)  /* averaging 平均 TAN1 P124 */
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_val (p_grp, GROUP_AVERAGING,
			(GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_125 (GtkSpinButton *spinbutton, gpointer data) /*抑制 Reject P125 */
{
	gint grp = get_current_group(pp->p_config);
	set_reject(pp->p_config, (guchar) (gtk_spin_button_get_value (spinbutton)));

	/*发送给硬件*/
	TMP(group_spi[grp]).reject = get_reject(pp->p_config) * 40.95;	
	send_spi_data (grp);
}

void data_130 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	GROUP_VAL(scan_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送给硬件*/
}

void data_131 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	GROUP_VAL(index_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送给硬件*/
}

void data_132 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	LAW_VAL(Angle_min) =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_134 (GtkSpinButton *spinbutton, gpointer data) /* beam delay */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	//pp->p_config->beam_delay =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	guint offset,k;
	guint group = get_current_group(pp->p_config);
	guint index = TMP(beam_num[get_current_group(pp->p_config)]);

	for (offset = 0, k = 0 ; k < group; k++)
		offset += TMP(beam_qty[k]);
	BEAM_INFO(index + offset,beam_delay) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	pp->G_delay[index] = BEAM_INFO(index + offset,beam_delay);
//	printf("UT_beam_delay = %d \n", pp->G_delay[index]);
	TMP(focal_law_all_beam[offset + index]).G_delay			= 
		get_group_val (get_group_by_id (pp->p_config, group), GROUP_WEDGE_DELAY)
		+	GROUP_VAL_POS (group, wedge.Probe_delay) + BEAM_INFO(index + offset, beam_delay);//modified by hefan 

	/*发送给硬件*/
	send_focal_spi(get_current_group(pp->p_config));
}

void data_135 (GtkSpinButton *spinbutton, gpointer data) /*gain offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	guint offset, k;
	gint grp = get_current_group(pp->p_config);
	gint index = TMP(beam_num[grp]);
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);

	pp->tmp_gain_off[ index + offset ] = (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	GROUP_VAL(gain_offset[index + offset]) =  pp->tmp_gain_off[ index + offset]; 

	/*发送给硬件*/
	send_focal_spi(get_current_group(pp->p_config));
}

void data_140 (GtkSpinButton *spinbutton, gpointer data)
{
#if ARM
	    pthread_t thread_id;
	    int ret;
		ret = pthread_create (&thread_id, NULL, (void*)thread_set_DB_eighty_percent, data);
		if(ret){
			perror("in1:");
			return;
		}
#endif
}



void data_1431 (GtkSpinButton *spinbutton, gpointer data) /* point qty P143 */
{
    request_refresh() ;

	gint grp = get_current_group(pp->p_config);
	//gint tt[4];
	gint temp_prf;
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	GROUP_VAL(point_qty) =  (guint)(gtk_spin_button_get_value (spinbutton));
	TMP(group_spi[grp]).point_qty = get_point_qty();

	get_prf();

	if ((GROUP_VAL(point_qty) * 10) >= (get_group_val (p_grp, GROUP_RANGE)))	
		set_group_val (p_grp, GROUP_RANGE,
				GROUP_VAL(point_qty) * 10);
	else
		set_group_val (p_grp, GROUP_RANGE,
				rounding(0, get_group_val (p_grp, GROUP_RANGE), GROUP_VAL(point_qty) * 10));


	TMP(group_spi[grp]).compress_rato	= 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) > 1 ? 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) : 1;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		get_group_val (p_grp, GROUP_RANGE) / 10;		
	//tt[0] = (GROUP_VAL_POS(grp, gate[0].start) + GROUP_VAL_POS (grp, gate[0].width));
	//tt[1] = (GROUP_VAL_POS(grp, gate[1].start) + GROUP_VAL_POS (grp, gate[1].width));
	//tt[2] = (GROUP_VAL_POS(grp, gate[2].start) + GROUP_VAL_POS (grp, gate[2].width));
	//tt[3] = MAX(tt[0], (MAX(tt[1],tt[2]))) / 10;

	TMP(group_spi[grp]).rx_time		= TMP(group_spi[grp]).sample_range  +
		TMP(max_beam_delay[grp]) + TMP(group_spi[grp]).compress_rato;
	temp_prf = TMP(beam_qty[grp]) * get_group_val (p_grp, GROUP_PRF_VAL);
	TMP(group_spi[grp]).idel_time	= 
		100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	/*		100000000 / (get_group_val (p_grp, GROUP_PRF_VAL) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;*/
	//send_spi_data (grp);
	/* 重新确认每次dma的点数 */
}

void data_143 (GtkMenuItem *menuitem, gpointer data) /* point qty P143 */
{
	request_refresh() ;

	guint temp = GPOINTER_TO_UINT (data);
	gint grp = get_current_group(pp->p_config);
	//gint tt[4];
	gint temp_prf;
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	GROUP_VAL(point_qty_pos) = temp;
	GROUP_VAL(point_qty) = get_point_qty();
	TMP(group_spi[grp]).point_qty = GROUP_VAL(point_qty);
	get_prf();
	if ((GROUP_VAL(point_qty) * 10) > (get_group_val (p_grp, GROUP_RANGE)))	
		set_group_val (p_grp, GROUP_RANGE, GROUP_VAL(point_qty) * 10);
	else
		set_group_val (p_grp, GROUP_RANGE,
				rounding(0, get_group_val (p_grp, GROUP_RANGE), GROUP_VAL(point_qty) * 10));
	if (temp != 4)
	{
		MENU_STATUS = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[3]), GROUP_VAL(point_qty));
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[3]));
	}

	TMP(group_spi[grp]).compress_rato	= 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) > 1 ? 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) : 1;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		get_group_val (p_grp, GROUP_RANGE) / 10;		
	//tt[0] = (GROUP_VAL_POS(grp, gate[0].start) + GROUP_VAL_POS (grp, gate[0].width));
	//tt[1] = (GROUP_VAL_POS(grp, gate[1].start) + GROUP_VAL_POS (grp, gate[1].width));
	//tt[2] = (GROUP_VAL_POS(grp, gate[2].start) + GROUP_VAL_POS (grp, gate[2].width));

	//tt[3] = MAX(tt[0], (MAX(tt[1],tt[2]))) / 10;
	TMP(group_spi[grp]).rx_time		= TMP(group_spi[grp]).sample_range  + TMP(max_beam_delay[grp]) + TMP(group_spi[grp]).compress_rato;
	temp_prf = TMP(beam_qty[grp]) * get_group_val (p_grp, GROUP_PRF_VAL);
	TMP(group_spi[grp]).idel_time	= 
		100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	/*		100000000 / (get_group_val (p_grp, GROUP_PRF_VAL) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;*/
	//send_spi_data (grp);
	/* 重新确认每次dma的点数 */
}

void data_1451 (GtkSpinButton *spinbutton, gpointer data) /* Sum Gain ->user_define*/
{
	//	gint	grp	= get_current_group(pp->p_config);
	GROUP_VAL(sum_gain) =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 100.0);

	/* 发送给硬件 
	//	if (GROUP_VAL_POS(grp, probe.Elem_qty) == 1)	
	if (LAW_VAL_POS(grp, Elem_qty) == 1)	
	TMP(group_spi[grp]).sum_gain	= 4095;	
	else 
	TMP(group_spi[grp]).sum_gain	= 
	4096 / GROUP_VAL_POS(grp, probe.Elem_qty);*/	
	//	TMP(group_spi[grp]).sum_gain	= GROUP_VAL(sum_gain)/100;
	//	send_spi_data (grp);
	/* 发送给硬件 */
}


void data_145 (GtkMenuItem *menuitem, gpointer data) /* Sum Gain ->Atuo*/
{
	guint	temp= GPOINTER_TO_UINT (data);
	//	gint	grp	= get_current_group(pp->p_config);
	GROUP_VAL(sum_gain_pos) = temp;
	GROUP_VAL(sum_gain) = get_sum_gain();
	if (temp != 1)
	{
		MENU_STATUS = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[5]), GROUP_VAL(sum_gain)/100.0);
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[5]));
	}

	/* 发送给硬件 
	//	if (GROUP_VAL_POS(grp, probe.Elem_qty) == 1)	
	if (LAW_VAL_POS(grp, Elem_qty) == 1)	
	TMP(group_spi[grp]).sum_gain	= 4095;	
	else 
	TMP(group_spi[grp]).sum_gain	= 
	4096 / GROUP_VAL_POS(grp, probe.Elem_qty);	
	send_spi_data (grp);*/
}

void data_200 (GtkMenuItem *menuitem, gpointer data) /* Gate 闸门选择 P200 */
{
	GROUP_VAL(gate_pos) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_201 (GtkMenuItem *menuitem, gpointer data) /* parameter 闸门参数选择 位置或者模式 P201 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].parameters) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);


}

void data_202 (GtkSpinButton *spinbutton, gpointer data)	/* 闸门开始位置 P202 */
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		pp->gate_start_clb = gtk_spin_button_get_value (spinbutton) * 2000.0;
		if (UNIT_MM == get_unit(pp->p_config))
			GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
		else  /* 英寸 */
			GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
	}
	else /* 显示方式为时间 */
	{
		GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;
		pp->gate_start_clb = gtk_spin_button_get_value (spinbutton) * 1000.0;
	}

	update_gate_info();
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	send_focal_spi (grp);
	send_group_spi (grp);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_2021 (GtkMenuItem *menuitem, gpointer data)	/* 闸门同步 */
{
	guint tmp=0;
	guint group = get_current_group(pp->p_config);
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].synchro) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	if (GROUP_VAL(gate_pos) == GATE_A)
		tmp = TMP(group_spi[group]).gate_a_logic;
	else if (GROUP_VAL(gate_pos) == GATE_B)
		tmp = TMP(group_spi[group]).gate_b_logic;
	else if (GROUP_VAL(gate_pos) == GATE_I)
		tmp = TMP(group_spi[group]).gate_i_logic;

	if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].synchro) == 0)
		tmp = (tmp & 0xfffffff3) | 0x00;
	else if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].synchro) == 1)
		tmp = (tmp & 0xfffffff3) | 0x0c;
	else if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].synchro) == 2)
		tmp = (tmp & 0xfffffff3) | 0x04;

	if (GROUP_VAL(gate_pos) == GATE_A)
		TMP(group_spi[group]).gate_a_logic = tmp;
	else if (GROUP_VAL(gate_pos) == GATE_B)
		TMP(group_spi[group]).gate_b_logic = tmp;
	else if (GROUP_VAL(gate_pos) == GATE_I)
		TMP(group_spi[group]).gate_i_logic = tmp;

	send_spi_data (group);
}

void data_203 (GtkSpinButton *spinbutton, gpointer data) /* 闸门宽度 P203 */
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		pp->gate_width_clb = gtk_spin_button_get_value (spinbutton) * 2000.0;
		if (UNIT_MM == get_unit(pp->p_config))
			GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
		else  /* 英寸 */
			GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
	}
	else /* 显示方式为时间 */
	{
		GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ; 
		pp->gate_width_clb = gtk_spin_button_get_value (spinbutton) * 1000.0;
	}
	
	update_gate_info();
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	send_focal_spi (grp);
	send_group_spi (grp);
	gtk_widget_queue_draw (pp->vboxtable);

}

void data_2031 (GtkMenuItem *menuitem, gpointer data)	/* 波峰或者前沿 测量选项 */
{
	guint tmp=0;
	guint group = get_current_group(pp->p_config);
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].measure) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	if (GROUP_VAL(gate_pos) == GATE_A)
		tmp = TMP(group_spi[group]).gate_a_logic;
	else if (GROUP_VAL(gate_pos) == GATE_B)
		tmp = TMP(group_spi[group]).gate_b_logic;
	else if (GROUP_VAL(gate_pos) == GATE_I)
		tmp = TMP(group_spi[group]).gate_i_logic;

	if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	if (GROUP_VAL(gate_pos) == GATE_A)
		TMP(group_spi[group]).gate_a_logic = tmp;
	else if (GROUP_VAL(gate_pos) == GATE_B)
		TMP(group_spi[group]).gate_b_logic = tmp;
	else if (GROUP_VAL(gate_pos) == GATE_I)
		TMP(group_spi[group]).gate_i_logic = tmp;

	send_spi_data (group);
}

void data_204 (GtkSpinButton *spinbutton, gpointer data) /* 闸门高度 P204 */
{
	guint group = get_current_group(pp->p_config);
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].height) =  (guchar) ( gtk_spin_button_get_value (spinbutton));
	if (GROUP_VAL(gate_pos) == GATE_A)
	{
		TMP(group_spi[group]).gate_a_height	= GROUP_VAL_POS(group, gate[0].height)*4095*0.01;
	}
	else if (GROUP_VAL(gate_pos) == GATE_B)
	{
		TMP(group_spi[group]).gate_b_height	= GROUP_VAL_POS(group, gate[1].height)*4095*0.01;
	}
	else if (GROUP_VAL(gate_pos) == GATE_I)
	{
		TMP(group_spi[group]).gate_i_height	= GROUP_VAL_POS(group, gate[2].height)*4095*0.01;
	}

	send_spi_data (group);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_2041 (GtkMenuItem *menuitem, gpointer data) /* 闸门RF 选择 射频时候才可以调节 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].rectifier_freq) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

}

void data_210 (GtkMenuItem *menuitem, gpointer data) /* Alarm  P210 */
{
	set_alarm_pos (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_211 (GtkMenuItem *menuitem, gpointer data) /* Group A P211 */
{
	set_alarm_groupa (pp->p_config,	(guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}
void data_212 (GtkMenuItem *menuitem, gpointer data) /* Condition A P212 */
{
	set_alarm_conditiona (pp->p_config,	(guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_213 (GtkMenuItem *menuitem, gpointer data) /* operator P213 */
{
	set_alarm_operator (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_214 (GtkMenuItem *menuitem, gpointer data) /* Group B P214 */
{
	set_alarm_groupb (pp->p_config,	(guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_215 (GtkMenuItem *menuitem, gpointer data) /* condition B P215 */
{
	set_alarm_conditionb (pp->p_config,	(guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_220 (GtkMenuItem *menuitem, gpointer data) /* Output P220 */
{	
	set_output_pos (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_221 (GtkMenuItem *menuitem, gpointer data) /* Output->alarm # P221 */
{
	gint temp = (GPOINTER_TO_INT (data));
	gint i;

	if (temp == 0)
	{
		for (i = 1; i < 17; i++) 
			set_output_alarm (pp->p_config, ALARM_OFF, i);
	}
	else if (temp == 1)
	{
		for (i = 1; i < 17; i++) 
			set_output_alarm (pp->p_config, ALARM_ON, i);
	}
	else if (temp ==  18)
	{
	}
	else 
	{
		set_output_alarm (pp->p_config, !get_output_alarm_pos (pp->p_config, temp -1), temp - 1);
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_2211 (GtkMenuItem *menuitem, gpointer data) /* Output->group */
{
	set_output_group (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_222 (GtkSpinButton *spinbutton, gpointer data) /* count P222 */
{
	set_output_count (pp->p_config, (guchar) (gtk_spin_button_get_value (spinbutton)));
}

void data_2221 (GtkMenuItem *menuitem, gpointer data) /* count */
{
	set_output_data (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_223 (GtkMenuItem *menuitem, gpointer data) /* sound P223 */
{
	set_output_sound (pp->p_config,	(guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_224 (GtkSpinButton *spinbutton, gpointer data) /*active_delay */
{
	set_output_delay (pp->p_config,	(guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
}

void data_225 (GtkSpinButton *spinbutton, gpointer data) /* holdtime P225*/
{
	set_output_holdtime (pp->p_config,	(guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
}


void data_230 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm->Sizing Curves->Mode P230 */
{
	GROUP_VAL(mode_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_231 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm->Sizing Curves->Curve P231 */
{
	GROUP_VAL(curve_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	draw_menu3(0, NULL);
}

void data_2311 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm->Sizing Curves->Point 231 */
{
	GROUP_VAL(point_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_2312 (GtkSpinButton *spinbutton, gpointer data) /* Mat.Attenuatior P2312 */
{
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			GROUP_VAL(mat_atten) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 );
		else  /* 英寸 */
			GROUP_VAL(mat_atten) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.03937);
	}
	else /* 显示方式为时间 */
		GROUP_VAL(mat_atten) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.338) ;
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_232 (GtkSpinButton *spinbutton, gpointer data) /* Ref.Amplitude P232 */
{
	int i;
	GROUP_VAL(ref_ampl) =  (gushort) (gtk_spin_button_get_value (spinbutton) * 10.0);
	GROUP_VAL(amplitude[0]) = GROUP_VAL(ref_ampl)*100;
	for(i =0; i< GROUP_VAL(dac_point_qty); i++)
	{
		if(100*GROUP_VAL(ref_ampl)< GROUP_VAL(amplitude[i]))
			GROUP_VAL(amplitude[i]) = 100*GROUP_VAL(ref_ampl);
	}
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}
void data_2321 (GtkSpinButton *spinbutton, gpointer data) /* Position P2321 */
{
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			GROUP_VAL(position[GROUP_VAL(point_pos)]) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
		else  /* 英寸 */
			GROUP_VAL(position[GROUP_VAL(point_pos)]) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
	}
	else /* 显示方式为时间 */
		GROUP_VAL(position[GROUP_VAL(point_pos)]) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.1159 / 0.03937);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}
void data_2322 (GtkSpinButton *spinbutton, gpointer data) /* Delay P2322 */
{
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			GROUP_VAL(delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 ) ;
		else  /* 英寸 */
			GROUP_VAL(delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937) ;
	}
	else /* 显示方式为时间 */
		GROUP_VAL(delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 2.945 ) ;
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_233 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	GROUP_VAL(ref_ampl_offset) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_2331 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	int i;
	GROUP_VAL(amplitude[GROUP_VAL(point_pos)]) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	for(i =GROUP_VAL(point_pos); i< GROUP_VAL(dac_point_qty); i++)
	{
		if(GROUP_VAL(amplitude[GROUP_VAL(point_pos)])< GROUP_VAL(amplitude[i]))
			GROUP_VAL(amplitude[i]) = GROUP_VAL(amplitude[GROUP_VAL(point_pos)]);
	}
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}
void data_2332 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	GROUP_VAL(tcg_gain) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_234 (GtkSpinButton *spinbutton, gpointer data) /*   */
{
	GROUP_VAL(curve_step) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_234_add_point (DRAW_UI_P p) /*   */
{
	GROUP_VAL(dac_point_qty) = GROUP_VAL(dac_point_qty)+1;
	if(GROUP_VAL(dac_point_qty)>=16)
		GROUP_VAL(dac_point_qty)= 16 ;
	else
	{
		GROUP_VAL(point_pos) = GROUP_VAL(dac_point_qty)-1;
		GROUP_VAL(position[GROUP_VAL(point_pos)]) = GROUP_VAL(position[GROUP_VAL(point_pos-1)]) ;
		GROUP_VAL(amplitude[GROUP_VAL(point_pos)]) = GROUP_VAL(amplitude[GROUP_VAL(point_pos-1)]) ;

		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		if(!pp->clb_flag)
			draw_area_all();
		else
			draw_area_calibration();
	}
}

void data_235_del_point  (DRAW_UI_P p) /*   */
{
	GROUP_VAL(dac_point_qty) = GROUP_VAL(dac_point_qty)-1 ;
	if(GROUP_VAL(dac_point_qty)<=3)
		GROUP_VAL(dac_point_qty)= 3 ;
	else
	{
		GROUP_VAL(point_pos) = GROUP_VAL(dac_point_qty)-1;
		//GROUP_VAL(position[GROUP_VAL(point_pos)]) = GROUP_VAL(position[GROUP_VAL(point_pos-1)]) ;
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		if(!pp->clb_flag)
			draw_area_all();
		else
			draw_area_calibration();
	}
}

void data_235 (GtkSpinButton *spinbutton, gpointer data) /*   */
{
	GROUP_VAL(ref_gain) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}








void data_300 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->list 300 */
{
	set_reading_list (pp->p_config, (gchar)(GPOINTER_TO_UINT (data)));

	if (get_reading_list(pp->p_config) == 0)
	{
		set_reading_field1 (pp->p_config, 0);
		set_reading_field2 (pp->p_config, 31);
		set_reading_field3 (pp->p_config, 29);
		set_reading_field4 (pp->p_config, 33);
	}
	else if ( get_reading_list(pp->p_config) == 1)
	{
		set_reading_field1 (pp->p_config, 0);
		set_reading_field2 (pp->p_config, 31);
		set_reading_field3 (pp->p_config, 35);
		set_reading_field4 (pp->p_config, 37);
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_302 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field1 302 */ 
{
	set_reading_field1 (pp->p_config, (gchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_303 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field2 303 */
{
	set_reading_field2 (pp->p_config, (gchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_304 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field3 304 */
{
	set_reading_field3 (pp->p_config, (gchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_305 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field4 305 */
{
	set_reading_field4 (pp->p_config, (gchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}

void data_310 (GtkMenuItem *menuitem, gpointer data) /* Measurements->cursors->selection 310 */
{
	GROUP_VAL(selection) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}


void data_311 (GtkSpinButton *spinbutton, gpointer data) 
{
	GROUP_VAL(per_reference) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
}
void data_3111 (GtkSpinButton *spinbutton, gpointer data) 
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_VAL(s_reference) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(s_reference) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3112 (GtkSpinButton *spinbutton, gpointer data) 
{
	GROUP_VAL(cursors_angle) =  (guint) (gtk_spin_button_get_value (spinbutton)*100.0);
}
void data_3113 (GtkSpinButton *spinbutton, gpointer data) 
{
	GROUP_CURSORS_POS(angle) =  (gshort) (gtk_spin_button_get_value (spinbutton)*100.0);
}

void data_312 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	//pp->p_config->cursors_scan =  (guint) (gtk_spin_button_get_value (spinbutton));
	GROUP_VAL(per_measure) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
}

void data_3121 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_VAL(s_measure) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(s_measure) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3122 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	GROUP_CURSORS_POS(amplitude) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
}

void data_313 (GtkSpinButton *spinbutton, gpointer data) /* */
{

	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			GROUP_VAL(u_reference) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(u_reference) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(u_reference) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 


	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3131 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_VAL(i_reference) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(i_reference) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_3132 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_VAL(s_refmeas) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(s_refmeas) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_3133 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*100.0);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if(get_unit(pp->p_config) == UNIT_MM)
			GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*1000.0);
		else
			GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*1000.0/0.03937);
	}
	else
		GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*1000.0);
}

void data_314 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			GROUP_VAL(u_measure) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(u_measure) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / 
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(u_measure) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 

	gtk_widget_queue_draw (pp->vboxtable);

}

void data_3141 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_VAL(i_measure) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(i_measure) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_3142 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_CURSORS_POS(scan) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_CURSORS_POS(scan) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_315 (GtkMenuItem *menuitem, gpointer data) /* Measurements->DATA LINK */
{
	GROUP_VAL(data_link) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_3151 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_CURSORS_POS(index) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_CURSORS_POS(index) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_324 (GtkSpinButton *spinbutton, gpointer data) /* entry_qty P324 */
{
	set_cur_entry (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton)));
}

void data_330 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Thickness->source p330 */
{
	GROUP_VAL(source) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_331 (GtkSpinButton *spinbutton, gpointer data) /*min_thickness p331 */
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_min_thickness(pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_min_thickness(pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 ));
}

void data_332 (GtkSpinButton *spinbutton, gpointer data) /*max_thickness p332 */
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_max_thickness(pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_max_thickness(pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 ));
}

void data_333 (GtkSpinButton *spinbutton, gpointer data) /*echo_qty p333 */
{
	set_echo_qty(pp->p_config, (guchar) (gtk_spin_button_get_value (spinbutton)));
}

void data_400 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->display p400 */
{
	set_display_pos (pp->p_config, (guchar)(GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	switch (get_display_pos(pp->p_config))
	{
		case A_SCAN:
			break;
		case B_SCAN:
			pp->bscan_mark = 1;
			break;
		case C_SCAN:
			pp->cscan_mark = 1;
			break;
		case S_SCAN:
			pp->sscan_mark = 1;
			break;
		case A_B_SCAN:
			pp->bscan_mark = 1;
			break;
		case A_B_C_SCAN:
			pp->bscan_mark = 1;
			pp->cscan_mark = 1;
			break;
		case A_B_S_SCAN:
			pp->bscan_mark = 1;
			pp->sscan_mark = 1;
			break;
		case A_C_CC_SCAN:
			pp->cscan_mark = 1;
			pp->ccscan_mark = 1;
			break;
		case A_S_CC_SCAN:
			pp->sscan_mark = 1;
			pp->cccscan_mark = 1;
			/*			pp->cscan_mark = 1;*/
			break;
		default:break;
	}
	//generate_focallaw(get_current_group (pp->p_config));
	draw_menu3 (0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_401 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection Ascan->group p401 */
{
	set_display_group (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	/*	gtk_widget_queue_draw (pp->vboxtable);*/
}

void data_4011 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->C-Scan1 地一个Cscan p401 */
{
	set_cscan_source (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), 0);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	pp->sscan_mark = 1;
}

void data_4012 (GtkMenuItem *menuitem, gpointer data) /* ASC显示模式后时候Cscan的source P401 */
{
	set_cscan_source (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), 0);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_4013 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->Data1 p401 */
{
	set_stripscan_data1 (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_402 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->C-Scan2 p402 */
{
	set_cscan_source (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), 1);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}
void data_4021 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->C-Scan2 p402 */
{
	set_stripscan_data2 (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_403 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->Mode p403 */
{
	set_stripscan_mode (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_404 (GtkSpinButton *spinbutton, gpointer data) /*Display Range P404 */
{
	set_stripscan_disrange (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton)*100.0));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_405 (GtkSpinButton *spinbutton, gpointer data) /*Avg.Scan Speed P405 */
{
}

void data_410 (GtkMenuItem *menuitem, gpointer data) /* Display->Overlay->UT Unit p410 */
{
	GROUP_VAL(ut_unit) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_411 (GtkMenuItem *menuitem, gpointer data) /* 选择栅格颜色  P411 */
{
	set_overlay_grid (pp->p_config,(guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	gtk_widget_queue_draw (pp->vboxtable);
	draw_menu3(0, NULL);
}

void data_420 (GtkMenuItem *menuitem, gpointer data) /* Display->Zoom->Display p420 */
{
}

void data_421 (GtkMenuItem *menuitem, gpointer data) /* Display->Zoom->Type p421 */
{
}

void data_422 (GtkSpinButton *spinbutton, gpointer data) /* start USound p422 */
{
}

void data_4221 (GtkSpinButton *spinbutton, gpointer data) /* start USound p422 */
{
}

void data_423 (GtkSpinButton *spinbutton, gpointer data) /* end USound p423 */
{
}

void data_4231 (GtkSpinButton *spinbutton, gpointer data) /* range USound p423 */
{
}

void data_424 (GtkSpinButton *spinbutton, gpointer data) /* start Amplitude p424 */
{
}

void data_4241 (GtkSpinButton *spinbutton, gpointer data) /* center Amplitude p424 */
{
}

void data_425 (GtkSpinButton *spinbutton, gpointer data) /* end Amplitude p425 */
{
}

void data_4251 (GtkSpinButton *spinbutton, gpointer data) /* range Amplitude p4251 */
{
}

void data_430 (GtkMenuItem *menuitem, gpointer data) /* Display->Color->select p430 */
{
	GROUP_VAL(col_select_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_431 (GtkSpinButton *spinbutton, gpointer data) /*color_start p431 */
{
	GROUP_VAL(col_start) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_4311 (GtkSpinButton *spinbutton, gpointer data) /*color_contrast */
{
	GROUP_VAL(col_contrast) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}
void data_4312 (GtkSpinButton *spinbutton, gpointer data) /*color_min */
{
	GROUP_VAL(col_min) =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}

void data_432 (GtkSpinButton *spinbutton, gpointer data) /*color_end */
{
	GROUP_VAL(col_end) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_4321 (GtkSpinButton *spinbutton, gpointer data) /*color_brightness */
{
	GROUP_VAL(col_brightness) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}
void data_4322 (GtkSpinButton *spinbutton, gpointer data) /*color_max */
{
	GROUP_VAL(col_max) =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}

void data_434 (GtkMenuItem *menuitem, gpointer data) /* Display->color->mode 434 */
{
	GROUP_VAL(col_mode) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_440 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->scan 440 */
{
	set_dis_prop_scan (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_441 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Color 441 */
{
	GROUP_VAL(ascan_color) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_4411 (GtkSpinButton *spinbutton, gpointer data) /* Display->Properties->Compress 4411 */
{
	set_b_compress (pp->p_config, (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0));
}

void data_4414 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Color P4414 */
{
	set_fft_color (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_4415 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Color P4415 */
{
	set_dis_prop_strip_orientation (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_442 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Envelope 442 */
{
	GROUP_VAL(ascan_envelope) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_443 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Source 443 */
{
	GROUP_VAL(ascan_source) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_444 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Appearance 444 */
{
	GROUP_VAL(ascan_appearance) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_445 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Overlay 445 */
{
	GROUP_VAL(ascan_overlay) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_500 (GtkMenuItem *menuitem, gpointer data) /* 增加删除选择group P500 */
{
	guchar temp = (guchar)(GPOINTER_TO_UINT (data));
	gchar *markup;
	gint grp;
	GROUP *p_grp;
	switch (temp)
	{
		case 0:
			set_group_qty (pp->p_config, get_group_qty(pp->p_config) + 1);
			grpcpy (pp->p_config, get_group_qty(pp->p_config) - 1, 0);
			set_current_group (pp->p_config, get_group_qty(pp->p_config) - 1, 1);
			break;						/* 增加*/
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			/* 把参数切换到当前选择的group 未完成 */
			set_current_group(pp->p_config, temp - 1, 1);
			break;
		case 9:
			set_group_qty (pp->p_config, get_group_qty(pp->p_config) - 1);
			set_current_group (pp->p_config, get_group_qty(pp->p_config) - 1, 1);
			break;
		default:break;
	}

	grp = get_current_group(pp->p_config);
	p_grp = get_group_by_id (pp->p_config, grp);
	if (get_group_db_ref (pp->p_config, grp))
		markup = g_markup_printf_escaped (
				"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
				(get_group_val (p_grp, GROUP_GAIN) -
				 get_group_val (p_grp, GROUP_GAINR)) / 100.0,
				get_group_val (p_grp, GROUP_GAINR) / 100.0);
	else
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
				get_group_val (p_grp, GROUP_GAIN) / 100.0 );
	gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);

	g_free(markup);

	if(GROUP_VAL(group_mode)!=PA_SCAN) /*group mode 选择UT,UT1,UT2时，focal law 不可用*/
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
	else
		gtk_widget_set_sensitive(pp->menuitem[6],TRUE);


	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_501 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Select->Group Mode 501 */
{
	gint temp_prf ;
	gint group = get_current_group(pp->p_config);
	GROUP_VAL(group_mode) = (gchar) (GPOINTER_TO_UINT (data));
	TMP(group_spi[group]).UT2			= (GROUP_VAL_POS (group, group_mode) == 3) ? 1 : 0;		
	TMP(group_spi[group]).UT1			= (GROUP_VAL_POS (group, group_mode) == 2) ? 1 : 0;		
	TMP(group_spi[group]).PA			= (GROUP_VAL_POS (group, group_mode) == 1) ? 1 : 0;		
	/*group mode 选择UT,UT1,UT2时，focal law 不可用*/
	if(GROUP_VAL(group_mode)!=1) 
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
	else
		gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	if(GROUP_VAL(group_mode) == 1) //PA
	{
		TMP(group_spi[group]).rx_time	= TMP(group_spi[group]).sample_range  + TMP(max_beam_delay[group]) + TMP(group_spi[group]).compress_rato;
		temp_prf = TMP(beam_qty[group]) * GROUP_VAL_POS(group, prf1);
		TMP(group_spi[group]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[group]).rx_time;
	}
	else  //UT
	{
		TMP(group_spi[group]).rx_time	= TMP(group_spi[group]).sample_range + TMP(group_spi[group]).compress_rato;
		temp_prf = TMP(beam_qty[group]) * GROUP_VAL_POS(group, prf1);
		TMP(group_spi[group]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[group]).rx_time;
	}
	send_group_spi(group);
	send_focal_spi(group);

}

void data_502 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Select->Select 502 */
{
	set_probe_select (pp->p_config, (gboolean) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_510 (GtkSpinButton *spinbutton, gpointer data) /*scanoffset */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_VAL(scan_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
		GROUP_VAL(scan_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_511 (GtkSpinButton *spinbutton, gpointer data) /*indexoffset */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		GROUP_VAL(index_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
		GROUP_VAL(index_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
}

void data_5121 (GtkSpinButton *spinbutton, gpointer data) /* Skew (deg) */
{
	GROUP_VAL(skew) =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_512 (GtkMenuItem *menuitem, gpointer data) /* Skew (deg) */
{
	gushort temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(skew_pos) = (guchar) (GPOINTER_TO_UINT (data));
	GROUP_VAL(skew) = get_skew();
	if (temp != 4)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[2]), GROUP_VAL(skew)/100.0);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[2]));
	}
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_521 (GtkSpinButton *spinbutton, gpointer data) /*gain */
{
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	set_group_val (p_grp, GROUP_START, (gshort) (gtk_spin_button_get_value (spinbutton) * 10.0));
}

void data_522 (GtkSpinButton *spinbutton, gpointer data) /*agate_start */
{
	GROUP_VAL(agate_start) =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_523 (GtkSpinButton *spinbutton, gpointer data) /*agate_width */
{
	GROUP_VAL(agate_width) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_530 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Parts->Geometry 530 */
{
	set_part_geometry (pp->p_config, data);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_531 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_part_thickness (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_part_thickness (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_532 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_part_diameter (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_part_diameter (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
}

/* P533 选择被测物的材料 */
void data_533 (GtkMenuItem *menuitem, gpointer data) 
{
	set_part_material (pp->p_config, data);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_534 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Parts -> Weld 534 */
{
	set_part_weld (pp->p_config, data);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
/* 聚焦法则类型 P600 */
void data_600 (GtkMenuItem *menuitem, gpointer data) 
{
	request_refresh() ;

	LAW_VAL(Focal_type) = (guchar) (GPOINTER_TO_UINT (data));
	if (LAW_VAL(Focal_type) != LINEAR_SCAN)
	{
		LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
	}
	if (LAW_VAL(Focal_type) == DEPTH_SCAN)
	{
		LAW_VAL (Focal_point_type) = HALFPATH_P;
	}
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	//gtk_widget_queue_draw (pp->vboxtable);
}

void data_601 (GtkSpinButton *spinbutton, gpointer data) /* connection_P P601 */
{
}

/* min_angle P610 */
void data_610 (GtkSpinButton *spinbutton, gpointer data)
{
	request_refresh() ;

	if(gtk_widget_get_sensitive(pp->eventbox30[1]))
	{
		gtk_spin_button_set_value (spinbutton, 
				rounding(LAW_VAL(Angle_max),
					(gint)(gtk_spin_button_get_value (spinbutton) * 100.0), 
					(LAW_VAL(Angle_step) + 5) / 10 * 10) / 100.0);
		LAW_VAL(Angle_min) = (gshort)
			(gtk_spin_button_get_value (spinbutton) * 100.0);
	}
	else
	{
		LAW_VAL(Angle_min) = (gshort)
			(gtk_spin_button_get_value (spinbutton) * 100.0);
	}
	//防止最大角比最小角要小 引起断错误
	if(LAW_VAL(Angle_min) > LAW_VAL(Angle_max))
	{
		LAW_VAL(Angle_max) = LAW_VAL(Angle_min);
	}
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}


/* max_angle P611 */
void data_611 (GtkSpinButton *spinbutton, gpointer data) 
{
	request_refresh() ;

	if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
	{
		gtk_spin_button_set_value (spinbutton, 
				rounding(LAW_VAL(Angle_min),
					(gint)(gtk_spin_button_get_value (spinbutton) * 100.0), 
					(LAW_VAL(Angle_step) + 5) / 10 * 10) / 100.0);
		LAW_VAL(Angle_max) = (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
	}
	pp->last_angle = LAW_VAL(Angle_max);
	if(pp->clb_flag)
		draw_area_calibration();
	else
		draw_area_all();
}

/* Angle Step P612 */
void data_612 (GtkSpinButton *spinbutton, gpointer data) 
{
	request_refresh() ;

	if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
	{
		LAW_VAL(Angle_step) = (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
		LAW_VAL(Angle_max) = rounding(LAW_VAL(Angle_min),
				LAW_VAL(Angle_max),	(LAW_VAL(Angle_step) + 5) / 10 * 10);
	}
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}


/* beam skew min 2D 的偏斜角min P613 */
void data_613 (GtkSpinButton *spinbutton, gpointer data) 
{
	LAW_VAL(Angle_beam_skew_min) = (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

/* beam skew max 2D 的偏斜角min P614 */
void data_614 (GtkSpinButton *spinbutton, gpointer data) 
{
	LAW_VAL(Angle_beam_skew_max) = (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}
# if 0
void data_6141 (GtkMenuItem *menuitem, gpointer data) /* 纵横波  P614 */
{
	LAW_VAL(Wave_type) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
#endif


/* focalpoint 聚焦点计算方法 P620 */
void data_620 (GtkMenuItem *menuitem, gpointer data) 
{
    request_refresh() ;

    LAW_VAL(Focal_point_type) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_621 (GtkSpinButton *spinbutton, gpointer data) /* Position start P621 */
{
	request_refresh() ;
	LAW_VAL(Position_start) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_6211 (GtkSpinButton *spinbutton, gpointer data) /* Offset start P621 */
{
	request_refresh() ;
	LAW_VAL(Offset_start) = (guint) (gtk_spin_button_get_value (spinbutton)* 1000.0);
}

void data_622 (GtkSpinButton *spinbutton, gpointer data) /* Angle Step P622 */
{
	request_refresh() ;
	LAW_VAL(Position_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_6221 (GtkSpinButton *spinbutton, gpointer data) /* Angle Step P622 */
{
	request_refresh() ;
	LAW_VAL(Offset_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

# if 0
void data_623 (GtkSpinButton *spinbutton, gpointer data) /* focus_depth P623*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		LAW_VAL(Focus_depth) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		LAW_VAL(Focus_depth) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}
#endif

void data_623 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P623 */
{
	request_refresh() ;
	LAW_VAL(Position_step) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_6231 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P623 */
{
	request_refresh() ;
	LAW_VAL(Depth_start) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_624 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P624 */
{
	request_refresh() ;
	LAW_VAL(Depth_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

/* element_qty 聚集 法则一次激发的阵元数量 P620 */
void data_630 (GtkSpinButton *spinbutton, gpointer data) 
{
	request_refresh() ;
	gint	grp	= get_current_group(pp->p_config);
	LAW_VAL (Elem_qty) = (guchar) (gtk_spin_button_get_value (spinbutton));
	if(LAW_VAL(Focal_type) == 0)//Azimuthal
		LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
	else//Linear
	{
		if( LAW_VAL (Last_tx_elem) < ((guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1) )
			LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
	}

	/*计算聚焦法则时，sumgain默认为Auto*/
	if (LAW_VAL_POS(grp, Elem_qty) == 1)	
		TMP(group_spi[grp]).sum_gain	= 4095;	
	else 
		TMP(group_spi[grp]).sum_gain	= 
			4096 / LAW_VAL_POS(grp, Elem_qty) ;
}

/* first_element 第一个接收阵元 */
void data_631 (GtkSpinButton *spinbutton, gpointer data) 
{
	request_refresh() ;
	LAW_VAL(First_tx_elem) =  (guchar) (gtk_spin_button_get_value (spinbutton));
	if(LAW_VAL(Focal_type) == 0)//Azimuthal
		LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
	else//Linear
	{
		if( LAW_VAL (Last_tx_elem) < ((guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1) )
			LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
	}
}
/* last_element 最后一个阵元编号 */
void data_632 (GtkSpinButton *spinbutton, gpointer data) 
{
	request_refresh() ;
	if( LAW_VAL (Last_tx_elem) < ((guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1) )
		LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
	else
		LAW_VAL(Last_tx_elem) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_633 (GtkSpinButton *spinbutton, gpointer data) /*element_step*/
{
	request_refresh() ;
	LAW_VAL(Elem_step) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

/* focalpoint 聚焦点计算方法 P620 */
void data_634 (GtkMenuItem *menuitem, gpointer data) 
{
	guint temp = (guchar) (GPOINTER_TO_UINT (data));
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	if (temp == 0)
		set_group_val (p_grp, GROUP_VELOCITY, get_material_lw (pp->p_config));
	else if (temp == 1) 
		set_group_val (p_grp, GROUP_VELOCITY, get_material_sw (pp->p_config));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_700 (GtkMenuItem *menuitem, gpointer data) /* Encoder */
{
	set_cur_encoder (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_701 (GtkMenuItem *menuitem, gpointer data) /* Polarity */
{
	set_enc_polarity (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), get_cur_encoder (pp->p_config));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_702 (GtkMenuItem *menuitem, gpointer data) /* Type */
{
	set_enc_type (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), get_cur_encoder (pp->p_config));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_703 (GtkSpinButton *spinbutton, gpointer data) /* Resolution */
{
	set_enc_resolution (pp->p_config, 
			(guint) (gtk_spin_button_get_value (spinbutton) * 1000.0), get_cur_encoder (pp->p_config));
}

void data_704 (GtkSpinButton *spinbutton, gpointer data) /* Origin */
{
	gint enc_id = get_cur_encoder(pp->p_config);
	if(	!(pp->p_config->encoder1[enc_id].Resolution))
		set_enc_origin (pp->p_config, 0.0, enc_id) ;
	else
		set_enc_origin (pp->p_config, 
				(guint) (gtk_spin_button_get_value (spinbutton) * 1000.0), enc_id);
}

void data_710 (GtkMenuItem *menuitem, gpointer data) /* Scan->Inspection->type */
{
	set_inspec_type (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	if( GPOINTER_TO_UINT (data) == 2)
	{
		if( pp->count == 0)
			set_part_geometry (pp->p_config, (void*)(1) );
		pp->count++;
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
	//gtk_widget_queue_draw (pp->vboxtable);
}

void data_711 (GtkMenuItem *menuitem, gpointer data) /* Scan->Inspection->scan */
{
	char *markup;
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	set_inspec_source (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));

	if(get_inspec_source (pp->p_config)==0)
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f s</span>",
				(gfloat)(get_group_val (p_grp, GROUP_PRF_VAL)));
	else
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f mm</span>",
				(gfloat)(get_group_val (p_grp, GROUP_PRF_VAL)));
	gtk_label_set_markup (GTK_LABEL (pp->label[7]), markup); 

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->vboxtable);
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();

}

void data_712 (GtkMenuItem *menuitem, gpointer data) /* Scan->Inspection->Index */
{
	set_inspec_index (pp->p_config,(guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_713 (GtkSpinButton *spinbutton, gpointer data) /*scan_speed*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_inspec_speed (pp->p_config,	(guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_inspec_speed (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 ));
}

void data_714 (GtkSpinButton *spinbutton, gpointer data) /*scan_speed*/
{
	set_inspec_rpmspeed (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 10.0));
}

void data_715 (GtkSpinButton *spinbutton, gpointer data) /*index_speed*/
{
	set_inspec_indexspeed (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 10.0));
}

void data_720 (GtkSpinButton *spinbutton, gpointer data) /* P720 scan_start */
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_scanstart (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_scanstart (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 ));

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_721 (GtkSpinButton *spinbutton, gpointer data) /* P721 scan_end*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_scanend (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_scanend (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 ));

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_722 (GtkSpinButton *spinbutton, gpointer data) /* P722 scan_resolution*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_scanresolution (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_scanresolution (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937));
}

void data_723 (GtkSpinButton *spinbutton, gpointer data) /* P723 index_start*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_indexstart (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_indexstart (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937));
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_724 (GtkSpinButton *spinbutton, gpointer data) /* P724 index_end*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_indexend (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_indexend (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937));
	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_725 (GtkSpinButton *spinbutton, gpointer data) /* P725 index_resolution*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_indexresolution (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_indexresolution (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937));
}

void data_730 (GtkMenuItem *menuitem, gpointer data) /* Scan->start->start mode */
{
	set_start_mode (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_740 (GtkMenuItem *menuitem, gpointer data) /* Scan->Data->storage */
{
	set_data_storage (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_741 (GtkMenuItem *menuitem, gpointer data) /* P741  Scan->Data->inspec.data */
{
	set_data_inspec_data (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_800 (GtkMenuItem *menuitem, gpointer data) /* P800 File->File->Storage */
{
	set_file_storage (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}


void data_810 (GtkMenuItem *menuitem, gpointer data) /* P810 File->Report->Template */
{
	set_report_template (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_804 (GtkMenuItem *menuitem, gpointer data) /* File->File->Save Mode */
{
	set_file_save_mode (pp->p_config, (gchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_812 (GtkMenuItem *menuitem, gpointer data) /* P812 File->Report->paper size */
{
	set_report_paper_size (pp->p_config, (gchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_824 (GtkMenuItem *menuitem, gpointer data) /* File->Format->view*/
{
	set_report_format_view (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_830(GtkMenuItem *menuitem, gpointer data) /* P830 File->User Field->select */
{
	set_report_userfield_select (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_900(GtkMenuItem *menuitem, gpointer data) /* Preferences->Pref.->Units */
{
	set_unit (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	if(!pp->clb_flag)
		draw_area_all();
	else
		draw_area_calibration();
}

void data_903(GtkMenuItem *menuitem, gpointer data) /* Preferences->Pref.->scheme */
{
}

void data_904(GtkMenuItem *menuitem, gpointer data) /* Preferences->Pref.->gate mode */
{
}

void data_912 (GtkMenuItem *menuitem, gpointer data)
{
	set_language (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	//draw_menu3(0, NULL);

	change_language (get_language(pp->p_config), pp);
	draw_menu1();
	draw_menu2(0);
	draw_menu3(0, NULL);
}

void data_913(GtkMenuItem *menuitem, gpointer data) /* Preferences->system->assign key*/
{
}

void data_9131(GtkMenuItem *menuitem, gpointer data) /* Preferences->system->assign key*/
{
}

void data_930(GtkMenuItem *menuitem, gpointer data) /* Preferences->system->assign key*/
{
}

void data_901 (GtkSpinButton *spinbutton, gpointer data) /*scan_resolution*/
{
	set_bright (pp->p_config, (guchar) (gtk_spin_button_get_value (spinbutton)));
}

/* Edit Notes 2个按键的处理 一个是保存一个是取消 */
void da_call_edit_notes (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTextBuffer *TextBuffer = (GTK_TEXT_BUFFER (user_data));
	if (GTK_RESPONSE_OK == response_id)  /* 保存信息 */
	{
		GtkTextIter start, end;
		gchar *Data;

		gtk_text_buffer_get_start_iter (TextBuffer, &start);
		gtk_text_buffer_get_end_iter (TextBuffer, &end);

		Data = gtk_text_buffer_get_text (TextBuffer, &start, &end, FALSE);
		set_edit_notes_info (pp->p_config, Data);

		g_free (Data);
		g_print ("OK_Pressed");
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
		g_print ("CANCEL_Pressed");
	gtk_widget_destroy (GTK_WIDGET (dialog));
	change_keypress_event (KEYPRESS_MAIN);
}

/* Edit Header 2个按键的处理 一个是保存一个是取消 */
void da_call_edit_header (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTextBuffer *TextBuffer = (GTK_TEXT_BUFFER (user_data));
	if (GTK_RESPONSE_OK == response_id)  /* 保存信息 */
	{
		GtkTextIter start, end;
		gchar *Data;

		gtk_text_buffer_get_start_iter (TextBuffer, &start);
		gtk_text_buffer_get_end_iter (TextBuffer, &end);

		Data = gtk_text_buffer_get_text (TextBuffer, &start, &end, FALSE);
		set_header_info (pp->p_config, Data);

		g_free (Data);
		g_print ("OK_Pressed");
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
		g_print ("CANCEL_Pressed");
	gtk_widget_destroy (GTK_WIDGET (dialog));
	change_keypress_event (KEYPRESS_MAIN);
}

/* File name 2个按键的处理 一个是保存一个是取消 */
void da_call_file_name (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTextBuffer *TextBuffer = (GTK_TEXT_BUFFER (user_data));
	if (GTK_RESPONSE_OK == response_id)  /* 保存信息 */
	{
		GtkTextIter start, end;
		gchar *Data;

		gtk_text_buffer_get_start_iter (TextBuffer, &start);
		gtk_text_buffer_get_end_iter (TextBuffer, &end);

		Data = gtk_text_buffer_get_text (TextBuffer, &start, &end, FALSE);
		set_file_name_info (pp->p_config, Data);
		gtk_label_set_text (GTK_LABEL (pp->data3[1]), get_file_name_info (pp->p_config));

		g_free (Data);
		g_print ("OK_Pressed");
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
		g_print ("CANCEL_Pressed");
	gtk_widget_destroy (GTK_WIDGET (dialog));
	change_keypress_event (KEYPRESS_MAIN);
}

void dialog_destroy(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	gtk_widget_destroy (GTK_WIDGET(data));
	change_keypress_event (KEYPRESS_MAIN);
}


void send_spi_data (gint group)
{
	write_group_data (&TMP(group_spi[group]), group);
}

//*********生存聚焦法则************
//  2011.6.25 何凡添加
//*********生存聚焦法则************
void generate_focallaw(int grp)
{
	gint	i, j;
	guint	temp_beam;
	gint    k, offset;
	gint step;
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	GROUP	*p_grp = get_group_by_id (pp->p_config, grp);

	temp_beam = 1;
	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		i = (LAW_VAL_POS(grp, Angle_max) - LAW_VAL_POS(grp, Angle_min)) /
			LAW_VAL_POS(grp, Angle_step) + 1;
		j = (LAW_VAL_POS(grp, Angle_beam_skew_min) - LAW_VAL_POS(grp, Angle_beam_skew_max)) /
			LAW_VAL_POS(grp, Angle_beam_skew_step) + 1;
		temp_beam = i * j;
		step = (gint)( (LAW_VAL_POS(grp, Angle_max) - LAW_VAL_POS(grp, Angle_min)) / LAW_VAL_POS(grp, Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		temp_beam = (gint)( ( LAW_VAL_POS (grp, Last_tx_elem) - LAW_VAL_POS(grp, First_tx_elem) - LAW_VAL_POS(grp, Elem_qty) + 1 ) /
				LAW_VAL_POS(grp, Elem_step) ) + 1;
		step = (gint)( ( LAW_VAL_POS (grp, Last_tx_elem)-LAW_VAL_POS(grp, First_tx_elem) - LAW_VAL_POS(grp, Elem_qty) + 1 ) /
				LAW_VAL_POS(grp, Elem_step) ) + 1;
	}
	else if(LAW_VAL (Focal_type) == DEPTH_SCAN) 
	{
		temp_beam = 1;
		step = 0;
	}
	else if(LAW_VAL (Focal_type) == STATIC_SCAN) 
	{
		temp_beam = 1;
		step = 0;
	}

	TMP(beam_qty[grp])	= temp_beam;
	TMP(beam_num[grp]) = 0;
	TMP(current_angle[grp]) = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
	TMP(group_spi[grp]).point_qty = GROUP_VAL_POS(grp, point_qty);

	cal_focal_law (grp);
	/*计算聚焦法则时，sumgain默认为Auto*/
	if (LAW_VAL_POS(grp, Elem_qty) == 1)	
		TMP(group_spi[grp]).sum_gain	= 4095;	
	else 
		TMP(group_spi[grp]).sum_gain	= 
			4096 / LAW_VAL_POS(grp, Elem_qty) ;
	TMP(group_spi[grp]).sample_start	= (get_group_val (p_grp, GROUP_START) +
			get_group_val (p_grp, GROUP_WEDGE_DELAY)) / 10 ;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start +
		get_group_val (p_grp, GROUP_RANGE) / 10 ;
	
	update_gate_info();
	if(!pp->clb_count)
	{
		//每次计算之后gain_offset清零
		for (i = 0; i < step; i++)
		{
			pp->tmp_gain_off[i + offset] = 0;
		}
	}
	if(!pp->clb_flag)
		draw_area_all ();
	else
		draw_area_calibration();

	pp->cscan_mark = 1;
	pp->ccscan_mark = 1;
	pp->cccscan_mark = 1;
	pp->sscan_mark = 1;	

}

//****************************************
//  编码器校准：2011.7.1 何凡
//****************************************
gfloat cba_encoder()
{
	gint K = 483;
	TMP_CBA(delt_distance) = pp->distance ;//- get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config));
	TMP_CBA(delt_measure) = TMP_CBA(measure_end) - TMP_CBA(measure_start);
	TMP_CBA(resolution) = K*TMP_CBA(delt_measure)/TMP_CBA(delt_distance);

	return TMP_CBA(resolution);
}

//****************************************
//  声速校准：2011.7.1 何凡
//****************************************
gfloat cba_ultrasound_velocity()
{
	if(TMP_CBA(time_start) != TMP_CBA(time_end))
	{
		switch(pp->echotype_pos)
		{
			case 0://Radius
				TMP_CBA(velocity_last) = 200000*fabs(TMP_CBA(radius2)-TMP_CBA(radius1)) / fabs(TMP_CBA(time_end)-TMP_CBA(time_start)) ;
				break;
			case 1://Depth
				TMP_CBA(velocity_last) = 200000*fabs(TMP_CBA(depth2)-TMP_CBA(depth1)) / fabs(TMP_CBA(time_end)-TMP_CBA(time_start)) ;
				break;
			case 2://Thickness
				TMP_CBA(velocity_last) = 200000*fabs(TMP_CBA(thickness2)-TMP_CBA(thickness1)) / fabs(TMP_CBA(time_end)-TMP_CBA(time_start)) ;
				break;
		}
		return TMP_CBA(velocity_last);//单位m/s
	}
	else
		return 0;
}

//****************************************
//  延时校准：2011.7.1 何凡
//****************************************
void cba_ultrasound_wedgedelay()
{
	gint i, step;
	gfloat s, vel, t1, wedge_delay;
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}

	vel = (get_group_val (p_grp, GROUP_VELOCITY) / 100.0);// m/s
	t1  = 2000000 * TMP_CBA(distance) / vel ;//ns
	wedge_delay = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_WEDGE_DELAY) / 1000.0;
	for (i = 0; i < step; i++)
	{
		s   = (TMP(clb_wedge_data[i]) - wedge_delay - pp->G_delay[i] - get_pw()) * vel / 2000000;//mm
		if(s<0) s = 0;
		//		printf("s=%f dis=%f \n", s, TMP_CBA(distance));
		if( fabs( TMP_CBA(distance) - s ) >  (pp->tolerance / 100.0) )
		{
			pp->G_delay[i] = (gint)(fabs(TMP(clb_wedge_data[i]) - t1 - get_pw()));
		}
	}
}

//****************************************
//  灵敏度校准：2011.7.1 何凡
//****************************************
void cba_ultrasound_sensitivity()
{
	gint i;
	gint offset,k,step;
	GtkWidget* dialog;
	GtkWindow *win = GTK_WINDOW (pp->window);

	gint count = 0;
	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}
	gint grp = get_current_group(pp->p_config);
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	TMP(clb_his_max_data) = 0;
	for (i = 0; i < step; i++)
	{
		//*****************************************
		if(TMP(clb_max_data[i]) >= 100.0)
		{	
			count++ ;
			if(count == 1)
			{
				dialog = gtk_message_dialog_new( win,
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_CLOSE,
						"At least one context has an amplitude peak of %s \nThe calibration cannot be performed",
						"100%" );
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
			}
			//		return 0;//最好能弹出一个警告框
		}
		else if(TMP(clb_max_data[i]) <= 0.0)
		{	
			count++ ;
			if(count == 1)
			{
				dialog = gtk_message_dialog_new( win,
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_CLOSE,
						"At least one context has an amplitude peak of %s \nThe calibration cannot be performed",
						"0%" );
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
			}
			//		return 0;//最好能弹出一个警告框
		}
		//*****************************************
		if(TMP(clb_his_max_data) < TMP(clb_real_data[i]))
			TMP(clb_his_max_data) = TMP(clb_real_data[i]);
	}
	for (i = 0; i < step; i++)
	{
		pp->tmp_gain_off[i + offset] = fabs( 20*log10(TMP(clb_real_data[i])/TMP(clb_his_max_data)) );
		TMP(clb_max_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;//清除包络线
	}
#if ARM											
	send_focal_spi(grp);
#endif										

}

//****************************************
//  TCG校准：2011.7.1 何凡
//****************************************
void cba_ultrasound_TCG()
{

}

void esc_calibration()
{
	if(!pp->ctype_pos)//当位Encoder时无需更新扫描
	{
		pp->clb_encoder = 0;
	}
	else
	{
		GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) = pp->save_ut_unit;
		pp->clb_count = 0;
	}
	pp->clb_flag = 0;
	pp->pos1[pp->pos] = 2;
	if(pp->cstart_qty != 1)
		pp->cstart_qty = 1;
	gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
	gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
	if(GROUP_VAL(group_mode)==PA_SCAN)
		gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
	else
		gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);
	gtk_widget_set_sensitive(pp->menubar,TRUE);
	draw_area_all();
}

void draw_encoder_value(gpointer data)
{
	DRAW_UI_P pp = (DRAW_UI_P) data;
	gchar* markup;
	gint offset,k;
	gint grp = get_current_group(pp->p_config);
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	gint index = offset + TMP(beam_num[grp]);	

	while(1)
	{
		pthread_mutex_lock(&qlock);
		pthread_cond_wait(&qready, &qlock);
		pthread_mutex_unlock(&qlock);

		TMP_CBA(measure_end) = TMP(measure_data[index][4]);
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f mm</span>",
				(TMP_CBA(measure_end) - TMP_CBA(measure_start)) );
		gtk_label_set_markup (GTK_LABEL (pp->label[7]), markup); 
	}

}

static int thread_set_DB_eighty_percent(gpointer data)
{
	DRAW_UI_P pp = (DRAW_UI_P) data;
	int i = 15;
	int k;
	int offset = 0;
	float scale   ;
	//group_data_spi *p1;

	int grp = pp->p_config->groupId ;   //当前group
	//memcpy (&new, &TMP(group_spi[grp]), sizeof (group_data_spi));
	//p1 = &TMP(group_spi[grp]) ;
	//p1->offset = 16 * grp;
	//p1->addr = 0x2;

	for (k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	int index = offset + TMP(beam_num[grp]);

	while(i)
	{
		//printf("thread run times %d \n", i)  ;
		if(fabs(DO_NOT_USE_CCFG(measure_data[index]).a_height-80.0) <=1 )  break  ;
		//printf("gate is %f", DO_NOT_USE_CCFG(measure_data[index]).a_height);
		scale =  80.0/DO_NOT_USE_CCFG(measure_data[index]).a_height  ;
		//printf("\n**********  %d   ******\n", i);
		//printf("a_height = %f \n", DO_NOT_USE_CCFG(measure_data[index]).a_height);
		//printf("scale = %f \n", scale) ;
		//printf("pre gain = %d \n" , GROUP_VAL(gain));
		GROUP_VAL (gain) =  GROUP_VAL (gain) + (short)(log10(scale)*2000) ;
		//printf("post gain = %d \n" , GROUP_VAL(gain));
		if(GROUP_VAL(gain) > 8000)  {GROUP_VAL(gain) = 8000; break;}
		TMP(group_spi[grp]).gain = GROUP_VAL (gain) / 10;
		//printf("gain %d\n", TMP(group_spi[grp]).gain);
		send_group_spi (grp);
		i--;
		draw_field_value ();
		usleep(400000);
	}
	return 0;
}

void update_gate_info()
{
	gfloat delay;
	guint offset, k;
	gint grp = get_current_group(pp->p_config);
	guint beam_qty =TMP(beam_qty[grp]);
	gint temp_prf ;
	// get current beam Number
	double current_angle ;
	double max_angle     ;
	int BeamNo = pp->p_tmp_config->beam_num[grp];

	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	if(LAW_VAL(Focal_type) == 0)
	{
		current_angle = LAW_VAL(Angle_min)/100.0 + BeamNo * LAW_VAL(Angle_step)/100.0 ;
		max_angle = MAX(fabs(LAW_VAL(Angle_min)), fabs(LAW_VAL(Angle_max))) * G_PI / 180.0 ;
	}
	else
	{
		current_angle = LAW_VAL(Angle_min)/100.0 ;
		max_angle = LAW_VAL(Angle_min) * G_PI / 180.0 ;
	}
	if(GROUP_VAL(group_mode) == 1) //PA
		current_angle = current_angle * G_PI / 180.0 ;
	else
		current_angle = 0 ;

	for (k = offset; k < offset + beam_qty; k++)//k:每个beam
	{
		delay = pp->G_delay[k - offset];
		if(LAW_VAL(Focal_point_type) == 1)//true depth
		{ 
			if (GROUP_VAL(gate_pos) == GATE_A)
			{
				pp->gate_a_start[k]	= (int)( (GROUP_GATE_POS(start) + delay) / (10*cos(current_angle)) );
				pp->gate_a_end[k]	= (int)( (GROUP_GATE_POS(start) + GROUP_GATE_POS(width) + delay) / (10*cos(current_angle)) );
			}
			else if (GROUP_VAL(gate_pos) == GATE_B)
			{ 
				pp->gate_b_start[k]	= (int)( (GROUP_GATE_POS(start) + delay) / (10*cos(current_angle)) );
				pp->gate_b_end[k]	= (int)( (GROUP_GATE_POS(start) + GROUP_GATE_POS(width) + delay) / (10*cos(current_angle)) );
			}
			else if (GROUP_VAL(gate_pos) == GATE_I)
			{
				pp->gate_i_start[k]	= (int)( (GROUP_GATE_POS(start) + delay) / (10*cos(current_angle)) );
				pp->gate_i_end[k]	= (int)( (GROUP_GATE_POS(start) + GROUP_GATE_POS(width) + delay) / (10*cos(current_angle)) );
			}
		}
		else //half path
		{
			if (GROUP_VAL(gate_pos) == GATE_A)
			{
				pp->gate_a_start[k]	= (int)( (GROUP_GATE_POS(start) + delay) / 10 );
				pp->gate_a_end[k]	= (int)( (GROUP_GATE_POS(start) + GROUP_GATE_POS (width) + delay) / 10 );
			}
			else if (GROUP_VAL(gate_pos) == GATE_B)
			{ 
				pp->gate_b_start[k]	= (int)( (GROUP_GATE_POS(start) + delay) / 10 );
				pp->gate_b_end[k]	= (int)( (GROUP_GATE_POS(start) + GROUP_GATE_POS (width) + delay) / 10 );
			}
			else if (GROUP_VAL(gate_pos) == GATE_I)
			{
				pp->gate_i_start[k]	= (int)( (GROUP_GATE_POS(start) + delay) / 10 );
				pp->gate_i_end[k]	= (int)( (GROUP_GATE_POS(start) + GROUP_GATE_POS (width) + delay) / 10 );
			}
		}

	}

	if(GROUP_VAL(group_mode) == 1) //PA
	{
		TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range  + TMP(max_beam_delay[grp]) + TMP(group_spi[grp]).compress_rato;
		temp_prf = TMP(beam_qty[grp]) * GROUP_VAL_POS(grp, prf1);
		TMP(group_spi[grp]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	}
	else  //UT
	{
//		grp = 0;
		TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range + TMP(group_spi[grp]).compress_rato;
		temp_prf = TMP(beam_qty[grp]) * GROUP_VAL_POS(grp, prf1);
		TMP(group_spi[grp]).idel_time	=
			100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	}

}


