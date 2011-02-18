
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include <gdk/gdkkeysyms.h>

static void handler_key(guint keyval);

extern const gchar ****con2_p;

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

gboolean key_press_handler (GtkWidget* pWidget,
		GdkEventKey* pEvent, gpointer data);

void send_dsp_data (guint data_type, guint value);
guint get_freq ();
guint get_pw ();
guint get_prf ();
guint get_filter ();

gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function1 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function2 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function3 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function4 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function5 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

void data_022 (GtkMenuItem *menuitem, gpointer data);
void data_023 (GtkMenuItem *menuitem, gpointer data); 

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
void data_313 (GtkSpinButton *spinbutton, gpointer data);
void data_324 (GtkSpinButton *spinbutton, gpointer data);

void data_330 (GtkMenuItem *menuitem, gpointer data); /* Measurements -> Thickness -> source 330 */
void data_331 (GtkSpinButton *spinbutton, gpointer data);
void data_332 (GtkSpinButton *spinbutton, gpointer data);
void data_333 (GtkSpinButton *spinbutton, gpointer data);

void data_400 (GtkMenuItem *menuitem, gpointer data);
void data_410 (GtkMenuItem *menuitem, gpointer data);
void data_411 (GtkMenuItem *menuitem, gpointer data);
void data_420 (GtkMenuItem *menuitem, gpointer data);
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
void data_614(GtkMenuItem *menuitem, gpointer data);

void data_620 (GtkSpinButton *spinbutton, gpointer data);
void data_623 (GtkSpinButton *spinbutton, gpointer data);

void data_710 (GtkMenuItem *menuitem, gpointer data);
void data_711 (GtkMenuItem *menuitem, gpointer data);
void data_713 (GtkSpinButton *spinbutton, gpointer data);
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
void data_812 (GtkMenuItem *menuitem, gpointer data);
void data_824 (GtkMenuItem *menuitem, gpointer data);
void data_830 (GtkMenuItem *menuitem, gpointer data);

void data_900 (GtkMenuItem *menuitem, gpointer data);
void data_901 (GtkSpinButton *spinbutton, gpointer data);
void data_903 (GtkMenuItem *menuitem, gpointer data);
void data_904 (GtkMenuItem *menuitem, gpointer data);
void data_912 (GtkMenuItem *menuitem, gpointer data);
void data_913 (GtkMenuItem *menuitem, gpointer data);
void data_924 (GtkMenuItem *menuitem, gpointer data);
void data_930 (GtkMenuItem *menuitem, gpointer data);


gboolean eventbox2_function0 (GtkWidget *widget, GdkEventButton *event,	gpointer data);
gboolean eventbox2_function1 (GtkWidget *widget, GdkEventButton *event,	gpointer data);
gboolean eventbox2_function2 (GtkWidget *widget, GdkEventButton *event,	gpointer data);
gboolean eventbox2_function3 (GtkWidget *widget, GdkEventButton *event,	gpointer data);
gboolean eventbox2_function4 (GtkWidget *widget, GdkEventButton *event,	gpointer data);


/*   */
static inline void data_process(guchar *data, guint pa);

/*三级菜单6个按钮的回调函数*/

/*data button press 回调函数*/
gboolean (*data_fun[6])(GtkWidget *widget, GdkEventButton *event, gpointer data) = 
{
	data_function0,	data_function1,	data_function2,	
	data_function3,	data_function4,	data_function5
};

/*二级菜单 button press 回调函数*/
gboolean (*eventbox2_fun[5])(GtkWidget *widget, GdkEventButton *event, gpointer data) = 
{
	eventbox2_function0,	eventbox2_function1,	eventbox2_function2,	
	eventbox2_function3,	eventbox2_function4
};

guint get_sum_gain ()
{
	if (GROUP_VAL(sum_gain_pos))
		return GROUP_VAL(sum_gain);
	else 
	{
               return 10;
	}
	return 0;
}


guint get_point_qty ()
{
	if (GROUP_VAL(point_qty_pos) == 4)
		return GROUP_VAL(point_qty);
	else 
	{
		switch (GROUP_VAL(point_qty_pos))
		{
			case 0:
				return 500;
				break;
			case 1:
				return 160;
				break;
			case 2:
				return 320;
				break;
			case 3:
				return 640;
				break;
			default:break;
		}
	}
	return 0;
}


guint get_prf ()
{
	if (GROUP_VAL(prf_pos) == 3)
		return GROUP_VAL(prf);
	else 
	{
		switch (GROUP_VAL(prf_pos))
		{
			case 0:
				return 10000;
				break;
			case 1:
				return 5000;
				break;
			case 2:
				return 500;
				break;
			default:break;
		}
	}
	return 0;
}

guint get_pw ()
{
	if (GROUP_VAL(pw_pos))
		return GROUP_VAL(pulser_width);
	else
		return 3000; /* 计算 */
}

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
	return GROUP_VAL(frequence);
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

/*5个二级菜单按钮的回调函数*/
void b2_fun0(DRAW_UI_P p, gint pos)
{
	p->pos_last1 = p->pos1[p->pos];
	p->pos1[p->pos] = pos;
	pp->pos_pos = MENU3_STOP;
	draw_2_menu(0);
	draw_3_menu(0, NULL);
}

gboolean eventbox2_function0 (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 0);
	return TRUE;
}

gboolean eventbox2_function1 (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 1);
	return TRUE;
}

gboolean eventbox2_function2 (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 2);
	return TRUE;
}

gboolean eventbox2_function3 (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 3);
	return TRUE;
}

gboolean eventbox2_function4 (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 4);
	return TRUE;
}

static inline void data_process(guchar* data, guint pa)
{
	(*data > 0) ? (*data)-- : (*data = pa);
	return ;
}

