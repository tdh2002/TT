
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include <gdk/gdkkeysyms.h>

static void handler_key(guint keyval);

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

gboolean key_press_handler (GtkWidget* pWidget,
		GdkEventKey* pEvent, gpointer data);

gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function1 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function2 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function3 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function4 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
gboolean data_function5 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

void data_100 (GtkSpinButton *spinbutton, gpointer data);
void data_101 (GtkSpinButton *spinbutton, gpointer data);
void data_102 (GtkSpinButton *spinbutton, gpointer data);
void data_103 (GtkSpinButton *spinbutton, gpointer data);
void data_104 (GtkSpinButton *spinbutton, gpointer data);

void data_110 (GtkSpinButton *spinbutton, gpointer data);
void data_125 (GtkSpinButton *spinbutton, gpointer data);

void data_130 (GtkSpinButton *spinbutton, gpointer data);
void data_131 (GtkSpinButton *spinbutton, gpointer data);
void data_132 (GtkSpinButton *spinbutton, gpointer data);
void data_134 (GtkSpinButton *spinbutton, gpointer data);
void data_135 (GtkSpinButton *spinbutton, gpointer data);

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

/*5个二级菜单按钮的回调函数*/
void b2_fun0(DRAW_UI_P p, gint pos)
{
	p->pos_last1 = p->pos1[p->pos];
	p->pos1[p->pos] = pos;
	pp->pos_pos = MENU3_STOP;
	draw_2_menu(0);
	draw_3_menu(0);
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
					   case 0: data_process (&(pp->p_tmp_config->db_reg), 4);			break; /* 100增益 5种步进 */
					   case 1: data_process (&(pp->p_tmp_config->pulser_reg), 2);		break; /* 110pulser发射 3种步进 */
					   case 2: data_process (&(pp->p_tmp_config->receiver_reg), 2);		break; /* 120receiver接收 3种步进 */
					   case 3: break; /* 130scan offset 这里只有显示数值 不能更改 */
					   case 4: /* 自动80% */	break; /* 140 自动80%  */
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
			case 3:
				   break;
			case 4:
				   break;
			case 5:
				   switch (pp->pos1[5])
				   {
					   case 0: break; /*   */
					   case 1: data_process (&(pp->p_tmp_config->scanoffset_reg), 2);  break; /*510 */
					   case 2: /* 弹出一个菜单选择  */			break; /*   */
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
					   case 3: break; 
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
			default:break;
		}
	}

	pp->pos_pos = MENU3_PRESSED;
	draw_2_menu(0);
	draw_3_menu(0);                          /**/

	return ;
}

void b3_fun1(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 1;
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
					   case 2: /* 弹出一个选择菜单,选择 */ break; 
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
					   case 3: data_process(&(pp->p_tmp_config->color_start_reg), 1); break;  /*431 */
					   case 4: break; 
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
					   case 0: break; /*   */
					   case 1: data_process (&(pp->p_tmp_config->first_element_reg), 2);  break; /*611 */
					   case 2: data_process (&(pp->p_tmp_config->max_angle_reg), 2);  break; /*621 */
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
					   case 3: break; 
					   default:break;
				   }
				   break;
			case 9:
				   switch (pp->pos1[9])
				   {
					   case 0: data_process (&(pp->p_tmp_config->bright_reg), 2);  break; /*901  */
					   case 1: break; /*710 */
					   case 2: break;   /*721  */
					   case 3: break; 
					   default:break;
				   }
				   break;
			default:break;
		}
	}

	draw_2_menu(0);
	draw_3_menu(0);                          /**/

	return ;
}

