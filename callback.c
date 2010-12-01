
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include <gdk/gdkkeysyms.h>

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

void button2_function0 (GtkButton *button, gpointer data);
void button2_function1 (GtkButton *button, gpointer data);
void button2_function2 (GtkButton *button, gpointer data);
void button2_function3 (GtkButton *button, gpointer data);
void button2_function4 (GtkButton *button, gpointer data);

gboolean button2_function00 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data);
gboolean button2_function01 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data);
gboolean button2_function02 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data);
gboolean button2_function03 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data);
gboolean button2_function04 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data);

void button3_function0 (GtkButton *button, gpointer data);
void button3_function1 (GtkButton *button, gpointer data);
void button3_function2 (GtkButton *button, gpointer data);
void button3_function3 (GtkButton *button, gpointer data);
void button3_function4 (GtkButton *button, gpointer data);
void button3_function5 (GtkButton *button, gpointer data);

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

/*二级菜单5个按钮的回调函数*/
/*button click 回调函数*/
void (*button2_fun[5])(GtkButton *button, gpointer data) = 
{
	button2_function0,	button2_function1,	button2_function2,	button2_function3,	button2_function4
};

/*button focus-in-event 回调函数*/
gboolean (*button20_fun[5])(GtkWidget *widget, GdkEventFocus *event,	gpointer data) =
{
	button2_function00,	button2_function01,	button2_function02,	button2_function03,	button2_function04
};

/*三级菜单6个按钮的回调函数*/
/*button click 回调函数*/
void (*button3_fun[6])(GtkButton *button, gpointer data) = 
{
	button3_function0,	button3_function1,	button3_function2,	
	button3_function3,	button3_function4,	button3_function5
};

/*data button press 回调函数*/
gboolean (*data_fun[6])(GtkWidget *widget, GdkEventButton *event, gpointer data) = 
{
	data_function0,	data_function1,	data_function2,	
	data_function3,	data_function4,	data_function5
};


/*5个二级菜单按钮的回调函数*/
void b2_fun0(DRAW_UI_P p, gint pos)
{
	p->pos1[p->pos] = pos;
	draw_2_menu(p);
	draw_3_menu(p);
}

void button2_function0 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 0);
	return ;
}

gboolean button2_function00 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 0);
	return TRUE;
}

void button2_function1 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 1);
	return ;
}

gboolean button2_function01 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 1);
	return TRUE;
}

void button2_function2 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 2);
	return ;
}

gboolean button2_function02 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 2);
	return TRUE;
}

void button2_function3 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 3);
	return ;
}

gboolean button2_function03 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 3);
	return TRUE;
}

void button2_function4 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 4);
	return ;
}

gboolean button2_function04 (GtkWidget *widget,	GdkEventFocus *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b2_fun0(p, 4);
	return TRUE;
}

/*6个数值区域共有的处理函数*/
void b3_fun0(DRAW_UI_P p)
{
	/*处理微调*/
	if (p->pos2[p->pos][p->pos1[p->pos]] == 0)
		switch (p->pos) 
		{
			case 0:
				break;
			case 1:
				switch (p->pos1[p->pos])
				{
					case 0:
						(p->p_tmp_config->db_reg > 0) ? p->p_tmp_config->db_reg-- : (p->p_tmp_config->db_reg = 4);

						break;
					case 1:
						(p->p_tmp_config->pulser_reg > 0) ? p->p_tmp_config->pulser_reg-- : (p->p_tmp_config->pulser_reg = 3);
						break;
					case 3:
						(p->p_tmp_config->scanoffset_reg > 0) ? p->p_tmp_config->scanoffset_reg-- : (p->p_tmp_config->scanoffset_reg = 2);
						break;
					default:break;
				}
				break;
			default:break;

		}

	p->pos2[p->pos][p->pos1[p->pos]] = 0;
	draw_3_menu(p);
#if 0
	if (gtk_widget_is_focus(p->data[0])) {
		g_object_set (( (p->data[0])), "editable", TRUE, NULL); 
	return ;
	}
#endif
	gtk_widget_set_can_focus (p->data[0], TRUE);
	if ( p->data[p->pos2[p->pos][p->pos1[p->pos]]] &&
			gtk_widget_get_can_focus( p->data[p->pos2[p->pos][p->pos1[p->pos]]]) ) ;
		g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],			
				"is-focus", TRUE,	NULL); 
	gtk_widget_set_can_focus (p->data[0], FALSE);
}

void b3_fun1(DRAW_UI_P p)
{
	/*处理微调*/
	if (p->pos2[p->pos][p->pos1[p->pos]] == 1)
		switch (p->pos) 
		{
			case 0:
				break;
			case 1:
				switch (p->pos1[p->pos])
				{
					case 0:
						(p->p_tmp_config->start_reg > 0) ? p->p_tmp_config->start_reg-- : (p->p_tmp_config->start_reg = 2);
					
						break;
					case 3:
						(p->p_tmp_config->indexoffset_reg > 0) ? p->p_tmp_config->indexoffset_reg-- : (p->p_tmp_config->indexoffset_reg = 2);
						break;
					default:break;
				}
				break;
			default:break;

		}
	p->pos2[p->pos][p->pos1[p->pos]] = 1;
	draw_3_menu(p);
	gtk_widget_set_can_focus (p->data[1], TRUE);
	if ( p->data[p->pos2[p->pos][p->pos1[p->pos]]] &&
			gtk_widget_get_can_focus( p->data[p->pos2[p->pos][p->pos1[p->pos]]]) )  
		g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],			
				"is-focus", TRUE,	NULL); 
	gtk_widget_set_can_focus (p->data[1], FALSE);
}