/*6个数值区域共有的处理函数 */
/* 第一个数值按键 快捷键是F12  */
void b3_fun0(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 0;
	/*	pp->pos_pos = MENU3_PRESSED;*/
	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) && 
			(pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0: /* Wizard */
				break;
			case 1: /* UT Settings*/
				switch (pp->pos1[1])
				{
					case 0: data_process (&(pp->p_tmp_config->db_reg), 4);         break; /* 100增益 5种步进 */
					case 1: data_process (&(pp->p_tmp_config->pulser_reg), 2);		break; /* 110pulser发射 3种步进 */
					case 2: data_process (&(pp->p_tmp_config->receiver_reg), 2);		break; /* 120receiver接收 3种步进 */
					case 3: break; /* 130scan offset 这里只有显示数值 不能更改 */
					case 4: break; /* 140 自动80%  */
					default:break;
				}
				break;
			case 2: /* Gate/Alarm */
				switch (pp->pos1[2])
				{
					case 0: /* 弹出一个菜单选择  */			break; /* 200闸门3种选择  */
					case 1: /* 弹出一个菜单选择  */			break; /* 210 Alarm  */
					case 2: /* 弹出一个菜单选择  */			break; /* 220 Alarm  */
					case 3: break; /* 230 */
					default:break;
				}
				break;
			case 3: /* Display Table */
				switch (pp->pos1[3])
				{
					case 0: /* 弹出一个菜单选择  */			break; /* 300 */
					case 1: /* 弹出一个菜单选择  */			break; /* 310  */
					case 2: pp->p_config->display_table = !pp->p_config->display_table;break; /* 320 display table  */
					case 3: break; /* 330 */
					default:break;
				}
				break;
			case 4:
				break;
			case 5:
				switch (pp->pos1[5])
				{
					case 0: break; /*   */
					case 1: data_process (&(pp->p_tmp_config->scanoffset_reg), 2);  break; /*510 */
					case 2: pp->p_config->fft = !pp->p_config->fft;break; /*   */
					case 3: break; 
					default:break;
				}
				break;
			case 6:
				switch (pp->pos1[6])
				{
					case 0: break; /*   */
					case 1: data_process (&(pp->p_tmp_config->element_qty_reg), 2);  break; /*610 */
					case 2: data_process (&(pp->p_tmp_config->min_angle_reg), 2);  break;   /*620  */
					case 3: pp->p_config->auto_program = !pp->p_config->auto_program;break; /*630*/ 
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0: break; /*   */
					case 1: break; /*710 */
					case 2: data_process (&(pp->p_tmp_config->scan_start_reg), 2);  break;   /*720  */
					case 3: break; 
					default:break;
				}
				break;

			case 8:
				switch (pp->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: pp->p_config->format_userfield = !pp->p_config->format_userfield;break;   /*820  */
					case 3: break; 
					default:break;
				}
				break;

			default:break;
		}

	}

	pp->pos_pos = MENU3_PRESSED;
	draw_2_menu (0);
	draw_3_menu (0, p);                          /**/

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
						   markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
								   GROUP_VAL(gain) / 100.0 );
					   gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);

					   g_free(markup);
					   break; /* Set Ref P141 设置参考增益值*/
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
			case 0:break;
			case 1:
				   switch (pp->pos1[1])
				   {
					   case 0:data_process(&(pp->p_tmp_config->start_reg), 2); break; /* 101start扫描延时 3种步进 */
					   case 1: /* 弹出一个选择菜单,选择 */ break; /*111 收发模式Tx/Rx Mode  */
					   case 2: /* 弹出一个选择菜单,选择 */ break; /*121 滤波Filter  */
					   case 3: /* 弹出一个选择菜单,选择 */ break; /*131 Index offset 不能更改  */
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
						case 1: data_process(&(pp->p_tmp_config->VPA_reg), 1); break;
						case 2: pp->p_config->entry_image= !pp->p_config->entry_image; /* 321 entry image*/ 
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
								switch(CFG(prop_scan)) 
								{
									case 0:data_process(&(pp->p_tmp_config -> color_start_reg ), 1 );break;
									case 1:data_process(&(pp->p_tmp_config -> color_contrast_reg ), 1 );break;
									case 2:data_process(&(pp->p_tmp_config->min_reg), 2);break;
								}
								break; 

						case 4:
								switch(CFG(prop_scan)) 
								{
									case 0:break;
									case 1:data_process(&(pp->p_tmp_config -> compress_reg ), 2 );break;
									case 2:pp->p_config->ratio = !pp->p_config->ratio;break;
									case 3:pp->p_config->interpolation = !pp->p_config->interpolation;break;
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
						case 1: data_process (&(pp->p_tmp_config->first_element_reg), 2);  break; /*611 */
						case 2: data_process (&(pp->p_tmp_config->max_angle_reg), 2);  break;     /*621 */
						case 3: break; 
						default:break;
					}
					break;
			case 7:
					switch (pp->pos1[7])
					{
						case 0: break; /*    */
						case 1: break; /*710 */
						case 2: data_process (&(pp->p_tmp_config->scan_end_reg), 2);  break;   /*721  */
						case 3: pp->p_config->pause = !pp->p_config->pause; break; /*731*/ 
						default:break;
					}
					break;
			case 8:
					switch (pp->pos1[8])
					{
						case 0: break; 
						case 1: break;
						case 2: pp->p_config->format_probe = !pp->p_config->format_probe;break;   /*821  */
						case 3: pp->p_config->enable = !pp->p_config->enable;     break;          /*831  */ 
						default:break;
					}
					break;
			case 9:
					switch (pp->pos1[9])
					{
						case 0: data_process (&(pp->p_tmp_config->bright_reg), 2);  break; /*901  */
						case 1: break; /*710 */
						case 2: break;   /*721  */
						case 3: pp->p_config->ezview = !pp->p_config->ezview;break;  /*931*/
						default:break;
					}
					break;
			default:break;
		}
	}

	pp->pos_pos = MENU3_PRESSED;
	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

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
		default:break;
	}
	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED)) 
	{
		switch (pp->pos) 
		{
			case 0:break;
			case 1:
				   switch (pp->pos1[1])
				   {
					   case 0:data_process(&(pp->p_tmp_config->range_reg), 2); break; /* 102range范围 3种步进 */
					   case 1:
							  if (pp->mark_pop_change)
								  data_process(&(TMP(frequence_reg)), 2); /* 112 频率范围 3种步进 */
							  break; /* 112 频率 Freq.  */
					   case 2: /* 弹出一个选择菜单,选择 */ break; /* 122 检波 Recitify  */
					   case 3: /* Angle. (deg) */ break; /* 132 角度 不能更改 */
					   case 4: break;/* dB Ref P142 开关 */
					   default:break;
				   }
				   break;
			case 2:
				   switch (pp->pos1[2])
				   {
					   case 0:data_process(&(pp->p_tmp_config->agate_start_reg), 3); break; /* 202 agate start*/
					   case 1:
						//if (CFG(alarm[CFG(alarm_pos)].conditiona)== 0)
						//	CFG(alarm[CFG(alarm_pos)].conditionb) = 0;
 /* 弹出一个选择菜单,选择 */                         break; 
					   case 2:data_process(&(pp->p_tmp_config->count_reg), 1); break; /* 222 output count*/
					   case 3:
						if( GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 || GROUP_VAL(curve_pos)==3 )
						{
							if( GROUP_VAL(mode_pos)==0 )
							data_process(&(pp->p_tmp_config->ref_ampl_reg), 2);
							else
							{
								if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==3)
								data_process(&(pp->p_tmp_config->position_reg), 3);
								else
								data_process(&(pp->p_tmp_config->delay_reg), 3);
							}
							
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
					   case 1: data_process(&(pp->p_tmp_config->cursors_scan_reg), 3); break; /*312*/
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
					   case 1: pp->p_config->sizing_curves = !pp->p_config->sizing_curves;break;  /*412*/
					   case 2: break; 
					   case 3: 
							   if (CFG(color_select == 0))
								   data_process(&(pp->p_tmp_config->color_end_reg), 1);  /*432 */
							   else if (CFG(color_select == 1))
								   data_process(&(pp->p_tmp_config->brightness_reg), 1);
							   else if (CFG(color_select == 2))
								   data_process(&(pp->p_tmp_config->max_reg), 2);
							   break;  /*432 */
					   case 4: pp->p_config->optimum = !pp->p_config->optimum;break;
					   default:break;
				   }
				   break;
			case 5:
				   switch (pp->pos1[5])
				   {
					   case 0: break; 
					   case 1: data_process(&(pp->p_tmp_config->skew_reg), 2); break;  /*512*/ 
					   case 2: data_process(&(pp->p_tmp_config->agate_start_reg), 3); break;  /*522*/
					   case 3: data_process(&(pp->p_tmp_config->diameter_reg), 3); break;  /*532*/
					   case 4: break;
					   default:break;
				   }
				   break;
			case 6:
				   switch (pp->pos1[6])
				   {
					   case 0: data_process(&(pp->p_tmp_config->connection_R_reg), 2); break;  /*602*/
					   case 1: data_process(&(pp->p_tmp_config->last_element_reg), 2);  break; /*612 */ 
					   case 2: data_process(&(pp->p_tmp_config->angle_step_reg), 2); break;  /*622*/
					   case 3: break;  /*632*/
					   case 4: break;
					   default:break;
				   }
				   break;
			case 7:
				   switch (pp->pos1[7])
				   {
					   case 0: break; /*    */
					   case 1: break; /*712 */
					   case 2: data_process (&(pp->p_tmp_config->scan_resolution_reg), 2);  break;   /*722  */
					   case 3: break; 
					   default:break;
				   }
				   break;
			case 8:
				   switch (pp->pos1[8])
				   {
					   case 0: break; 
					   case 1: break;
					   case 2: pp->p_config->format_setup = !pp->p_config->format_setup;break;   /*822  */
					   case 3: break; 
					   default:break;
				   }
				   break;


			default:break;
		}
	}

	pp->pos_pos = MENU3_PRESSED;
	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

