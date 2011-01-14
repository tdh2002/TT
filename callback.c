
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

gfloat get_freq ();

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
void data_222 (GtkSpinButton *spinbutton, gpointer data);
void data_223 (GtkMenuItem *menuitem, gpointer data);          /* 223 sound */
void data_224 (GtkSpinButton *spinbutton, gpointer data);
void data_225 (GtkSpinButton *spinbutton, gpointer data);
void data_230 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm -> Sizing Curves -> Mode 230 */
void data_231 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm -> Sizing Curves -> Curve 231 */

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

gfloat get_freq ()
{
	if (CFG(frequence) <99)
	{
		switch (CFG(frequence))
		{
			case 0:return 1000.0;break;
			case 1:return 1500.0;break;
			case 2:return 2000.0;break;
			case 3:return 2250.0;break;
			case 4:return 3500.0;break;
			case 5:return 4000.0;break;
			case 6:return 5000.0;break;
			case 7:return 7500.0;break;
			case 8:return 10000.0;break;
			case 9:return 12000.0;break;
			case 10:return 15000.0;break;
			case 11:return 20000.0;break;
			default:break;
		}
	}
	return CFG(frequence);
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
	pp->pos_pos = MENU3_PRESSED;

	/* 一次点击处理 */
	switch (pp->pos)
	{
		case 1:
			   switch (pp->pos1[1])
			   {
				   case 4: 
					   CFG(gainr) = CFG(gain);
					   pp->pos_pos = MENU3_STOP;
					   if (CFG(db_ref))
						   markup = g_markup_printf_escaped (
								   "<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
								   (CFG(gain) - CFG(gainr)) / 100.0, CFG(gainr) / 100.0);
					   else
						   markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
								   CFG(gain) / 100.0 );
					   gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);

					   g_free(markup);
					   break; /* Set Ref P141 设置参考增益值*/
				   default:break;
			   }
			   break;
		default:break;
	}


	/*处理微调*/
	if (pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
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
			case 2: break;
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
	pp->pos_pos = MENU3_PRESSED;
	/*处理微调*/
	switch (pp->pos)
	{
		case 1:
			   switch (pp->pos1[1])
			   {
				   case 4: 
					   CFG(db_ref) = !CFG(db_ref);
					   pp->pos_pos = MENU3_STOP;
					   if (CFG(db_ref))
					   {
						   tt_label_show_string (pp->label[GAIN_LABEL], con2_p[1][0][7], "\n", "(dB)", "white", 10);
						   markup = g_markup_printf_escaped (
								   "<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
								   (CFG(gain) - CFG(gainr)) / 100.0, CFG(gainr) / 100.0);
					   }
					   else
					   {
						   tt_label_show_string (pp->label[GAIN_LABEL], con2_p[1][0][0], "\n", "(dB)", "white", 10);
						   markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
								   CFG(gain) / 100.0 );
					   }
					   gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);
					   g_free(markup);
					   break; /* dB Ref P142 开关 */
				   default:break;
			   }
				   break;
		default:break;
	}

	if (pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
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
								  data_process(&(pp->p_tmp_config->frequence_reg), 2); /* 112 频率范围 3种步进 */
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
					   case 1: /* 弹出一个选择菜单,选择 */ break; 
					   case 2:data_process(&(pp->p_tmp_config->count_reg), 1); break; /* 222 output count*/
					   case 3: /* Angle. (deg) */ break; 
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

	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

void b3_fun3(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 3;
	pp->pos_pos = MENU3_PRESSED;
	/*处理微调*/
	if (pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
	{
		switch (pp->pos) 
		{
			case 0:break;
			case 1:
				   switch (pp->pos1[1])
				   {
					   case 0:data_process(&(pp->p_tmp_config->wedge_delay_reg), 2); break;   /* 103wedge delay 楔款延时 3种步进 */
					   case 1: /* 弹出一个选择菜单,选择 */ break; 
					   case 2: pp->p_config->video_filter = !pp->p_config->video_filter;break; /* 123 视频滤波 on or off  */
					   case 3: /* Skew  */ break; /* 133 Skew (deg)  */
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
					   case 3: /* Skew  */ break; /* 133 Skew (deg)  */
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

	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

void b3_fun4(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 4;
	pp->pos_pos = MENU3_PRESSED;
	/*处理微调*/
	if (pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
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
					   case 3:  break; /* 134 beamdelay 波束延时 */
					   case 4: /*  */ break; /* 144 Scale Factor 多少点压缩一点 只能看  */
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

	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

void b3_fun5(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 5;
	pp->pos_pos = MENU3_PRESSED;
	/*处理微调*/
	if (pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
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
					   case 3:data_process(&(pp->p_tmp_config->gainoffset_reg), 4); break; /* 135 Gain Offset  */
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
					   case 3: break; 
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

	draw_2_menu(0);
	draw_3_menu(0, NULL);                          /**/

	return ;
}

static void handler_key(guint keyval)
{
	guchar tmp = pp->pos_pos;
	switch (keyval) 
	{
		case GDK_Escape:
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
		case GDK_Return:
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
		case GDK_F12:
			if (pp->pos_pos == MENU3_PRESSED)
				b3_fun0(GUINT_TO_POINTER (0x01));
			else 
				pp->pos_pos = MENU3_PRESSED;
			break;
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
	if (tmp != pp->pos_pos)
	{
		draw_2_menu(0);
		draw_3_menu(0, NULL);
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
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gchar *markup;
	if (CFG(db_ref))
		p->p_config->gain = (gushort) (gtk_spin_button_get_value (spinbutton) * 100 + CFG(gainr));
	else
		p->p_config->gain = (gushort) (gtk_spin_button_get_value (spinbutton) * 100);

	if (CFG(db_ref))
		markup = g_markup_printf_escaped (
				"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
				(CFG(gain) - CFG(gainr)) / 100.0, CFG(gainr) / 100.0);
	else
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
				CFG(gain) / 100.0 );
	gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);

	g_free(markup);
		/* 发送增益给硬件 */
}

void data_101 (GtkSpinButton *spinbutton, gpointer data) /*Start 扫描延时 P101 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			CFG(start) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (CFG(part.Velocity_LW) / 100000.0));
		else  /* 英寸 */
			CFG(start) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * CFG(part.Velocity_LW) / 100000.0));
	}
	else /* 显示方式为时间 */
		CFG(start) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 

	/*发送增益给硬件*/
}

void data_102 (GtkSpinButton *spinbutton, gpointer data) /*Range 范围 P102 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
	{
		if (UNIT_MM == CFG(unit))
			CFG(range) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / (CFG(part.Velocity_LW) / 100000.0));
		else  /* 英寸 */
			CFG(range) = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 / ( 0.03937 * CFG(part.Velocity_LW) / 100000.0));
	}
	else /* 显示方式为时间 */
		CFG(range) = gtk_spin_button_get_value (spinbutton) * 1000.0 ; 

	/*发送增益给硬件*/
}

void data_103 (GtkSpinButton *spinbutton, gpointer data) /*楔块延时  P103 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->wedge_delay = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;

	/*发送增益给硬件*/
}

void data_104 (GtkSpinButton *spinbutton, gpointer data) /*声速 P104 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	if (UNIT_MM == CFG(unit))
		CFG(part.Velocity_LW) = (guint) (gtk_spin_button_get_value (spinbutton) * 100);
	else   /* 英寸/微秒 */
		CFG(part.Velocity_LW) = (guint) (gtk_spin_button_get_value (spinbutton) * 25400 * 100 );

	/*发送增益给硬件*/
}