void b3_fun2(gpointer p)
{
	/* 之前的位置 */
	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 2;
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
					   case 0:data_process(&(pp->p_tmp_config->range_reg), 2); break; /* 102range范围 3种步进 */
					   case 1: /* 弹出一个选择菜单,选择 */ break; /* 112 频率 Freq.  */
					   case 2: /* 弹出一个选择菜单,选择 */ break; /* 122 检波 Recitify  */
					   case 3: /* Angle. (deg) */ break; /* 132 角度 不能更改 */
					   case 4: /* dB Ref. 开关 */ break; /* 142 dB Ref, on or off  */
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
					   case 0: break; /* 202 agate start*/
					   case 1: data_process(&(pp->p_tmp_config->cursors_scan_reg), 3); break; 
					   case 2: break; /* 222 output count*/
					   case 3: data_process(&(pp->p_tmp_config->max_thickness_reg), 3); break; /*332 */
					   case 4: break;
					   default:break;
				   }
                                   break;
			case 4:
				   switch (pp->pos1[4])
				   {
					   case 0: break; 
					   case 1: break; 
					   case 2: break; 
					   case 3: data_process(&(pp->p_tmp_config->color_end_reg), 1); break; /*432 */
					   case 4: break;
					   default:break;
				   }
                                   break;
			case 5:
				   switch (pp->pos1[5])
				   {
					   case 0: break; 
					   case 1: break; 
					   case 2: data_process(&(pp->p_tmp_config->agate_start_reg), 3); break;  /*522*/
					   case 3: data_process(&(pp->p_tmp_config->diameter_reg), 3); break;  /*532*/
					   case 4: break;
					   default:break;
				   }
                                   break;
			case 6:
				   switch (pp->pos1[6])
				   {
					   case 0: break; 
					   case 1: break; 
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

			default:break;
		}
	}

	draw_2_menu(0);
	draw_3_menu(0);                          /**/

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
					   case 0:data_process(&(pp->p_tmp_config->wedge_delay_reg), 2); break; /* 103wedge delay 楔款延时 3种步进 */
					   case 1: /* 弹出一个选择菜单,选择 */ break; /* 113 电压功率 Voltage  */
					   case 2: /* 视频滤波 */ break; /* 123 视频滤波 on or off  */
					   case 3: /* Skew  */ break; /* 133 Skew (deg)  */
					   case 4: /* 弹出一个选择菜单,选择 */ break; /* 143 Points Qty. */
					   default:break;
				   }
                                   break;
			case 2:
				   switch (pp->pos1[2])
				   {
					   case 0:data_process(&(pp->p_tmp_config->agate_width_reg), 3); break; /* 203 agate width 3种步进 */
					   case 1: /* 弹出一个选择菜单,选择 */ break; /* 113 电压功率 Voltage  */
					   case 2: /* 视频滤波 */ break; /* 123 视频滤波 on or off  */
					   case 3: /* Skew  */ break; /* 133 Skew (deg)  */
					   case 4: /* 弹出一个选择菜单,选择 */ break; /* 143 Points Qty. */
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
			case 4:break;
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
					   case 1: break;
					   case 2: data_process(&(pp->p_tmp_config->focus_depth_reg), 2);  break; 
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
					   case 2: break; 
					   case 3: break;  
					   case 4: break; 
					   default:break;
				   }
                                   break;

			default:break;
		}
	}

	draw_2_menu(0);
	draw_3_menu(0);                          /**/

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
					   case 1: /* 弹出一个选择菜单,选择 */ break; /* 114 脉冲宽度 PW  */
					   case 2: /* 弹出一个选择菜单,选择 */ break; /* 124 平均 averaging  */
					   case 3:data_process(&(pp->p_tmp_config->beam_delay_reg), 2); break; /* 134 beamdelay 波束延时 */
					   case 4: /*  */ break; /* 144 Scale Factor 多少点压缩一点 只能看  */
					   default:break;
				   }
			case 2:
				   switch (pp->pos1[2])
				   {
					   case 0:data_process(&(pp->p_tmp_config->agate_height_reg), 1); break; /* 204 threshold  2种步进 */
					   case 1: /* 弹出一个选择菜单,选择 */ break; /* 114 脉冲宽度 PW  */
					   case 2: data_process(&(pp->p_tmp_config->active_delay_reg), 3); break; /*224 Delay*/
					   case 3:  break; /* 134 beamdelay 波束延时 */
					   case 4: /*  */ break; /* 144 Scale Factor 多少点压缩一点 只能看  */
					   default:break;
				   }
			case 3:
                              break;
				   
			default:break;
		}
	}

	draw_2_menu(0);
	draw_3_menu(0);                          /**/

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
					   case 1: /* 弹出一个选择菜单,选择 */ break; /* 115 重复频率 PRF */
					   case 2:data_process(&(pp->p_tmp_config->reject_reg), 2); break; /* 125 Reject 抑制  */
					   case 3:data_process(&(pp->p_tmp_config->gainoffset_reg), 4); break; /* 135 Gain Offset  */
					   case 4: /* 弹出一个选择菜单,选择 */ break; /* 145 Sum Gain  */
					   default:break;
				   }
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
			default:break;
		}
	}

	draw_2_menu(0);
	draw_3_menu(0);                          /**/

	return ;
}