void b3_fun3(gpointer p)
{
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
						send_dsp_data (VIDEO_FILTER_DSP, GROUP_VAL(video_filter)); /* P123 */
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
			case 0:break;
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
					   case 0:data_process(&(pp->p_tmp_config->agate_width_reg), 3); break; /* 203 agate width 3种步进 */
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
					   case 1: data_process(&(pp->p_tmp_config->cursors_index_reg), 2); break;/* 313  index */ 
					   case 2: /* 视频滤波 */ break; 
					   case 3: data_process(&(pp->p_tmp_config->echo_qty_reg), 1); break;  /*333 */
					   case 4: /* 弹出一个选择菜单,选择 */ break; 
					   default:break;
				   }
				   break;
			case 4:
				   switch (pp->pos1[4])
				   {
					   case 0: break;
					   case 1: pp->p_config->overlay_gate = !pp->p_config->overlay_gate;break; /*413*/
					   case 2: break; 
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
					   case 2: data_process(&(pp->p_tmp_config->agate_width_reg), 3);  break; 
					   case 3: break;  
					   case 4: break; 
					   default:break;
				   }
				   break;

			case 6:
				   switch (pp->pos1[6])
				   {
					   case 0: break;
					   case 1: data_process(&(pp->p_tmp_config->element_step_reg), 2);  break; /*613 */
					   case 2: data_process(&(pp->p_tmp_config->focus_depth_reg), 2);  break;  /*623*/
					   case 3: break;  
					   case 4: break; 
					   default:break;
				   }
				   break;
			case 7:
				   switch (pp->pos1[7])
				   {
					   case 0: break;
					   case 1: data_process(&(pp->p_tmp_config->scan_speed_reg), 2);  break; 
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
					   case 2: pp->p_config->format_note = !pp->p_config->format_note;break;   /*823  */
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
					   case 3: pp->p_config->remote_desktop = !pp->p_config->remote_desktop;break;   /*933  */ 
					   default:break;
				   }
				   break;

			default:break;
		}
	}

	pp->pos_pos = MENU3_PRESSED;
	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

void b3_fun4(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 4;
	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:break;
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
					   case 1: break;  /* 314 */ 
					   case 2: data_process(&(pp->p_tmp_config->entry_reg), 1); break;  /*324 */
					   case 3: break;  /*334 */
					   case 4: break; 
					   default:break;
				   }
				   break;
			case 4:
				   switch (pp->pos1[4])
				   {
					   case 0:break; 
					   case 1:pp->p_config->overlay_cursor = !pp->p_config->overlay_cursor;break; /*414*/
					   case 2:break;
					   case 3:break; 
					   case 4:break; 
					   default:break;
				   }
				   break;

			case 7:
				   switch (pp->pos1[7])
				   {
					   case 0:break; 
					   case 1:break; 
					   case 2:data_process(&(pp->p_tmp_config->index_end_reg), 2);  break; /*724*/
					   case 3:break; 
					   case 4:break; 
					   default:break;
				   }
				   break;

			default:break;

		}
	}

	pp->pos_pos = MENU3_PRESSED;
	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

