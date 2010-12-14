
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>

gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);
GtkWidgetClass *widget_window_class;

GdkPoint a[512] = {{0,0},{240,200},{400,200}};

void change_language();
void draw_1_menu(DRAW_UI_P p);
void draw_2_menu(gint pa);
void draw_3_menu(gint pa, gpointer p);
void init_ui(DRAW_UI_P p);				/*初始化界面,*/

void draw3_pressed1(gfloat step, guint digit);
/**/
const gchar **con0_p	= content_en10;
const gchar ***con1_p	= content1_en;
const gchar ****con2_p	= content2_en;

void menuitem0_function(GtkMenuItem *menuitem, gpointer data);
void menuitem1_function(GtkMenuItem *menuitem, gpointer data);
void menuitem2_function(GtkMenuItem *menuitem, gpointer data);
void menuitem3_function(GtkMenuItem *menuitem, gpointer data);
void menuitem4_function(GtkMenuItem *menuitem, gpointer data);
void menuitem5_function(GtkMenuItem *menuitem, gpointer data);
void menuitem6_function(GtkMenuItem *menuitem, gpointer data);
void menuitem7_function(GtkMenuItem *menuitem, gpointer data);
void menuitem8_function(GtkMenuItem *menuitem, gpointer data);
void menuitem9_function(GtkMenuItem *menuitem, gpointer data);

gint (*entry_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);

gint my_keypress_event(GtkWidget *widget, GdkEventKey *event)
{
	gpointer data;
	key_press_handler (widget, event, data);
	return 0;
}


static gint gtk_entry_digit_only_keypress_event(GtkWidget *widget, GdkEventKey *event)
{
	switch (event->keyval)
	{
		case GDK_0:
		case GDK_1:
		case GDK_2:
		case GDK_3:
		case GDK_4:
		case GDK_5:
		case GDK_6:
		case GDK_7:
		case GDK_8:
		case GDK_9: 
		case GDK_KP_0:
		case GDK_KP_1:
		case GDK_KP_2:
		case GDK_KP_3:
		case GDK_KP_4:
		case GDK_KP_5:
		case GDK_KP_6:
		case GDK_KP_7:
		case GDK_KP_8:
		case GDK_KP_9:
		case GDK_period:
		case GDK_Delete:
		case GDK_BackSpace:
		case GDK_Up: 
		case GDK_Down:
		case GDK_Left:
		case GDK_Right:
			return entry_keypress_event_orig(widget, event); 
			break;
		default:
			break;
	} 

	return FALSE;
}

void gtk_entry_digit_only(GtkWidget *entry)
{ 
	GtkWidgetClass *widget_class;

	//	widget_class = GTK_WIDGET_CLASS (((GObject*)(entry))->klass); 
	widget_class = GTK_WIDGET_GET_CLASS (((GtkObject*)(entry))); 
	// 取代原來的處理函式
	entry_keypress_event_orig=widget_class->key_press_event; 
	widget_class->key_press_event=gtk_entry_digit_only_keypress_event;
} 

void (*menu_fun[10])(GtkMenuItem *menuitem, gpointer data) = 
{
	menuitem0_function,	menuitem1_function,
	menuitem2_function,	menuitem3_function,
	menuitem4_function,	menuitem5_function,
	menuitem6_function,	menuitem7_function,
	menuitem8_function,	menuitem9_function
};

/*1级菜单 回调函数*/
void menuitem0_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 0;
	pp->menu2_qty = 3;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem1_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 1;
	pp->menu2_qty = 5;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem2_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 2;
	pp->menu2_qty = 4;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem3_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 3;
	pp->menu2_qty = 5;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem4_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 4;
	pp->menu2_qty = 5;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem5_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 5;
	pp->menu2_qty = 4;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem6_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 6;
	pp->menu2_qty = 4;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d \n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem7_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 7;
	pp->menu2_qty = 5;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem8_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 8;
	pp->menu2_qty = 5;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

void menuitem9_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	pp->pos_last = pp->pos;
	p->pos = 9;
	pp->menu2_qty = 5;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

/**/
void change_language()
{
	return ;
}

/* 画一级菜单*/
void draw_1_menu(DRAW_UI_P p)
{
	gint i;

	for (i = 0; i < 10; i++)
		gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem[i]),con0_p[i]);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), con0_p[p->pos]);
}

/* 二级菜单 */
void draw_2_menu(gint pa)
{
	gint i;

	for ( i = 0 ; i < 5 ; i++ )   /* 6个二级菜单 */
	{
		/* 
		 * 情况1 1级菜单改变了,所有2级菜单更新 
		 * 情况2 1级菜单没改变,只更新改变了的2级菜单
		 * 情况3 pa 不为0强制刷新
		 * */
		if ( (pp->pos_last != pp->pos) || 
				(( pp->pos_last1 == i ) || ( pp->pos1[pp->pos] == i )) || pa )
		{
			/*
			 * 如果二级菜单存在,就显示出来
			 * 不存在就隐藏
			 * */
			if ( con1_p[pp->pos][i] )
			{
				gtk_label_set_text (GTK_LABEL (pp->label2[i]), con1_p[pp->pos][i]);
				gtk_widget_modify_bg (pp->eventbox2[i], GTK_STATE_NORMAL, &color_button1);
				gtk_widget_show (pp->eventbox2[i]);
			}
			else
			{
				gtk_label_set_text (GTK_LABEL (pp->label2[i]), " ");
				gtk_widget_hide (pp->eventbox2[i]);
			}
		}
	}
	/* 当前二级菜单不是停留就是按下 */
	if (pp->pos_pos == 0)
		gtk_widget_modify_bg (pp->eventbox2[pp->pos1[pp->pos]], GTK_STATE_NORMAL, &color_button2);
	else 
		gtk_widget_modify_bg (pp->eventbox2[pp->pos1[pp->pos]], GTK_STATE_NORMAL, &color_button0);

	return ;
}


/*
 * 处理 三级菜单按下状态的画图  
 * fun   为回调函数 
 * cur_value 为菜单当前数值
 * lower 为菜单最小值
 * upper 为菜单最大值
 * step  为菜单步进
 * digit 为保留小数点数
 */

static void draw3_pressed(void (*fun)(GtkSpinButton*, gpointer),const gchar *unit, 
		gfloat cur_value, gfloat lower, gfloat upper, gfloat step, guint digit, gpointer p)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;
	GtkAdjustment *adj;
	guint temp = GPOINTER_TO_UINT (p);

	x = pp->pos;
	y = pp->pos1[x];
	z = pp->pos2[x][y];
	gtk_widget_destroy (pp->vscale);


	switch (digit)
	{
		case 0:
			str = g_strdup_printf ("%s\n(%s) Δ%0.0f", con2_p[x][y][z], unit, step);			break;
		case 1:
			str = g_strdup_printf ("%s\n(%s) Δ%0.1f", con2_p[x][y][z], unit, step);			break;
		case 2:
			str = g_strdup_printf ("%s\n(%s) Δ%0.2f", con2_p[x][y][z], unit, step);			break;
		case 3:
			str = g_strdup_printf ("%s\n(%s) Δ%0.3f", con2_p[x][y][z], unit, step);			break;
		default:break;
	}
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	gtk_widget_modify_bg (pp->eventbox30[z], GTK_STATE_NORMAL, &color_button0);
	widget_window_class->key_press_event = window_keypress_event_orig;
	g_signal_connect (G_OBJECT(pp->sbutton[z]), "value-changed", G_CALLBACK(fun), (gpointer) (pp));

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_hide (pp->eventbox31[z]);
	gtk_widget_show (pp->sbutton[z]);
	gtk_widget_grab_focus (pp->sbutton[z]);
	/* 设置值的范围 */
	adj = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (pp->sbutton[z]));
	gtk_adjustment_configure (adj, cur_value, lower, upper, step , 10.0, 0.0);
	gtk_spin_button_set_digits (GTK_SPIN_BUTTON (pp->sbutton[z]), digit);

	pp->vscale = gtk_vscale_new(adj);
	gtk_widget_set_size_request (GTK_WIDGET(pp->vscale), 30, 460);
	gtk_scale_set_draw_value (GTK_SCALE (pp->vscale), FALSE);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->vscale, TRUE, TRUE, 0);
	gtk_widget_show (pp->vscale);

	if (str)
		g_free(str);
}