void data_110 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->pulser =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送增益给硬件*/
}

void data_111 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode */
{
	pp->p_config->tx_rxmode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_1121 (GtkSpinButton *spinbutton, gpointer data) /* 频率 Freq 数值改变 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->frequence =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);

	/* 发送给硬件 */
}


void data_112 (GtkMenuItem *menuitem, gpointer data) /* 频率 Freq */
{
	guint temp = GPOINTER_TO_UINT (data);
	if (temp != 12)
	{
		CFG(frequence) = (gushort) (GPOINTER_TO_UINT (data));
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
	/* 发送增益给硬件 */
}


void data_113 (GtkMenuItem *menuitem, gpointer data)  /* Voltage */
{
	pp->p_config->voltage_cfg = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}


void data_1141 (GtkSpinButton *spinbutton, gpointer data) /* PW */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->pulser_width =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 100.0);

	/* 发送给硬件 */
}


void data_114 (GtkMenuItem *menuitem, gpointer data) /* PW */
{
	guint temp = GPOINTER_TO_UINT (data);
	if (temp != 1)
	{
		CFG(pulser_width) = (gushort) (GPOINTER_TO_UINT (data));
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
	/* 发送增益给硬件 */
}

void data_1151 (GtkSpinButton *spinbutton, gpointer data) /* PRF */
{
	gchar *markup;
	//	DRAW_UI_P p = (DRAW_UI_P)(data);
	CFG(prf) =  (guint) (gtk_spin_button_get_value (spinbutton));
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>PRF: %d(%d)                        </span>",CFG(prf),CFG(prf));
	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);

	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>V: %.2f mm/s                    </span>",(gfloat)CFG(prf));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;

	g_free(markup);

	/* 发送给硬件 */
}