void b3_fun5(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 5;
	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]]) &&
			(pp->pos_pos == MENU3_PRESSED));
	{
		switch (pp->pos) 
		{
			case 0:break;
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
				   break;

			case 4:
				   switch (pp->pos1[4])
				   {
					   case 0:break; 
					   case 1:pp->p_config->overlay_overlay = !pp->p_config->overlay_overlay;break; /*415*/
					   case 2:break;
					   case 3:break; 
					   case 4:break; 
					   default:break;
				   }
				   break;
			case 5:
				   switch (pp->pos1[5])
				   {
					   case 0:pp->p_config->auto_detect = !pp->p_config->auto_detect;break;  /*505*/
					   case 1:break; /*515*/
					   case 2:break;
					   case 3:break; 
					   case 4:break; 
					   default:break;
				   }
				   break;

			case 7:
				   switch (pp->pos1[7])
				   {
					   case 0:break; 
					   case 1:break; 
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
	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

static void handler_key(guint keyval)
{
	guchar tmp = pp->pos_pos;
	guchar tmp1 = pp->mark_pop_change;
	switch (keyval) 
	{
		case GDK_Escape:  /* 按下Escape键 */
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
			break;
		case GDK_Return: /* 按下enter键 */
			switch (pp->pos_pos)
			{
				case MENU2_STOP:
					pp->pos_pos = MENU3_STOP;
					break;
				case MENU2_PRESSED:
				case MENU3_STOP:
					pp->pos_pos = MENU3_PRESSED;
					/* 按下的动作在这里实现 */
					break;
				case MENU3_PRESSED:
					pp->pos_pos = MENU3_STOP;
					break;
				default:break;
			}
			break;
		case GDK_F7: /* 按下 F7 */
			if (gtk_widget_get_visible(pp->menu3) && CUR_POS == 5) /* 处理弹出窗口 */
			{
				gtk_menu_item_deselect  (GTK_MENU_ITEM (pp->menu_item3[pp->pop_pos]));
				if (pp->pop_pos < pp->pop_qty - 1)	
					pp->pop_pos += 1;
				else 
					pp->pop_pos = 0;
//				gtk_menu_set_active (GTK_MENU (pp->menu3), pp->pop_pos);
				gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pp->pop_pos]));
//				gtk_menu_shell_select_item (GTK_MENU_SHELL(pp->menu3),
//						GTK_WIDGET (pp->menu_item3[pp->pop_pos]));
				g_print("%dddd", pp->pop_pos);
			}
			else 
				b3_fun5(GUINT_TO_POINTER (0x01));
			break;
		case GDK_F8: /* 按下 F8 */
				b3_fun4(GUINT_TO_POINTER (0x01));
			break;
		case GDK_F9: /* 按下 F9 */
				b3_fun3(GUINT_TO_POINTER (0x01));
			break;
		case GDK_F10: /* 按下 F10 */
				b3_fun2(GUINT_TO_POINTER (0x01));
			break;
		case GDK_F11: /* 按下 F11 */
				b3_fun1(GUINT_TO_POINTER (0x01));
			break;
		case GDK_F12: /* 按下 F12 */
				b3_fun0(GUINT_TO_POINTER (0x01));
			break;
			/* 上下左右 */
		case GDK_Up:
			switch (pp->pos_pos)
			{
				case MENU2_STOP:
					pp->pos_last1 = pp->pos1[pp->pos];
					pp->pos1[pp->pos] < (pp->menu2_qty - 1) ? pp->pos1[pp->pos]++ :  (pp->pos1[pp->pos] = 0);
					draw_2_menu(0);
					draw_3_menu(1, NULL);
					break;
				case MENU2_PRESSED:
					break;
				case MENU3_STOP:
					break;
				case MENU3_PRESSED:
					break;
			}
			break;
		case GDK_Right:
			break;
		case GDK_Down:
			switch (pp->pos_pos)
			{
				case MENU2_STOP:
					pp->pos_last1 = pp->pos1[pp->pos];
					pp->pos1[pp->pos] > 0 ? pp->pos1[pp->pos]-- :  (pp->pos1[pp->pos] = (pp->menu2_qty - 1));
					draw_2_menu(0);
					draw_3_menu(1, NULL);
					break;
				case MENU2_PRESSED:
					break;
				case MENU3_STOP:
					break;
				case MENU3_PRESSED:
					break;
			}
			break;
		case GDK_Left:
			break;
		default:break;
	}

	return ;
}

/* 快捷键处理函数 */
gboolean key_press_handler (GtkWidget* pWidget,	GdkEventKey* pEvent, gpointer data)
{
	if (pEvent->type == GDK_KEY_PRESS)
	{
		handler_key(pEvent->keyval);
	}
	return TRUE;
}

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data)
{
	handler_key(keyval);
	return 0;
}

gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	b3_fun0(data);
	return TRUE;
}

gboolean data_function1 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	b3_fun1(data);
	return TRUE;
}

gboolean data_function2 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	b3_fun2(data);
	return TRUE;
}

gboolean data_function3 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	b3_fun3(data);
	return TRUE;
}

gboolean data_function4 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	b3_fun4(data);
	return TRUE;
}

gboolean data_function5 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	b3_fun5(data);
	return TRUE;
}

void data_022 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Calibration -> Type */
{
	pp->p_config->type = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_023 (GtkMenuItem *menuitem, gpointer data) /* Wizard -> Calibration -> Mode */
{
	pp->p_config->calibration_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_100 (GtkSpinButton *spinbutton, gpointer data) /* 增益Gain P100 */
{
	gchar *markup;
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
		/* 发送增益给硬件 */
}

void data_101 (GtkSpinButton *spinbutton, gpointer data) /*Start 扫描延时 P101 */
{
	if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ; 

	/*发送增益给硬件*/
}

void data_102 (GtkSpinButton *spinbutton, gpointer data) /*Range 范围 P102 */
{
	if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_VAL(range) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_VAL(range) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_VAL(range) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 

	/*发送增益给硬件*/
}

void data_103 (GtkSpinButton *spinbutton, gpointer data) /*楔块延时  P103 */
{
	GROUP_VAL(wedge_delay) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;

	/*发送增益给硬件*/
}

void data_104 (GtkSpinButton *spinbutton, gpointer data) /*声速 P104 */
{
	if (UNIT_MM == CFG(unit))
		GROUP_VAL(velocity) = (guint) (gtk_spin_button_get_value (spinbutton) * 100);
	else   /* 英寸/微秒 */
		GROUP_VAL(velocity) = (guint) (gtk_spin_button_get_value (spinbutton) * 25400 * 100 );

	/*发送增益给硬件*/
}

void data_110 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	GROUP_VAL(pulser) =  (guchar) (gtk_spin_button_get_value (spinbutton));
	if (GROUP_VAL(tx_rxmode) == PULSE_ECHO)
		GROUP_VAL(receiver) =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送增益给硬件*/
}

void data_120 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	GROUP_VAL(receiver) =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送增益给硬件*/
}

