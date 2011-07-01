
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include "drawuif.h"		/* 计算聚焦法则的头文件 */
#include "base_config.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>

extern void focal_law(gpointer data,gint *TimeDelay);/*回调函数*/

static int handler_key(guint keyval, gpointer data);

extern const gchar ****con2_p;

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

gboolean key_press_handler (GtkWidget* pWidget,
		GdkEventKey* pEvent, gpointer data);

void send_dsp_data (guint data_type, guint value);
void send_spi_data (gint group);
guint get_beam_qty();
guint get_skew();
guint get_freq ();
guint get_pw ();
guint get_prf ();
guint get_max_prf();
guint get_filter ();
guint get_max_point_qty();

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

void data_121 (GtkMenuItem *menuitem, gpointer data);        /* 121 Filter */
void data_122 (GtkMenuItem *menuitem, gpointer data);        /* 122 Rectifier */
void data_124 (GtkMenuItem *menuitem, gpointer data);        /* 124 Averaging */
void data_125 (GtkSpinButton *spinbutton, gpointer data); 

void data_130 (GtkSpinButton *spinbutton, gpointer data);
void data_131 (GtkSpinButton *spinbutton, gpointer data);
void data_132 (GtkSpinButton *spinbutton, gpointer data);
void data_134 (GtkSpinButton *spinbutton, gpointer data);
void data_135 (GtkSpinButton *spinbutton, gpointer data);

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
void data_221 (GtkMenuItem *menuitem, gpointer data);         /* Output -> alarm # */
void data_2211 (GtkMenuItem *menuitem, gpointer data);         /* Output -> group */
void data_222 (GtkSpinButton *spinbutton, gpointer data);	/* Output -> count */
void data_2221 (GtkMenuItem *menuitem, gpointer data);		/* Output -> data */
void data_223 (GtkMenuItem *menuitem, gpointer data);          /* 223 sound */
void data_224 (GtkSpinButton *spinbutton, gpointer data);
void data_225 (GtkSpinButton *spinbutton, gpointer data);
void data_230 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm -> Sizing Curves -> Mode 230 */
void data_231 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm -> Sizing Curves -> Curve 231 */
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

void data_300 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> Reading -> list 300 */
void data_302 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> Reading -> Field1 302 */
void data_303 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> Reading -> Field2 303 */
void data_304 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> Reading -> Field3 304 */
void data_305 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> Reading -> Field4 305 */

void data_310 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> cursors -> selection 310 */
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

void data_330 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> Thickness -> source 330 */
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

static void setup_para(PARAMETER_P p, guint group)
{
	/* 探头 */
	p->probe_p->D1_D2 = 0;			/* 0 1d 1 2d*/
	p->probe_p->Pitch_Catch = !GROUP_VAL_POS(group, tx_rxmode);	/* 0 PE 1 PT 与我的相反 */
	p->probe_p->transmi_trans_longi = 0;	/* 1 SW 横波慢 0 LW 纵波快 */
	p->probe_p->recev_trans_longi	= 0;	/* */
	p->probe_p->pb_skew_angle	= GROUP_VAL_POS(group, skew) / 100.0;	/* ffff*/
	p->probe_p->pb_frequency	= GROUP_VAL_POS(group, frequency) / 1000.0;	/* ffff*/
	p->probe_p->ele_num_pri = GROUP_VAL_POS(group, probe.Elem_qty);	/* */
	p->probe_p->ele_num_sec = 1;	/* 2D 时候就不为1了 */
	p->probe_p->pri_axis_pitch = GROUP_VAL_POS(group, probe.Pitch) / 1000.0;	/* */
	p->probe_p->sec_axis_pitch = 0;	/* 2D时候有效 */
	p->probe_p->pri_ele_size = 1;	/* ffff*/
	p->probe_p->sec_ele_size = 0;	/* 2D 时候有效 */

//	p->probe_p->pb_skew_angle_r = 0;	/*ffff */
//	p->probe_p->pb_frequency = 5.0;	/*ffff */
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
    p->wedge_p->wg_pri_elem_offset_fir = 0;/*第一主轴阵元偏移mm*/
    p->wedge_p->wg_sec_elem_offset_fir = 0;/*第一次轴阵元偏移mm*/
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
//    p->wedge_r->wg_density= 7.8;/* 密度 */
    p->wedge_r->wg_heigh_fir	= GROUP_VAL_POS(group, wedge.Height) / 1000.0;	/*第一阵元高度mm*/
    p->wedge_r->wg_pri_elem_offset_fir = 0;/*第一主轴阵元偏移mm*/
    p->wedge_r->wg_sec_elem_offset_fir = 0;/*第一次轴阵元偏移mm*/
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

    p->beam_angle->beam_type = 0;
    p->beam_angle->beam_angle_sel = 0;

	/* 样本 */
    p->specimen->speci_longitudinal_wave	= GROUP_VAL_POS (group, velocity) / 100.0;	/*样本纵波声速 */  
    p->specimen->speci_transverse_wave		= GROUP_VAL_POS (group, velocity) / 100.0;	/*样本横波声速 */
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
    p->focal_point->focal_focus_type = 1;	/* 0 half path 1 TURE DEPTH */
    p->focal_point->focal_focus_point_start = LAW_VAL_POS (group, Focus_depth) / 1000.0;	/* type =0 是 声程 type =1 是深度 */
    p->focal_point->focal_focus_point_stop = 0; 
    p->focal_point->focal_focus_point_resolution = 0; 
    
    p->element_sel->pri_axis_ape = LAW_VAL_POS (group, Elem_qty);
    p->element_sel->sec_axis_ape = 1;
}

static void save_cal_law(gint offset, gint group, PARAMETER_P p)
{
	gint i, j;
	for (i = 0; i < TMP(beam_qty[group]); i++)
	{
		TMP(focal_law_all_beam[offset + i]).N_ActiveElements	= LAW_VAL_POS (group, Elem_qty);
		TMP(focal_law_all_beam[offset + i]).frequency		= GROUP_VAL_POS (group, frequency);
		TMP(focal_law_all_beam[offset + i]).cycle			= 1; 
		TMP(focal_law_all_beam[offset + i]).sumgain			= GROUP_VAL_POS (group, sum_gain) / 100;
		TMP(focal_law_all_beam[offset + i]).mode			= !GROUP_VAL_POS (group, tx_rxmode);
		TMP(focal_law_all_beam[offset + i]).filter			= GROUP_VAL_POS (group, filter); 
		TMP(focal_law_all_beam[offset + i]).R_angle			= (LAW_VAL_POS (group, Angle_min) + 
				LAW_VAL_POS(group, Angle_step) * i) / 100.0;
		TMP(focal_law_all_beam[offset + i]).S_angle			= GROUP_VAL_POS (group, skew) / 100.0; 
		TMP(focal_law_all_beam[offset + i]).T_first			= LAW_VAL_POS (group, First_tx_elem);
		TMP(focal_law_all_beam[offset + i]).R_first			= LAW_VAL_POS (group, First_rx_elem);
		TMP(focal_law_all_beam[offset + i]).Scan_offset		= GROUP_VAL_POS (group, scan_offset) * 100;
		TMP(focal_law_all_beam[offset + i]).Index_offset	= GROUP_VAL_POS (group, index_offset) * 100;
		TMP(focal_law_all_beam[offset + i]).G_delay			= 
			GROUP_VAL_POS (group, wedge_delay)
			+	GROUP_VAL_POS (group, wedge.Probe_delay) + p->G_Time[i];
		TMP(focal_law_all_beam[offset + i]).F_depth			= LAW_VAL_POS (group, Focus_depth);
		TMP(focal_law_all_beam[offset + i]).M_velocity		= GROUP_VAL_POS (group, velocity) / 100;


		for (j = 0; j < TMP(focal_law_all_beam[offset + i]).N_ActiveElements; j++)
		{
			TMP(focal_law_all_elem[offset + i][j]).E_number = j + 1;
			TMP(focal_law_all_elem[offset + i][j]).FL_gain	= GROUP_VAL_POS (group, gain) / 100;
			TMP(focal_law_all_elem[offset + i][j]).T_delay	= p->timedelay[i][j];
			TMP(focal_law_all_elem[offset + i][j]).R_delay	= p->timedelay[i][j];
			TMP(focal_law_all_elem[offset + i][j]).Amplitude = CFG (voltage_pa); 
			TMP(focal_law_all_elem[offset + i][j]).P_width	= GROUP_VAL_POS (group, pulser_width) / 100; 
		} 

	} 
}

void cal_focal_law (guint group)
{
	gint G_Delay[256*33];
	gint offset, k;
	PARAMETER_P p;
	
	p = (PARAMETER_P)g_malloc0(sizeof(PARAMETER));
//	p = (PARAMETER_P)malloc(sizeof(PARAMETER));
    PROBEF        probe_p;
    WEDGEF        wedge_p;
	WEDGEF        wedge_r;
    BEAM_ANGLE   beam_angle;
    SPECIMEN     specimen;
    FOCAL_POINT  focal_point;
    ELEMENT_SEL  element_sel;

    p->probe_p	= &probe_p;
	p->wedge_p	= &wedge_p;
	p->wedge_r	= &wedge_r;
	p->beam_angle = &beam_angle;
	p->specimen	= &specimen;
	p->focal_point = &focal_point;
	p->element_sel = &element_sel;
	p->k = 0 ;	            
    
	setup_para(p, group);
    
	focal_law(p, G_Delay);

	
	/* 把聚集法则信息保存起来 */
	for (offset = 0, k = 0 ; k < group; k++)
		offset += TMP(beam_qty[k]);
/*	g_print("offset = %d group=%d beam_qty = %d\n", 
			offset, group, TMP(beam_qty[group]));
			*/
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
	for (i = 0; i < setup_MAX_GROUP_QTY; i++)
		beam_qty += TMP(beam_qty[i]);
	return beam_qty;
}