static void handler_key(guint keyval)
{
		guchar tmp = pp->pos_pos;
		g_print("%x  \n", keyval);
	switch (keyval) 
	{
		case GDK_Escape:
			switch (pp->pos_pos)
			{
				case MENU2_STOP:
					g_print("menu2stop\n");
					break;
				case MENU2_PRESSED:
					pp->pos_pos = MENU2_STOP;
					g_print("menu2pressed\n");
					break;
				case MENU3_STOP:
					pp->pos_pos = MENU2_STOP;
					g_print("menu3stop\n");
					break;
				case MENU3_PRESSED:
					pp->pos_pos = MENU3_STOP;
					g_print("menu3pressed\n");
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
			{
				b3_fun0(NULL);
			}
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
					draw_3_menu(1);
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
					draw_3_menu(1);
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
			draw_3_menu(0);
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

void data_100 (GtkSpinButton *spinbutton, gpointer data) /*增益Gain*/
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gchar *str;
	p->p_config->gain = gtk_spin_button_get_value (spinbutton);

	str = g_strdup_printf ("%0.1f", pp->p_config->gain) ;
	gtk_label_set_text (GTK_LABEL (pp->label[1]), str);
	g_free(str);
	/*发送增益给硬件*/
}

void data_101 (GtkSpinButton *spinbutton, gpointer data) /*Start 扫描延时 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	if (p->p_config->ut_unit == 0) 
		p->p_config->start = (gint) (gtk_spin_button_get_value (spinbutton) * 100.0 / (p->p_config->part.Velocity / 20000.0));
	else
		p->p_config->start = (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_102 (GtkSpinButton *spinbutton, gpointer data) /*Range 范围 */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	if (p->p_config->ut_unit == 0) 
		p->p_config->range = (guint) (gtk_spin_button_get_value (spinbutton) * 100.0 / (p->p_config->part.Velocity / 20000.0));
	else
		p->p_config->range = (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_103 (GtkSpinButton *spinbutton, gpointer data) /*楔块延时  Wedge Delay */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->wedge_delay = (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_104 (GtkSpinButton *spinbutton, gpointer data) /*声速 Velocity */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->part.Velocity = (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);

	/*发送增益给硬件*/
}

void data_110 (GtkSpinButton *spinbutton, gpointer data) /*声速 Velocity */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->pulser =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送增益给硬件*/
}

void data_125 (GtkSpinButton *spinbutton, gpointer data) /*抑制 Reject */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->reject =  (guchar) (gtk_spin_button_get_value (spinbutton));

	/*发送增益给硬件*/
}

void data_130 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->scan_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_131 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->index_offset =  (gint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_132 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->angle =  (gint) (gtk_spin_button_get_value (spinbutton) * 10.0);

	/*发送增益给硬件*/
}

void data_134 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->beam_delay =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送增益给硬件*/
}

void data_135 (GtkSpinButton *spinbutton, gpointer data) /*gain offset */
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->gain_offset =  (guint) (gtk_spin_button_get_value (spinbutton) * 10.0);

	/*发送增益给硬件*/
}