void data_111 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode P111 */
{
	GROUP_VAL(tx_rxmode) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	send_dsp_data (TX_RX_MODE_DSP, GROUP_VAL(tx_rxmode));
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_1121 (GtkSpinButton *spinbutton, gpointer data) /* 频率 Freq 数值改变 */
{
	GROUP_VAL(frequence) =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);

	send_dsp_data (FREQUENCE_DSP, (guint) (GROUP_VAL(frequence)));
	/* 发送给硬件 */
}


void data_112 (GtkMenuItem *menuitem, gpointer data) /* 频率 Freq P112 */
{
	guint temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(freq_pos) = (gushort) (GPOINTER_TO_UINT (data));
	GROUP_VAL(frequence) = get_freq();
	if (temp != 12)
	{
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
		send_dsp_data (FREQUENCE_DSP, (guint) (GROUP_VAL(frequence)));
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
	/* 发送增益给硬件 */
}


void data_113 (GtkMenuItem *menuitem, gpointer data)  /* Voltage  P113 */
{
	if (CFG(groupId) != UT_CHANNEL)
	{
		CFG(voltage_pa) = (gchar) (GPOINTER_TO_UINT (data));
		send_dsp_data (VOLTAGE_DSP, CFG(voltage_pa));
	}
	else
	{
		CFG(voltage_ut) = (gchar) (GPOINTER_TO_UINT (data));
		send_dsp_data (VOLTAGE_DSP, CFG(voltage_ut));
	}
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}


void data_1141 (GtkSpinButton *spinbutton, gpointer data) /* PW  P114 */
{
	guint temp;
	temp =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 100.0);
	temp = (temp / 250) * 250;
	GROUP_VAL(pulser_width) = temp;

	send_dsp_data (PW_DSP, GROUP_VAL(pulser_width) / 250) ;  /* 以2.5ns 为单位发出信息 */
	/* 发送给硬件 */
}


void data_114 (GtkMenuItem *menuitem, gpointer data) /* PW */
{
	guint temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(pw_pos) = temp;
	GROUP_VAL(pulser_width) = get_pw();
	if (!temp)
	{
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
		send_dsp_data (PW_DSP, GROUP_VAL(pulser_width) / 250) ;  /* 以2.5ns 为单位发出信息 */
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
	/* 发送增益给硬件 */
}

void data_1151 (GtkSpinButton *spinbutton, gpointer data) /* PRF P115 */
{
	gchar *markup;
	//	DRAW_UI_P p = (DRAW_UI_P)(data);
	GROUP_VAL(prf) =  (guint) (gtk_spin_button_get_value (spinbutton) * 10);
	markup=g_markup_printf_escaped(
			"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",GROUP_VAL(prf) / 10, (GROUP_VAL(prf) / 10) * 1);
	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
	g_free(markup);

	markup = g_markup_printf_escaped (
			"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",(gfloat)(GROUP_VAL(prf) / 10.0));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;

	g_free(markup);
	send_dsp_data (PRF_DSP, get_prf());
	/* 发送给硬件 */
}


void data_115 (GtkMenuItem *menuitem, gpointer data) /* PRF */
{
	guint temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(prf_pos) = temp;
	GROUP_VAL(prf) = get_prf();
	if (temp != 3)
	{
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
		send_dsp_data (PRF_DSP, get_prf());
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
}


void data_121 (GtkMenuItem *menuitem, gpointer data)  /* filter */
{
    GROUP_VAL(filter) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (FILTER_DSP, get_filter());
	/* 发送增益给硬件 */
}

void data_122 (GtkMenuItem *menuitem, gpointer data)  /* Rectifier 检波 P122 */
{
	GROUP_VAL(rectifier) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);

	send_dsp_data (RECTIFIER_DSP, GROUP_VAL(rectifier));
	/* 发送增益给硬件 */
}

void data_124 (GtkMenuItem *menuitem, gpointer data)  /* averaging */
{
	GROUP_VAL(averaging) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);

	send_dsp_data (AVERAGING_DSP, GROUP_VAL(averaging));
	/* 发送增益给硬件 */
}


void data_125 (GtkSpinButton *spinbutton, gpointer data) /*抑制 Reject */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->reject =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送增益给硬件*/
}

void data_130 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->scan_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_131 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->index_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_132 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->angle =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);

	/*发送增益给硬件*/
}

void data_134 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	//pp->p_config->beam_delay =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	BEAM_INFO(0,beam_delay) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);

	/*发送增益给硬件*/
}

void data_135 (GtkSpinButton *spinbutton, gpointer data) /*gain offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	GROUP_VAL(gain_offset) =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);

	/*发送增益给硬件*/
}


void data_1431 (GtkSpinButton *spinbutton, gpointer data) /* point qty */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	//pp->p_config->point_qty =  (guint) (gtk_spin_button_get_value (spinbutton));
	GROUP_VAL(point_qty) =  (guint) (gtk_spin_button_get_value (spinbutton));

}


void data_143 (GtkMenuItem *menuitem, gpointer data) /* point qty */
{
	guint temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(point_qty_pos) = temp;
	GROUP_VAL(point_qty) = get_point_qty();
	if (temp != 4)
	{
		//CFG(point_qty) = (gushort) (GPOINTER_TO_UINT (data));
		MENU_STATUS = MENU3_STOP;
		draw_3_menu(0, NULL);
		send_dsp_data (POINT_QTY_DSP, get_point_qty());
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
}

void data_1451 (GtkSpinButton *spinbutton, gpointer data) /* Sum Gain */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	GROUP_VAL(sum_gain) =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 100.0);

	/* 发送给硬件 */
}