void draw3_pressed1(gfloat step, guint digit)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pp->pos2[x][y];

	switch (digit)
	{
		case 0:
			str = g_strdup_printf ("%s\n(dB) Δ%0.0f", con2_p[x][y][z], step);			break;
		case 1:
			str = g_strdup_printf ("%s\n(dB) Δ%0.1f", con2_p[x][y][z], step);			break;
		case 2:
			str = g_strdup_printf ("%s\n(dB) Δ%0.2f", con2_p[x][y][z], step);			break;
		case 3:
			str = g_strdup_printf ("%s\n(dB) Δ%0.3f", con2_p[x][y][z], step);			break;
		default:break;
	}
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);

	if (str)
		g_free(str);
}

static void draw3_stop(gfloat cur_value, const gchar *unit,  guint digit)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pp->pos2[x][y];

	str = g_strdup_printf ("%s\n(%s)", con2_p[x][y][z], unit);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	if ((CUR_POS) == 0 && (pp->pos_pos == MENU3_STOP))
	{
		gtk_widget_modify_bg (pp->eventbox30[z], GTK_STATE_NORMAL, &color_button2);
		gtk_widget_modify_bg (pp->eventbox31[z], GTK_STATE_NORMAL, &color_button2);
	}
	else
	{
		gtk_widget_modify_bg (pp->eventbox30[z], GTK_STATE_NORMAL, &color_button1);
		gtk_widget_modify_bg (pp->eventbox31[z], GTK_STATE_NORMAL, &color_button1);
	}
	/* 更新当前增益值显示 */
	switch (digit)
	{
		case 0:
			str = g_strdup_printf ("%0.0f", cur_value);			break;
		case 1:
			str = g_strdup_printf ("%0.1f", cur_value);			break;
		case 2:
			str = g_strdup_printf ("%0.2f", cur_value);			break;
		case 3:
			str = g_strdup_printf ("%0.3f", cur_value);			break;
		default:break;
	}
	gtk_label_set_text (GTK_LABEL (pp->data3[z]), str);
/*	gtk_label_set_text (GTK_LABEL (pp->label[1]), str);*/
	if (str)
		g_free(str);
	widget_window_class->key_press_event = my_keypress_event;
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	gtk_widget_hide (pp->vscale);
	/*						gtk_widget_grab_focus (pp->button);*/
}

static void draw3_none()
{
}

/* 三级菜单第一个 */
void draw3_data0(gpointer p) 
{
	gchar temp[52];
	gchar *str;
	gfloat tmpf;/**/


	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> Back*/
					g_sprintf (temp,"%s", con2_p[0][0][0]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Group Wizard");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*Wizard -> Focal Law -> Back*/
					g_sprintf (temp,"%s", con2_p[0][1][0]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Law Wizard");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*Wizard -> Calibration -> Back*/
					g_sprintf (temp,"%s", con2_p[0][2][0]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Select Calibration");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* 增益 Gain 100 */
					/* 当前步进 */
					switch (pp->p_tmp_config->db_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pressed (data_100, "dB", pp->p_config->gain ,	0.0, 74.0, tmpf, 1, p);
					else 
						draw3_stop (pp->p_config->gain, "dB", 1);
					break;
				case 1: /*发射 Pulser*/
					/* 当前步进 */
					switch (pp->p_tmp_config->pulser_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						g_sprintf (temp,"%s\n(1 to 1) Δ%d", con2_p[1][1][0], (gint) (tmpf));
/*						gtk_window_set_keep_above( GTK_WINDOW (pp->window1), TRUE);
						gtk_window_get_position (GTK_WINDOW (pp->window), &x, &y);
						gtk_window_move(GTK_WINDOW (pp->window1), x + 685, y + 137);
						gtk_widget_show_all(pp->window1);
						*/
					}
					else 
						g_sprintf (temp,"%s\n(1 to 1)", con2_p[1][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->pulser / 10.0));
					g_free(str);
					/*当前步进*/
					break;
				case 2: /* Reveiver 接收器  */
					/* 当前步进 */
					switch (pp->p_tmp_config->receiver_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0) )
						g_sprintf (temp,"%s\n(1 to 1) Δ%d", con2_p[1][2][0], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n(1 to 1)", con2_p[1][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->receiver / 10.0));
					g_free(str);
					break;
				case 3:/*Scan Offset*/
					/* 当前步进 */
					switch (pp->p_tmp_config->scanoffset_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0) )
						g_sprintf (temp,"%s\n(mm) Δ%d", con2_p[1][3][0], (guint) (tmpf));
					else 
						g_sprintf (temp,"%s\n  (mm)", con2_p[1][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->scan_offset / 10.0));
					g_free(str);
					break;
				case 4:/*Set 80%*/
					g_sprintf (temp,"%s", con2_p[1][4][0]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "");

					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);

					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/*Gate*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "A");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                       break;
				case 1:/*Alarm*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "1");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*Output*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Output 1");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*Sizing Curves -> Setup*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Setup");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> List*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "1");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*Measurements -> Cursors -> Selection*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "A-scan");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*Measurements -> Table -> Display Table*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*Measurements -> Thickness -> Source*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "A^");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:/*Measurements -> Export -> Export Table*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), " ");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/*Display -> Selection -> Display*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "A");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*Display -> Overlay -> UT Unit*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Sound path");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*Display -> Zoom -> Display*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*Display -> Color -> Select*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Amplitude");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:/*Display -> Properties -> Scan*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "A-Scan");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> group*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "1");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*probe/part -> position -> Scan Offset*/
					/* 当前步进 */
					switch (pp->p_tmp_config->scanoffset_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0) )
						g_sprintf (temp,"%s\n  (mm)   Δ%.1f", con2_p[5][1][0], tmpf);
					else 
						g_sprintf (temp,"%s\n      (mm)", con2_p[5][1][0]);
 
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "0.00");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
					
                                        break;
				case 2:/*Probe/Part -> characterize -> FFT*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "off");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*Probe/Part -> Parts -> Geometry*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Plate");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:/*Focal Law -> configuration -> Law Config.*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[6][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Linear");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*Focal Law -> aperture -> element qty.*/
					/* 当前步进 */
					switch (pp->p_tmp_config->element_qty_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0) )
						g_sprintf (temp,"%s\n           Δ%d", con2_p[6][1][0], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s", con2_p[6][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "1");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*Focal Law -> beam -> Min.Angle*/
					/* 当前步进 */
					switch (pp->p_tmp_config->min_angle_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0) )
						g_sprintf (temp,"%s\n   (deg)  Δ%.1f", con2_p[6][2][0], tmpf);
					else 
						g_sprintf (temp,"%s\n      (deg)", con2_p[6][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "1.0");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*Focal Law -> laws -> auto program*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[6][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> Encoder*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "1");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*Scan -> Inspection -> type*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "One-line Scan");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*Scan -> Area -> scan start*/
					/* 当前步进 */
					switch (pp->p_tmp_config->scan_start_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0) )
						g_sprintf (temp,"%s\n   (mm)  Δ%d", con2_p[7][2][0], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n      (mm)", con2_p[7][2][0]);


					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "0.00");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*Scan -> start -> start mode*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Reset All");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:/*Scan -> data -> storage*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Last");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/*File -> File -> Storage*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Storage Card");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*File -> report -> template*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Complete");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*File -> format -> user field*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*File -> user field -> select*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "1");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:/*File -> notes-> edit notes*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), " ");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*Preferences -> pref.-> units*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][0][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Millmeters");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 1:/*Preferences -> system -> clock set*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n(HH:MM:SS)", con2_p[9][1][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "02:55:06 PM");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 2:/*Preferences -> service -> system info*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), " ");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 3:/*Preferences -> options -> mouse*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Mouse");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				case 4:/*Preferences -> network -> DHCP*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[0], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
                                        break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data1(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;
	gchar *str;
	gint x, y;
	GtkAdjustment *adj;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> start*/
					g_sprintf (temp,"%s", con2_p[0][0][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Group Wizard");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*Wizard -> Focal Law -> start*/
					g_sprintf (temp,"%s", con2_p[0][1][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Law Wizard");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 2:/*Wizard -> Calibration -> start*/
					g_sprintf (temp,"%s", con2_p[0][2][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Select Calibration");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
#if 0
			switch (pp->pos1[1])
			{
				case 0: /* start 扫描延时 101 */
					/* 当前步进 */
					switch (pp->p_tmp_config->start_reg)
					{
						case 0:	tmpf = p->p_config->range / (1 * 1000.0); break;
						case 1:	tmpf = p->p_config->range / (2 * 1000.0); break;
						case 2:	tmpf = p->p_config->range / (32 * 1000.0); break;
						default:break;
					}

					/* 格式化字符串 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						gtk_widget_destroy (pp->vscale);

						g_sprintf (temp,"%s\n(dB) Δ%.1f", con2_p[1][0][1], tmpf);
						gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
						gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button0);
						widget_window_class->key_press_event = window_keypress_event_orig;
						g_signal_connect(G_OBJECT(pp->sbutton[1]), "value-changed", 
								G_CALLBACK(data_101), (gpointer) (p));
						/* 显示和隐藏控件 */
						gtk_widget_show (pp->eventbox30[1]);
						gtk_widget_hide (pp->eventbox31[1]);
						gtk_widget_show (pp->sbutton[1]);
						gtk_widget_grab_focus (pp->sbutton[1]);
						/* 设置值的范围 */
						adj = gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON (pp->sbutton[1]));
						gtk_adjustment_configure (adj , p->p_config->gain / 100.0, 
								0.0, 74.0, tmpf , 10.0, 0.0);
						gtk_spin_button_set_digits (GTK_SPIN_BUTTON (pp->sbutton[1]), 1);


						pp->vscale = gtk_vscale_new(adj);
						gtk_widget_set_size_request(GTK_WIDGET(pp->vscale), 30, 460);
						gtk_scale_set_draw_value (pp->vscale, FALSE);
						gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->vscale, TRUE, TRUE, 0);
						gtk_widget_show (pp->vscale);
					}
					else 
					{
						g_sprintf (temp,"%s\n(dB)", con2_p[1][0][1]);
						gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
						if ((CUR_POS) == 0 && (pp->pos_pos == MENU3_STOP))
						{
							gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button2);
							gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button2);
						}
						else
						{
							gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
							gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);
						}
						/* 更新当前增益值显示 */
						str = g_strdup_printf ("%0.1f", (gfloat)(pp->p_config->gain) / 100.0);
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), str);
						g_free(str);
						widget_window_class->key_press_event = my_keypress_event;
						/* 显示和隐藏控件 */
						gtk_widget_show (pp->eventbox30[1]);
						gtk_widget_show (pp->eventbox31[1]);
						gtk_widget_show (pp->data3[1]);
						gtk_widget_hide (pp->sbutton[1]);
						gtk_widget_hide (pp->vscale);