void b3_fun2(DRAW_UI_P p)
{
	/*处理微调*/
	if (p->pos2[p->pos][p->pos1[p->pos]] == 2)
		switch (p->pos) 
		{
			case 0:
				break;
			case 1:
				switch (p->pos1[p->pos])
				{
					case 0:
						(p->p_tmp_config->range_reg > 0) ? p->p_tmp_config->range_reg-- : (p->p_tmp_config->range_reg = 2);
						break;
					case 3:
						break;
					default:break;
				}
				break;
			default:break;

		}
	p->pos2[p->pos][p->pos1[p->pos]] = 2;
	draw_3_menu(p);
	gtk_widget_set_can_focus (p->data[2], TRUE);
	if ( p->data[p->pos2[p->pos][p->pos1[p->pos]]] &&
			gtk_widget_get_can_focus( p->data[p->pos2[p->pos][p->pos1[p->pos]]]) )  
		g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],			
				"is-focus", TRUE,	NULL); 
	gtk_widget_set_can_focus (p->data[2], FALSE);
}

void b3_fun3(DRAW_UI_P p)
{
	/*处理微调*/
	if (p->pos2[p->pos][p->pos1[p->pos]] == 3)
		switch (p->pos) 
		{
			case 0:
				break;
			case 1:
				switch (p->pos1[p->pos])
				{
					case 0:
						(p->p_tmp_config->wedge_delay_reg > 0) ? p->p_tmp_config->wedge_delay_reg-- : (p->p_tmp_config->wedge_delay_reg = 2);

						break;
					case 2:
						p->p_config->video_filter = !p->p_config->video_filter;
					default:break;
				}
				break;
			default:break;

		}
	p->pos2[p->pos][p->pos1[p->pos]] = 3;
	draw_3_menu(p);
	gtk_widget_set_can_focus (p->data[3], TRUE);
	if ( p->data[p->pos2[p->pos][p->pos1[p->pos]]] &&
			gtk_widget_get_can_focus( p->data[p->pos2[p->pos][p->pos1[p->pos]]]) )  
		g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],			
				"is-focus", TRUE,	NULL); 
	gtk_widget_set_can_focus (p->data[3], FALSE);
}

void b3_fun4(DRAW_UI_P p)
{
	/*处理微调*/
	if (p->pos2[p->pos][p->pos1[p->pos]] == 4)
		switch (p->pos) 
		{
			case 0:
				break;
			case 1:
				switch (p->pos1[p->pos])
				{
					case 0:
						(p->p_tmp_config->velocity_reg > 0) ? p->p_tmp_config->velocity_reg-- : (p->p_tmp_config->velocity_reg = 3);
						break;
					case 3:
						(p->p_tmp_config->beam_delay_reg > 0) ? p->p_tmp_config->beam_delay_reg-- : (p->p_tmp_config->beam_delay_reg = 2);
						break;
					default:break;
				}
				break;
			default:break;

		}
	p->pos2[p->pos][p->pos1[p->pos]] = 4;
	draw_3_menu(p);
	gtk_widget_set_can_focus (p->data[4], TRUE);
	if ( p->data[p->pos2[p->pos][p->pos1[p->pos]]] &&
			gtk_widget_get_can_focus( p->data[p->pos2[p->pos][p->pos1[p->pos]]]) )  
		g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],			
				"is-focus", TRUE,	NULL); 
	gtk_widget_set_can_focus (p->data[4], FALSE);
}

void b3_fun5(DRAW_UI_P p)
{
	/*处理微调*/
	if (p->pos2[p->pos][p->pos1[p->pos]] == 5)
		switch (p->pos) 
		{
			case 0:
				break;
			case 1:
				switch (p->pos1[p->pos])
				{
					case 0:
						break;
					case 2:
						(p->p_tmp_config->reject_reg > 0) ? p->p_tmp_config->reject_reg-- : (p->p_tmp_config->reject_reg = 2);
						break;
					case 3:
						(p->p_tmp_config->gainoffset_reg > 0) ? p->p_tmp_config->gainoffset_reg-- : (p->p_tmp_config->gainoffset_reg = 2);
						break;
					default:break;
				}
				break;
			default:break;

		}
	p->pos2[p->pos][p->pos1[p->pos]] = 5;
	draw_3_menu(p);
	gtk_widget_set_can_focus (p->data[5], TRUE);
	if ( p->data[p->pos2[p->pos][p->pos1[p->pos]]] &&
			gtk_widget_get_can_focus( p->data[p->pos2[p->pos][p->pos1[p->pos]]]) )  
		g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],			
				"is-focus", TRUE,	NULL); 
	gtk_widget_set_can_focus (p->data[5], FALSE);
}

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data)
{
//	DRAW_UI_P p = (DRAW_UI_P)(data);

	g_print("%d  %c\n", keyval);

	return 0;
}

void button3_function0 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun0(p);
	return ;
}

gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun0(p);
	return TRUE;
}

void button3_function1 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun1(p);
	return ;
}

gboolean data_function1 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun1(p);
	return TRUE;
}

void button3_function2 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun2(p);
	return ;
}

gboolean data_function2 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun2(p);
	return TRUE;
}

void button3_function3 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun3(p);
	return ;
}

gboolean data_function3 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun3(p);
	return TRUE;
}

void button3_function4 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun4(p);
	return ;
}

gboolean data_function4 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun4(p);
	return TRUE;
}

void button3_function5 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun5(p);
	return ;
}

gboolean data_function5 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	b3_fun5(p);
	return TRUE;
}

void data_100 (GtkSpinButton *spinbutton, gpointer data) /*增益Gain*/
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->p_config->gain = (gshort) (gtk_spin_button_get_value (spinbutton) * 10.0);

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