/*  */
guint get_sum_gain ()
{
	if (GROUP_VAL(sum_gain_pos))
		return GROUP_VAL(sum_gain);
	else 
	{
		if (LAW_VAL(Elem_qty) == 1)
			GROUP_VAL(sum_gain) = 4800;
		else
/*			GROUP_VAL(sum_gain) = (46.1 - log10(LAW_VAL(Elem_qty))) * 100;*/
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

/* 取得压缩点的数量 */
guint get_point_qty ()
{
	if (GROUP_VAL(point_qty_pos) == 4)
		return GROUP_VAL(point_qty);
	else 
	{
		switch (GROUP_VAL(point_qty_pos))
		{
			case 0:GROUP_VAL(point_qty) = MIN(get_max_point_qty(), 605);break;
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
	guint i, point_qty = 0 ;
	for (i = 0; i < setup_MAX_GROUP_QTY; i++)
		point_qty += TMP(beam_qty[i]) * (GROUP_VAL_POS (i, point_qty) + 32);
//	return ((192000 / (point_qty + 32)) * 25);
	return 250;
}

/* 计算prf,并且附加限制 限制计算 */
guint get_prf ()
{
	guint prf_temp = get_max_prf();
	/*
	range_all =	GROUP_VAL(start) + GROUP_VAL(range);
	gate_range_all[0] = GROUP_VAL(gate[0].start) + GROUP_VAL(gate[0].width);
	gate_range_all[1] = GROUP_VAL(gate[1].start) + GROUP_VAL(gate[1].width);
	gate_range_all[2] = GROUP_VAL(gate[2].start) + GROUP_VAL(gate[2].width);
	prf_temp = 	MAX((MAX(range_all, gate_range_all[0])), (MAX(gate_range_all[1], gate_range_all[2])));
	prf_temp = (guint)((1000000 / 4) / (prf_temp /1000.0));
	(prf_temp > 20000) ? (prf_temp = 20000) : (prf_temp = prf_temp) ;
	TMP(max_prf) = prf_temp;
	*/
	if (GROUP_VAL(prf_pos) == 3)
	{
		if (GROUP_VAL(prf) > prf_temp * 10)
			GROUP_VAL(prf) = prf_temp * 10;
	}
	else 
	{
		switch (GROUP_VAL(prf_pos))
		{
			case 0:
				GROUP_VAL(prf) = prf_temp * 10;
				break;
			case 1:
				GROUP_VAL(prf) = prf_temp * 5;
				break;
			case 2:
				GROUP_VAL(prf) = (prf_temp > 60 ) ? 600 : prf_temp * 10;
				break;
			default:break;
		}
	}
	return GROUP_VAL(prf);
}

/* 计算脉冲宽度 */
guint get_pw ()
{
	if (GROUP_VAL(pw_pos))
		return GROUP_VAL(pulser_width);
	else
		return ((guint)(GROUP_VAL(frequency) * 2.0) / 250)  * 250 ; /* 改变脉冲宽度 */
}

/* 计算滤波 0 1 None 和 Auto 时候怎么计算 */
guint get_filter ()
{
	switch (GROUP_VAL(filter))
	{
		case 0:break;
		case 1:break;
		default:break;
	}
	return GROUP_VAL(filter);
}

guint get_freq ()
{
	switch (GROUP_VAL(freq_pos))
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
	return GROUP_VAL(frequency);
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
			draw_menu2(0);
			draw_menu3(0, NULL);
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
	(*data > 0) ? (*data)-- : (*data = pa);
	return ;
}

/*6个数值区域共有的处理函数 */
/* 第一个数值按键 快捷键是F12  */
void b3_fun0(gpointer pt)
{
	DRAW_UI_P p = (DRAW_UI_P) (pt);
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
					break;
				case 3:break;
				default:break;
			}
			break;
		case 1: /* UT Settings*/
			switch (p->pos1[1])
			{
				case 4: 
					g_print ("set 80%% \n");
					break; /* P140 自动80%  */
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 2: 
					CFG(display_table) = !CFG(display_table); /* P320 */
					break; 
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 2: 
					CFG(fft) = !CFG(fft); /* P520 */
					break; 
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 4: 
					CFG(auto_program) = !CFG(auto_program); /* P640 */
					break; 
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 2: 
					CFG(format_userfield) = !CFG(format_userfield); /* P820 */
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

	}

	p->pos_pos = MENU3_PRESSED;
	draw_menu2 (0);
	draw_menu3 (0, p);                          /**/

	return ;
}

void b3_fun1(gpointer p)
{
	gchar *markup;
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 1;
	/*	pp->pos_pos = MENU3_PRESSED;*/

	/* 一次点击处理 */
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
					case 0:((pp->start_qty) < 9) ? (pp->start_qty) ++ : ((pp->start_qty) = 1); break;
					case 1:((pp->fstart_qty) < 6) ? (pp->fstart_qty) ++ : ((pp->fstart_qty) = 1); break;
					case 2:
						switch(pp->ctype_pos)
						{
							case 0:
								((pp->cstart_qty) < 4) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
								break;
							case 1:
								switch(pp->cmode_pos)
								{
									case 0:
										((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
										break;
									case 1:
										((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
										break;
									case 2:
										((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
										break;
									case 3:
										((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
										break;
									default:break;
								}
								break;
							case 2:
								((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
								break;
							default:break;
						}
							break;
					case 3:break;
					default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 4: 
					GROUP_VAL(gainr) = GROUP_VAL(gain);
					pp->pos_pos = MENU3_STOP;
					if (GROUP_VAL(db_ref))
						markup = g_markup_printf_escaped (
								"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
								(GROUP_VAL(gain) - GROUP_VAL(gainr)) / 100.0, GROUP_VAL(gainr) / 100.0);
					else
						markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",	GROUP_VAL(gain) / 100.0 );
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
					CFG(entry_image)= !CFG(entry_image);/* P321 */
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 4:
					switch(CFG(prop_scan)) 
					{
						case 0:break;
						case 1:break;
						case 2:CFG(ratio)= !CFG(ratio);break;
						case 3:CFG(interpolation) = !CFG(interpolation);break;
						default:break;
					}
					break;  
					/* P441 */
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 3: 
					CFG(pause)= !CFG(pause);/* P731 */
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					CFG(format_probe)= !CFG(format_probe);/* P821 */
					break;
				case 3: 
					CFG(enable)= !CFG(enable);/* P831 */
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
					   case 2: break; 
					   case 3:
							   switch(GROUP_VAL(col_select_pos)) 
							   {
								   case 0:data_process(&(TMP(color_start_reg)), 1 );break;
								   case 1:data_process(&(TMP(color_contrast_reg)), 1 );break;
								   case 2:data_process(&(TMP(min_reg)), 2);break;
							   }
							   break; 

					   case 4:
							   switch(CFG(prop_scan)) 
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
					   case 2: data_process (&(pp->p_tmp_config->db_reg), 2);  break;     /*521*/
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
					GROUP_VAL(db_ref) = !GROUP_VAL(db_ref);
					pp->pos_pos = MENU3_STOP;
					if (GROUP_VAL(db_ref))
					{
						tt_label_show_string (pp->label[GAIN_LABEL], con2_p[1][0][7], "\n", "(dB)", "white", 10);
						markup = g_markup_printf_escaped (
								"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
								(GROUP_VAL(gain) - GROUP_VAL(gainr)) / 100.0, GROUP_VAL(gainr) / 100.0);
					}
					else
					{
						tt_label_show_string (pp->label[GAIN_LABEL], con2_p[1][0][0], "\n", "(dB)", "white", 10);
						markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
								GROUP_VAL(gain) / 100.0 );
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
					CFG(sizing_curves) = !CFG(sizing_curves);
					break; /* p412 */
				case 4: 
					CFG(optimum) = !CFG(optimum);
					break; /* p442 */
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					CFG(format_setup)= !CFG(format_setup);/* P822 */
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 3: 
					CFG(remote_desktop)= !CFG(remote_desktop);/* P932 */
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
						case 3:break;
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
							   if (CFG_ZOOM_POS(zoom_type) == 0 || CFG_ZOOM_POS(zoom_type) == 1)
								   data_process(&(TMP(start_usound_reg)), 2);
							   else if (CFG_ZOOM_POS(zoom_type) == 2)
								   data_process(&(TMP(center_usound_reg)), 2);
							   break;  /*p422 */
					   case 3: 
							   if (GROUP_VAL(col_select_pos) == 0)
								   data_process(&(TMP(color_end_reg)), 1);  /*432 */
							   else if (GROUP_VAL(col_select_pos) == 1)
								   data_process(&(TMP(brightness_reg)), 1);
							   else if (GROUP_VAL(col_select_pos) == 2)
								   data_process(&(TMP(max_reg)), 2);
							   break;  /*432 */
					   case 4: break;
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

	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/

	return ;
}

void b3_fun3(gpointer p)
{
	gint	i, j;
	guint	temp_beam;
	gint	grp = CFG (groupId);
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 3;

	switch (pp->pos)
	{
		case 1:
			switch (pp->pos1[1])
			{
				case 2: 
					GROUP_VAL(video_filter) = !GROUP_VAL(video_filter);
					TMP(group_spi[grp]).video_filter	= GROUP_VAL_POS(grp, video_filter);
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
					CFG(overlay_gate) = !CFG(overlay_gate);
					break; /* p413 */
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 4:
					temp_beam = 1;
					if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
					{
						i = (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) /
							LAW_VAL(Angle_step) + 1;
						j = (LAW_VAL(Angle_beam_skew_min) - LAW_VAL(Angle_beam_skew_max)) /
							LAW_VAL(Angle_beam_skew_step) + 1;
						temp_beam = i * j;
					}
					else  
					{

					}

					TMP(beam_qty[get_current_group(pp->p_config)])	= temp_beam;
					TMP(beam_num[get_current_group(pp->p_config)]) = 0;

					TMP(group_spi[get_current_group(pp->p_config)]).beam_qty = TMP(beam_qty[get_current_group(pp->p_config)]) - 1; 
					TMP(group_spi[grp]).idel_time		= 
						100000000 / (GROUP_VAL_POS(grp, prf) / 10) - 2048 - TMP(group_spi[grp]).rx_time;
					write_group_data (&TMP(group_spi[get_current_group(pp->p_config)]), get_current_group(pp->p_config));
					cal_focal_law (get_current_group(pp->p_config));
					send_focal_spi (get_current_group(pp->p_config));
					pp->cscan_mark = 1;
					pp->ccscan_mark = 1;
					pp->cccscan_mark = 1;
					pp->sscan_mark = 1;
					break;  /* 计算聚焦法则 P643 */
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					CFG(format_note)= !CFG(format_note);/* P823 */
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
											data_process (&(TMP(last_angle_reg)), 2);
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
						case 3:break;
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
													  data_process(&(pp->p_tmp_config->ref_ampl_offset_reg), 3); /* 233 ref.ampl.offset  */
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
					   case 2: /*423*/
							   if(CFG_ZOOM_POS(zoom_type) == 0) 
								   data_process(&(TMP(end_usound_reg)), 2);
							   else if( CFG_ZOOM_POS(zoom_type) == 1 || CFG_ZOOM_POS(zoom_type) == 2 )
								   data_process(&(TMP(range_usound_reg)), 2);
							   break;

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

	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:	/* p004 */
 						if ( pp->start_qty == 5 )
 							CFG(auto_detect) = !CFG(auto_detect);
 						break;
				case 1:break;
				case 2:break;
				case 3:break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					CFG(overlay_cursor) = !CFG(overlay_cursor);
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
						case 3:break;
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
					   case 3: data_process(&(pp->p_tmp_config->curve_step_reg), 4);  break; /* 234  */
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
					   case 2:/*424*/
							  if(CFG_ZOOM_POS(zoom_type) == 0 || CFG_ZOOM_POS(zoom_type) == 1) 
								  data_process(&(TMP(start_amplitude_reg)), 1);
							  else if(CFG_ZOOM_POS(zoom_type) == 2 )
								  data_process(&(TMP(center_amplitude_reg)), 1);
							  break;
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

	switch (pp->pos)
	{
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					CFG(overlay_overlay) = !CFG(overlay_overlay);
					gtk_widget_queue_draw (pp->vboxtable);
					break; /* p415 */
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0: 
					CFG(auto_detect) = !CFG(auto_detect);
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
					   case 2:/*425*/
							  if(CFG_ZOOM_POS(zoom_type) == 0) 
								  data_process(&(TMP(end_amplitude_reg)), 1);
							  else if(CFG_ZOOM_POS(zoom_type) == 1 || CFG_ZOOM_POS(zoom_type) == 2 )
								  data_process(&(TMP(range_amplitude_reg)), 1);
							  break;
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
	switch (keyval) 
	{
		case GDK_KP_0:
			(TMP(beam_num[get_current_group(pp->p_config)]) < (TMP(beam_qty[get_current_group(pp->p_config)]) - 1))	?
				(TMP(beam_num[get_current_group(pp->p_config)]) += 1)	:	
			(TMP(beam_num[get_current_group(pp->p_config)]) = 0);
			g_print("beam num =%d\n", TMP(beam_num[get_current_group(pp->p_config)]));
			draw_area_all ();
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
					break;
				case MENU2_PRESSED:
					pp->pos_pos = MENU3_STOP;
					break;
				case MENU3_STOP:
					if(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS]))
						pp->pos_pos = MENU3_PRESSED;
					else
						pp->pos_pos = MENU3_STOP;

					/* 按下的动作在这里实现 */
					break;
				case MENU3_PRESSED:
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
			break;

		case GDK_F1:
			if(pp->help_yn == 1)
				show_help(HELP_N);/*隐藏帮助窗口*/
			else if(pp->help_yn == 0)
				show_help(HELP_Y);/*弹出帮助窗口*/
			pp->help_yn = !pp->help_yn;
			break;
		case GDK_F2:
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
			break;
		case GDK_F8:
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
			break;
		case GDK_F9:
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
			break;
		case GDK_F10:
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
			break;
		case GDK_F11:
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
			break;
		case GDK_F12:
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
	return TRUE;
}

void data_0021 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Group -> Operation */
{
	pp->operation_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0022 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Group -> Group */
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
	g_tmp_group_struct.pulser =  (guchar) (gtk_spin_button_get_value (spinbutton));
	if (g_tmp_group_struct.tx_rxmode == PULSE_ECHO)
		g_tmp_group_struct.receiver =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送给硬件*/
}

void data_00242 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode P111 */
{
	g_tmp_group_struct.tx_rxmode = (gchar) (GPOINTER_TO_UINT (data));
	if ( g_tmp_group_struct.tx_rxmode == PULSE_ECHO )
		g_tmp_group_struct.receiver = g_tmp_group_struct.pulser;
	pp->pos_pos = MENU3_STOP;
	send_dsp_data (TX_RX_MODE_DSP,g_tmp_group_struct.tx_rxmode);
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0025 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Select -> Select 502 */
{
	pp->p_config->probe_select = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0027 (GtkSpinButton *spinbutton, gpointer data) /*scanoffset */
{
	if(CFG(unit) == UNIT_MM)
	{
		g_tmp_group_struct.scan_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	}
	else
	{
		g_tmp_group_struct.scan_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
	}

	draw_area_all ();
}

void data_00341 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	g_tmp_group_struct.receiver =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送给硬件*/
}

void data_00342 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	g_tmp_group_struct.pulser =  (guchar) (gtk_spin_button_get_value (spinbutton));
	if ((g_tmp_group_struct.tx_rxmode) == PULSE_ECHO)
		(g_tmp_group_struct.receiver) =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送给硬件*/
}

void data_0037 (GtkSpinButton *spinbutton, gpointer data) /*indexoffset */
{
	if(CFG(unit) == UNIT_MM)
		g_tmp_group_struct.index_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
		g_tmp_group_struct.index_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
}

void data_0044 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	g_tmp_group_struct.receiver =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送给硬件*/
}

void data_00471 (GtkSpinButton *spinbutton, gpointer data) /* Skew (deg) */
{
	g_tmp_group_struct.skew =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_00472 (GtkMenuItem *menuitem, gpointer data) /* Skew (deg) */
{
	gushort temp = GPOINTER_TO_UINT (data);
	g_tmp_group_struct.skew_pos = (guchar) (GPOINTER_TO_UINT (data));
	g_tmp_group_struct.skew = get_skew();
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

void data_022 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Calibration -> Type */
{
	pp->ctype_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0221 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Calibration -> Echo Type */
{
	pp->echotype_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}
void data_0222 (GtkSpinButton *spinbutton, gpointer data) /* wizard  First Angle  */
{
	pp->first_angle =  (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}
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

void data_023 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Calibration -> Mode */
{
	pp->cmode_pos = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0231 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Calibration -> Select code */
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
}

void data_0235 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Radius A */
{
	pp->radiusa =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_0236 (GtkSpinButton *spinbutton, gpointer data) /* wizard  depth A */
{
	pp->deptha =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
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
	gint gain = ((gint)(gtk_spin_button_get_value(spinbutton) * 100) + 5) / 10 * 10;
	gtk_spin_button_set_value (spinbutton, gain / 100.0);

	if (GROUP_VAL(db_ref))
		GROUP_VAL(gain) = (gushort) (gtk_spin_button_get_value (spinbutton) * 100 + GROUP_VAL(gainr));
	else
		GROUP_VAL(gain) = (gushort) (gtk_spin_button_get_value (spinbutton) * 100);

	if (GROUP_VAL(db_ref))
		markup = g_markup_printf_escaped (
				"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
				(GROUP_VAL(gain) - GROUP_VAL(gainr)) / 100.0, GROUP_VAL(gainr) / 100.0);
	else
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
				GROUP_VAL(gain) / 100.0 );
	gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);

	g_free(markup);
	TMP(group_spi[grp]).gain = GROUP_VAL (gain) / 10;
	write_group_data (&TMP(group_spi[grp]), grp);

	/* 发送给硬件 */
}

void data_101 (GtkSpinButton *spinbutton, gpointer data) /*Start 扫描延时 P101 */
{
	gint tt[4];
	gint grp = get_current_group(pp->p_config);
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ; 

 	draw_area_all ();
	TMP(group_spi[grp]).sample_start	= (GROUP_VAL_POS (grp, start) + 
		GROUP_VAL_POS(grp, wedge_delay)) / 10;		
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		GROUP_VAL_POS(grp, range) / 10;		
	tt[0] = (GROUP_VAL_POS(grp, gate[0].start) +	GROUP_VAL_POS (grp, gate[0].width));
	tt[1] = (GROUP_VAL_POS(grp, gate[1].start) +	GROUP_VAL_POS (grp, gate[1].width));
	tt[2] = (GROUP_VAL_POS(grp, gate[2].start) +	GROUP_VAL_POS (grp, gate[2].width));

	tt[3] = MAX(tt[0], (MAX(tt[1],tt[2]))) / 10;

	TMP(group_spi[grp]).rx_time		= MAX (tt[3], TMP(group_spi[grp]).sample_range);
	TMP(group_spi[grp]).idel_time		= 
		100000000 / (GROUP_VAL_POS(grp, prf) / 10) - 2048 - TMP(group_spi[grp]).rx_time;
	send_spi_data (grp);
	/*发送给硬件*/
}

void data_102 (GtkSpinButton *spinbutton, gpointer data) /*Range 范围 P102 */
{
	gint grp = get_current_group(pp->p_config);
	gint tt[4];
	gint range = ((gint)(gtk_spin_button_get_value(spinbutton) * 1000) + 5) / 10 * 10;

	gtk_spin_button_set_value (spinbutton, rounding(0, range, TMP(range_step_min)) / 1000.0);
	
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(range) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(range) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(range) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 

 	draw_area_all ();
	TMP(group_spi[grp]).compress_rato	= 
		((GROUP_VAL_POS(grp, range) / 10.0) / GROUP_VAL_POS(grp, point_qty)) > 1 ? 
		((GROUP_VAL_POS(grp, range) / 10.0) / GROUP_VAL_POS(grp, point_qty)) : 1;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		GROUP_VAL_POS(grp, range) / 10;		
	tt[0] = (GROUP_VAL_POS(grp, gate[0].start) + GROUP_VAL_POS (grp, gate[0].width));
	tt[1] = (GROUP_VAL_POS(grp, gate[1].start) + GROUP_VAL_POS (grp, gate[1].width));
	tt[2] = (GROUP_VAL_POS(grp, gate[2].start) + GROUP_VAL_POS (grp, gate[2].width));

	tt[3] = MAX(tt[0], (MAX(tt[1],tt[2]))) / 10;
	TMP(group_spi[grp]).rx_time		= MAX (tt[3], TMP(group_spi[grp]).sample_range);
	TMP(group_spi[grp]).idel_time		= 
		100000000 / (GROUP_VAL_POS(grp, prf) / 10) - 2048 - TMP(group_spi[grp]).rx_time;

	send_spi_data (grp);
	/*发送给硬件*/
}

void data_103 (GtkSpinButton *spinbutton, gpointer data) /*楔块延时  P103 */
{
	gint tt[4];
	gint grp = get_current_group(pp->p_config);
	GROUP_VAL(wedge_delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;

	TMP(group_spi[grp]).sample_start	= (GROUP_VAL_POS (grp, start) + 
		GROUP_VAL_POS(grp, wedge_delay)) / 10;		
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		GROUP_VAL_POS(grp, range) / 10;		
	tt[0] = (GROUP_VAL_POS(grp, gate[0].start) +	GROUP_VAL_POS (grp, gate[0].width));
	tt[1] = (GROUP_VAL_POS(grp, gate[1].start) +	GROUP_VAL_POS (grp, gate[1].width));
	tt[2] = (GROUP_VAL_POS(grp, gate[2].start) +	GROUP_VAL_POS (grp, gate[2].width));

	tt[3] = MAX(tt[0], (MAX(tt[1],tt[2]))) / 10;

	TMP(group_spi[grp]).rx_time		= MAX (tt[3], TMP(group_spi[grp]).sample_range);
	TMP(group_spi[grp]).idel_time		= 
		100000000 / (GROUP_VAL_POS(grp, prf) / 10) - 2048 - TMP(group_spi[grp]).rx_time;
	send_spi_data (grp);
	/*发送给硬件*/
}

void data_104 (GtkSpinButton *spinbutton, gpointer data) /*声速 P104 */
{
	if (UNIT_MM == CFG(unit))
		GROUP_VAL(velocity) = (guint) (gtk_spin_button_get_value (spinbutton) * 100);
	else   /* 英寸/微秒 */
		GROUP_VAL(velocity) = (guint) (gtk_spin_button_get_value (spinbutton) * 25400 * 100 );

 	draw_area_all ();
	/*发送给硬件*/
}

void data_110 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	GROUP_VAL(pulser) =  (guchar) (gtk_spin_button_get_value (spinbutton));
	if (GROUP_VAL(tx_rxmode) == PULSE_ECHO)
		GROUP_VAL(receiver) =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送给硬件*/
}

void data_120 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	GROUP_VAL(receiver) =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送给硬件*/
}

void data_111 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode P111 */
{
	GROUP_VAL(tx_rxmode) = (gchar) (GPOINTER_TO_UINT (data));
	if (GROUP_VAL(tx_rxmode) == PULSE_ECHO )
		GROUP_VAL(receiver) = GROUP_VAL(pulser);
	pp->pos_pos = MENU3_STOP;
	send_dsp_data (TX_RX_MODE_DSP, GROUP_VAL(tx_rxmode));
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_1121 (GtkSpinButton *spinbutton, gpointer data) /* 频率 Freq 数值改变 */
{
	GROUP_VAL(frequency) =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
	if (!GROUP_VAL(pw_pos))
		GROUP_VAL(pulser_width) = ((guint)(GROUP_VAL(frequency) * 2.0) / 250)  * 250 ; /* 改变脉冲宽度 */
	send_dsp_data (FREQUENCY_DSP, (guint) (GROUP_VAL(frequency)));
	/* 发送给硬件 */
}

void data_112 (GtkMenuItem *menuitem, gpointer data) /* 频率 Freq P112 */
{
	guint temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(freq_pos) = (guchar) (GPOINTER_TO_UINT (data));
	GROUP_VAL(frequency) = get_freq();
	if (!GROUP_VAL(pw_pos))
		GROUP_VAL(pulser_width) = GROUP_VAL(frequency) * 2.0; /* 改变脉冲宽度 */
	if (temp != 12)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		send_dsp_data (FREQUENCY_DSP, (guint) (GROUP_VAL(frequency)));
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
	}
	/* 发送给硬件 */
}

void data_113 (GtkMenuItem *menuitem, gpointer data)  /* Voltage  P113 */
{
	gint grp = get_current_group (pp->p_config);
	set_voltage (pp->p_config, grp, (guchar)(GPOINTER_TO_UINT (data)));

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	TMP(group_spi[grp]).voltage = get_voltage (pp->p_config, grp);	
	send_spi_data (grp);
	/* 发送给硬件 */
}

void data_1141 (GtkSpinButton *spinbutton, gpointer data) /* PW  P114 */
{
	guint temp;
	temp =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 100.0);
	temp = (temp / 250) * 250;
	GROUP_VAL(pulser_width) = temp;

	send_focal_spi (get_current_group(pp->p_config));
	/* 发送给硬件 */
}

void data_114 (GtkMenuItem *menuitem, gpointer data) /* PW */
{
	guint temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(pw_pos) = temp;
	GROUP_VAL(pulser_width) = get_pw();
	if (temp == AUTO_SET)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		send_dsp_data (PW_DSP, GROUP_VAL(pulser_width) / 250) ;  /* 以2.5ns 为单位发出信息 */
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
	}
	send_focal_spi (get_current_group(pp->p_config));
	/* 发送给硬件 */
}

void data_1151 (GtkSpinButton *spinbutton, gpointer data) /* PRF P115 */
{
	gchar *markup;
	gint grp = get_current_group(pp->p_config);
	GROUP_VAL(prf) =  (guint) (gtk_spin_button_get_value (spinbutton) * 10);
	markup=g_markup_printf_escaped(
			"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",GROUP_VAL(prf) / 10, (GROUP_VAL(prf) / 10) * 1);
	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
	g_free(markup);

	markup = g_markup_printf_escaped (
			"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",(gfloat)(GROUP_VAL(prf) / 10.0));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;

	g_free(markup);
	if (GROUP_VAL_POS(grp, prf)  >= 250 )
		/*
		TMP(group_spi[grp]).idel_time		= 
			100000000 / (GROUP_VAL_POS(grp, prf) / (10.0 * CFG(prf_compress))) - 2048 - TMP(group_spi[grp]).rx_time;
			*/
	GROUP_VAL_POS(grp, prf) = 250;
	
	TMP(group_spi[grp]).idel_time		= 
		100000000 / (GROUP_VAL_POS(grp, prf) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;

	g_print ("ideal_time = %d\n", TMP(group_spi[grp]).idel_time);

	write_group_data (&TMP(group_spi[grp]), grp);
	/* 发送给硬件 */
}

void data_115 (GtkMenuItem *menuitem, gpointer data) /* PRF */
{
	guint temp = GPOINTER_TO_UINT (data);
	gint grp = get_current_group(pp->p_config);
	GROUP_VAL(prf_pos) = temp;
	GROUP_VAL(prf) = get_prf();			/*  */
	if (temp != 3)
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

	if (GROUP_VAL_POS(grp, prf)  >= 250)
		/*
		TMP(group_spi[grp]).idel_time		= 
			100000000 / (GROUP_VAL_POS(grp, prf) / (10.0 * CFG(prf_compress))) - 2048 - TMP(group_spi[grp]).rx_time;
			*/
	GROUP_VAL_POS(grp, prf) = 250;
	TMP(group_spi[grp]).idel_time		= 
		100000000 / (GROUP_VAL_POS(grp, prf) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	/* 如何 */
	write_group_data (&TMP(group_spi[grp]), grp);

}

/* filter 滤波 P121 */
void data_121 (GtkMenuItem *menuitem, gpointer data)  
{
	gint grp = get_current_group(pp->p_config);
	GROUP_VAL(filter) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
/*	send_dsp_data (FILTER_DSP, get_filter());*/
	/* 发送给硬件 */
	if (GROUP_VAL_POS(grp, filter) == 0)
	{
		TMP(group_spi[grp]).freq_band	= 0;
	}
	else if (GROUP_VAL_POS(grp, filter) == 1)
	{
		if (GROUP_VAL_POS(grp, frequency) < 1250)
			TMP(group_spi[grp]).freq_band	= 1;
		else if (GROUP_VAL_POS(grp, frequency) < 1750)
			TMP(group_spi[grp]).freq_band	= 2;
		else if (GROUP_VAL_POS(grp, frequency) < 2125)
			TMP(group_spi[grp]).freq_band	= 3;
		else if (GROUP_VAL_POS(grp, frequency) < 3125)
			TMP(group_spi[grp]).freq_band	= 4;
		else if (GROUP_VAL_POS(grp, frequency) < 4500)
			TMP(group_spi[grp]).freq_band	= 5;
		else if (GROUP_VAL_POS(grp, frequency) < 6250)
			TMP(group_spi[grp]).freq_band	= 6;
		else if (GROUP_VAL_POS(grp, frequency) < 8750)
			TMP(group_spi[grp]).freq_band	= 7;
		else if (GROUP_VAL_POS(grp, frequency) < 11000)
			TMP(group_spi[grp]).freq_band	= 8;
		else if (GROUP_VAL_POS(grp, frequency) < 13500)
			TMP(group_spi[grp]).freq_band	= 9;
		else if (GROUP_VAL_POS(grp, frequency) < 17500)
			TMP(group_spi[grp]).freq_band	= 10;
		else 
			TMP(group_spi[grp]).freq_band	= 11;
	}
	else
		TMP(group_spi[grp]).freq_band	= GROUP_VAL_POS(grp, filter) - 1;
	write_group_data (&TMP(group_spi[grp]), grp);
}

void data_122 (GtkMenuItem *menuitem, gpointer data)  /* Rectifier 检波 P122 */
{
	gint grp = get_current_group(pp->p_config);
	GROUP_VAL(rectifier) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	/* 发送给硬件 */
	TMP(group_spi[grp]).rectifier = GROUP_VAL_POS(grp, rectifier);
	send_spi_data (grp);
}

void data_124 (GtkMenuItem *menuitem, gpointer data)  /* averaging 平均 TAN1 P124 */
{
	GROUP_VAL(averaging) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	send_dsp_data (AVERAGING_DSP, GROUP_VAL(averaging));
	/* 发送给硬件 */
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

void data_134 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	//pp->p_config->beam_delay =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	BEAM_INFO(0,beam_delay) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);

	/*发送给硬件*/
}

void data_135 (GtkSpinButton *spinbutton, gpointer data) /*gain offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	GROUP_VAL(gain_offset) =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);

	/*发送给硬件*/
}

void data_1431 (GtkSpinButton *spinbutton, gpointer data) /* point qty P143 */
{
	gint grp = get_current_group(pp->p_config);
	GROUP_VAL(point_qty) =  (guint)(gtk_spin_button_get_value (spinbutton));
	get_prf();

	if (GROUP_VAL(point_qty) * 10 > GROUP_VAL(range))
		GROUP_VAL(range) = GROUP_VAL(point_qty) * 10;
	else
		GROUP_VAL(range) = rounding(0, GROUP_VAL(range), GROUP_VAL(point_qty) * 10);

	TMP(group_spi[grp]).compress_rato	= 
		((GROUP_VAL_POS(grp, range) / 10.0) / GROUP_VAL_POS(grp, point_qty)) > 1 ? 
		((GROUP_VAL_POS(grp, range) / 10.0) / GROUP_VAL_POS(grp, point_qty)) : 1;
	if (GROUP_VAL_POS(grp, prf)  >= 250)
		;
	else
		TMP(group_spi[grp]).idel_time		= 
			100000000 / (GROUP_VAL_POS(grp, prf) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	send_spi_data (grp);
	/* 重新确认每次dma的点数 */
}

void data_143 (GtkMenuItem *menuitem, gpointer data) /* point qty P143 */
{
	guint temp = GPOINTER_TO_UINT (data);
	gint grp = get_current_group(pp->p_config);
	GROUP_VAL(point_qty_pos) = temp;
	GROUP_VAL(point_qty) = get_point_qty();
	get_prf();
	if (GROUP_VAL(point_qty) * 10 > GROUP_VAL(range))
		GROUP_VAL(range) = GROUP_VAL(point_qty) * 10;
	else
		GROUP_VAL(range) = rounding(0, GROUP_VAL(range), GROUP_VAL(point_qty) * 10);
	if (temp != 4)
	{
		MENU_STATUS = MENU3_STOP;
		draw_menu3(0, NULL);
		send_dsp_data (POINT_QTY_DSP, get_point_qty());
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_menu3(0, NULL);
	}

	TMP(group_spi[grp]).compress_rato	= 
		((GROUP_VAL_POS(grp, range) / 10.0) / GROUP_VAL_POS(grp, point_qty)) > 1 ? 
		((GROUP_VAL_POS(grp, range) / 10.0) / GROUP_VAL_POS(grp, point_qty)) : 1;
	if (GROUP_VAL_POS(grp, prf)  >= 250)
		;
	else
		TMP(group_spi[grp]).idel_time		= 
			100000000 / (GROUP_VAL_POS(grp, prf) / (10)) - 2048 - TMP(group_spi[grp]).rx_time;
	send_spi_data (grp);
	/* 重新确认每次dma的点数 */
}

void data_1451 (GtkSpinButton *spinbutton, gpointer data) /* Sum Gain */
{
	gint	grp	= get_current_group(pp->p_config);
	GROUP_VAL(sum_gain) =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 100.0);

	/* 发送给硬件 */
	if (GROUP_VAL_POS(grp, probe.Elem_qty) == 1)	
		TMP(group_spi[grp]).sum_gain	= 4095;	
	else 
		TMP(group_spi[grp]).sum_gain	= 
			4096 / GROUP_VAL_POS(grp, probe.Elem_qty);	
	send_spi_data (grp);
	/* 发送给硬件 */
}


void data_145 (GtkMenuItem *menuitem, gpointer data) /* Sum Gain */
{
	guint	temp= GPOINTER_TO_UINT (data);
	gint	grp	= get_current_group(pp->p_config);
	GROUP_VAL(sum_gain_pos) = temp;
	GROUP_VAL(sum_gain) = get_sum_gain();
	if (temp != 1)
	{
		MENU_STATUS = MENU3_STOP;
		draw_menu3(0, NULL);
/*		send_dsp_data (SUM_GAIN_DSP, get_sum_gain());*/
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_menu3(0, NULL);
	}

	/* 发送给硬件 */
	if (GROUP_VAL_POS(grp, probe.Elem_qty) == 1)	
		TMP(group_spi[grp]).sum_gain	= 4095;	
	else 
		TMP(group_spi[grp]).sum_gain	= 
			4096 / GROUP_VAL_POS(grp, probe.Elem_qty);	
	send_spi_data (grp);
}

void data_200 (GtkMenuItem *menuitem, gpointer data) /* Gate 闸门选择 P200 */
{
	GROUP_VAL(gate_pos) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	send_dsp_data (GATE_POS_DSP, GROUP_VAL(gate_pos));
}

void data_201 (GtkMenuItem *menuitem, gpointer data) /* parameter 闸门参数选择 位置或者模式 P201 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].parameters) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);


	send_dsp_data (PARAMETERS_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].parameters));
}

void data_202 (GtkSpinButton *spinbutton, gpointer data)	/* 闸门开始位置 P202 */
{
	guint group = get_current_group(pp->p_config);
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ; 

	
	if (GROUP_VAL(gate_pos) == GATE_A)
	{
		TMP(group_spi[group]).gate_a_start	= 	GROUP_GATE_POS(start) / 10;
		TMP(group_spi[group]).gate_a_end	= (GROUP_VAL_POS(group, gate[0].start) + 
				GROUP_VAL_POS (group, gate[0].width)) / 10;
	}
	else if (GROUP_VAL(gate_pos) == GATE_B)
	{
		TMP(group_spi[group]).gate_b_start	= 	GROUP_GATE_POS(start) / 10;
		TMP(group_spi[group]).gate_b_end	= (GROUP_VAL_POS(group, gate[1].start) + 
				GROUP_VAL_POS (group, gate[1].width)) / 10;
	}
	else if (GROUP_VAL(gate_pos) == GATE_I)
	{
		TMP(group_spi[group]).gate_i_start	= 	GROUP_GATE_POS(start) / 10;
		TMP(group_spi[group]).gate_i_end	= (GROUP_VAL_POS(group, gate[2].start) + 
				GROUP_VAL_POS (group, gate[2].width)) / 10;
	}
	send_spi_data (group);

	gtk_widget_queue_draw(pp->draw_area->drawing_area);

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
	else if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].synchro) == 0)
		tmp = (tmp & 0xfffffff3) | 0x0c;
	else if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].synchro) == 0)
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
	guint group = get_current_group(pp->p_config);
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ; 

	if (GROUP_VAL(gate_pos) == GATE_A)
	{
		TMP(group_spi[group]).gate_a_end	= (GROUP_VAL_POS(group, gate[0].start) + 
				GROUP_VAL_POS (group, gate[0].width)) / 10;
	}
	else if (GROUP_VAL(gate_pos) == GATE_B)
	{
		TMP(group_spi[group]).gate_b_end	= (GROUP_VAL_POS(group, gate[1].start) + 
				GROUP_VAL_POS (group, gate[1].width)) / 10;
	}
	else if (GROUP_VAL(gate_pos) == GATE_I)
	{
		TMP(group_spi[group]).gate_i_end	= (GROUP_VAL_POS(group, gate[2].start) + 
				GROUP_VAL_POS (group, gate[2].width)) / 10;
	}

	send_spi_data (group);
	gtk_widget_queue_draw(pp->draw_area->drawing_area);
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
		tmp = (tmp & 0xfffffffc) | 0x00;
	else if (GROUP_VAL (gate[GROUP_VAL(gate_pos)].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	
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
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].height) =  (guchar) (gtk_spin_button_get_value (spinbutton) );

	if (GROUP_VAL(gate_pos) == GATE_A)
	{
		TMP(group_spi[group]).gate_a_height	= GROUP_VAL_POS(group, gate[0].height);
	}
	else if (GROUP_VAL(gate_pos) == GATE_B)
	{
		TMP(group_spi[group]).gate_b_height	= GROUP_VAL_POS(group, gate[1].height);
	}
	else if (GROUP_VAL(gate_pos) == GATE_I)
	{
		TMP(group_spi[group]).gate_i_height	= GROUP_VAL_POS(group, gate[2].height);
	}

	send_spi_data (group);
	gtk_widget_queue_draw(pp->draw_area->drawing_area);
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

void data_221 (GtkMenuItem *menuitem, gpointer data) /* Output -> alarm # P221 */
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

void data_2211 (GtkMenuItem *menuitem, gpointer data) /* Output -> group */
{
	CFG_ANALOG_POS(group) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	send_dsp_data (GROUPA_DSP, CFG_ANALOG_POS(group));
}

void data_222 (GtkSpinButton *spinbutton, gpointer data) /* count P222 */
{
	set_output_count (pp->p_config, (guchar) (gtk_spin_button_get_value (spinbutton)));
}

void data_2221 (GtkMenuItem *menuitem, gpointer data) /* count */
{
	CFG_ANALOG_POS(data) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	send_dsp_data (DATA_DSP, CFG_ANALOG_POS(data));
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

void data_230 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Mode P230 */
{
	GROUP_VAL(mode_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	send_dsp_data (MODE_POS_DSP, GROUP_VAL(mode_pos));
}

void data_231 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Curve P231 */
{
	GROUP_VAL(curve_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	send_dsp_data (CURVE_POS_DSP, GROUP_VAL(curve_pos));
}

void data_2311 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Point 231 */
{
	GROUP_VAL(point_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	send_dsp_data (POINT_POS_DSP, GROUP_VAL(point_pos));
}

void data_2312 (GtkSpinButton *spinbutton, gpointer data) /* Mat.Attenuatior P2312 */
{
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(mat_atten) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 );
		else  /* 英寸 */
			GROUP_VAL(mat_atten) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.03937);
	}
	else /* 显示方式为时间 */
		GROUP_VAL(mat_atten) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.338) ; 

	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}

void data_232 (GtkSpinButton *spinbutton, gpointer data) /* Ref.Amplitude P232 */
{
	GROUP_VAL(ref_ampl) =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}
void data_2321 (GtkSpinButton *spinbutton, gpointer data) /* Position P2321 */
{
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(position) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
		else  /* 英寸 */
			GROUP_VAL(position) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
	}
	else /* 显示方式为时间 */
			GROUP_VAL(position) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.1159 / 0.03937);

	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}
void data_2322 (GtkSpinButton *spinbutton, gpointer data) /* Delay P2322 */
{
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 ) ;
		else  /* 英寸 */
			GROUP_VAL(delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937) ;
	}
	else /* 显示方式为时间 */
		GROUP_VAL(delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 2.945 ) ; 

	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}

void data_233 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	GROUP_VAL(ref_ampl_offset) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}

void data_2331 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	GROUP_VAL(amplitude) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}
void data_2332 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	GROUP_VAL(tcg_gain) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}

void data_234 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	GROUP_VAL(curve_step) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}

void data_235 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	GROUP_VAL(ref_gain) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	/*send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));*/
}

void data_300 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> list 300 */
{
	CFG(list) = (gchar)(GPOINTER_TO_UINT (data));
	
	if (CFG(list) == 0)
	{
		CFG(field1) = 0;
		CFG(field2) = 31;
		CFG(field3) = 29;
		CFG(field4) = 33;
	}
	else if (CFG(list) == 1)
	{
		CFG(field1) = 0;
		CFG(field2) = 31;
		CFG(field3) = 35;
		CFG(field4) = 37;
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_302 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field1 302 */ 
{
	CFG(field1) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_303 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field2 303 */
{
	CFG(field2) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_304 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field3 304 */
{
	CFG(field3) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_305 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field4 305 */
{
	CFG(field4) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}

void data_310 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> cursors -> selection 310 */
{
	GROUP_VAL(selection) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}


void data_311 (GtkSpinButton *spinbutton, gpointer data) 
{
	//pp->p_config->VPA =  (guint) (gtk_spin_button_get_value (spinbutton));
	GROUP_VAL(per_reference) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
}
void data_3111 (GtkSpinButton *spinbutton, gpointer data) 
{
	if(CFG(unit) == UNIT_MM)
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
	if(CFG(unit) == UNIT_MM)
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
#if 0
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if(CFG(unit) == UNIT_MM)
			GROUP_VAL(u_reference) =  (guint) (gtk_spin_button_get_value (spinbutton)*1000.0);
		else
			GROUP_VAL(u_reference) =  (guint) (gtk_spin_button_get_value (spinbutton)*1000.0/0.03937);
	}
	else
		GROUP_VAL(u_reference) =  (guint) (gtk_spin_button_get_value (spinbutton)/(GROUP_VAL(velocity))*200.0);

#endif


	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(u_reference) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(u_reference) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(u_reference) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 


	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3131 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(CFG(unit) == UNIT_MM)
		GROUP_VAL(i_reference) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(i_reference) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_3132 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(CFG(unit) == UNIT_MM)
		GROUP_VAL(s_refmeas) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(s_refmeas) =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_3133 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*100.0);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if(CFG(unit) == UNIT_MM)
			GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*1000.0);
		else
			GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*1000.0/0.03937);
	}
	else
		GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*1000.0);
}