/*						gtk_widget_grab_focus (pp->button);*/
					}
					break;
				case 1: /* Tx/Rx Mode 收发模式  */
					g_sprintf (temp,"%s", con2_p[1][1][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);

					switch (pp->p_config->tx_rxmode) 
					{
						case 0:	gtk_label_set_text (GTK_LABEL (pp->data3[1]), "PC"); break;
						case 1:	gtk_label_set_text (GTK_LABEL (pp->data3[1]), "PE"); break;
						case 2:	gtk_label_set_text (GTK_LABEL (pp->data3[1]), "TT"); break;
						default:break;
					}
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				case 2: /*Filter*/					

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s", con2_p[1][2][1]);
					else 
						g_sprintf (temp,"%s", con2_p[1][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "None");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				case 3:/*Index Offset*/
					/* 当前步进 */
					switch (pp->p_tmp_config->indexoffset_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n(mm) Δ%d", con2_p[1][3][1], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n  (mm)", con2_p[1][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->index_offset / 10.0));
					g_free(str);
					break;
				case 4:/*Set Ref.*/
					g_sprintf (temp,"%s", con2_p[1][4][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "");

					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				default:break;
			}
#endif
			break;

		case 2:
			switch (p->pos1[2])
			{
				case 0:/*Gate->Parameters*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Position");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*Group A*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "1");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 2:/*Alarm #*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "None");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*Sizing Curves -> Curve*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "None");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Group*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Custom");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*Measurements -> Cursors -> Angle*/
					/* 当前步进 */
					switch (pp->p_tmp_config->VPA_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n(o)     Δ%d", con2_p[3][1][1], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n    (o)", con2_p[3][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->VPA / 10.0));
					g_free(str);
                                        break;
				case 2:/*Measurements -> Table -> Entry Image*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*Measurements -> Thickness -> Min*/
					/* 当前步进 */
					switch (pp->p_tmp_config->min_thickness_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[3][3][1], tmpf);
					else 
						g_sprintf (temp,"%s\n (mm)", con2_p[3][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->min_thickness / 10.0));
					g_free(str);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{
				case 0:/*Display -> Selection -> group*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Current");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*Display -> Overlay -> grid*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 2:/*Display -> Zoom -> Type*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Absolute");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*Display -> Color -> start*/
					/* 当前步进 */
					switch (pp->p_tmp_config->color_start_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n(%%)  Δ%d", con2_p[4][3][1], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n   (%%)", con2_p[4][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->color_start / 10.0));
					g_free(str);
                                        break;
				case 4:/*Display -> Color -> Color*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][4][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Red");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Group Mode*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "PA");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*probe/part -> position -> Index Offset*/
					/* 当前步进 */
					switch (pp->p_tmp_config->indexoffset_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n  (mm)   Δ%.1f", con2_p[5][1][1], tmpf);
					else 
						g_sprintf (temp,"%s\n      (mm)", con2_p[5][1][1]);
 
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "0.00");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					
                                        break;
				case 2:/*probe/part -> characterize -> gain*/
					/* 当前步进 */
					switch (pp->p_tmp_config->db_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n(dB) Δ%.1f", con2_p[5][2][1], tmpf);
					else 
						g_sprintf (temp,"%s\n  (dB)", con2_p[5][2][1]);
 
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "31.0");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*probe/part -> characterize -> gain*/
					/* 当前步进 */
					switch (pp->p_tmp_config->part_thickness_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n  (mm)  Δ%.2f", con2_p[5][3][1], tmpf);
					else 
						g_sprintf (temp,"%s\n      (mm)", con2_p[5][3][1]);
 
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "31.0");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:/*Focal Law -> configuration -> Law Config.*/
					/* 当前步进 */
					switch (pp->p_tmp_config->connection_P_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					//if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						//g_sprintf (temp,"%s\n  (1 to 113)  Δ%d", con2_p[6][0][1], (gint) (tmpf));
					//else 
						g_sprintf (temp,"%s\n     (1 to 113)", con2_p[6][0][1]);
 
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "1");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);

                                       break;
				case 1:/*Focal Law -> aperture -> first element*/
					/* 当前步进 */
					switch (pp->p_tmp_config->first_element_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n           Δ%d", con2_p[6][1][1], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s", con2_p[6][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "1");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 2:/*focal law -> beam -> max angle*/
					/* 当前步进 */
					switch (pp->p_tmp_config->max_angle_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n(deg)  Δ%.1f", con2_p[6][2][1], tmpf);
					else 
						g_sprintf (temp,"%s\n      (deg)", con2_p[6][2][1]);


					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "0.0");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*Focal Law -> laws -> load law file*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[6][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), " ");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[7])
			{
				case 0:/*Scan -> Encoder -> polarity*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Normal");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*Scan -> Inspection -> scan*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Time");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 2:/*Scan -> Area -> scan end*/
					/* 当前步进 */
					switch (pp->p_tmp_config->scan_end_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n   (mm)  Δ%d", con2_p[7][2][1], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n      (mm)", con2_p[7][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "400.00");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*Scan -> start -> pause*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 4:/*Scan -> data -> inspec.data*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][4][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "All A & C scans");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> Open*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), " ");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*File -> report -> file name*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Report####");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 2:/*File -> format -> probe*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*File -> user field -> enable*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 4:/*File -> notes-> edit header*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][4][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), " ");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:/*preferences -> pref. -> bright*/
					/* 当前步进 */
					switch (pp->p_tmp_config->bright_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1) )
						g_sprintf (temp,"%s\n  (%%)  Δ%d", con2_p[9][0][1], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n    (%%)", con2_p[9][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "25");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 1:/*Preferences -> system -> data set*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n(YYYY/MM/DD)", con2_p[9][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "2010/12/09");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 2:/*Preferences -> service -> Win CE*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), " ");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 3:/*Preferences -> options -> EZView*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				case 4:/*Preferences -> Network -> IP Address*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "192.168.0.2");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
                                        break;
				default:break;
			}
			break;
		default:break;
	}

}

void draw3_data2(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/
	gchar *str;
	gint x, y;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:/*Wizard -> Calibration -> Type*/
					g_sprintf (temp,"%s", con2_p[0][2][2]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Ultrasound");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* range范围 */
					/*当前步进*/
					switch (p->p_tmp_config->range_reg)
					{
						case 0:	tmpf = p->p_config->part.Velocity * 1.6 / 5000.0; break;
						case 1:	tmpf = p->p_config->part.Velocity * 8.0 / 5000.0; break;
						case 2:	tmpf = p->p_config->part.Velocity * 16.0 / 5000.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
					{
						g_sprintf (temp,"%s\n(%s) Δ%.1f", con2_p[1][0][2],!p->p_config->unit ? "mm" : "inch", tmpf);
						/*
						gtk_widget_show_all(pp->window1);
						gtk_window_set_keep_above( GTK_WINDOW (pp->window1), TRUE);
						gtk_window_get_position (GTK_WINDOW (pp->window), &x, &y);
						gtk_window_move(GTK_WINDOW (pp->window1), x + 685, y + 137 + 87 * 2);
						*/
					}
					else 
						g_sprintf (temp,"%s\n (%s)", con2_p[1][0][2],!p->p_config->unit ? "mm" : "inch");

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->range / 10.0);
					g_free(str);
					break;
				case 1: /* Freq频带(Mhz) */
					g_sprintf (temp,"%s", con2_p[1][1][2]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);

					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);

					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.2f", pp->p_config->frequence / 100.0);
					gtk_label_set_text(GTK_LABEL (pp->data3[2]),  (const gchar *) (str));
					g_free(str);
					break;
				case 2:/*Rectifier*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "RF");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
					break;
				case 3:/*Angle (deg.)*/
					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s", con2_p[1][3][2]);
					else 
						g_sprintf (temp,"%s", con2_p[1][3][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "0.0");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
					break;
				case 4:/*dB Ref.*/
					g_sprintf (temp,"%s", con2_p[1][4][2]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "On");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*Gate->Start*/
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_start_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.10; break;
						case 2:	tmpf = 1.00; break;
						case 3:	tmpf = 10.00; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[2][0][2], tmpf);
					else 
						g_sprintf (temp,"%s\n(mm)", con2_p[2][0][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->agate_start / 10.0);
					g_free(str);
                                        break;
				case 1:/*Condition*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "None");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->count / 10.0);
					g_free(str);
                                        break;
				case 3:/*NULL*/
                                       break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Field 1*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][0][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "A%");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 1:/*Measurements -> Cursors -> Scan*/
					/* 当前步进 */
					switch (pp->p_tmp_config->cursors_scan_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1;  break;
						case 2:	tmpf = 1.0;  break;
						case 3:	tmpf = 10.0;  break;					
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[3][1][2], tmpf);
					else 
						g_sprintf (temp,"%s\n  (mm)", con2_p[3][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->cursors_scan / 10.0);
					g_free(str);
                                        break;
				case 2:/*Measurements -> Table -> Add Entry*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 3:/*Measurements -> Thickness -> Max*/
					/* 当前步进 */
					switch (pp->p_tmp_config->max_thickness_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[3][3][2], tmpf);
					else 
						g_sprintf (temp,"%s\n (mm)", con2_p[3][3][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->max_thickness / 10.0));
					g_free(str);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{
				case 0:break;
				case 1:/*Display -> Overlay -> sizing curves*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "On");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 2:break;
				case 3:/*Display -> Color -> end*/
					/* 当前步进 */
					switch (pp->p_tmp_config->color_end_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n(%%)   Δ%d", con2_p[4][3][2], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n   (%%)", con2_p[4][3][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->color_end / 10.0));
					g_free(str);break;
				case 4:/*Display -> Properties -> Envelope*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][4][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "None");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Select*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Select Tx/Rx");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;

				case 1:/*Probe/Part -> position -> skew*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "90.0");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 2:/*Probe/Part -> Characterize -> start*/
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_start_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[5][2][2], tmpf);
					else 
						g_sprintf (temp,"%s\n  (mm)", con2_p[5][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "20.00");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);
					
					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 3:/*Probe/Part -> Parts -> diameter*/
					/* 当前步进 */
					switch (pp->p_tmp_config->diameter_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}
					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[5][3][2], tmpf);
					else 
						g_sprintf (temp,"%s\n     (mm)", con2_p[5][3][2]);


					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "100.00");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:/*Focal Law -> Configuration -> connection R*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n     (1 to 113)", con2_p[6][0][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "1");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 1:/*Focal Law -> aperture -> last element*/
					/* 当前步进 */
					switch (pp->p_tmp_config->last_element_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					//if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						//g_sprintf (temp,"%s\n           Δ%d", con2_p[6][1][2], (gint) (tmpf));
					//else 
						g_sprintf (temp,"%s", con2_p[6][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "128");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 2:/*focal law -> beam -> angel step*/
					/* 当前步进 */
					switch (pp->p_tmp_config->angle_step_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n  (deg) Δ%.1f", con2_p[6][2][2], tmpf);
					else 
						g_sprintf (temp,"%s\n     (deg)", con2_p[6][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "0.1");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 3:/*focal law -> laws -> save law file*/

					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[6][3][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), " ");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[7])
			{
				case 0:/*Scan -> Encoder -> type*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][0][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Quad");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 1:/*Scan -> Inspection -> Index*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 2:/*Scan -> Area -> scan resolution*/
					/* 当前步进 */
					switch (pp->p_tmp_config->scan_resolution_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2) )
						g_sprintf (temp,"%s\n       (mm)  Δ%.1f", con2_p[7][2][2], tmpf);
					else 
						g_sprintf (temp,"%s\n          (mm)", con2_p[7][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "1.00");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 3:/*Scan -> start -> start*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][3][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), " ");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> save setup as*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][0][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), " ");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 1:/*File -> report -> paper size*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "A4");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 2:/*File -> format -> setup*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 3:/*File -> user field -> label*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][3][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Contractor");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:/*preferences -> pref. -> admin password*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][0][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), " ");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 1:/*Preferences -> system -> select key*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][1][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "F2");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 2:/*Preferences -> service -> file manager*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), " ");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				case 4:/*Preferences -> network -> subnet mask*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), "255.255.255.0");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
                                        break;
				default:break;
			}
			break;
		default:break;
	}

}

void draw3_data3(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/
	gchar *str;
	gint x, y;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:/*Wizard -> Calibration -> Mode*/
					g_sprintf (temp,"%s", con2_p[0][2][3]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Sensitivity");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0:/* wedge delay */
					/* 当前步进 */
					switch (pp->p_tmp_config->wedge_delay_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
					{
						g_sprintf (temp,"%s\n(us) Δ%.2f", con2_p[1][0][3], tmpf);
						/*
						gtk_widget_show_all(pp->window1);
						gtk_window_set_keep_above( GTK_WINDOW (pp->window1), TRUE);
						gtk_window_get_position (GTK_WINDOW (pp->window), &x, &y);
						gtk_window_move(GTK_WINDOW (pp->window1), x + 685, y + 137 + 87 * 3);
						*/
					}
					else 
						g_sprintf (temp,"%s\n(us)", con2_p[1][0][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->wedge_delay / 10.0);
					g_free(str);
					break;
				case 1: /* 电压高低 功率? */
					g_sprintf (temp,"%s", con2_p[1][1][3]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);

					switch (pp->p_config->voltage) 
					{
						case VOL_LOW:	gtk_label_set_text (GTK_LABEL (pp->data3[3]), "45"); break; /* 低 45V */
						case VOL_HIGH:	gtk_label_set_text (GTK_LABEL (pp->data3[3]), "90"); break; /* 高 90V */
						default:break;
					}
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
					break;
				case 2:/*Video Filter*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "On");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
					break;
				case 3:/*Skew (deg.)*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][3][3]);				

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "0.0");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
					break;
				case 4:/*Points Qty.*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][4][3]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "320");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*Gate->Width*/
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_width_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[2][0][3], tmpf);
					else 
						g_sprintf (temp,"%s\n(mm)", con2_p[2][0][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->agate_width / 10.0);
					g_free(str);
                                        break;
				case 1:/*Operator*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][1][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "AND");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
                                        break;
				case 2:/*Sound*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
			}
		case 3:
			switch (p->pos1[3]) {
				case 0:break;
				case 1:/*Measurements -> Cursors -> Index*/
					   /* 当前步进 */
					switch (pp->p_tmp_config->cursors_index_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0;  break;
						case 2:	tmpf = 100.0;  break;				
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
						g_sprintf (temp,"%s\n(mm) Δ%.1f", con2_p[3][1][3], tmpf);
					else 
						g_sprintf (temp,"%s\n  (mm)", con2_p[3][1][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->cursors_index / 10.0);
					g_free(str);
                                        break;
				case 2:/*Measurements -> Table -> Delete Entry*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), " ");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 3:/*Measurements -> Thickness -> Echo Qty.*/
					/* 当前步进 */
					switch (pp->p_tmp_config->echo_qty_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
						g_sprintf (temp,"%s\n        Δ%d", con2_p[3][3][3], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s", con2_p[3][3][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					
					/* 更新当前pulser值显示 */
					str = g_strdup_printf ("%d", (guint) (pp->p_config->max_thickness / 10.0));
					g_free(str);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{
				case 0:break;
				case 1:/*Display -> Overlay -> gate*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "On");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 2:
                                        break;
				case 3:/*Display -> Color -> load*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][3][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), " ");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 4:/*Display -> Properties -> Source*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][4][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Normal");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Probe*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Unknown");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 1:break;
				case 2:/*Probe/Part -> Characterize -> width*/
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_width_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
						g_sprintf (temp,"%s\n(mm) Δ%.2f", con2_p[5][2][3], tmpf);
					else 
						g_sprintf (temp,"%s\n  (mm)", con2_p[5][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "29.98");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);
					
					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 3:/*Probe/Part -> parts -> material*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][3][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "STEEL.MILD");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:break;
				case 1:/*Focal Law -> aperture -> element step*/
					/* 当前步进 */
					switch (pp->p_tmp_config->element_step_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					//if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
						//g_sprintf (temp,"%s\n           Δ%d", con2_p[6][1][3], (gint) (tmpf));
					//else 
						g_sprintf (temp,"%s", con2_p[6][1][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "1");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 2:/*Focal law -> Beam -> focus depth*/
					/* 当前步进 */
					switch (pp->p_tmp_config->focus_depth_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
						g_sprintf (temp,"%s\n    (mm) Δ%.1f", con2_p[6][2][3], tmpf);
					else 
						g_sprintf (temp,"%s\n       (mm)", con2_p[6][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "50.00");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[7])
			{
				case 0:/*Scan -> Encoder -> resolution*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n  (Step/mm)", con2_p[7][0][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "1.00");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 1:/*Scan -> Inspection -> scan speed*/
					/* 当前步进 */
					switch (pp->p_tmp_config->scan_speed_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3) )
						g_sprintf (temp,"%s\n  (mm/s) Δ%d", con2_p[7][1][3], (gint) (tmpf));
					else 
						g_sprintf (temp,"%s\n     (mm/s)", con2_p[7][1][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "46.00");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 2:/*Scan -> area -> index start*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n     (mm)", con2_p[7][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "0.00");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> save data*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][0][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), " ");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 1:/*File -> report -> build*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][1][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), " ");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 2:/*File -> format -> Note*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 3:/*File -> user field -> content*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][3][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Name");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:/*preferences -> pref. -> scheme*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][0][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Indoor");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 1:/*Preferences -> system -> assign key*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][1][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Gain");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 2:/*Preferences -> service -> Import/Export*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), " ");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 3:/*Preferences -> Options -> remote desktop*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][3][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Off");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				case 4:/*Preferences -> Network -> Apply*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
					gtk_widget_modify_bg (pp->eventbox30[3], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[3]), " ");
					gtk_widget_modify_bg (pp->eventbox31[3], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[3]);
					gtk_widget_show (pp->eventbox31[3]);
					gtk_widget_show (pp->data3[3]);
                                        break;
				default:break;
			}
			break;
		default:break;
	}


}

void draw3_data4(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/
	gchar *str;
	gint x, y;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0:/* wedge delay */
					/* 当前步进 */
					switch (pp->p_tmp_config->velocity_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4) )
					{
						g_sprintf (temp,"%s\n(m/s) Δ%.1f", con2_p[1][0][4], tmpf);
						/*
						gtk_widget_show_all(pp->window1);
						gtk_window_set_keep_above( GTK_WINDOW (pp->window1), TRUE);
						gtk_window_get_position (GTK_WINDOW (pp->window), &x, &y);
						gtk_window_move(GTK_WINDOW (pp->window1), x + 685, y + 137 + 87 * 4);
						*/
					}
					else 
						g_sprintf (temp,"%s\n(m/s)", con2_p[1][0][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->part.Velocity / 10.0);
					g_free(str);

					break;
				case 1: /* 脉冲宽度 pulser width */
					g_sprintf (temp,"%s", con2_p[1][1][4]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);

					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->pulser_width / 10.0);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]),  (const gchar *) (str));
					g_free(str);
					break;
				case 2:/*Averaging*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "1");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
					break;
				case 3:/*Beam Delay*/
					/* 当前步进 */
					switch (pp->p_tmp_config->beam_delay_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4) )
						g_sprintf (temp,"%s\n(us) Δ%.2f", con2_p[1][3][4], tmpf);
					else 
						g_sprintf (temp,"%s\n(us)", con2_p[1][3][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->beam_delay / 10.0);
					g_free(str);
					break;
				case 4:/*Scale Factor*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][4][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "10");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*Gate->Threshold*/
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_height_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4) )
						g_sprintf (temp,"%s\n(%%) Δ%d", con2_p[2][0][4], (guint) (tmpf));
					else 
						g_sprintf (temp,"%s\n(%%)", con2_p[2][0][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->agate_height / 10.0);
					g_free(str);
                                       break;
				case 1:/*Group B*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][1][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "1");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 2:/*Output->delay*/
					/* 当前步进 */
					switch (pp->p_tmp_config->active_delay_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
                                                case 3:	tmpf = 10.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4) )
						g_sprintf (temp,"%s\n(ms) Δ%.2f", con2_p[2][2][4], tmpf);
					else 
						g_sprintf (temp,"%s\n (ms)", con2_p[2][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->active_delay / 10.0);
					g_free(str);

                                       break;
				case 3:/*NULL*/
                                       break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Field 3*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][0][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "ViA^");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 1:/*Measurements -> Cursors -> Add Entry*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][1][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);
					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);

                                        break;
				case 2:/*Measurements -> Table -> Select Entry*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "0");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);
					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{
				case 0:break;
				case 1:/*Display -> Overlay -> cursor*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "On");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 2:break;
				case 3:break;
				case 4:/*Display -> properties -> appearence*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][4][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Hollow");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Wedge*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Contact");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 1:break;
				case 2:/*Probe/Part -> characterize -> procedure*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Undefined");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:break;
				case 1:/*Focal Law -> aperture -> wave type*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[6][1][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "LW");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[7])
			{
				case 0:/*Scan -> Encodr -> Origin*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n  (mm)", con2_p[7][0][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "0.00");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 1:break;
				case 2:/*Scan -> area -> index end*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n     (mm)", con2_p[7][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "0.00");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> save mode*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][0][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Inspection Data");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 1:break;
				case 2:/*File -> format -> view*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Current Layout");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:/*preferences -> pref. -> gate mode*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][0][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "By Group");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 1:break;
				case 2:/*Preferences -> service -> startup mode*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Automatic");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				case 3:break;
				case 4:/*Preferences -> Network -> Remote PC*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "RemoteName");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
                                        break;
				default:break;
			}
			break;
		default:break;
	}


}

void draw3_data5(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/
	gchar *str;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:/* Wizard -> Calibration -> clear calib */
					g_sprintf (temp,"%s", con2_p[0][2][5]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* NULL 空 */
					break;
				case 1: /* 重复频率 PRF */
					g_sprintf (temp,"%s", con2_p[1][1][5]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);

					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%d", pp->p_config->prf);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]),  (const gchar *) (str));
					g_free(str);
					break;
				case 2:/*Reject*/
					/* 当前步进 */
					switch (pp->p_tmp_config->reject_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;                                               				
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5) )
						g_sprintf (temp,"%s\n(%%) Δ%d", con2_p[1][2][5], (guint) (tmpf));
					else 
						g_sprintf (temp,"%s\n(%%)", con2_p[1][2][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->reject / 10.0);
					g_free(str);
					break;
				case 3:/*Gain Offset*/
					/* 当前步进 */
					switch (pp->p_tmp_config->gainoffset_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break; 
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;                                              				
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5) )
						g_sprintf (temp,"%s\n(dB) Δ%.1f", con2_p[1][3][5], tmpf);
					else 
						g_sprintf (temp,"%s\n(dB)", con2_p[1][3][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->gain_offset / 10.0);
					g_free(str);

					break;
				case 4:/*Sum Gain*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][4][5]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Auto");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);

					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*NULL*/
                                       break;
				case 1:/*Condition*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[2][1][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "None");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 2:/*Hold Time*/
					/* 当前步进 */
					switch (pp->p_tmp_config->holdtime_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
                                                case 3:	tmpf = 10.0; break;						
						default:break;
					}

					/* 格式化字符串 */
					if ( (pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5) )
						g_sprintf (temp,"%s\n(ms) Δ%.2f", con2_p[2][2][5], tmpf);
					else 
						g_sprintf (temp,"%s\n     (ms)", con2_p[2][2][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%0.1f", pp->p_config->holdtime / 10.0);
					g_free(str);
                                        break;
				case 3:/*NULL*/
                                       break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Field 4*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][0][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "VsA^");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 1:break;
				case 2:/*Measurements -> Table -> Edit Comments*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), " ");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                       break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{
				case 0:break;
				case 1:/*Display -> Overlay -> overlay*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "On");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 2:break;
				case 3:break;
				case 4:/*Display -> properties -> overlay*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][4][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "None");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Auto Detect*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "On");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 1:
                                        break;
				case 2:/*Probe/Part -> characterize -> procedure*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][2][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Undefined");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[7])
			{
				case 0:/*Scan -> Encodr -> preset*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][0][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), " ");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 1:break;
				case 2:/*Scan -> area -> index resolution*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n         (mm)", con2_p[7][2][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "128.00");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> file name*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][0][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Data####");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:/*Preferences -> Network -> connect*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
                                        break;
				default:break;
			}
			break;
		default:break;
	}


}

/* 三级菜单*/
void draw_3_menu(gint pa, gpointer p)
{
	gint i;

	for (i = 0 ; i < 6 ; i++) 
	{
		/*
		 * 情况1 一级或者二级菜单改变时候, 更新所有三级菜单
		 * 情况2 一二 级菜单都不改变的时候, 更新 改变的三级菜单 
		 * 情况3 pa 强制全部更新
		 *
		 * */
		if ( (pp->pos_last != pp->pos) || 
				( pp->pos_last1 != pp->pos1[pp->pos] ) ||
				(pp->pos_last2 == i || pp->pos2[pp->pos][pp->pos1[pp->pos]] == i) || pa) 
		{
			gtk_widget_set_sensitive (pp->eventbox30[i], TRUE);
			gtk_widget_set_sensitive (pp->eventbox31[i], TRUE);
			gtk_widget_set_size_request(GTK_WIDGET(pp->eventbox30[i]), 115, 56);            /* */


			/*
			 * 如果三级菜单存在, 显示出来, 并显示 选项或者数值
			 * 不存在就隐藏
			 * */
			if ( con2_p[pp->pos][pp->pos1[pp->pos]][i] ) 
			{
				/* 0-5 表示6个3三级菜单 */
				switch (i)
				{
					case 0:	draw3_data0(p);break;
					case 1:	draw3_data1(pp);break;
					case 2:	draw3_data2(pp);break;
					case 3:	draw3_data3(pp);break;
					case 4:	draw3_data4(pp);break;
					case 5:	draw3_data5(pp);break;
					default:break;
				}
			}
			else
			{
				gtk_label_set_text (GTK_LABEL (pp->label3[i]), " ");
				gtk_widget_hide (pp->eventbox30[i]);	/**/
				gtk_widget_hide (pp->eventbox31[i]);	/**/
				gtk_widget_hide (pp->sbutton[i]);	/**/
			}
		}
	}
#if 0
	/* 停留或者按下 颜色不一样 按下时候获取焦点*/
	if (pp->pos_pos == MENU3_PRESSED)
	{
		gtk_widget_modify_bg (pp->eventbox30[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button0);
		gtk_widget_modify_bg (pp->eventbox31[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button0);
	}
	else if (pp->pos_pos == MENU3_STOP) 
	{
		gtk_widget_modify_bg (pp->eventbox30[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button2);
		gtk_widget_modify_bg (pp->eventbox31[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button2);
/*		gtk_widget_grab_focus (pp->button);*/
		if (gtk_widget_get_visible(pp->window1) && 0)
		{
			 gtk_widget_set_visible  (  (pp->window1), FALSE);
		}
		widget_window_class->key_press_event = my_keypress_event;
	}
	else
	{
/*		gtk_widget_grab_focus (pp->button);*/
		if (gtk_widget_get_visible(pp->window1) && 0)
		{
			gtk_widget_set_visible  (  (pp->window1), FALSE);
		}
		widget_window_class->key_press_event = my_keypress_event;
	}

#endif
	return ;
#if 0
	g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],
			0, "is-focus", TRUE,	NULL); 
	g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],
			0, "editable", FALSE, NULL); 
#endif
}


static void travel_path (cairo_t *cr)
{
	gint i;

	//   cairo_set_source_rgb (cr, 1, 1, 1);
	//   cairo_paint (cr);

	/* Use IBM Blue Pen Color with no ALPHA */
	cairo_set_source_rgba (cr, 1, 1, 0, 1);
	cairo_set_line_width (cr, 1);
	//   cairo_move_to (cr, 10, 10);
	//   cairo_line_to (cr, 11, 200);
	//   cairo_set_line_width (cr, 1);
	cairo_stroke (cr);

	//   cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

	//	   cairo_move_to (cr, a3[i].x, a3[i].y);
	//	   cairo_move_to (cr, a3[0].x, a3[0].y);
	for (i = 0; i < 500; i++) {
		cairo_move_to (cr, a[i].x + 0.5, a[i].y );
		cairo_line_to (cr, a[i].x + 0.5, (a[i + 1].y + a[i].y)/2); 
		cairo_move_to (cr, a[i + 1].x + 0.5, (a[i + 1].y + a[i].y)/2); 
		cairo_line_to (cr, a[i + 1].x + 0.5, a[i + 1].y); 
	}

	cairo_stroke(cr);
}

static void draw_gtk (GtkWidget      *widget, GdkEventExpose *eev, gpointer        data)
{
	cairo_t *cr;
	cr = gdk_cairo_create (widget->window);
	travel_path (cr);
	cairo_destroy (cr);
}

static gboolean time_handler(GtkWidget *widget)
{
	gint32 i;

	while (1) 
	{
		for ( i = 0; i < 512; i++)
			a[i].y = g_random_int_range(1, 512);

		gdk_threads_enter();
		gtk_widget_queue_draw(widget);
		gdk_threads_leave();
		g_usleep(100000);
	}
	g_thread_exit(NULL);
	return TRUE;
}

void init_ui(DRAW_UI_P p)				/*初始化界面,*/
{
	gint i;
	GtkWidget *drawing_area;
	GtkWidget *drawing_area1;
	GtkWidget *window = p->window;
	pp->pos_pos = MENU3_STOP;
	pp->menu2_qty = 5;

	for (i = 0; i < 512; i++) 
	{
		a[i].x =  i;
		a[i].y =  i;
	}


	/* New a window */
	p->pos	= 1;
	p->pos1[0]	= 0;
	p->pos2[0][0]	= 0;

	p->vbox			= gtk_vbox_new(FALSE, 0);	
	p->hbox1		= gtk_hbox_new(FALSE, 0);	
	p->vbox11		= gtk_vbox_new(FALSE, 0);	
	p->hbox111		= gtk_hbox_new(FALSE, 0);	
	p->vbox1111[0]	= gtk_vbox_new(FALSE, 0);	
	p->vbox1111[1]	= gtk_vbox_new(FALSE, 0);	
	p->vbox1111[2]	= gtk_vbox_new(FALSE, 0);	
	p->hbox112		= gtk_hbox_new(FALSE, 0);	

	p->hbox2		= gtk_hbox_new(FALSE, 0);	
	p->vbox21		= gtk_vbox_new(FALSE, 0);	
	p->hbox211		= gtk_hbox_new(FALSE, 0);	
	p->vbox2111		= gtk_vbox_new(FALSE, 0);	
	p->hbox2112		= gtk_hbox_new(FALSE, 0);	
	p->hbox212		= gtk_hbox_new(FALSE, 0);	

	p->vbox22		= gtk_vbox_new(FALSE, 0);	

	for (i = 0; i < 6; i++)
		p->vbox221[i]	= gtk_vbox_new(FALSE, 0);	

	for (i = 0; i < 20; i++) {
		pp->label[i]	= gtk_label_new ("AA");
		gtk_widget_modify_fg(pp->label[i], GTK_STATE_NORMAL, &color_white);					/* 字体颜色白色 */
		pp->event[i]  =  gtk_event_box_new();
		gtk_container_add(GTK_CONTAINER(p->event[i]), p->label[i]);
		gtk_container_set_border_width( GTK_CONTAINER(pp->event[i]), 1);     /*设置边框大小，这个地方使用图片*/
		gtk_widget_modify_bg (pp->event[i], GTK_STATE_NORMAL, &color_button1);
	}

	p->view			= gtk_text_view_new ();

	widget_window_class = GTK_WIDGET_GET_CLASS (((GtkObject*)(pp->window))); 
	// 取代原來的處理函式
	window_keypress_event_orig = widget_window_class->key_press_event; 

/*
	pp->button = gtk_button_new_with_label(" ");
	gtk_box_pack_start(GTK_BOX(pp->hbox1), pp->button, FALSE, FALSE, 0);
	gtk_widget_show(pp->button);
	*/

	/*一级菜单的初始化*/
	p->menubar		= gtk_menu_bar_new();
	p->menu			= gtk_menu_new();
	p->menuitem_main	= gtk_menu_item_new_with_label(content_en10[1]);
	gtk_widget_set_size_request(GTK_WIDGET(p->menuitem_main), 113, 60);          /*刚好合适的大小*/
	/*	gtk_widget_modify_fg(p->menuitem_main, GTK_STATE_NORMAL, &color_red);
		gtk_widget_modify_bg(p->menuitem_main, GTK_STATE_NORMAL, &color_green);*/
	gtk_menu_bar_append(GTK_MENU_BAR(p->menubar), p->menuitem_main);
	for (i = 0; i < 10; i++)
	{
		p->menuitem[i]	= gtk_menu_item_new_with_label(content_en10[i]);
		gtk_widget_set_size_request(GTK_WIDGET(p->menuitem[i]), 110, 45);
		/*		gtk_widget_modify_bg(p->menuitem[i], GTK_STATE_NORMAL, &color_green);*/
		g_signal_connect(G_OBJECT(p->menuitem[i]), "activate", 
				G_CALLBACK(menu_fun[i]), (gpointer) (p));
		gtk_menu_shell_append(GTK_MENU_SHELL(p->menu), p->menuitem[i]);
		gtk_widget_show(p->menuitem[i]);
	}
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(p->menuitem_main), p->menu);/*最后把菜单menu1粘到菜单项menuitem1上*/
	gtk_box_pack_start(GTK_BOX(p->hbox212), p->menubar, FALSE, FALSE, 0);
	g_object_set (p->menubar, "pack-direction", GTK_PACK_DIRECTION_LTR, NULL); 
	gtk_widget_show(p->menubar);
	gtk_widget_show(p->menu);
	gtk_widget_show(p->menuitem_main);



	/*二级菜单的初始化*/
	for (i = 0; i < 5; i++) 
	{
		p->eventbox2[i] = gtk_event_box_new();
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox2[i]), 114, 60);
		p->label2[i] = gtk_label_new("<^_^>");
		gtk_container_add(GTK_CONTAINER(p->eventbox2[i]), p->label2[i]);
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox2[i]), 1);     /*设置边框大小，这个地方使用图片做背景*/
/*		color_green.red = 0x7e00, color_green.green = 0xb700, color_green.blue = 0xf300;      */
/*		gtk_widget_modify_fg(p->eventbox2[i], GTK_STATE_NORMAL, &color_white);*/
		gtk_widget_modify_fg(p->label2[i], GTK_STATE_NORMAL, &color_white);
		g_signal_connect(G_OBJECT(p->eventbox2[i]), "button-press-event", 
				G_CALLBACK(eventbox2_fun[i]), (gpointer) (p));

		gtk_box_pack_start(GTK_BOX(p->hbox212), p->eventbox2[i], FALSE, FALSE, 0);
		gtk_widget_show(p->eventbox2[i]);
		gtk_widget_show(p->label2[i]);
	}


	/*三级菜单的初始化*/
	for (i = 0; i < 6; i++)
	{
		p->eventbox30[i] = gtk_event_box_new();
		p->eventbox31[i] = gtk_event_box_new();
		p->label3[i]     = gtk_label_new("<^_^>");
		p->data3[i]      = gtk_label_new("<^_^>");
		pp->adj = (GtkAdjustment *) gtk_adjustment_new (10.0, 0.0, 74.0, 0.1, 10.0, 0.0);
		pp->sbutton[i] = gtk_spin_button_new (pp->adj, 0, 1);

		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox30[i]), 115, 56);           /* 配置名称*/
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox31[i]), 115, 27);           /* 标签 */
		gtk_widget_set_size_request(GTK_WIDGET(p->sbutton[i]), 115, 27);           /* 标签 */

		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox30[i]), 0);     /*设置边框大小，这个地方使用图片*/
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox31[i]), 0);     /*设置边框大小，这个地方使用图片*/
		gtk_container_add(GTK_CONTAINER(p->eventbox30[i]), p->label3[i]);
		gtk_container_add(GTK_CONTAINER(p->eventbox31[i]), p->data3[i]);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox30[i], FALSE, FALSE, 1);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox31[i], FALSE, FALSE, 1);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->sbutton[i], FALSE, FALSE, 1);
		gtk_widget_modify_fg(p->label3[i], GTK_STATE_NORMAL, &color_white);					/* 字体颜色白色 */
		gtk_widget_modify_fg(p->data3[i], GTK_STATE_NORMAL, &color_white);
		g_signal_connect(G_OBJECT(p->eventbox30[i]), "button-press-event", 
				G_CALLBACK(data_fun[i]), (GUINT_TO_POINTER (i)));
		g_signal_connect(G_OBJECT(p->eventbox31[i]), "button-press-event", 
				G_CALLBACK(data_fun[i]), (GUINT_TO_POINTER (i)));
		gtk_widget_show(p->eventbox30[i]);
		gtk_widget_show(p->eventbox31[i]);
		gtk_widget_show(p->label3[i]);
	}



	gtk_container_add(GTK_CONTAINER(window), p->vbox);
	gtk_widget_show(p->vbox);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox1, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox1);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox2, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox2);