void data_115 (GtkMenuItem *menuitem, gpointer data) /* PRF */
{

	guint temp = GPOINTER_TO_UINT (data);
	if (temp != 3)
	{
		CFG(prf) = temp;
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


void data_121 (GtkMenuItem *menuitem, gpointer data)  /* filter */
{
	pp->p_config->filter = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_122 (GtkMenuItem *menuitem, gpointer data)  /* Rectifier */
{
	pp->p_config->rectifier = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_124 (GtkMenuItem *menuitem, gpointer data)  /* averaging */
{
	pp->p_config->averaging = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
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
	pp->p_config->beam_delay =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_135 (GtkSpinButton *spinbutton, gpointer data) /*gain offset */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->gain_offset =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);

	/*发送增益给硬件*/
}


void data_1431 (GtkSpinButton *spinbutton, gpointer data) /* point qty */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->point_qty =  (guint) (gtk_spin_button_get_value (spinbutton));

	/* 发送给硬件 */
}


void data_143 (GtkMenuItem *menuitem, gpointer data) /* point qty */
{
	guint temp = GPOINTER_TO_UINT (data);
	if (temp != 4)
	{
		CFG(point_qty) = (gushort) (GPOINTER_TO_UINT (data));
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
	/* 发送增益给硬件 */
}

void data_1451 (GtkSpinButton *spinbutton, gpointer data) /* Sum Gain */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->sum_gain =  (gushort) ((gtk_spin_button_get_value (spinbutton)) * 100.0);

	/* 发送给硬件 */
}


void data_145 (GtkMenuItem *menuitem, gpointer data) /* Sum Gain */
{
	guint temp = GPOINTER_TO_UINT (data);
	if (temp != 1)
	{
		CFG(sum_gain) = (gushort) temp;
		pp->pos_pos = MENU3_STOP;
		draw_3_menu(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_3_menu(0, NULL);
	}
	/* 发送增益给硬件 */
}

void data_200 (GtkMenuItem *menuitem, gpointer data) /* Gate */
{
	pp->p_config->gate = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_201 (GtkMenuItem *menuitem, gpointer data) /* parameter */
{
	pp->p_config->parameter = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_202 (GtkSpinButton *spinbutton, gpointer data) /*agate start */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->agate_start =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);

	/*发送增益给硬件*/
}
void data_2021 (GtkMenuItem *menuitem, gpointer data) /* agate synchro */
{
	pp->p_config->synchro = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_203 (GtkSpinButton *spinbutton, gpointer data) /*agate width */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->agate_width =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}
void data_2031 (GtkMenuItem *menuitem, gpointer data) /* agate measure */
{
	pp->p_config->measure = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_204 (GtkSpinButton *spinbutton, gpointer data) /*agate height */
{
	//DRAW_UI_P p = (DRAW_UI_P)(data);
	pp->p_config->agate_height =  (guint) (gtk_spin_button_get_value (spinbutton));

	/*发送增益给硬件*/
}
void data_2041 (GtkMenuItem *menuitem, gpointer data) /* agate rf */
{
	pp->p_config->rf = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_210 (GtkMenuItem *menuitem, gpointer data) /* Alarm */
{
	pp->p_config->alarm = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}

void data_211 (GtkMenuItem *menuitem, gpointer data) /* Group A */
{
	pp->p_config->groupA = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}
void data_212 (GtkMenuItem *menuitem, gpointer data) /* Condition A */
{
	pp->p_config->conditionA = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
	/* 发送增益给硬件 */
}
void data_213 (GtkMenuItem *menuitem, gpointer data) /* operator */
{
	pp->p_config->oprt = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_214 (GtkMenuItem *menuitem, gpointer data) /* Group B */
{
	pp->p_config->groupB = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_215 (GtkMenuItem *menuitem, gpointer data) /* condition B */
{
	pp->p_config->conditionB = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}
void data_220 (GtkMenuItem *menuitem, gpointer data) /* Output */
{
	pp->p_config->output = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_221 (GtkMenuItem *menuitem, gpointer data) /* Output -> alarm # */
{
	pp->p_config->alarm1 = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_222 (GtkSpinButton *spinbutton, gpointer data) /*count */
{
	pp->p_config->count =  (guint) (gtk_spin_button_get_value (spinbutton));
}
void data_223 (GtkMenuItem *menuitem, gpointer data) /* sound */
{
	pp->p_config->sound = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_224 (GtkSpinButton *spinbutton, gpointer data) /*active_delay */
{
	pp->p_config->active_delay =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_225 (GtkSpinButton *spinbutton, gpointer data) /*holdtime */
{
	pp->p_config->holdtime =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_230 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Mode 230 */
{
	pp->p_config->mode = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
}

void data_231 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm -> Sizing Curves -> Curve 231 */
{
	pp->p_config->curve = (gchar) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_3_menu(0, NULL);
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
	pp->p_config->gain =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);
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



void send_dsp_data (guint data_type, guint value)
{
	switch (data_type)	
	{
		case BEAM_DELAY_DSP:break;
		default:break;
	}
}
