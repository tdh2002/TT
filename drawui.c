
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>

GdkPoint a[512] = {{0,0},{240,200},{400,200}};

void change_language();
void draw_1_menu(DRAW_UI_P p);
void draw_2_menu(gint pa);
void draw_3_menu(gint pa);
void init_ui(DRAW_UI_P p);				/*初始化界面,*/

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
	p->pos = 0;
	pp->menu2_qty = 3;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem1_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 1;
	pp->menu2_qty = 5;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem2_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	p->pos = 2;
	pp->menu2_qty = 4;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem3_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 3;
	pp->menu2_qty = 5;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem4_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	p->pos = 4;
	pp->menu2_qty = 5;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem5_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 5;
	pp->menu2_qty = 4;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem6_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 6;
	pp->menu2_qty = 4;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d \n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem7_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 7;
	pp->menu2_qty = 5;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem8_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 8;
	pp->menu2_qty = 5;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
}

void menuitem9_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 9;
	pp->menu2_qty = 5;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(1);
	draw_3_menu(1);
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

/* 画二级菜单*/
void draw_2_menu(gint pa)
{
	gint i;

	for ( i = 0 ; i < 5 ; i++ )
	{
		if (( pp->pos_last1 == i ) || ( pp->pos1[pp->pos] == i ) || pa )
		{
			if (con1_p[pp->pos][i])
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
	if (pp->pos_pos == 0)
		gtk_widget_modify_bg (pp->eventbox2[pp->pos1[pp->pos]], GTK_STATE_NORMAL, &color_button2);
	else 
		gtk_widget_modify_bg (pp->eventbox2[pp->pos1[pp->pos]], GTK_STATE_NORMAL, &color_button0);
}

/**/
void draw3_data0(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
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
			switch (p->pos1[1])
			{
				case 0:/*增益 Gain */
					/*当前步进*/
					switch (p->p_tmp_config->db_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;
						default:break;
					}
					if ( (pp->pos_pos == MENU3_PRESSED) && (p->pos2[p->pos][p->pos1[p->pos]] == 0) )
						g_sprintf (temp,"%s\ndB Δ%.1f", con2_p[1][0][0], tmpf);
					else 
						g_sprintf (temp,"%s\ndB", con2_p[1][0][0]);

					gtk_label_set_text (GTK_LABEL (p->label3[0]), temp);
					gtk_widget_modify_bg (p->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
/*					gtk_widget_modify_bg (p->eventbox31[0], GTK_STATE_NORMAL, &color_button1);*/

					gtk_widget_show (p->eventbox30[0]);
					gtk_widget_hide (p->eventbox31[0]);
					gtk_widget_show (p->entry3[0]);

					gtk_widget_set_can_focus (p->entry3[0], FALSE);
					g_object_set ( p->entry3[0],	"is-focus", FALSE,	NULL);
					g_signal_connect(G_OBJECT(p->entry3[0]), "button-press-event", 
							G_CALLBACK(data_fun[0]), (gpointer) (p));
					//g_signal_connect(G_OBJECT(p->data[0]), "button-press-event", 
				//		G_CALLBACK(data_fun[0]), (gpointer) (p));
					break;
				case 1:/*发射 Pulser*/
					/*当前步进*/
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[0])
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
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
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

}

void draw3_data2(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

}

void draw3_data3(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

}

void draw3_data4(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

}

void draw3_data5(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

}

/* 画三级菜单*/
void draw_3_menu(gint pa)
{
	gint i;

	for ( i = 0 ; i < 6 ; i++) 
	{
		if (pp->pos_last2 == i || pp->pos2[pp->pos][pp->pos1[pp->pos]] == i || pa) 
		{
			if (con2_p[pp->pos][pp->pos1[pp->pos]][i]) 
			{

				if (i == 0)
					draw3_data0(pp);		/*第一个数值*/
				else if (i == 1)
					draw3_data1(pp);
				else if (i == 2)
					draw3_data2(pp);
				else if (i == 3)
					draw3_data3(pp);
				else if (i == 4)
					draw3_data4(pp);
				else if (i == 5)
					draw3_data5(pp);
			}
			else
			{
				gtk_label_set_text (GTK_LABEL (pp->label3[i]), " ");
				gtk_widget_hide (pp->eventbox30[i]);
				gtk_widget_hide (pp->eventbox31[i]);
			}
		}
	}
	/* 停留 或者 按下 颜色*/
	if (pp->pos_pos == MENU3_PRESSED)
	{
		gtk_widget_modify_bg (pp->eventbox30[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button0);
		gtk_widget_modify_bg (pp->eventbox31[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button0);
		if (!gtk_widget_get_can_focus (pp->entry3[pp->pos2[pp->pos][pp->pos1[pp->pos]]]))
			gtk_widget_set_can_focus ((pp->entry3[pp->pos2[pp->pos][pp->pos1[pp->pos]]]), TRUE);
//		g_object_set ( pp->entry3[pp->pos2[pp->pos][pp->pos1[pp->pos]]],	"is-focus", TRUE,	NULL );
		gtk_widget_grab_focus ( pp->entry3[pp->pos2[pp->pos][pp->pos1[pp->pos]]]);
	}
	else if (pp->pos_pos == MENU3_STOP) 
	{
		gtk_widget_modify_bg ( pp->eventbox30[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button2);
		gtk_widget_modify_bg ( pp->eventbox31[pp->pos2[pp->pos][pp->pos1[pp->pos]]],
				GTK_STATE_NORMAL, &color_button2);
//		g_object_set ( pp->entry3[pp->pos2[pp->pos][pp->pos1[pp->pos]]],	"is-focus", FALSE,	NULL );
		gtk_widget_grab_focus ( pp->button);
	}
	else
	{
//		g_object_set ( pp->entry3[pp->pos2[pp->pos][pp->pos1[pp->pos]]],	"is-focus", FALSE,	NULL );
		gtk_widget_grab_focus ( pp->button);
	}

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
	cairo_set_source_rgba (cr, 1, 0, 0, 1);
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


	while (1) {
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
	GtkWidget *window = p->window;
	pp->pos_pos = MENU3_STOP;
	pp->menu2_qty = 5;

	for (i = 0; i < 512; i++) {
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
	p->hbox1111[0]	= gtk_hbox_new(FALSE, 0);	
	p->hbox1111[1]	= gtk_hbox_new(FALSE, 0);	
	p->hbox1111[2]	= gtk_hbox_new(FALSE, 0);	
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

	for (i = 0; i < 20; i++)
		p->buffer[i]	= gtk_text_buffer_new (NULL);

	p->view			= gtk_text_view_new ();

	pp->button = gtk_button_new_with_label(" ");
	gtk_box_pack_start(GTK_BOX(pp->hbox1), pp->button, FALSE, FALSE, 0);
	gtk_widget_show(pp->button);

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
/*		color_green.red = 0x7e00, color_green.green = 0xb700, color_green.blue = 0xf300;      
		gtk_widget_modify_bg(p->eventbox2[i], GTK_STATE_NORMAL, &color_green);*/
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
		p->entry3[i]     = gtk_entry_new();

		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox30[i]), 115, 58);            /* 配置名称 */
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox31[i]), 115, 29);            /* 标签 */
		gtk_widget_set_size_request(GTK_WIDGET(p->entry3[i]), 115, 29);            /* 输入项目  */
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox30[i]), 0);     /*设置边框大小，这个地方使用图片*/
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox31[i]), 0);     /*设置边框大小，这个地方使用图片*/
		gtk_container_add(GTK_CONTAINER(p->eventbox30[i]), p->label3[i]);
		gtk_container_add(GTK_CONTAINER(p->eventbox31[i]), p->data3[i]);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox30[i], FALSE, FALSE, 1);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox31[i], FALSE, FALSE, 1);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->entry3[i], FALSE, FALSE, 1);
		g_signal_connect(G_OBJECT(p->eventbox30[i]), "button-press-event", 
				G_CALLBACK(data_fun[i]), (gpointer) (p));
		g_signal_connect(G_OBJECT(p->eventbox31[i]), "button-press-event", 
				G_CALLBACK(data_fun[i]), (gpointer) (p));
		g_signal_connect(G_OBJECT(p->entry3[i]), "button-press-event", 
				G_CALLBACK(data_fun[i]), (gpointer) (p));
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



	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 800, 78);
	gtk_box_pack_start (GTK_BOX (p->hbox1), drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x5555, p->col.green = 0x5555, p->col.blue = 0x5555;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show(drawing_area);

	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox21, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox22, FALSE, FALSE, 0);
	gtk_widget_show(p->vbox21);
	gtk_widget_show(p->vbox22);

	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox211, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox212, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox211);
	gtk_widget_show(p->hbox212);

	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 685, 460);
	gtk_box_pack_start (GTK_BOX (p->hbox211), drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x5555, p->col.green = 0x0, p->col.blue = 0x5555;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show(drawing_area);
	g_signal_connect(G_OBJECT (drawing_area), "expose_event", G_CALLBACK(draw_gtk), NULL);

	for (i = 0; i < 6; i++)
	{
		gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221[i], FALSE, FALSE, 0);
		gtk_widget_show(p->vbox221[i]);
	}

	draw_1_menu(p);
	draw_2_menu(1);
	draw_3_menu(1);


	g_thread_create((GThreadFunc)(time_handler), (gpointer) (drawing_area), FALSE, NULL);
	/*
	   gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221, FALSE, FALSE, 0);
	   gtk_widget_show(p->vbox221);
	   */

}