/*
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(drawing_area), 800, 78);
	gtk_box_pack_start (GTK_BOX (p->hbox1), drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x5555, p->col.green = 0x5555, p->col.blue = 0x5555;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show(drawing_area);
	*/

	/* 上方数据显示  */
	gtk_box_pack_start (GTK_BOX (p->hbox1), pp->vbox11, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox111, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox112, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[0], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[0]), 34, 34);
	gtk_label_set_text (GTK_LABEL (pp->label[0]), "Gain\n(dB)");
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[1], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[1]), 137, 34);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[2], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[2], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[2]), 171, 16);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[3], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[3]), 171, 16);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[4], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[4]), 171, 16);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[5], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[5]), 171, 16);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[6], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[6]), 171, 16);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[7], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[7]), 171, 16);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[8], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[8]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[8]), 34, 34);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[9], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[9]), 137, 34);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[10], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[10]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[10]), 34, 34);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[11], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[11]), 137, 34);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[12], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[12]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[12]), 34, 34);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[13], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[13]), 137, 34);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[14], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[14]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[14]), 34, 34);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[15], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[15]), 137, 34);


	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox21, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox22, FALSE, FALSE, 0);
	gtk_widget_show(p->vbox21);
	gtk_widget_show(p->vbox22);

	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox211, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox212, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox211);
	gtk_widget_show(p->hbox212);

	gtk_widget_show_all (pp->hbox1);

	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(drawing_area), 655, 460);
	gtk_box_pack_start (GTK_BOX (p->hbox211), drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x0, p->col.green = 0x0, p->col.blue = 0x0;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show (drawing_area);
	g_signal_connect (G_OBJECT (drawing_area), "expose_event", G_CALLBACK(draw_gtk), NULL);

	pp->vscalebox = gtk_vbox_new (FALSE, 0);
/*	gtk_widget_set_size_request(GTK_WIDGET(pp->vscalebox), 30, 460);*/
	gtk_box_pack_start (GTK_BOX (p->hbox211), pp->vscalebox, FALSE, FALSE, 0);
	gtk_widget_show(pp->vscalebox);

	/*
	drawing_area1 = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(drawing_area1), 20, 460);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), drawing_area1, FALSE, FALSE, 0);
	p->col.red = 0x1111, p->col.green = 0x1111, p->col.blue = 0x1111;
	gtk_widget_modify_bg(drawing_area1, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show(drawing_area1);

*/
	for (i = 0; i < 6; i++)
	{
		gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221[i], FALSE, FALSE, 0);
		gtk_widget_show(p->vbox221[i]);
	}

	draw_1_menu(p);
	draw_2_menu(1);
	draw_3_menu(1, NULL);

	pp->window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (pp->window1), FALSE);			/*不可以装饰*/
	pp->window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (pp->window2), FALSE);			/*不可以装饰*/


	g_thread_create((GThreadFunc)(time_handler), (gpointer) (drawing_area), FALSE, NULL);
	/*
	   gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221, FALSE, FALSE, 0);
	   gtk_widget_show(p->vbox221);
	   */

}