void data_145 (GtkMenuItem *menuitem, gpointer data) /* Sum Gain */
{
	guint temp = GPOINTER_TO_UINT (data);
	GROUP_VAL(sum_gain_pos) = temp;
	GROUP_VAL(sum_gain) = get_sum_gain();
	if (temp != 1)
	{
		//CFG(sum_gain) = (gushort) temp;
		MENU_STATUS = MENU3_STOP;
		draw_3_menu(0, NULL);
		send_dsp_data (SUM_GAIN_DSP, get_sum_gain());
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
	/* 发送增益给硬件 */
}

void data_200 (GtkMenuItem *menuitem, gpointer data) /* Gate 闸门选择 P200 */
{
	GROUP_VAL(gate_pos) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);

	send_dsp_data (GATE_POS_DSP, GROUP_VAL(gate_pos));
}

void data_201 (GtkMenuItem *menuitem, gpointer data) /* parameter 闸门参数选择 位置或者模式 P201 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].parameters) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);

	send_dsp_data (PARAMETERS_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].parameters));
}

void data_202 (GtkSpinButton *spinbutton, gpointer data)	/* 闸门开始位置 P202 */
{
	if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_GATE_POS(start) = (gint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ; 

	send_dsp_data (START_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].start));
}

void data_2021 (GtkMenuItem *menuitem, gpointer data)	/* 闸门同步 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].synchro) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (SYNCHRO_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].synchro));
}

void data_203 (GtkSpinButton *spinbutton, gpointer data) /* 闸门宽度 P203 */
{
	if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (GROUP_VAL(velocity) / 100000.0));
		else  /* 英寸 */
			GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * GROUP_VAL(velocity) / 100000.0));
	}
	else /* 显示方式为时间 */
		GROUP_GATE_POS(width) = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ; 

	send_dsp_data (WIDTH_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].width));
}

void data_2031 (GtkMenuItem *menuitem, gpointer data)	/* 波峰或者前沿 测量选项 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].measure) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (MEASURE_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].measure));
}

void data_204 (GtkSpinButton *spinbutton, gpointer data) /* 闸门高度 P204 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].height) =  (guchar) (gtk_spin_button_get_value (spinbutton) );
	send_dsp_data (HEIGHT_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].height));
}

void data_2041 (GtkMenuItem *menuitem, gpointer data) /* 闸门RF 选择 射频时候才可以调节 */
{
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].rectifier_freq) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (RECTIFIER_FREQ_DSP, GROUP_VAL(gate[GROUP_VAL(gate_pos)].rectifier_freq));
}