void data_314 (GtkSpinButton *spinbutton, gpointer data) /* */
{

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(u_measure) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(u_measure) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(u_measure) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 

	gtk_widget_queue_draw (pp->vboxtable);

}

void data_3141 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(CFG(unit) == UNIT_MM)
		GROUP_VAL(i_measure) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_VAL(i_measure) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_3142 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(CFG(unit) == UNIT_MM)
		GROUP_CURSORS_POS(scan) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_CURSORS_POS(scan) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_315 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> DATA LINK */
{
	GROUP_VAL(data_link) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_3151 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(CFG(unit) == UNIT_MM)
		GROUP_CURSORS_POS(index) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		GROUP_CURSORS_POS(index) =  (gint) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
}

void data_324 (GtkSpinButton *spinbutton, gpointer data) /*entry_qty */
{
	pp->p_config->entry_qty =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_330 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Thickness -> source p330 */
{
	GROUP_VAL(source) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_331 (GtkSpinButton *spinbutton, gpointer data) /*min_thickness p331 */
{
	if(UNIT_MM == CFG(unit))
		CFG(min_thickness) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		CFG(min_thickness) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937);
}

void data_332 (GtkSpinButton *spinbutton, gpointer data) /*max_thickness p332 */
{
	if(UNIT_MM == CFG(unit))
		CFG(max_thickness) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		CFG(max_thickness) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}

void data_333 (GtkSpinButton *spinbutton, gpointer data) /*echo_qty p333 */
{
	CFG(echo_qty) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_400 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection -> display p400 */
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
	draw_menu3 (0, NULL);
	draw_area_all ();
}

void data_401 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection Ascan -> group p401 */
{
	set_display_group (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_4011 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection -> C-Scan1 地一个Cscan p401 */
{
	set_cscan_source (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), 0);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all ();
	pp->sscan_mark = 1;
}

void data_4012 (GtkMenuItem *menuitem, gpointer data) /* ASC显示模式后时候Cscan的source P401 */
{
	set_cscan_source (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), 0);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all ();
}

void data_4013 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection -> Data1 p401 */
{
	set_stripscan_data1 (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_402 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection -> C-Scan2 p402 */
{
	set_cscan_source (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)), 1);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all ();
}
void data_4021 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection -> C-Scan2 p402 */
{
	set_stripscan_data2 (pp->p_config, (guchar) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_403 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection -> Mode p403 */
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

void data_410 (GtkMenuItem *menuitem, gpointer data) /* Display -> Overlay -> UT Unit p410 */
{
	GROUP_VAL(ut_unit) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
}

void data_411 (GtkMenuItem *menuitem, gpointer data) /* 选择栅格颜色  P411 */
{
	CFG(grid) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	gtk_widget_queue_draw (pp->vboxtable);
	draw_menu3(0, NULL);
}

void data_420 (GtkMenuItem *menuitem, gpointer data) /* Display -> Zoom -> Display p420 */
{
	CFG(zoom_display_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_421 (GtkMenuItem *menuitem, gpointer data) /* Display -> Zoom -> Type p421 */
{
	CFG_ZOOM_POS(zoom_type) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_422 (GtkSpinButton *spinbutton, gpointer data) /* start USound p422 */
{
	CFG_ZOOM_POS(start_usound) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_4221 (GtkSpinButton *spinbutton, gpointer data) /* start USound p422 */
{
	CFG_ZOOM_POS(center_usound) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_423 (GtkSpinButton *spinbutton, gpointer data) /* end USound p423 */
{
	CFG_ZOOM_POS(end_usound) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_4231 (GtkSpinButton *spinbutton, gpointer data) /* range USound p423 */
{
	CFG_ZOOM_POS(range_usound) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_424 (GtkSpinButton *spinbutton, gpointer data) /* start Amplitude p424 */
{
	CFG_ZOOM_POS(start_amplitude) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}
void data_4241 (GtkSpinButton *spinbutton, gpointer data) /* center Amplitude p424 */
{
	CFG_ZOOM_POS(center_amplitude) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}
void data_425 (GtkSpinButton *spinbutton, gpointer data) /* end Amplitude p425 */
{
	CFG_ZOOM_POS(end_amplitude) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}
void data_4251 (GtkSpinButton *spinbutton, gpointer data) /* range Amplitude p4251 */
{
	CFG_ZOOM_POS(range_amplitude) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_430 (GtkMenuItem *menuitem, gpointer data) /* Display -> Color -> select p430 */
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

void data_434 (GtkMenuItem *menuitem, gpointer data) /* Display -> color -> mode 434 */
{
	GROUP_VAL(col_mode) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_440 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> scan 440 */
{
	pp->p_config->prop_scan = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_441 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Color 441 */
{
	GROUP_VAL(ascan_color) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_4411 (GtkSpinButton *spinbutton, gpointer data) /*Display -> Properties -> Compress 4411 */
{
	pp->p_config->compress =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}
void data_4414 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Color 4414 */
{
	CFG(fft_color) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_4415 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Color 4415 */
{
	pp->p_config->orientation = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_442 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Envelope 442 */
{
	GROUP_VAL(ascan_envelope) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_443 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Source 443 */
{
	GROUP_VAL(ascan_source) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_444 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Appearance 444 */
{
	GROUP_VAL(ascan_appearance) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_445 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Overlay 445 */
{
	GROUP_VAL(ascan_overlay) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_500 (GtkMenuItem *menuitem, gpointer data) /* 增加删除选择group P500 */
{
	guchar temp = (guchar)(GPOINTER_TO_UINT (data));
	switch (temp)
	{
		case 0:
			set_group_qty(pp->p_config, get_group_qty(pp->p_config) + 1);
			grpcpy (pp->p_config, get_group_qty(pp->p_config) - 1, 0);
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
			set_current_group(pp->p_config, temp - 1);
			break;
		case 9:
			set_group_qty(pp->p_config, get_group_qty(pp->p_config) - 1);
			break;
		default:break;
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_501 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Select -> Group Mode 501 */
{
	GROUP_VAL(group_mode) = (gchar) (GPOINTER_TO_UINT (data));
	if(!GROUP_VAL(group_mode)) /*group mode 选择UT时，focal law 不可用*/
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
	else
		gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_502 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Select -> Select 502 */
{
	pp->p_config->probe_select = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_510 (GtkSpinButton *spinbutton, gpointer data) /*scanoffset */
{
	if(CFG(unit) == UNIT_MM)
	GROUP_VAL(scan_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
	GROUP_VAL(scan_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);

	draw_area_all ();
}

void data_511 (GtkSpinButton *spinbutton, gpointer data) /*indexoffset */
{
	if(CFG(unit) == UNIT_MM)
	GROUP_VAL(index_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
	GROUP_VAL(index_offset) =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
}

void data_5121 (GtkSpinButton *spinbutton, gpointer data) /* Skew (deg) */
{
	GROUP_VAL(skew) =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
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
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
	}
}

void data_521 (GtkSpinButton *spinbutton, gpointer data) /*gain */
{
	GROUP_VAL(gain) =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_522 (GtkSpinButton *spinbutton, gpointer data) /*agate_start */
{
	GROUP_VAL(agate_start) =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_523 (GtkSpinButton *spinbutton, gpointer data) /*agate_width */
{
	GROUP_VAL(agate_width) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_530 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Parts -> Geometry 530 */
{
	set_part_geometry (pp->p_config, data);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_531 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	if(UNIT_MM == CFG(unit))
		set_part_thickness (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_part_thickness (pp->p_config, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_532 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	if(UNIT_MM == CFG(unit))
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

/* 聚焦法则类型 P600 */
void data_600 (GtkMenuItem *menuitem, gpointer data) 
{
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
}

void data_601 (GtkSpinButton *spinbutton, gpointer data) /* connection_P P601 */
{
}

/* min_angle P610 */
void data_610 (GtkSpinButton *spinbutton, gpointer data)
{
	gtk_spin_button_set_value (spinbutton, 
			rounding(LAW_VAL(Angle_max),
				(gint)(gtk_spin_button_get_value (spinbutton) * 100.0), 
				(LAW_VAL(Angle_step) + 5) / 10 * 10) / 100.0);
	LAW_VAL(Angle_min) = (gshort)
		(gtk_spin_button_get_value (spinbutton) * 100.0);
	draw_area_all();
}


/* max_angle P611 */
void data_611 (GtkSpinButton *spinbutton, gpointer data) 
{
	if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
	{
		gtk_spin_button_set_value (spinbutton, 
				rounding(LAW_VAL(Angle_min),
					(gint)(gtk_spin_button_get_value (spinbutton) * 100.0), 
					(LAW_VAL(Angle_step) + 5) / 10 * 10) / 100.0);
		LAW_VAL(Angle_max) = (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
	}
	draw_area_all();
}

/* Angle Step P612 */
void data_612 (GtkSpinButton *spinbutton, gpointer data) 
{
	if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
	{
		LAW_VAL(Angle_step) = (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
		LAW_VAL(Angle_max) = rounding(LAW_VAL(Angle_min),
					LAW_VAL(Angle_max),	(LAW_VAL(Angle_step) + 5) / 10 * 10);
	}
	draw_area_all();
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
	LAW_VAL(Focal_point_type) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_621 (GtkSpinButton *spinbutton, gpointer data) /* Position start P621 */
{
	LAW_VAL(Position_start) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_6211 (GtkSpinButton *spinbutton, gpointer data) /* Offset start P621 */
{
	LAW_VAL(Offset_start) = (guint) (gtk_spin_button_get_value (spinbutton)* 1000.0);
}

void data_622 (GtkSpinButton *spinbutton, gpointer data) /* Angle Step P622 */
{
	LAW_VAL(Position_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_6221 (GtkSpinButton *spinbutton, gpointer data) /* Angle Step P622 */
{
	LAW_VAL(Offset_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

# if 0
void data_623 (GtkSpinButton *spinbutton, gpointer data) /* focus_depth P623*/
{
	if(UNIT_MM == CFG(unit))
		LAW_VAL(Focus_depth) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		LAW_VAL(Focus_depth) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}
#endif

void data_623 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P623 */
{
	LAW_VAL(Position_step) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}
void data_6231 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P623 */
{
	LAW_VAL(Depth_start) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_624 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P624 */
{
	LAW_VAL(Depth_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

/* element_qty 聚集 法则一次激发的阵元数量 P620 */
void data_630 (GtkSpinButton *spinbutton, gpointer data) 
{
	LAW_VAL (Elem_qty) = (guchar) (gtk_spin_button_get_value (spinbutton));
	LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
}

/* first_element 第一个接收阵元 */
void data_631 (GtkSpinButton *spinbutton, gpointer data) 
{
	LAW_VAL(First_tx_elem) =  (guchar) (gtk_spin_button_get_value (spinbutton));
	LAW_VAL (Last_tx_elem) = (guchar) (LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty)) - 1;
}
/* last_element 最后一个阵元编号 */
void data_632 (GtkSpinButton *spinbutton, gpointer data) 
{
	LAW_VAL(Last_tx_elem) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_633 (GtkSpinButton *spinbutton, gpointer data) /*element_step*/
{
	LAW_VAL(Elem_step) =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

/* focalpoint 聚焦点计算方法 P620 */
void data_634 (GtkMenuItem *menuitem, gpointer data) 
{
	guint temp = (guchar) (GPOINTER_TO_UINT (data));
	if (temp == 0)
		GROUP_VAL (velocity) = get_material_lw (pp->p_config);
	else if (temp == 1) 
		GROUP_VAL (velocity) = get_material_sw (pp->p_config);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_700 (GtkMenuItem *menuitem, gpointer data) /* Encoder */
{
	CFG(encoder) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_701 (GtkMenuItem *menuitem, gpointer data) /* Polarity */
{
	CFG(polarity) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_702 (GtkMenuItem *menuitem, gpointer data) /* Type */
{
	CFG(e_type) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_703 (GtkSpinButton *spinbutton, gpointer data) /* Resolution */
{
	CFG(encoder_resolution) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_704 (GtkSpinButton *spinbutton, gpointer data) /* Origin */
{
	CFG(origin) =  (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_710 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Inspection -> type */
{
	CFG(i_type) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_711 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Inspection -> scan */
{
	CFG(i_scan) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
}

void data_712 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Inspection -> Index */
{
	CFG(i_index) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_713 (GtkSpinButton *spinbutton, gpointer data) /*scan_speed*/
{
	if(UNIT_MM == CFG(unit))
		CFG(scanspeed) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		CFG(scanspeed) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}

void data_714 (GtkSpinButton *spinbutton, gpointer data) /*scan_speed*/
{
	pp->p_config->scanspeed_rpm =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_715 (GtkSpinButton *spinbutton, gpointer data) /*index_speed*/
{
	CFG(indexspeed) =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_720 (GtkSpinButton *spinbutton, gpointer data) /*scan_start*/
{
	if(UNIT_MM == CFG(unit))
		CFG(scan_start) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		CFG(scan_start) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );

	draw_area_all();
}

void data_721 (GtkSpinButton *spinbutton, gpointer data) /*scan_end*/
{
	if(UNIT_MM == CFG(unit))
		CFG(scan_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		CFG(scan_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );

	draw_area_all();
}

void data_722 (GtkSpinButton *spinbutton, gpointer data) /*scan_resolution*/
{
	if(UNIT_MM == CFG(unit))
		CFG(scan_resolution) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		CFG(scan_resolution) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}

void data_723 (GtkSpinButton *spinbutton, gpointer data) /*index_start*/
{
	if(UNIT_MM == CFG(unit))
		CFG(index_start) =  (guint) (gtk_spin_button_get_value (spinbutton)*1000.0);
	else
		CFG(index_start) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}

void data_724 (GtkSpinButton *spinbutton, gpointer data) /*index_end*/
{
	if(UNIT_MM == CFG(unit))
		CFG(index_end) =  (guint) (gtk_spin_button_get_value (spinbutton)*1000.0);
	else
		CFG(index_end) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}

void data_725 (GtkSpinButton *spinbutton, gpointer data) /*index_resolution*/
{
	if(UNIT_MM == CFG(unit))
		CFG(index_resolution) =  (guint) (gtk_spin_button_get_value (spinbutton)*1000.0);
	else
		CFG(index_resolution) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 );
}

void data_730 (GtkMenuItem *menuitem, gpointer data) /* Scan -> start -> start mode */
{
	CFG(start_mode) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_740 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Data -> storage */
{
	CFG(storage) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_741 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Data -> inspec.data */
{
	CFG(inspec_data) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_800 (GtkMenuItem *menuitem, gpointer data) /*  File -> File -> Storage */
{
	CFG(file_storage) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}


void data_810 (GtkMenuItem *menuitem, gpointer data) /*  File -> Report -> Template */
{
	CFG(templa) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_804 (GtkMenuItem *menuitem, gpointer data) /* File -> File -> Save Mode */
{
	pp->p_config->save_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_812 (GtkMenuItem *menuitem, gpointer data) /* File -> Report -> paper size */
{
	pp->p_config->paper_size = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_824 (GtkMenuItem *menuitem, gpointer data) /* File -> Format -> view*/
{
	pp->p_config->view = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_830(GtkMenuItem *menuitem, gpointer data) /* File -> User Field -> select */
{
	pp->p_config->file_select = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_900(GtkMenuItem *menuitem, gpointer data) /* Preferences -> Pref. -> Units */
{
	CFG(unit) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	draw_area_all();
}

void data_903(GtkMenuItem *menuitem, gpointer data) /* Preferences -> Pref. -> scheme */
{
	CFG(scheme) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_904(GtkMenuItem *menuitem, gpointer data) /* Preferences -> Pref. -> gate mode */
{
	CFG(gate_mode) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_912(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> select key */
{
	CFG(select_key) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_913(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> assign key*/
{
	CFG(assign_key) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_9131(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> assign key*/
{
	CFG(assign_key_p) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

# if 0
void data_923(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> assign key*/
{
	pp->p_config->startup_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
#endif

void data_930(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> assign key*/
{
	pp->p_config->mouse = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}



void data_901 (GtkSpinButton *spinbutton, gpointer data) /*scan_resolution*/
{
	pp->p_config->bright =  (guint) (gtk_spin_button_get_value (spinbutton));
}


/*  */
void send_dsp_data (guint data_type, guint value)
{
	switch (data_type)	
	{
		case BEAM_DELAY_DSP:break;
		case GATEA_DSP:break;
		case GATEB_DSP:break;
		case GATEI_DSP:break;
		case TX_RX_MODE_DSP:
					   g_print("%d\n", value);
					   break;
		case FREQUENCY_DSP:
					   g_print("%d\n", value);
					   break;
		case VOLTAGE_DSP:
					   if (get_current_group(pp->p_config) != 3)
						   switch (value)
						   {
							   case 0:g_print("%x\n", setup_VOLTAGE_MIN_PA);break;
							   case 1:g_print("%x\n", setup_VOLTAGE_MAX_PA);break;
							   default:break;
						   }
					   else 
						   switch (value)
						   {
							   case 0:g_print("%x\n", setup_VOLTAGE_MIN_UT);break;
							   case 1:g_print("%x\n", setup_VOLTAGE_MED_UT);break;
							   case 2:g_print("%x\n", setup_VOLTAGE_MAX_UT);break;
							   default:break;
						   }
					   break;
		case PW_DSP:
					   g_print("%d\n", value);
					   break;
		case PRF_DSP:
					   g_print("%d\n", value);
					   break;
		case FILTER_DSP:
					   g_print("%d\n", value);
					   break;
		case RECTIFIER_DSP:
					   g_print("%d\n", value);
					   break;
		case AVERAGING_DSP:
					   g_print("%d\n", value);
					   break;
		case VIDEO_FILTER_DSP:
					   g_print("%d\n", value);
					   break;
		case POINT_QTY_DSP:
					   g_print("%d\n", value);
					   break;
		case SUM_GAIN_DSP:
					   g_print("%d\n", value);
					   break;
		case GATE_POS_DSP:
					   g_print("%d\n", value);
					   break;
		case PARAMETERS_DSP:
					   g_print("%d\n", value);
					   break;
		case START_DSP:
					   g_print("%d\n", value);
					   break;
		case SYNCHRO_DSP:
					   g_print("%d\n", value);
					   break;
		case WIDTH_DSP:
					   g_print("%d\n", value);
					   break;
		case MEASURE_DSP:
					   g_print("%d\n", value);
					   break;
		case HEIGHT_DSP:
					   g_print("%d\n", value);
					   break;
		case RECTIFIER_FREQ_DSP:
					   g_print("%d\n", value);
					   break;
		case ALARM_POS_DSP:
					   g_print("%d\n", value);
					   break;
		case GROUPA_DSP:
					   g_print("%d\n", value);
					   break;
		case CONDITIONA_DSP:
					   g_print("%d\n", value);
					   break;
		case OPERAT_DSP:
					   g_print("%d\n", value);
					   break;
		case GROUPB_DSP:
					   g_print("%d\n", value);
					   break;
		case CONDITIONB_DSP:
					   g_print("%d\n", value);
					   break;
		case OUTPUT_POS_DSP:
					   g_print("%d\n", value);
					   break;
		case ALARM1_DSP:
					   g_print("%d\n", value);
					   break;
		case COUNT_DSP:
					   g_print("%d\n", value);
					   break;
		case DATA_DSP:
					   g_print("%d\n", value);
					   break;
		case SOUND_DSP:
					   g_print("%d\n", value);
					   break;
		case DELAY_DSP:
					   g_print("%d\n", value);
					   break;
		case HOLDTIME_DSP:
					   g_print("%d\n", value);
					   break;
		case MODE_POS_DSP:
					   g_print("%d\n", value);
					   break;
		case CURVE_POS_DSP:
					   g_print("%d\n", value);
					   break;
		default:break;
	}
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
		memcpy (CFG(edit_notes_info), Data, sizeof(CFG(edit_notes_info)));

		g_free (Data);
		g_print ("OK_Pressed");
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
		g_print ("CANCEL_Pressed");
	gtk_widget_destroy (GTK_WIDGET (dialog));
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
		memcpy (CFG(edit_header_info), Data, sizeof(CFG(edit_header_info)));

		g_free (Data);
		g_print ("OK_Pressed");
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
		g_print ("CANCEL_Pressed");
	gtk_widget_destroy (GTK_WIDGET (dialog));
}

void dialog_destroy(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	gtk_widget_destroy (GTK_WIDGET(data));
}


void send_spi_data (gint group)
{
	write_group_data (&TMP(group_spi[group]), group);
}