void data_210 (GtkMenuItem *menuitem, gpointer data) /* Alarm  P210 */
{
	CFG(alarm_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (ALARM_POS_DSP, CFG(alarm_pos));
	/* 发送增益给硬件 */
}

void data_211 (GtkMenuItem *menuitem, gpointer data) /* Group A P211 */
{
	CFG(alarm[CFG(alarm_pos)].groupa) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (GROUPA_DSP, CFG(alarm[CFG(alarm_pos)].groupa));
	/* 发送增益给硬件 */
}
void data_212 (GtkMenuItem *menuitem, gpointer data) /* Condition A P212 */
{
	CFG(alarm[CFG(alarm_pos)].conditiona) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (CONDITIONA_DSP, CFG(alarm[CFG(alarm_pos)].conditiona));
	/* 发送增益给硬件 */
}

void data_213 (GtkMenuItem *menuitem, gpointer data) /* operator P213 */
{
	CFG(alarm[CFG(alarm_pos)].operat) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (OPERAT_DSP, CFG(alarm[CFG(alarm_pos)].operat));
}

void data_214 (GtkMenuItem *menuitem, gpointer data) /* Group B P214 */
{
	CFG(alarm[CFG(alarm_pos)].groupb) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (GROUPB_DSP, CFG(alarm[CFG(alarm_pos)].groupb));
}

void data_215 (GtkMenuItem *menuitem, gpointer data) /* condition B P215 */
{
	CFG(alarm[CFG(alarm_pos)].conditionb) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (CONDITIONB_DSP, CFG(alarm[CFG(alarm_pos)].conditionb));
}

void data_220 (GtkMenuItem *menuitem, gpointer data) /* Output P220 */
{	
	CFG(output_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (OUTPUT_POS_DSP, CFG(output_pos));
}

void data_221 (GtkMenuItem *menuitem, gpointer data) /* Output -> alarm # P221 */
{
	guchar temp = (guchar) (GPOINTER_TO_UINT (data));
	gushort tmp1 = 0;
	gint i;
	if (temp != 18)
		CFG_OUTPUT_POS(alarm1) = (guchar) (GPOINTER_TO_UINT (data));
	if (temp == 0)
	{
		CFG_OUTPUT_POS(alarm1_status) = 0x0;
		CFG_OUTPUT_POS(alarm1_qty) = 0;
	}
	else if (temp == 1)
	{
		CFG_OUTPUT_POS(alarm1_status) = 0xffff;
		CFG_OUTPUT_POS(alarm1_qty) = 16;
	}
	else if (temp == 18)
	{
	}
	else 
	{
		tmp1 = ((0x1 << (temp - 2)) & CFG_OUTPUT_POS(alarm1_status));
		if (tmp1) 
		{
			CFG_OUTPUT_POS(alarm1_status) &= (~(0x01 << (temp - 2)));
			CFG_OUTPUT_POS(alarm1_qty) -= 1;
		}
		else
		{
			CFG_OUTPUT_POS(alarm1_status) |= ((0x01 << (temp - 2)));
			CFG_OUTPUT_POS(alarm1_qty) += 1;
		}
	}
	if ((CFG_OUTPUT_POS(alarm1_qty) == 0) ||
			(CFG_OUTPUT_POS(alarm1_qty) == 16))
	{
	}
	else if (CFG_OUTPUT_POS(alarm1_qty) == 1)
	{
		for ( i = 0 ; i < 16; i++) 
		{
			if (( 0x01 << i) & CFG_OUTPUT_POS(alarm1_status)) {
				CFG_OUTPUT_POS(alarm1) = i + 2;
				break;
			}
		}
	}
	else 
		CFG_OUTPUT_POS(alarm1) = 18;

		pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (ALARM1_DSP, CFG_OUTPUT_POS(alarm1));
}

void data_2211 (GtkMenuItem *menuitem, gpointer data) /* Output -> group */
{
	CFG_ANALOG_POS(group) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (GROUPA_DSP, CFG_ANALOG_POS(group));
}

void data_222 (GtkSpinButton *spinbutton, gpointer data) /* count P222 */
{
	CFG_OUTPUT_POS(count) =  (guchar) (gtk_spin_button_get_value (spinbutton));
	send_dsp_data (COUNT_DSP, CFG_OUTPUT_POS(count));
}

void data_2221 (GtkMenuItem *menuitem, gpointer data) /* count */
{
	CFG_ANALOG_POS(data) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (DATA_DSP, CFG_ANALOG_POS(data));
}

void data_223 (GtkMenuItem *menuitem, gpointer data) /* sound P223 */
{
	CFG_OUTPUT_POS(sound) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (SOUND_DSP, CFG_OUTPUT_POS(sound));
}

void data_224 (GtkSpinButton *spinbutton, gpointer data) /*active_delay */
{
	CFG_OUTPUT_POS(delay) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	send_dsp_data (DELAY_DSP, CFG_OUTPUT_POS(delay));
}

void data_225 (GtkSpinButton *spinbutton, gpointer data) /* holdtime P225*/
{
	CFG_OUTPUT_POS(holdtime) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	send_dsp_data (HOLDTIME_DSP, CFG_OUTPUT_POS(holdtime));
}

void data_230 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Mode P230 */
{
	GROUP_VAL(mode_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (MODE_POS_DSP, GROUP_VAL(mode_pos));
}

void data_231 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Curve P231 */
{
	GROUP_VAL(curve_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (CURVE_POS_DSP, GROUP_VAL(curve_pos));
}

void data_2311 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Point 231 */
{
	GROUP_VAL(point_pos) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	send_dsp_data (POINT_POS_DSP, GROUP_VAL(point_pos));
}

void data_2312 (GtkSpinButton *spinbutton, gpointer data) /* Mat.Attenuatior P2312 */
{
	GROUP_VAL(mat_atten) =  (gushort) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}

void data_232 (GtkSpinButton *spinbutton, gpointer data) /* Ref.Amplitude P232 */
{
	GROUP_VAL(ref_ampl) =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}
void data_2321 (GtkSpinButton *spinbutton, gpointer data) /* Position P2321 */
{
	GROUP_VAL(position) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}
void data_2322 (GtkSpinButton *spinbutton, gpointer data) /* Delay P2322 */
{
	GROUP_VAL(delay) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
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
	//send_dsp_data (REF_AMPL_DSP, GROUP_VAL(ref_ampl));
}

void data_300 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> list 300 */
{
	pp->p_config->list = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_302 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field1 302 */
{
	pp->p_config->field1 = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_303 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field2 303 */
{
	pp->p_config->field2 = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_304 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field3 304 */
{
	pp->p_config->field3 = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_305 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Reading -> Field4 305 */
{
	pp->p_config->field4 = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_310 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> cursors -> selection 310 */
{
	pp->p_config->select = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}


void data_311 (GtkSpinButton *spinbutton, gpointer data) /*VPA */
{
	pp->p_config->VPA =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_312 (GtkSpinButton *spinbutton, gpointer data) /*VPA */
{
	pp->p_config->cursors_scan =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_330 (GtkMenuItem *menuitem, gpointer data) /* Measurements -> Thickness -> source 330 */
{
	pp->p_config->source = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_313 (GtkSpinButton *spinbutton, gpointer data) /*Index */
{
	pp->p_config->cursors_index =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_324 (GtkSpinButton *spinbutton, gpointer data) /*entry_qty */
{
	pp->p_config->entry_qty =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_331 (GtkSpinButton *spinbutton, gpointer data) /*min_thickness */
{
	pp->p_config->min_thickness =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_332 (GtkSpinButton *spinbutton, gpointer data) /*max_thickness */
{
	pp->p_config->max_thickness =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_333 (GtkSpinButton *spinbutton, gpointer data) /*echo_qty */
{
	pp->p_config->echo_qty =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_400 (GtkMenuItem *menuitem, gpointer data) /* Display -> Selection -> display 400 */
{
	pp->p_config->display = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	draw_area_all();
}

void data_410 (GtkMenuItem *menuitem, gpointer data) /* Display -> Overlay -> UT Unit 410 */
{
	CFG(ut_unit) = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	draw_area_all();
}

void data_411 (GtkMenuItem *menuitem, gpointer data) /* Display -> Overlay -> Grid 411 */
{
	pp->p_config->grid = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_420 (GtkMenuItem *menuitem, gpointer data) /* Display -> Zoom -> Display 420 */
{
	pp->p_config->zoom_display = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_430 (GtkMenuItem *menuitem, gpointer data) /* Display -> Color -> select 430 */
{
	pp->p_config->color_select = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_431 (GtkSpinButton *spinbutton, gpointer data) /*color_start */
{
	pp->p_config->color_start =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_4311 (GtkSpinButton *spinbutton, gpointer data) /*color_contrast */
{
	pp->p_config->color_contrast =  (guint) (gtk_spin_button_get_value (spinbutton));
}
void data_4312 (GtkSpinButton *spinbutton, gpointer data) /*color_min */
{
	pp->p_config->min =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}

void data_432 (GtkSpinButton *spinbutton, gpointer data) /*color_end */
{
	pp->p_config->color_end =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_4321 (GtkSpinButton *spinbutton, gpointer data) /*color_brightness */
{
	pp->p_config->brightness =  (guint) (gtk_spin_button_get_value (spinbutton));
}
void data_4322 (GtkSpinButton *spinbutton, gpointer data) /*color_max */
{
	pp->p_config->max =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}

void data_434 (GtkMenuItem *menuitem, gpointer data) /* Display -> color -> mode 434 */
{
	pp->p_config->color_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_440 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> scan 440 */
{
	pp->p_config->prop_scan = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_441 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Color 441 */
{
	pp->p_config->color = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_4411 (GtkSpinButton *spinbutton, gpointer data) /*Display -> Properties -> Compress 4411 */
{
	pp->p_config->compress =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}
void data_4414 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Color 4414 */
{
	pp->p_config->fft_color = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_4415 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Color 4415 */
{
	pp->p_config->orientation = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_442 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Envelope 442 */
{
	pp->p_config->envelope = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_443 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Source 443 */
{
	pp->p_config->prop_source = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_444 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Appearance 444 */
{
	pp->p_config->prop_app = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_445 (GtkMenuItem *menuitem, gpointer data) /* Display -> Properties -> Overlay 445 */
{
	pp->p_config->prop_overlay = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_501 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Select -> Group Mode 501 */
{
	pp->p_config->group_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_502 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Select -> Select 502 */
{
	pp->p_config->probe_select = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_510 (GtkSpinButton *spinbutton, gpointer data) /*scanoffset */
{
	pp->p_config->scan_offset =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_511 (GtkSpinButton *spinbutton, gpointer data) /*indexoffset */
{
	pp->p_config->index_offset =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_5121 (GtkSpinButton *spinbutton, gpointer data) /* Skew (deg) */
{
	pp->p_config->skew =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 100.0);
}


void data_512 (GtkMenuItem *menuitem, gpointer data) /* Skew (deg) */
{
	guint temp = GPOINTER_TO_UINT (data);
	if (temp != 4)
	{
		CFG(skew) = (gushort) temp;
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
}

void data_521 (GtkSpinButton *spinbutton, gpointer data) /*gain */
{
	GROUP_VAL(gain) =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_522 (GtkSpinButton *spinbutton, gpointer data) /*agate_start */
{
	pp->p_config->agate_start =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_523 (GtkSpinButton *spinbutton, gpointer data) /*agate_width */
{
	pp->p_config->agate_width =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_530 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Parts -> Geometry 530 */
{
	pp->p_config->part.Geometry = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_531 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	pp->p_config->part_thickness =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}
void data_532 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	pp->p_config->part.Diameter =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_533 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Parts -> Material 533 */
{
	pp->p_config->part.Material = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_600 (GtkMenuItem *menuitem, gpointer data) /* Focal law -> Configuration -> Law Config. */
{
	pp->p_config->law_config = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_601 (GtkSpinButton *spinbutton, gpointer data) /*connection_P*/
{
	pp->p_config->connection_P =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_610 (GtkSpinButton *spinbutton, gpointer data) /*element_qty*/
{
	pp->p_config->element_qty =  (guchar) (gtk_spin_button_get_value (spinbutton));
}

void data_611 (GtkSpinButton *spinbutton, gpointer data) /*first_element*/
{
	pp->p_config->first_element =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_612 (GtkSpinButton *spinbutton, gpointer data) /*last_element*/
{
	pp->p_config->last_element =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_613 (GtkSpinButton *spinbutton, gpointer data) /*element_step*/
{
	pp->p_config->element_step =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_614 (GtkMenuItem *menuitem, gpointer data) /* Focal law -> Configuration -> Wave Type */
{
	pp->p_config->wave_type = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_620 (GtkSpinButton *spinbutton, gpointer data) /*min_angle*/
{
	pp->p_config->min_angle =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_623 (GtkSpinButton *spinbutton, gpointer data) /*focus_depth*/
{
	pp->p_config->focus_depth =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_710 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Inspection -> type */
{
	pp->p_config->scan_type = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_711 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Inspection -> scan */
{
	pp->p_config->inspec_scan = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_713 (GtkSpinButton *spinbutton, gpointer data) /*scan_speed*/
{
	pp->p_config->scan_speed =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
}

void data_720 (GtkSpinButton *spinbutton, gpointer data) /*scan_start*/
{
	pp->p_config->scan_start =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_721 (GtkSpinButton *spinbutton, gpointer data) /*scan_end*/
{
	pp->p_config->scan_end =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_722 (GtkSpinButton *spinbutton, gpointer data) /*scan_resolution*/
{
	pp->p_config->scan_resolution =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_723 (GtkSpinButton *spinbutton, gpointer data) /*index_start*/
{
	pp->p_config->index_start =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_724 (GtkSpinButton *spinbutton, gpointer data) /*index_end*/
{
	pp->p_config->index_end =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_725 (GtkSpinButton *spinbutton, gpointer data) /*index_resolution*/
{
	pp->p_config->index_resolution =  (guint) (gtk_spin_button_get_value (spinbutton));
}

void data_730 (GtkMenuItem *menuitem, gpointer data) /* Scan -> start -> start mode */
{
	pp->p_config->start_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_740 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Data -> storage */
{
	pp->p_config->storage = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_741 (GtkMenuItem *menuitem, gpointer data) /* Scan -> Data -> inspec.data */
{
	pp->p_config->inspec_data = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_800 (GtkMenuItem *menuitem, gpointer data) /*  File -> File -> Storage */
{
	pp->p_config->file_storage = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_804 (GtkMenuItem *menuitem, gpointer data) /* File -> File -> Save Mode */
{
	pp->p_config->save_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_812 (GtkMenuItem *menuitem, gpointer data) /* File -> Report -> paper size */
{
	pp->p_config->paper_size = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_824 (GtkMenuItem *menuitem, gpointer data) /* File -> Format -> view*/
{
	pp->p_config->view = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_830(GtkMenuItem *menuitem, gpointer data) /* File -> User Field -> select */
{
	pp->p_config->file_select = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_900(GtkMenuItem *menuitem, gpointer data) /* Preferences -> Pref. -> Units */
{
	CFG(unit) = (guchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_903(GtkMenuItem *menuitem, gpointer data) /* Preferences -> Pref. -> scheme */
{
	pp->p_config->scheme = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_904(GtkMenuItem *menuitem, gpointer data) /* Preferences -> Pref. -> gate mode */
{
	pp->p_config->gate_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_912(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> select key */
{
	pp->p_config->select_key = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_913(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> assign key*/
{
	pp->p_config->assign_key = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_924(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> assign key*/
{
	pp->p_config->startup_mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_930(GtkMenuItem *menuitem, gpointer data) /* Preferences -> system -> assign key*/
{
	pp->p_config->mouse = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
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
		case FREQUENCE_DSP:
					   g_print("%d\n", value);
					   break;
		case VOLTAGE_DSP:
					   if (CFG(groupId) != 3)
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
