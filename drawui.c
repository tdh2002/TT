
/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 *
 *
 */

#include "drawui.h"
#include "file_op.h"
#include "drawfb.h"
#include "draw_dialog.h"
#include <dirent.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <glib/gprintf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <webkit/webkit.h>
#include <gdk/gdkkeysyms.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <math.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> 
#include <semaphore.h>

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS((GtkObject*)(i))->x
#define YOFFSET  26

#define	P_CFG	(pp->p_config)

GdkColor	color_black     = {0x0, 0x0, 0x0, 0x0};
GdkColor	color_black1    = {0x0, 0x0, 0x0, 0x0800};
GdkColor	color_white     = {0x0, 0xffff, 0xffff, 0xffff};
GdkColor	color_yellow    = {0x0, 0xffff, 0xffff, 0x0};
GdkColor	color_green     = {0x0, 0x0, 0xffff, 0x0};
GdkColor	color_blue      = {0x0, 0x0, 0x0, 0xffff};
GdkColor	color_red       = {0x0, 0x0, 0x0, 0xffff};
GdkColor	color_text_base = {0x0, 0x1300, 0x4900, 0x7600};
GdkColor	color_rule      = {0x0, 0xc300, 0xf000, 0x1d00};

GROUP g_tmp_group_struct;

pthread_mutex_t draw_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  draw_thread_signal = PTHREAD_COND_INITIALIZER;
volatile int *DMA_MARK ;

#if 0
static char *keyboard_display[] = 
{
	"1\0", "2\0", "3\0", "4\0", "5\0", "6\0", "7\0", "8\0", "9\0", "0\0", 
	"-\0", "=\0", "BackSpace\0", "Tab\0", "Q\0", "W\0", "E\0", "R\0", "T\0",
	"Y\0", "U\0", "I\0", "O\0", "P\0", "[\0", "]\0","Caps Lock\0","A\0", "S\0", 
	"D\0", "F\0", "G\0","H\0", "J\0", "K\0", "L\0", ";\0","'\0","Enter\0", 
	"Shift       \0", "Z\0", "X\0", "C\0", "V\0", "B\0","N\0", "M\0", ",\0", 
	".\0", "↑\0", "/\0", "Delete\0","SuperKey\0","Alt\0", 
	"          Space            \0", "←\0", "↓\0", "→\0"
};

static gushort keyboard_send[] = 
{
	XK_1, XK_2, XK_3, XK_4, XK_5, XK_6,XK_7, XK_8, XK_9, XK_0, XK_minus, 
	XK_equal, XK_BackSpace, XK_Tab, XK_Q, XK_W, XK_E, XK_R, XK_T, XK_Y, 
	XK_U, XK_I, XK_O, XK_P, XK_bracketleft, XK_bracketright, XK_Caps_Lock, 
	XK_A, XK_S, XK_D,XK_F, XK_G, XK_H, XK_J, XK_K, XK_L,XK_semicolon, 
	XK_quotedbl, XK_Return,  XK_Shift_L, XK_Z, XK_X, XK_C, XK_V, XK_B,XK_N, 
	XK_M, XK_comma, XK_period, XK_Up, XK_slash,XK_Delete, XK_Super_L, XK_Alt_L,
	XK_space,XK_Left, XK_Down, XK_Right
};
#endif

static char *numkeyboard_display[]=
{
	"1\0", "2\0", "3\0", "+","4\0", "5\0", "6\0","-", "7\0", "8\0", "9\0", ".", "0\0",  "←\0", "→\0", "Backspace\0", "Delete\0", "Enter\0"
};

static gushort numkeyboard_send[] = 
{
	XK_1, XK_2, XK_3, XK_plus, XK_4, XK_5, XK_6, XK_minus, XK_7, XK_8,  XK_9, XK_period, XK_0,  XK_Left, XK_Right, XK_BackSpace, XK_Delete, XK_Return
};

static gchar buffer[32];
#if ARM
static guchar dot_temp[800];
#endif
static DRAW_UI_P p_drawui_c;

gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);/* window 原始的按键处理 */
gint (*dialog_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);/* window 原始的按键处理 */
gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);			/* 自己的按键处理*/

GtkWidgetClass *widget_window_class;									/* 主windows class */
GtkWidgetClass *dialog_window_class;									/* 主windows class */

const gchar *backpic[] = 
{
	"pic/10.png", "pic/11.png", "pic/12.png",		/* 二级菜单 0按下 1未选中 2 停留 */
	"pic/00.png", "pic/01.png", "pic/02.png",	        /* 00上方数值 标识栏 01 数值背景 02 比较小的背景 */
	"pic/20.png", "pic/21.png", "pic/22.png",		/* 三级菜单名称 0按下 1未选中 2 停留 */
	"pic/30.png", "pic/31.png", "pic/32.png",		/* 三级菜单数值 0按下 1未选中 2 停留 */
	"pic/tt.png", "pic/311.png", "pic/322.png"      	/* 软键盘图标 */
};

GdkPixbuf *g_pixbuf_[18];

void change_language(guint lang, DRAW_UI_P p);
void draw_menu1();
void draw_menu2(gint pa);
void draw_menu3(gint pa, gpointer pt);
void init_ui(DRAW_UI_P p);				/* 初始化界面 */
void draw_area_all();
void draw_dac_tcg_curve(cairo_t *cr, int width, int height);

void save_config (GtkWidget *widget, GdkEventButton *event,	gpointer data);

void draw3_data0(DRAW_UI_P p);
void draw3_data1(DRAW_UI_P p);
void draw3_data2(DRAW_UI_P p);
void draw3_data3(DRAW_UI_P p);
void draw3_data4(DRAW_UI_P p);
void draw3_data5(DRAW_UI_P p);
extern void data_234_add_point(DRAW_UI_P p);
extern void data_235_del_point(DRAW_UI_P p);

void draw_field_name ();
void draw_field_value ();
void calc_measure_data();//Defined by hefan

void (*draw_data3[6])(DRAW_UI_P p) =
{
	draw3_data0,
	draw3_data1,
	draw3_data2,
	draw3_data3,
	draw3_data4,
	draw3_data5
};

/* 显示各个菜单图标的名称 */
const gchar **con0_p;
const gchar ***con1_p;
const gchar ****con2_p;
const gchar **units;
const gchar **menu_content;

const gchar **list;
const gchar **list1;
const gchar **field1;
const gchar **field;
const gchar **field_unit;

void menuitem0_function(GtkMenuItem *menuitem, gpointer data);

gint my_keypress_event(GtkWidget *widget, GdkEventKey *event)
{
	gpointer data = NULL;
	key_press_handler (widget, event, data);
	return 0;
}

void set_menu_position_tdh (GtkMenu *menu, gint *x, gint *y,
		gboolean *push_in, gpointer status_icon)
{
	*x = pp->x_pos;
	*y = pp->y_pos;
	return;
}
void set_menu_position_mainmenu (GtkMenu *menu, gint *x, gint *y,
		gboolean *push_in, gpointer status_icon)
{
	*x = 0;
	*y = 160;
	return;
}

static void all_bg_pic_in_mem()
{   
    gint i = 0;
    int count = sizeof(backpic) / sizeof(backpic[0]);

	for(i=0;i<count;i++)
	{
		g_pixbuf_[i] = gdk_pixbuf_new_from_file(backpic[i],NULL);
	}
}

/* 设置控件的背景图片 */
void update_widget_bg(GtkWidget *widget,int i)
{
	GtkStyle *style;   
	GdkPixmap *pixmap; 
	GdkColormap *colormap;

	int alpha_threshold = 0;

	colormap = gdk_colormap_get_system ();
    gdk_pixbuf_render_pixmap_and_mask_for_colormap(g_pixbuf_[i], 
			colormap, &pixmap, NULL, alpha_threshold);

	style = gtk_style_copy(widget->style);  

	if (style->bg_pixmap[GTK_STATE_NORMAL])      
	{
		g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);  
    }

	style->bg_pixmap[GTK_STATE_NORMAL] = (pixmap);
	gtk_widget_set_style( widget, style);
	g_object_unref(style);
}

void main_menu_pop(guint action)
{
	int i;
	if (MENU_POP == action)	/*弹出主菜单*/
	{
		gtk_menu_popup ( GTK_MENU (pp->menu), NULL, NULL, 
				(GtkMenuPositionFunc)set_menu_position_mainmenu,
				NULL,
				0,
				gtk_get_current_event_time());
		for(i=0;i<10;i++)
			gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[i]));
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
	}
	else if (MENU_HIDE == action) /*收回主菜单*/
	{
		gtk_menu_popdown( GTK_MENU (pp->menu));
	}
	else if ( (MENU_CHANGE == action)||(MENU_DOWN == action) )	/*轮流切换主菜单*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		if((pp->pos) == 9)
		{
			pp->pos = 0;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
				pp->pos++;
		}
		else
		{
			pp->pos++;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
				pp->pos++;
		}
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		gtk_menu_item_set_label(GTK_MENU_ITEM (pp->menuitem_main), pp->con0_p[pp->pos]);
		MENU_STATUS = MENU3_STOP;
		draw_menu2(0);
		draw_menu3(0, NULL);
	}
	else if (MENU_UP == action)/*向上切换主菜单*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		if((pp->pos) == 0)
		{
			pp->pos = 9;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
				pp->pos--;
		}
		else
		{
			pp->pos--;
			while (!gtk_widget_get_sensitive(pp->menuitem[pp->pos]))
				pp->pos--;
		}
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		gtk_menu_item_set_label(GTK_MENU_ITEM (pp->menuitem_main), pp->con0_p[pp->pos]);
		MENU_STATUS = MENU3_STOP;
		draw_menu2(0);
		draw_menu3(0, NULL);
	}
	else if (MENU_ENTER == action) /*选中当前菜单项，并收回主菜单*/
	{
		gtk_menu_item_activate (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		gtk_menu_popdown( GTK_MENU (pp->menu));
	}

}

/* 按键 弹出三级菜单选项 的处理函数 */
void menu3_pop(guint action)
{
	int i;
	if (MENU3_TURN == action)	/* 轮流、向下切换三级菜单选项 */
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));

		pp->menu3_e= pp->menu3_qty - 1;
		while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_e]))
			pp->menu3_e--;		/* pp->menu3_e为最后一个可以选的菜单选项位置 */

		if( (pp->menu3_poppos) == (pp->menu3_e) )	/*当在三级菜单选项的可以选择的最后一个位置时*/
		{
			pp->menu3_poppos = 0;			/*选中的位置跳到最前去*/
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos]))/*判断这一个位置是否可选*/
				pp->menu3_poppos++;
		}
		else
		{
			pp->menu3_poppos++;
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos]))
				pp->menu3_poppos++;
		}
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));
	}
	else if (MENU3_UP == action) /*向上切换三级菜单选项*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));

		pp->menu3_s=0;
		while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_s]))
			pp->menu3_s++;		/* pp->menu3_s为第一个可以选的菜单选项位置 */

		if((pp->menu3_poppos) == pp->menu3_s )		/*当在三级菜单选项的可以选择的第一个位置时*/
		{
			pp->menu3_poppos = (pp->menu3_qty - 1);	/*选中的位置跳到最后去*/
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos]))/*判断这一个位置是否可选*/
				pp->menu3_poppos--;
		}
		else
		{
			pp->menu3_poppos--;
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos])) 
				pp->menu3_poppos--;
		}

		gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));		
	}
	else if (MENU3_ENTER == action) 	/* 选中当前菜单项，并收回三级菜单 */
	{
		gtk_menu_item_activate (GTK_MENU_ITEM (pp->menu_item3[pp->menu3_poppos]));
		for(i=0;i<6;i++)
			gtk_menu_popdown( GTK_MENU (pp->menu33[i]));
		draw_menu3(1, NULL);
	}
	else if (MENU3_HIDE == action) 		/* 收回三级菜单选项 */
	{
		for(i=0;i<6;i++)
			gtk_menu_popdown( GTK_MENU (pp->menu33[i]));
	}

}

/* 弹出 隐藏 帮助窗口 */
void show_help(guint i)
{
	if (i == HELP_Y) /*弹出帮助窗口*/
	{
		switch(pp->pos)		/*改变帮助文档的路径*/
		{
			case 0:
				pp->file_path = "file:///home/tt/TT/source/system/Help/Contextual/Wizard_Menu.html";
				break;
			case 1:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/UT_Settings_Menu.html";
				break;
			case 2:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/GatesAlarm_Menu.html";
				break;
			case 3:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/Measurements_Menu.html";
				break;
			case 4:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/Display_Menu.html";
				break;
			case 5:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/PA__UT_Menu.html";
				break;
			case 6:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/FocalLaw_Submenu.html";
				break;
			case 7:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/Scan_Menu.html";
				break;
			case 8:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/File_Menu.html";
				break;
			case 9:
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/Preferences_Menu.html";
				break;
			default:break;
		}
		webkit_web_view_load_uri (pp->web_view, pp->file_path);		
		gtk_widget_hide(pp->hbox211);
		gtk_widget_show(pp->sw);

	}
	else  if (i == HELP_N) /*隐藏帮助窗口*/
	{
		gtk_widget_show(pp->hbox211);
		gtk_widget_hide(pp->sw);
	}
}

gboolean main_menu_press (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	main_menu_pop(MENU_POP);
	return TRUE;
}

void add_click (GtkButton *button, gpointer data)
{
	gtk_adjustment_set_value (pp->adj, gtk_adjustment_get_value (pp->adj) + gtk_adjustment_get_step_increment (pp->adj));

	gtk_adjustment_value_changed (pp->adj);
	return ;
}

void sub_click (GtkButton *button, gpointer data)
{
	gtk_adjustment_set_value (pp->adj, gtk_adjustment_get_value (pp->adj) - gtk_adjustment_get_step_increment (pp->adj));

	gtk_adjustment_value_changed (pp->adj);
	return ;
}

/*1级菜单 回调函数*/
void menuitem_function (GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = p_drawui_c;
	guint num = GPOINTER_TO_UINT(data);

	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));	/*  */
	p->pos_last = pp->pos; /*  */
	p->pos = num;
	/* 根据p的值来确定有几个么menu2_qty	*/
	switch (num)
	{
		case 1:
		case 3:
		case 6:
		case 7:
		case 8:
			p->menu2_qty = 5;
			break;
		case 0:
			p->menu2_qty = 3;
			break;
		case 2:
		case 4:
		case 5:
		case 9:
			p->menu2_qty = 4;
			break;
		default:break;
	}
	MENU_STATUS = MENU3_STOP; /* */
	draw_menu2(1);
	draw_menu3(1, NULL);
}

/* 画一级菜单*/
void draw_menu1 ()
{
	gint i;
	DRAW_UI_P p = p_drawui_c;
	for (i = 0; i < 10; i++)
		gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem[i]), p->con0_p[i]);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), p->con0_p[p->pos]);
	return ;
}

/* 画二级菜单 pa的作用未曾体现，用来表示菜单名子的位置 6 6 6 6 6 */
void draw_menu2 (gint pa)
{
	GdkColor	yellow    = {0x0, 0xffff, 0xffff, 0x0};
	GdkColor	white     = {0x0, 0xffff, 0xffff, 0xffff};
	DRAW_UI_P p = p_drawui_c;
	gint i;
#if 0
	gint a = 0, b = 0, c = 0, d = 0, e = 0;
	a = (pa >> 0) & 0x3f ;
	b = (pa >> 6) & 0x3f ;
	c = (pa >> 12) & 0x3f ;
	d = (pa >> 18) & 0x3f ;
	e = (pa >> 24) & 0x3f ;
#endif

	if (p->pos_pos != MENU3_PRESSED)
		p->mark_pop_change = 0;
	for (i = 0 ; i < 5 ; i++) 
	{
		if (p->con1_p[p->pos][i])
		{
			gtk_label_set_text (GTK_LABEL (p->label2[i]), p->con1_p[p->pos][i]);
			update_widget_bg (p->eventbox2[i], /*backpic[1]*/1);
			gtk_widget_modify_fg (p->label2[i], GTK_STATE_NORMAL, &white);
			gtk_widget_show (p->eventbox2[i]);
		}
		else
		{
			gtk_label_set_text (GTK_LABEL (p->label2[i]), " ");
			gtk_widget_hide (p->eventbox2[i]);
		}
	}
	/* 当前二级菜单不是停留就是按下 */
	if (p->pos_pos == 0)
	{
		update_widget_bg(p->eventbox2[p->pos1[p->pos]], /*backpic[2]*/ 2);
	}
	else 
	{
		update_widget_bg(p->eventbox2[p->pos1[p->pos]], /*backpic[0]*/ 0);
	}

	if((p->pos == 0) && (GROUP_VAL(group_mode) == 0))
		gtk_widget_set_sensitive(p->eventbox2[1], FALSE);
	else
		gtk_widget_set_sensitive(p->eventbox2[1], TRUE);

	gtk_widget_modify_fg(p->label2[p->pos1[p->pos]], GTK_STATE_NORMAL, &yellow);

	return ;
}

/* 画三级菜单*/
void draw_menu3 (gint pa, gpointer pt)
{
	DRAW_UI_P p = p_drawui_c;
	gint i;

	for (i = 0 ; i < 6 ; i++) 
	{
		gtk_widget_set_sensitive (p->eventbox30[i], TRUE);
		gtk_widget_set_sensitive (p->eventbox31[i], TRUE);

		if (CUR_POS_P(p) != i)
		{
			draw_data3[i](p);
		}
		gtk_widget_hide (pp->sbutton[i]);
	}
	draw_data3[CUR_POS_P(p)](p);

	p->menu3_amount = 6;
	/* 计算当前3级菜单活动的数量 */
	while (!(gtk_widget_get_visible(p->eventbox30[p->menu3_amount - 1])))
		p->menu3_amount--;
	while (!(gtk_widget_get_sensitive(p->eventbox30[p->menu3_amount - 1])))
	{
		if (p->menu3_amount > 1)
			p->menu3_amount--;
		else
			p->menu3_amount = 0;
	}

	return ;
}

/*
 * 处理 三级菜单弹出菜单的画图
 * fun   为回调函数 
 * set_menu_position 用来指定弹出菜单的位置
 * cur_value 为菜单当前数值
 * content 弹出菜单的字符串数组
 * qty 多少个弹出菜单
 * pos 这个菜单的位置
 * pop_pos 弹出菜单选中的位置
 * menu_status 标志当前menu_item是否可以选择 0-31 位 0是第一个 1是第二个....
 */
void draw3_pop_tt (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos, guint menu_status)
{
	gint i, x, y, z;
	GtkMenu *menu;
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
	for (i = 0; i < 64; i++) 
	{
		if (pp->menu_item3[i]) 
		{
			gtk_widget_destroy(pp->menu_item3[i]);
			pp->menu_item3[i] = NULL;
		}
		else 
			break;
	}

	for (i = 0; i < qty; i++ )
	{
		pp->menu_item3[i] = gtk_menu_item_new_with_label (content[i]);
		//		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu3), pp->menu_item3[i]);
		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu33[pos]), pp->menu_item3[i]);
		g_signal_connect (pp->menu_item3[i], "activate",
				G_CALLBACK(fun), (GUINT_TO_POINTER (i)));
		if (0x01 & (menu_status >> i))
			gtk_widget_set_sensitive (pp->menu_item3[i], FALSE);
		gtk_widget_show (pp->menu_item3[i]);
	}
	gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pop_pos]));

	//	menu = GTK_MENU (pp->menu3);
	menu = GTK_MENU (pp->menu33[pos]);

	gtk_menu_popup (menu, NULL, NULL, 
			(GtkMenuPositionFunc)set_menu_position_tdh,
			NULL,
			0,
			gtk_get_current_event_time());

	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	gtk_label_set_text (GTK_LABEL (pp->data3[pos]), cur_value);
	gtk_label_set_text (GTK_LABEL (pp->label3[pos]), str);
	if (str)
		g_free (str);
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[pos]);
	gtk_widget_show (pp->eventbox31[pos]);
	gtk_widget_show (pp->data3[pos]);
	gtk_widget_hide (pp->sbutton[pos]);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));

	pp->menu3_qty = qty;
	pp->menu3_poppos = pop_pos;
//	pp->pos_pos=MENU3_STOP;
	//pp->menu3_pos = pos;

	return ;
}

/*
 * 处理 三级菜单弹出菜单的画图
 * fun   为回调函数 
 * set_menu_position 用来指定弹出菜单的位置
 * cur_value 为菜单当前数值
 * content 弹出菜单的字符串数组
 * qty 多少个弹出菜单
 * pos 这个菜单的位置
 * pop_pos 弹出菜单选中的位置
 * menu_status 标志当前menu_item是否可以选择 0-31 位 0是第一个 1是第二个....
 * menu_on 标志当前menu_item是否显示［On］在其后
 * offset  偏移量
 */
static void draw3_pop_tt_on (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos, guint menu_status, guint menu_on, guint offset)
{
	gint i, x, y, z;
	GtkMenu *menu;
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
	for (i = 0; i < 64; i++) 
	{
		if (pp->menu_item3[i]) 
		{
			gtk_widget_destroy(pp->menu_item3[i]);
			pp->menu_item3[i] = NULL;
		}
		else 
			break;
	}

	for (i = 0; i < qty; i++ )
	{
		if (0x01 & (menu_on >> i))
			pp->menu_item3[i] = gtk_menu_item_new_with_label (content[i + offset]);
		else
			pp->menu_item3[i] = gtk_menu_item_new_with_label (content[i]);

		//		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu3), pp->menu_item3[i]);
		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu33[pos]), pp->menu_item3[i]);
		g_signal_connect (pp->menu_item3[i], "activate",
				G_CALLBACK(fun), (GUINT_TO_POINTER (i)));
		if (0x01 & (menu_status >> i))
			gtk_widget_set_sensitive (pp->menu_item3[i], FALSE);
		gtk_widget_show (pp->menu_item3[i]);
	}
	gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pop_pos]));

	//	menu = GTK_MENU (pp->menu3);
	menu = GTK_MENU (pp->menu33[pos]);

	gtk_menu_popup (menu, NULL, NULL, 
			(GtkMenuPositionFunc)set_menu_position_tdh,
			NULL,
			0,
			gtk_get_current_event_time());

	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	gtk_label_set_text (GTK_LABEL (pp->data3[pos]), cur_value);
	gtk_label_set_text (GTK_LABEL (pp->label3[pos]), str);
	if (str)
		g_free (str);
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[pos]);
	gtk_widget_show (pp->eventbox31[pos]);
	gtk_widget_show (pp->data3[pos]);
	gtk_widget_hide (pp->sbutton[pos]);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));

	pp->menu3_qty = qty;
	pp->menu3_poppos = pop_pos;
//	pp->pos_pos=MENU3_STOP;

	return ;
}

/* 
 * 三级菜单只做显示时函数
 * cur_value 为当前值 
 * pos 表示第几个位置
 * big_menu 表示当前栏没有数值 只有菜单 像set Ref set 80%..
 *
 */

static void draw3_popdown (const gchar *cur_value, guint pos, guint big_menu)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	//	gtk_menu_popdown( GTK_MENU (pp->menu3));
	gtk_menu_popdown( GTK_MENU (pp->menu33[pos]));

	if (big_menu)
		str = g_strdup_printf ("\n\n%s", con2_p[x][y][z]);	
	else
		str = g_strdup_printf ("%s", con2_p[x][y][z]);	

	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_PRESSED))
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}
	}
	else if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}	
	}
	else
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[7]*/ 7);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[13]*/ 13);
		}
		else 
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[10]*/ 10);
		}
	}
	gtk_label_set_text (GTK_LABEL (pp->data3[z]), cur_value);
	if (str)
		g_free(str);
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	gtk_widget_hide (pp->dialog);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));


	change_keypress_event (KEYPRESS_MAIN);

	return ;
}

/* 
 * 三级菜单只做显示时函数
 * cur_value 为当前值 
 * pos 表示第几个位置
 * big_menu 表示当前栏没有数值 只有菜单 像set Ref set 80%..
 * content_pos 表示一个菜单有多种名字的情况 与 pos不是一个值 为零时候不起作用
 */

static void draw3_popdown_offset (const gchar *cur_value, guint pos, guint big_menu, guint content_pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	//	gtk_menu_popdown( GTK_MENU (pp->menu3));
	gtk_menu_popdown( GTK_MENU (pp->menu33[pos]));

	if (big_menu)
		str = g_strdup_printf ("\n\n%s", con2_p[x][y][content_pos ? content_pos : pos]);	
	else
		str = g_strdup_printf ("%s", con2_p[x][y][content_pos ? content_pos : pos]);	

	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_PRESSED))
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
	    {
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}
	}
	else if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[8]*/ 8);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[14]*/ 14);
		}
		else
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[11]*/ 11);
		}
	}
	else
	{
		update_widget_bg (pp->eventbox30[z], /*backpic[7]*/ 7);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[13]*/ 13);
		}
		else 
		{
			update_widget_bg (pp->eventbox31[z], /*backpic[10]*/ 10);
		}
	}
	gtk_label_set_text (GTK_LABEL (pp->data3[z]), cur_value);
	if (str)
		g_free(str);
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	gtk_widget_hide (pp->dialog);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));


	change_keypress_event (KEYPRESS_MAIN);

	return ;
}


/*
 * 
 * 弹出scale触摸条的函数
 * 处理 三级菜单按下状态的画图，这个处理数值按键的部分
 * fun   为回调函数 
 * cur_value 为菜单当前数值
 * lower 为菜单最小值
 * upper 为菜单最大值
 * step  为菜单步进
 * digit 为保留小数点数
 * pos 为第几个3级菜单
 * content_pos 表示一个菜单有多种名字的情况 与 pos不是一个值 为零时候不起作用
 * p 预留
 * 
 */
static void draw3_digit_pressed (void (*fun)(GtkSpinButton*, gpointer), const gchar *unit, 
		gfloat cur_value, gfloat lower, gfloat upper, gfloat step, 
		guint digit, gpointer p, guint pos, guint content_pos)
{
	guint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	if (!unit )
		str = g_strdup_printf ("%s", con2_p[x][y][content_pos ? content_pos : pos] );	
	else
		str = g_strdup_printf ("%s\n%s Δ%0.*f", 
				con2_p[x][y][content_pos ? content_pos : pos], unit, digit, step);	/* %*.*f 可以指点位数 */		

	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	update_widget_bg(pp->eventbox30[z], /*backpic[6]*/ 6);
	//	widget_window_class->key_press_event = window_keypress_event_orig;
	/* 一个信号能对应多个回调函数，所以先把对应的回调函数取消 */
	if (g_signal_handler_is_connected (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]))
		g_signal_handler_disconnect (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]);
	pp->signal_id[z] = g_signal_connect (G_OBJECT(pp->sbutton[z]), "value-changed", G_CALLBACK(fun), (gpointer) (pp));
	/* 设置值的范围 */
	pp->adj = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (pp->sbutton[z]));
	gtk_adjustment_configure (pp->adj, cur_value, lower, upper, step , 10.0, 0.0);
	gtk_spin_button_set_digits (GTK_SPIN_BUTTON (pp->sbutton[z]), digit);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_hide (pp->eventbox31[z]);
	gtk_widget_show (pp->sbutton[z]);
	gtk_widget_grab_focus (pp->sbutton[z]);
	/* 设置scale */
	gtk_range_set_adjustment (GTK_RANGE (pp->vscale), pp->adj);
	gtk_range_set_inverted (GTK_RANGE (pp->vscale), TRUE);
	gtk_scale_set_draw_value (GTK_SCALE (pp->vscale), FALSE);

	gtk_widget_hide (pp->scale_drawarea);
	gtk_widget_show (pp->button_add);
	gtk_widget_show (pp->button_sub);
	gtk_widget_show (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));

	if (str)
		g_free(str);

	change_keypress_event (KEYPRESS_MAIN_SPINBUTTON);
//	pp->pos_pos=MENU3_STOP;

	return ;
}

/* 
 *
 *
 *
 */
static void draw3_digit_stop(gfloat cur_value, const gchar *unit, 
		guint digit, guint pos, guint content_pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	if (g_signal_handler_is_connected (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]))
		g_signal_handler_disconnect (G_OBJECT (pp->sbutton[z]), pp->signal_id[z]);
	if (!unit )
		str = g_strdup_printf ("%s", con2_p[x][y][content_pos ? content_pos : pos]);	
	else
		str = g_strdup_printf ("%s\n%s", con2_p[x][y][content_pos ? content_pos : pos], unit);	

	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	gtk_label_set_justify (GTK_LABEL(pp->label3[z]),GTK_JUSTIFY_CENTER);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[8]*/ 8);
		update_widget_bg(pp->eventbox31[z], /*backpic[11]*/ 11);
	}
	else
	{
		update_widget_bg(pp->eventbox30[z], /*backpic[7]*/ 7);
		update_widget_bg(pp->eventbox31[z], /*backpic[10]*/ 10);
	}
	/* 更新当前增益值显示 */
	str = g_strdup_printf ("%0.*f", digit, cur_value);

	gtk_label_set_text (GTK_LABEL (pp->data3[z]), str);
	/*	gtk_label_set_text (GTK_LABEL (pp->label[1]), str);*/
	if (str)
		g_free(str);
	//	widget_window_class->key_press_event = my_keypress_event;

	gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[z]), cur_value);
	/* 显示和隐藏控件 */
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[z]), cur_value);
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	//	gtk_widget_hide (pp->dialog);
	//gtk_widget_set_size_request (GTK_WIDGET(pp->drawing_area), 658, 390);
	//gtk_widget_hide (pp->vscalebox);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
	//gtk_widget_hide (GTK_WIDGET (pp->entry));

	change_keypress_event (KEYPRESS_MAIN);

	return ;
	/*						gtk_widget_grab_focus (pp->button);*/
}

/*  */
static gboolean draw_info(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gfloat color_r = 0, color_g = 0, color_b = 0;
	gint i,j,m,n,w,h;
	cairo_t *cr;
	gfloat k;
	gchar *str = NULL;
    DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;

	gint offset,l;

	gint grp = get_current_group(pp->p_config);//当前group
	for (offset = 0, l = 0 ; l < grp; l++)
		offset += TMP(beam_qty[l]);
	gint index = offset + TMP(beam_num[grp]);	

	gtk_widget_get_size_request (widget, &w, &h);

/*
	if (prule->mask == 0x08)
    {
		w = w + 20;
    }
	else*/ if ((prule->mask == 0x06) || (prule->mask == 0x16))
	{
    	w = w + 30;
    }
	else if (prule->mask == 0x04)
    {
		w = w + 10;
    }
	else if (prule->mask == 0x02)
	{
    	w = w + 20;
    }
	else if ((prule->mask == 0x01) || (prule->mask == 0x11))
	{
    	h = h + 20;
    }


	if(h>400)
	{
		prule->hrule1_copies = 100;
		prule->hrule2_copies = 100;
	}
	else if ((h>300) && (h<400))
	{
		prule->hrule1_copies = 80;
		prule->hrule2_copies = 80;
	}
	else if ((h>150) && (h<300))
	{
		prule->hrule1_copies = 40;
		prule->hrule2_copies = 40;
	}
/*	else if ((h>100) && (h<150))
	{
		prule->hrule1_copies = 30;
		prule->hrule2_copies = 30;
	}*/
	else if ((h>50) && (h<150))
	{
		prule->hrule1_copies = 20;
		prule->hrule2_copies = 20;
	}
	else
	{
		prule->hrule1_copies = 0;
		prule->hrule2_copies = 0;
	}

	if(w>600)
		prule->wrule_copies = 100;
	else if((w>400)&&(w<600))
		prule->wrule_copies = 75;
	else if((w>299)&&(w<400))
		prule->wrule_copies = 50;
	else if((w>200)&&(w<300))
		prule->wrule_copies = 30;
	else if ((w>100)&&(w<200))
		prule->wrule_copies = 25;
	else
		prule->wrule_copies = 0;





	cr = gdk_cairo_create(widget->window);
	/* 网格 */
	cairo_set_line_width(cr, 1);
	switch(get_overlay_grid(pp->p_config))
	{
		case 0:
			cairo_set_source_rgba(cr,0.0,0.5,0.5,1.0);break;
		case 1:
			cairo_set_source_rgba(cr,0.3,0.5,0.0,1.0);break;
		case 2:
			cairo_set_source_rgba(cr,0.5,0.0,0.0,1.0);break;
		case 3:
			cairo_set_source_rgba(cr,0.5,0.5,0.0,1.0);break;
		case 4:
			cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);break;
		default:break;
	}

	if (get_overlay_grid(pp->p_config) != 5)
	{
		for (j = 0; j < (h-20); j += ((h-20) / 10.0))
		{
			for  (m = 20; m < (w-30); m += ((w-50) / 50.0))
			{
				cairo_move_to (cr, (int)(m) , (int)(j) + 0.5);
				cairo_line_to (cr, (int)(m) +1 , (int)(j) + 0.5);
				cairo_stroke (cr);
			}
		}
		for ( i = 20; i < (w-30); i += ((w-50) /10.0))
		{
			for (n = 0; n < (h-20); n += ((h-20) / 50.0))
			{
				cairo_move_to (cr, (int)(i) , (int)(n) + 0.5);
				cairo_line_to (cr, (int)(i) +1 , (int)(n) + 0.5);
				cairo_stroke(cr);
			}
		}
	}


	/* 调色条信息 */
if(!(prule->mask & 0x04))
{
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 1.0);
	cairo_rectangle (cr, w-30, 0, 10, h-20);
	cairo_fill (cr);
	for ( i = 21; i < h ; i++ )
	{
		switch (((DRAW_AREA_P)(data))->scan_type)
		{
			case	A_SCAN:
			case	A_SCAN_R:
			case	B_SCAN:
			case	S_SCAN:
			case	S_SCAN_A:
            case    S_SCAN_L:  // modified by shensheng for Linear S scan color bar is gray
				color_r = ((TMP(color_amp[i * 256 / h]) >> 11)) / 32.0;
				color_g = ((TMP(color_amp[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
				color_b = ((TMP(color_amp[i * 256 / h]) & 0x1f)) /  32.0;
				break;
			case	C_SCAN:
				switch (get_cscan_source(pp->p_config, 0))
				{
					case C_SCAN_A_HEIGHT:
					case C_SCAN_B_HEIGHT:
						color_r = ((TMP(color_amp[i * 256 / h]) >> 11)) / 32.0;
						color_g = ((TMP(color_amp[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
						color_b = ((TMP(color_amp[i * 256 / h]) & 0x1f)) /  32.0;
						break;
					case C_SCAN_THICKNESS:
					case C_SCAN_I_GATE:
						color_r = ((TMP(color_depth[i * 256 / h]) >> 11)) / 32.0;
						color_g = ((TMP(color_depth[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
						color_b = ((TMP(color_depth[i * 256 / h]) & 0x1f)) /  32.0;
						break;
					default:break;
				}
				break;
			case	CC_SCAN:
				switch (get_cscan_source(pp->p_config, 1))
				{
					case C_SCAN_A_HEIGHT:
					case C_SCAN_B_HEIGHT:
						color_r = ((TMP(color_amp[i * 256 / h]) >> 11)) / 32.0;
						color_g = ((TMP(color_amp[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
						color_b = ((TMP(color_amp[i * 256 / h]) & 0x1f)) /  32.0;
						break;
					case C_SCAN_THICKNESS:
					case C_SCAN_I_GATE:
						color_r = ((TMP(color_depth[i * 256 / h]) >> 11)) / 32.0;
						color_g = ((TMP(color_depth[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
						color_b = ((TMP(color_depth[i * 256 / h]) & 0x1f)) /  32.0;
						break;
					default:break;
				}
				break;
			case	CCC_SCAN:
				switch (get_cscan_source(pp->p_config, 0))
				{
					case C_SCAN_A_HEIGHT:
					case C_SCAN_B_HEIGHT:
						color_r = ((TMP(color_amp[i * 256 / h]) >> 11)) / 32.0;
						color_g = ((TMP(color_amp[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
						color_b = ((TMP(color_amp[i * 256 / h]) & 0x1f)) /  32.0;
						break;
					case C_SCAN_THICKNESS:
					case C_SCAN_I_GATE:
						color_r = ((TMP(color_depth[i * 256 / h]) >> 11)) / 32.0;
						color_g = ((TMP(color_depth[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
						color_b = ((TMP(color_depth[i * 256 / h]) & 0x1f)) /  32.0;
						break;
					default:break;
				}
				break;
			default:break;
		}
		cairo_set_source_rgba (cr, color_r, color_g, color_b, 1.0);
		cairo_move_to (cr, w - 29, h - i);
		cairo_line_to (cr, w - 21, h - i);
		cairo_stroke (cr);
	}
}
	/* 画 overlay */
	if (get_overlay_overlay(pp->p_config) == 1)
	{
		cairo_set_source_rgba(cr,1.0,1.0,1.0,1.0);

		switch (((DRAW_AREA_P)(data))->scan_type)
		{

			case A_SCAN:
			case B_SCAN:
				if((GROUP_VAL(ut_unit)==UT_UNIT_SOUNDPATH)||(GROUP_VAL(ut_unit)==UT_UNIT_TRUE_DEPTH))
				{
					for(i=1;i*(get_part_thickness(pp->p_config) / 1000.0) < (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); i++)
					{
						for(j=0;j<h-20;j+=5)
						{
						cairo_move_to (cr, 20+ (w-50)*(i*get_part_thickness(pp->p_config)/1000.0) / ((get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)), j);
						cairo_line_to (cr, 20+ (w-50)*(i*get_part_thickness(pp->p_config)/1000.0) / ((get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)), j+2.5);
						}
						cairo_stroke(cr);
					}
				}
				break;

			case S_SCAN:
			case S_SCAN_A:
			case S_SCAN_L:
				if(GROUP_VAL(ut_unit)==UT_UNIT_SOUNDPATH)
				{
					for(i=1;i*(get_part_thickness(pp->p_config) / 1000.0) < (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); i++)
					{
						for(j=0;j<h-20;j+=5)
						{
						cairo_move_to (cr, 20+ (w-50)*(i*get_part_thickness(pp->p_config)/1000.0) / ((get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)), j);
						cairo_line_to (cr, 20+ (w-50)*(i*get_part_thickness(pp->p_config)/1000.0) / ((get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)), j+2.5);
						}
						cairo_stroke(cr);
					}
				}
				else if(GROUP_VAL(ut_unit)==UT_UNIT_TRUE_DEPTH)
				{
					for(i=1;i*(get_part_thickness(pp->p_config) / 1000.0) < get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)+tan((LAW_VAL (Angle_min) / 100.0)*(3.14/180.0))*tan((LAW_VAL (Angle_min) / 100.0)*(3.14/180.0))*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0); i++)
					{
						for(j=20;j<w-30;j+=5)
						{
						cairo_move_to (cr, j, (h-20)*(i*get_part_thickness(pp->p_config)/1000.0) / ((get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)) );
						cairo_line_to (cr, j+2.5 , (h-20)*(i*get_part_thickness(pp->p_config)/1000.0) / ((get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)) );
						}
						cairo_stroke(cr);
					}
				}
				break;
			default:break;


		}
	}



	/* 画cursor */
	if (get_overlay_cursor(pp->p_config) == 1)
	{
		switch (((DRAW_AREA_P)(data))->scan_type)
		{
			case A_SCAN:
				/* 参考光标高度 ％(r) */
				cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
				cairo_move_to (cr, 20, (h-20)*(1.0-GROUP_VAL(per_reference)/10000.0));
				cairo_line_to (cr, w-30, (h-20)*(1.0-GROUP_VAL(per_reference)/10000.0));
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.93,0.945,0.412,1.0);/*显示当前值的黄色框*/
				cairo_rectangle(cr,21,(h-20)*(1.0-GROUP_VAL(per_reference)/10000.0)-14,25,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,23,(h-20)*(1.0-GROUP_VAL(per_reference)/10000.0)-4);
				str=g_strdup_printf("%.1f",GROUP_VAL(per_reference)/100.0);
				cairo_show_text(cr,str);
				cairo_stroke(cr);

				/* 测量光标高度 %(m) */
				cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
				cairo_move_to (cr, 20, (h-20)*(1.0-GROUP_VAL(per_measure)/10000.0));
				cairo_line_to (cr, w-30, (h-20)*(1.0-GROUP_VAL(per_measure)/10000.0));
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.93,0.945,0.412,1.0);/*显示当前值的黄色框*/
				cairo_rectangle(cr,21,(h-20)*(1.0-GROUP_VAL(per_measure)/10000.0)-14,25,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,23,(h-20)*(1.0-GROUP_VAL(per_measure)/10000.0)-4);
				str=g_strdup_printf("%.1f",GROUP_VAL(per_measure)/100.0);
				cairo_show_text(cr,str);
				cairo_stroke(cr);



				cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
				/* 当前值除以最大值再乘以窗口宽度*/
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+3,10);

				if(GROUP_VAL(ut_unit) == UT_UNIT_TIME)
					str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0);
				else
				{
					if(get_unit(pp->p_config)==UNIT_MM)
						str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					else
						str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
				}
				cairo_show_text(cr,str);
				cairo_stroke(cr);


				cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
				/* 当前值除以最大值再乘以窗口宽度 */
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+3,10);

				if(GROUP_VAL(ut_unit) == UT_UNIT_TIME)
					str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0);
				else
				{
					if(get_unit(pp->p_config)==UNIT_MM)
						str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					else
						str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
				}
				cairo_show_text(cr,str);
				cairo_stroke(cr);
				break;

		case B_SCAN:
				cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
				/* 当前值除以最大值再乘以窗口宽度*/
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+3,10);

				if(GROUP_VAL(ut_unit) == UT_UNIT_TIME)
					str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0);
				else
				{
					if(get_unit(pp->p_config)==UNIT_MM)
						str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					else
						str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
				}
				cairo_show_text(cr,str);
				cairo_stroke(cr);

				cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
				/* 当前值除以最大值再乘以窗口宽度 */
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+3,10);

				if(GROUP_VAL(ut_unit) == UT_UNIT_TIME)
					str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0);
				else
				{
					if(get_unit(pp->p_config)==UNIT_MM)
						str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					else
						str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
				}
				cairo_show_text(cr,str);
				cairo_stroke(cr);
				break;

			case C_SCAN:
				cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
				/* 当前值除以最大值再乘以窗口宽度*/
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(s_reference)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(s_reference)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(s_reference)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(s_reference)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0)+3,10);

				if(get_unit(pp->p_config)==UNIT_MM)
					str=g_strdup_printf("%.2f",GROUP_VAL(s_reference)/100.0);
				else
					str=g_strdup_printf("%.2f",GROUP_VAL(s_reference)/100.0 * 0.03937 );
				cairo_show_text(cr,str);
				cairo_stroke(cr);

				cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
				/* 当前值除以最大值再乘以窗口宽度 */
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(s_measure)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(s_measure)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(s_measure)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(s_measure)/100.0-GROUP_VAL(scan_offset)/10.0-get_area_scanstart (pp->p_config)/1000.0)/(get_area_scanend (pp->p_config)/1000.0-get_area_scanstart (pp->p_config)/1000.0)+3,10);
				if(get_unit(pp->p_config)==UNIT_MM)
					str=g_strdup_printf("%.2f",GROUP_VAL(s_measure)/100.0);
				else
					str=g_strdup_printf("%.2f",GROUP_VAL(s_measure)/100.0 * 0.03937 );
				cairo_show_text(cr,str);
				cairo_stroke(cr);
				break;

		case S_SCAN:
		case S_SCAN_A:
		case S_SCAN_L:

				if((GROUP_VAL(ut_unit)==UT_UNIT_SOUNDPATH)||(GROUP_VAL(ut_unit)==UT_UNIT_TIME))
				{
					cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
					/* 当前值除以最大值再乘以窗口宽度*/
					cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), 0);
					cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), h-20);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
					cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+1,0,30,13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
					cairo_move_to(cr,20+(w-50)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+3,10);
					if(GROUP_VAL(ut_unit)==UT_UNIT_TIME)
							str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0);
					else
							str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));

					cairo_show_text(cr,str);
					cairo_stroke(cr);


					cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
					/* 当前值除以最大值再乘以窗口宽度 */
					cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), 0);
					cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0), h-20);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
					cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+1,0,30,13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
					cairo_move_to(cr,20+(w-50)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)+3,10);
					if(GROUP_VAL(ut_unit)==UT_UNIT_TIME)
							str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0);
					else
							str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					cairo_show_text(cr,str);
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,0.0,0.0,1.0,1.0);/*蓝色cursor*/
					cairo_move_to (cr, 20, (h-20)*(1.0-(GROUP_VAL(cursors_angle)/100.0 - LAW_VAL(Angle_min)/100.0)/(LAW_VAL(Angle_max)/100.0-LAW_VAL(Angle_min)/100.0)) );
					cairo_line_to (cr, w-30, (h-20)*(1.0 - (GROUP_VAL(cursors_angle)/100.0 - LAW_VAL(Angle_min)/100.0)/(LAW_VAL(Angle_max)/100.0-LAW_VAL(Angle_min)/100.0)));
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.93,0.945,0.412,1.0);/*显示当前值的黄色框*/
					cairo_rectangle(cr,21,(h-20)*(1.0 - (GROUP_VAL(cursors_angle)/100.0 - LAW_VAL(Angle_min)/100.0)/(LAW_VAL(Angle_max)/100.0-LAW_VAL(Angle_min)/100.0))-14,25,13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
					cairo_move_to(cr,23,(h-20)*(1.0 - (GROUP_VAL(cursors_angle)/100.0 - LAW_VAL(Angle_min)/100.0)/(LAW_VAL(Angle_max)/100.0-LAW_VAL(Angle_min)/100.0))-4);
					str=g_strdup_printf("%.1f",GROUP_VAL(cursors_angle)/100.0);
					cairo_show_text(cr,str);
					cairo_stroke(cr);
				}
				else
				{
					cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
					/* 当前值除以最大值再乘以窗口宽度*/
					cairo_move_to (cr, 20, (h-20)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) );
					cairo_line_to (cr, w-30, (h-20)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) );
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
					cairo_rectangle(cr,21, (h-20)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)-14,30,13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
					cairo_move_to(cr,23, (h-20)*(GROUP_VAL(u_reference)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)-4 );
					if(get_unit(pp->p_config)==UNIT_MM)
						str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					else
						str=g_strdup_printf("%.2f",GROUP_VAL(u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					cairo_show_text(cr,str);
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
					/* 当前值除以最大值再乘以窗口宽度*/
					cairo_move_to (cr, 20, (h-20)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) );
					cairo_line_to (cr, w-30, (h-20)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0) );
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
					cairo_rectangle(cr,21, (h-20)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)-14,30,13);
					cairo_fill (cr);
					cairo_stroke (cr);
					cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
					cairo_move_to(cr,23, (h-20)*(GROUP_VAL(u_measure)/1000.0-get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0)/(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0)-4 );
					if(get_unit(pp->p_config)==UNIT_MM)
						str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					else
						str=g_strdup_printf("%.2f",GROUP_VAL(u_measure)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0));
					cairo_show_text(cr,str);
					cairo_stroke(cr);

				cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
				/* 当前值除以最大值再乘以窗口宽度*/
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(i_reference)/100.0-pp->swmin)/(pp->swmax - pp->swmin), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(i_reference)/100.0-pp->swmin)/(pp->swmax - pp->swmin), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(i_reference)/100.0-pp->swmin)/(pp->swmax - pp->swmin)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(i_reference)/100.0-pp->swmin)/(pp->swmax - pp->swmin)+3,10);
				str=g_strdup_printf("%.2f",GROUP_VAL(i_reference)/100.0);
				cairo_show_text(cr,str);
				cairo_stroke(cr);

				cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
				/* 当前值除以最大值再乘以窗口宽度 */
				cairo_move_to (cr, 20+(w-50)*(GROUP_VAL(i_measure)/100.0-pp->swmin)/(pp->swmax - pp->swmin), 0);
				cairo_line_to (cr, 20+(w-50)*(GROUP_VAL(i_measure)/100.0-pp->swmin)/(pp->swmax - pp->swmin), h-20);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
				cairo_rectangle(cr,20+(w-50)*(GROUP_VAL(i_measure)/100.0-pp->swmin)/(pp->swmax - pp->swmin)+1,0,30,13);
				cairo_fill (cr);
				cairo_stroke (cr);
				cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
				cairo_move_to(cr,20+(w-50)*(GROUP_VAL(i_measure)/100.0-pp->swmin)/(pp->swmax - pp->swmin)+3,10);
				str=g_strdup_printf("%.2f",GROUP_VAL(i_measure)/100.0);
				cairo_show_text(cr,str);
				cairo_stroke(cr);

				}
				break;
		}

	}



	/* 画闸门 gate */
	if(get_overlay_gate (pp->p_config) == 1)
	{
DO_NOT_USE_CCFG(measure_data[index]).i_position = 5;
DO_NOT_USE_CCFG(measure_data[index]).a_position = 10;
		cairo_save(cr);
		cairo_set_line_width(cr, 2);
		switch (((DRAW_AREA_P)(data))->scan_type)
		{
			case	A_SCAN:
			case	A_SCAN_R:

				if((get_display_pos (pp->p_config) == A_S_CC_SCAN ) && (get_cscan_source(pp->p_config, 0)==4) && (GROUP_VAL(ut_unit) == UT_UNIT_TRUE_DEPTH) && (get_display_group(pp->p_config) == DISPLAY_CURRENT_GROUP))	/*A_S scan 在 true depth时， A scan 需要旋转90度*/
				{
					cairo_set_source_rgba(cr,1.0,0.0,0.0,1.0);	/* A闸门为红色 */
					if(  GROUP_VAL_POS(prule->group,gate[0].synchro) == 0 )/* A闸门 pulse 时 */
					{
					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+20, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+20, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+17, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+23, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*A闸门上端点*/

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+17, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+23, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*A闸门下端点*/
					}
					else if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 1 )/* A闸门 I/ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+20, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+20, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+17, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+23, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*A闸门上端点*/

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+17, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(w-50)+23, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*A闸门下端点*/

					}
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,0.0,1.0,0.0,1.0);	/* B闸门为绿色 */
					if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 0 )/* B闸门 pulse 时 */
					{
					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+20, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+20, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+17, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+23, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*B闸门上端点*/

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+17, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+23, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*B闸门下端点*/
					}
					else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+20, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+20, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+17, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+23, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*B闸门上端点*/

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+17, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+23, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*B闸门下端点*/

					}
					else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 2 )/* B闸门 A^ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).a_position 为A闸门内波峰的位置，这里需要将其单位转化为ns来使用*/
					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+20, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+20, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+17, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+23, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*B闸门上端点*/

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+17, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(w-50)+23, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*B闸门下端点*/

					}
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,1.0,1.0,0.0,1.0);	/* I闸门为黄色 */
					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(w-50)+20, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(w-50)+20, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(w-50)+17, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(w-50)+23, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*I闸门上端点*/

					cairo_move_to(cr, (GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(w-50)+17, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
					cairo_line_to(cr, (GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(w-50)+23, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));/*I闸门下端点*/
					cairo_stroke(cr);
				}
				else	/*A Scan 需要旋转的情况除外*/
				{
					cairo_set_source_rgba(cr,1.0,0.0,0.0,1.0);	/* A闸门为红色 */
					if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 0 )/* A闸门 pulse 时 */
					{
					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20) );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20) );

					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)+3 );/*A闸门左端点*/

					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)+3 );/*A闸门右端点*/
					}
					else if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 1 )/* A闸门 I/ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/

					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20) );
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20) );

					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)+3 );
/*A闸门左端点*/
					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)-3);
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[0].height) / 100.0)*(h-20)+3);
/*A闸门右端点*/
					}
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,0.0,1.0,0.0,1.0);	/* B闸门为绿色 */
					if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 0 )/* B闸门 pulse 时 */
					{
					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20) );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20) );

					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)+3 );/*B闸门左端点*/

					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)+3 );/*B闸门右端点*/
					}
					else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20) );
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20) );

					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)+3 );
/*B闸门左端点*/
					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)-3);
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position + (GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)+3);
/*B闸门右端点*/

					}
					else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 2 )/* B闸门 A^ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).a_position 为A闸门内波峰的位置，这里需要将其单位转化为ns来使用*/
					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20) );
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20) );

					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)-3);
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)+3);/*B闸门左端点*/

					cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)-3);
					cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[1].height) / 100.0)*(h-20)+3);
/*B闸门右端点*/
					}
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,1.0,1.0,0.0,1.0);	/* C闸门为黄色 */
					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(h-20) );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(h-20) );

					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(h-20)+3 );/*C闸门左端点*/

					cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(h-20)-3 );
					cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,(1.0-GROUP_VAL_POS(prule->group,gate[2].height) / 100.0)*(h-20)+3 );/*C闸门右端点*/

					cairo_stroke(cr);
				}
					break;

			case	B_SCAN:
					cairo_set_source_rgba(cr,1.0,0.0,0.0,1.0);	/* A闸门为红色   虚线 纵向 */
					if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 0 )/* A闸门 pulse 时 */
					{
						for(j=0; j<h-20; j+=5)
						{
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );

						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
						}	
					}
					else if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 1 )/* A闸门 I/ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
						for(j=0; j<h-20; j+=5)
						{
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position +(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position +(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );

						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position +(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position +(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
						}
					}
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,0.0,1.0,0.0,1.0);	/* B闸门为绿色   虚线  纵向*/
					if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 0 )/* B闸门 pulse 时 */
					{
						for(j=0; j<h-20; j+=5)
						{
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
						}
					}
					else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
						for(j=0; j<h-20; j+=5)
						{
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
						}
					}
					else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 2 )/* B闸门 A^ 时 */
					{
/*DO_NOT_USE_CCFG(measure_data[index]).a_position 为A闸门内波峰的位置，这里需要将其单位转化为ns来使用*/
						for(j=0; j<h-20; j+=5)
						{
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
						}
					}
					cairo_stroke(cr);

					cairo_set_source_rgba(cr,1.0,1.0,0.0,1.0);	/* C闸门为黄色   虚线  纵向*/
					for(j=0; j<h-20; j+=5)
					{
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
					}
					cairo_stroke(cr);

					break;

			case	S_SCAN:
			case	S_SCAN_A:
			case	S_SCAN_L:
					if ((GROUP_VAL_POS(prule->group, ut_unit) == UT_UNIT_SOUNDPATH) || (GROUP_VAL_POS(prule->group, ut_unit) == UT_UNIT_TIME))
					{
						cairo_set_source_rgba(cr,1.0,0.0,0.0,1.0);	/* A闸门为红色   虚线 纵向*/
						if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 0 )/* A闸门 pulse 时 */
						{
							for(j=0; j<h-20; j+=5)
							{
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
							}
						}
						else if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 1 )/* A闸门 I/ 时 */
						{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
							for(j=0; j<h-20; j+=5)
							{
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
							}
						}
						cairo_stroke(cr);

						cairo_set_source_rgba(cr,0.0,1.0,0.0,1.0);	/* B闸门为绿色   虚线 纵向 */
						if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 0 )/* B闸门 pulse 时 */
						{
							for(j=0; j<h-20; j+=5)
							{
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
							}
						}
						else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
						{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
							for(j=0; j<h-20; j+=5)
							{
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
							}
						}
						else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 2 )/* B闸门 A^ 时 */
						{
/*DO_NOT_USE_CCFG(measure_data[index]).a_position 为A闸门内波峰的位置，这里需要将其单位转化为ns来使用*/
							for(j=0; j<h-20; j+=5)
							{
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
							}
						}
						cairo_stroke(cr);

						cairo_set_source_rgba(cr,1.0,1.0,0.0,1.0);	/* C闸门为黄色   虚线 纵向 */
						for(j=0; j<h-20; j+=5)
						{
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j );
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20, j+2.5 );
						cairo_move_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j);
						cairo_line_to(cr, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(w-50)+20,j+2.5);
						}
						cairo_stroke(cr);

					}
					else	/* TRUE_DEPTH */
					{
						cairo_set_source_rgba(cr,1.0,0.0,0.0,1.0);	/* A闸门为红色   虚线 横向*/
						if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 0 )/* A闸门 pulse 时 */
						{
							for(j=20; j<w-30; j+=5)
							{
						cairo_move_to(cr, j, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_move_to(cr, j, ((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5,((GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
							}
						}
						else if( GROUP_VAL_POS(prule->group,gate[0].synchro) == 1 )/* A闸门 I/ 时 */
						{
/*DO_NOT_USE_CCFG(measure_data[index]).i_position 为I闸门与波形的交点位置，这里需要将其单位转化为ns来使用*/
							for(j=20; j<w-30; j+=5)
							{
						cairo_move_to(cr, j, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_move_to(cr, j, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5,(DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[0].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[0].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
							}

						}
						cairo_stroke(cr);

						cairo_set_source_rgba(cr,0.0,1.0,0.0,1.0);	/* B闸门为绿色   虚线 横向*/
						if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 0 )/* B闸门 pulse 时 */
						{
							for(j=20; j<w-30; j+=5)
							{
						cairo_move_to(cr, j, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_move_to(cr, j, ((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5,((GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
							}
						}
						else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 1 )/* B闸门 I/ 时 */
						{
							for(j=20; j<w-30; j+=5)
							{
						cairo_move_to(cr, j, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_move_to(cr, j, (DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5,(DO_NOT_USE_CCFG(measure_data[index]).i_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
							}
						}
						else if( GROUP_VAL_POS(prule->group,gate[1].synchro) == 2 )/* B闸门 A^ 时 */
						{
							for(j=20; j<w-30; j+=5)
							{
						cairo_move_to(cr, j, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_move_to(cr, j, (DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5,(DO_NOT_USE_CCFG(measure_data[index]).a_position+(GROUP_VAL_POS(prule->group,gate[1].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[1].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
							}
						}
						cairo_stroke(cr);

						cairo_set_source_rgba(cr,1.0,1.0,0.0,1.0);	/* C闸门为黄色   虚线 横向*/
						for(j=20; j<w-30; j+=5)
						{
						cairo_move_to(cr, j, 
								((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0) - 
								 (get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)) / 
								(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_move_to(cr, j, ((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						cairo_line_to(cr, j+2.5,((GROUP_VAL_POS(prule->group,gate[2].start) / 1000.0)-(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_START) / 1000.0)+(GROUP_VAL_POS(prule->group,gate[2].width) / 1000.0))/(get_group_val (get_group_by_id (pp->p_config, prule->group), GROUP_RANGE) / 1000.0)*(h-20));
						}
						cairo_stroke(cr);

					}

					break;

			case	C_SCAN:
			case	CC_SCAN:
			case	CCC_SCAN:
					break;
		}

		cairo_restore(cr);

	}


	/* 画ruler */

	if (!(prule->mask & 0x08))
	{
	//cairo_set_source_rgba(cr,0.72,0.94,0.1,1.0);    /* hruler1颜色为绿色 */

	cairo_set_source_rgba(cr,((prule->h1_color)>>16)/255.0, (((prule->h1_color)>>8)&0x00FF)/255.0, ((prule->h1_color)&0xFF)/255.0, 1.0);
    //cairo_set_source_rgba(cr , 1.0, 1.0 , 1.0 , 1.0);
	cairo_rectangle(cr,0.0,0.0,20.0,h-20);		/* hruler1 */
	cairo_fill (cr);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_set_font_size(cr,8);

	cairo_save (cr);				/* 保存当前画笔 */
	if(prule->hmax1 > prule->hmin1)
	{
		cairo_move_to (cr, 12, (h-40));	/*hruler1 单位位置(底端) ％*/
		cairo_rotate (cr, G_PI * 3/2);
		cairo_show_text (cr, units[prule->h1_unit]);
	}
	else
	{
		cairo_move_to (cr, 12, 20);	/*hruler1 单位位置（顶端） ％*/
		cairo_rotate (cr, G_PI * 3/2);
		cairo_show_text (cr, units[prule->h1_unit]);
	}
	cairo_restore (cr);

	i=0;
	//for( k=0;k<(h-20.0);k=k+(h-20.0)/prule->hrule1_copies)	/* hruler1 标尺 */
	for( k=h-20.0;k>0;k=k-(h-20.0)/prule->hrule1_copies)	/* hruler1 标尺 */
	{
		if(i%10 == 0)
		{
			cairo_save(cr);/* 保存当前画笔 */
			cairo_move_to(cr,5,(int)k+0.5);
			cairo_line_to(cr,20,(int)k+0.5);
			cairo_stroke(cr);
			cairo_move_to(cr,12,(int)k-3.5);
			cairo_rotate(cr, G_PI * 3/2);		/*旋转270度*/
			if(prule->hmax1 > prule->hmin1)
				str=g_strdup_printf("%.*f",prule->h1_bit,  (float)((prule->hmax1-prule->hmin1)*i/(prule->hrule1_copies)+prule->hmin1) );
			else
				str=g_strdup_printf("%.*f",prule->h1_bit,  prule->hmin1-(float)(prule->hmin1-prule->hmax1)*i/(prule->hrule1_copies) );
			cairo_show_text(cr,str); 
			cairo_restore(cr);/* 恢复当前画笔 */
		}
		else if(i%5 == 0)
		{

			cairo_move_to(cr,10,(int)k+0.5);
			cairo_line_to(cr,20,(int)k+0.5);
			cairo_stroke(cr);
		}
		else
		{
			cairo_move_to(cr,15,(int)k+0.5);
			cairo_line_to(cr,20,(int)k+0.5);
			cairo_stroke(cr);
		}

		i++;
	}
	}


	if (!(prule->mask & 0x01))
	{
	//cairo_set_source_rgba(cr,0.72,0.94,0.1,1.0);    /* wruler颜色为绿色 */
	cairo_set_source_rgba(cr,((prule->w_color)>>16)/255.0, (((prule->w_color)>>8)&0x00FF)/255.0, ((prule->w_color)&0xFF)/255.0, 1.0);
	cairo_rectangle(cr,20.0,h-20,w-50,20.0);	/* wruler */
	cairo_fill (cr);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_set_font_size(cr,8);
	cairo_move_to(cr,50,(h-5));/*hruler 单位 mm*/
	//cairo_show_text(cr,data->w_unit);
	//cairo_show_text(cr,"mm");
	cairo_show_text(cr,units[prule->w_unit]);

	i=0;
	for( k=0;k<w-50;k=k+(w-50.0)/prule->wrule_copies )	/* wruler 标尺 */
	{

		if(i%10 == 0)
		{
			cairo_move_to(cr,20+(int)k+0.5,h-20);
			cairo_line_to(cr,20+(int)k+0.5,h-5);
			cairo_stroke(cr);
			cairo_move_to(cr,23.5+(int)k,h-5);
			//		cairo_set_font_size(cr, 7);		/*设置字体大小*/
			//		cairo_translate(cr,250,0);		/*平移*/
			//		cairo_rotate(cr, G_PI*3/2);		/*旋转270度*/
			//str=g_strdup_printf("%d",prule->wmin1 + i);
			str=g_strdup_printf("%.2f",prule->wmin1 + (float)(prule->wmax1-prule->wmin1)*i/(prule->wrule_copies));
			cairo_show_text(cr,str);   		/*标签*/
			//		cairo_show_text(cr,"mm");   		/*标签*/
		}
		else if(i%5 == 0)
		{
			cairo_move_to(cr,20+(int)k+0.5,h-20);
			cairo_line_to(cr,20+(int)k+0.5,h-10);
			cairo_stroke(cr);
		}
		else
		{
			cairo_move_to(cr,20+(int)k+0.5,h-20);
			cairo_line_to(cr,20+(int)k+0.5,h-15);
			cairo_stroke(cr);
		}
		i++;
	}
	}


	if (!(prule->mask & 0x02))
	{

	//cairo_set_source_rgba(cr,0.72,0.94,0.1,1.0);    /* hruler2颜色为绿色 */
	cairo_set_source_rgba(cr,((prule->h2_color)>>16)/255.0, (((prule->h2_color)>>8)&0x00FF)/255.0, ((prule->h2_color)&0xFF)/255.0, 1.0);
	cairo_rectangle(cr,w-20.0,0.0,20.0,h-20.0);	/* hruler2 */
	cairo_fill (cr);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_set_font_size(cr,8);
	cairo_save(cr);
	cairo_move_to(cr,(w-5),(h-30));/*hruler2 单位 ％*/
	cairo_rotate(cr, G_PI*3/2);
	//cairo_show_text(cr,"%");
	cairo_show_text(cr,units[prule->h2_unit]);
	cairo_restore(cr);

		i=0;
		//for( k=0;k<h-20;k=k+(h-20.0)/prule->hrule2_copies )	/* hruler2 标尺 */
		for( k=h-20.0;k>0;k=k-(h-20.0)/prule->hrule2_copies)	/* hruler2 标尺 */
		{
			if(i%10 == 0)
			{
			cairo_save(cr);
			cairo_move_to(cr,w-20,(int)k+0.5);
			cairo_line_to(cr,w-5,(int)k+0.5);
			cairo_stroke(cr);
			cairo_move_to(cr,w-7,(int)k-2.5);
			//		cairo_set_font_size(cr, 7);		/*设置字体大小*/
			//		cairo_translate(cr,250,0);		/*平移*/
			cairo_rotate(cr, G_PI * 3/2);		/*旋转270度*/
			//		cairo_show_text(cr,"mm");   		/*标签*/
			//str=g_strdup_printf("%d",(prule->hmax2-i));
			str=g_strdup_printf("%.0f",(float)(prule->hmax2-prule->hmin2)*i/(prule->hrule2_copies) );
			cairo_show_text(cr,str);   		/*标签*/
			cairo_restore(cr);
			}
			else if(i%5 == 0)
			{
			cairo_move_to(cr,w-20,(int)k+0.5);
			cairo_line_to(cr,w-10,(int)k+0.5);
			cairo_stroke(cr);
			}
			else
			{
			cairo_move_to(cr,w-20,(int)k+0.5);
			cairo_line_to(cr,w-15,(int)k+0.5);
			cairo_stroke(cr);
			}
			i++;
		}
	}


	if (GROUP_VAL(curve_pos) && get_display_group(pp->p_config))
	{
		switch (get_display_pos(pp->p_config))
		{
			case A_SCAN:
				draw_dac_tcg_curve(cr,605,390);
				break;
			case B_SCAN:
                break;
			case C_SCAN:
				break;
			case S_SCAN:
				break;
			case A_B_SCAN:
				//draw_dac_tcg_curve();
				break;
			case A_B_C_SCAN:
				break;
			case A_B_S_SCAN:
				break;
			case A_C_CC_SCAN:
				break;
			case A_S_CC_SCAN:
				break;
			case Strip_Chart_AA:
				break;
			default:
				break;
		}
	}

	g_free(str);
	cairo_destroy(cr);
	return TRUE;
}

/* 画电池等信息 */
static gboolean draw_other_info (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{

	gint  y1 = 3;
	gint  y2 = 23;

	cairo_t *cr;        //声明一支画笔
	cr = gdk_cairo_create(widget->window);//创建画笔
	cairo_set_line_width(cr, 2);

	/* 清空背景 */
	cairo_rectangle (cr , 0, 0, 115, 65);
	cairo_fill (cr);

	/* 第一个电池 需要根据电量来改变颜色 */
	cairo_set_source_rgba (cr, 1, 1, 1, 1);

	cairo_move_to (cr, 0, 3);
	cairo_line_to (cr, 50, 3);

	cairo_move_to (cr, 0, 3);
	cairo_line_to (cr, 0, 18);

	cairo_move_to (cr, 0, 18);
	cairo_line_to (cr, 50, 18);

	cairo_move_to (cr, 50, 3);
	cairo_line_to (cr, 50, 18);

	cairo_move_to (cr, 50, 8);
	cairo_line_to (cr, 55, 8);

	cairo_move_to (cr, 50, 13);
	cairo_line_to (cr, 55, 13);

	cairo_move_to (cr, 55, 8);
	cairo_line_to (cr, 55, 13);

	cairo_stroke (cr);

	/* 第二个电池 */
	cairo_move_to (cr, 0, y2);
	cairo_line_to (cr, 50, y2);

	cairo_move_to (cr, 0, y2);
	cairo_line_to (cr, 0, y2 + 15);

	cairo_move_to (cr, 0, y2 +15);
	cairo_line_to (cr, 50, y2 + 15);

	cairo_move_to (cr, 50, y2);
	cairo_line_to (cr, 50, y2 +15);

	cairo_move_to (cr, 50, y2 + 5);
	cairo_line_to (cr, 55, y2 + 5);

	cairo_move_to (cr, 50, y2 + 10);
	cairo_line_to (cr, 55, y2 + 10);

	cairo_move_to (cr, 55, y2 + 5);
	cairo_line_to (cr, 55, y2 + 10);

	/* 更新电池信息*/ 
	switch(pp->battery.flag1)//电池1
	{
		case 0x00://没连接
			break;
		case 0x01://放电
			//显示剩余电量
			cairo_set_source_rgba (cr, 0.3, 0.3, 0.3, 1);
			cairo_rectangle (cr , 0, y1, 50*(pp->battery.power1), 15);
			cairo_fill (cr);
			break;
		case 0x02://充电
			// 显示充电标志

			cairo_set_source_rgba (cr, 0.3, 0.3, 0.3, 1);
			cairo_rectangle (cr , 0, y1, 50*(pp->battery.power1), 15);
			cairo_fill (cr);
			break;
		default :
			break;
	}

	cairo_stroke (cr);
	cairo_destroy(cr);//销毁画笔
	return TRUE;
}

gboolean progress_button_release (GtkWidget *progress, GdkEvent *event, gpointer data)
{
	GdkEventButton *eb = (GdkEventButton *)event;
	GtkAllocation allocation;
	gint pressed;

	pressed = (gint)g_object_get_data (G_OBJECT(progress), "pressed");
#if GTK_MINOR_VERSION >= 18
	gtk_widget_get_allocation (progress, &allocation);
#else
	allocation = progress->allocation;
#endif
	if (pressed)
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(progress), eb->x / (allocation.width * 1.0));

	g_object_set_data (G_OBJECT(progress), "pressed", (gpointer)0);
	return FALSE;
}

gboolean progress_button_press (GtkWidget *progress, GdkEvent *event, gpointer data)
{
	g_object_set_data (G_OBJECT(progress), "pressed", (gpointer)1);
	return FALSE;
}

gboolean progress_motion_notify (GtkWidget *progress, GdkEvent *event, gpointer data)
{
	gint x, y, tmp;
	guint tmp2;
	gint pressed;
	Window fromroot, tmpwin;
	Window root;
	root = DefaultRootWindow(pp->disp);
	XQueryPointer(pp->disp, root, &fromroot, &tmpwin, &x, &y, &tmp, &tmp, &tmp2);


	pressed = (gint)g_object_get_data (G_OBJECT(progress), "pressed");
	if (pressed)
	{
		GROUP_GATE_POS(height) = (390 - (y - 115)) / 3.90 ;
		gtk_widget_queue_draw (progress);
	}

	return FALSE;
}

#if 0
static void draw_area(GtkWidget *parent_box, DRAW_AREA *draw_area, guint width, guint height,
		const gchar *title, 
		gdouble v1s, gdouble v1e,
		gdouble v2s, gdouble v2e, 
		gdouble h1s, gdouble h1e, 
		gpointer *other)
{
	draw_area->vbox = gtk_vbox_new (FALSE, 0);
	/* 窗口名字 */
	draw_area->ebox = gtk_event_box_new ();
	gtk_widget_modify_bg (draw_area->ebox, GTK_STATE_NORMAL, &color_text_base);
	draw_area->label = gtk_label_new (title);
	gtk_widget_modify_fg (draw_area->label, GTK_STATE_NORMAL, &color_white);
	gtk_container_add (GTK_CONTAINER (draw_area->ebox), draw_area->label);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->ebox, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET (draw_area->ebox), width, 15);

	/* 3个刻度尺 1个画图区 */
	draw_area->drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(draw_area->drawing_area), width, height-15);
	gtk_widget_modify_bg (draw_area->drawing_area, GTK_STATE_NORMAL, &color_black1);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->drawing_area, FALSE, FALSE, 0);
	/* 调用 draw_info 函数 */
	g_signal_connect (G_OBJECT (draw_area->drawing_area), "expose-event",
			G_CALLBACK (draw_info), (gpointer)(draw_area));
	//	gtk_widget_set_events (draw_area->drawing_area, GDK_POINTER_MOTION_MASK |
	//			GDK_POINTER_MOTION_HINT_MASK);
#if 0	
	gtk_widget_add_events (draw_area->drawing_area, GDK_BUTTON_RELEASE_MASK | 
			GDK_BUTTON_PRESS_MASK   |
			GDK_POINTER_MOTION_MASK );
	//	g_signal_connect_swapped ((draw_area->drawing_area), "motion-notify-event",
	//			G_CALLBACK (EVENT_METHOD ((draw_area->vruler1), motion_notify_event)),
	//			draw_area->vruler1);
	g_signal_connect ((draw_area->drawing_area), "button-release-event", 
			G_CALLBACK (progress_button_release), NULL);
	g_signal_connect ((draw_area->drawing_area), "button-press-event", 
			G_CALLBACK (progress_button_press), NULL);
	g_signal_connect ((draw_area->drawing_area), "motion-notify-event", 
			G_CALLBACK (progress_motion_notify), NULL);
#endif

	gtk_widget_show_all(draw_area->vbox);

	gtk_box_pack_start (GTK_BOX (parent_box), draw_area->vbox, FALSE, FALSE, 0);
	return ;
}
#endif

static void draw_area_(GtkWidget *parent_box, DRAW_AREA *draw_area, guint width, guint height)
{
	GdkColor text_base	= {0x0, 0x1300, 0x4900, 0x7600};
	GdkColor _white = {0x0, 0xffff, 0xffff, 0xffff};
	GdkColor _black	= {0x0, 0x0, 0x0, 0x0800};
	draw_area->width = width;
	draw_area->height = height;

	draw_area->vbox = gtk_vbox_new (FALSE, 0);
	/* 窗口名字 */
if(!(draw_area->mask & 0x10))
{
	draw_area->ebox = gtk_event_box_new ();
	gtk_widget_modify_bg (draw_area->ebox, GTK_STATE_NORMAL, &text_base);
	draw_area->label = gtk_label_new (draw_area->title);
	gtk_widget_modify_fg (draw_area->label, GTK_STATE_NORMAL, &_white);
	gtk_container_add (GTK_CONTAINER (draw_area->ebox), draw_area->label);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->ebox, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET (draw_area->ebox), width, 15);
	draw_area->drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(draw_area->drawing_area), width, height-15);
}
	/* 3个刻度尺 1个画图区 */
else
{
	draw_area->drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(draw_area->drawing_area), width, height);
}
	gtk_widget_modify_bg (draw_area->drawing_area, GTK_STATE_NORMAL, &_black);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->drawing_area, FALSE, FALSE, 0);
	/* 调用 draw_info 函数 */
	g_signal_connect (G_OBJECT (draw_area->drawing_area), "expose-event",
			G_CALLBACK (draw_info), (gpointer)(draw_area));
#if 0
	gtk_widget_add_events (draw_area->drawing_area, GDK_BUTTON_RELEASE_MASK | 
			GDK_BUTTON_PRESS_MASK |	GDK_POINTER_MOTION_MASK );

	g_signal_connect ((draw_area->drawing_area), "button-release-event", 
			G_CALLBACK (progress_button_release), NULL);
	g_signal_connect ((draw_area->drawing_area), "button-press-event", 
			G_CALLBACK (progress_button_press), NULL);
	g_signal_connect ((draw_area->drawing_area), "motion-notify-event", 
			G_CALLBACK (progress_motion_notify), NULL);
#endif

	gtk_widget_show_all(draw_area->vbox);

	gtk_box_pack_start (GTK_BOX (parent_box), draw_area->vbox, FALSE, FALSE, 0);
	return ;
}

/*
 * scan_num 第几个窗口0-15,
 * scan_type ABSC 
 * w h 宽高
 * xoff yoff 偏移
 * group 哪个组
 *  
 */
static inline void set_scan_config (guchar scan_num,guchar scan_type, guint aw, guint w, guint h, 
		guint xoff, guint yoff, guchar group)
{

	TMP(scan_type[scan_num])	=	scan_type;
	TMP(scan_xpos[scan_num])	=	xoff;
	TMP(scan_ypos[scan_num])	=	yoff;
	TMP(scan_group[scan_num])	=	group;
	TMP(a_scan_dot_qty)			=	aw;
	switch (scan_type)
	{
		case A_SCAN:
		case A_SCAN_R:
			TMP(a_scan_width)	=	w;
			TMP(a_scan_height)	=	h;
			break;
		case B_SCAN:
			TMP(b_scan_width)	=	w;
			TMP(b_scan_height)	=	h;
			break;
		case C_SCAN:
		case CC_SCAN:
		case CCC_SCAN:
			TMP(c_scan_width)	=	w;
			TMP(c_scan_height)	=	h;
			break;
		case S_SCAN:
		case S_SCAN_A:
		case S_SCAN_L:
			TMP(s_scan_width)	=	w;
			TMP(s_scan_height)	=	h;
			break;
		default:break;
	}
	return ;
}

/*
 * 修改波形区的 刻度尺 标签的信息 
 * mask 后面3为是表示左右下3个方向的刻度尺 存在否
 *
 */
void set_drawarea_property( DRAW_AREA *p, guint type, guint mask)
{
	gint angle = 0, num = 0;
	gfloat mid = 0.0, middle = 0.0;


	p->scan_type = type;

	/* mask = 二进制10000 则不显示 label */
	/* mask = 二进制01000 则不显示 hruler1 */
	/* mask = 二进制00100 则不显示 调色条 */
	/* mask = 二进制00010 则不显示 hruler2 */
	/* mask = 二进制00001 则不显示 wruler */
	p->mask = mask;

	switch (LAW_VAL(Focal_type))
	{
		case AZIMUTHAL_SCAN:
			num = TMP(beam_num[get_current_group(pp->p_config)]);
			angle = LAW_VAL (Angle_min) + LAW_VAL (Angle_step)*num;
			break;
		case LINEAR_SCAN:
		case DEPTH_SCAN:
		case STATIC_SCAN:
			angle = LAW_VAL (Angle_min);
			num = TMP(beam_num[get_current_group(pp->p_config)]);
		default:break;
	}
	switch (type)
	{
		case A_SCAN:
		case A_SCAN_R:

			if((get_display_pos (pp->p_config) == A_S_CC_SCAN ) && (get_cscan_source(pp->p_config, 0)==4) && (GROUP_VAL_POS(p->group, ut_unit) == UT_UNIT_TRUE_DEPTH) && (get_display_group(pp->p_config) == DISPLAY_CURRENT_GROUP))
			{	/*A_S scan 在 true depth时， A scan 需要旋转90度*/
				if(get_unit(pp->p_config) == UNIT_MM) /* hrule1 */
				{
					p->hmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->hmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->h1_unit = UNIT_MM;
					p->h1_color = 0xD6ABF1;/*紫色*/

				}
				else
				{
					p->hmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->hmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->h1_unit = UNIT_INCH;
					p->h1_color = 0xD6ABF1;/*紫色*/
				}
				p->h1_bit = 2;

				p->wmin1 = 0;
				p->wmax1 = 100;
				p->w_unit = UNIT_BFH;
				p->w_color = 0xEDF169;	/*黄色*/

				p->hmin2 = 0;
				p->hmax2 = 100;
				p->h2_unit = UNIT_BFH;
				p->h2_color = 0xEDF169;	/*黄色*/
			}
			else /*除了A scan 需要旋转的那种情况以外*/
			{
				p->hmin1 = 0;
				p->hmin2 = 0;
				p->h1_unit = UNIT_BFH;
				p->h1_color = 0xEDF169;	/*黄色*/
				p->h1_bit = 0;
				p->hmax1 = 100;
				p->hmax2 = 100;
				p->h2_unit = UNIT_BFH;
				p->h2_color = 0xEDF169;	/*黄色*/

				if(GROUP_VAL_POS(p->group, ut_unit) == UT_UNIT_TIME)	/* wrule */	/*淡黄色*/
				{
					p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0;
					p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0;
					p->w_unit = UNIT_US;
					p->w_color = 0xF8EAC4;
				}
				else
				{
					if(get_unit(pp->p_config) == UNIT_MM)
					{
						p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
						p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
						p->w_unit = UNIT_MM;
					}
					else
					{
						p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
						p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
						p->w_unit = UNIT_INCH;
					}
					if(GROUP_VAL_POS(p->group, ut_unit) == UT_UNIT_TRUE_DEPTH)
						p->w_color = 0xD6ABF1;	/* 紫色 */
					else
						p->w_color = 0xF49CD6;	/* 粉色 */
				}
			}
			g_sprintf (p->title, "A scan|Gr %d|CH %0.1f|SK%0.1f|L%d", 
					get_current_group(pp->p_config) + 1, angle / 100.0, GROUP_VAL_POS(p->group, skew) / 100.0, num + 1);
			break;
		//case A_SCAN_R:
			//break;
		case B_SCAN:
			/* hrule1 */
			if(get_inspec_source (pp->p_config)==0)	/* scan -> time */
			{
				p->hmin1 = 0;
				p->hmax1 = 415 * 10.0/get_prf();
				p->h1_unit = UNIT_S;
				p->h1_color = 0xCCD9D5;	/*浅灰色*/
				p->h1_bit = 1;
			}
			else		/* scan -> Encoder1/Encoder2  */
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->hmin1 = GROUP_VAL_POS(p->group, scan_offset)/10.0 + get_area_scanstart (pp->p_config)/1000.0;
					p->hmax1 = GROUP_VAL_POS(p->group, scan_offset)/10.0 + get_area_scanend (pp->p_config)/1000.0;
					p->h1_unit = UNIT_MM;
					p->h1_color = 0x0AD5D3;
					p->h1_bit = 0;
				}
				else
				{
					p->hmin1 = GROUP_VAL_POS(p->group, scan_offset)/10.0*0.03937 + get_area_scanstart (pp->p_config)/1000.0*0.03937;
					p->hmax1 = GROUP_VAL_POS(p->group, scan_offset)/10.0*0.03937 + get_area_scanend (pp->p_config)/1000.0*0.03937;
					p->h1_unit = UNIT_INCH;
					p->h1_color = 0x0AD5D3;	/*深绿色*/
					p->h1_bit = 0;
				}
			}

			p->hmin2 = 0;
			p->hmax2 = 100;
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;	/*黄色*/

			if(GROUP_VAL_POS(p->group, ut_unit) == UT_UNIT_TIME)	/* wrule */
			{
				p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0;
				p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0;
				p->w_unit = UNIT_US;
				p->w_color = 0xF8EAC4;	/*浅黄色*/
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->w_unit = UNIT_MM;
				}
				else
				{
					p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->w_unit = UNIT_INCH;
				}
				if(GROUP_VAL_POS(p->group, ut_unit) == UT_UNIT_TRUE_DEPTH)
					p->w_color = 0xD6ABF1;	/* 紫色 */
				else
					p->w_color = 0xF49CD6;	/* 粉色 */
			}

			g_sprintf (p->title, "B scan|Gr %d|CH %0.1f|SK%0.1f|L%d", 
					get_current_group(pp->p_config) + 1, angle / 100.0, GROUP_VAL_POS(p->group, skew) / 100.0, num + 1);
			break;
		case C_SCAN:
		case CC_SCAN:
		case CCC_SCAN:
			if ((get_inspec_type (pp->p_config) == 0) && (LAW_VAL(Focal_type)==AZIMUTHAL_SCAN))
			{
				//if(LAW_VAL(Focal_type)==AZIMUTHAL_SCAN)	/* hrule1 *//*灰色*/
				//{
					if(LAW_VAL (Angle_min) == LAW_VAL(Angle_max) )
					{
						p->hmin1 = LAW_VAL_POS (p->group, Angle_min) / 100.0;
						p->hmax1 = LAW_VAL_POS (p->group, Angle_max) / 100.0 + 1.0;
					}
					else
					{
						p->hmin1 = LAW_VAL_POS (p->group, Angle_min) / 100.0;
						p->hmax1 = LAW_VAL_POS (p->group, Angle_max) / 100.0;
					}
					p->h1_unit = UNIT_DEG;
					p->h1_color = 0xB2C1C1;
					p->h1_bit = 1;
				//}
			}

			//else if(LAW_VAL(Focal_type)==LINEAR_SCAN) /*浅绿色0xADFBE3*/
			else
			{
				if(LAW_VAL (Angle_min) == LAW_VAL(Angle_max) )
				{
					p->hmin1 = GROUP_VAL_POS(p->group, index_offset)/10.0 + get_area_indexstart (pp->p_config) / 1000.0;
					p->hmax1 = GROUP_VAL_POS(p->group, index_offset)/10.0 + get_area_indexend (pp->p_config) / 1000.0 + 0.5;
				}
				else
				{
					p->hmin1 = GROUP_VAL_POS(p->group, index_offset)/10.0 + get_area_indexstart (pp->p_config) / 1000.0;
					p->hmax1 = GROUP_VAL_POS(p->group, index_offset)/10.0 + get_area_indexend (pp->p_config) / 1000.0 ;
				}
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					p->h1_unit = UNIT_MM;
				}
				else
				{
					p->h1_unit = UNIT_INCH;
					p->hmin1  = p->hmin1 * 0.03937;
					p->hmax1  = p->hmax1 * 0.03937;
				}

				p->h1_color = 0xADFBE3;
				p->h1_bit = 1;
			}

			if((get_cscan_source(pp->p_config, 0)==0) || (get_cscan_source(pp->p_config, 0)==1))	/* hrule2 */
			{
				p->hmin2 = 0;
				p->hmax2 = 100;
				p->h2_unit = UNIT_BFH;
				p->h2_color = 0xEDF169;	/*黄色*/
			}

			else
			{
				p->hmin2 = 0;
				p->hmax2 = 100;
				p->h2_unit = UNIT_MM;
				p->h2_color = 0xD6ABF1;	/*紫色*/
			}

			if(get_unit(pp->p_config) == UNIT_MM)	/* wrule */		/* 深绿色 */
			{
				p->wmin1 = GROUP_VAL_POS(p->group, scan_offset)/10.0 + get_area_scanstart (pp->p_config)/1000.0;
				p->wmax1 = GROUP_VAL_POS(p->group, scan_offset)/10.0 + get_area_scanend (pp->p_config)/1000.0;
				p->w_unit = UNIT_MM;
				p->w_color = 0x0AD5D3;
			}
			else
			{
				p->wmin1 = GROUP_VAL_POS(p->group, scan_offset)/10.0*0.03937 + get_area_scanstart (pp->p_config)/1000.0*0.03937;
				p->wmax1 = GROUP_VAL_POS(p->group, scan_offset)/10.0*0.03937 + get_area_scanend (pp->p_config)/1000.0*0.03937;
				p->w_unit = UNIT_INCH;
				p->w_color = 0x0AD5D3;
			}

			g_sprintf (p->title, "C scan|Gr %d|CH %0.1f|SK%0.1f|L%d", 
					get_current_group(pp->p_config) + 1, angle / 100.0, GROUP_VAL_POS(p->group, skew) / 100.0, num + 1);
			break;
		case S_SCAN:
		case S_SCAN_A:
		case S_SCAN_L:
			if(GROUP_VAL_POS(p->group, ut_unit)==UT_UNIT_SOUNDPATH)
			{
				if(LAW_VAL (Angle_min) == LAW_VAL(Angle_max) )	/*hrule1*/	/* 灰色 */
				{
					p->hmin1 = LAW_VAL_POS (p->group, Angle_min) / 100.0;
					p->hmax1 = LAW_VAL_POS (p->group, Angle_max) / 100.0 + 1.0;
				}
				else
				{
					p->hmin1 = LAW_VAL_POS (p->group, Angle_min) / 100.0;
					p->hmax1 = LAW_VAL_POS (p->group, Angle_max) / 100.0 ;
				}
				p->h1_unit = UNIT_DEG;
				p->h1_color = 0xB2C1C1;
				p->h1_bit = 1;



				if(get_unit(pp->p_config) == UNIT_MM)	/*wrule*/
				{
					p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->w_unit = UNIT_MM;
					p->w_color = 0xF49CD6; /*粉色*/
				}
				else
				{
					p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->w_unit = UNIT_INCH;
					p->w_color = 0xF49CD6; /*粉色*/
				}
			}
			else if(GROUP_VAL_POS(p->group, ut_unit) == UT_UNIT_TIME)
			{
				if(LAW_VAL (Angle_min) == LAW_VAL(Angle_max) )	/*hrule1*/
				{
					p->hmin1 = LAW_VAL_POS (p->group, Angle_min) / 100.0;
					p->hmax1 = LAW_VAL_POS (p->group, Angle_max) / 100.0 + 1.0;
				}
				else
				{
					p->hmin1 = LAW_VAL_POS (p->group, Angle_min) / 100.0;
					p->hmax1 = LAW_VAL_POS (p->group, Angle_max) / 100.0 ;
				}
				p->h1_unit = UNIT_DEG;
				p->h1_color = 0xB2C1C1;	/*灰色*/
				p->h1_bit = 1;

				p->wmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0;	/*wrule*/
				p->wmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0 + get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0;
				p->w_unit = UNIT_US;
				p->w_color =0xF8EAC4; /*浅黄色*/

			}

			else if(GROUP_VAL_POS(p->group, ut_unit) == UT_UNIT_TRUE_DEPTH)
			{
				if(get_unit(pp->p_config) == UNIT_MM) /* hrule1 */
				{
					p->hmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+tan((LAW_VAL_POS (p->group, Angle_min) / 100.0)*(3.14/180.0))*tan((LAW_VAL_POS (p->group, Angle_min) / 100.0)*(3.14/180.0))*get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->hmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->h1_unit = UNIT_MM;
					p->h1_color = 0xD6ABF1;/*紫色*/

				}
				else
				{
					p->hmin1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->hmax1 = get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_START)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_VELOCITY)/200000.0);
					p->h1_unit = UNIT_INCH;
					p->h1_color = 0xD6ABF1;/*紫色*/
				}
				p->h1_bit = 2;

				//p->wmin1 = 0;	/*wrule1*/
				/*if(wedge == contact)
					mid = pp->probe_primary_offset - (pp->element_qty * pp->element_pitch)/2.0;
				else*/
					mid = (-1)*GROUP_VAL_POS(get_current_group(pp->p_config),wedge.Primary_offset)/1000.0 - (GROUP_VAL_POS(get_current_group(pp->p_config),probe.Elem_qty) * GROUP_VAL_POS(get_current_group(pp->p_config),probe.Pitch)/1000.0)/2.0;

				if(GROUP_VAL_POS(p->group, skew_pos)==0)
					middle = GROUP_VAL_POS(p->group, scan_offset)/10.0 - mid;
				else if(GROUP_VAL_POS(p->group, skew_pos)==1)
					middle = GROUP_VAL_POS(p->group, index_offset)/10.0 - mid;
				else if(GROUP_VAL_POS(p->group, skew_pos)==2)
					middle = GROUP_VAL_POS(p->group, scan_offset)/10.0 + mid;
				else if (GROUP_VAL_POS(p->group, skew_pos)==3)
					middle = GROUP_VAL_POS(p->group, index_offset)/10.0 + mid;

				if((LAW_VAL (Angle_min)<0)&&(LAW_VAL (Angle_max)>0))
				{
				     p->wmin1 = middle + get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0 * sin((LAW_VAL_POS (p->group, Angle_min) / 100.0)*(3.14/180.0));
				     p->wmax1 = middle + get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0 * sin((LAW_VAL_POS (p->group, Angle_max) / 100.0)*(3.14/180.0));
				}
				else if((LAW_VAL (Angle_min)<0)&&(LAW_VAL (Angle_max)<0))
				{
				     p->wmin1 = middle + get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0 * sin((LAW_VAL_POS (p->group, Angle_min) / 100.0)*(3.14/180.0));
				     p->wmax1 = middle - get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0 * sin((LAW_VAL_POS (p->group, Angle_max) / 100.0)*(3.14/180.0));
				}
				else if((LAW_VAL (Angle_min)>0)&&(LAW_VAL (Angle_max)>0))
				{
				     p->wmin1 = middle - get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0 * sin((LAW_VAL_POS (p->group, Angle_min) / 100.0)*(3.14/180.0));
				     p->wmax1 = middle + get_group_val (get_group_by_id (pp->p_config, p->group), GROUP_RANGE)/1000.0 * sin((LAW_VAL_POS (p->group, Angle_max) / 100.0)*(3.14/180.0));
				}
				pp->swmin = p->wmin1;
				pp->swmax = p->wmax1;

				g_print("\nwedge_primary_offset = %.2f  ",GROUP_VAL_POS(get_current_group(pp->p_config),wedge.Primary_offset)/1000.0);
				g_print("element_qty = %d  ",GROUP_VAL_POS(get_current_group(pp->p_config),probe.Elem_qty));
				g_print("element_pitch ＝ %.2f\n",GROUP_VAL_POS(get_current_group(pp->p_config),probe.Pitch)/1000.0);
				g_print("wmin1 = %.2f  ", pp->swmin);
				g_print("wmax1 = %.2f  ", pp->swmax);
				g_print("middle = %.2f  ", middle );
				g_print("angle_min = %.1f  ", LAW_VAL_POS (p->group, Angle_min) / 100.0 );
				g_print("sin = %.2f"  , sin((LAW_VAL_POS (p->group, Angle_min) / 100.0)*(3.14/180.0)));
				//p->wmax1 = 100;
				if(get_unit(pp->p_config) == UNIT_MM)
						p->w_unit = UNIT_MM;
				else if(get_unit(pp->p_config)== UNIT_INCH)
						p->w_unit = UNIT_INCH  ;
				p->w_color = 0x0AD5D3;/* 深绿色 */
			}


			p->hmin2 = 0;	/*hrule2*/	/* 黄色 */
			p->hmax2 = 100;
			p->h2_unit = UNIT_BFH;
			p->h2_color = 0xEDF169;

			g_sprintf (p->title, "S scan|Gr %d|CH %0.1f|SK%0.1f|L%d", 
					get_current_group(pp->p_config) + 1, angle / 100.0, GROUP_VAL_POS(p->group, skew) / 100.0, num + 1);
			break;
		case A_B_SCAN:

			break;
		default:break;
	}
}


/* 画波形数据显示区 */
void draw_area_all()
{
	gint i;
	gchar *str = NULL;

	/* 把之前的区域释放 */
	for (i = 0; i < 4; i ++)
	{
		if (pp->vbox_area[i])
		{
			gtk_widget_destroy(pp->vbox_area[i]);
		}
		if (pp->hbox_area[i])
		{
			gtk_widget_destroy(pp->hbox_area[i]);
		}
		pp->vbox_area[i] = gtk_vbox_new(FALSE, 0);
		pp->hbox_area[i] = gtk_hbox_new(FALSE, 0);
		memset (TMP(scan_type), 0xff, 16);
	}

	if ((get_display_group(pp->p_config) == DISPLAY_CURRENT_GROUP) || (get_group_qty(pp->p_config)==1))
	{
		switch (get_display_pos(pp->p_config))
		{
			case A_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 425);
				gtk_widget_show (pp->vbox_area[0]);
				set_scan_config (0, A_SCAN, 605, 605, 390, 0, 0, get_current_group(pp->p_config));
				/* 显示的位置 偏移等等 */
				break;
			case B_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				set_drawarea_property (&(pp->draw_area[0]), B_SCAN, 0);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 425);
				gtk_widget_show (pp->vbox_area[0]);
				set_scan_config (0, B_SCAN, 605, 605, 390, 0, 0, get_current_group(pp->p_config));
				break;
			case C_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				set_drawarea_property (&(pp->draw_area[0]), C_SCAN, 0);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 425);
				set_scan_config (0, C_SCAN, 605, 605, 390, 0, 0, get_current_group(pp->p_config));
				gtk_widget_show (pp->vbox_area[0]);
				break;
			case S_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				if ((GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_SOUNDPATH) ||
						(GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_TIME))
				{
					pp->draw_area[0].scan_type	=	S_SCAN;
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
					set_drawarea_property (&(pp->draw_area[0]), S_SCAN, 0);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 425);
					gtk_widget_show (pp->vbox_area[0]);
					set_scan_config (0, S_SCAN, 605, 605, 390, 0, 0, get_current_group(pp->p_config));
				} 
				else if (GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
					{
						pp->draw_area[0].scan_type	=	S_SCAN_A;
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
						set_drawarea_property (&(pp->draw_area[0]), S_SCAN_A, 0);
						draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 425);
						gtk_widget_show (pp->vbox_area[0]);
						set_scan_config (0, S_SCAN_A, 605, 605, 390, 0, 0, get_current_group(pp->p_config));
					}
					else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
					{
						pp->draw_area[0].scan_type	=	S_SCAN_L;
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
						set_drawarea_property (&(pp->draw_area[0]), S_SCAN_L, 0);
						draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 425);
						gtk_widget_show (pp->vbox_area[0]);
						set_scan_config (0, S_SCAN_L, 605, 605, 390, 0, 0, get_current_group(pp->p_config));
					}
				}
				break;
			case A_B_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				pp->draw_area[1].group	=	get_current_group(pp->p_config);
				//pp->draw_area[0].scan_type	=	A_SCAN;
				//pp->draw_area[1].scan_type	=	B_SCAN;
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x01);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 130);
				set_drawarea_property (&(pp->draw_area[1]), B_SCAN, 0);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 295);
				gtk_widget_show (pp->vbox_area[0]);
				set_scan_config (0, A_SCAN, 605, 605, 115, 0, 0, get_current_group(pp->p_config));
				set_scan_config (1, B_SCAN, 605, 605, 260, 0, 130, get_current_group(pp->p_config));
				break;
			case A_B_C_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				pp->draw_area[1].group	=	get_current_group(pp->p_config);
				pp->draw_area[2].group	=	get_current_group(pp->p_config);
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x01);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 125);
				set_drawarea_property (&(pp->draw_area[1]), B_SCAN, 0);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 150);
				set_drawarea_property (&(pp->draw_area[2]), C_SCAN, 0);
				draw_area_ (pp->vbox_area[0], &(pp->draw_area[2]), 655, 150);
				gtk_widget_show (pp->vbox_area[0]);
				set_scan_config (0, A_SCAN, 605, 605, 110, 0, 0, get_current_group(pp->p_config));
				set_scan_config (1, B_SCAN, 605, 605, 115, 0, 125, get_current_group(pp->p_config));
				set_scan_config (2, C_SCAN, 605, 605, 115, 0, 275, get_current_group(pp->p_config));
				break;
			case A_B_S_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				pp->draw_area[1].group	=	get_current_group(pp->p_config);
				pp->draw_area[2].group	=	get_current_group(pp->p_config);
				if (GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_TRUE_DEPTH)
				{
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
					gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
					gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
					if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
						set_drawarea_property (&(pp->draw_area[0]), S_SCAN_A, 0);
					else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
						set_drawarea_property (&(pp->draw_area[0]), S_SCAN_L, 0);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 300, 425);
					set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0x01);
					draw_area_ (pp->vbox_area[1], &(pp->draw_area[1]), 355, 210);
					set_drawarea_property (&(pp->draw_area[2]), B_SCAN, 0);
					draw_area_ (pp->vbox_area[1], &(pp->draw_area[2]), 355, 215);
					gtk_widget_show (pp->hbox_area[0]);
					gtk_widget_show (pp->vbox_area[0]);
					gtk_widget_show (pp->vbox_area[1]);
					if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
						set_scan_config (0, S_SCAN_A, 305, 250, 390, 0, 0, get_current_group(pp->p_config));
					else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
						set_scan_config (0, S_SCAN_L, 305, 250, 390, 0, 0, get_current_group(pp->p_config));
					set_scan_config (1, A_SCAN, 305, 305, 195, 300, 0, get_current_group(pp->p_config));
					set_scan_config (2, B_SCAN, 305, 305, 180, 300, 210, get_current_group(pp->p_config));
				}
				else
				{
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
					set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x01);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 125);
					set_drawarea_property (&(pp->draw_area[1]), S_SCAN, 0x01);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 150);
					set_drawarea_property (&(pp->draw_area[2]), B_SCAN, 0);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[2]), 655, 150);
					gtk_widget_show (pp->vbox_area[0]);
					set_scan_config (0, A_SCAN, 605, 605, 110, 0, 0, get_current_group(pp->p_config));
					set_scan_config (1, S_SCAN, 605, 605, 135, 0, 125, get_current_group(pp->p_config));
					set_scan_config (2, B_SCAN, 605, 605, 115, 0, 275, get_current_group(pp->p_config));
				}
				break;
			case A_C_CC_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				pp->draw_area[1].group	=	get_current_group(pp->p_config);
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
				{
					set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 150);
					set_drawarea_property (&(pp->draw_area[1]), C_SCAN, 0);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 275);
					set_scan_config (0, A_SCAN, 605, 605, 115, 0, 0, get_current_group(pp->p_config));
					set_scan_config (1, C_SCAN, 605, 605, 240, 0, 150, get_current_group(pp->p_config));
				}
				else if (get_cscan_source(pp->p_config, 1) != C_SCAN_OFF)
				{
					set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 125);
					set_drawarea_property (&(pp->draw_area[1]), C_SCAN, 0x01);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 150);
					set_drawarea_property (&(pp->draw_area[2]), CC_SCAN, 0);
					draw_area_ (pp->vbox_area[0], &(pp->draw_area[2]), 655, 150);
					set_scan_config (0, A_SCAN, 605, 605, 90, 0, 0, get_current_group(pp->p_config));
					set_scan_config (1, C_SCAN, 605, 605, 135, 0, 125, get_current_group(pp->p_config));
					set_scan_config (2, CC_SCAN, 605, 605, 115, 0, 275, get_current_group(pp->p_config));
				}
				gtk_widget_show (pp->vbox_area[0]);
				break;
			case A_S_CC_SCAN:
				pp->draw_area[0].group	=	get_current_group(pp->p_config);
				pp->draw_area[1].group	=	get_current_group(pp->p_config);
				pp->draw_area[2].group	=	get_current_group(pp->p_config);
				if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
				{
					if ((GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_SOUNDPATH) ||
							(GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_TIME))
					{
						//pp->draw_area[0].scan_type	=	A_SCAN;
						//pp->draw_area[1].scan_type	=	S_SCAN;
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
						set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x01);
						draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 130);
						set_drawarea_property (&(pp->draw_area[1]), S_SCAN, 0);
						draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 295);
						gtk_widget_show (pp->vbox_area[0]);
						set_scan_config (0, A_SCAN, 605, 605, 115, 0, 0, get_current_group(pp->p_config));
						set_scan_config (1, S_SCAN, 605, 605, 260, 0, 130, get_current_group(pp->p_config));
					}
					else if (GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_TRUE_DEPTH)
					{
						if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
						{
							//pp->draw_area[0].scan_type	=	A_SCAN_R;
							//pp->draw_area[1].scan_type	=	S_SCAN_A;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN_R, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 250, 425);
							set_drawarea_property (&(pp->draw_area[1]), S_SCAN_A, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 405, 425);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, A_SCAN_R, 390, 230, 390, 0, 0, get_current_group(pp->p_config));
							set_scan_config (1, S_SCAN_A, 390, 355, 390, 250, 0, get_current_group(pp->p_config));
						} 
						else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
						{
							//pp->draw_area[0].scan_type	=	A_SCAN_R;
							//pp->draw_area[1].scan_type	=	S_SCAN_L;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN_R, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 250, 425);
							//set_drawarea_property (&(pp->draw_area[1]), S_SCAN_L, 0x08);
							set_drawarea_property (&(pp->draw_area[1]), S_SCAN_L, 0x0);//modified by shengsheng
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 405, 425);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, A_SCAN_R, 390, 230, 390, 0, 0, get_current_group(pp->p_config));
							set_scan_config (1, S_SCAN_L, 390, 355, 390, 250, 0, get_current_group(pp->p_config));
						}
					}
				}
				else
				{
					if ((GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_SOUNDPATH) ||
							(GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_TIME))
					{
						//pp->draw_area[0].scan_type	=	A_SCAN;
						//pp->draw_area[1].scan_type	=	S_SCAN;
						//pp->draw_area[2].scan_type	=	CCC_SCAN;
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
						set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
						draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 125);
						set_drawarea_property (&(pp->draw_area[1]), S_SCAN, 0x01);
						draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 150);
						set_drawarea_property (&(pp->draw_area[2]), CCC_SCAN, 0);
						draw_area_ (pp->vbox_area[0], &(pp->draw_area[2]), 655, 150);
						gtk_widget_show (pp->vbox_area[0]);
						set_scan_config (0, A_SCAN, 605, 605, 95, 0, 0, get_current_group(pp->p_config));
						set_scan_config (1, S_SCAN, 605, 605, 135, 0, 125, get_current_group(pp->p_config));
						set_scan_config (2, CCC_SCAN, 605, 605, 115, 0, 275, get_current_group(pp->p_config));
					}
					else if (GROUP_VAL_POS(get_current_group(pp->p_config), ut_unit) == UT_UNIT_TRUE_DEPTH)
					{
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[1], FALSE, FALSE, 0);
						if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
						{
							//pp->draw_area[0].scan_type	=	A_SCAN_R;
							//pp->draw_area[1].scan_type	=	S_SCAN_A;
							//pp->draw_area[2].scan_type	=	CCC_SCAN;
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN_R, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 250, 275);
							set_drawarea_property (&(pp->draw_area[1]), S_SCAN_A, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 405, 275);
							set_drawarea_property (&(pp->draw_area[2]), CCC_SCAN, 0);
							draw_area_ (pp->vbox_area[1], &(pp->draw_area[2]), 655, 150);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, A_SCAN_R, 230, 230, 240, 0, 0, get_current_group(pp->p_config));
							set_scan_config (1, S_SCAN_A, 230, 355, 240, 250, 0, get_current_group(pp->p_config));
							set_scan_config (2, CCC_SCAN, 230, 605, 115, 0, 275, get_current_group(pp->p_config));
						} 
						else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
						{
							//pp->draw_area[0].scan_type	=	A_SCAN_R;
							//pp->draw_area[1].scan_type	=	S_SCAN_L;
							//pp->draw_area[2].scan_type	=	CCC_SCAN;
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN_R, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 250, 275);
							set_drawarea_property (&(pp->draw_area[1]), S_SCAN_L, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 405, 275);
							set_drawarea_property (&(pp->draw_area[2]), CCC_SCAN, 0);
							draw_area_ (pp->vbox_area[1], &(pp->draw_area[2]), 655, 150);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, A_SCAN_R, 230, 230, 240, 0, 0, get_current_group(pp->p_config));
							set_scan_config (1, S_SCAN_L, 230, 355, 240, 250, 0, get_current_group(pp->p_config));
							set_scan_config (2, CCC_SCAN, 230, 605, 115, 0, 275, get_current_group(pp->p_config));
						}
					}
				}
				break;
			case Strip_Chart_AA:
				break;
			default:
				break;
		}
	}
	else if (get_display_group(pp->p_config) == DISPLAY_ALL_GROUP) 
		{
			switch (get_group_qty(pp->p_config))
			{
				case 2:
					switch (get_display_pos(pp->p_config))
					{
						case A_SCAN:
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
							g_sprintf (pp->draw_area[0].title, "multiple A scan");
							draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 212);
							set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0x10);
							draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 213);
							gtk_widget_show (pp->vbox_area[0]);
							set_scan_config (0, A_SCAN, 615, 615, 178, 0, 0,  0);
							set_scan_config (1, A_SCAN, 615, 615, 193, 0, 212, 1);
							/* 显示的位置 偏移等等 */
							break;
						case S_SCAN:
							if ((GROUP_VAL(ut_unit) == UT_UNIT_SOUNDPATH) ||
									(GROUP_VAL(ut_unit) == UT_UNIT_TIME))
							{
								pp->draw_area[0].scan_type	=	S_SCAN;
								pp->draw_area[1].scan_type	=	S_SCAN;
							} 
							else if (GROUP_VAL(ut_unit) == UT_UNIT_TRUE_DEPTH)
							{
								if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
								{
									pp->draw_area[0].scan_type	=	S_SCAN_A;
									pp->draw_area[1].scan_type	=	S_SCAN_A;
								}
								else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
								{
									pp->draw_area[0].scan_type	=	S_SCAN_L;
									pp->draw_area[1].scan_type	=	S_SCAN_L;
								}
							}
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0);
							draw_area_(pp->hbox_area[0], &(pp->draw_area[0]), 327, 425);
							set_drawarea_property (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0);
							draw_area_(pp->hbox_area[0], &(pp->draw_area[1]), 328, 425);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, pp->draw_area[0].scan_type, 615, 287, 390, 0, 0,  0);
							set_scan_config (1, pp->draw_area[1].scan_type, 615, 288, 390, 327, 0, 1);
							break;
						case A_C_CC_SCAN:
							if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
							{
								pp->draw_area[0].group	=	0;
								pp->draw_area[1].group	=	0;
								pp->draw_area[2].group	=	1;
								pp->draw_area[3].group	=	1;
								gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
								set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
								g_sprintf (pp->draw_area[0].title, "multiple A_C_CC_SCAN");
								draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 327, 150);
								set_drawarea_property (&(pp->draw_area[1]), C_SCAN, 0x10);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 327, 275);
								set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[2]), 328, 150);
								set_drawarea_property (&(pp->draw_area[3]), C_SCAN, 0x10);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 328, 275);
								gtk_widget_show (pp->hbox_area[0]);
								gtk_widget_show (pp->vbox_area[0]);
								gtk_widget_show (pp->vbox_area[1]);
								set_scan_config (0, A_SCAN, 277, 277, 115, 0, 0,  0);
								set_scan_config (1, C_SCAN, 277, 277, 255, 0, 150, 0);
								set_scan_config (2, A_SCAN, 278, 278, 115, 327, 0,  1);
								set_scan_config (3, C_SCAN, 278, 278, 255, 327,150, 1); 
							}
							else
							{
								pp->draw_area[0].group	=	0;
								pp->draw_area[1].group	=	0;
								pp->draw_area[2].group	=	0;
								pp->draw_area[3].group	=	1;
								pp->draw_area[4].group	=	1;
								pp->draw_area[5].group	=	1;
								gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
								set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 327, 141);
								set_drawarea_property (&(pp->draw_area[1]), C_SCAN, 0x11);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 327, 142);
								set_drawarea_property (&(pp->draw_area[2]), CC_SCAN, 0x10);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[2]), 327, 142);
								set_drawarea_property (&(pp->draw_area[3]), A_SCAN, 0);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 328, 141);
								set_drawarea_property (&(pp->draw_area[4]), C_SCAN, 0x11);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[4]), 328, 142);
								set_drawarea_property (&(pp->draw_area[5]), CC_SCAN, 0x10);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[5]), 328, 142);
								gtk_widget_show (pp->hbox_area[0]);
								gtk_widget_show (pp->vbox_area[0]);
								gtk_widget_show (pp->vbox_area[1]);
								set_scan_config (0, A_SCAN, 277, 277, 106, 0, 0,  0);
								set_scan_config (1, C_SCAN, 277, 277, 142, 0, 141, 0);
								set_scan_config (2, CC_SCAN, 277, 277, 122, 0, 283, 0);
								set_scan_config (3, A_SCAN, 278, 278, 106, 327, 0,  1);
								set_scan_config (4, C_SCAN, 278, 278, 142, 327, 141, 1);
								set_scan_config (5, CC_SCAN, 278, 278, 122, 327, 283, 1); 
							}
							break;
						case A_S_CC_SCAN:
							if ((GROUP_VAL( ut_unit) == UT_UNIT_SOUNDPATH) ||
									(GROUP_VAL( ut_unit) == UT_UNIT_TIME))
							{
									pp->draw_area[0].scan_type	=	A_SCAN;
									pp->draw_area[1].scan_type	=	S_SCAN;
									pp->draw_area[2].scan_type	=	CCC_SCAN;
									pp->draw_area[3].scan_type	=	A_SCAN;
									pp->draw_area[4].scan_type	=	S_SCAN;
									pp->draw_area[5].scan_type	=	CCC_SCAN;
							} 
							else if (GROUP_VAL(ut_unit) == UT_UNIT_TRUE_DEPTH)
							{
								if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
								{
									pp->draw_area[0].scan_type	=	A_SCAN;
									pp->draw_area[1].scan_type	=	S_SCAN_A;
									pp->draw_area[2].scan_type	=	CCC_SCAN;
									pp->draw_area[3].scan_type	=	A_SCAN;
									pp->draw_area[4].scan_type	=	S_SCAN_A;
									pp->draw_area[5].scan_type	=	CCC_SCAN;
								}
								else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
								{
									pp->draw_area[0].scan_type	=	A_SCAN;
									pp->draw_area[1].scan_type	=	S_SCAN_L;
									pp->draw_area[2].scan_type	=	CCC_SCAN;
									pp->draw_area[3].scan_type	=	A_SCAN;
									pp->draw_area[4].scan_type	=	S_SCAN_L;
									pp->draw_area[5].scan_type	=	CCC_SCAN;
								}
							}

							if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
							{
								pp->draw_area[0].group	=	0;
								pp->draw_area[1].group	=	0;
								pp->draw_area[3].group	=	1;
								pp->draw_area[4].group	=	1;
									gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
									set_drawarea_property (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 327, 150);
									set_drawarea_property (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x10);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 327, 275);
									set_drawarea_property (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 328, 150);
									set_drawarea_property (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x10);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[4]), 328, 275);
									gtk_widget_show (pp->hbox_area[0]);
									gtk_widget_show (pp->vbox_area[0]);
									gtk_widget_show (pp->vbox_area[1]);
									set_scan_config (0, pp->draw_area[0].scan_type, 277, 277, 115, 0, 0,  0);
									set_scan_config (1, pp->draw_area[1].scan_type, 277, 277, 255, 0, 150, 0);
									set_scan_config (2, pp->draw_area[3].scan_type, 278, 278, 115, 327, 0,  1);
									set_scan_config (3, pp->draw_area[4].scan_type, 278, 278, 255, 327, 150, 1); 
							}
							else
							{
									pp->draw_area[0].group	=	0;
									pp->draw_area[1].group	=	0;
									pp->draw_area[2].group	=	0;
									pp->draw_area[3].group	=	1;
									pp->draw_area[4].group	=	1;
									pp->draw_area[5].group	=	1;
									gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
									set_drawarea_property (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 327, 141);
									set_drawarea_property (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x10);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 327, 142);
									set_drawarea_property (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0x10);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[2]), 327, 142);

									set_drawarea_property (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 328, 141);
									set_drawarea_property (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x10);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[4]), 328, 142);
									set_drawarea_property (&(pp->draw_area[5]), pp->draw_area[5].scan_type, 0x10);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[5]), 328, 142);
									gtk_widget_show (pp->hbox_area[0]);
									gtk_widget_show (pp->vbox_area[0]);
									gtk_widget_show (pp->vbox_area[1]);
									set_scan_config (0, pp->draw_area[0].scan_type, 277, 277, 106, 0, 0,  0);
									set_scan_config (1, pp->draw_area[1].scan_type, 277, 277, 122, 0, 141, 0);
									set_scan_config (2, pp->draw_area[2].scan_type, 277, 277, 122, 0, 283, 0);
									set_scan_config (3, pp->draw_area[3].scan_type, 278, 278, 106, 327, 0, 1);
									set_scan_config (4, pp->draw_area[4].scan_type, 278, 278, 122, 327, 141,  1);
									set_scan_config (5, pp->draw_area[5].scan_type, 278, 278, 122, 327, 283, 1);
							}
							break;
						default:
							break;
					}
					break;
				case 3:
					switch (get_display_pos(pp->p_config))
					{
						case A_SCAN:
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							pp->draw_area[2].group	=	2;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
							draw_area_ (pp->vbox_area[0], &(pp->draw_area[0]), 655, 141);
							set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0x10);
							draw_area_ (pp->vbox_area[0], &(pp->draw_area[1]), 655, 142);
							set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0x10);
							draw_area_ (pp->vbox_area[0], &(pp->draw_area[2]), 655, 142);
							gtk_widget_show (pp->vbox_area[0]);
							set_scan_config (0, A_SCAN, 615, 615, 106, 0, 0,  0);
							set_scan_config (1, A_SCAN, 615, 615, 122, 0, 141, 1);
							set_scan_config (2, A_SCAN, 615, 615, 122, 0, 283, 2);
							break;
						case S_SCAN:
							if ((GROUP_VAL(ut_unit) == UT_UNIT_SOUNDPATH) ||
									(GROUP_VAL(ut_unit) == UT_UNIT_TIME))
							{
								pp->draw_area[0].scan_type	=	S_SCAN;
								pp->draw_area[1].scan_type	=	S_SCAN;
								pp->draw_area[2].scan_type	=	S_SCAN;
							} 
							else if (GROUP_VAL(ut_unit) == UT_UNIT_TRUE_DEPTH)
							{
								if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
								{
									pp->draw_area[0].scan_type	=	S_SCAN_A;
									pp->draw_area[1].scan_type	=	S_SCAN_A;
									pp->draw_area[2].scan_type	=	S_SCAN_A;
								}
								else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
								{
									pp->draw_area[0].scan_type	=	S_SCAN_L;
									pp->draw_area[1].scan_type	=	S_SCAN_L;
									pp->draw_area[2].scan_type	=	S_SCAN_L;
								}
							}
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							pp->draw_area[2].group	=	2;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 218, 425);
							set_drawarea_property (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 218, 425);
							set_drawarea_property (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[2]), 219, 425);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, pp->draw_area[0].scan_type, 615, 178, 390, 0, 0,  0);
							set_scan_config (1, pp->draw_area[1].scan_type, 615, 178, 390, 218, 0, 1);
							set_scan_config (2, pp->draw_area[2].scan_type, 615, 179, 390, 436, 0, 2);
							break;
						case A_C_CC_SCAN:
							if (get_cscan_source(pp->p_config, 1) == C_SCAN_OFF)
							{
								pp->draw_area[0].group	=	0;
								pp->draw_area[1].group	=	0;
								pp->draw_area[2].group	=	1;
								pp->draw_area[3].group	=	1;
								pp->draw_area[4].group	=	2;
								pp->draw_area[5].group	=	2;
								gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
								set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 218, 150);
								set_drawarea_property (&(pp->draw_area[1]), C_SCAN, 0x10);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 218, 275);
								set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[2]), 218, 150);
								set_drawarea_property (&(pp->draw_area[3]), C_SCAN, 0x10);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 218, 275);
								set_drawarea_property (&(pp->draw_area[4]), A_SCAN, 0);
								draw_area_(pp->vbox_area[2], &(pp->draw_area[4]), 219, 150);
								set_drawarea_property (&(pp->draw_area[5]), C_SCAN, 0x10);
								draw_area_(pp->vbox_area[2], &(pp->draw_area[5]), 219, 275);
								gtk_widget_show (pp->hbox_area[0]);
								gtk_widget_show (pp->vbox_area[0]);
								gtk_widget_show (pp->vbox_area[1]);
								gtk_widget_show (pp->vbox_area[2]);
								set_scan_config (0, A_SCAN, 168, 168, 115, 0, 0,  0);
								set_scan_config (1, C_SCAN, 168, 168, 255, 0, 150, 0);
								set_scan_config (2, A_SCAN, 168, 168, 115, 218, 0,  1);
								set_scan_config (3, C_SCAN, 168, 168, 255, 218, 150, 1); 
								set_scan_config (4, A_SCAN, 169, 169, 115, 436, 0,  2);
								set_scan_config (5, C_SCAN, 169, 169, 255, 436, 150, 2); 
							}
							else
							{
								pp->draw_area[0].group	=	0;
								pp->draw_area[1].group	=	0;
								pp->draw_area[2].group	=	0;
								pp->draw_area[3].group	=	1;
								pp->draw_area[4].group	=	1;
								pp->draw_area[5].group	=	1;
								pp->draw_area[6].group	=	2;
								pp->draw_area[7].group	=	2;
								pp->draw_area[8].group	=	2;
								gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
								gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
								set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 218, 141);
								set_drawarea_property (&(pp->draw_area[1]), C_SCAN, 0x11);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 218, 142);
								set_drawarea_property (&(pp->draw_area[2]), CC_SCAN, 0x10);
								draw_area_(pp->vbox_area[0], &(pp->draw_area[2]), 218, 142);
								set_drawarea_property (&(pp->draw_area[3]), A_SCAN, 0);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 218, 141);
								set_drawarea_property (&(pp->draw_area[4]), C_SCAN, 0x11);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[4]), 218, 142);
								set_drawarea_property (&(pp->draw_area[5]), CC_SCAN, 0x10);
								draw_area_(pp->vbox_area[1], &(pp->draw_area[5]), 218, 142);

								set_drawarea_property (&(pp->draw_area[6]), A_SCAN, 0);
								draw_area_(pp->vbox_area[2], &(pp->draw_area[6]), 219, 141);
								set_drawarea_property (&(pp->draw_area[7]), C_SCAN, 0x11);
								draw_area_(pp->vbox_area[2], &(pp->draw_area[7]), 219, 142);
								set_drawarea_property (&(pp->draw_area[8]), CC_SCAN, 0x10);
								draw_area_(pp->vbox_area[2], &(pp->draw_area[8]), 219, 142);

								gtk_widget_show (pp->hbox_area[0]);
								gtk_widget_show (pp->vbox_area[0]);
								gtk_widget_show (pp->vbox_area[1]);
								gtk_widget_show (pp->vbox_area[2]);
								set_scan_config (0, A_SCAN, 168, 168, 106, 0, 0,  0);
								set_scan_config (1, C_SCAN, 168, 168, 122, 0, 141, 0);
								set_scan_config (2, CC_SCAN, 168, 168, 122, 0, 283, 0);
								set_scan_config (3, A_SCAN, 168, 168, 106, 218, 0,  1);
								set_scan_config (4, C_SCAN, 168, 168, 122, 218, 141, 1);
								set_scan_config (5, CC_SCAN, 168, 168, 122, 218, 283, 1); 
								set_scan_config (6, A_SCAN, 169, 169, 106, 436, 0,  2);
								set_scan_config (7, C_SCAN, 169, 169, 122, 436, 141, 2);
								set_scan_config (8, CC_SCAN, 169, 169, 122, 436, 283, 2); 
							}
							break;

						case A_S_CC_SCAN:
							if ((GROUP_VAL(ut_unit) == UT_UNIT_SOUNDPATH) ||
									(GROUP_VAL(ut_unit) == UT_UNIT_TIME))
							{
									pp->draw_area[0].scan_type	=	A_SCAN;
									pp->draw_area[1].scan_type	=	S_SCAN;
									pp->draw_area[2].scan_type	=	CCC_SCAN;
									pp->draw_area[3].scan_type	=	A_SCAN;
									pp->draw_area[4].scan_type	=	S_SCAN;
									pp->draw_area[5].scan_type	=	CCC_SCAN;
									pp->draw_area[6].scan_type	=	A_SCAN;
									pp->draw_area[7].scan_type	=	S_SCAN;
									pp->draw_area[8].scan_type	=	CCC_SCAN;
							} 
							else if (GROUP_VAL(ut_unit) == UT_UNIT_TRUE_DEPTH)
							{
								if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
								{
									pp->draw_area[0].scan_type	=	A_SCAN;
									pp->draw_area[1].scan_type	=	S_SCAN_A;
									pp->draw_area[2].scan_type	=	CCC_SCAN;
									pp->draw_area[3].scan_type	=	A_SCAN;
									pp->draw_area[4].scan_type	=	S_SCAN_A;
									pp->draw_area[5].scan_type	=	CCC_SCAN;
									pp->draw_area[6].scan_type	=	A_SCAN;
									pp->draw_area[7].scan_type	=	S_SCAN;
									pp->draw_area[8].scan_type	=	CCC_SCAN;
								}
								else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
								{
									pp->draw_area[0].scan_type	=	A_SCAN;
									pp->draw_area[1].scan_type	=	S_SCAN_L;
									pp->draw_area[2].scan_type	=	CCC_SCAN;
									pp->draw_area[3].scan_type	=	A_SCAN;
									pp->draw_area[4].scan_type	=	S_SCAN_L;
									pp->draw_area[5].scan_type	=	CCC_SCAN;
									pp->draw_area[6].scan_type	=	A_SCAN;
									pp->draw_area[7].scan_type	=	S_SCAN;
									pp->draw_area[8].scan_type	=	CCC_SCAN;
								}
							}

							if (get_cscan_source(pp->p_config, 0) == C_SCAN_OFF) /* c 扫描不存在的情况 */
							{
								pp->draw_area[0].group	=	0;
								pp->draw_area[1].group	=	0;
								pp->draw_area[3].group	=	1;
								pp->draw_area[4].group	=	1;
								pp->draw_area[6].group	=	2;
								pp->draw_area[7].group	=	2;
									gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
									set_drawarea_property (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 218, 150);
									set_drawarea_property (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x10);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 218, 275);
									set_drawarea_property (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 218, 150);
									set_drawarea_property (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x10);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[4]), 218, 275);
									set_drawarea_property (&(pp->draw_area[6]), pp->draw_area[6].scan_type, 0);
									draw_area_(pp->vbox_area[2], &(pp->draw_area[6]), 219, 150);
									set_drawarea_property (&(pp->draw_area[7]), pp->draw_area[7].scan_type, 0x10);
									draw_area_(pp->vbox_area[2], &(pp->draw_area[7]), 219, 275);
									gtk_widget_show (pp->hbox_area[0]);
									gtk_widget_show (pp->vbox_area[0]);
									gtk_widget_show (pp->vbox_area[1]);
									gtk_widget_show (pp->vbox_area[2]);
									set_scan_config (0, pp->draw_area[0].scan_type, 168, 168, 115, 0, 0,  0);
									set_scan_config (1, pp->draw_area[1].scan_type, 168, 168, 255, 0, 150, 0);
									set_scan_config (2, pp->draw_area[3].scan_type, 168, 168, 115, 218, 0,  1);
									set_scan_config (3, pp->draw_area[4].scan_type, 168, 168, 255, 218, 150, 1); 
									set_scan_config (4, pp->draw_area[6].scan_type, 169, 169, 115, 436, 0,  2);
									set_scan_config (5, pp->draw_area[7].scan_type, 169, 169, 255, 436, 150, 2); 
							}
							else
							{
									pp->draw_area[0].group	=	0;
									pp->draw_area[1].group	=	0;
									pp->draw_area[2].group	=	0;
									pp->draw_area[3].group	=	1;
									pp->draw_area[4].group	=	1;
									pp->draw_area[5].group	=	1;
									pp->draw_area[6].group	=	2;
									pp->draw_area[7].group	=	2;
									pp->draw_area[8].group	=	2;
									gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE,0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
									gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[2], FALSE, FALSE, 0);
									set_drawarea_property (&(pp->draw_area[0]), pp->draw_area[0].scan_type, 0);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[0]), 218, 141);
									set_drawarea_property (&(pp->draw_area[1]), pp->draw_area[1].scan_type, 0x10);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[1]), 218, 142);
									set_drawarea_property (&(pp->draw_area[2]), pp->draw_area[2].scan_type, 0x10);
									draw_area_(pp->vbox_area[0], &(pp->draw_area[2]), 218, 142);

									set_drawarea_property (&(pp->draw_area[3]), pp->draw_area[3].scan_type, 0);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[3]), 218, 141);
									set_drawarea_property (&(pp->draw_area[4]), pp->draw_area[4].scan_type, 0x10);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[4]), 218, 142);
									set_drawarea_property (&(pp->draw_area[5]), pp->draw_area[5].scan_type, 0x10);
									draw_area_(pp->vbox_area[1], &(pp->draw_area[5]), 218, 142);

									set_drawarea_property (&(pp->draw_area[6]), pp->draw_area[6].scan_type, 0);
									draw_area_(pp->vbox_area[2], &(pp->draw_area[6]), 219, 141);
									set_drawarea_property (&(pp->draw_area[7]), pp->draw_area[7].scan_type, 0x10);
									draw_area_(pp->vbox_area[2], &(pp->draw_area[7]), 219, 142);
									set_drawarea_property (&(pp->draw_area[8]), pp->draw_area[8].scan_type, 0x10);
									draw_area_(pp->vbox_area[2], &(pp->draw_area[8]), 219, 142);

									gtk_widget_show (pp->hbox_area[0]);
									gtk_widget_show (pp->vbox_area[0]);
									gtk_widget_show (pp->vbox_area[1]);
									gtk_widget_show (pp->vbox_area[2]);
									set_scan_config (0, pp->draw_area[0].scan_type, 168, 178, 106, 0, 0,  0);
									set_scan_config (1, pp->draw_area[1].scan_type, 168, 178, 122, 0, 141, 0);
									set_scan_config (2, pp->draw_area[2].scan_type, 168, 178, 122, 0, 283, 0);
									set_scan_config (3, pp->draw_area[3].scan_type, 168, 178, 106, 218, 0, 1);
									set_scan_config (4, pp->draw_area[4].scan_type, 168, 178, 122, 218, 141,  1);
									set_scan_config (5, pp->draw_area[5].scan_type, 168, 178, 122, 218, 283, 1);
									set_scan_config (6, pp->draw_area[6].scan_type, 169, 179, 106, 436, 0, 2);
									set_scan_config (7, pp->draw_area[7].scan_type, 169, 179, 122, 436, 141,  2);
									set_scan_config (8, pp->draw_area[8].scan_type, 169, 179, 122, 436, 283, 2);
							}
							break;

						default:break;
					}
					break;
				case 4:
					switch (get_display_pos(pp->p_config))
					{
						case A_SCAN:
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							pp->draw_area[2].group	=	2;
							pp->draw_area[3].group	=	3;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 327, 212);
							set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 328, 212);
							set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[2]), 327, 213);
							set_drawarea_property (&(pp->draw_area[3]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[3]), 328, 213);
							gtk_widget_show (pp->vbox_area[0]);
							gtk_widget_show (pp->hbox_area[0]);
							gtk_widget_show (pp->hbox_area[1]);
							set_scan_config (0, A_SCAN, 307, 307, 177, 0, 0,  0);
							set_scan_config (1, A_SCAN, 278, 278, 177, 327, 0, 1);
							set_scan_config (2, A_SCAN, 307, 307, 193, 0, 212, 2);
							set_scan_config (3, A_SCAN, 278, 278, 193, 327, 212, 3);
							break;
						default:break;
					}
					break;
				case 5:
					switch (get_display_pos(pp->p_config))
					{
						case A_SCAN:
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							pp->draw_area[2].group	=	2;
							pp->draw_area[3].group	=	3;
							pp->draw_area[4].group	=	4;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 327, 141);
							set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 328, 141);
							set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[2]), 327, 142);
							set_drawarea_property (&(pp->draw_area[3]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[3]), 328, 142);
							set_drawarea_property (&(pp->draw_area[4]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[2], &(pp->draw_area[4]), 327, 142);
							gtk_widget_show (pp->vbox_area[0]);
							gtk_widget_show (pp->hbox_area[0]);
							gtk_widget_show (pp->hbox_area[1]);
							gtk_widget_show (pp->hbox_area[2]);
							set_scan_config (0, A_SCAN, 307, 307, 106, 0, 0,  0);
							set_scan_config (1, A_SCAN, 278, 278, 106, 327, 0, 1);
							set_scan_config (2, A_SCAN, 307, 307, 122, 0, 141, 2);
							set_scan_config (3, A_SCAN, 278, 278, 122, 327, 141, 3);
							set_scan_config (4, A_SCAN, 307, 307, 122, 0, 283, 4);
							break;
						default:break;
					}
					break;
				case 6:
					switch (get_display_pos(pp->p_config))
					{
						case A_SCAN:
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							pp->draw_area[2].group	=	2;
							pp->draw_area[3].group	=	3;
							pp->draw_area[4].group	=	4;
							pp->draw_area[5].group	=	5;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 327, 141);
							set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 328, 141);
							set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[2]), 327, 142);
							set_drawarea_property (&(pp->draw_area[3]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[3]), 328, 142);
							set_drawarea_property (&(pp->draw_area[4]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[2], &(pp->draw_area[4]), 327, 142);
							set_drawarea_property (&(pp->draw_area[5]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[2], &(pp->draw_area[5]), 328, 142);
							gtk_widget_show (pp->vbox_area[0]);
							gtk_widget_show (pp->hbox_area[0]);
							gtk_widget_show (pp->hbox_area[1]);
							gtk_widget_show (pp->hbox_area[2]);
							set_scan_config (0, A_SCAN, 307, 307, 106, 0, 0,  0);
							set_scan_config (1, A_SCAN, 278, 278, 106, 327, 0, 1);
							set_scan_config (2, A_SCAN, 307, 307, 122, 0, 141, 2);
							set_scan_config (3, A_SCAN, 278, 278, 122, 327, 141, 3);
							set_scan_config (4, A_SCAN, 307, 307, 122, 0, 283, 4);
							set_scan_config (5, A_SCAN, 278, 278, 122, 327, 283, 5);
							break;
						default:break;
					}
					break;
				case 7:
					switch (get_display_pos(pp->p_config))
					{
						case A_SCAN:
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							pp->draw_area[2].group	=	2;
							pp->draw_area[3].group	=	3;
							pp->draw_area[4].group	=	4;
							pp->draw_area[5].group	=	5;
							pp->draw_area[6].group	=	6;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[3], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 327, 106);
							set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 328, 106);
							set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[2]), 327, 106);
							set_drawarea_property (&(pp->draw_area[3]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[3]), 328, 106);
							set_drawarea_property (&(pp->draw_area[4]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[2], &(pp->draw_area[4]), 327, 106);
							set_drawarea_property (&(pp->draw_area[5]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[2], &(pp->draw_area[5]), 328, 106);
							set_drawarea_property (&(pp->draw_area[6]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[3], &(pp->draw_area[6]), 327, 107);
							gtk_widget_show (pp->vbox_area[0]);
							gtk_widget_show (pp->hbox_area[0]);
							gtk_widget_show (pp->hbox_area[1]);
							gtk_widget_show (pp->hbox_area[2]);
							gtk_widget_show (pp->hbox_area[3]);
							set_scan_config (0, A_SCAN, 307, 307, 71, 0, 0,  0);
							set_scan_config (1, A_SCAN, 278, 278, 71, 327, 0, 1);
							set_scan_config (2, A_SCAN, 307, 307, 86, 0, 106, 2);
							set_scan_config (3, A_SCAN, 278, 278, 86, 327, 106, 3);
							set_scan_config (4, A_SCAN, 307, 307, 86, 0, 212, 4);
							set_scan_config (5, A_SCAN, 278, 278, 86, 327, 212,5);
							set_scan_config (6, A_SCAN, 307, 307, 87, 0, 318, 6);
							break;
						default:break;
					}
					break;
				case 8:
					switch (get_display_pos(pp->p_config))
					{
						case A_SCAN:
							pp->draw_area[0].group	=	0;
							pp->draw_area[1].group	=	1;
							pp->draw_area[2].group	=	2;
							pp->draw_area[3].group	=	3;
							pp->draw_area[4].group	=	4;
							pp->draw_area[5].group	=	5;
							pp->draw_area[6].group	=	6;
							pp->draw_area[7].group	=	7;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[1], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[2], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->vbox_area[0]), pp->hbox_area[3], FALSE, FALSE, 0);
							set_drawarea_property (&(pp->draw_area[0]), A_SCAN, 0x06);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[0]), 327, 106);
							set_drawarea_property (&(pp->draw_area[1]), A_SCAN, 0);
							draw_area_ (pp->hbox_area[0], &(pp->draw_area[1]), 328, 106);
							set_drawarea_property (&(pp->draw_area[2]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[2]), 327, 106);
							set_drawarea_property (&(pp->draw_area[3]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[1], &(pp->draw_area[3]), 328, 106);
							set_drawarea_property (&(pp->draw_area[4]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[2], &(pp->draw_area[4]), 327, 106);
							set_drawarea_property (&(pp->draw_area[5]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[2], &(pp->draw_area[5]), 328, 106);
							set_drawarea_property (&(pp->draw_area[6]), A_SCAN, 0x16);
							draw_area_ (pp->hbox_area[3], &(pp->draw_area[6]), 327, 107);
							set_drawarea_property (&(pp->draw_area[7]), A_SCAN, 0x10);
							draw_area_ (pp->hbox_area[3], &(pp->draw_area[7]), 328, 107);
							gtk_widget_show (pp->vbox_area[0]);
							gtk_widget_show (pp->hbox_area[0]);
							gtk_widget_show (pp->hbox_area[1]);
							gtk_widget_show (pp->hbox_area[2]);
							gtk_widget_show (pp->hbox_area[3]);
							set_scan_config (0, A_SCAN, 307, 307, 71, 0, 0,  0);
							set_scan_config (1, A_SCAN, 278, 278, 71, 327, 0, 1);
							set_scan_config (2, A_SCAN, 307, 307, 86, 0, 106, 2);
							set_scan_config (3, A_SCAN, 278, 278, 86, 327, 106, 3);
							set_scan_config (4, A_SCAN, 307, 307, 86, 0, 212, 4);
							set_scan_config (5, A_SCAN, 278, 278, 86, 327, 212,5);
							set_scan_config (6, A_SCAN, 307, 307, 87, 0, 318, 6);
							set_scan_config (7, A_SCAN, 278, 278, 87, 327, 318, 7);
							break;
						default:break;
					}
					break;
				default:break;
			}

		}

	if (str)
	{
		g_free(str);
	}
	return ;
}

/* 三级菜单第一个 */
void draw3_data0(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf = 0.0;
	gchar *str = NULL;

	gfloat cur_value=0.0, lower, upper, step;
	guint digit, pos, unit, content_pos, menu_status = 0, tt;

	guint menu_on=0, i, temp_beam, t;

	int inet_sock;
	struct ifreq ifr;
	char ip_temp[256];

	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:/*Wizard -> Group -> Back P000 */
					draw3_popdown (NULL, 0, 1);
					if ((p->start_qty == 0)||(p->start_qty == 1)||(p->start_qty == 9))
					{
						gtk_widget_set_sensitive(p->eventbox30[0], FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0], FALSE);
					}
					else
					{
						gtk_widget_set_sensitive(p->eventbox30[0], TRUE);
						gtk_widget_set_sensitive(p->eventbox31[0], TRUE);
					}
					break;
				case 1:/*Wizard -> Focal Law -> Back p010 */
					draw3_popdown (NULL, 0, 1);
					if ((p->fstart_qty == 0)||(p->fstart_qty == 1)||(p->fstart_qty == 6))
					{
						gtk_widget_set_sensitive(p->eventbox30[0], FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0], FALSE);
					}
					else
					{
						gtk_widget_set_sensitive(p->eventbox30[0], TRUE);
						gtk_widget_set_sensitive(p->eventbox31[0], TRUE);
					}
					break;
				case 2:/* Wizard -> Calibration -> Back P020 */
					draw3_popdown (NULL, 0, 1);
					if ((p->cstart_qty == 0)||(p->cstart_qty == 1))
					{
						gtk_widget_set_sensitive(p->eventbox30[0], FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0], FALSE);
					}
					else
					{
						gtk_widget_set_sensitive(p->eventbox30[0], TRUE);
						gtk_widget_set_sensitive(p->eventbox31[0], TRUE);
					}
					break;
				case 3:/*Wizard -> Weld -> Back p030 */
					draw3_popdown (NULL, 0, 1);
					break;
				case 4:
					if ( !con2_p[0][4][0] )
						gtk_widget_hide (p->eventbox30[0]);
					gtk_widget_hide (p->eventbox31[0]);
					break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0: /* 增益 Gain P100 */
					switch (TMP(db_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;
						default:break;
					}
					if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
						content_pos = 6;
					else
						content_pos = 0;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = ((int)(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN)) - (int)(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))) / 100.0; 
						lower = 0.0 - (int)(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR)) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
						upper = GAIN_MAX - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
						step = tmpf;
						digit = 1;
						pos = 0;
						unit = UNIT_DB;
						draw3_digit_pressed (data_100, units[unit], cur_value ,
								lower, upper, step, digit, p, pos, content_pos);
					}
					else 
					{
						cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0; 
						digit = 1;
						pos = 0;
						unit = UNIT_DB;
						draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
					}
					break;
				case 1: /* 发射 Pulser P110  TAN1 和 focal connection P一样的 */
					switch (TMP(pulser_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					upper = (gfloat) (128 + 1 - LAW_VAL(Last_tx_elem));
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_PULSER));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 0;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_PULSER));
						digit = 0;
						pos = 0;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_stop (cur_value, str, digit, pos, 0);
					}
					g_free(str);
					break;
				case 2: /* Reveiver 接收器  P120 TAN1 */
					switch (TMP(receiver_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if (get_group_val (p_grp, GROUP_TX_RX_MODE) == PULSE_ECHO )
						/* 脉冲回波模式不可以调节 */
					{
						gtk_widget_set_sensitive (p->eventbox30[0], FALSE);
						gtk_widget_set_sensitive (p->eventbox31[0], FALSE);
					}
					upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 0;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
						digit = 0;
						pos = 0;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_stop (cur_value, str, digit, pos, 0);
					}
					g_free(str);
					break;
				case 3:/* Scan Offset P130 TAN1 */
					if (GROUP_VAL(group_mode) == PA_SCAN)	/* 脉冲回波模式不可以调节 */
					{
						gtk_widget_set_sensitive (p->eventbox30[0], FALSE);
						gtk_widget_set_sensitive (p->eventbox31[0], FALSE);
					}
					switch (TMP(scanoffset_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = GROUP_VAL(scan_offset)/10.0;
						lower = -100000.0;
						upper = 9999999.0;
						step = tmpf;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_digit_pressed (data_510, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = GROUP_VAL(scan_offset)/10.0;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 4:/* Set 80% P140 */
					draw3_popdown (NULL, 0, 1);
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/* Gate 选择调节哪个闸门 P200 TAN1 */
					p->x_pos = 630, p->y_pos = 119 - YOFFSET;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_200, NULL, 
								menu_content[GATE_POS + GROUP_VAL(gate_pos)],
								menu_content + GATE_POS, 3, 0, GROUP_VAL(gate_pos), 0);
					else 
						draw3_popdown (menu_content[GATE_POS + GROUP_VAL(gate_pos)], 0, 0);
					break;
				case 1:/* Alarm 选择哪个报警项 P210 TAN1 */
					p->x_pos = 624, p->y_pos = 119-YOFFSET;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						menu_on = 0;
						t = get_alarm_pos (pp->p_config);
						for (i = 0 ; i < 16; i++)
						{
							set_alarm_pos (pp->p_config, i);
							menu_on |= ((get_alarm_conditiona (pp->p_config)) !=0 ) << i;
						}
						set_alarm_pos (pp->p_config, (guchar)(t));
						draw3_pop_tt_on(data_210, NULL,
								menu_content[ALARM_POS + get_alarm_pos(pp->p_config)],
								menu_content + ALARM_POS, 16, 0, get_alarm_pos(pp->p_config), 0, menu_on, 16);
					}
					else 
						draw3_popdown(menu_content[ALARM_POS + get_alarm_pos(pp->p_config)], 0, 0);
					break;
				case 2:/* Output  P220  */
					p->x_pos = 581, p->y_pos = 120-YOFFSET;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						menu_on = 0x0;
						tt = get_output_pos(pp->p_config);
						for (i = 0 ; i < 3; i++) /*alarm#的值决定前三项后面是否跟[On]*/
						{
							set_output_pos(pp->p_config, i); 
							menu_on |= (get_output_alarm_qty(pp->p_config) != 0 ) << i;
						}

						for (i = 3 ; i < 5; i++) /*data的值决定后两项后面是否跟[On]*/
						{
							set_output_pos(pp->p_config, i); 
							menu_on |= (get_output_data(pp->p_config) != 0 ) << i;
						}
						set_output_pos (pp->p_config, tt);
						draw3_pop_tt_on (data_220, NULL, menu_content[OUTPUT_POS + get_output_pos(pp->p_config)],
								menu_content + OUTPUT_POS, 5, 0, get_output_pos(pp->p_config), 0, menu_on, 5);
					}
					else 
						draw3_popdown (menu_content[OUTPUT_POS + get_output_pos(pp->p_config)], 0, 0);
					break;

				case 3:/* Sizing Curves -> Setup  p230 */
					p->x_pos = 600, p->y_pos = 119-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						if (GROUP_VAL(curve_pos))
							menu_status = 0;
						else
							menu_status = 0x02;

						draw3_pop_tt (data_230, NULL, 
								menu_content[MODE_POS+GROUP_VAL(mode_pos)],
								menu_content + MODE_POS, 2, 0, GROUP_VAL(mode_pos), menu_status);
					}
					else 
						draw3_popdown (menu_content[MODE_POS+GROUP_VAL(mode_pos)], 0, 0);
					break;
				case 4:
					if ( !con2_p[2][4][0] )
						gtk_widget_hide (p->eventbox30[0]);
					gtk_widget_hide (p->eventbox31[0]);
					break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 0:/* Measurements -> Reading -> List P300 */
					p->x_pos = 398, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_300, NULL, 
								list1[get_reading_list(pp->p_config)],
								list, 2, 0, get_reading_list(pp->p_config), 0);
					else 
						draw3_popdown (list1[get_reading_list(pp->p_config)], 0, 0);
					break;
				case 1:/*Measurements -> Cursors -> Selection p310 */
					p->x_pos = 569, p->y_pos = 116-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_310, NULL, 
								menu_content[SELECTION + GROUP_VAL(selection)],
								menu_content + SELECTION, 9, 0, GROUP_VAL(selection), 0);
					else 
						draw3_popdown (menu_content[SELECTION + GROUP_VAL(selection)], 0, 0);

					break;

				case 2:/*Measurements -> Table -> Display Table  p320 */

					draw3_popdown (menu_content[OFF_ON + get_display_table(pp->p_config)], 0, 0);
					break;

				case 3:/*Measurements -> Thickness -> Source  p330 */
					p->x_pos = 602, p->y_pos = 117-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_330, NULL, 
								menu_content[SOURCE + GROUP_VAL(source)],
								menu_content + SOURCE, 9, 0, GROUP_VAL(source), 0);
					else 
						draw3_popdown (menu_content[SOURCE + GROUP_VAL(source)], 0, 0);
					break;
				case 4:/*Measurements -> Export -> Export Table p340 */
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw_dialog_all (DIALOG_REMARK);
					else
						draw3_popdown(NULL,0,1);
					break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{ 
				case 0:/* 选择显示模式ABSC P400 */
					p->x_pos = 415, p->y_pos = 90;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						if (get_group_qty(pp->p_config) != 3)
							menu_status = 0x200;
						if (GROUP_VAL(ascan_source) != 0)
							menu_status |= 0x72;
						draw3_pop_tt (data_400, NULL, 
								menu_content[DISPL + get_display_pos(pp->p_config)],
								menu_content + DISPLAY, 11, 0, get_display_pos(pp->p_config), menu_status);
					}
					else 
						draw3_popdown (menu_content[DISPL + get_display_pos(pp->p_config)], 0, 0);
					break;
				case 1:/*Display -> Overlay -> UT Unit  P410 */
					p->x_pos = 566, p->y_pos = 120-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_410, NULL, 
								menu_content[UTUNIT + GROUP_VAL(ut_unit)],
								menu_content + UTUNIT, 3, 0, GROUP_VAL(ut_unit), 0);
					else 
						draw3_popdown (menu_content[UTUNIT + GROUP_VAL(ut_unit)], 0, 0);
					gtk_widget_queue_draw(p->draw_area->drawing_area);

					break;

				case 2:/*Display -> Color -> Select  p420 */
					p->x_pos = 516, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_430, NULL, 
								menu_content[COL_SELECT + GROUP_VAL(col_select_pos)],
								menu_content +COL_SELECT, 3, 0, GROUP_VAL(col_select_pos), 0);
					else 
						draw3_popdown (menu_content[COL_SELECT + GROUP_VAL(col_select_pos)], 0, 0);

					break;

				case 3:/*Display -> Properties -> Scan P430 */
					p->x_pos = 570, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_440, NULL, 
								menu_content[PROP_SCAN + get_dis_prop_scan(pp->p_config)],
								menu_content + PROP_SCAN, 6, 0, get_dis_prop_scan(pp->p_config), 0);
					else 
						draw3_popdown (menu_content[PROP_SCAN + get_dis_prop_scan(pp->p_config)], 0, 0);
					break;

				case 4:
					if ( !con2_p[4][4][0] )
						gtk_widget_hide (p->eventbox30[0]);
					gtk_widget_hide (p->eventbox31[0]);
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/* 组设置 添加删除选择组 P500 */
					p->x_pos = 590, p->y_pos = 90;	/* 位置ok */
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						switch (get_group_qty(pp->p_config))
						{
							case 1:	menu_status = 0x03fc;break;
							case 2: menu_status = 0x01f8;break;
							case 3: menu_status = 0x01f0;break;
							case 4: menu_status = 0x01e0;break;
							case 5:	menu_status = 0x01c0;break;
							case 6: menu_status = 0x0180;break;
							case 7: menu_status = 0x0100;break;
							case 8: menu_status = 0x0001;break;
							default:break;
						}
						draw3_pop_tt (data_500, NULL, 
								menu_content[GROUP_P + get_current_group(pp->p_config) + 1],
								menu_content + GROUP_P, 10, 0, get_current_group(pp->p_config) + 1, menu_status);
					}
					else 
						draw3_popdown (menu_content[GROUP_P + get_current_group(pp->p_config) + 1], 0, 0);
					break;
				case 1:/* Scan Offset  P510 */
					switch (TMP(scanoffset_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = GROUP_VAL(scan_offset)/10.0;
							lower = -100000.0;
							upper = 9999999.0;
							step = tmpf;
							digit = 1;
							pos = 0;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = GROUP_VAL(scan_offset)/10.0*0.03937;
							lower = -100000.0*0.03937;
							upper = 9999999.0*0.03937;
							step = tmpf/100.0;
							digit = 1;
							pos = 0;
							unit = UNIT_INCH;
						}
						draw3_digit_pressed (data_510, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = GROUP_VAL(scan_offset)/10.0;
							digit = 1;
							pos = 0;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						else
						{
							cur_value = GROUP_VAL(scan_offset)/10.0*0.03937;
							digit = 1;
							pos = 0;
							unit = UNIT_INCH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					break;
				case 2:/*Probe/Part -> characterize -> FFT p520 */
					draw3_popdown (menu_content[OFF_ON + get_probe_fft (pp->p_config)], 0, 0);
					break;

				case 3: /* 被检则工件的几何形状  P530 */
					p->x_pos = 608, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						if(get_inspec_type (pp->p_config)==2)
						{
							draw3_pop_tt (data_530, NULL, 
									menu_content[GEOMETRY + get_part_geometry(p->p_config)],
									menu_content+GEOMETRY, 3, 0, get_part_geometry(p->p_config), 0x01);
						}
						else
							draw3_pop_tt (data_530, NULL, 
									menu_content[GEOMETRY + get_part_geometry(p->p_config)],
									menu_content+GEOMETRY, 3, 0, get_part_geometry(p->p_config), 0);
					}

					else 
						draw3_popdown (menu_content[GEOMETRY + get_part_geometry(p->p_config)], 0, 0);
					break;
				case 4:
					if ( !con2_p[5][4][0] )
						gtk_widget_hide (p->eventbox30[0]);
					gtk_widget_hide (p->eventbox31[0]);
					break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:/* 聚焦法则类型 线性扫查 角度(扇形)扫查
						  深度扫查 静态扫查  P600 TAN1 */
					p->x_pos = 450, p->y_pos = 116 - YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_600, NULL, 
								menu_content[L_CONFIG + LAW_VAL(Focal_type)],
								menu_content+LAW_CONFIG, 4, 0, LAW_VAL(Focal_type), 0x0);
					else 
						draw3_popdown (menu_content[L_CONFIG + LAW_VAL(Focal_type)], 0, 0);
					break;
				case 1:
					/* beam的最小角度 线性扫描就只有一个角度
					 * 角度扫差时候需可设置多个角度  P610
					 **/
					switch (TMP(min_angle_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
						/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
					{
						if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							/* 最大不能超过最大Angle_max */
							cur_value = LAW_VAL (Angle_min) / 100.0;
							/* 计算lower为妙 */
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
							TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
									LAW_VAL(Angle_beam_skew_min)) /
								LAW_VAL(Angle_beam_skew_step) + 1;
							temp_beam = temp_beam / TMP(beam_skew_num) - 1;
							lower = MAX (((gint)(LAW_VAL (Angle_max)) - 
										(gint)(temp_beam * LAW_VAL(Angle_step))) / 100.0, -89.9);
							if(gtk_widget_get_sensitive(pp->eventbox30[1]))
								upper = LAW_VAL (Angle_max) / 100.0;
							else
								upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 0;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_610, units[unit], cur_value , lower, 
									upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL (Angle_min) / 100.0;
							digit = 1;
							pos = 0;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /* 聚焦法则自动计算为OFF, Min.Angle 不可调节 */
					{
						cur_value = LAW_VAL (Angle_min) / 100.0;
						digit = 1;
						pos = 0;
						unit = UNIT_DEG;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(p->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0],FALSE);
					}
					break;
				case 2:/* 聚焦点的计算方式 0halfpath 1truedepth 2projection 3focalplane 4automatic P620 */
					p->x_pos = 555, p->y_pos = 116 - YOFFSET;
					if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
					{
						if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							if (get_group_val (p_grp, GROUP_TX_RX_MODE) == PITCH_CATCH )
							{
								menu_status = 0x0c;
							} else if (LAW_VAL (Focal_type) == DEPTH_SCAN)
							{
								menu_status = 0x1c;
							}
							else
							{
								menu_status = 0x10;
							}

							draw3_pop_tt (data_620, NULL, 
									menu_content[FOCAL_POINT_TYPE1 + LAW_VAL(Focal_point_type)],
									menu_content+ FOCAL_POINT_TYPE, 5, 0, LAW_VAL(Focal_point_type), menu_status);
						}
						else 
						{
							draw3_popdown (menu_content[FOCAL_POINT_TYPE1 + LAW_VAL(Focal_point_type)], 0, 0);
						}
					}
					else
					{
						draw3_popdown (menu_content[FOCAL_POINT_TYPE1 + LAW_VAL(Focal_point_type)], 0, 0);
						gtk_widget_set_sensitive(p->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0],FALSE);
					}
					break;
				case 3:/* 聚焦 阵元数量 P630 */
					switch (p->p_tmp_config->element_qty_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) /* 聚焦法则自动计算为 on 时，Element Qty 才可调节 */
					{
						if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							cur_value = LAW_VAL(Elem_qty);
							lower = 1.0;
							/* 计算最大激发阵元数 */
							upper = MIN( MIN(32.0, GROUP_VAL(probe.Elem_qty)), 
									(GROUP_VAL(probe.Elem_qty) - LAW_VAL(First_tx_elem) + 1));
							step = tmpf;
							digit = 0;
							pos = 0;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_630, units[unit], cur_value , lower,
									upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL(Elem_qty);
							digit = 0;
							pos = 0;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /* 聚焦法则自动计算为off时，Element Qty 不可以调节 */
					{
						cur_value = LAW_VAL(Elem_qty);
						digit = 0;
						pos = 0;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(p->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0],FALSE);
					}
					break;
				case 4:/* 自动计算聚焦法则 P640 */
					draw3_popdown (menu_content[OFF_ON + get_auto_focal (pp->p_config)], 0, 0);
					break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[7])
			{
				case 0:/*Scan -> Encoder -> Encoder p700 */
					p->x_pos = 638, p->y_pos = 130-YOFFSET;
					if ((get_inspec_type (pp->p_config) == 1) || (get_inspec_type (pp->p_config) == 2))
					{
						if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
							draw3_pop_tt (data_700, NULL, 
								menu_content[ENCODER+get_cur_encoder (pp->p_config)],
								menu_content+ENCODER, 2, 0,get_cur_encoder (pp->p_config), 0);
						else 
							draw3_popdown (menu_content[ENCODER+get_cur_encoder (pp->p_config)], 0, 0);
					}
					else
					{
						if ((get_inspec_source (pp->p_config) == 1) || (get_inspec_source (pp->p_config) == 2))
							draw3_popdown (menu_content[ENCODER+(get_inspec_source (pp->p_config)-1)], 0, 0);
						else
							draw3_popdown (menu_content[ENCODER+ 0 ], 0, 0);
						gtk_widget_set_sensitive(p->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0],FALSE);
					}
					break;
				case 1:/*Scan -> Inspection -> type  p710 */

					p->x_pos = 546, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_710, NULL, 
								menu_content[I_TYPE+get_inspec_type (pp->p_config)],
								menu_content+I_TYPE, 3, 0,get_inspec_type (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[I_TYPE+get_inspec_type (pp->p_config)], 0, 0);
					break;

				case 2:/*Scan -> Area -> scan start p720 */
					/* 当前步进 */
					switch (TMP(scan_start_reg))
					{
						case 0:	tmpf = 0.001 * get_area_scanresolution (pp->p_config); break;
						case 1:	tmpf = 0.01 * get_area_scanresolution (pp->p_config); break;
						case 2:	tmpf = 0.1 * get_area_scanresolution (pp->p_config); break;
						default:break;
					}
					if(get_inspec_type (pp->p_config)==0 || get_inspec_type (pp->p_config)==1)
						/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
					{
						if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = get_area_scanstart (pp->p_config)/1000.0;
								lower = -99999.0;
								upper = 99999.0;
								step = tmpf;
								digit = 2;
								pos = 0;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_scanstart (pp->p_config)/1000.0*0.03937;
								lower = -99999.0*0.03937;
								upper = 99999.0*0.03937;
								step = tmpf*0.03937;
								digit = 3;
								pos = 0;
								unit = UNIT_INCH;
							}

							draw3_digit_pressed (data_720, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = get_area_scanstart (pp->p_config)/1000.0;
								digit = 2;
								pos = 0;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_scanstart (pp->p_config)/1000.0*0.03937;
								digit = 3;
								pos = 0;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else  /* Inspection -> Type 选择 Helicoidal Scan  时 */
					{
						if(get_unit(pp->p_config) == UNIT_MM)
						{
							set_area_scanstart (pp->p_config, 0);
							digit = 0;
							pos = 0;
							unit = UNIT_MM;
						}
						else
						{
							set_area_scanstart (pp->p_config, 0);
							digit = 0;
							pos = 0;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

						gtk_widget_set_sensitive(p->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(p->eventbox31[0],FALSE);
					}
					break;


				case 3:/*Scan -> start -> start mode  p730 */
					p->x_pos = 550, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_730, NULL, 
								menu_content[START_MODE+get_start_mode (pp->p_config)],
								menu_content+START_MODE, 3, 0,get_start_mode (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[START_MODE+get_start_mode (pp->p_config)], 0, 0);

					break;

				case 4:/*Scan -> data -> storage  p740 */
					p->x_pos = 550, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_740, NULL, 
								menu_content[STORAGE+get_data_storage (pp->p_config)],
								menu_content+STORAGE, 4, 0,get_data_storage (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[STORAGE+get_data_storage (pp->p_config)], 0, 0);

					break;

				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> Storage  p800 */
					p->x_pos = 442, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_800, NULL, 
								menu_content[F_STORAGE_P+get_file_storage (pp->p_config)],
								menu_content+F_STORAGE, 2, 0,get_file_storage (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[F_STORAGE_P+get_file_storage (pp->p_config)], 0, 0);

					break;

				case 1:/*File -> report -> template p810 */
					p->x_pos = 502, p->y_pos = 100;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_810, NULL, 
								menu_content[TEMPLA_P+get_report_template (pp->p_config)],
								menu_content+TEMPLA, 1, 0,get_report_template (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[TEMPLA_P+get_report_template (pp->p_config)], 0, 0);


					break;

				case 2:/*File -> format -> user field  p820 */
					draw3_popdown (menu_content[OFF_ON + get_report_format_userfield (pp->p_config)], 0, 0);
					break;

				case 3:/*File -> user field -> select  p830 */
					p->x_pos = 630, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_830, NULL, 
								menu_content[F_SELECT+get_report_userfield_select (pp->p_config)],
								menu_content+F_SELECT, 10, 0,get_report_userfield_select (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[F_SELECT+get_report_userfield_select (pp->p_config)], 0, 0);
					break;
				case 4:/*File -> notes-> edit notes  p840 */
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw_dialog_all (DIALOG_EDIT_NOTES);
					else
						draw3_popdown(NULL,0,1);
					break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:/*Preferences -> pref.-> units p900 */
					p->x_pos = 570, p->y_pos = 118-YOFFSET;
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_900, NULL, 
								menu_content[P_UNITS+get_unit(pp->p_config)],
								menu_content+P_UNITS, 2, 0,get_unit(pp->p_config), 0);
					else 
						draw3_popdown (menu_content[P_UNITS+get_unit(pp->p_config)], 0, 0);
					break;
				case 1:/* Preferences -> system -> clock set 设置时间 p910 */
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw_dialog_all (DIALOG_TIME);
					else
						draw3_popdown (buffer + 11, 0, 0);
					break;
				case 2:/* 系统信息  P920*/
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw_dialog_all(DIALOG_SYSTEM_INFO);
					draw3_popdown(NULL,0,1);
					break;
				case 3:/*Preferences -> network -> IP Address  p930*/
					/* 格式化字符串 */
					if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw_dialog_all (DIALOG_IP);
						//gtk_label_set_text (GTK_LABEL (p->data3[0]), "192.168.1.95");
					}
					else
					{
						inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
						strcpy(ifr.ifr_name, "usb0");
						ioctl(inet_sock, SIOCGIFADDR, &ifr);
						sprintf(ip_temp,"%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
						gtk_label_set_text (GTK_LABEL (p->data3[0]), ip_temp);
					}

					g_sprintf (temp,"%s", con2_p[9][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (p->label3[0]), temp);
					//gtk_label_set_text (GTK_LABEL (p->data3[0]), ip_temp);

					/* 显示和隐藏控件 */
					gtk_widget_show (p->eventbox30[0]);
					gtk_widget_show (p->eventbox31[0]);
					gtk_widget_show (p->data3[0]);

					break;
				case 4:/*Preferences -> options -> mouse  p940 */
					if ( !con2_p[9][4][0] )
						gtk_widget_hide (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
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
	gfloat tmpf = 0.0;
	gchar *str;

	gfloat cur_value=0.0, lower, upper, step;
	guint digit, pos, unit, content_pos, menu_status = 0, temp_beam, tt;

	int inet_sock;
	struct ifreq ifr;
	static char mask_temp[256];
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> start  p001 */
					draw3_popdown (NULL, 1, 1);
					if( (pp->start_qty >0) && (pp->start_qty < 8) )
						draw3_popdown_offset (NULL, 1, 1, 6 );
					else if (pp->start_qty == 8)
						draw3_popdown_offset (NULL, 1, 1, 14 );
					break;

				case 1:/*Wizard -> Focal Law -> start p011 */
					draw3_popdown (NULL, 1, 1);
					if( (pp->fstart_qty >0) && (pp->fstart_qty < 5) )
						draw3_popdown_offset (NULL, 1, 1, 6 );
					else if (pp->fstart_qty == 5)
						draw3_popdown_offset (NULL, 1, 1, 13 );
					break;

				case 2:/* Wizard -> Calibration -> start p021 */
					draw3_popdown (NULL, 1, 1);
					if(pp->ctype_pos == 0)
					{
						if((get_inspec_type (pp->p_config)==0)&&(get_inspec_source (pp->p_config)==0))/*Type选择one-Line Scan   &&   Scan选择Time时，此键不可调*/
						{
							gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
						}
						else
						{
							if (pp->cstart_qty == 2)
								draw3_popdown_offset (NULL, 1, 1, 19 );
							else if (pp->cstart_qty == 3)
								draw3_popdown_offset (NULL, 1, 1, 30 );
							else if (pp->cstart_qty == 4)
								draw3_popdown_offset (NULL, 1, 1, 32 );
						}
					}
					else if(pp->ctype_pos == 1)
					{
						if( (pp->cstart_qty >1) && (pp->cstart_qty < 4) )
							draw3_popdown_offset (NULL, 1, 1, 19 );
						else if (pp->cstart_qty == 4)
						{
							if (pp->cmode_pos == 0)
								draw3_popdown_offset (NULL, 1, 1, 22 );
							else
								draw3_popdown_offset (NULL, 1, 1, 19 );
						}
						else if (pp->cstart_qty == 5)
						{
							if (pp->cmode_pos == 0)
								draw3_popdown_offset (NULL, 1, 1, 22 );
							else if (pp->cmode_pos == 1)
								draw3_popdown_offset (NULL, 1, 1, 32 );
							else if (pp->cmode_pos == 3)
								draw3_popdown_offset (NULL, 1, 1, 37 );
							else
								draw3_popdown_offset (NULL, 1, 1, 19 );
						}
						else if (pp->cstart_qty == 6)
						{
							if ((pp->cmode_pos == 0)||(pp->cmode_pos == 2))
								draw3_popdown_offset (NULL, 1, 1, 32 );
							else
								draw3_popdown_offset (NULL, 1, 1, 19 );
						}
						else
							draw3_popdown (NULL, 1, 1);
					}
					else if(pp->ctype_pos == 2)
					{
						if (pp->cstart_qty == 1)
							draw3_popdown (NULL, 1, 1);
						else if (pp->cstart_qty == 4)
							draw3_popdown_offset (NULL, 1, 1, 30 );
						else if (pp->cstart_qty == 5)
							draw3_popdown_offset (NULL, 1, 1, 32 );
						else
							draw3_popdown_offset (NULL, 1, 1, 19 );
					}
					break;
				case 3:/*Wizard -> Calibration -> start p031 */
					draw3_popdown (NULL, 1, 1);
					break;
				case 4:
					if ( !con2_p[0][4][1] )
						gtk_widget_hide (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* start 显示延时 P101 TAN1 等硬件 */
					switch (TMP(start_reg))
					{
						case 0:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / (gfloat)(GROUP_VAL(point_qty)); break;
						case 1:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 20.0 ; break;
						case 2:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 10.0 ; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || 
								(UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
//								lower = (BEAM_INFO(0,beam_delay) /1000.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								lower = 0;
								upper =	(MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								(step < 0.01) ? (step = 0.01) : (step = step);
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
//								lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								lower = 0;
								upper =	(MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								digit = 3;
								pos = 1;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 ;
							lower =	BEAM_INFO(0,beam_delay) / 1000.0;
							upper =	(MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0);
							step = tmpf;
							(step < 0.01) ? (step = 0.01) : (step = step);
							pos = 1;
							digit = 2;
							unit = UNIT_US;
						}
						draw3_digit_pressed (data_101, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 1;
							}
							else
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
								unit = UNIT_INCH;
								digit = 3;
								pos = 1;
							}
						}
						else
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 ;
							unit = UNIT_US;
							digit = 2;
							pos = 1;
						}
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 1: /* 收发模式 Tx/Rx Mode P111 TAN1 */
					pp->x_pos = 478, pp->y_pos = 203-YOFFSET; 	
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						/* PA时候如何能够选择 TT PC */
						if (GROUP_VAL (group_mode) == PA_SCAN)
							draw3_pop_tt (data_111, NULL, 
									menu_content[TX_RX_MODE + 4 + get_group_val (p_grp, GROUP_TX_RX_MODE)],
									menu_content + TX_RX_MODE, 3, 1, get_group_val (p_grp, GROUP_TX_RX_MODE), 0x05);
						else if (GROUP_VAL (group_mode) == UT_SCAN)
							draw3_pop_tt (data_111, NULL, 
									menu_content[TX_RX_MODE + 4 + get_group_val (p_grp, GROUP_TX_RX_MODE)],
									menu_content + TX_RX_MODE, 4, 1, get_group_val (p_grp, GROUP_TX_RX_MODE), 0x00);
					}
					else 
						draw3_popdown (menu_content[TX_RX_MODE + 4 + get_group_val (p_grp, GROUP_TX_RX_MODE)], 1, 0);
					break;
				case 2: /* 接收滤波 P121 TAN1 */
					pp->x_pos = 516, pp->y_pos = 201-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS))
							draw3_pop_tt (data_121, NULL, 
									menu_content[FILTER + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS)],
									menu_content + FILTER , 15, 1, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS), 0);
						else
							draw3_pop_tt (data_121, NULL, "None",
									menu_content + FILTER , 15, 1, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS), 0);
					}
					else 
					{
						if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS))
							draw3_popdown (menu_content[FILTER + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS)], 1, 0);
						else
							draw3_popdown ("None", 1, 0);
					}
					break;
				case 3:/* Index Offset P131 TAN1 */
					if (GROUP_VAL(group_mode) == PA_SCAN)	/* 脉冲回波模式不可以调节 */
					{
						gtk_widget_set_sensitive (pp->eventbox30[1], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[1], FALSE);
					}
					switch (TMP(indexoffset_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = GROUP_VAL(index_offset)/10.0;
						lower = -1000000.0;
						upper =  9999999.0;
						step = tmpf;
						digit = 1;
						pos = 1;
						unit = UNIT_MM;
						draw3_digit_pressed (data_511, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value =GROUP_VAL(index_offset)/10.0;
						digit = 1;
						pos = 1;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 4:/*Set Ref P141*/
					draw3_popdown (NULL, 1, 1);
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/* 闸门调节 Mode还是Position  P201 TAN1 */
					pp->x_pos = 584, pp->y_pos = 203 - YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt(data_201, NULL, 
								menu_content[PARAMETERS + GROUP_GATE_POS(parameters)],
								menu_content + PARAMETERS, 2, 1, GROUP_GATE_POS(parameters), 0);
					else 
						draw3_popdown(menu_content[PARAMETERS + GROUP_GATE_POS(parameters)], 1, 0);
					break;
				case 1:/* Group A 第一组报警 P211 TAN1 */
					pp->x_pos = 632, pp->y_pos = 201-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_211, NULL,menu_content[GROUPA + get_alarm_groupa(pp->p_config)],
								menu_content + GROUPA, 4, 1, get_alarm_groupa(pp->p_config), 0);
					else 
						draw3_popdown (menu_content[GROUPA + get_alarm_groupa(pp->p_config)], 1, 0);
					break;
				case 2: /* Gate -> Output -> Alarm or Group #  P221 */
					pp->x_pos = 604, pp->y_pos = 202-YOFFSET;
					if (get_output_pos(pp->p_config) < 3)	/* ALARM */
					{
						tt = get_output_alarm_qty (pp->p_config);
						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 1))
						{
							if (tt == 0)
								draw3_pop_tt_on (data_221, NULL, menu_content[ALARM1 + tt],
										menu_content + ALARM1, 18, 1, tt, 0, 0, 19);
							else if (tt == 16)
								draw3_pop_tt_on (data_221, NULL, menu_content[ALARM1 + 1],
										menu_content + ALARM1, 19, 1, 1,
										0, get_output_alarm(pp->p_config) << 2, 19);
							else if (tt == 1)
								draw3_pop_tt_on (data_221, NULL, menu_content[ALARM1 + get_1output_alarm_pos (pp->p_config) + 1],
										menu_content + ALARM1, 18, 1, get_1output_alarm_pos (pp->p_config) + 1,
										0, get_output_alarm(pp->p_config) << 2, 19);
							else
								draw3_pop_tt_on (data_221, NULL, menu_content[ALARM1 + 18],
										menu_content + ALARM1, 19, 1, 18,
										0, get_output_alarm(pp->p_config) << 2, 19);
						}
						else
						{ 
							if (tt == 0)
								draw3_popdown (menu_content[ALARM1 + 0], 1, 0);
							else if (tt == 16)
								draw3_popdown (menu_content[ALARM1 + 1], 1, 0);
							else if (tt == 1)
								draw3_popdown (menu_content[ALARM1 +
									get_1output_alarm_pos (pp->p_config) + 1], 1, 0);
							else
								draw3_popdown (menu_content[ALARM1 + 18], 1, 0);
						}
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							draw3_pop_tt (data_2211, NULL, 
									menu_content[GROUPA +
									get_output_group(pp->p_config)],
									menu_content + GROUPA, 4, 1, get_output_group(pp->p_config), 0);
						}
						else 
						{
							draw3_popdown (menu_content[GROUPA + get_output_group(pp->p_config)], 1, 0);
						}
						str = g_strdup_printf ("%s", con2_p[2][2][6]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
					}
					break;
				case 3:/*Sizing Curves -> Curve  p231 */
					pp->x_pos = 565, pp->y_pos = 202-YOFFSET;
					if (GROUP_VAL(mode_pos) == 0)  /* MODE 为Setup 时候 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
							draw3_pop_tt (data_231, NULL, 
									menu_content[CURVE_POS+GROUP_VAL(curve_pos)],
									menu_content + CURVE_POS, 4, 1, GROUP_VAL(curve_pos), 0);
						else 
							draw3_popdown (menu_content[CURVE_POS+GROUP_VAL(curve_pos)], 1, 0);
					}
					else if (GROUP_VAL(mode_pos) == 1)  /* MODE 为 Edit的时候 */
					{
						if (GROUP_VAL(curve_pos) == 1 || GROUP_VAL(curve_pos) == 3)
							/* DAC 或者 TCG 时候 */
						{
							if((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
							{
								draw3_pop_tt (data_2311, NULL, 
										menu_content[ ALARM_POS + GROUP_VAL(point_pos) ],
										menu_content + ALARM_POS, GROUP_VAL(dac_point_qty), 1, GROUP_VAL(point_pos), 0);
								str = g_strdup_printf ("%s", con2_p[2][3][10]);	
								gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
							}
							else
							{ 
								draw3_popdown (menu_content[ALARM_POS + GROUP_VAL(point_pos)], 1, 0);
								str = g_strdup_printf ("%s", con2_p[2][3][10]);	
								gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
							}
						}
						else if (GROUP_VAL(curve_pos) == 2) /* linear DAC */ 
						{
							/* 当前步进 */
							switch (pp->p_tmp_config->mat_atten_reg)
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;
								default: break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value = GROUP_VAL(mat_atten) / 1000.0;
										lower = 0.00;
										upper = 4002000.0/get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY);
										step = tmpf;
										digit = 2;
										pos = 1;
										unit = UNIT_DB_MM;
									}
									else
									{
										cur_value = GROUP_VAL(mat_atten) /1000.0 / 0.03937;
										lower = 0.00;
										upper = 4002000.0/get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/0.03937;
										step = tmpf/10.0;
										digit = 3;
										pos = 1;
										unit = UNIT_DB_INCH;
									}
								}
								else
								{
									cur_value = GROUP_VAL(mat_atten) / 1000.0/0.338;
									lower = 0.00;
									upper = 20.0;
									step = tmpf/10.0;
									digit = 2;
									pos = 1;
									unit = UNIT_DB_US;
								}
								draw3_digit_pressed (data_2312, units[unit], cur_value , lower, upper, step, digit, p, pos, 15);
							}
							else 
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value = GROUP_VAL(mat_atten) / 1000.0;
										digit = 2;
										pos = 1;
										unit = UNIT_DB_MM;
									}
									else
									{
										cur_value = GROUP_VAL(mat_atten) /1000.0 / 0.03937;
										digit = 3;
										pos = 1;
										unit = UNIT_DB_INCH;
									}
								}
								else
								{
									cur_value = GROUP_VAL(mat_atten) / 1000.0/0.338;
									digit = 2;
									pos = 1;
									unit = UNIT_DB_US;
								}
								draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
							}
						}
					}
					break;

				case 4:
					if ( !con2_p[2][4][1] )
						gtk_widget_hide (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Group p301 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][0][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					if(get_reading_list(pp->p_config)==0)
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Manual Weld 1");
					else
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Auto. Weld 1");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				case 1:/*Measurements -> Cursors -> Angle p311 */
					if( !GROUP_VAL(selection) )
					{
						/* 当前步进 */
						switch (pp->p_tmp_config->per_reference_reg)
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(per_reference)/100.0;
							lower = 0.0;
							upper = 100.0;
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_311, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(per_reference)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}

					}
					else if (GROUP_VAL(selection)==1 || GROUP_VAL(selection)==2 || GROUP_VAL(selection)==3)
					{
						/* 当前步进 */
						switch (pp->p_tmp_config->s_reference_reg)
						{
							case 0:	tmpf = 0.5; break;
							case 1:	tmpf = 2.5; break;
							case 2:	tmpf = 5.0; break;
							case 3:	tmpf = 7.5; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(s_reference)/100.0;
								lower = -1000000.0;
								upper =  1000000.0;
								step = tmpf;
								digit = 1;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(s_reference)/100.0*0.03937;
								lower = -1000000.0*0.03937;
								upper =  1000000.0*0.03937;
								step = tmpf;
								digit = 3;
								pos = 1;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3111, units[unit], cur_value , lower, upper, step, digit, p, pos, 6);
						}
						else 
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(s_reference)/100.0;
								digit = 1;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(s_reference)/100.0*0.03937;
								digit = 3;
								pos = 1;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
						}

						if(GROUP_VAL(selection)==1)
						{
							gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
						}

					}
					else if (GROUP_VAL(selection)==4 || GROUP_VAL(selection)==8)
					{
						/* 当前步进 */
						switch (pp->p_tmp_config->cursors_angle_reg)
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(cursors_angle)/100.0;
							lower = 0.0;
							upper = 90.0;
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_3112, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
						}
						else 
						{
							cur_value = GROUP_VAL(cursors_angle)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
						}
					}

					else if (GROUP_VAL(selection)==5||GROUP_VAL(selection)==6||GROUP_VAL(selection)==7)
					{
						/* 当前步进 */
						switch (TMP(cursors_angle_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_CURSORS_POS(angle)/100.0;
							lower = -30.0;
							upper = 40.0;
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_3113, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
						}
						else 
						{
							cur_value = GROUP_CURSORS_POS(angle)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
						}
					}
					gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;

				case 2:/*Measurements -> Table -> Entry Image p321*/

					draw3_popdown(menu_content[OFF_ON + get_entry_image(pp->p_config)],1,0);
					break;

				case 3:/*Measurements -> Thickness -> Min  p331 */
					/* 当前步进 */
					switch (pp->p_tmp_config->min_thickness_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if (get_unit(pp->p_config)==UNIT_MM)
						{
							cur_value = get_min_thickness(pp->p_config)/1000.0;
							lower = 0.05;
							upper = (get_max_thickness(pp->p_config)/1000.0)-0.01;
							step = tmpf;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_min_thickness(pp->p_config)/1000.0*0.03937;
							lower = 0.002;
							upper = (get_max_thickness(pp->p_config)/1000.0*0.03937)-0.005;
							step = tmpf/2.0;
							digit = 3;
							pos = 1;
							unit = UNIT_INCH;
						}

						draw3_digit_pressed (data_331, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if (get_unit(pp->p_config)==UNIT_MM)
						{
							cur_value = get_min_thickness(pp->p_config)/1000.0;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_min_thickness(pp->p_config)/1000.0*0.03937;
							digit = 3;
							pos = 1;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 4:
					if ( !con2_p[3][4][1] )
						gtk_widget_hide (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/* Ascan Sscan 时候是Group  P401 */
					pp->x_pos = 593, pp->y_pos = 175;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))/*选中401这个位置*/
					{
						if(get_display_pos(pp->p_config) == A_SCAN || 
								get_display_pos(pp->p_config) == S_SCAN)
						{	
							if (get_group_qty(pp->p_config) == 1)
								menu_status = 0x01;
							draw3_pop_tt (data_401, NULL, 
									menu_content[GROUP_POS + get_display_group(pp->p_config)],
									menu_content + GROUP_POS, 2, 1, get_display_group(pp->p_config), menu_status);
						}
						else if ((get_display_pos(pp->p_config) == C_SCAN) || 
								(get_display_pos(pp->p_config) == A_B_C_SCAN) ||
								(get_display_pos(pp->p_config) == A_C_CC_SCAN))
						{
							draw3_pop_tt (data_4011, NULL, 
									menu_content[C_SCAN1 + get_cscan_source(pp->p_config, 0)],
									menu_content + C_SCAN1, 4, 1, get_cscan_source(pp->p_config, 0), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if (get_display_pos(pp->p_config) == A_S_CC_SCAN)
						{
							draw3_pop_tt (data_4012, NULL, 
									menu_content[C_SCAN1+get_cscan_source(pp->p_config, 0)],
									menu_content + C_SCAN1, 5, 1, get_cscan_source(pp->p_config, 0), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if(get_display_pos(pp->p_config)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_pop_tt (data_4013, NULL, 
									menu_content[C_SCAN1+get_stripscan_data1(pp->p_config)],
									menu_content + C_SCAN1, 3, 1, get_stripscan_data1(pp->p_config), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}
						else if(get_display_pos(pp->p_config)==1 || get_display_pos(pp->p_config)==4 || get_display_pos(pp->p_config)==6)
						{
							gtk_widget_hide (pp->eventbox30[1]);
							gtk_widget_hide (pp->eventbox31[1]);
						}
					}
					else 
					{
						if(get_display_pos(pp->p_config) == A_SCAN || get_display_pos(pp->p_config) == S_SCAN)
						{
							draw3_popdown (menu_content[GROUP_POS+get_display_group(pp->p_config)], 1, 0);
						}
						else if ((get_display_pos(pp->p_config) == C_SCAN) || 
								(get_display_pos(pp->p_config) == A_B_C_SCAN) ||
								(get_display_pos(pp->p_config) == A_C_CC_SCAN))
						{
							draw3_popdown (menu_content[C_SCAN1+get_cscan_source(pp->p_config, 0)], 1, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if(get_display_pos(pp->p_config)==8)
							/*Display 为 A-S-[C]*/
						{
							draw3_popdown (menu_content[C_SCAN1+get_cscan_source(pp->p_config, 0)], 1, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if(get_display_pos(pp->p_config)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_popdown (menu_content[C_SCAN1+get_stripscan_data1(pp->p_config)], 1, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}
						else if(get_display_pos(pp->p_config)==1 || get_display_pos(pp->p_config)==4 || get_display_pos(pp->p_config)==6)
						{
							gtk_widget_hide (pp->eventbox30[1]);
							gtk_widget_hide (pp->eventbox31[1]);
						}
					}
					break;
				case 1:/* 窗口的栅格线 P411 */
					pp->x_pos = 600, pp->y_pos = 175;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_411, NULL, 
								menu_content[GRID + get_overlay_grid(pp->p_config)],
								menu_content + GRID, 6, 1, get_overlay_grid(pp->p_config), 0);
					else 
						draw3_popdown (menu_content[GRID + get_overlay_grid(pp->p_config)], 1, 0);
					break;
				case 2:/*Display -> Color -> start p421 */
					if(GROUP_VAL(col_select_pos) == 0)
					{
						/* 当前步进 */
						switch (TMP(color_start_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(col_start);
							lower = 0.0;
							upper = GROUP_VAL(col_end)-1.0;
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_431, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(col_start);
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}

					else if(GROUP_VAL(col_select_pos) == 1)
					{

						/* 当前步进 */
						switch (TMP(color_contrast_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(col_contrast);
							lower = 0.0;
							upper = 99.0;
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_4311, units[unit], cur_value , lower, upper, step, digit, p, pos, 6);
							//str = g_strdup_printf ("%s\n (%%) Δ%0.0f", con2_p[4][3][6],tmpf);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else 
						{
							cur_value = GROUP_VAL(col_contrast);
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
							//str = g_strdup_printf ("%s\n    (%%)", con2_p[4][3][6]);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}

					}
					else if (GROUP_VAL(col_select_pos) == 2)
					{

						/* 当前步进 */
						switch (TMP(min_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf =  1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(col_min)/1000.0;
							lower = 0.05;
							upper = GROUP_VAL(col_max)/1000.0-0.01;
							step = tmpf;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
							draw3_digit_pressed (data_4312, units[unit], cur_value , lower, upper, step, digit, p, pos, 8);
							//str = g_strdup_printf ("   %s\n(mm) Δ%0.2f", con2_p[4][3][8],tmpf);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else 
						{
							cur_value = GROUP_VAL(col_min)/1000.0;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
							//str = g_strdup_printf ("%s\n(mm)", con2_p[4][3][8]);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
					}
					break;

				case 3:/* Ascan->Color  p431 */
					switch(get_dis_prop_scan(pp->p_config))
					{
						case 0:	/* properties scan 选择Ascan时候 */
							pp->x_pos = 598, pp->y_pos = 175;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_441, NULL, 
										menu_content[PROP_COLOR + GROUP_VAL(ascan_color)],
										menu_content+PROP_COLOR, 6, 1, GROUP_VAL(ascan_color), 0x10);
							else 
								draw3_popdown (menu_content[PROP_COLOR + GROUP_VAL(ascan_color)], 1, 0);
							break;
						case 1:
							/* 当前步进 */
							switch (pp->p_tmp_config->compress_reg)
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf =  1.0; break;
								case 2:	tmpf = 10.0; break;
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
							{
								cur_value = get_b_compress (pp->p_config) / 1000.0;
								lower = 0.10;
								upper = 65536.0;
								step = tmpf;
								digit = 2;
								pos = 1;
								unit = UNIT_NONE;
								draw3_digit_pressed (data_4411, units[unit], cur_value , lower, upper, step, digit, p, pos, 6);
							}
							else 
							{
								cur_value = get_b_compress(pp->p_config) / 1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_NONE;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 6);

							}
							break;
						case 2:
							draw3_popdown(menu_content[OFF_ON + get_dis_prop_cratio(pp->p_config)],1,0);
							g_sprintf (temp,"%s", con2_p[4][3][8]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

							break;
						case 3:
							draw3_popdown (menu_content[OFF_ON + get_dis_prop_sinterpolation(pp->p_config)],1,0);
							g_sprintf (temp,"%s", con2_p[4][3][9]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
							break;
						case 4:
							pp->x_pos = 600, pp->y_pos = 210;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_4414, NULL, 
										menu_content[FFT_COLOR+get_fft_color (pp->p_config)],
										menu_content+FFT_COLOR, 4, 1, get_fft_color (pp->p_config), 0x08);
							else 
								draw3_popdown (menu_content[FFT_COLOR+get_fft_color (pp->p_config)], 1, 0);
							g_sprintf (temp,"%s", con2_p[4][3][10]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
							break;
						case 5:
							pp->x_pos = 575, pp->y_pos = 230;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_4415, NULL, 
										menu_content[ORIENTATION + get_dis_prop_strip_orientation(pp->p_config)],
										menu_content+ORIENTATION, 2, 1, get_dis_prop_strip_orientation(pp->p_config), 0);
							else 
								draw3_popdown (menu_content[ORIENTATION + get_dis_prop_strip_orientation(pp->p_config)], 1, 0);
							g_sprintf (temp,"%s", con2_p[4][3][11]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
							break;

						default:break;
					}
					break;
				case 4:
					if ( !con2_p[4][4][1] )
						gtk_widget_hide (p->eventbox30[1]);
					gtk_widget_hide (p->eventbox31[1]);

					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Group Mode  p501 */
					pp->x_pos = 454, pp->y_pos = 203-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_501, NULL, 
								menu_content[GROUP_MODE_P + GROUP_VAL(group_mode)],
								menu_content + GROUP_MODE, 2, 1, GROUP_VAL(group_mode), 0);
					else 
						draw3_popdown (menu_content[GROUP_MODE_P + GROUP_VAL(group_mode)], 1, 0);
					break;
				case 1:/* Index Offset P511 */
					switch (TMP(indexoffset_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = GROUP_VAL(index_offset)/10.0;
							lower = -1000000.0;
							upper =  9999999.0;
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{						
							cur_value = GROUP_VAL(index_offset)/10.0*0.03937;
							lower = -1000000.0*0.03937;
							upper =  9999999.0*0.03937;
							step = tmpf/10.0;
							digit = 1;
							pos = 1;
							unit = UNIT_INCH;
						}
						draw3_digit_pressed (data_511, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value =GROUP_VAL(index_offset)/10.0;
							digit = 1;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value =GROUP_VAL(index_offset)/10.0*0.03937;
							digit = 1;
							pos = 1;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 2:/*probe/part -> characterize -> gain P521 */
					/* 当前步进 */
					switch (TMP(db_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;
						default:break;
					}
					if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
						content_pos = 6;
					else
						content_pos = 0;

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0; 
						lower = 0.0 - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
						upper = 80.0 - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
						step = tmpf;
						digit = 1;
						pos = 1;
						unit = UNIT_DB;
						draw3_digit_pressed (data_100, units[unit], cur_value ,
								lower, upper, step, digit, p, pos, content_pos);
					}
					else 
					{
						cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0; 
						digit = 1;
						pos = 1;
						unit = UNIT_DB;
						draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
					}
					break;

				case 3:/*probe/part -> parts -> thickness p531 */
					/* 当前步进 */
					switch (TMP(part_thickness_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = get_part_thickness(pp->p_config) / 1000.0;
							lower = 0.05;
							if (get_part_geometry(pp->p_config) == PLATE_PART)
								upper = 1000.0;
							else if (get_part_geometry(pp->p_config) == OD_PART)
								upper = get_part_diameter (pp->p_config) / 2000.0;
							else
								upper = (get_part_diameter (pp->p_config) - 10.0) / 2000.0;
							step = tmpf;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_part_thickness(pp->p_config) / 1000.0*0.03937;
							lower = 0.002;
							//upper = get_part_diameter(pp->p_config) / 2000.0*0.03937;
							if (get_part_geometry(pp->p_config) == PLATE_PART)
								upper = 1000.0 *0.03937 ;
							else if (get_part_geometry(pp->p_config) == OD_PART)
								upper = get_part_diameter (pp->p_config) / 2000.0 *0.03937 ;
							else
								upper = (get_part_diameter (pp->p_config) - 10.0) / 2000.0 *0.03937;
							step = tmpf*0.03937;
							digit = 3;
							pos = 1;
							unit = UNIT_INCH;
						}
						draw3_digit_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = get_part_thickness(pp->p_config) / 1000.0;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_part_thickness(pp->p_config) / 1000.0*0.03937;
							digit = 3;
							pos = 1;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;

				case 4:
					if ( !con2_p[5][4][1] )
						gtk_widget_hide (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0: /* 第一个发射阵元在连接器上面的位置 与UT设置的pulser一样 P601  TAN1 */
					switch (TMP(pulser_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					upper = (gfloat) (128 + 1 - LAW_VAL(Last_tx_elem));
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_PULSER));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_PULSER));
						digit = 0;
						pos = 1;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_stop (cur_value, str, digit, pos, 0);
					}
					g_free(str);
					break;
				case 1:/* 角度扫查的最大角度 出射角度 P611 */
					switch (TMP(max_angle_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((LAW_VAL(Focal_type) == AZIMUTHAL_SCAN) &&
							(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON))
						/* 聚焦法则自动计算开启时并且是角度扫查, Max Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							/* 计算lower为妙 */
							cur_value = LAW_VAL(Angle_max) / 100.0;
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
							TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
									LAW_VAL(Angle_beam_skew_min)) /
								LAW_VAL(Angle_beam_skew_step) + 1;
							temp_beam = temp_beam / TMP(beam_skew_num);
							lower = LAW_VAL (Angle_min) / 100.0;
							upper = MIN ((gint)(LAW_VAL (Angle_min) +
										(gint)(temp_beam * LAW_VAL(Angle_step))) / 100.0, 89.9);
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_611, units[unit], cur_value , lower, upper,
									step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL(Angle_max) / 100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						cur_value = LAW_VAL(Angle_max) / 100.0;
						digit = 1;
						pos = 1;
						unit = UNIT_DEG;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					break;
				case 2: /* P621
						   0 Halfpath  时候 这里设置positon start
						   1 TrueDepth 时候 这里设置positon start
						   2 Halfpath  时候 这里设置offset  start
						   3 Halfpath  时候 这里设置offset  start
						   4 Automatic 时候 不能设置 */

					if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
						/* 聚焦法则自动计算开启时, Max Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
								/*type为half path 或 true depth 时*/
							{
								switch (TMP(positions_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								cur_value = LAW_VAL(Position_start) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 1;
								pos = 1;
								unit = UNIT_MM;

								draw3_digit_pressed (data_621, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 0);
							}
							else if((LAW_VAL(Focal_point_type)==PROJECTION_P)||(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
								/*type 为 projection  或 focal plane 时*/
							{
								switch (TMP(offsets_reg))
								{
									case 0:	tmpf = 0.5; break;
									case 1:	tmpf = 5.0; break;
									case 2:	tmpf = 50.0; break;
									default:break;
								}
								cur_value = LAW_VAL(Offset_start) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;

								draw3_digit_pressed (data_6211, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 6);
							}
							else
							{
								cur_value = LAW_VAL(Position_start) / 1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

								gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
							}

						}
						else 
						{
							if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
								/*type为half path 或 true depth 时*/
							{
								cur_value = LAW_VAL(Position_start) / 1000.0;
								digit = 1;
								pos = 1;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
							else if((LAW_VAL(Focal_point_type)==PROJECTION_P)||(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
								/*type 为 projection  或 focal plane 时*/
							{
								cur_value = LAW_VAL(Offset_start) / 1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
							}
							else
							{
								cur_value = LAW_VAL(Position_start) / 1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

								gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
							}
						}
					}
					else
					{
						if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
						{
							cur_value = LAW_VAL(Position_start) / 1000.0;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						else if((LAW_VAL(Focal_point_type)==PROJECTION_P)||(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
						{
							cur_value = LAW_VAL(Offset_start) / 1000.0;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
						}
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					break;
				case 3:/* 聚焦法则的第一个阵元编号 P631 */
					switch (TMP(first_element_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)/* 聚焦法则自动计算开启时, First Element才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = LAW_VAL(First_tx_elem);
							lower = 1.0;
							/* 计算最大值 */
							upper = (gfloat)MIN((GROUP_VAL(probe.Elem_qty) - LAW_VAL(Elem_qty) + 1),
								  (128 + 1 - get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_PULSER)));
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_631, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL(First_tx_elem);
							digit = 0;
							pos = 1;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /* 聚焦法则自动计算为off时, First Element 不可以调节 */
					{
						cur_value = LAW_VAL(First_tx_elem);
						digit = 0;
						pos = 1;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					break;
				case 4:/* 读取聚集法则 P641*/
					if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON )
					{
						draw3_popdown(NULL, 1, 1);
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							draw_dialog_all (DIALOG_LAW_READ);
						}
						else
							draw3_popdown(NULL, 1, 1);
					}
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> polarity  p701 */
					pp->x_pos = 596, pp->y_pos = 204-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_701, NULL, 
								menu_content[POLARITY + get_enc_polarity (pp->p_config, get_cur_encoder (pp->p_config))],
								menu_content + POLARITY, 2, 1, get_enc_polarity (pp->p_config, get_cur_encoder (pp->p_config)), 0);
					else 
						draw3_popdown (menu_content[POLARITY + get_enc_polarity (pp->p_config, get_cur_encoder (pp->p_config))], 1, 0);
					if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
					{
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					break;

				case 1:/*Scan -> Inspection -> scan  p711 */
					pp->x_pos = 580, pp->y_pos = 201-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if(get_inspec_type (pp->p_config)==1)
							draw3_pop_tt (data_711, NULL, 
									menu_content[I_SCAN+get_inspec_source (pp->p_config)],
									menu_content+I_SCAN, 3, 1, get_inspec_source (pp->p_config), 0x01);
						else
							draw3_pop_tt (data_711, NULL, 
									menu_content[I_SCAN+get_inspec_source (pp->p_config)],
									menu_content+I_SCAN, 3, 1, get_inspec_source (pp->p_config), 0);
					}
					else 
						draw3_popdown (menu_content[I_SCAN+get_inspec_source (pp->p_config)], 1, 0);
					break;

				case 2:/*Scan -> Area -> scan end  p721 */
					/* 当前步进 */
					switch (TMP(scan_end_reg))
					{
						case 0:	tmpf = 0.001 * get_area_scanresolution (pp->p_config); break;
						case 1:	tmpf = 0.01 * get_area_scanresolution (pp->p_config); break;
						case 2:	tmpf = 0.1 * get_area_scanresolution (pp->p_config); break;
						default:break;
					}

					if(get_inspec_type (pp->p_config)==0 || get_inspec_type (pp->p_config)==1)
						/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = get_area_scanend (pp->p_config)/1000.0;
								lower = get_area_scanstart (pp->p_config)/1000.0;
								upper = 100000.0;
								step = tmpf;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_scanend (pp->p_config)/1000.0 * 0.03937;
								lower = get_area_scanstart (pp->p_config)/1000.0 * 0.03937;
								upper = 100000.0;
								step = tmpf*0.03937;
								digit = 3;
								pos = 1;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_721, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = get_area_scanend (pp->p_config)/1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_scanend (pp->p_config)/1000.0*0.03937;
								digit = 3;
								pos = 1;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else  /* Inspection -> Type 选择 Helicoidal Scan  时  ????get_area_scanstart (pp->p_config)的值有待确定 */
					{
						if(get_unit(pp->p_config) == UNIT_MM)
						{
							set_area_scanstart (pp->p_config, 346);
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							set_area_scanstart (pp->p_config, 346.0*0.03937);
							digit = 3;
							pos = 1;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					break;
				case 3:/*Scan -> start -> pause  p731 */
					draw3_popdown (menu_content[OFF_ON + get_start_pause (pp->p_config)], 1, 0);
					break;

				case 4:/*Scan -> data -> inspec.data  p741 */
					pp->x_pos = 545, pp->y_pos = 202;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_741, NULL, 
								menu_content[INSPEC_DATA + get_data_inspec_data (pp->p_config)],
								menu_content + INSPEC_DATA, 2, 1, get_data_inspec_data (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[INSPEC_DATA + get_data_inspec_data (pp->p_config)], 1, 0);


					break;

				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/* 打开文件 P801 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw_dialog_all (DIALOG_FILE_OPEN);
					draw3_popdown(NULL,1,1);
					break;

				case 1:/*File -> report -> file name p811 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw_dialog_all (DIALOG_FILE_NAME);
					}
					else
					{
						draw3_popdown (get_file_name_info (pp->p_config), 1, 0);
					}
					break;


				case 2:/*File -> format -> probe  p821 */
					draw3_popdown (menu_content[OFF_ON + get_report_format_probe (pp->p_config)], 1, 0);
					break;

				case 3:/*File -> user field -> enable  p831 */
					draw3_popdown (menu_content[OFF_ON + get_report_userfield_enable (pp->p_config, get_report_userfield_select (pp->p_config))], 1, 0);
					break;

				case 4:/*File -> notes-> edit header p841 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw_dialog_all (DIALOG_EDIT_HEADER);
					else
						draw3_popdown(NULL,1,1);
					break;

				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> bright 901 */
					/* 当前步进 */
					switch (pp->p_tmp_config->bright_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = get_bright (pp->p_config);
						lower = 1.0;
						upper = 100.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						unit = UNIT_BFH;
						draw3_digit_pressed (data_901, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = get_bright (pp->p_config);
						digit = 0;
						pos = 1;
						unit = UNIT_BFH;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 1: /* Preferences -> system -> data set 设置日期  p911*/
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw_dialog_all (DIALOG_DATE);
					}
					else
					{
						memcpy (temp, buffer, 11);
						temp[10] = 0;
						draw3_popdown (temp, 1, 0);
                    }
					break;
				case 2:/*Preferences -> service -> File Manager  P921 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw_dialog_all(DIALOG_FILE_MANAGE);
					draw3_popdown(NULL,1,1);
					break;
				case 3:/*Preferences -> Network -> IP Address  p931*/
					/* 格式化字符串 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw_dialog_all (DIALOG_MASK);
					}
					else
					{
						inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
						strcpy(ifr.ifr_name, "usb0");
						ioctl(inet_sock, SIOCGIFNETMASK, &ifr);
						sprintf(mask_temp,"%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), mask_temp);
					}
					g_sprintf (temp,"%s", con2_p[9][4][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				case 4:/*Preferences -> options -> Option Key  p941 */
					if ( !con2_p[9][4][1] )
						gtk_widget_hide (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
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
	gfloat tmpf = 0.0, tmpfm;
	gchar *str = NULL;
	guint menu_status = 0;

	gfloat cur_value=0.0, lower=0, upper=0, step=0, max_tmp=0, max_tmp1=0, temp_beam=0;
	guint digit = 0, pos, unit = 0, temp_qty =0, temp_pos = 0;

	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	//	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:	/* p002 */
					pp->x_pos = 590, pp->y_pos = 330;

					if ( pp->start_qty == 0 )
					{
						gtk_widget_hide (pp->eventbox30[2]);
						gtk_widget_hide (pp->eventbox31[2]);
					}
					else if ( pp->start_qty == 1 )
					{
						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))	
						{
							//进入这块就是用户点击了选项菜单
							//如果group数量是1~7，则显示弹出菜单为add和modify
							if ( ( pp->p_config->groupQty >= 1 ) && ( pp->p_config->groupQty < 8 ) )
							{
								//点击按钮，弹出菜单。
								draw3_pop_tt (data_0021, NULL, 
									menu_content[OPERATION + pp->operation_pos],
									menu_content + OPERATION, 2, 2, pp->operation_pos, 0);
							}
							//如果group数量是8，则显示弹出菜单为modify
							else if ( pp->p_config->groupQty == 8 )
							{
								draw3_pop_tt (data_0021, NULL, 
									menu_content[OPERATION + pp->operation_pos + 1],
									menu_content + OPERATION + 1, 1, 2, pp->operation_pos, 0);
							}
                        }
						else 
						{
							//在本块，代表没有点击的常规显示

							//group数量在1~7那时，默认显示为add
							//group数量在8那时，默认显示为modify
							if ( ( pp->p_config->groupQty >= 1 ) && ( pp->p_config->groupQty < 8 ) )
							{
								draw3_popdown (menu_content[OPERATION + pp->operation_pos], 2, 0);
							}
							else if ( pp->p_config->groupQty == 8 )
							{
								draw3_popdown (menu_content[OPERATION + pp->operation_pos + 1], 2, 0);
							}

							//判断用户是选择了add，还是选择了modify，
							switch ( pp->operation_pos )
							{
								//选中第一个选项，
								//如果group数量在1~7那时，选中是add
								//如果group数量在8那时，选中是modify
								case 0: 
									if ( ( pp->p_config->groupQty >= 1 ) && ( pp->p_config->groupQty < 8 ) ) 
									{
										g_group_wizard_struct.operation = add;
									}
									else
									{
										g_group_wizard_struct.operation = modify;
									}
									break;
								
								//选中第二个选项
								//如果有二个选项，第一个为add，第二个为modify。
								//所以选中这个选项的，必是选中modify。
								case 1:
									g_group_wizard_struct.operation = modify;
									break;
							    
								//不能，并不会出这种情况	 
								default:
									break;
							}
						}
					}
					else if ( pp->start_qty == 2 )
					{
						//if ( g_group_wizard_struct.operation == modify )
						{
						   if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
						    {	
								//根据有多少个group，在那个弹出菜单，显示多少个group选项
								draw3_pop_tt (data_0022, NULL, 
									menu_content[WGROUP + pp->wgroup_pos],
									menu_content + WGROUP,pp->p_config->groupQty,2,pp->wgroup_pos,0);
						    }
						    else 
						    {
							    //显示用户选中那个选项
							    draw3_popdown (menu_content[WGROUP + pp->wgroup_pos], 2, 0);
						
								if ( g_group_wizard_struct.operation == modify )
								{
									//把指定的group copy到g_tmp_group_struct
									//，修改就修改g_tmp_group_struct

									memset(&g_tmp_group_struct,0,sizeof(struct _Group));

									memcpy(&g_tmp_group_struct,&(pp->p_config->group[pp->wgroup_pos]),sizeof(struct _Group));
								}
								else if ( g_group_wizard_struct.operation == add)
								{
									//
									memset(&g_tmp_group_struct,0,sizeof(struct _Group));

									memcpy(&g_tmp_group_struct,&(pp->p_config->group[pp->p_config->groupId]),sizeof(struct _Group));
								}

                            }

						    str = g_strdup_printf ("%s", con2_p[0][0][7]);	
						    gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						    if (str)
								g_free (str);
					    }
					}
					else if ( pp->start_qty == 3 )
					{       
						//if ( g_group_wizard_struct.operation == modify)
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								draw3_pop_tt (data_0023, NULL, 
									menu_content[GROUP_MODE_P + (g_tmp_group_struct.group_mode)],
									menu_content + GROUP_MODE, 2, 2, (g_tmp_group_struct.group_mode), 0);
							}
							else 
							{
								draw3_popdown (menu_content[GROUP_MODE_P + (g_tmp_group_struct.group_mode)], 2, 0);
                            }
							
							str = g_strdup_printf ("%s", con2_p[0][0][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							if (str)
								g_free (str);
						}
					}
					else if (pp->start_qty == 4)
					{
						//if ( g_group_wizard_struct.operation == modify )
						{
							if( g_tmp_group_struct.group_mode == PA_SCAN )
							{
								//计算步进
								switch (TMP(connection_P_reg))
								{
									case 0:	tmpf = 1.0; break;
									case 1:	tmpf = 10.0; break;
									case 2:	tmpf = 100.0; break;
									default:break;
								}
								upper = (gfloat) (128 + 1 - LAW_VAL(Last_tx_elem));
								if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_PULSER));
									lower = 1.0;
									step = tmpf;
									digit = 0;
									pos = 2;
									str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
									draw3_digit_pressed (data_00241, str, cur_value , lower, upper, step, digit, p, pos, 9);
								}
								else 
								{
									cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_PULSER));
									digit = 0;
									pos = 2;
									str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
									draw3_digit_stop (cur_value, str, digit, pos, 9);
								}
								g_free (str);
							}
							else
							{
								pp->x_pos = 478, pp->y_pos = 203-YOFFSET; 	
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									/* PA时候如何能够选择 TT PC */
									if ( g_tmp_group_struct.group_mode == PA_SCAN )
									{
										draw3_pop_tt (data_00242, NULL, 
											menu_content[TX_RX_MODE + 4 + 
											get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE)],
											menu_content + TX_RX_MODE, 3, 2, get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE), 0x05);
									}
									else if ( g_tmp_group_struct.group_mode  == UT_SCAN )
									{
										draw3_pop_tt (data_00242, NULL, 
											menu_content[TX_RX_MODE + 4 + get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE)],
											menu_content + TX_RX_MODE, 4, 2, get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE), 0x00);
									}
								}
								else 
								{
									draw3_popdown (menu_content[TX_RX_MODE + 4 + 
											get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE)], 2, 0);
								}

								str = g_strdup_printf ("%s", con2_p[0][0][16]);	
								gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
								if (str)
									g_free (str);							
							}
						}
					}
					else if (( pp->start_qty == 5 )||( pp->start_qty == 6 ))
					{
						//if ( g_group_wizard_struct.operation == modify ) 
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								draw3_pop_tt (data_0025, NULL, 
										menu_content[PROB_SELECT + get_probe_select (pp->p_config)],
										menu_content + PROB_SELECT, 2, 2, get_probe_select (pp->p_config), 0);
							}
							else 
							{
								draw3_popdown (menu_content[PROB_SELECT + get_probe_select (pp->p_config)], 2, 0);
							}

							str = g_strdup_printf ("%s", con2_p[0][0][10]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							if (str)
								g_free (str);
						}
					}

					else if (pp->start_qty == 7)
					{
						//if ( g_group_wizard_struct.operation == modify )
						{
							switch (TMP(scanoffset_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 1.0; break;
								case 2:	tmpf = 5.0; break;
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								cur_value = g_tmp_group_struct.scan_offset/10.0;
								lower = -100000.0;
								upper = 9999999.0;
								step = tmpf;
								digit = 1;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_pressed (data_0027, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);
							}
							else 
							{
								cur_value = g_tmp_group_struct.scan_offset/10.0;
								digit = 1;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
							}
						}
					}
					else if (pp->start_qty == 8)
					{
						//if (g_group_wizard_struct.operation == modify)
						{
							draw3_popdown_offset (NULL, 2, 1, 15 );
						}
					}
					else if( pp->start_qty == 9 )
					{	
						if ( g_group_wizard_struct.operation == modify ) 
						{	
							//
							pp->p_config->groupId = pp->wgroup_pos;
							//用户点击finish按钮后，把临时的group copy去
							memcpy(&(pp->p_config->group[pp->p_config->groupId]),&g_tmp_group_struct,sizeof(struct _Group));
						}
						else if ( g_group_wizard_struct.operation == add )
						{
							//之所以这么写，
							//是因为groupQty代表group的数量，是从1开始数的。
							//groudId代表，选中是那个group，是从0开始数的。
							//新增group，是在最后那个group后面新增的。
							pp->p_config->groupId = pp->p_config->groupQty;

							(pp->p_config->groupQty)++;

							memcpy(&(pp->p_config->group[pp->p_config->groupId]),&g_tmp_group_struct,sizeof(struct _Group));
						}
                        						
						pp->start_qty = 0;
						draw3_data2(p);
					}
					
					break;
				case 1: /* p012 */
					pp->x_pos = 480, pp->y_pos = 300;
					if (pp->fstart_qty == 1)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_600, NULL, 
									menu_content[L_CONFIG + LAW_VAL(Focal_type)],
									menu_content+LAW_CONFIG, 4, 2, LAW_VAL(Focal_type), 0x0);
						else 
							draw3_popdown (menu_content[L_CONFIG + LAW_VAL(Focal_type)], 2, 0);
					}
					else if (pp->fstart_qty == 2)
					{
						g_sprintf (TMP (velocity_data[0]), "LW:%dm/s", get_material_lw(pp->p_config) / 100);
						g_sprintf (TMP (velocity_data[1]), "SW:%dm/s", get_material_sw(pp->p_config) / 100);
						g_sprintf (TMP (velocity_data[2]), "%d m/s", get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 100);
						const gchar **tp = (const gchar **)(TMP(velocity_data_p));
						if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) == get_material_lw(pp->p_config )) 
						{
							temp_pos = 0;
							temp_qty = 2;
						}
						else if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) == get_material_sw(pp->p_config)) 
						{
							temp_pos = 1;
							temp_qty = 2;
						}
						else
						{
							temp_pos = 2;
							temp_qty = 3;
						}

						if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{

								if (get_material_sw(pp->p_config ) == 0)
									menu_status = 0x02;

								draw3_pop_tt (data_634, NULL, 
										tp[temp_pos],	tp,	temp_qty, 2, temp_pos, menu_status);
							}
							else 
							{
								draw3_popdown (TMP(velocity_data[temp_pos]), 2, 0);
							}
						}
						else
						{
							draw3_popdown ( TMP(velocity_data[temp_pos]), 2, 0);
							gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
						}
						str = g_strdup_printf ("%s", con2_p[0][1][7]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						if (str)
							g_free (str);
					}
					else if (pp->fstart_qty == 3)
					{
						switch (pp->p_tmp_config->element_qty_reg)
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) /* 聚焦法则自动计算为 on 时，Element Qty 才可调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								cur_value = LAW_VAL(Elem_qty);
								lower = 1.0;
								/* 计算最大激发阵元数 */
								upper = MIN( MIN(32.0, GROUP_VAL(probe.Elem_qty)), 
										(GROUP_VAL(probe.Elem_qty) - LAW_VAL(First_tx_elem) + 1));
								step = tmpf;
								digit = 0;
								pos = 2;
								unit = UNIT_NONE;
								draw3_digit_pressed (data_630, units[unit], cur_value , lower,
										upper, step, digit, p, pos, 8);
							}
							else 
							{
								cur_value = LAW_VAL(Elem_qty);
								digit = 0;
								pos = 2;
								unit = UNIT_NONE;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
							}
						}
						else /* 聚焦法则自动计算为off时，Element Qty 不可以调节 */
						{
							cur_value = LAW_VAL(Elem_qty);
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
							gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
						}
					}
					else if(pp->fstart_qty == 4)
					{
						switch (TMP(min_angle_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
							/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								/* 最大不能超过最大Angle_max */
								cur_value = LAW_VAL (Angle_min) / 100.0;
								/* 计算lower为妙 */
								temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
								TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
										LAW_VAL(Angle_beam_skew_min)) /
									LAW_VAL(Angle_beam_skew_step) + 1;
								temp_beam = temp_beam / TMP(beam_skew_num);
								lower = MAX (((gint)(LAW_VAL (Angle_max)) - 
											(gint)(temp_beam * LAW_VAL(Angle_step))) / 100.0, -89.9);
								upper = LAW_VAL (Angle_max) / 100.0;
								step = tmpf;
								digit = 1;
								pos = 2;
								unit = UNIT_DEG;
								draw3_digit_pressed (data_610, units[unit], cur_value , lower, 
										upper, step, digit, p, pos, 10);
							}
							else 
							{
								cur_value = LAW_VAL (Angle_min) / 100.0;
								digit = 1;
								pos = 2;
								unit = UNIT_DEG;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
							}
						}
						else /* 聚焦法则自动计算为OFF, Min.Angle 不可调节 */
						{
							cur_value = LAW_VAL (Angle_min) / 100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
							gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[2]);
						gtk_widget_hide (pp->eventbox31[2]);
					}
					break;

				case 2:/*Wizard -> Calibration -> Type  p022 */
					pp->x_pos = 570, pp->y_pos = 295;
					switch(pp->cstart_qty)
					{
						case 0:
						case 1:
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								draw3_pop_tt (data_022, NULL,
										menu_content[CTYPE + pp->ctype_pos],
										menu_content+CTYPE, 3, 2, pp->ctype_pos, 0x0);
							else
								draw3_popdown (menu_content[CTYPE + pp->ctype_pos], 2, 0);
							break;
						case 2:
							if (pp->ctype_pos == 0)
							{
								switch (TMP(origin_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
									lower = -1000;
									upper = 1000.00;
									step = tmpf;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
									draw3_digit_pressed (data_704, units[unit], cur_value , lower, upper, step, digit, p, pos, 41);
								}
								else 
								{
									cur_value = get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 41);
								}
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								switch (TMP(cangle_reg))
								{
									case 0:	tmpf = 1.0; break;
									case 1:	tmpf = 10.0; break;
									default:break;
								}
								if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = pp->cangle ;
									lower =	0.0;
									upper =	89.9;
									step = tmpf;
									digit = 0;
									pos = 2;
									unit = UNIT_DEG;
									draw3_digit_pressed (data_0226, units[unit], cur_value,
											lower, upper, step, digit, p, pos, 8);
								}
								else
								{
									cur_value = pp->cangle ;
									unit = UNIT_DEG;
									pos = 2;
									digit = 0;
									draw3_digit_stop (cur_value , units[unit], digit, pos, 8);
								}
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
									draw3_pop_tt (data_0221, NULL,
											menu_content[ECHOTYPE + pp->echotype_pos],
											menu_content+ECHOTYPE, 3, 2, pp->echotype_pos, 0x0);
								else
									draw3_popdown (menu_content[ECHOTYPE + pp->echotype_pos], 2, 0);
								str = g_strdup_printf ("%s", con2_p[0][2][12]);
								gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
								g_free (str);
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
							{
								switch (TMP(ref_amplitude_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = pp->ref_amplitude/100.0 ;
									lower =	0.0;
									upper =	100.0;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_BFH;
									draw3_digit_pressed (data_0223, units[unit], cur_value,
											lower, upper, step, digit, p, pos, 33);
								}
								else
								{
									cur_value = pp->ref_amplitude/100.0 ;
									unit = UNIT_BFH;
									pos = 2;
									digit = 1;
									draw3_digit_stop (cur_value , units[unit], digit, pos, 33);
								}
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 3))
							{
								switch (pp->p_tmp_config->ref_ampl_reg)
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value =GROUP_VAL(ref_ampl)/10.0;
									lower = 0.0;
									upper = 100.0;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_BFH;
									draw3_digit_pressed (data_232, units[unit], cur_value , lower, upper, step, digit, p, pos, 36);
								}
								else
								{
									cur_value = GROUP_VAL(ref_ampl)/10.0;
									digit = 1;
									pos = 2;
									unit = UNIT_BFH;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 36);
								}
							}
							else if ((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
									draw3_pop_tt (data_0225, NULL,
											menu_content[WELDTYPE + pp->weldtype_pos],
											menu_content+WELDTYPE, 2, 2, pp->weldtype_pos, 0x0);
								else
									draw3_popdown (menu_content[WELDTYPE + pp->weldtype_pos], 2, 0);

								str = g_strdup_printf ("%s", con2_p[0][2][38]);
								gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							}
						break;
						case 3:
							if (pp->ctype_pos == 0)
							{
								switch (TMP(distance_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = pp->distance /1000.0;
									lower = -99999.00;
									upper = 99999.00;
									step = tmpf;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
									draw3_digit_pressed (data_0227, units[unit], cur_value , lower, upper, step, digit, p, pos, 42);
								}
								else 
								{
									cur_value = pp->distance /1000.0;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 42);
								}
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
									draw3_pop_tt (data_0221, NULL,
											menu_content[ECHOTYPE + pp->echotype_pos],
											menu_content+ECHOTYPE, 3, 2, pp->echotype_pos, 0x0);
								else
									draw3_popdown (menu_content[ECHOTYPE + pp->echotype_pos], 2, 0);
								str = g_strdup_printf ("%s", con2_p[0][2][12]);
								gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							}


							else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
							{

								cur_value = LAW_VAL (Angle_min) / 100.0;
								digit = 1;
								pos = 2;
								unit = UNIT_DEG;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 27);
								gtk_widget_set_sensitive(p->eventbox30[2],FALSE);
								gtk_widget_set_sensitive(p->eventbox31[2],FALSE);
#if 0
								switch (TMP(first_angle_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = pp->first_angle/100.0 ;
									lower =	0.0;
									upper =	89.9;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_DEG;
									draw3_digit_pressed (data_0222, units[unit], cur_value,
											lower, upper, step, digit, p, pos, 27);
								}
								else
								{
									cur_value = pp->first_angle/100.0 ;
									unit = UNIT_DEG;
									pos = 2;
									digit = 1;
									draw3_digit_stop (cur_value , units[unit], digit, pos, 27);
								}
								gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
#endif
							}
							else if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
							{
								switch (TMP(cstart_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的起位数值mm */
											lower = (BEAM_INFO(0,beam_delay) /1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											digit = 2;
											pos = 2;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											digit = 3;
											pos = 2;
											unit = UNIT_INCH;
										}
									}
									else
									{
											cur_value = GROUP_VAL(gate[0].start) / 1000.0 ;   /* us */
											lower = BEAM_INFO(0,beam_delay) /1000.0;
											upper =	MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0;
											step = tmpf;
											digit = 2;
											pos = 2;
											unit = UNIT_US;
									}
									draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
								}
								else
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 2;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 2;
										}
									}
									else
									{
											cur_value = GROUP_VAL(gate[0].start) / 1000.0 ;   /* us */
											digit = 2;
											pos = 2;
											unit = UNIT_US;
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 10);
								}
							}
							break;
						case 4:
							if (pp->ctype_pos == 0)
							{
								cur_value = get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
								digit = 3;
								pos = 2;
								unit = UNIT_STEP_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 43);

								gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
							}

							else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 0)||(pp->cmode_pos == 1)||(pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
							{
								switch (TMP(cstart_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的起位数值mm */
											lower = (BEAM_INFO(0,beam_delay) /1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											digit = 2;
											pos = 2;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											digit = 3;
											pos = 2;
											unit = UNIT_INCH;
										}
									}
									else
									{
											cur_value = GROUP_VAL(gate[0].start) / 1000.0 ;   /* us */
											lower = BEAM_INFO(0,beam_delay) /1000.0;
											upper =	MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0;
											step = tmpf;
											digit = 2;
											pos = 2;
											unit = UNIT_US;
									}
									draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
								}
								else
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 2;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 2;
										}
									}
									else
									{
											cur_value = GROUP_VAL(gate[0].start) / 1000.0 ;   /* us */
											digit = 2;
											pos = 2;
											unit = UNIT_US;
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 10);
								}
							}
							else if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
							{
								switch (TMP(db_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 0.5; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 2.0; break;
									case 4:	tmpf = 6.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
									lower = 0.0 - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
									upper = GAIN_MAX - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_pressed (data_100, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 9);
								}
								else
								{
									cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
								}
							}
							break;
						case 5:
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{

								switch (TMP(cstart_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的起位数值mm */
											lower = (BEAM_INFO(0,beam_delay) /1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											digit = 2;
											pos = 2;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											digit = 3;
											pos = 2;
											unit = UNIT_INCH;
										}
									}
									else
									{
											cur_value = GROUP_VAL(gate[0].start) / 1000.0 ;   /* us */
											lower = BEAM_INFO(0,beam_delay) /1000.0;
											upper =	MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0;
											step = tmpf;
											digit = 2;
											pos = 2;
											unit = UNIT_US;
									}
									draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
								}
								else
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 2;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 2;
										}
									}
									else
									{
											cur_value = GROUP_VAL(gate[0].start) / 1000.0 ;   /* us */
											digit = 2;
											pos = 2;
											unit = UNIT_US;
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 10);
								}


							}


							else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 3)) )
							{
								switch (TMP(db_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 0.5; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 2.0; break;
									case 4:	tmpf = 6.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
									lower = 0.0 - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
									upper = GAIN_MAX - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_pressed (data_100, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 9);
								}
								else
								{
									cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
								}
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
							{
								switch (TMP(compdb_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value = pp->comp_gain / 100.0;
									lower = 0.0;
									upper = 10.0;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_DB_US;
									draw3_digit_pressed (data_0224, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 34);
								}
								else
								{
									cur_value = pp->comp_gain / 100.0;
									digit = 1;
									pos = 2;
									unit = UNIT_DB_US;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 34);
								}

							}
							else
							{
								gtk_widget_hide (pp->eventbox30[2]);
								gtk_widget_hide (pp->eventbox31[2]);
							}
							break;
						case 6:
							if (pp->ctype_pos == 1)
							{
								if(pp->cmode_pos == 0)
								{
									if (get_unit(pp->p_config) == 0)
									{
										cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 100.0;
										digit = 1;
										unit = UNIT_M_S;
									}
									else
									{
										cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) * 0.000000394;
										digit = 4;
										unit = UNIT_IN_US;
									}
									pos = 2;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 44);
									gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
									gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);

								}

								else if(pp->cmode_pos == 2)
								{
									switch (TMP(db_reg))
									{
										case 0:	tmpf = 0.1; break;
										case 1:	tmpf = 0.5; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 2.0; break;
										case 4:	tmpf = 6.0; break;
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
									{
										cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
										lower = 0.0 - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
										upper = GAIN_MAX - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
										step = tmpf;
										digit = 1;
										pos = 2;
										unit = UNIT_DB;
										draw3_digit_pressed (data_100, units[unit], cur_value ,
												lower, upper, step, digit, p, pos, 9);
									}
									else
									{
										cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
										digit = 1;
										pos = 2;
										unit = UNIT_DB;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
									}
								}
							}
							break;
					}
					break;

				case 3:/*p032*/
					if ( !con2_p[0][3][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
				case 4:
					if ( !con2_p[0][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* range范围 P102 TAN1 */
					switch (TMP(range_reg))
					{
						case 0:	tmpf = GROUP_VAL(point_qty) / 100.0; break;
						case 1:	tmpf = GROUP_VAL(point_qty) / 20.0; break;
						case 2:	tmpf = GROUP_VAL(point_qty) / 10.0; break;
						default:break;
					}
					tmpfm = GROUP_VAL(point_qty) / 100.0;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						max_tmp = (MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0);
						max_tmp1 = GROUP_VAL(point_qty) * 20.0;
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
								lower = (GROUP_VAL(point_qty) / 100.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								upper = MIN(max_tmp, max_tmp1) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								tmpfm = tmpfm * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
								lower =	(GROUP_VAL(point_qty) / 100.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								upper = MIN(max_tmp, max_tmp1) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								tmpfm = tmpfm * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
							lower =	GROUP_VAL(point_qty) / 100.0;
							upper = MIN(max_tmp, max_tmp1);										
							step = tmpf;
							tmpfm = tmpfm;
							digit = 2;
							pos = 2;
							unit = UNIT_US;
						}
						draw3_digit_pressed (data_102, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 2;
							}
							else
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
								unit = UNIT_INCH;
								digit = 3;
								pos = 2;
							}
						}
						else
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
							unit = UNIT_US;
							pos = 2;
							digit = 2;
						}
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
/*					TMP(range_step_min) = ((gint)(tmpfm * 1000)+ 5) / 10 * 10;*/
					break;
				case 1: /* Freq频带(Mhz)  P112 TAN1 */
					pp->x_pos = 587, pp->y_pos = 288-YOFFSET;	
					switch (TMP(frequency_reg))
					{
						case 0:	tmpf = 0.01;	break;
						case 1:	tmpf = 0.1;		break;
						case 2:	tmpf = 1.0;		break;
						default:break;
					}
					if (GROUP_VAL(probe.Model[0]) != 32) 	/* 选择 unknown 时候可以调节频率 */
					{
						gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
					}
					/*if (GRPUP_VAL(probe.Name[0]) == 0)*/ /*选择探头的时候需要同时修改GROUP_VAL(frequecne) */
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2) &&
							(GROUP_VAL(probe.Model[0]) == 32))
					{
						if (pp->mark_pop_change)
						{
							cur_value = get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0;
							lower =	1.0;
							upper =	20.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_1121, units[unit], cur_value,
									lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							str = g_strdup_printf ("%0.2f", get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0);
							draw3_pop_tt (data_112, NULL, str,
									menu_content + FREQUENCY, 13, 2, get_group_val (p_grp, GROUP_FREQ_POS), 0);

							g_free (str);
						}
					}
					else 
					{
						cur_value = get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0;
						unit = UNIT_NULL;
						pos = 2;
						digit = 2;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 2:/* Rectifier 检波 P122 */
					pp->x_pos = 611, pp->y_pos = 288 - YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_122, NULL, 
								menu_content[RECTIFIER + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RECTIFIER)],
								menu_content + RECTIFIER, 4, 2, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RECTIFIER), 
								((get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER)  == NORMAL_ON) ? 0x01 : 0x0));
					else 
						draw3_popdown (menu_content[RECTIFIER + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RECTIFIER)], 2, 0);
					break;
				case 3:/* Angle (deg.) PA 当前Beam的角度 UT 当前探头的角度  P132 TAN1 */
					if (GROUP_VAL(group_mode) == PA_SCAN)
					{
						gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
						/* 计算当前beam的角度 */
						switch (LAW_VAL(Focal_type))
						{
							case AZIMUTHAL_SCAN:
								cur_value = 0.0;break;
							case LINEAR_SCAN:
								cur_value = 0.0;break;
							default:
								cur_value = 0.0;break;
						}
						unit = UNIT_NULL;
						pos = 2;
						digit = 1;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					else if (GROUP_VAL(group_mode) == UT_SCAN)
					{
						switch (TMP(angle_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;						
							default:break;
						}
						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = LAW_VAL(Angle_min)/100.0 ;
							lower =	0.0;
							upper =	89.9;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_132, units[unit], cur_value,
									lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL(Angle_min)/100.0 ;
							unit = UNIT_NULL;
							pos = 2;
							digit = 1;
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
					}
					break;
				case 4:/* dB Ref 开关 P142 */
					draw3_popdown (menu_content[OFF_ON + get_group_db_ref (pp->p_config, get_current_group (pp->p_config))], 2, 0);
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/* 闸门起点 或者 同步模式 P202 TAN1 */
					pp->x_pos = 608, pp->y_pos = 295 - YOFFSET;
					if (GROUP_GATE_POS(parameters) == 0) /* 闸门起点 */
					{

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								switch (TMP(agate_start_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;
									default:break;
								}
								if (UNIT_MM == get_unit(pp->p_config)) 
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的起位数值mm */
									lower = (BEAM_INFO(0,beam_delay) /1000.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
									upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
									//step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
									step = tmpf;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else 
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
									lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
									upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
									//step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
									step = tmpf;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
							}
							else /*ut_unit 选择 time 时 */
							{
								switch (TMP(agate_start_reg))
								{
									case 0:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / (gfloat)(GROUP_VAL(point_qty)); break;
									case 1:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 20.0 ; break;
									case 2:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 10.0 ; break;
									case 3:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 4.0 ; break;
									default:break;
								}
								cur_value = GROUP_GATE_POS(start) / 1000.0 ;
								lower =	BEAM_INFO(0,beam_delay) / 1000.0;
								upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0);
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_US;
							}
							draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if (UNIT_MM == get_unit(pp->p_config))
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
									unit = UNIT_MM;
									digit = 2;
									pos = 2;
								}
								else
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
									unit = UNIT_INCH;
									digit = 3;
									pos = 2;
								}
							}
							else
							{
								cur_value = GROUP_GATE_POS(start)/1000.0;
								unit = UNIT_US;
								digit = 2;
								pos = 2;
							}
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
					}
					else  /* 同步模式 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if (GROUP_VAL(gate_pos) == GATE_B) /* 闸门选择B时候 有3个同步选项 */
								draw3_pop_tt (data_2021, NULL,
										menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
										menu_content + SYNCHRO, 3, 2, GROUP_GATE_POS(synchro), 0);
							else if (GROUP_VAL(gate_pos) == GATE_A) /* 闸门选择A时候 有2个同步选项 */
								draw3_pop_tt (data_2021, NULL, 
										menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
										menu_content + SYNCHRO, 2, 2, GROUP_GATE_POS(synchro), 0);
							else if (GROUP_VAL(gate_pos) == GATE_I) /* 闸门选择I时候 有1个同步选项 */
								draw3_pop_tt (data_2021, NULL, 
										menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
										menu_content + SYNCHRO, 1, 2, GROUP_GATE_POS(synchro), 0);
						}
						else 
							draw3_popdown (menu_content[SYNCHRO +GROUP_GATE_POS(synchro)], 2, 0);
						str = g_strdup_printf ("%s", con2_p[2][0][6]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						if (str)
							g_free (str);
					}
					//gtk_widget_queue_draw(pp->draw_area->drawing_area);
					gtk_widget_queue_draw (pp->vboxtable);
					break;
				case 1:/* Condition GroupA P212 TAN1 */
					pp->x_pos = 535, pp->y_pos = 285-YOFFSET;
					if ((get_alarm_groupa(pp->p_config) == get_alarm_groupb(pp->p_config)) &&
							get_alarm_conditionb(pp->p_config))
						menu_status = 1<<(get_alarm_conditionb(pp->p_config));
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_212, NULL, menu_content[CONDITIONA +	get_alarm_conditiona(pp->p_config)],
								menu_content + CONDITIONA, 9, 2, get_alarm_conditiona(pp->p_config), menu_status);
					else 
						draw3_popdown (menu_content[CONDITIONA + get_alarm_conditiona(pp->p_config)], 2, 0);
					break;
				case 2:/* Output->Count or Data P222 */
					pp->x_pos = 578, pp->y_pos = 300;
					/* 当前步进 */
					if (get_output_pos(pp->p_config) < 3)
					{
						switch (TMP(count_reg))
						{
							case 0:	tmpf = 1.00; break;
							case 1:	tmpf = 10.00; break;						
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = get_output_count (pp->p_config);
							lower = 1.0;
							upper = 100.0;
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_222, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value =	get_output_count (pp->p_config);
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							draw3_pop_tt (data_2221, NULL, 
									menu_content[DATA + get_output_data(pp->p_config)],
									menu_content + DATA, 4, 2, get_output_data(pp->p_config), 0);
						}
						else 
						{
							draw3_popdown (menu_content[DATA + get_output_data(pp->p_config)], 2, 0);
						}
						str = g_strdup_printf ("%s", con2_p[2][2][7]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
					}
					break;

				case 3:/* Gate/Alarm -> Sizing Curves-> ref.amplitude  p232 */
					if (GROUP_VAL(mode_pos) == 0)  /* MODE 为 Setup 时候 */
					{
						if ((GROUP_VAL(curve_pos) == 1) || (GROUP_VAL(curve_pos) == 2)) /* DAC or linear Dac*/
						{
							/* 当前步进 */
							switch (pp->p_tmp_config->ref_ampl_reg)
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 1.0; break;
								case 2:	tmpf = 10.0; break;
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								cur_value =GROUP_VAL(ref_ampl)/10.0;
								lower = 0.0;
								upper = 100.0;
								step = tmpf;
								digit = 1;
								pos = 2;
								unit = UNIT_BFH;
								draw3_digit_pressed (data_232, units[unit], cur_value , lower, upper, step, digit, p, pos, 6);
							}
							else 
							{
								cur_value = GROUP_VAL(ref_ampl)/10.0;
								digit = 1;
								pos = 2;
								unit = UNIT_BFH;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
							} 
						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[2]);
							gtk_widget_hide (pp->eventbox31[2]);
							gtk_widget_hide (pp->sbutton[2]);
						}
					}
					else if (GROUP_VAL(mode_pos) == 1)  /* MODE 为Edit 时候 */
					{
						if( GROUP_VAL(curve_pos) == 1|| GROUP_VAL(curve_pos)==3) 
							/* Curve选项为 DAC或TCG 时 */
						{
							/* 当前步进 */
							switch (pp -> p_tmp_config -> position_reg)
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;					
								default:break;
							}

							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value = GROUP_VAL(position[GROUP_VAL(point_pos)]) / 1000.0;
										lower = 0.0;
										upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/100.0 * 4.95;
										step = tmpf;
										digit = 2;
										pos = 2;
										unit = UNIT_MM;
									}
									else
									{
										cur_value = GROUP_VAL(position[GROUP_VAL(point_pos)]) / 1000.0 * 0.03937;
										lower = 0.0;
										upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/100.0 * 4.95 * 0.03937;
										step = tmpf/2.0;
										digit = 3;
										pos = 2;
										unit = UNIT_INCH;
									}
								}
								else
								{
									cur_value = GROUP_VAL(position[GROUP_VAL(point_pos)]) / 1000.0 * 0.03937/0.1159;
									lower = 0.0;
									upper = 9900.80;
									step = tmpf*5.0;
									digit = 2;
									pos = 2;
									unit = UNIT_US;
								}
								draw3_digit_pressed (data_2321, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);

							}
							else 
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value = GROUP_VAL(position[GROUP_VAL(point_pos)])/1000.0;
										digit = 2;
										pos = 2;
										unit = UNIT_MM;
									}
									else
									{
										cur_value = GROUP_VAL(position[GROUP_VAL(point_pos)])/1000.0 * 0.03937;
										digit = 3;
										pos = 2;
										unit = UNIT_INCH;
									}
								}
								else
								{
									cur_value = GROUP_VAL(position[GROUP_VAL(point_pos)]) / 1000.0 * 0.03937/0.1159;
									digit = 2;
									pos = 2;
									unit = UNIT_US;
								}
								draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
							}
						}
						else if (GROUP_VAL(curve_pos == 2)) /* Curve 为 linear DAC */
						{
							/* 当前步进 */
							switch (pp->p_tmp_config->delay_reg)
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3: tmpf = 10.0; break;
								default:break;
							}

							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value =GROUP_VAL(delay)/1000.0;
										lower = 0.0;
										upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 2000.0;
										step = tmpf;
										digit = 2;
										pos = 2;
										unit = UNIT_MM;
									}
									else
									{
										cur_value =GROUP_VAL(delay)/1000.0*0.03937;
										lower = 0.0;
										upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 2000.0*0.03937;
										step = tmpf/10.0;
										digit = 3;
										pos = 2;
										unit = UNIT_INCH;
									}
								}
								else
								{
									cur_value =GROUP_VAL(delay)/1000.0/2.945;
									lower = 0.0;
									upper = 100.0;
									step = tmpf/10.0;
									digit = 2;
									pos = 2;
									unit = UNIT_US;

								}

								draw3_digit_pressed (data_2322, units[unit], cur_value , lower,
										upper, step, digit, p, pos, 16);

							}
							else 
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value =GROUP_VAL(delay)/1000.0;
										digit = 2;
										pos = 2;
										unit = UNIT_MM;
									}
									else
									{
										cur_value =GROUP_VAL(delay)/1000.0*0.03937;
										digit = 3;
										pos = 2;
										unit = UNIT_INCH;
									}
								}
								else
								{
									cur_value =GROUP_VAL(delay)/1000.0/2.945;
									digit = 2;
									pos = 2;
									unit = UNIT_US;

								}
								draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
							}

						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[2]);
							gtk_widget_hide (pp->eventbox31[2]);
							gtk_widget_hide (pp->sbutton[2]);
						}
					}
					break;

				case 4:
					if ( !con2_p[2][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
				default:break;
			}
			break;

		case 3:
			switch (pp->pos1[3])
			{
				case 0:/* Measurements -> Reading -> Field 1 P302 */
					pp->x_pos = 200, pp->y_pos = 0;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_302, NULL, 
								field[get_reading_field1(pp->p_config)],
								field1, 60, 2, get_reading_field1(pp->p_config), 0);
					else 
						draw3_popdown (field[get_reading_field1(pp->p_config)], 2, 0);
//					g_printf("\n--------->field_index=%d\n",DO_NOT_USE_CCFG(field[0]));
					break;
				case 1:/*Measurements -> Cursors -> Scan p312 */
					if(!GROUP_VAL(selection))
					{
						/* 当前步进 */
						switch (TMP(per_measure_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 1.0;  break;
							case 2:	tmpf = 10.0;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(per_measure)/100.0;
							lower = 0.0;
							upper = 100.0;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_312, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							cur_value = GROUP_VAL(per_measure)/100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else if (GROUP_VAL(selection)==1 || GROUP_VAL(selection)==2|| GROUP_VAL(selection)==3)
					{
						/* 当前步进 */
						switch (pp->p_tmp_config->s_measure_reg)
						{
							case 0:	tmpf = 0.5; break;
							case 1:	tmpf = 2.5; break;
							case 2:	tmpf = 5.0; break;
							case 3:	tmpf = 7.5; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(s_measure)/100.0;
								lower = -1000000.0;
								upper =  1000000.0;
								step = tmpf;
								digit = 1;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(s_measure)/100.0*0.03937;
								lower = -1000000.0*0.03937;
								upper =  1000000.0*0.03937;
								step = tmpf/500.0;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3121, units[unit], cur_value , lower, upper, step, digit, p, pos, 7);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(s_measure)/100.0;
								digit = 1;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(s_measure)/100.0*0.03937;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
						}

						if(GROUP_VAL(selection)==1)
						{
							gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
						}

					}
					else if (GROUP_VAL(selection)==4)
					{
						/* 当前步进 */
						switch (TMP(u_reference_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 10.0;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
 									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)
										+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									step = tmpf;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference) / 1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									step = tmpf/2.0;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
							}
							else	/* us */
							{
								cur_value = GROUP_VAL(u_reference)/1000.0;
								lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0;
								upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_US;
							}
							draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, p, pos, 3);
						}
						else
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
							}
							else
							{
								cur_value = GROUP_VAL(u_reference)/1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_US;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 3);
						}

					}
					else if (GROUP_VAL(selection)==5||GROUP_VAL(selection)==6||GROUP_VAL(selection)==7)
					{
						/* 当前步进 */
						switch (TMP(cursors_amplitude_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_CURSORS_POS(amplitude)/100.0;
							lower = 0.0;
							upper = 100.0;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_3122, units[unit], cur_value , lower, upper, step, digit, p, pos, 13);
						}
						else
						{
							cur_value = GROUP_CURSORS_POS(amplitude)/100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 13);
						}
					}
					else  if (GROUP_VAL(selection)==8)
					{
						/* 当前步进 */
						switch (TMP(cursors_scan_reg))
						{
							case 0:	tmpf = 0.5; break;
							case 1:	tmpf = 2.5; break;
							case 2:	tmpf = 5.0; break;
							case 3:	tmpf = 7.5; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0;
								lower = -100000.0;
								upper = 100000;
								step = tmpf;
								digit = 1;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0*0.03937;
								lower = -100000.0*0.03937;
								upper = 100000*0.03937;
								step = tmpf;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3142, units[unit], cur_value , lower, upper, step, digit, p, pos, 15);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0;
								digit = 1;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0*0.03937;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
						}
					}
					gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;


				case 2:/*Measurements -> Table -> Add Entry p322 */
					draw3_popdown(NULL,2,1);
					break;
				case 3:/*Measurements -> Thickness -> Max p332 */
					/* 当前步进 */
					switch (TMP(max_thickness_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = get_max_thickness(pp->p_config)/1000.0;
							lower = (get_min_thickness(pp->p_config)/1000.0) + 0.01;
							upper = 99999.00;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_max_thickness(pp->p_config)/1000.0 * 0.03937;
							lower = (get_min_thickness(pp->p_config)/1000.0 * 0.03937) + 0.005;
							upper = 99999.00 * 0.03937;
							step = tmpf/2.0;
							digit = 3;
							pos = 2;
							unit = UNIT_INCH;
						}
						draw3_digit_pressed (data_332, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = get_max_thickness(pp->p_config)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_max_thickness(pp->p_config)/1000.0 * 0.03937;
							digit = 3;
							pos = 2;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 4:
					if ( !con2_p[3][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/* C-Scan2  P402 */
					pp->x_pos = 580, pp->y_pos = 330;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))/*选中402这个位置*/
					{
						if(get_display_pos(pp->p_config)==7)
							/*Display 为 A-C-[C]*/
						{
							draw3_pop_tt (data_402, NULL, 
									menu_content[C_SCAN1 + get_cscan_source(pp->p_config, 1)],
									menu_content + C_SCAN1, 5, 2, get_cscan_source(pp->p_config, 1), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							gtk_widget_queue_draw(pp->vboxtable);

						}
						else if(get_display_pos(pp->p_config)==8)
							/*Display 为 A-S-[C]*/
						{
							if (get_group_qty(pp->p_config) == 1)
								menu_status = 0x01;
							draw3_pop_tt (data_401, NULL, 
									menu_content[GROUP_POS + get_display_group(pp->p_config)],
									menu_content + GROUP_POS, 2, 2, get_display_group(pp->p_config), menu_status);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

						}
						else if(get_display_pos(pp->p_config)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_pop_tt (data_4021, NULL, 
									menu_content[DATA2+get_stripscan_data2(pp->p_config)],
									menu_content + DATA2, 4, 2, get_stripscan_data2(pp->p_config), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][9]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[2]);
							gtk_widget_hide (pp->eventbox31[2]);
						}

					}
					else 
					{
						if(get_display_pos(pp->p_config)==7)/* Display 为 A-C-[C] */
						{
							draw3_popdown (menu_content[C_SCAN1+get_cscan_source(pp->p_config, 1)], 2, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}

						else if(get_display_pos(pp->p_config)==8)/* Display 为 A-S-[C] */
						{
							draw3_popdown (menu_content[GROUP_POS+get_display_group(pp->p_config)], 2, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}

						else if(get_display_pos(pp->p_config)==10)/* Display 为 Strip Chart-[A] */
						{
							draw3_popdown (menu_content[DATA2+get_stripscan_data2(pp->p_config)], 2, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][9]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[2]);
							gtk_widget_hide (pp->eventbox31[2]);
						}
					}
					break;

				case 1:/*Display -> Overlay -> sizing curves  p412 */
					draw3_popdown(menu_content[OFF_ON + get_overlay_sizing_curves(pp->p_config)],2,0);
					break;
				case 2:/*Display -> Color -> end p422 */
					if(GROUP_VAL(col_select_pos) == 0)
					{
						/* 当前步进 */
						switch (TMP(color_end_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(col_end);
							lower = GROUP_VAL(col_start)+1;
							upper = 100.0;
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_432, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(col_end);
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else if(GROUP_VAL(col_select_pos) == 1)
					{
						/* 当前步进 */
						switch (TMP(brightness_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(col_brightness);
							lower = (int)((100.0-GROUP_VAL(col_contrast))/2.0);
							upper = 100.0-lower;
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_4321, units[unit], cur_value , lower, upper, step, digit, p, pos, 7);
							//str = g_strdup_printf ("%s\n    (%%) Δ%0.0f", con2_p[4][3][7],tmpf);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}
						else 
						{
							cur_value = GROUP_VAL(col_brightness);
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos,7);
							//str = g_strdup_printf ("%s\n    (%%)", con2_p[4][3][7]);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

						}
					}
					else if (GROUP_VAL(col_select_pos) == 2)
					{
						/* 当前步进 */
						switch (TMP(max_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf =  1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(col_max)/1000.0;
							lower = GROUP_VAL(col_min)/1000.0 + 0.01;
							upper = 99999.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_4322, units[unit], cur_value , lower, upper, step, digit, p, pos, 9);
							//str = g_strdup_printf ("   %s\n(mm) Δ%0.2f", con2_p[4][3][9],tmpf);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}
						else 
						{
							cur_value = GROUP_VAL(col_max)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
							//str = g_strdup_printf ("%s\n(mm)", con2_p[4][3][9]);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}
					}
					break;

				case 3:/* Display->Properties-> Ascan Envelope  Bscan optimum p432 */

					switch(get_dis_prop_scan(pp->p_config))
					{
						case 0: /* Ascan 时候的包络 */
							pp->x_pos = 596, pp->y_pos = 320-YOFFSET;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								draw3_pop_tt (data_442, NULL, 
										menu_content[ENVELOPE + GROUP_VAL(ascan_envelope)],
										menu_content+ENVELOPE, 2, 2, GROUP_VAL(ascan_envelope), 0);
							else 
								draw3_popdown (menu_content[ENVELOPE + GROUP_VAL(ascan_envelope)], 2, 0);
							break;
						case 1: /* Bscan 时候是optimum */
							draw3_popdown (menu_content[OFF_ON + get_dis_prop_boptimum (pp->p_config)], 2, 0);
							g_sprintf (temp,"%s", con2_p[4][3][7]);
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
							break;
						case 2:
							gtk_widget_hide(pp->eventbox30[2]);
							gtk_widget_hide(pp->eventbox31[2]);
							break;
						case 3:
							gtk_widget_hide(pp->eventbox30[2]);
							gtk_widget_hide(pp->eventbox31[2]);
							break;
						case 4:
							gtk_widget_hide(pp->eventbox30[2]);
							gtk_widget_hide(pp->eventbox31[2]);
							break;
						case 5:
							gtk_widget_hide(pp->eventbox30[2]);
							gtk_widget_hide(pp->eventbox31[2]);
							break;

						default:break;
					}
					break;
				case 4:
					if ( !con2_p[4][4][2] )
						gtk_widget_hide (p->eventbox30[2]);
					gtk_widget_hide (p->eventbox31[2]);
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Select  p502 */
					pp->x_pos = 562, pp->y_pos = 288-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_502, NULL, 
								menu_content[PROB_SELECT + get_probe_select (pp->p_config)],
								menu_content + PROB_SELECT, 2, 2, get_probe_select (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[PROB_SELECT + get_probe_select (pp->p_config)], 2, 0);
					break;

				case 1:/* skew angle P512 */
					pp->x_pos = 588, pp->y_pos = 285-YOFFSET;
					switch (TMP(skew_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;						
						default:break;
					}
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if (pp->mark_pop_change)
						{
							cur_value = GROUP_VAL(skew)/100.0 ;
							lower =	0.0;
							upper =	359.9;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_NULL;
							draw3_digit_pressed (data_5121, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							str = g_strdup_printf ("%.1f", GROUP_VAL(skew)/100.0);
							draw3_pop_tt (data_512, NULL, 
									str, menu_content+PROB_SKEW, 5, 2, GROUP_VAL(skew_pos), 0);
							g_free(str);
						}
					}
					else 
					{
						cur_value = GROUP_VAL(skew)/100.0 ;
						unit = UNIT_NULL;
						pos = 2;
						digit = 1;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 2:/*Probe/Part -> Characterize -> start p522 */
# if 0
					/* 当前步进 */
					switch (TMP(agate_start_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = GROUP_VAL(agate_start)/1000.0;
						lower = 0.00;
						upper = 29127.88;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_pressed (data_522, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = GROUP_VAL(agate_start)/1000.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}

#endif

					switch (TMP(agate_start_reg))
					{
						case 0:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 320.0; break;
						case 1:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 20.0 ; break;
						case 2:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 10.0 ; break;
						case 3:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 1.0  ; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config)) 
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的起位数值mm */
								lower = (BEAM_INFO(0,beam_delay) /1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
							}
							else 
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
								lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = GROUP_VAL(gate[0].start) / 1000.0 ;
							lower =	BEAM_INFO(0,beam_delay) / 1000.0;
							upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0);
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_US;
						}
						draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 2;
							}
							else
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
								unit = UNIT_INCH;
								digit = 3;
								pos = 2;
							}
						}
						else
						{
							cur_value = GROUP_VAL(gate[0].start)/1000.0;
							unit = UNIT_US;
							digit = 2;
							pos = 2;
						}
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}

					break;

				case 3:/*Probe/Part -> Parts -> diameter p532 */
					switch(get_part_geometry(pp->p_config))
					{
						case 0:
							if(UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = get_part_diameter(pp->p_config) / 1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_part_diameter (pp->p_config) / 1000.0 * 0.03937;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
							break;
						case 1:
							/* 当前步进 */
							switch (pp->p_tmp_config->diameter_reg)
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								if(UNIT_MM == get_unit(pp->p_config))
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0;
									lower = 2.0 * get_part_thickness(pp->p_config) / 1000.0;
									upper = 1000000.00;
									step = tmpf;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0 * 0.03937;
									lower = 2.0 * get_part_thickness(pp->p_config) / 1000.0*0.03937;
									upper = 1000000.00 * 0.03937;
									step = tmpf;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
								draw3_digit_pressed (data_532, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							}
							else 
							{
								if(UNIT_MM == get_unit(pp->p_config))
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0 * 0.03937;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
							break;
						case 2:
							/* 当前步进 */
							switch (pp->p_tmp_config->diameter_reg)
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								if(UNIT_MM == get_unit (pp->p_config))
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0;
									lower = 2.0 * get_part_thickness(pp->p_config) / 1000.0 + 10.0;
									upper = 1000000.00;
									step = tmpf;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0 * 0.03937;
									lower =(2.0 * get_part_thickness(pp->p_config) / 1000.0 + 10.0)*0.03937;
									upper = 1000000.00 * 0.03937;
									step = tmpf;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
								draw3_digit_pressed (data_532, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							}
							else 
							{
								if(UNIT_MM == get_unit (pp->p_config))
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = get_part_diameter(pp->p_config) / 1000.0 * 0.03937;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
							break;
						default:break;
					}
					break; 
				case 4:
					if ( !con2_p[5][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0: /* 第一个接收阵元在连接器上面的位置 与UT设置的receiver一样 P602  TAN1 */
					switch (TMP(receiver_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if (get_group_val (p_grp, GROUP_TX_RX_MODE) == PULSE_ECHO)	/* 脉冲回波模式不可以调节 */
					{
						gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
					}
					upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 2;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
						digit = 0;
						pos = 2;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_stop (cur_value, str, digit, pos, 0);
					}
					g_free(str);
					break;
				case 1:/* 角度扫查的步进 P612 */
					switch (pp->p_tmp_config->angle_step_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((LAW_VAL(Focal_type) == AZIMUTHAL_SCAN) &&
							(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON))
						/* 角度扫查时开始自动计算聚焦法则时候可以调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = LAW_VAL (Angle_step) / 100.0;
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
							TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
									LAW_VAL(Angle_beam_skew_min)) /
								LAW_VAL(Angle_beam_skew_step) + 1;
							temp_beam = temp_beam / TMP(beam_skew_num);
							lower = MAX (((gint)(LAW_VAL (Angle_max)) - 
										(gint)(LAW_VAL (Angle_min))) 
									/ (100.0 * temp_beam), 0.1);
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_612, units[unit], cur_value,
									lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL (Angle_step) / 100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						cur_value = LAW_VAL (Angle_step) / 100.0;
						digit = 1;
						pos = 2;
						unit = UNIT_DEG;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}
					break;
				case 2:/* 角度扫查的步进 P622 */
					if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
						/* 聚焦法则自动计算开启时, Max Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							//if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
							if(LAW_VAL(Focal_type) == 2)
								/*law type 为 depth sector 时*/
							{
								switch (TMP(positione_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								cur_value = LAW_VAL(Position_end) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;

								draw3_digit_pressed (data_622, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 0);
							}
							else if(LAW_VAL(Focal_point_type)==FOCALPLANE_P)
								/*type 为 focal plane 时*/
							{
								switch (TMP(offsete_reg))
								{
									case 0:	tmpf = 0.5; break;
									case 1:	tmpf = 5.0; break;
									case 2:	tmpf = 50.0; break;
									default:break;
								}
								cur_value = LAW_VAL(Offset_end) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;

								draw3_digit_pressed (data_6221, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 7);
							}
							else
							{
								cur_value = LAW_VAL(Position_end) / 1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

								gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
							}

						}
						else 
						{
							if(LAW_VAL(Focal_type) == 2)
								/*law type 为 depth sector 时*/
							{
								cur_value = LAW_VAL(Position_end) / 1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
							else if(LAW_VAL(Focal_point_type)==FOCALPLANE_P)
								/*type 为 focal plane 时*/
							{
								cur_value = LAW_VAL(Offset_end) / 1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
							}
							else
							{
								cur_value = LAW_VAL(Position_end) / 1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

								gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
							}
						}
					}
					else
					{
						cur_value = LAW_VAL(Position_end) / 1000.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}
					break;

				case 3:/* 最后一个发射阵元 P632 */
					switch (TMP(last_element_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					/* 聚焦法则自动计算开启时  */
					if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) &&
							(LAW_VAL(Focal_type) == LINEAR_SCAN))
					{
						/*Law Config 为 Linear 时，Last Element可调*/
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = LAW_VAL (Last_tx_elem);
							lower = LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty) - 1; 
							upper = GROUP_VAL (probe.Elem_qty);
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_632, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL(Last_tx_elem);
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /*Law Config 不为 Linear 时，Last Element  Unsensitive*/
					{
						cur_value = LAW_VAL(Last_tx_elem);
						digit = 0;
						pos = 2;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}
					break;
				case 4:/* 保存聚焦法则 P642 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw_dialog_all (DIALOG_LAW_SAVE);
					else
						draw3_popdown(NULL, 2, 1);
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> type p702 */

					pp->x_pos = 540, pp->y_pos = 288-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_702, NULL, 
								menu_content[E_TYPE + get_enc_type (pp->p_config, get_cur_encoder (pp->p_config))],
								menu_content + E_TYPE, 9, 2, get_enc_type (pp->p_config, get_cur_encoder (pp->p_config)), 0);
					else 
						draw3_popdown (menu_content[E_TYPE + get_enc_type (pp->p_config, get_cur_encoder (pp->p_config))], 2, 0);
					if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
					{
						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}
					break;
				case 1:/*Scan -> Inspection -> Index p712 */
					pp->x_pos = 570, pp->y_pos = 305;
					if(get_inspec_type (pp->p_config)==0)
					{
						/*if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_712, NULL, 
									menu_content[I_INDEX + 4],
									menu_content + I_INDEX, 5, 2, 4, 0);
						else */
							draw3_popdown (menu_content[I_INDEX + 4], 2, 0);

						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}

					else if(get_inspec_type (pp->p_config)==1)
					{
						if(get_inspec_source (pp->p_config)==0)
							set_inspec_source (pp->p_config, SCAN_ENCODER1);
						/*if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_712, NULL, 
									menu_content[I_INDEX + get_inspec_index (pp->p_config)],
									menu_content + I_INDEX, 5, 2, get_inspec_index (pp->p_config), 0);
						else */
						if(get_inspec_source (pp->p_config)==1)
							draw3_popdown (menu_content[I_INDEX + 2], 2, 0);
						else
							draw3_popdown (menu_content[I_INDEX + 1], 2, 0);

						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if(get_inspec_source (pp->p_config)==1)
								menu_status = 0x12;
							else if(get_inspec_source (pp->p_config)==2)
								menu_status = 0x14;
							else
								menu_status = 0x10;

							draw3_pop_tt (data_712, NULL, 
									menu_content[I_INDEX + get_inspec_index (pp->p_config)],
									menu_content + I_INDEX, 5, 2, get_inspec_index (pp->p_config), menu_status);
						}
						else 
							draw3_popdown (menu_content[I_INDEX + get_inspec_index (pp->p_config)], 2, 0);
					}
					break;

				case 2:/*Scan -> Area -> scan resolution p722 */
					/* 当前步进 */
					switch (TMP(scan_resolution_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if(get_unit(pp->p_config) == UNIT_MM)
						{
							cur_value = get_area_scanresolution (pp->p_config)/1000.0;
							//cur_value = get_inspec_speed (pp->p_config)/100.0/get_prf();
							lower = 0.08;
							upper = 357.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_area_scanresolution (pp->p_config)/1000.0*0.03937;
							//cur_value = get_inspec_speed (pp->p_config)/100.0/get_prf()*0.03937;
							lower = 0.001;
							upper = 9.281;
							step = tmpf/10.0;
							digit = 3;
							pos = 2;
							unit = UNIT_INCH;
						}
						draw3_digit_pressed (data_722, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if(get_unit(pp->p_config) == UNIT_MM)
						{
							cur_value = get_area_scanresolution (pp->p_config)/1000.0;
							//cur_value = get_inspec_speed (pp->p_config)/100.0/get_prf();
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_area_scanresolution (pp->p_config)/1000.0*0.03937;
							//cur_value = get_inspec_speed (pp->p_config)/100.0/get_prf()*0.03937;
							digit = 3;
							pos = 2;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 3:/*Scan -> start -> start  p732 */
					if (get_start_pause (pp->p_config)==0)
						draw3_popdown(NULL,2,1);
					else
					{
						draw3_popdown(NULL,2,1);
						gtk_widget_set_sensitive (pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[2],FALSE);
					}
					break;
				case 4:
					if ( !con2_p[7][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/* File -> File -> save setup as  p802 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw_dialog_all (DIALOG_SAVE_SETUP_AS);
					else
						draw3_popdown(NULL,2,1);

					break;

				case 1:/*File -> report -> paper size  p812 */
					pp->x_pos = 605, pp->y_pos = 287-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_812, NULL, 
								menu_content[PAPER_SIZE+get_report_paper_size (pp->p_config)],
								menu_content+PAPER_SIZE, 4, 2,get_report_paper_size (pp->p_config), 0x0b);
					else 
						draw3_popdown (menu_content[PAPER_SIZE+get_report_paper_size (pp->p_config)], 2, 0);
					//gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					//gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);

					break;

				case 2:/* File -> format -> setup  p822 */
					draw3_popdown (menu_content[OFF_ON + get_report_format_setup (pp->p_config)], 2, 0);
					break;

				case 3:/*File -> user field -> label p832 */
					draw3_popdown (menu_content[U_LABEL + get_report_userfield_select (pp->p_config)], 2, 0);
					break;
				case 4:
					if ( !con2_p[8][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> scheme  p902 */
					if ( !con2_p[9][0][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;

				case 1:/*Preferences -> system -> select key  p912 */
					if ( !con2_p[9][1][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;


				case 2:/*Preferences -> service -> Import/Export p922 */
					if ( !con2_p[9][2][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;

				case 3:/*Preferences -> Options -> Remote Desktop p932 */
					if ( !con2_p[9][3][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;

				case 4:/*Preferences -> network -> Apply p942 */
					if ( !con2_p[9][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
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
	gfloat tmpf = 0.0;
	gchar *str;

	gfloat cur_value=0.0, lower = 0, upper = 0, step = 0;
	guint digit = 0, pos, unit = 0, content_pos, menu_status = 0, temp_beam, temp_beam1;

	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/* p003 */

					if (pp->start_qty == 4)
					{
						if(g_tmp_group_struct.group_mode == PA_SCAN)
						{
							switch (TMP(receiver_reg))
							{
								case 0:	tmpf = 1.0; break;
								case 1:	tmpf = 10.0; break;
								case 2:	tmpf = 100.0; break;
								default:break;
							}
							upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
							if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 3))
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_RECEIVER));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 3;
								str = g_strdup_printf ("( 1 to %d)", (int)(upper));
								draw3_digit_pressed (data_00341, str, cur_value , lower, upper, step, digit, p, pos, 19);
							}
							else 
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_RECEIVER));
								digit = 0;
								pos = 3;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_stop (cur_value, str, digit, pos, 19);
							}
							gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
							gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
							g_free(str);

						}
						else
						{
							switch (TMP(pulser_reg))
							{
								case 0:	tmpf = 1.0; break;
								case 1:	tmpf = 10.0; break;
								case 2:	tmpf = 100.0; break;
								default:break;
							}
							upper = (gfloat) (128 + 1 - LAW_VAL(Last_tx_elem));
							if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 3))
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_PULSER));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 3;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_00342, str, cur_value , lower, upper, step, digit, p, pos, 17);
							}
							else 
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_PULSER));
								digit = 0;
								pos = 3;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_stop (cur_value, str, digit, pos, 17);
							}
							g_free(str);
						}
					}

					else if ( pp->start_qty == 5 )
					{
						if(get_auto_detect (pp->p_config))
						{
							if(get_probe_select (pp->p_config)==0)
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
									draw_dialog_all (DIALOG_PROBE);
								}
								else
									draw3_popdown((g_tmp_group_struct.probe.Model), 3,0);

								g_sprintf (temp,"%s", con2_p[5][0][3]);
								gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
							}
							else if(get_probe_select (pp->p_config)==1)
							{
								draw3_popdown(NULL,3,1);
								g_sprintf (temp,"%s", con2_p[5][0][6]);
								gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
							}

							gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
						}
						else
						{
							if(get_probe_select (pp->p_config)==0)
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
									draw_dialog_all (DIALOG_PROBE);
								}
								else
									draw3_popdown((g_tmp_group_struct.probe.Model), 3, 0);

								g_sprintf (temp,"%s", con2_p[5][0][3]);
								gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
								gtk_label_set_text (GTK_LABEL (pp->data3[3]), (g_tmp_group_struct.probe.Model));
							}
							else if( get_probe_select (pp->p_config)==1 )
							{
								draw3_popdown(NULL,3,1);
								g_sprintf (temp,"%s", con2_p[5][0][6]);
								gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
							}
						}
						if( (g_tmp_group_struct.group_mode) && get_auto_detect (pp->p_config))	/* 自动检测开启时同时又在PA模式时，探头不可选 */
						{
							gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
						}
						else
						{
							gtk_widget_set_sensitive(pp->eventbox30[3],TRUE);
							gtk_widget_set_sensitive(pp->eventbox31[3],TRUE);
						}

					}
					else if( pp->start_qty == 6 )
					{
						if (get_probe_select (pp->p_config)==0)
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								draw_dialog_all (DIALOG_WEDGE);
							else
								draw3_popdown((g_tmp_group_struct.wedge.Model), 3, 0);

							g_sprintf (temp,"%s", con2_p[5][0][4]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}
						else if(get_probe_select (pp->p_config)==1)
						{
							draw3_popdown(NULL,3,1);
							g_sprintf (temp,"%s", con2_p[5][0][7]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}
					}
					else if( pp->start_qty == 7 )
					{
						switch (TMP(indexoffset_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 1.0; break;
							case 2:	tmpf = 5.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							cur_value = (g_tmp_group_struct.index_offset)/10.0;
							lower = -1000000.0;
							upper =  9999999.0;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_0037, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
						}
						else 
						{
							cur_value =(g_tmp_group_struct.index_offset)/10.0;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[3]);
						gtk_widget_hide (pp->eventbox31[3]);
					}
					break;
				case 1:/*p013*/
					if(pp->fstart_qty == 2)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							draw3_pop_tt (data_533, NULL, 
									get_material_name (pp->p_config),
									menu_content + MATERIAL, 24, 3, get_part_material(pp->p_config), 0);
						else 
							draw3_popdown (get_material_name(pp->p_config), 3, 0);
					}
					else if(pp->fstart_qty == 3)
					{
						switch (TMP(first_element_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)/* 聚焦法则自动计算开启时, First Element才可调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								cur_value = LAW_VAL(First_tx_elem);
								lower = 1.0;
								/* 计算最大值 */
								upper = GROUP_VAL(probe.Elem_qty) - LAW_VAL(Elem_qty) + 1;
								step = tmpf;
								digit = 0;
								pos = 3;
								unit = UNIT_NONE;
								draw3_digit_pressed (data_631, units[unit], cur_value , lower, upper, step, digit, p, pos, 9);
							}
							else 
							{
								cur_value = LAW_VAL(First_tx_elem);
								digit = 0;
								pos = 3;
								unit = UNIT_NONE;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
							}
						}
						else /* 聚焦法则自动计算为off时, First Element 不可以调节 */
						{
							cur_value = LAW_VAL(First_tx_elem);
							digit = 0;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
							gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
						}
					}
					else if(pp->fstart_qty == 4)
					{
						switch (TMP(max_angle_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((LAW_VAL(Focal_type) == AZIMUTHAL_SCAN) &&
								(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON))
							/* 聚焦法则自动计算开启时并且是角度扫查, Max Angle 才可调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								/* 计算lower为妙 */
								cur_value = LAW_VAL(Angle_max) / 100.0;
								temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
								TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
										LAW_VAL(Angle_beam_skew_min)) /
									LAW_VAL(Angle_beam_skew_step) + 1;
								temp_beam = temp_beam / TMP(beam_skew_num);
								lower = LAW_VAL (Angle_min) / 100.0;
								upper = MIN ((gint)(LAW_VAL (Angle_min) +
											(gint)(temp_beam * LAW_VAL(Angle_step))) / 100.0, 89.9);
								step = tmpf;
								digit = 1;
								pos = 3;
								unit = UNIT_DEG;
								draw3_digit_pressed (data_611, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 11);
							}
							else 
							{
								cur_value = LAW_VAL(Angle_max) / 100.0;
								digit = 1;
								pos = 3;
								unit = UNIT_DEG;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
							}
						}
						else
						{
							cur_value = LAW_VAL(Angle_max) / 100.0;
							digit = 1;
							pos = 3;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
							gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[3]);
						gtk_widget_hide (pp->eventbox31[3]);
					}
					break;

				case 2:/*Wizard -> Calibration -> Mode  p023 */
					pp->x_pos = 560, pp->y_pos = 380;
					switch(pp->cstart_qty)
					{
						case 0:
						case 1:
							if(pp->ctype_pos == 0)
							{
								if ((get_inspec_type (pp->p_config) == 1) || (get_inspec_type (pp->p_config) == 2))
								{
									if ((p->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
										draw3_pop_tt (data_700, NULL, 
											menu_content[ENCODER+get_cur_encoder (pp->p_config)],
											menu_content+ENCODER, 2, 3,get_cur_encoder (pp->p_config), 0);
									else 
										draw3_popdown (menu_content[ENCODER+get_cur_encoder (pp->p_config)], 3, 0);
								}
								else
								{
									if ((get_inspec_source (pp->p_config) == 1) || (get_inspec_source (pp->p_config) == 2))
										draw3_popdown (menu_content[ENCODER+(get_inspec_source (pp->p_config)-1)], 3, 0);
									else
										draw3_popdown (menu_content[ENCODER+ 0 ], 3, 0);
									gtk_widget_set_sensitive(p->eventbox30[3],FALSE);
									gtk_widget_set_sensitive(p->eventbox31[3],FALSE);
								}
							str = g_strdup_printf ("%s", con2_p[0][2][40]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
							}
							else if(pp->ctype_pos == 1)
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									draw3_pop_tt (data_023, NULL,
											menu_content[CMODE + pp->cmode_pos],
											menu_content+CMODE, 4, 3, pp->cmode_pos, 0x0);
								else
									draw3_popdown (menu_content[CMODE + pp->cmode_pos], 3, 0);
							}
							else if(pp->ctype_pos == 2)
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									draw3_pop_tt (data_0231, NULL,
											menu_content[SCODE + pp->scode_pos],
											menu_content+SCODE, 2, 3, pp->scode_pos, 0x0);
								else
									draw3_popdown (menu_content[SCODE + pp->scode_pos], 3, 0);

								g_sprintf (temp,"%s", con2_p[0][2][6]);
								gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
							}
							break;

						case 2:
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								switch (TMP(db_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 0.5; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 2.0; break;
									case 4:	tmpf = 6.0; break;
									default:break;
								}

								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
									cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
									lower = 0.0 - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
									upper = GAIN_MAX - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
									step = tmpf;
									digit = 1;
									pos = 3;
									unit = UNIT_DB;
									draw3_digit_pressed (data_100, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 9);
								}
								else
								{
									cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
									digit = 1;
									pos = 3;
									unit = UNIT_DB;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
								}
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{
								if(pp->echotype_pos == 0)
								{
									switch (TMP(radiusa_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									{
										cur_value = pp->radiusa / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0235, units[unit], cur_value , lower, upper, step, digit, p, pos, 23);
									}
									else
									{
										cur_value = pp->radiusa / 1000.0;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 23);
									}

								}
								else if(pp->echotype_pos == 1)
								{
									switch (TMP(deptha_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									{
										cur_value = pp->deptha / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0236, units[unit], cur_value , lower, upper, step, digit, p, pos, 24);
									}
									else
									{
										cur_value = pp->deptha / 1000.0;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 24);
									}
								}
								else
								{
									switch (TMP(thickness1_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									{
										cur_value = pp->thickness1 / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0234, units[unit], cur_value , lower, upper, step, digit, p, pos, 25);
									}
									else
									{
										cur_value = pp->thickness1 / 1000.0;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 25);
									}
								}
							}
							else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
							{
								switch(TMP(tolerance_t_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
									cur_value = pp->tolerance_t / 100.0 ;
									lower = 0.0;
									upper = 50.0;
									step = tmpf;
									digit = 1;
									pos = 3;
									unit = UNIT_BFH;
									draw3_digit_pressed (data_0239, units[unit], cur_value , lower, upper, step, digit, p, pos, 26);
								}
								else
								{
									cur_value = pp->tolerance_t / 100.0 ;
									digit = 1;
									pos = 3;
									unit = UNIT_BFH;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 26);
								}
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[3]);
								gtk_widget_hide (pp->eventbox31[3]);
							}
							break;
						case 3:
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								if(pp->echotype_pos == 0)
								{
									switch (TMP(radius1_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									{
										cur_value = pp->radius1 / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0232, units[unit], cur_value , lower, upper, step, digit, p, pos, 13);
									}
									else
									{
										cur_value = pp->radius1 / 1000.0;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 13);
									}

								}
								else if(pp->echotype_pos == 1)
								{
									switch (TMP(depth1_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									{
										cur_value = pp->depth1 / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0233, units[unit], cur_value , lower, upper, step, digit, p, pos, 15);
									}
									else
									{
										cur_value = pp->depth1 / 1000.0;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
									}
								}
								else
								{
									switch (TMP(thickness1_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
									{
										cur_value = pp->thickness1 / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0234, units[unit], cur_value , lower, upper, step, digit, p, pos, 17);
									}
									else
									{
										cur_value = pp->thickness1 / 1000.0;
										digit = 2;
										pos = 3;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 17);
									}

								}
							}
							else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
							{
								switch (TMP(max_angle_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
								/* 计算lower为妙 */
									cur_value = LAW_VAL(Angle_max) / 100.0;
									temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
									TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
											LAW_VAL(Angle_beam_skew_min)) /
										LAW_VAL(Angle_beam_skew_step) + 1;
									temp_beam = temp_beam / TMP(beam_skew_num);
									lower = LAW_VAL (Angle_min) / 100.0;
									upper = LAW_VAL(Angle_max) / 100.0;
									step = tmpf;
									digit = 1;
									pos = 3;
									unit = UNIT_DEG;
									draw3_digit_pressed (data_611, units[unit], cur_value , lower, upper,
											step, digit, p, pos, 28);
								}
								else 
								{
									cur_value = LAW_VAL(Angle_max) / 100.0;
									digit = 1;
									pos = 3;
									unit = UNIT_DEG;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 28);
								}
#if 0
								switch (TMP(last_angle_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 3))
								{
									cur_value = pp->last_angle/100.0 ;
									lower =	0.0;
									upper =	89.9;
									step = tmpf;
									digit = 1;
									pos = 3;
									unit = UNIT_DEG;
									draw3_digit_pressed (data_0238, units[unit], cur_value,
											lower, upper, step, digit, p, pos, 28);
								}
								else
								{
									cur_value = pp->last_angle/100.0 ;
									unit = UNIT_DEG;
									pos = 3;
									digit = 1;
									draw3_digit_stop (cur_value , units[unit], digit, pos, 28);
								}
#endif
							}
							else if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
							{
								switch (pp->p_tmp_config->cwidth_reg)
								{
									case 0:	tmpf = 3.2; break;
									case 1:	tmpf = 16.0; break;
									case 2:	tmpf = 32.0; break;
									case 3:	tmpf = 64.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											lower = 3.2 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											digit = 2;
											pos = 3;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											lower =	3.2 * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											digit = 3;
											pos = 3;
											unit = UNIT_INCH;
										}
									}
									else
									{
											cur_value = GROUP_GATE_POS(width) / 1000.0;   /* us */
											lower = 3.2 ;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) ;
											step = tmpf;
											digit = 2;
											pos = 3;
											unit = UNIT_US;
									}
									draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos, 20);
								}
								else
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 3;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 3;
										}
									}
									else
									{
											cur_value = GROUP_GATE_POS(width) / 1000.0;   /* us */
											digit = 2;
											pos = 3;
											unit = UNIT_US;
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 20);
								}
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[3]);
								gtk_widget_hide (pp->eventbox31[3]);
							}
							break;
						case 4:
							if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 0)||(pp->cmode_pos == 1)||(pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
							{
								switch (pp->p_tmp_config->cwidth_reg)
								{
									case 0:	tmpf = 3.2; break;
									case 1:	tmpf = 16.0; break;
									case 2:	tmpf = 32.0; break;
									case 3:	tmpf = 64.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											lower = 3.2 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											digit = 2;
											pos = 3;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											lower =	3.2 * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											digit = 3;
											pos = 3;
											unit = UNIT_INCH;
										}
									}
									else
									{
											cur_value = GROUP_GATE_POS(width) / 1000.0;   /* us */
											lower = 3.2 ;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) ;
											step = tmpf;
											digit = 2;
											pos = 3;
											unit = UNIT_US;
									}
									draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos, 20);
								}
								else
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 3;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 3;
										}
									}
									else
									{
											cur_value = GROUP_GATE_POS(width) / 1000.0;   /* us */
											digit = 2;
											pos = 3;
											unit = UNIT_US;
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 20);
								}
							}
							else if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
							{
								draw3_popdown_offset(NULL, 3,1,29);
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[3]);
								gtk_widget_hide (pp->eventbox31[3]);
							}
							break;
						case 5:
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0) )
							{

								switch (pp->p_tmp_config->cwidth_reg)
								{
									case 0:	tmpf = 3.2; break;
									case 1:	tmpf = 16.0; break;
									case 2:	tmpf = 32.0; break;
									case 3:	tmpf = 64.0; break;
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											lower = 3.2 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											digit = 2;
											pos = 3;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											lower =	3.2 * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											digit = 3;
											pos = 3;
											unit = UNIT_INCH;
										}
									}
									else
									{
											cur_value = GROUP_GATE_POS(width) / 1000.0;   /* us */
											lower = 3.2 ;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) ;
											step = tmpf;
											digit = 2;
											pos = 3;
											unit = UNIT_US;
									}
									draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos, 20);
								}
								else
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 3;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 3;
										}
									}
									else
									{
											cur_value = GROUP_GATE_POS(width) / 1000.0;   /* us */
											digit = 2;
											pos = 3;
											unit = UNIT_US;
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 20);
								}


							}
							else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 3)) )
							{
								draw3_popdown_offset(NULL, 3,1,29);
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
							{
								draw3_popdown_offset(NULL, 3,1,35);
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[3]);
								gtk_widget_hide (pp->eventbox31[3]);
							}
							break;
						case 6:
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
							{
								draw3_popdown_offset(NULL, 3,1,29);
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[3]);
								gtk_widget_hide (pp->eventbox31[3]);
							}
							break;

					}
					break;


				case 3:
					if ( !con2_p[0][3][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 4:
					if ( !con2_p[0][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0:/* wedge delay P103 TAN1 */
					switch (TMP(wedge_delay_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_WEDGE_DELAY) / 1000.0;
						lower = 0.0;
						upper = 1000.0;
						step = tmpf;
						digit = 2;
						pos = 3;
						unit = UNIT_US;
						draw3_digit_pressed (data_103, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_WEDGE_DELAY) / 1000.0;
						digit = 2;
						pos = 3;
						unit = UNIT_US;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 1: /* 发射电压高低  P113 TAN1 */
					pp->x_pos = 586, pp->y_pos = 373 - YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop_tt (data_113, NULL, 
								menu_content[VOLTAGE + 3 + get_voltage(pp->p_config, grp)],
								menu_content + VOLTAGE, 3, 3, get_voltage (pp->p_config, grp), 
								((pp->p_config->group[grp].group_mode) == PA_SCAN) ? 4 : 0);
					}
					else 
					{
						draw3_popdown (menu_content[VOLTAGE + 3 + get_voltage (pp->p_config, grp)], 3, 0);
					}
					break;
				case 2: /* Video Filter 视频滤波 P123 TAN1 */
					if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RECTIFIER) == RF_WAVE)
					{
						gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
					}
					draw3_popdown (menu_content
							[OFF_ON + get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER)], 3, 0);
					break;
				case 3: /* Skew (deg.) UT 无 P133 TAN1 */
					if (GROUP_VAL(group_mode) == UT_SCAN)
					{
						gtk_widget_hide (pp->eventbox30[3]);
						gtk_widget_hide (pp->eventbox31[3]);
					}
					else if (GROUP_VAL(group_mode) == PA_SCAN)
					{
						gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
						cur_value = GROUP_VAL(skew)/100.0 ;
						unit = UNIT_NULL;
						pos = 3;
						digit = 1;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 4:/* Points Qty.  P143 TAN1 */
					pp->x_pos = 585, pp->y_pos = 371-YOFFSET;
					switch (TMP(point_qty_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;						
						default:break;
					}
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 3))
					{
						if (pp->mark_pop_change)
						{
							cur_value = GROUP_VAL(point_qty) ;
							lower =	32.0;
//							upper =	8192.0;	/* 最大值需要计算 TAN1 */
							upper = (gfloat) (get_max_point_qty());
							step = tmpf;
							digit = 0;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_1431, units[unit], cur_value , lower, upper, step, 											digit,p, pos, 0);
						}
						else
						{
							if (GROUP_VAL(point_qty_pos) == 4)	/* 自定义数值时候按下显示数值 */
							{
								/* 更新当前增益值显示 */
								str = g_strdup_printf ("%d", GROUP_VAL(point_qty));
								draw3_pop_tt (data_143, NULL, 
										str, menu_content + POINT_QTY, 5, 3, 
										GROUP_VAL(point_qty_pos), 0);
								g_free(str);
							}
							else	/* Auto ==时候 显示当前选项 */
								draw3_pop_tt (data_143, NULL, 
										menu_content[POINT_QTY + GROUP_VAL(point_qty_pos)],
										menu_content + POINT_QTY, 5, 3, GROUP_VAL(point_qty_pos), 0);
						}
					}
					else 
					{
						cur_value = GROUP_VAL(point_qty) ;
						unit = UNIT_NULL;
						pos = 3;
						digit = 0;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}

					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/* 闸门宽度 或者 测量(Measure) P203 TAN1 */
					pp->x_pos = 580, pp->y_pos = 380 - YOFFSET;
					if (GROUP_GATE_POS(parameters) == 0)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || 
									(UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								switch (TMP(gate_width_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;
									default:break;
								}
								if (UNIT_MM == get_unit(pp->p_config))
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
									lower = 0.04 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
									upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 : 
											(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
									//step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
									step = tmpf;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
									lower =	0.04 * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
									upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
											(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
									//step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
									step = tmpf;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else /*ut_unit 选择 time 时*/
							{
								switch (TMP(gate_width_reg))
								{
									case 0:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / (gfloat)(GROUP_VAL(point_qty)); break;
									case 1:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 20.0 ; break;
									case 2:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 10.0 ; break;
									case 3:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 4.0 ; break;
									default:break;
								}
								cur_value = GROUP_GATE_POS(width) / 1000.0 ;
								lower =	0.04;
								upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) /1000.0 ) > 6400.0 ? 6400.0 : (MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0));
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_US;
							}
							draw3_digit_pressed (data_203, units[unit], cur_value , 
									lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) ||
									(UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if (UNIT_MM == get_unit(pp->p_config))
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
									unit = UNIT_MM;
									digit = 2;
									pos = 3;
								}
								else
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
									unit = UNIT_INCH;
									digit = 3;
									pos = 3;
								}
							}
							else
							{
								cur_value = GROUP_GATE_POS(width) / 1000.0 ;
								unit = UNIT_US;
								pos = 3;
								digit = 2;
							}
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							draw3_pop_tt (data_2031, NULL,
									menu_content[MEASURE + GROUP_GATE_POS(measure)],	
									menu_content + MEASURE, 2, 3, GROUP_GATE_POS(measure), 0);
							str = g_strdup_printf ("%s", con2_p[2][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
						}
						else 
						{
							draw3_popdown (menu_content[MEASURE + GROUP_GATE_POS(measure)], 3, 0);
							str = g_strdup_printf ("%s", con2_p[2][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
						}
						if (str)
							g_free(str);
					}
					//gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;
				case 1:/*Operator  P213*/
					pp->x_pos = 609, pp->y_pos = 371-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_213, NULL,menu_content[OPERAT + get_alarm_operator(pp->p_config)],
								menu_content + OPERAT, 2, 3, get_alarm_operator(pp->p_config), 0);
					else 
						draw3_popdown (menu_content[OPERAT + get_alarm_operator(pp->p_config)], 3, 0);
					break;

				case 2:/* Sound  p223 */
					pp->x_pos = 424, pp->y_pos = 370-YOFFSET;
					if (get_output_pos(pp->p_config) < 3)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							draw3_pop_tt (data_223, NULL,menu_content[SOUND + 5 + get_output_sound(pp->p_config)],
									menu_content + SOUND, 5, 3, get_output_sound(pp->p_config), 0);
						else 
							draw3_popdown (menu_content[SOUND + 5 + get_output_sound(pp->p_config)], 3, 0);
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[3]);
						gtk_widget_hide (pp->eventbox31[3]);
					}
					break;

				case 3:/* Gate/Alarm->Sizing Curves->Ref.Ampl.Offset  p233*/
					if( GROUP_VAL(mode_pos)==0 ) /* Mode 为 setup 时 */
					{
						if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2) /*Curve 选择 DAC or Linear DAC 时*/
						{
							/* 当前步进 */
							switch (TMP(ref_ampl_offset_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 0.5; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 2.0; break;
								case 4:	tmpf = 6.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								cur_value =GROUP_VAL(ref_ampl_offset)/100.0;
								lower = 0.0;
								upper = 74.0;
								step = tmpf;
								digit = 1;
								pos = 3;
								unit = UNIT_DB;
								draw3_digit_pressed (data_233, units[unit], cur_value , lower, upper, step, digit, p, pos, 7);
							}
							else 
							{
								cur_value = GROUP_VAL(ref_ampl_offset)/100.0;
								digit = 1;
								pos = 3;
								unit = UNIT_DB;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
							} 
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[3]);
							gtk_widget_hide (pp->eventbox31[3]);
							gtk_widget_hide (pp->sbutton[3]);
						}

					}
					else if( GROUP_VAL(mode_pos)==1 ) /* Mode 为 edit 时 */
					{
						if(GROUP_VAL(curve_pos)==1)/*Mode 选择 Edit , Curve 选择 DAC 时*/
						{
							/* 当前步进 */
							switch (TMP(amplitude_reg))
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								cur_value =GROUP_VAL(amplitude[GROUP_VAL(point_pos)])/1000.0;
								lower = 0.02;
								upper = 80.10;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_BFH;
								draw3_digit_pressed (data_2331, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
							}
							else 
							{
								cur_value = GROUP_VAL(amplitude[GROUP_VAL(point_pos)])/1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_BFH;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
							} 

							if(GROUP_VAL(point_pos)==0)
							{
								gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
							}
						}
						else if(GROUP_VAL(curve_pos)==2) /*Mode 选择 Edit , Curve 选择 Linear DAC 时*/
						{
							gtk_widget_hide (pp->eventbox30[3]);
							gtk_widget_hide (pp->eventbox31[3]);
							gtk_widget_hide (pp->sbutton[3]);
						}
						else if(GROUP_VAL(curve_pos)==3) /*Mode 选择 Edit , Curve 选择 TCG 时*/
						{
							/* 当前步进 */
							switch (TMP(tcg_gain_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 0.5; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 2.0; break;
								case 4:	tmpf = 6.0; break;					
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								cur_value =GROUP_VAL(tcg_gain)/100.0;
								lower = 0.0;
								upper = 74.0;
								step = tmpf;
								digit = 1;
								pos = 3;
								unit = UNIT_DB;
								draw3_digit_pressed (data_2332, units[unit], cur_value , lower, upper, step, digit, p, pos, 17);
							}
							else 
							{
								cur_value = GROUP_VAL(tcg_gain)/100.0;
								digit = 1;
								pos = 3;
								unit = UNIT_DB;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 17);
							}
							if(GROUP_VAL(point_pos)==0)
							{
								gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
							}
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[3]);
							gtk_widget_hide (pp->eventbox31[3]);
							gtk_widget_hide (pp->sbutton[3]);
						}

					}
					break;
				case 4:
					if ( !con2_p[2][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				default:break;

			}
			break;
		case 3:
			switch (pp->pos1[3]) 
			{
				case 0:/* Measurements -> reading -> field2  P303*/
					pp->x_pos = 200, pp->y_pos = 0;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_303, NULL, 
								field[get_reading_field2(pp->p_config)],
								field1, 60, 3, get_reading_field2(pp->p_config), 0);
					else 
						draw3_popdown (field[get_reading_field2(pp->p_config)], 3, 0);

					break;


				case 1:/*Measurements -> Cursors -> Index p313 */
					if( GROUP_VAL(selection)==0 || GROUP_VAL(selection)==1)
					{
						/* 当前步进 */
						switch (TMP(u_reference_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 10.0;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
 									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)
										+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									step = tmpf;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference) / 1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									step = tmpf/2.0;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else	/* us */
							{
								cur_value = GROUP_VAL(u_reference)/1000.0;
								lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0;
								upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_US;
							}
							draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else
							{
								cur_value = GROUP_VAL(u_reference)/1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_US;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else if( GROUP_VAL(selection)==2)
					{
						/* 当前步进 */
						switch (TMP(i_reference_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 0.5;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 1.5;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_reference)/100.0;
								lower = -999999.0;
								upper = 999999.0;
								step = tmpf;
								digit = 1;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_reference)/100.0*0.03937;
								lower = -999999.0*0.03937;
								upper = 999999.0*0.03937;
								step = tmpf;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3131, units[unit], cur_value , lower, upper, step, digit, p, pos, 9);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_reference)/100.0;
								digit = 1;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_reference)/100.0*0.03937;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
						}
					}
					else if( GROUP_VAL(selection)==3)
					{
						/* 当前步进 */
						switch (TMP(s_refmeas_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 5.0;  break;
							case 2:	tmpf = 10.0;  break;
							case 3:	tmpf = 15.0;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(s_refmeas)/100.0;
								lower = 12.0;
								upper = 400.5;
								step = tmpf;
								digit = 1;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(s_refmeas)/100.0*0.03937;
								lower = 0.001;
								upper = 400.5*0.03937;
								step = tmpf/100.0;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3132, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(s_refmeas)/100.0;
								digit = 1;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(s_refmeas)/100.0*0.03937;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
						}

					}
					else if( GROUP_VAL(selection)==4)
					{
						/* 当前步进 */
						switch (TMP(u_measure_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 10.0;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									step = tmpf;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									step = tmpf;
									digit = 2;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else	/*us*/
							{
								cur_value = GROUP_VAL(u_measure)/1000.0;
								lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0;
								upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_US;
							}
							draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, p, pos, 4);
						}
						else
						{
							if((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else	/*????*/
							{
								cur_value = GROUP_VAL(u_measure)/1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_US;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 4);
						}

					}
					else if (GROUP_VAL(selection)==5||GROUP_VAL(selection)==6||GROUP_VAL(selection)==7)
					{
						/* 当前步进 */
						switch (TMP(cursors_ut_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1; break;
							case 2:	tmpf = 1.0; break;
							case 3:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_CURSORS_POS(UT)/100.0;
									lower = -0.07;
									upper = 58.3;
									step = tmpf;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_CURSORS_POS(UT)/100.0*0.03937;
									lower = -0.07*0.03937;
									upper = 58.3*0.03937;
									step = tmpf;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(UT)/100.0;
								lower = -0.07;
								upper = 58.3;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_US;

							}
							draw3_digit_pressed (data_3133, units[unit], cur_value , lower, upper, step, digit, p, pos, 14);
						}
						else
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_CURSORS_POS(UT)/100.0;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_CURSORS_POS(UT)/100.0*0.03937;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(UT)/100.0;
								digit = 2;
								pos = 3;
								unit = UNIT_US;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 14);
						}
					}
					else if (GROUP_VAL(selection)==8)
					{
						/* 当前步进 */
						switch (TMP(cursors_index_reg))
						{
							case 0:	tmpf = 0.5; break;
							case 1:	tmpf = 2.5; break;
							case 2:	tmpf = 5.0; break;
							case 3:	tmpf = 7.5; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0;
								lower = -100000.0;
								upper = 100000;
								step = tmpf;
								digit = 1;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0*0.03937;
								lower = -100000.0*0.03937;
								upper = 100000*0.03937;
								step = tmpf;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3151, units[unit], cur_value , lower, upper, step, digit, p, pos, 16);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0;
								digit = 1;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0*0.03937;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
						}

					}
					gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;

				case 2:/*Measurements -> Table -> Delete Entry  p323 */
					draw3_popdown(NULL,3,1);
					break;

				case 3:/*Measurements -> Thickness -> Echo Qty.  p333*/
					/* 当前步进 */
					switch (TMP(echo_qty_reg))
					{
						case 0:	tmpf = 1.0; break;

						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = get_echo_qty(pp->p_config);
						lower = 1.0;
						upper = 10.0;
						step = tmpf;
						digit = 0;
						pos = 3;
						unit = UNIT_NONE;
						draw3_digit_pressed (data_333, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = get_echo_qty(pp->p_config);
						digit = 0;
						pos = 3;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}

					break;


				case 4:
					if ( !con2_p[3][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/*p403*/
					pp->x_pos = 550, pp->y_pos = 410;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))/*选中403这个位置*/
					{
						if(get_display_pos(pp->p_config)==7)
							/*Display 为 A-C-[C]*/
						{
							if (get_group_qty(pp->p_config) == 1)
								menu_status = 0x01;
							draw3_pop_tt (data_401, NULL, 
									menu_content[GROUP_POS+get_display_group(pp->p_config)],
									menu_content + GROUP_POS, 2, 3, get_display_group(pp->p_config), menu_status);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);

						}
						else if(get_display_pos(pp->p_config)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_pop_tt (data_403, NULL, 
									menu_content[DIS_MODE+get_stripscan_mode(pp->p_config)],
									menu_content + DIS_MODE, 4, 3, get_stripscan_mode(pp->p_config), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][10]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);

						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[3]);
							gtk_widget_hide (pp->eventbox31[3]);
						}

					}
					else 
					{
						if(get_display_pos(pp->p_config)==7)/* Display 为 A-C-[C] */
						{
							draw3_popdown (menu_content[GROUP_POS+get_display_group(pp->p_config)], 3, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
						}
						else if(get_display_pos(pp->p_config)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_popdown (menu_content[DIS_MODE+get_stripscan_mode(pp->p_config)], 3, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][10]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);

						}

						else 
						{
							gtk_widget_hide (pp->eventbox30[3]);
							gtk_widget_hide (pp->eventbox31[3]);
						}
					}
					break;

				case 1:/*Display -> Overlay -> gate  p413 */
					draw3_popdown(menu_content[OFF_ON + get_overlay_gate (pp->p_config)],3,0);
					gtk_widget_queue_draw(pp->vboxtable);
					break;

				case 2:/* Load color 调色板 P423 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw_dialog_all (DIALOG_COLOR_PALETTE);
					draw3_popdown(NULL, 3, 1);
					break;
				case 3:/* A_SCAN -> Source  P433 */
					pp->x_pos = 567, pp->y_pos =368-YOFFSET;
					switch(get_dis_prop_scan(pp->p_config))
					{
						case 0:	/* Ascan 时候的source */
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								if ((get_display_pos(pp->p_config) == B_SCAN) ||	(get_display_pos(pp->p_config) == A_B_SCAN)||
										(get_display_pos(pp->p_config) == A_B_C_SCAN)|| (get_display_pos(pp->p_config) == A_B_S_SCAN))
									menu_status = 0x0e;
								draw3_pop_tt (data_443, NULL, 
										menu_content[PROP_SOURCE + GROUP_VAL(ascan_source)],
										menu_content+PROP_SOURCE, 4, 3, GROUP_VAL(ascan_source), menu_status);
							}
							else 
								draw3_popdown (menu_content[PROP_SOURCE + GROUP_VAL(ascan_source)], 3, 0);
							break;
						case 1:
							gtk_widget_hide(pp->eventbox30[3]);
							gtk_widget_hide(pp->eventbox31[3]);
							break;
						case 2:
							gtk_widget_hide(pp->eventbox30[3]);
							gtk_widget_hide(pp->eventbox31[3]);
							break;
						case 3:
							gtk_widget_hide(pp->eventbox30[3]);
							gtk_widget_hide(pp->eventbox31[3]);
							break;
						case 4:
							gtk_widget_hide(pp->eventbox30[3]);
							gtk_widget_hide(pp->eventbox31[3]);
							break;
						case 5:
							gtk_widget_hide(pp->eventbox30[3]);
							gtk_widget_hide(pp->eventbox31[3]);
							break;
						default:break;

					}
					break;
				case 4:
					if ( !con2_p[4][4][3] )
						gtk_widget_hide (p->eventbox30[3]);
					gtk_widget_hide (p->eventbox31[3]);
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/* Probe/Part -> Select -> Probe 探头选择  P503 */
					if(get_auto_detect (pp->p_config))
					{
						if(get_probe_select (pp->p_config)==0)
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								draw_dialog_all (DIALOG_PROBE);
							}
							else
								draw3_popdown(GROUP_VAL(probe.Model), 3,0);

							g_sprintf (temp,"%s", con2_p[5][0][3]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}
						else if(get_probe_select (pp->p_config)==1)
						{
							draw3_popdown(NULL,3,1);
							g_sprintf (temp,"%s", con2_p[5][0][6]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);	
						}

						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					else
					{
						if(get_probe_select (pp->p_config)==0)
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								draw_dialog_all (DIALOG_PROBE);
							}
							else
								draw3_popdown(GROUP_VAL(probe.Model), 3, 0);

							g_sprintf (temp,"%s", con2_p[5][0][3]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
							gtk_label_set_text (GTK_LABEL (pp->data3[3]), GROUP_VAL(probe.Model));
						}
						else if( get_probe_select (pp->p_config)==1 )
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								draw_dialog_all (DIALOG_DEFINE_PROBE);
							}
							else
							{	
								draw3_popdown(GROUP_VAL(probe.Model), 3,0);
							}
							g_sprintf (temp,"%s", con2_p[5][0][6]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}
					}
					if( GROUP_VAL(group_mode) && get_auto_detect (pp->p_config))	/* 自动检测开启时同时又在PA模式时，探头不可选 */
					{
						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					else
					{
						gtk_widget_set_sensitive(pp->eventbox30[3],TRUE);
						gtk_widget_set_sensitive(pp->eventbox31[3],TRUE);
					}
					break;
				case 1:
					if ( !con2_p[5][1][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 2:/*Probe/Part -> Characterize -> width p523 */
					/* 当前步进 */
# if 0

					switch (TMP(agate_width_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = GROUP_VAL(agate_width)/1000.0;
						lower = 0.12;
						upper = 29.98;
						step = tmpf;
						digit = 2;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_pressed (data_523, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = GROUP_VAL(agate_width)/1000.0;
						digit = 2;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
#endif

					switch (TMP(gate_width_reg))
					{
						case 0:	tmpf = 3.2; break;
						case 1:	tmpf = 16.0; break;
						case 2:	tmpf = 32.0; break;
						case 3:	tmpf = 64.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
								lower = 3.2 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 : 
										(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
								lower =	3.2 * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
										(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
								step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = GROUP_VAL(gate[0].width) / 1000.0 ;
							lower =	3.2;
							upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) /1000.0 ) > 6400.0 ? 6400.0 : (MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0));
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_US;
						}
						draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 3;
							}
							else
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
								unit = UNIT_INCH;
								digit = 3;
								pos = 3;
							}
						}
						else
						{
							cur_value = GROUP_VAL(gate[0].width) / 1000.0 ;
							unit = UNIT_US;
							pos = 3;
							digit = 2;
						}
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 3:/* 选择被测材料类型 P533 */
					pp->x_pos = 520, pp->y_pos = 140 - YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_533, NULL, 
								get_material_name(pp->p_config),
								menu_content + MATERIAL, 24, 3, get_part_material(pp->p_config), 0);
					else 
						draw3_popdown (get_material_name(pp->p_config), 3, 0);
					break;
				case 4:
					if ( !con2_p[5][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:
					if ( !con2_p[6][0][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 1:/* beam skew angle min 2D时候可以用 P613 */
					switch (TMP(beam_skew_min_angle_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((LAW_VAL(Focal_type) == AZIMUTHAL_SCAN) &&
							(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) 
							&& (GROUP_VAL(probe.PA_probe_type) == 9)
							/* 如何判断2D探头 */
					   )
						/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							/* 最大不能超过最大Angle_beam_skew_max */
							cur_value = LAW_VAL (Angle_beam_skew_min) / 100.0;
							/* 计算lower为妙 */
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
							TMP(angle_num)	= (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) /
								LAW_VAL(Angle_step) + 1;
							temp_beam1 = temp_beam / TMP(angle_num);
							lower = MAX (((gint)(LAW_VAL (Angle_beam_skew_max)) - 
										(gint)(temp_beam1 * LAW_VAL(Angle_beam_skew_step))) / 100.0, -89.9);
							upper = LAW_VAL (Angle_beam_skew_max) / 100.0;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_613, units[unit], cur_value , lower, 
									upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL (Angle_beam_skew_min) / 100.0;
							digit = 1;
							pos = 3;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /* 聚焦法则自动计算为OFF, Min.Angle 不可调节 */
					{
						cur_value = LAW_VAL (Angle_beam_skew_min) / 100.0;
						digit = 1;
						pos = 3;
						unit = UNIT_DEG;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					break;
				case 2:/* 聚焦深度 线扫   聚焦声程 角度扫查 P623 */
					if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
						/* 聚焦法则自动计算开启时, Max Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(LAW_VAL(Focal_type) == 2)
								/*law type 为 depth sector 时*/
							{
								switch (TMP(positionstep_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								cur_value = LAW_VAL(Position_step) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;

								draw3_digit_pressed (data_623, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 0);
							}
							else if(LAW_VAL(Focal_point_type)==FOCALPLANE_P)
								/*type 为 focal plane 时*/
							{
								switch (TMP(depths_reg))
								{
									case 0:	tmpf = 0.5; break;
									case 1:	tmpf = 5.0; break;
									case 2:	tmpf = 50.0;break;
									default:break;
								}
								cur_value = LAW_VAL(Depth_start) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;

								draw3_digit_pressed (data_6231, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 8);
							}
							else
							{
								cur_value = LAW_VAL(Position_step) / 1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

								gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
							}
						}
						else
						{
							if(LAW_VAL(Focal_type) == 2)
								/*law type 为 depth sector 时*/
							{
								cur_value = LAW_VAL(Position_step) / 1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
							else if(LAW_VAL(Focal_point_type)==FOCALPLANE_P)
								/*type 为 focal plane 时*/
							{
								cur_value = LAW_VAL(Depth_start) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
							}
							else
							{
								cur_value = LAW_VAL(Position_step) / 1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

								gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
							}
						}
					}
					else
					{
						cur_value = LAW_VAL(Position_step) / 1000.0;
						digit = 2;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					break;

# if 0
					switch (TMP(focus_depth_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON))
						/* 自动计算聚焦法则时候可以调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = LAW_VAL(Focus_depth) / 1000.0;
								lower = 0.10;
								upper = 10000.00;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = LAW_VAL(Focus_depth) / 1000.0 * 0.03937 ;
								lower = 0.004;
								upper = 10000.00*0.03937;
								step = tmpf/10.0;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_623, units[unit], cur_value , lower, upper,
									step, digit, p, pos, 0);
						}
						else 
						{
							if(UNIT_MM == get_unit(pp->p_config))
							{
								cur_value = LAW_VAL(Focus_depth)/1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = LAW_VAL(Focus_depth)/1000.0 * 0.03937;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = LAW_VAL(Focus_depth)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = LAW_VAL(Focus_depth)/1000.0 * 0.03937;
							digit = 3;
							pos = 3;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					break;
#endif
				case 3:/* 阵元步进 线扫时候用 P633 */
					switch (pp->p_tmp_config->element_step_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) &&
							(LAW_VAL(Focal_type) == LINEAR_SCAN))
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							cur_value = LAW_VAL(Elem_step);
							lower = 1.0;
							upper = GROUP_VAL (probe.Elem_qty) - LAW_VAL (Last_tx_elem);
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_633, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL(Elem_step);
							digit = 1;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /*Law Config 不为 Linear 时，Element Step  Unsensitive*/
					{
						cur_value = LAW_VAL(Elem_step);
						digit = 1;
						pos = 3;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					break;
				case 4: /* 计算聚焦法则 P643 */
					draw3_popdown (NULL, 3, 1);
					if (get_auto_focal (pp->p_config) == AUTO_FOCAL_OFF)
					{
						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> resolution  p703 */

					/* 当前步进 */
					switch (TMP(encoder_resolution_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
						lower = 0.001;
						upper = 100.00;
						step = tmpf;
						digit = 3;
						pos = 3;
						unit = UNIT_STEP_MM;
						draw3_digit_pressed (data_703, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
						digit = 3;
						pos = 3;
						unit = UNIT_STEP_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
					{
						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					break;
				case 1:/* 扫查移动速度 P713 */
					switch (TMP(scanspeed_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if(get_inspec_source (pp->p_config)==0)
						content_pos=0;
					else
						content_pos=6;

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = get_inspec_speed (pp->p_config) / 1000.0;
							lower = 1.0;
							upper = 6000.0;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM_S;
						}
						else
						{
							cur_value = get_inspec_speed (pp->p_config) / 1000.0 * 0.03937;
							lower = 0.039;
							upper = 236.220;
							step = tmpf;
							digit = 3;
							pos = 3;
							unit = UNIT_INCH_S;
						}
						draw3_digit_pressed (data_713, units[unit], cur_value , lower, upper, step, digit, p, pos, content_pos);
					}
					else 
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = get_inspec_speed (pp->p_config)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM_S;
						}
						else
						{
							cur_value = get_inspec_speed (pp->p_config)/1000.0 * 0.03937;
							digit = 3;
							pos = 3;
							unit = UNIT_INCH_S;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
					}
					break;
				case 2:/*Scan -> area -> index start p723 */
					/* 当前步进 */
					switch (TMP(index_start_reg))
					{
						case 0:	tmpf = 0.001 * get_area_indexresolution (pp->p_config); break;
						case 1:	tmpf = 0.01 * get_area_indexresolution (pp->p_config); break;
						case 2:	tmpf = 0.1 * get_area_indexresolution (pp->p_config); break;
						default:break;
					}
					if(get_inspec_type (pp->p_config)==1 || get_inspec_type (pp->p_config)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = get_area_indexstart (pp->p_config)/1000.0;
								lower = -99999.0;
								upper = 99999.0;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_indexstart (pp->p_config) / 1000.0 * 0.03937;
								lower = -99999.0 * 0.03937;
								upper = 99999.0 * 0.03937;
								step = tmpf*0.03937;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_723, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = get_area_indexstart (pp->p_config)/1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_indexstart (pp->p_config)/1000*0.03937;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
						/* Inspection -> Type 选择 One-Line Scan 时 */
					{
						if(get_unit(pp->p_config) == UNIT_MM)
						{
							cur_value = get_area_indexstart (pp->p_config);
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_area_indexstart (pp->p_config)/1000*0.03937;
							digit = 3;
							pos = 3;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
					}
					break;

				case 3:
					if ( !con2_p[7][3][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 4:
					if ( !con2_p[7][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:draw3_popdown(NULL,3,1);break;/*File -> File -> save data p803 */
				case 1:/*File -> report -> build p813 */
					draw3_popdown(NULL,3,1);
					//gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
					//gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
                    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
                    {
                        draw_dialog_all(DIALOG_REPORT_BUILD);
                    }
					break;

				case 2:/*File -> format -> Note  p823 */
					draw3_popdown (menu_content[OFF_ON + get_report_format_note (pp->p_config)], 3, 0);
					break;
				case 3:/*File -> user field -> content  p833*/
					//draw3_popdown (menu_content[U_CONTENT + get_report_userfield_select (pp->p_config)], 3, 0);
					draw3_popdown(NULL,3,0);
					gtk_label_set_text(GTK_LABEL(pp->data3[3]),
								get_report_userfield_content(pp->p_config,get_report_userfield_select(pp->p_config)));
					break;

				case 4:
					if ( !con2_p[8][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> scheme  p903 */
					if ( !con2_p[9][0][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 1:/*Preferences -> system -> assign key  p913 */
					if ( !con2_p[9][1][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 2:/*Preferences -> service -> Import/Export  p923 */
					if ( !con2_p[9][2][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;


				case 3:/* 933 */
					if ( !con2_p[9][3][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;

				case 4:/*  943 */
					if ( !con2_p[9][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
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
	gfloat tmpf = 0.0;
	gchar *str;

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit, content_pos, temp_beam, menu_status = 0, temp_qty =0, temp_pos = 0;
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0: /* p004 */
					if (pp->start_qty == 4)
					{
						if((g_tmp_group_struct.group_mode) == PA_SCAN)
						{
							switch (TMP(receiver_reg))
							{
								case 0:	tmpf = 1.0; break;
								case 1:	tmpf = 10.0; break;
								case 2:	tmpf = 100.0; break;
								default:break;
							}
							upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
							if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_RECEIVER));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 4;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_0044, str, cur_value , lower, upper, step, digit, p, pos, 18);
							}
							else 
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_RECEIVER));
								digit = 0;
								pos = 4;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_stop (cur_value, str, digit, pos, 18);
							}
							gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
							gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);
							g_free(str);
						}
						else
						{
							switch (TMP(receiver_reg))
							{
								case 0:	tmpf = 1.0; break;
								case 1:	tmpf = 10.0; break;
								case 2:	tmpf = 100.0; break;
								default:break;
							}
							if (get_group_val (&g_tmp_group_struct, GROUP_TX_RX_MODE) == PULSE_ECHO)
								/* 脉冲回波模式不可以调节 */
							{
								gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
								gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);
							}
							upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
							if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_RECEIVER));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 4;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_0044, str, cur_value , lower, upper, step, digit, p, pos, 18);
							}
							else 
							{
								cur_value = (gfloat) (get_group_val (&g_tmp_group_struct, GROUP_RECEIVER));
								digit = 0;
								pos = 4;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_stop (cur_value, str, digit, pos, 18);
							}
							g_free(str);
						}
					}

					else if (( pp->start_qty == 5 )&&(g_tmp_group_struct.group_mode)==1)/*PA模式时才有Auto Detect键*/
					{
						draw3_popdown (menu_content[OFF_ON + get_auto_detect (pp->p_config)], 4, 0);
					}
					else if(pp->start_qty == 7)
					{
						switch (TMP(skew_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;						
							default:break;
						}
						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
						{
							if (pp->mark_pop_change)
							{
								cur_value = GROUP_VAL(skew)/100.0 ;
								lower =	0.0;
								upper =	359.9;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_NULL;
								draw3_digit_pressed (data_00471, units[unit], cur_value , lower, upper, step, digit, p, pos, 13);
							}
							else
							{
								str = g_strdup_printf ("%.1f", (g_tmp_group_struct.skew)/100.0);
								draw3_pop_tt (data_00472, NULL, 
										str, menu_content+PROB_SKEW, 5, 4, (g_tmp_group_struct.skew_pos), 0);
								g_sprintf (temp,"%s", con2_p[0][0][13]);
								gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
								g_free(str);
							}
						}
						else 
						{
							cur_value = (g_tmp_group_struct.skew)/100.0 ;
							unit = UNIT_NULL;
							pos = 4;
							digit = 1;
							draw3_digit_stop (cur_value , units[unit], digit, pos, 13);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
					break;
				case 1:	 /* p014 */
					if(pp->fstart_qty == 3 )
					{
						switch (TMP(last_element_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						/* 聚焦法则自动计算开启时  */
						if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) &&
								(LAW_VAL(Focal_type) == LINEAR_SCAN))
						{
							/*Law Config 为 Linear 时，Last Element可调*/
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = LAW_VAL (Last_tx_elem);
								lower = LAW_VAL (First_tx_elem) + LAW_VAL (Elem_qty) - 1; 
								upper = GROUP_VAL (probe.Elem_qty);
								step = tmpf;
								digit = 0;
								pos = 4;
								unit = UNIT_NONE;
								draw3_digit_pressed (data_632, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							}
							else 
							{
								cur_value = LAW_VAL(Last_tx_elem);
								digit = 0;
								pos = 4;
								unit = UNIT_NONE;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
						}
						else /*Law Config 不为 Linear 时，Last Element  Unsensitive*/
						{
							cur_value = LAW_VAL(Last_tx_elem);
							digit = 0;
							pos = 4;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
						}
					}
					else if(pp->fstart_qty == 4)
					{
						switch (pp->p_tmp_config->angle_step_reg)
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((LAW_VAL(Focal_type) == AZIMUTHAL_SCAN) &&
								(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON))
							/* 角度扫查时开始自动计算聚焦法则时候可以调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = LAW_VAL (Angle_step) / 100.0;
								temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
								TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
										LAW_VAL(Angle_beam_skew_min)) /
									LAW_VAL(Angle_beam_skew_step) + 1;
								temp_beam = temp_beam / TMP(beam_skew_num);
								lower = MAX (((gint)(LAW_VAL (Angle_max)) - 
											(gint)(LAW_VAL (Angle_min))) 
										/ (100.0 * temp_beam), 0.1);
								upper = 89.9;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_DEG;
								draw3_digit_pressed (data_612, units[unit], cur_value,
										lower, upper, step, digit, p, pos, 12);
							}
							else 
							{
								cur_value = LAW_VAL (Angle_step) / 100.0;
								digit = 1;
								pos = 4;
								unit = UNIT_DEG;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
							}
						}
						else
						{
							cur_value = LAW_VAL (Angle_step) / 100.0;
							digit = 1;
							pos = 4;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
							gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
					break;
				case 2:/*p024*/
					if(pp->cstart_qty == 2)
					{
						if((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
						{
							switch (TMP(start_reg))
							{
								case 0:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / (gfloat)(GROUP_VAL(point_qty)); break;
								case 1:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 20.0 ; break;
								case 2:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 10.0 ; break;
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || 
										(UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围数值mm */
										lower = (BEAM_INFO(0,beam_delay) /1000.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
										upper =	(MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
										step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
										(step < 0.01) ? (step = 0.01) : (step = step);
										digit = 2;
										pos = 4;
										unit = UNIT_MM;
									}
									else
									{
										cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /*当前显示的范围inch */
										lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
										upper =	(MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
										step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
										digit = 3;
										pos = 4;
										unit = UNIT_INCH;
									}
								}
								else 
								{
									cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 ;
									lower =	BEAM_INFO(0,beam_delay) / 1000.0;
									upper =	(MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0);
									step = tmpf;
									(step < 0.01) ? (step = 0.01) : (step = step);
									pos = 4;
									digit = 2;
									unit = UNIT_US;
								}
								draw3_digit_pressed (data_101, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
							}
							else
							{
								if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
								{
									if (UNIT_MM == get_unit(pp->p_config))
									{
										cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
										unit = UNIT_MM;
										digit = 2;
										pos = 4;
									}
									else
									{
										cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
										unit = UNIT_INCH;
										digit = 3;
										pos = 4;
									}
								}
								else
								{
									cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 ;
									unit = UNIT_US;
									digit = 2;
									pos = 4;
								}
								draw3_digit_stop (cur_value , units[unit], digit, pos, 10);
							}

						}
						else if((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
						{
							switch(TMP(tolerance_reg))
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = pp->tolerance / 1000.0 ;
								lower = 0.0; 
								upper = 1000.0;
								step = tmpf;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
								draw3_digit_pressed (data_0243, units[unit], cur_value , lower, upper, step, digit, p, pos, 26);
							}
							else 
							{
								cur_value = pp->tolerance / 1000.0 ;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 26);
							}
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);
						}
					}
					else if(pp->cstart_qty == 3)
					{
						if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
						{
							if(pp->echotype_pos == 0)
							{
								switch (TMP(radius2_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;						
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
								{
									cur_value = pp->radius2 / 1000.0;
									lower = 0.0;
									upper = 1000.0;
									step = tmpf;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
									draw3_digit_pressed (data_024, units[unit], cur_value , lower, upper, step, digit, p, pos, 14);
								}
								else 
								{
									cur_value = pp->radius2 / 1000.0;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 14);
								}

							}
							else if(pp->echotype_pos == 1)
							{
								switch (TMP(depth2_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;						
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
								{
									cur_value = pp->depth2 / 1000.0;
									lower = 0.0;
									upper = 1000.0;
									step = tmpf;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
									draw3_digit_pressed (data_0241, units[unit], cur_value , lower, upper, step, digit, p, pos, 16);
								}
								else 
								{
									cur_value = pp->depth2 / 1000.0;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
								}
							}
							else
							{
								switch (TMP(thickness2_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;						
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
								{
									cur_value = pp->thickness2 / 1000.0;
									lower = 0.0;
									upper = 1000.0;
									step = tmpf;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
									draw3_digit_pressed (data_0242, units[unit], cur_value , lower, upper, step, digit, p, pos, 18);
								}
								else 
								{
									cur_value = pp->thickness2 / 1000.0;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 18);
								}
							}
						}
						else if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
						{
							switch (TMP(cheight_reg))
							{
								case 0:	tmpf = 1.0; break;
								case 1:	tmpf = 10.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = GROUP_VAL(gate[0].height);
								lower = 0.0;
								upper = 98.0;
								step = tmpf;
								digit = 0;
								pos = 4;
								unit = UNIT_BFH;
								draw3_digit_pressed (data_204, units[unit], cur_value,
										lower, upper, step, digit, p, pos, 21);
							}
							else 
							{
								cur_value =GROUP_VAL(gate[0].height);
								digit = 0;
								pos = 4;
								unit = UNIT_BFH;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 21);
							}	

						}
						else
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);
						}
					}
					else if (pp->cstart_qty == 4)
					{
						if(pp->ctype_pos == 1)
						{
							switch (TMP(cheight_reg))
							{
								case 0:	tmpf = 1.0; break;
								case 1:	tmpf = 10.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = GROUP_VAL(gate[0].height);
								lower = 0.0;
								upper = 98.0;
								step = tmpf;
								digit = 0;
								pos = 4;
								unit = UNIT_BFH;
								draw3_digit_pressed (data_204, units[unit], cur_value,
										lower, upper, step, digit, p, pos, 21);
							}
							else 
							{
								cur_value =GROUP_VAL(gate[0].height);
								digit = 0;
								pos = 4;
								unit = UNIT_BFH;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 21);
							}	
						}
						else if(pp->ctype_pos == 2)
						{
							switch (TMP(thickness_reg))
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = pp->thickness / 1000.0;
								lower = 0.0;
								upper = 1000.0;
								step = tmpf;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
								draw3_digit_pressed (data_0237, units[unit], cur_value , lower, upper, step, digit, p, pos, 39);
							}
							else 
							{
								cur_value = pp->thickness / 1000.0;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 39);
							}
						}	
						else
						{
								gtk_widget_hide (pp->eventbox30[4]);
								gtk_widget_hide (pp->eventbox31[4]);
						}			
					}
					else if (pp->cstart_qty == 5)
					{
						if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
						{
							switch (TMP(cheight_reg))
							{
								case 0:	tmpf = 1.0; break;
								case 1:	tmpf = 10.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = GROUP_VAL(gate[0].height);
								lower = 0.0;
								upper = 98.0;
								step = tmpf;
								digit = 0;
								pos = 4;
								unit = UNIT_BFH;
								draw3_digit_pressed (data_204, units[unit], cur_value,
										lower, upper, step, digit, p, pos, 21);
							}
							else 
							{
								cur_value =GROUP_VAL(gate[0].height);
								digit = 0;
								pos = 4;
								unit = UNIT_BFH;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 21);
							}
							
						}
						else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
						{
							draw3_popdown_offset(NULL, 4,1,30);
						}
						else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
						{
							switch (TMP(db_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 0.5; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 2.0; break;
								case 4:	tmpf = 6.0; break;
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0; 
								lower = 0.0 - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
								upper = GAIN_MAX - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config)) / 100.0 ;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_DB;
								draw3_digit_pressed (data_100, units[unit], cur_value ,
										lower, upper, step, digit, p, pos, 9);
							}
							else 
							{
								cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0; 
								digit = 1;
								pos = 4;
								unit = UNIT_DB;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
							}
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);							
						}
					}
					else if (pp->cstart_qty == 6)
					{
						if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
						{
							draw3_popdown_offset(NULL, 4,1,30);
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);							
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
					break;
				case 3:
					if ( !con2_p[0][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 4:
					if ( !con2_p[0][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0:/* velocity 声速 P104 TAN1 */
					switch (pp->p_tmp_config->velocity_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						if (get_unit(pp->p_config) == 0)
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 100.0;
							lower = 635.0;
							upper = 15240.0;
							step = tmpf;
							digit = 1;
							unit = UNIT_M_S;
						}
						else
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / (100.0 * 25400 );
							lower = 0.025;
							upper = 0.6;
							step = tmpf / 1000.0;
							digit = 4;
							unit = UNIT_IN_US;
						}
						pos = 4;
						draw3_digit_pressed (data_104, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if (get_unit(pp->p_config) == 0)
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 100.0;
							digit = 1;
							unit = UNIT_M_S;
						}
						else
						{
							cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) * 0.000000394;
							digit = 4;
							unit = UNIT_IN_US;
						}
						pos = 4;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 1: /* 脉冲宽度 pulser width  P114 TAN1 */
					pp->x_pos = 588, pp->y_pos = 460 - YOFFSET;
					switch (TMP(pulser_width_reg))
					{
						case 0:	tmpf = 2.5; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 25.0; break;
						default:break;
					}
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
					{
						if (pp->mark_pop_change)
						{
							cur_value = get_group_val (p_grp, GROUP_PW_VAL) / 100.0;
							lower =	30.0;
							upper =	500.0;
							step = tmpf;
							digit = 1;
							pos = 4;
							unit = UNIT_NONE;

							draw3_digit_pressed (data_1141, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							if (!get_group_val (p_grp, GROUP_PW_POS))
							{
								/* 更新当前增益值显示 */
								str = g_strdup_printf ("%s %0.1f", 
										menu_content[PULSER_WIDTH + get_group_val (p_grp, GROUP_PW_POS)], get_group_val (p_grp, GROUP_PW_VAL) / 100.0);
								draw3_pop_tt (data_114, NULL, 
										str, menu_content + PULSER_WIDTH, 2, 4, get_group_val (p_grp, GROUP_PW_POS), 0);
								g_free(str);
							}
							else
							{
								str = g_strdup_printf ("%0.1f", get_group_val (p_grp, GROUP_PW_VAL) / 100.0);
								draw3_pop_tt (data_114, NULL, 
										str, menu_content + PULSER_WIDTH, 2, 4, get_group_val (p_grp, GROUP_PW_POS), 0);
								g_free(str);
							}
						}
					}
					else 
					{
						/* 这个选中Auto 时候显示 Auto + 数值 */
						if (!get_group_val (p_grp, GROUP_PW_POS))
						{
							/* Auto 时候计算脉冲宽度 */
							str = g_strdup_printf ("%s %0.1f", 
									menu_content[PULSER_WIDTH + get_group_val (p_grp, GROUP_PW_POS)], get_group_val (p_grp, GROUP_PW_VAL) / 100.0);
							draw3_popdown (str, 4, 0);
							g_free(str);
						}
						else 
						{
							cur_value = get_group_val (p_grp, GROUP_PW_VAL) / 100.0;
							unit = UNIT_NULL;
							pos = 4;
							digit = 1;
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
					}
					break;
				case 2:/* Averaging  P124 TAN1 */
					pp->x_pos = 627, pp->y_pos = 458-YOFFSET;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_124, NULL, 
								menu_content[AVERAGING + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_AVERAGING)],
								menu_content + AVERAGING, 5, 4, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_AVERAGING), 0);
					else 
						draw3_popdown (menu_content[AVERAGING + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_AVERAGING)], 4, 0);
					break;
				case 3:/* Beam Delay 波束延时 UT无 P134 */
					if (GROUP_VAL(group_mode) == UT_SCAN)
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
					else if (GROUP_VAL(group_mode) == PA_SCAN)
					{
						switch (TMP(beam_delay_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1; break;
							case 2:	tmpf = 1.0; break;						
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = BEAM_INFO(TMP(beam_num[get_current_group(pp->p_config)]),beam_delay)/1000.0;
							lower = 0.0;
							upper = 1000.0;
							step = tmpf;
							digit = 2;
							pos = 4;
							unit = UNIT_US;
							draw3_digit_pressed (data_134, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = BEAM_INFO(TMP(beam_num[get_current_group(pp->p_config)]),beam_delay)/1000.0;
							digit = 2;
							pos = 4;
							unit = UNIT_US;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					break;
				case 4:/* Scale Factor P144 OK */
					gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
					gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);

					cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 10.0) / GROUP_VAL(point_qty);
					digit = 0;
					pos = 4;
					unit = UNIT_NULL;
					draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/* 闸门高度 或者 RF P204 TAN1 */
					pp->x_pos = 590, pp->y_pos = 470;
					if (GROUP_GATE_POS(parameters) == 0)
					{
						switch (TMP(agate_height_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;						
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = GROUP_GATE_POS(height);
							lower = 0.0;
							upper = 98.0;
							step = tmpf;
							digit = 0;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_204, units[unit], cur_value,
									lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_GATE_POS(height);
							digit = 0;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RECTIFIER) != RF_WAVE)
						{
							gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							draw3_pop_tt (data_2041, NULL, menu_content[RECTIFIER_FREQ + GROUP_GATE_POS(rectifier_freq)],menu_content + RECTIFIER_FREQ, 3, 4, GROUP_GATE_POS(rectifier_freq), 0);
							str = g_strdup_printf ("%s", con2_p[2][0][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[4]), str);
						}
						else 
						{
							draw3_popdown (menu_content[RECTIFIER_FREQ + GROUP_GATE_POS(rectifier_freq)], 4, 0);
							str = g_strdup_printf ("%s", con2_p[2][0][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[4]), str);
						}
						if (str)
							g_free(str);
					}
					//gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;
				case 1:/*Group B   p214 */
					pp->x_pos = 632, pp->y_pos = 456-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_214, NULL,menu_content[GROUPA + get_alarm_groupb(pp->p_config)],
								menu_content + GROUPA, 4, 4, get_alarm_groupb(pp->p_config), 0);
					else 
						draw3_popdown (menu_content[GROUPA + get_alarm_groupb(pp->p_config)], 4, 0);
					break;

				case 2:/* Output->delay P224 */
					if (get_output_pos(pp->p_config) < 3)
					{
						/* 当前步进 */
						switch (TMP(active_delay_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1; break;
							case 2:	tmpf = 1.0; break;
							case 3:	tmpf = 10.0; break;						
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = get_output_delay(pp->p_config) / 1000.0;
							lower = 0.0;
							upper = 5000.0;
							step = tmpf;
							digit = 2;
							pos = 4;
							unit = UNIT_MS;
							draw3_digit_pressed (data_224, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = get_output_delay(pp->p_config) / 1000.0;
							digit = 2;
							pos = 4;
							unit = UNIT_MS;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
					break;

				case 3:/*Gate/Alarm -> Sizing Curves -> Curve Step  p234 */
					if(GROUP_VAL(mode_pos)==0)
					{
						if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2)
						{
							/* 当前步进 */
							switch (TMP(curve_step_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 0.5; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 2.0; break;
								case 4:	tmpf = 6.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value =GROUP_VAL(curve_step)/100.0;
								lower = 0.0;
								upper = 12.0;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_DB;
								draw3_digit_pressed (data_234, units[unit], cur_value , lower, upper, step, digit, p, pos, 8);


							}
							else 
							{
								cur_value = GROUP_VAL(curve_step)/100.0;
								digit = 1;
								pos = 4;
								unit = UNIT_DB;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
							} 

						}
						else
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);
							gtk_widget_hide (pp->sbutton[4]);

						}
					}
					else if(GROUP_VAL(mode_pos)==1)
					{
						if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==3)
						{
							draw3_popdown(NULL,4,1);
							str = g_strdup_printf ("%s", con2_p[2][3][13]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[4]), str);
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
								             data_234_add_point(p);
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);
							gtk_widget_hide (pp->sbutton[4]);
						}
					}
					break;

				case 4:
					if ( !con2_p[2][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Field 3 P304 */
					pp->x_pos = 200, pp->y_pos = 0;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_304, NULL, 
								field[get_reading_field3(pp->p_config)],
								field1, 60, 4, get_reading_field3(pp->p_config), 0);
					else 
						draw3_popdown (field[get_reading_field3(pp->p_config)], 4, 0);

					break;

				case 1:/*Measurements -> Cursors -> Add Entry p314 */
					if(GROUP_VAL(selection)==0 || GROUP_VAL(selection)==1)
					{
						/* 当前步进 */
						switch (TMP(u_measure_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 10.0;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							if((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									step = tmpf;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
									step = tmpf;
									digit = 2;
									pos = 4;
									unit = UNIT_INCH;
								}
							}
							else	/*us*/
							{
								cur_value = GROUP_VAL(u_measure)/1000.0;
								lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0;
								upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0;
								step = tmpf;
								digit = 2;
								pos = 4;
								unit = UNIT_US;
							}
							draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							if((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(get_unit(pp->p_config) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
									digit = 3;
									pos = 4;
									unit = UNIT_INCH;
								}
							}
							else	/*????*/
							{
								cur_value = GROUP_VAL(u_measure)/1000.0;
								digit = 2;
								pos = 4;
								unit = UNIT_US;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else if( GROUP_VAL(selection)==2)
					{
						/* 当前步进 */
						switch (TMP(i_measure_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 0.5;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 1.5;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_measure)/100.0;
								lower = -999999.0;
								upper = 999999.0;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_measure)/100.0*0.03937;
								lower = -999999.0*0.03937;
								upper = 999999.0*0.03937;
								step = tmpf;
								digit = 3;
								pos = 4;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3141, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_measure)/100.0;
								digit = 1;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_measure)/100.0*0.03937;
								digit = 3;
								pos = 4;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
						}
					}
					else if( GROUP_VAL(selection)==3)
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
					else if (GROUP_VAL(selection)==4)
					{
						/* 当前步进 */
						switch (TMP(i_reference_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 0.5;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 1.5;  break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_reference)/100.0;
								lower = -999999.0;
								upper = 999999.0;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_reference)/100.0*0.03937;
								lower = -999999.0*0.03937;
								upper = 999999.0*0.03937;
								step = tmpf;
								digit = 3;
								pos = 4;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3131, units[unit], cur_value , lower, upper, step, digit, p, pos, 9);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_reference)/100.0;
								digit = 1;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_reference)/100.0*0.03937;
								digit = 3;
								pos = 4;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
						}
					}
					else if (GROUP_VAL(selection)==5||GROUP_VAL(selection)==6||GROUP_VAL(selection)==7)
					{
						/* 当前步进 */
						switch (TMP(cursors_scan_reg))
						{
							case 0:	tmpf = 0.5; break;
							case 1:	tmpf = 2.5; break;
							case 2:	tmpf = 5.0; break;
							case 3:	tmpf = 7.5; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0;
								lower = -100000.0;
								upper = 100000;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0*0.03937;
								lower = -100000.0*0.03937;
								upper = 100000*0.03937;
								step = tmpf;
								digit = 3;
								pos = 4;
								unit = UNIT_MM;
							}
							draw3_digit_pressed (data_3142, units[unit], cur_value , lower, upper, step, digit, p, pos, 15);
						}
						else
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0;
								digit = 1;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(scan)/100.0*0.03937;
								digit = 3;
								pos = 4;
								unit = UNIT_MM;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
						}
					}
					else if (GROUP_VAL(selection)==8)
					{
						draw3_popdown(NULL,4,1);
						g_sprintf (temp,"%s", con2_p[3][1][17]);
						gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					}
					gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;
				case 2:/* Measurements -> Table -> Select Entry  P324*/
					/* 当前步进 */
					switch (pp->p_tmp_config->entry_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						cur_value = get_cur_entry (pp->p_config);
						lower = 1.0;
						upper = get_entry_qty (pp->p_config);
						step = tmpf;
						digit = 0;
						pos = 4;
						unit = UNIT_NONE;
						draw3_digit_pressed (data_324, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = get_cur_entry (pp->p_config);
						digit = 0;
						pos = 4;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 3:
					if ( !con2_p[3][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 4:
					if ( !con2_p[3][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/* p404 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))/*选中404这个位置*/
					{
						if(get_display_pos(pp->p_config)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							/* 当前步进 */
							switch (TMP(dis_range_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 1.0; break;
								case 2:	tmpf = 10.0; break;
								default:break;
							}

							cur_value = get_stripscan_disrange(pp->p_config)/100.0;
							lower = 0.51;
							upper = 46.0;
							step = tmpf;
							digit = 2;
							pos = 4;
							unit = UNIT_S;
							draw3_digit_pressed (data_404, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);							

						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);
						}

					}
					else 
					{
						if(get_display_pos(pp->p_config)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							cur_value = get_stripscan_disrange(pp->p_config)/100.0;
							digit = 2;
							pos = 4;
							unit = UNIT_S;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
						}

						else 
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);
						}
					}
					break;


				case 1:/*Display -> Overlay -> cursor  p414 */
					draw3_popdown(menu_content[OFF_ON + get_overlay_cursor (pp->p_config)],4,0);
					break;

				case 2:/*Display -> color -> Mode  p424 */
					pp->x_pos = 560, pp->y_pos = 500;
					if (GROUP_VAL(col_select_pos) == 2)
					{

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							draw3_pop_tt (data_434, NULL, 
									menu_content[COL_MODE+GROUP_VAL(col_mode)],
									menu_content+COL_MODE, 2, 4, GROUP_VAL(col_mode), 0);
						else 
							draw3_popdown (menu_content[COL_MODE+GROUP_VAL(col_mode)], 4, 0);
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
						//gtk_widget_hide (pp->data3[4]);
					}

					break;


				case 3:/*Display -> properties -> appearence  p434 */
					pp->x_pos = 536, pp->y_pos = 455-YOFFSET;
					switch(get_dis_prop_scan(pp->p_config))
					{
						case 0:	/* Ascan 时候的 appearance */
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
								draw3_pop_tt (data_444, NULL, 
										menu_content[PROP_APP + GROUP_VAL(ascan_appearance)],
										menu_content+PROP_APP, 4, 4, GROUP_VAL(ascan_appearance), 0);
							else 
								draw3_popdown (menu_content[PROP_APP + GROUP_VAL(ascan_appearance)], 4, 0);
							break;
						case 1:
							gtk_widget_hide(pp->eventbox30[4]);
							gtk_widget_hide(pp->eventbox31[4]);
							break;
						case 2:
							gtk_widget_hide(pp->eventbox30[4]);
							gtk_widget_hide(pp->eventbox31[4]);
							break;
						case 3:
							gtk_widget_hide(pp->eventbox30[4]);
							gtk_widget_hide(pp->eventbox31[4]);
							break;
						case 4:
							gtk_widget_hide(pp->eventbox30[4]);
							gtk_widget_hide(pp->eventbox31[4]);
							break;
						case 5:
							gtk_widget_hide(pp->eventbox30[4]);
							gtk_widget_hide(pp->eventbox31[4]);
							break;
						default:break;

					}
					break;
				case 4:
					if ( !con2_p[4][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;

				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/* Probe/Part -> Select -> Wedge P504*/
					if (get_probe_select (pp->p_config)==0)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							draw_dialog_all (DIALOG_WEDGE);
						else
							draw3_popdown(GROUP_VAL(wedge.Model), 4, 0);

						g_sprintf (temp,"%s", con2_p[5][0][4]);
						gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					}
					else if(get_probe_select (pp->p_config)==1)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							draw_dialog_all (DIALOG_DEFINE_WEDGE);
						}
						else
						{
							draw3_popdown(GROUP_VAL(wedge.Model), 4, 0);
						}

						g_sprintf (temp,"%s", con2_p[5][0][7]);
						gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					}
					break;

				case 1:/* p514 */
					if ( !con2_p[5][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 2:/*Probe/Part -> characterize -> procedure  p524 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Undefined");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);

					gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
					break;
				case 3:
					if ( !con2_p[5][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 4:
					if ( !con2_p[5][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:
					if ( !con2_p[6][0][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 1: /* beam skew angle max 2D时候可以用 P614 */
#if 0
					pp->x_pos = 544, pp->y_pos = 456-YOFFSET;
					if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
						/* 聚焦法则自动计算开启时, 纵横才可以调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							draw3_pop_tt (data_614, NULL, 
									menu_content[W_TYPE + LAW_VAL(Wave_type)],
									menu_content+WAVE_TYPE, 3, 4, LAW_VAL(Wave_type), 0);
						else 
							draw3_popdown (menu_content[W_TYPE + LAW_VAL(Wave_type)], 4, 0);
					}
					else /* 聚焦法则自动计算为OFF, Min.Angle 不可调节 */
					{
						draw3_popdown (menu_content[W_TYPE + LAW_VAL(Wave_type)], 4, 0);
						gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
					}
					break;
#endif
					switch (TMP(beam_skew_max_angle_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((LAW_VAL(Focal_type) == AZIMUTHAL_SCAN) &&
							(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) 
							&& (GROUP_VAL(probe.PA_probe_type) == 9)
							/* 如何判断2D探头 */
					   )
						/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							/* 最大不能超过最大Angle_beam_skew_max */
							cur_value = LAW_VAL (Angle_beam_skew_max) / 100.0;
							/* 计算lower为妙 */
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
							TMP(angle_num)	= (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) /
								LAW_VAL(Angle_step) + 1;
							temp_beam = temp_beam / TMP(angle_num);
							lower = LAW_VAL (Angle_beam_skew_min) / 100.0;
							upper = MIN (((gint)(LAW_VAL (Angle_beam_skew_min)) + 
										(gint)(temp_beam * LAW_VAL(Angle_beam_skew_step))) / 100.0, 89.9);
							step = tmpf;
							digit = 1;
							pos = 4;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_614, units[unit], cur_value , lower, 
									upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL (Angle_beam_skew_max) / 100.0;
							digit = 1;
							pos = 4;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /* 聚焦法则自动计算为OFF, Max.Angle 不可调节 */
					{
						cur_value = LAW_VAL (Angle_beam_skew_min) / 100.0;
						digit = 1;
						pos = 4;
						unit = UNIT_DEG;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
					}
					break;
				case 2:	/* P624 */
					switch (TMP(depthe_reg))
					{
						case 0:	tmpf = 0.5; break;
						case 1:	tmpf = 5.0; break;
						case 2:	tmpf = 50.0; break;
						default:break;
					}
					if((LAW_VAL(Focal_point_type)==HALFPATH_P)||(LAW_VAL(Focal_point_type)==DEPTH_P))
					{
						gtk_widget_hide(pp->eventbox30[4]);
						gtk_widget_hide(pp->eventbox31[4]);
					}
					else
					{
						if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)&&(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
						{
							if((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = LAW_VAL(Depth_end) / 1000.0;
								lower = 0.1;
								upper = 10000.0;
								step = tmpf;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;

								draw3_digit_pressed (data_624, units[unit], cur_value , lower, upper,
										step, digit, p, pos, 0);
							}
							else
							{
								cur_value = LAW_VAL(Depth_end) / 1000.0;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
						}
						else
						{
							cur_value = LAW_VAL(Depth_end) / 1000.0;
							digit = 2;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
						}

					}
					break;
				case 3:	/* P634 */
					pp->x_pos = 555, pp->y_pos = 400;
					g_sprintf (TMP (velocity_data[0]), "LW:%dm/s", get_material_lw(pp->p_config) / 100);
					g_sprintf (TMP (velocity_data[1]), "SW:%dm/s", get_material_sw(pp->p_config) / 100);
					g_sprintf (TMP (velocity_data[2]), "%d m/s", get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 100);
					gchar **tp = TMP(velocity_data_p);
					if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) == get_material_lw(pp->p_config)) 
					{
						temp_pos = 0;
						temp_qty = 2;
					}
					else if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) == get_material_sw(pp->p_config))
					{
						temp_pos = 1;
						temp_qty = 2;
					}
					else
					{
						temp_pos = 2;
						temp_qty = 3;
					}

					if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{

							if (get_material_sw(pp->p_config) == 0)
								menu_status = 0x02;

							draw3_pop_tt (data_634, NULL, 
									tp[temp_pos],	(const gchar **)tp,	temp_qty, 4, temp_pos, menu_status);
						}
						else 
						{
							draw3_popdown (TMP(velocity_data[temp_pos]), 4, 0);
						}
					}
					else
					{
						draw3_popdown ( TMP(velocity_data[temp_pos]), 4, 0);
						gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
					}
					break;
				case 4:
					if ( !con2_p[6][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encodr -> Origin  p704 */
					/* 当前步进 */
					switch (TMP(origin_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						cur_value = get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
						lower = -1000;
						upper = 1000.00;
						step = tmpf;
						digit = 2;
						pos = 4;
						unit = UNIT_MM;
						draw3_digit_pressed (data_704, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
						digit = 2;
						pos = 4;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
					{
						gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
					}
					break;

				case 1: /* Max.Scan Speed p714 */
					if(get_inspec_type (pp->p_config)==2)
					{
						/* 当前步进 */
						switch (TMP(scanspeed_rpm_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}

						if(get_inspec_source (pp->p_config)==0)
							content_pos=0;
						else
							content_pos=6;

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = get_inspec_rpmspeed (pp->p_config)/10.0;
							lower = 0.0;
							upper = 1000.0;
							step = tmpf;
							digit = 1;
							pos = 4;
							unit = UNIT_RPM;
							draw3_digit_pressed (data_714, units[unit], cur_value , lower, upper, step, digit, p, pos, content_pos);
						}
						else 
						{
							cur_value = get_inspec_rpmspeed (pp->p_config)/10.0;
							digit = 1;
							pos = 4;
							unit = UNIT_RPM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
					break;

				case 2:/*Scan -> area -> index end  p724 */
					/* 当前步进 */
					switch (pp->p_tmp_config->index_end_reg)
					{
						case 0:	tmpf = 0.001 * get_area_indexresolution (pp->p_config); break;
						case 1:	tmpf = 0.01 * get_area_indexresolution (pp->p_config); break;
						case 2:	tmpf = 0.1 * get_area_indexresolution (pp->p_config); break;
						default:break;
					}

					if(get_inspec_type (pp->p_config)==1 || get_inspec_type (pp->p_config)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							if( get_unit(pp->p_config) == UNIT_MM )
							{
								cur_value = get_area_indexend (pp->p_config)/1000.0;
								lower = get_area_indexstart (pp->p_config)/1000.0;
								upper = 100000.0;
								step = tmpf;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_indexend (pp->p_config)/1000.0*0.03937;
								lower = get_area_indexstart (pp->p_config)/1000.0*0.03937;
								upper = 100000.0*0.03937;
								step = tmpf*0.03937;
								digit = 3;
								pos = 4;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_724, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							if( get_unit(pp->p_config) == UNIT_MM )
							{
								cur_value = get_area_indexend (pp->p_config)/1000.0;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_indexend (pp->p_config)/1000.0*0.03937;
								digit = 3;
								pos = 4;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}

					else
						/* Inspection -> Type 选择 One-Line Scan 时 */
					{
						if( get_unit(pp->p_config) == UNIT_MM )
						{
							cur_value = get_area_indexend (pp->p_config);
							digit = 2;
							pos = 4;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_area_indexend (pp->p_config)/1000.0*0.03937;
							digit = 3;
							pos = 4;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

						gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);
					}
					break;

				case 3:
					if ( !con2_p[7][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 4:
					if ( !con2_p[7][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/*File -> File -> save mode  p804 */
					pp->x_pos = 540, pp->y_pos = 456-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_804, NULL, 
								menu_content[SAVE_MODE+get_file_save_mode (pp->p_config)],
								menu_content+SAVE_MODE, 4, 4, get_file_save_mode (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[SAVE_MODE+get_file_save_mode (pp->p_config)], 4, 0);
					break;
				case 1:
					if ( !con2_p[8][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 2:/*File -> format -> view  p824 */
					pp->x_pos = 547, pp->y_pos = 456-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_824, NULL, 
								menu_content[VIEW+get_report_format_view (pp->p_config)],
								menu_content+VIEW, 3, 4, get_report_format_view (pp->p_config), 0);
					else 
						draw3_popdown (menu_content[VIEW+get_report_format_view (pp->p_config)], 4, 0);

					break;

				case 3:
					if ( !con2_p[8][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 4:
					if ( !con2_p[8][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*preferences -> pref. -> gate mode   p904 */
					if ( !con2_p[9][0][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 1:
					if ( !con2_p[9][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 2:/*  924*/
					if ( !con2_p[9][2][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);

					break;

				case 3:
					if ( !con2_p[9][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 4:/*Preferences -> Network -> Remote PC*/
					if ( !con2_p[9][4][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);

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
	gfloat tmpf = 0.0, tmpfm;
	gchar *str;
	guint menu_status  = 0;

	gfloat max_tmp = 0.0,  max_tmp1 = 0.0, cur_value, lower, upper, step;
	guint digit, pos, unit, content_pos, temp_beam;
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:
					if ( !con2_p[0][0][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 1: /* p015 */
					if(pp->fstart_qty == 3)
					{
						switch (pp->p_tmp_config->element_step_reg)
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) &&
								(LAW_VAL(Focal_type) == LINEAR_SCAN))
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
							{
								cur_value = LAW_VAL(Elem_step);
								lower = 1.0;
								upper = GROUP_VAL (probe.Elem_qty) - LAW_VAL (Last_tx_elem);
								step = tmpf;
								digit = 1;
								pos = 5;
								unit = UNIT_NONE;
								draw3_digit_pressed (data_633, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							}
							else 
							{
								cur_value = LAW_VAL(Elem_step);
								digit = 1;
								pos = 5;
								unit = UNIT_NONE;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
						}
						else /*Law Config 不为 Linear 时，Element Step  Unsensitive*/
						{
							cur_value = LAW_VAL(Elem_step);
							digit = 1;
							pos = 5;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
							gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[5]);
						gtk_widget_hide (pp->eventbox31[5]);
					}
					break;
				case 2:/* Wizard -> Calibration -> clear calib p025*/
					switch(pp->cstart_qty)
					{
						case 0:
						case 1:
							if ((pp->ctype_pos == 1)&&(pp->cmode_pos == 2))
							{
								draw3_popdown(NULL, 5,1);
							}
							else if ((pp->ctype_pos == 1)&&(pp->cmode_pos == 3))
							{
								draw3_popdown_offset(NULL, 5,1,7);
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[5]);
								gtk_widget_hide (pp->eventbox31[5]);
							}
							break;
						case 2:
							if((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								switch (TMP(range_reg))
								{
									case 0:	tmpf = GROUP_VAL(point_qty) / 100.0; break;
									case 1:	tmpf = GROUP_VAL(point_qty) / 20.0; break;
									case 2:	tmpf = GROUP_VAL(point_qty) / 10.0; break;
									default:break;
								}
								tmpfm = GROUP_VAL(point_qty) / 100.0;
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
								{
									max_tmp = (MAX_RANGE_US - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0);
									max_tmp1 = GROUP_VAL(point_qty) * 20.0;
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											lower = (GROUP_VAL(point_qty) / 100.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper = MIN(max_tmp, max_tmp1) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											tmpfm = tmpfm * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											digit = 2;
											pos = 5;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											lower =	(GROUP_VAL(point_qty) / 100.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											upper = MIN(max_tmp, max_tmp1) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
											step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											tmpfm = tmpfm * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
											digit = 3;
											pos = 5;
											unit = UNIT_INCH;
										}
									}
									else 
									{
										cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
										lower =	GROUP_VAL(point_qty) / 100.0;
										upper = MIN(max_tmp, max_tmp1);										
										step = tmpf;
										tmpfm = tmpfm;
										digit = 2;
										pos = 5;
										unit = UNIT_US;
									}
									draw3_digit_pressed (data_102, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);
								}
								else
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == get_unit(pp->p_config))
										{
											cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 5;
										}
										else
										{
											cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 5;
										}
									}
									else
									{
										cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
										unit = UNIT_US;
										pos = 5;
										digit = 2;
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 11);
								}
/*								TMP(range_step_min) = ((gint)(tmpfm * 1000)+ 5) / 10 * 10;*/
							}

							else if ((pp->ctype_pos == 1)&&(pp->cmode_pos == 2))
							{
								draw3_popdown(NULL, 5,1);
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[5]);
								gtk_widget_hide (pp->eventbox31[5]);
							}
							break;
						case 3:
							gtk_widget_hide (pp->eventbox30[5]);
							gtk_widget_hide (pp->eventbox31[5]);
							break;
						case 4:
							if (pp->ctype_pos == 0)
							{
								draw3_popdown_offset(NULL, 5,1,31);
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{
								if(pp->echotype_pos == 0)
								{
									switch (TMP(radius1_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;						
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
									{
										cur_value = pp->radius1 / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0232, units[unit], cur_value , lower, upper, step, digit, p, pos, 13);
									}
									else 
									{
										cur_value = pp->radius1 / 1000.0;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 13);
									}

								}
								else if(pp->echotype_pos == 1)
								{
									draw3_popdown_offset(NULL, 5, 1, 29);
								}
								else
								{
									switch (TMP(thickness1_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;						
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
									{
										cur_value = pp->thickness1 / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0234, units[unit], cur_value , lower, upper, step, digit, p, pos, 17);
									}
									else 
									{
										cur_value = pp->thickness1 / 1000.0;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 17);
									}
								}
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{
								if(pp->echotype_pos == 0)
								{
									switch (TMP(radiusa_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;						
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
									{
										cur_value = pp->radiusa / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0235, units[unit], cur_value , lower, upper, step, digit, p, pos, 23);
									}
									else 
									{
										cur_value = pp->radiusa / 1000.0;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 23);
									}
								}
								else if(pp->echotype_pos == 1)
								{
									switch (TMP(deptha_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;						
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
									{
										cur_value = pp->deptha / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0236, units[unit], cur_value , lower, upper, step, digit, p, pos, 24);
									}
									else 
									{
										cur_value = pp->deptha / 1000.0;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 24);
									}
								}
								else
								{
#if 0
									switch (TMP(thicknessa_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;						
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
									{
										cur_value = pp->thicknessa / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0237, units[unit], cur_value , lower, upper, step, digit, p, pos, 25);
									}
									else 
									{
										cur_value = pp->thicknessa / 1000.0;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 25);
									}
#endif
									switch (TMP(thickness1_reg))
									{
										case 0:	tmpf = 0.01; break;
										case 1:	tmpf = 0.1; break;
										case 2:	tmpf = 1.0; break;
										case 3:	tmpf = 10.0; break;						
										default:break;
									}
									if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
									{
										cur_value = pp->thickness1 / 1000.0;
										lower = 0.0;
										upper = 1000.0;
										step = tmpf;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_pressed (data_0234, units[unit], cur_value , lower, upper, step, digit, p, pos, 25);
									}
									else 
									{
										cur_value = pp->thickness1 / 1000.0;
										digit = 2;
										pos = 5;
										unit = UNIT_MM;
										draw3_digit_stop (cur_value, units[unit], digit, pos, 25);
									}

								}
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[5]);
								gtk_widget_hide (pp->eventbox31[5]);
							}
							break;

						case 5:
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
							{

							if(pp->echotype_pos == 0)
							{
								switch (TMP(radius2_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;						
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
								{
									cur_value = pp->radius2 / 1000.0;
									lower = 0.0;
									upper = 1000.0;
									step = tmpf;
									digit = 2;
									pos = 5;
									unit = UNIT_MM;
									draw3_digit_pressed (data_024, units[unit], cur_value , lower, upper, step, digit, p, pos, 14);
								}
								else 
								{
									cur_value = pp->radius2 / 1000.0;
									digit = 2;
									pos = 5;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 14);
								}

							}
							else if(pp->echotype_pos == 1)
							{
								switch (TMP(depth2_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;						
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
								{
									cur_value = pp->depth2 / 1000.0;
									lower = 0.0;
									upper = 1000.0;
									step = tmpf;
									digit = 2;
									pos = 5;
									unit = UNIT_MM;
									draw3_digit_pressed (data_0241, units[unit], cur_value , lower, upper, step, digit, p, pos, 16);
								}
								else 
								{
									cur_value = pp->depth2 / 1000.0;
									digit = 2;
									pos = 5;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
								}
							}
							else
							{
								switch (TMP(thickness2_reg))
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:	tmpf = 10.0; break;						
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
								{
									cur_value = pp->thickness2 / 1000.0;
									lower = 0.0;
									upper = 1000.0;
									step = tmpf;
									digit = 2;
									pos = 5;
									unit = UNIT_MM;
									draw3_digit_pressed (data_0242, units[unit], cur_value , lower, upper, step, digit, p, pos, 18);
								}
								else 
								{
									cur_value = pp->thickness2 / 1000.0;
									digit = 2;
									pos = 5;
									unit = UNIT_MM;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 18);
								}
							}

							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{
								draw3_popdown_offset(NULL, 5,1,31);
							}
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
							{
								draw3_popdown_offset(NULL, 5,1,29);
							}
							else if (pp->ctype_pos == 2)
							{
								draw3_popdown_offset(NULL, 5,1,31);
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[5]);
								gtk_widget_hide (pp->eventbox31[5]);
							}

# if 0
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{							
								switch (TMP(last_angle_reg))
								{
									case 0:	tmpf = 0.1; break;
									case 1:	tmpf = 1.0; break;
									case 2:	tmpf = 10.0; break;
									default:break;
								}
								if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 5))
								{
									cur_value = pp->last_angle/100.0 ;
									lower =	0.0;
									upper =	89.9;
									step = tmpf;
									digit = 1;
									pos = 5;
									unit = UNIT_DEG;
									draw3_digit_pressed (data_0238, units[unit], cur_value,
											lower, upper, step, digit, p, pos, 28);
								}
								else 
								{
									cur_value = pp->last_angle/100.0 ;
									unit = UNIT_DEG;
									pos = 5;
									digit = 1;
									draw3_digit_stop (cur_value , units[unit], digit, pos, 28);
								}
							}
#endif
							break;
						case 6:
							if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 0)||(pp->cmode_pos == 2)))
							{
								draw3_popdown_offset(NULL, 5,1,31);
							}
							else
							{
								gtk_widget_hide (pp->eventbox30[5]);
								gtk_widget_hide (pp->eventbox31[5]);
							}
							break;

						default:
							break;
					}
					break;
				case 3:
					if ( !con2_p[0][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:
					if ( !con2_p[0][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* NULL 空 P105*/
					if ( !con2_p[1][0][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;

				case 1: /* 重复频率 PRF P115 TAN1  */
					pp->x_pos = 578, pp->y_pos = 533-YOFFSET;
					switch (TMP(prf_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 5))
					{
						if (pp->mark_pop_change)
						{
							cur_value = get_prf() / 10.0;
							lower =	1.0;
							upper = get_max_prf() / 10.0;
							step = tmpf;
							digit = 0;
							pos = 5;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_1151, units[unit], cur_value,
									lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							if (GROUP_VAL(prf_pos) == 3)	/* 自定义数值时候按下显示数值 */
							{
								/* 更新当前增益值显示 */
								str = g_strdup_printf ("%d", get_prf() / 10);
								draw3_pop_tt (data_115, NULL, 
										str, menu_content + PRF, 4, 5, GROUP_VAL(prf_pos), 0);
								g_free(str);
							}
							else	/* Auto ==时候 显示当前选项 */
								draw3_pop_tt (data_115, NULL, menu_content[PRF + GROUP_VAL(prf_pos)],
										menu_content + PRF, 4, 5, GROUP_VAL(prf_pos), 0);
						}
					}
					else 
					{
						cur_value = GROUP_VAL(prf) / 10.0;
						unit = UNIT_NULL;
						pos = 5;
						digit = 0;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 2:/* Reject 抑制 P125 TAN1 */
					switch (TMP(reject_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						cur_value = get_reject(pp->p_config);
						lower = 0.0;
						upper = 100.0;
						step = tmpf;
						digit = 0;
						pos = 5;
						unit = UNIT_BFH;
						draw3_digit_pressed (data_125, units[unit], cur_value , lower,
								upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = get_reject(pp->p_config);
						digit = 0;
						pos = 5;
						unit = UNIT_BFH;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;
				case 3:/* Gain Offset UT 无 P135 */
					if (GROUP_VAL(group_mode) == UT_SCAN)
					{
						gtk_widget_hide (pp->eventbox30[5]);
						gtk_widget_hide (pp->eventbox31[5]);
					}
					else if (GROUP_VAL(group_mode) == PA_SCAN) 
					{
						switch (TMP(gain_offset_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 0.5; break;
							case 2:	tmpf = 1.0; break; 
							case 3:	tmpf = 2.0; break;
							case 4:	tmpf = 6.0; break;                                              				
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							cur_value = GROUP_VAL(gain_offset)/10.0;
							lower = 1.0;
							upper = 80.0;
							step = tmpf;
							digit = 1;
							pos = 5;
							unit = UNIT_DB;
							draw3_digit_pressed (data_135, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(gain_offset)/10.0;
							digit = 1;
							pos = 5;
							unit = UNIT_DB;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					break;
				case 4:/* Sum Gain  P145 */
					pp->x_pos = 587, pp->y_pos = 543-YOFFSET;
					switch (TMP(sum_gain_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;						
						default:break;
					}
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 5))
					{
						if (pp->mark_pop_change)
						{
							cur_value = GROUP_VAL(sum_gain)/100.0 ;
							lower =	0.0;
							upper =	48.0;
							step = tmpf;
							digit = 1;
							pos = 5;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_1451, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							if (!GROUP_VAL(sum_gain_pos))
							{
								/* 更新当前增益值显示 */
								str = g_strdup_printf ("%s %0.1f", 
										menu_content[SUM_GAIN + GROUP_VAL(sum_gain_pos)], GROUP_VAL(sum_gain) / 100.0);
								draw3_pop_tt (data_145, NULL, 
										str, menu_content + SUM_GAIN, 2, 5, GROUP_VAL(sum_gain_pos), 0);
								g_free(str);
							}
							else
							{
								str = g_strdup_printf ("%0.1f", GROUP_VAL(sum_gain) / 100.0);
								draw3_pop_tt (data_145, NULL, 
										str, menu_content + SUM_GAIN, 2, 5, GROUP_VAL(sum_gain_pos), 0);
								g_free(str);
							}
						}
					}
					else 
					{
						if (!GROUP_VAL(sum_gain_pos))
						{
							str = g_strdup_printf ("%s %0.1f", 
									menu_content[SUM_GAIN + GROUP_VAL(sum_gain_pos)], GROUP_VAL(sum_gain) / 100.0);
							draw3_popdown (str, 5, 0);
							g_free(str);
						}
						else 
						{
							cur_value = GROUP_VAL(sum_gain) / 100.0;
							unit = UNIT_NULL;
							pos = 5;
							digit = 1;
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
					}
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/*NULL*/
					if ( !con2_p[2][0][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;

				case 1:/*Condition   p215 */
					pp->x_pos = 532, pp->y_pos = 395-YOFFSET;
					/* 当groupa与groupb的值相同时，condition互相制约 */
					if ( get_alarm_groupa(pp->p_config) == get_alarm_groupb(pp->p_config) )
					{
						/* 当 groupa 的值不为 None 时*/
						if (get_alarm_conditiona (pp->p_config))
						{
							menu_status = 1<<(get_alarm_conditiona(pp->p_config));
						}
						/* 当groupa与groupb的值相同,且condition a 与 condition b 的值相同时 */
						if ((get_alarm_conditiona(pp->p_config)==get_alarm_conditionb(pp->p_config)) &&
								get_alarm_conditiona(pp->p_config))
						{
							menu_status = 1<<(get_alarm_conditiona(pp->p_config));
							set_alarm_conditionb(pp->p_config, ALARM_NONE);  /* conditionb 变为 None */
							gtk_label_set_text (GTK_LABEL (pp->data3[5]), menu_content[CONDITIONA +
									get_alarm_conditionb(pp->p_config)]);
						}
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						draw3_pop_tt (data_215, NULL,menu_content[CONDITIONA + get_alarm_conditionb(pp->p_config)],
								menu_content + CONDITIONA, 9, 5, get_alarm_conditionb(pp->p_config), menu_status);
					else 
						draw3_popdown (menu_content[CONDITIONA + get_alarm_conditionb(pp->p_config)], 5, 0);

					if (get_alarm_conditiona(pp->p_config) == 0)/*conditiona为None时，conditionb不可用*/
					{
						gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					}
					break;

				case 2:/* Hold Time P225 */
					if (get_output_pos(pp->p_config) < 3)
					{
						/* 当前步进 */
						switch (TMP(holdtime_reg))
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf = 0.1; break;
							case 2:	tmpf = 1.0; break;
							case 3:	tmpf = 10.0; break;						
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							cur_value = get_output_holdtime(pp->p_config)/1000.0;
							lower = 0.0;
							upper = 5000.0;
							step = tmpf;
							digit = 2;
							pos = 5;
							unit = UNIT_MS;
							draw3_digit_pressed (data_225, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = get_output_holdtime(pp->p_config)/1000.0;
							digit = 2;
							pos = 5;
							unit = UNIT_MS;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[5]);
						gtk_widget_hide (pp->eventbox31[5]);
					}
					break;

				case 3:/* p235 */
					if(GROUP_VAL(mode_pos)==0)
					{
						if( GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 )
						{
							/* 当前步进 */
							switch (TMP(ref_gain_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 0.5; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 2.0; break;
								case 4:	tmpf = 6.0; break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
							{
								cur_value =GROUP_VAL(ref_gain)/100.0;
								lower = 0.0;
								upper = 74.0;
								step = tmpf;
								digit = 1;
								pos = 5;
								unit = UNIT_DB;
								draw3_digit_pressed (data_235, units[unit], cur_value , lower, upper, step, digit, p, pos, 9);
							}
							else 
							{
								cur_value = GROUP_VAL(ref_gain)/100.0;
								digit = 1;
								pos = 5;
								unit = UNIT_DB;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
							}
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[5]);
							gtk_widget_hide (pp->eventbox31[5]);
							gtk_widget_hide (pp->sbutton[5]);
						}
					}
					else if(GROUP_VAL(mode_pos)==1)
					{
						if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==3)
						{
							draw3_popdown(NULL,5,1);
							str = g_strdup_printf ("%s", con2_p[2][3][14]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[5]), str);
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
							data_235_del_point(p);
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[5]);
							gtk_widget_hide (pp->eventbox31[5]);
							gtk_widget_hide (pp->sbutton[5]);

						}
					}
					break;

				case 4:
					if ( !con2_p[2][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Field 4 p305 */
					pp->x_pos = 200, pp->y_pos = 0;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						draw3_pop_tt (data_305, NULL, 
								field[get_reading_field4(pp->p_config)],
								field1, 60, 5, get_reading_field4(pp->p_config), 0);
					else 
						draw3_popdown (field[get_reading_field4(pp->p_config)], 5, 0);

					break;

				case 1:/* p315 */
					if( GROUP_VAL(selection)==0||GROUP_VAL(selection)==8)
					{
						gtk_widget_hide (pp->eventbox30[5]);
						gtk_widget_hide (pp->eventbox31[5]);
					}
					else if(GROUP_VAL(selection)==1 || GROUP_VAL(selection)==2 || GROUP_VAL(selection)==3 )
					{
						pp->x_pos = 553, pp->y_pos = 555;
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							draw3_pop_tt(data_315, NULL, 
									menu_content[DATA_LINK + GROUP_VAL(data_link)],
									menu_content + DATA_LINK, 4, 5, GROUP_VAL(data_link), 0);
						}
						else 
							draw3_popdown (menu_content[DATA_LINK+GROUP_VAL(data_link)], 5, 0);
						str = g_strdup_printf ("%s", con2_p[3][1][8]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[5]), str);

						gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					}
					else if(GROUP_VAL(selection)==4)
					{
						/* 当前步进 */
						switch (TMP(i_measure_reg))
						{
							case 0:	tmpf = 0.1; break;
							case 1:	tmpf = 0.5;  break;
							case 2:	tmpf = 1.0;  break;
							case 3:	tmpf = 1.5;  break;				
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_measure)/100.0;
								lower = -999999.0;
								upper = 999999.0;
								step = tmpf;
								digit = 1;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_measure)/100.0*0.03937;
								lower = -999999.0*0.03937;
								upper = 999999.0*0.03937;
								step = tmpf;
								digit = 3;
								pos = 5;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3141, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
						}
						else 
						{
							if(get_unit(pp->p_config) == UNIT_MM)
							{
								cur_value = GROUP_VAL(i_measure)/100.0;
								digit = 1;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_VAL(i_measure)/100.0*0.03937;
								digit = 3;
								pos = 5;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
						}
					}
					else if (GROUP_VAL(selection)==5||GROUP_VAL(selection)==6||GROUP_VAL(selection)==7)
					{
						/* 当前步进 */
						switch (TMP(cursors_index_reg))
						{
							case 0:	tmpf = 0.5; break;
							case 1:	tmpf = 2.5; break;
							case 2:	tmpf = 5.0; break;
							case 3:	tmpf = 7.5; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							if(get_unit(pp->p_config)==UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0;
								lower = -100000.0;
								upper = 100000;
								step = tmpf;
								digit = 1;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0*0.03937;
								lower = -100000.0*0.03937;
								upper = 100000*0.03937;
								step = tmpf;
								digit = 3;
								pos = 5;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_3151, units[unit], cur_value , lower, upper, step, digit, p, pos, 16);
						}
						else 
						{
							if(get_unit(pp->p_config)==UNIT_MM)
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0;
								digit = 1;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = GROUP_CURSORS_POS(index)/100.0*0.03937;
								digit = 3;
								pos = 5;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
						}
					}
					break;

				case 2:/*Measurements -> Table -> Edit Comments p325 */
					draw3_popdown(NULL,5,1);
					break;

				case 3:
					if ( !con2_p[3][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:
					if ( !con2_p[3][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:		/*  */
					if(get_display_pos(pp->p_config)==10)
						/*Display 为 Strip Chart-[A]*/
					{
						if(UNIT_MM == get_unit(pp->p_config))
						{
							cur_value = GROUP_VAL(prf)/10.0;
							digit = 2;
							pos = 5;
							unit = UNIT_MM_S;
						}
						else
						{
							cur_value = GROUP_VAL(prf)/10.0*0.03937;
							digit = 3;
							pos = 5;
							unit = UNIT_INCH_S;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
						gtk_widget_set_sensitive (pp->eventbox30[5],FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[5],FALSE);
					}
					else 
					{
						gtk_widget_hide (pp->eventbox30[5]);
						gtk_widget_hide (pp->eventbox31[5]);
					}

					break;

				case 1:/*Display -> Overlay -> overlay  p415 */
					draw3_popdown(menu_content[OFF_ON + get_overlay_overlay(pp->p_config)],5,0);
					break;

				case 2:
					if ( !con2_p[4][2][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 3:/* Display -> properties -> overlay  p435 */
					pp->x_pos = 575, pp->y_pos = 541-YOFFSET;
					switch(get_dis_prop_scan(pp->p_config))
					{
						case 0:	/* Ascan 时候的 overlay */
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
								draw3_pop_tt (data_445, NULL, 
										menu_content[PROP_OVERLAY + GROUP_VAL(ascan_overlay)],
										menu_content+PROP_OVERLAY, 3, 5, GROUP_VAL(ascan_overlay), 0);
							else 
								draw3_popdown (menu_content[PROP_OVERLAY + GROUP_VAL(ascan_overlay)], 5, 0);
							break;
						case 1:
							gtk_widget_hide(pp->eventbox30[5]);
							gtk_widget_hide(pp->eventbox31[5]);
							break;
						case 2:
							gtk_widget_hide(pp->eventbox30[5]);
							gtk_widget_hide(pp->eventbox31[5]);
							break;
						case 3:
							gtk_widget_hide(pp->eventbox30[5]);
							gtk_widget_hide(pp->eventbox31[5]);
							break;
						case 4:
							gtk_widget_hide(pp->eventbox30[5]);
							gtk_widget_hide(pp->eventbox31[5]);
							break;
						case 5:
							gtk_widget_hide(pp->eventbox30[5]);
							gtk_widget_hide(pp->eventbox31[5]);
							break;
						default:break;

					}
					break;
				case 4:
					if ( !con2_p[4][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Auto Detect  p505 */
					draw3_popdown (menu_content[OFF_ON + get_auto_detect (pp->p_config)], 5, 0);
					break;

				case 1:
					if ( !con2_p[5][1][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 2:/*Probe/Part -> characterize -> procedure  p525 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][2][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Undefined");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);

					gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					break;
				case 3:
					if ( !con2_p[5][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:
					if ( !con2_p[5][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:
					if ( !con2_p[6][0][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 1:/* beam skew angle step P615 */
					switch (TMP(beam_skew_angle_step_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((LAW_VAL(Focal_type) == AZIMUTHAL_SCAN) &&
							(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) 
							&& (GROUP_VAL(probe.PA_probe_type) == 9)
							/* 如何判断2D探头 */
					   )
						/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							/* 最大不能超过最大Angle_beam_skew_max */
							cur_value = LAW_VAL (Angle_beam_skew_step) / 100.0;
							/* 计算lower为妙 */
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
							TMP(angle_num)	= (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) /
								LAW_VAL(Angle_step) + 1;
							temp_beam = temp_beam / TMP(angle_num);
							lower = MAX (((gint)(LAW_VAL (Angle_beam_skew_max)) - 
										(gint)(LAW_VAL (Angle_beam_skew_min))) 
									/ (100.0 * temp_beam), 0.1);
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 5;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_614, units[unit], cur_value , lower, 
									upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = LAW_VAL (Angle_beam_skew_max) / 100.0;
							digit = 1;
							pos = 5;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /* 聚焦法则自动计算为OFF, Max.Angle 不可调节 */
					{
						cur_value = LAW_VAL (Angle_beam_skew_min) / 100.0;
						digit = 1;
						pos = 5;
						unit = UNIT_DEG;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					}
					break;
				case 2:
					if ( !con2_p[6][2][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 3:
					if ( !con2_p[6][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:
					if ( !con2_p[6][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encodr -> preset p705 */
					draw3_popdown(NULL,5,1);
					if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
					{
						gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					}
					break;
				case 1: /* Max.Index  Speed  p715 */
					if(get_inspec_type (pp->p_config)==2)
					{
						/* 当前步进 */
						switch (TMP(indexspeed_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}

						if(get_inspec_source (pp->p_config)==0)
							content_pos=0;
						else
							content_pos=7;

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							cur_value = get_inspec_indexspeed (pp->p_config)/10.0;
							lower = 0.0;
							upper = 1000.0;
							step = tmpf;
							digit = 1;
							pos = 5;
							unit = UNIT_RPM;
							draw3_digit_pressed (data_715, units[unit], cur_value , lower, upper, step, digit, p, pos, content_pos);
						}
						else 
						{
							cur_value = get_inspec_indexspeed (pp->p_config)/10.0;
							digit = 1;
							pos = 5;
							unit = UNIT_RPM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
						}

						gtk_widget_set_sensitive (pp->eventbox30[5], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[5], FALSE);
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[5]);
						gtk_widget_hide (pp->eventbox31[5]);
					}
					break;

				case 2:/*Scan -> area -> index resolution p725 */
					/* 当前步进 */
					switch (TMP(index_resolution_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}

					if(get_inspec_type (pp->p_config)==1 || get_inspec_type (pp->p_config)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							if( get_unit(pp->p_config) == UNIT_MM )
							{
								cur_value = get_area_indexresolution (pp->p_config)/1000.0;
								lower = 0.01;
								upper = 99999.0;
								step = tmpf;
								digit = 2;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_indexresolution (pp->p_config)/1000.0*0.03937;
								lower = 0.001;
								upper = 99999.0 * 0.03937;
								step = tmpf/10.0;
								digit = 3;
								pos = 5;
								unit = UNIT_INCH;
							}
							draw3_digit_pressed (data_725, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							if( get_unit(pp->p_config) == UNIT_MM )
							{
								cur_value = get_area_indexresolution (pp->p_config)/1000.0;
								digit = 2;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = get_area_indexresolution (pp->p_config)/1000.0 * 0.03937;
								digit = 3;
								pos = 5;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
						/* Inspection -> Type 选择 One-Line Scan 时 */
					{
						if( get_unit(pp->p_config) == UNIT_MM )
						{
							cur_value = get_area_indexresolution (pp->p_config)/1000.0;
							digit = 2;
							pos = 5;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_area_indexresolution (pp->p_config)/1000.0 * 0.03937;
							digit = 3;
							pos = 5;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

						gtk_widget_set_sensitive (pp->eventbox30[5], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[5], FALSE);
					}
					break;


				case 3:
					if ( !con2_p[7][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:
					if ( !con2_p[7][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0: /*File -> File -> file name  p805 */
					draw3_popdown ("Data####", 5, 0); break;
				case 1:
					if ( !con2_p[8][1][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 2:
					if ( !con2_p[8][2][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 3:
					if ( !con2_p[8][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:
					if ( !con2_p[8][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:
					if ( !con2_p[9][0][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 1:
					if ( !con2_p[9][1][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 2:
					if ( !con2_p[9][2][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 3:
					if ( !con2_p[9][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:/*Preferences -> Network -> connect*/
					if ( !con2_p[9][4][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				default:break;
			}
			break;
		default:break;
	}


}

/* 显示系统时间 */
static gboolean time_handler1 (GtkWidget *widget)
{
	time_t curtime;
	struct tm *loctime;
	gchar *markup;

	curtime = time(NULL);
	loctime = localtime(&curtime);
	strftime (buffer, 32, "%F %T", loctime);
	markup=g_markup_printf_escaped ("<span foreground='white' font_desc='10'>%s</span>", buffer);
	gtk_label_set_markup (GTK_LABEL(pp->label[4]),markup);

	g_free (markup);
	return TRUE;
}

#if ARM
static void	compress_data (DOT_TYPE *source_data, DOT_TYPE *target_data,
		gint qty1, gint qty2, guint	rectify)
{
	guint	temp_1, temp_2, i, j;
	for (i = 0; i < qty2; i++) 
	{
		temp_2 = qty1 / qty2;
		temp_1 = i * qty1 / qty2;
		target_data[i] = source_data[temp_1];
		for (j = 1; j < temp_2; j++) {
			if (rectify == RF_WAVE) {
				if (target_data[i] > 127) {
					if (source_data[temp_1 + j] > target_data[i]) 
						target_data[i] = source_data[temp_1 + j];
				} else {
					if (source_data[temp_1 + j] < target_data[i]) 
						target_data[i] = source_data[temp_1 + j];
				}
			} else {
				if (source_data[temp_1 + j] > target_data[i]) 
					target_data[i] = source_data[temp_1 + j];
			}
		}
	}
}


static void interpolation_data (DOT_TYPE *source_data, DOT_TYPE *target_data,
		gint qty1, gint qty2)
{
	gint	t1, t2, i, j;
	if (qty1 == qty2)
		memcpy ((void *)(target_data), (void *)(source_data), qty1 * sizeof(DOT_TYPE));
	else 
	{
		for (i = 0 ; i < qty1 ; i++)
		{
			gint tmp = (((gint)(source_data[i + 1]) - (gint)(source_data[i])) / (qty2 / qty1));
			for (t1 = i * qty2 / qty1, t2 = (i + 1) * qty2 /qty1, j = t1; j < t2; j++)
			{
				target_data[j] = (guchar)((gint)(source_data[i]) + 
						(j - t1) * tmp);//((gint)(source_data[i + 1]) - (gint)(source_data[i])) / (qty2 / qty1));
			}
		}
	}
}
/*
static void interpolation_data1 (DOT_TYPE *source_data, DOT_TYPE *target_data,
		gint qty1, gint qty2)
{
	gint i;
	for (i = 0 ; i < qty1 ; i++)
	{
		target_data[i] = 0x80;
	}
}
*/
#endif

#if 0
gboolean bt_release (GtkWidget *widget, GdkEventButton *event,
		gpointer user_data) 
{
	gint i = GPOINTER_TO_UINT(user_data);
	fakekey_press_keysym(pp->fk, keyboard_send[i], 0);
	fakekey_release(pp->fk);

	return TRUE;
}
#endif

gboolean numbt_release (GtkWidget *widget, GdkEventButton *event,
		gpointer user_data) 
{
	gint i = GPOINTER_TO_UINT(user_data);
	fakekey_press_keysym(pp->fk, numkeyboard_send[i], 0);
	fakekey_release(pp->fk);

	return TRUE;
}

void draw_keyboard (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	if (pp->win_keyboard) 
	{
		gtk_widget_destroy (pp->win_keyboard);
		pp->win_keyboard = NULL;
	}
# if 0
	else 
	{

		gint i, j;
		GtkWidget *button[5];
		GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
		GtkWidget *hbox[5];

		pp->win_keyboard = gtk_window_new (GTK_WINDOW_POPUP);

		//for ( i = 0 ; i < 3; i++) 
		//{
		hbox[0] = gtk_hbox_new(FALSE, 5);
		for (j = 0 ; j < 13; j++ ) {
			button[0] = gtk_button_new_with_label (keyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[0]), button[0]);
			g_signal_connect(G_OBJECT(button[0]), "button-release-event",
					G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[0]);

		hbox[1] = gtk_hbox_new(FALSE, 5);
		for (j = 13 ; j < 26; j++ ) {
			button[1] = gtk_button_new_with_label (keyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[1]), button[1]);
			g_signal_connect(G_OBJECT(button[1]), "button-release-event",
					G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[1]);

		hbox[2] = gtk_hbox_new(FALSE, 5);
		for (j = 26 ; j < 39; j++ ) {
			button[2] = gtk_button_new_with_label (keyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[2]), button[2]);
			g_signal_connect(G_OBJECT(button[2]), "button-release-event",
					G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[2]);

		hbox[3] = gtk_hbox_new(FALSE, 5);
		for (j = 39 ; j < 51; j++ ) {
			button[3] = gtk_button_new_with_label (keyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[3]), button[3]);
			g_signal_connect(G_OBJECT(button[3]), "button-release-event",
					G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[3]);

		hbox[4] = gtk_hbox_new(FALSE, 5);
		for (j = 51 ; j < 58; j++ ) {
			button[4] = gtk_button_new_with_label (keyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[4]), button[4]);
			g_signal_connect(G_OBJECT(button[4]), "button-release-event",
					G_CALLBACK(bt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[4]);

		//}
		gtk_container_add(GTK_CONTAINER(pp->win_keyboard), vbox);
		gtk_window_move (GTK_WINDOW (pp->win_keyboard), 215, 155); /* 设置窗口位置 */
		//	gtk_window_set_position (win, GTK_WIN_POS_CENTER);
		gtk_widget_show_all(pp->win_keyboard);

	}
#endif

	else 
	{

		gint j;
		GtkWidget *button[5];
		GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
		GtkWidget *hbox[5];

		pp->win_keyboard = gtk_window_new (GTK_WINDOW_POPUP);

		hbox[0] = gtk_hbox_new(TRUE, 5);
		for (j = 0 ; j < 4; j++ ) {
			button[0] = gtk_button_new_with_label (numkeyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[0]), button[0]);
			g_signal_connect(G_OBJECT(button[0]), "button-release-event",
					G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[0]);

		hbox[1] = gtk_hbox_new(TRUE, 5);
		for (j = 4 ; j < 8; j++ ) {
			button[1] = gtk_button_new_with_label (numkeyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[1]), button[1]);
			g_signal_connect(G_OBJECT(button[1]), "button-release-event",
					G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[1]);

		hbox[2] = gtk_hbox_new(TRUE, 5);
		for (j = 8 ; j < 12; j++ ) {
			button[2] = gtk_button_new_with_label (numkeyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[2]), button[2]);
			g_signal_connect(G_OBJECT(button[2]), "button-release-event",
					G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[2]);

		hbox[3] = gtk_hbox_new(TRUE, 5);
		for (j = 12 ; j < 15; j++ ) {
			button[3] = gtk_button_new_with_label (numkeyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[3]), button[3]);
			g_signal_connect(G_OBJECT(button[3]), "button-release-event",
					G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[3]);

		hbox[4] = gtk_hbox_new(TRUE, 5);
		for (j = 15 ; j < 18; j++ ) {
			button[4] = gtk_button_new_with_label (numkeyboard_display[j]);
			gtk_box_pack_start_defaults(GTK_BOX(hbox[4]), button[4]);
			g_signal_connect(G_OBJECT(button[4]), "button-release-event",
					G_CALLBACK(numbt_release), GUINT_TO_POINTER (j));
		}
		gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox[4]);

		gtk_container_add(GTK_CONTAINER(pp->win_keyboard), vbox);
		gtk_window_move (GTK_WINDOW (pp->win_keyboard), 450, 155); /* 设置窗口位置 */
		gtk_widget_set_size_request (GTK_WIDGET (pp->win_keyboard), 200, 200); /* 设置窗口大小 */
		//	gtk_window_set_position (win, GTK_WIN_POS_CENTER);
		gtk_widget_show_all(pp->win_keyboard);

	}
}

#if ARM
void process_key_press (gchar key)	
{
	switch(key)
	{
		case 0xd0:
			fakekey_press_keysym(pp->fk, XK_Up, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd8:
			fakekey_press_keysym(pp->fk, XK_Down, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd1:
			fakekey_press_keysym(pp->fk, XK_Left, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd7:
			fakekey_press_keysym(pp->fk, XK_Right, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd6:
			fakekey_press_keysym(pp->fk, XK_Return, 0);
			fakekey_release(pp->fk);
			break;
		case 0xf1:
			fakekey_press_keysym(pp->fk, XK_Return, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd2:
			fakekey_press_keysym(pp->fk, XK_Super_L, 0);
			fakekey_release(pp->fk);
			break;
		case 0xd9:
			fakekey_press_keysym(pp->fk, XK_Escape, 0);
			fakekey_release(pp->fk);
			break;
		case 0xef:
			fakekey_press_keysym(pp->fk, XK_Escape, 0);
			fakekey_release(pp->fk);
			break;

			   case 0xd3:
			   fakekey_press_keysym(pp->fk, GDK_KP_8, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xda:
			   fakekey_press_keysym(pp->fk, GDK_KP_9, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xd4:
			   fakekey_press_keysym(pp->fk, GDK_KP_6, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xdb:
			   fakekey_press_keysym(pp->fk, GDK_KP_7, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xd5:
			   fakekey_press_keysym(pp->fk, GDK_KP_4, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xdc:
			   fakekey_press_keysym(pp->fk, GDK_KP_5, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xdd:
			   fakekey_press_keysym(pp->fk, GDK_KP_2, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xde:
			   fakekey_press_keysym(pp->fk, GDK_KP_3, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xdf:
			   fakekey_press_keysym(pp->fk, GDK_KP_0, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xe0:
			   fakekey_press_keysym(pp->fk, GDK_KP_1, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xe1:
			   fakekey_press_keysym(pp->fk, GDK_KP_Subtract, 0);/* +/- ?????? */
			   fakekey_release(pp->fk);
			   break;
			   case 0xe2:
			   fakekey_press_keysym(pp->fk, GDK_KP_Divide, 0); /*  小数点 ??????  */
			   fakekey_release(pp->fk);
			   break;
			   case 0xe3:
			   fakekey_press_keysym(pp->fk, GDK_Super_L, 0);
			   fakekey_release(pp->fk);
			   break;

		case 0xe4:
			fakekey_press_keysym(pp->fk, XK_F2, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe5:
			fakekey_press_keysym(pp->fk, XK_F3, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe6:
			fakekey_press_keysym(pp->fk, XK_F4, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe7:
			fakekey_press_keysym(pp->fk, XK_F5, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe8:
			fakekey_press_keysym(pp->fk, XK_F6, 0);
			fakekey_release(pp->fk);
			break;
		case 0xe9:
			fakekey_press_keysym(pp->fk, XK_Help, 0);
			fakekey_release(pp->fk);
			break;
		case 0xea:
			fakekey_press_keysym(pp->fk, XK_F7, 0);
			fakekey_release(pp->fk);
			break;
		case 0xeb:
			fakekey_press_keysym(pp->fk, XK_F8, 0);
			fakekey_release(pp->fk);
			break;
		case 0xec:
			fakekey_press_keysym(pp->fk, XK_F9, 0);
			fakekey_release(pp->fk);
			break;
		case 0xed:
			fakekey_press_keysym(pp->fk, XK_F10, 0);
			fakekey_release(pp->fk);
			break;
		case 0xee:
			fakekey_press_keysym(pp->fk, XK_F11, 0);
			fakekey_release(pp->fk);
			break;
		case 0xf2:
			fakekey_press_keysym(pp->fk, XK_F12, 0);
			fakekey_release(pp->fk);
			break;
	}
}


/* 计算测量数值 */
void calc_measure_data()
{
	gint offset,k;
	gint l,n;

	gint grp = get_current_group(pp->p_config);//当前group
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	gint index = offset + TMP(beam_num[grp]);	
	gint i = TMP(beam_num[grp]);//当前beam
	gfloat a = ( (gfloat)(LAW_VAL_POS (grp, Angle_min) + LAW_VAL_POS (grp, Angle_step)*i)/100.0 )*G_PI/180.0;//当前折射角
	gfloat thickness = (gfloat)(get_part_thickness(pp->p_config)/1000.0);//工件厚度	
//	printf("----->distance[%d]=%f a=%f thickness=%f\n",i,TMP(field_distance[i]),a,thickness);

	for(l=0;l<4;l++)//4个field
	{
		switch( DO_NOT_USE_CCFG(field[l]) )//field1
		{
			case 0://A%
				DO_NOT_USE_CCFG(measure_data[index]).a_height = (float)(((TMP(measure_data[index][1])>>20) & 0xfff)/20.47);//满屏时200% 4095 
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).a_height;
				break;
			case 3://B%
				DO_NOT_USE_CCFG(measure_data[index]).b_height = (float)(((TMP(measure_data[index][2])>>20) & 0xfff)/20.47);
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).b_height;
				break;
			case 6://A^
				if(GROUP_VAL(ut_unit)==1)//Time
					DO_NOT_USE_CCFG(measure_data[index]).a_position = (float)(((TMP(measure_data[index][1])) & 0xfffff)/100.0);//直接显示时间微妙
				else
					DO_NOT_USE_CCFG(measure_data[index]).a_position = (float)(((TMP(measure_data[index][1])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/20000000.0);//距离模式s= time * velo/2
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).a_position;
				break;
			case 7://B^
				if(GROUP_VAL(ut_unit)==1)
					DO_NOT_USE_CCFG(measure_data[index]).b_position = (float)(((TMP(measure_data[index][2])) & 0xfffff)/100.0);//直接显示时间微妙
				else
					DO_NOT_USE_CCFG(measure_data[index]).b_position = (float)(((TMP(measure_data[index][2])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/20000000.0);
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).b_position;
				break;
			case 8://I/
				if(GROUP_VAL(ut_unit)==1)
					DO_NOT_USE_CCFG(measure_data[index]).i_position = (float)(((TMP(measure_data[index][3])) & 0xfffff)/100.0);//直接显示时间微妙
				else
					DO_NOT_USE_CCFG(measure_data[index]).i_position = (float)(((TMP(measure_data[index][3])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/20000000.0);
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).i_position;

				break;
			case 27://RA
				break;
			case 28://RB
				break;
			case 29://PA
				break;
			case 30://PB
				break;
			case 31://DA^
				/********先计算SA^***********/
				switch(GROUP_VAL(ut_unit))	
				{
					case 0://Sound Path
						//DO_NOT_USE_CCFG(measure_data[index]).a_sound_path = ((TMP(measure_data[index][1])) & 0xfffff)/100.0;//=A^
						//break;
					case 1://Time
						DO_NOT_USE_CCFG(measure_data[index]).a_sound_path = (float)(((TMP(measure_data[index][1])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/20000000.0);//Time(A^)*声速/2
						break;
					case 2://True Depth
						DO_NOT_USE_CCFG(measure_data[index]).a_sound_path = (float)(((TMP(measure_data[index][1])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/(cos(a)*20000000.0) +TMP(field_distance[i]));//=DA^/cos(a)+distance				
						break;
				}
				/******由SA^计算DA^*************/	
				n = (gint)((DO_NOT_USE_CCFG(measure_data[index]).a_sound_path -TMP(field_distance[i]))*cos(a)/thickness);//反射次数
				if( n%2 == 0 )
				{
					DO_NOT_USE_CCFG(measure_data[index]).a_depth = (float)((DO_NOT_USE_CCFG(measure_data[index]).a_sound_path -TMP(field_distance[i]))*cos(a) - thickness);
				}
				else
				{
					DO_NOT_USE_CCFG(measure_data[index]).a_depth =  (float)((n+1)*thickness - (DO_NOT_USE_CCFG(measure_data[index]).a_sound_path -TMP(field_distance[i]))*cos(a));
				}
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).a_depth;
				break;
			case 32://DB^
				/********先计算SA^***********/
				switch(GROUP_VAL(ut_unit))	
				{
					case 0://Sound Path
						//DO_NOT_USE_CCFG(measure_data[index]).b_sound_path = ((TMP(measure_data[index][2])) & 0xfffff)/100.0;//=A^
						//break;
					case 1://Time
						DO_NOT_USE_CCFG(measure_data[index]).b_sound_path = (float)(((TMP(measure_data[index][2])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/20000000.0);//Time(A^)*声速/2
						break;
					case 2://True Depth
						DO_NOT_USE_CCFG(measure_data[index]).b_sound_path = (float)(((TMP(measure_data[index][2])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/(cos(a)*20000000.0) +TMP(field_distance[i]));//=DA^/cos(a)+distance				
						break;
				}
				/******由SA^计算DA^*************/	
				n = (gint)((DO_NOT_USE_CCFG(measure_data[index]).a_sound_path -TMP(field_distance[i]))*cos(a)/thickness);//反射次数
				if( n%2 == 0 )
				{
					DO_NOT_USE_CCFG(measure_data[index]).b_depth = (float)((DO_NOT_USE_CCFG(measure_data[index]).b_sound_path -TMP(field_distance[i]))*cos(a) - thickness);
				}
				else
				{
					DO_NOT_USE_CCFG(measure_data[index]).b_depth =  (float)((n+1)*thickness - (DO_NOT_USE_CCFG(measure_data[index]).b_sound_path -TMP(field_distance[i]))*cos(a));
				}
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).b_depth;
				break;
			case 33://SA^
				switch(GROUP_VAL(ut_unit))	
				{
					case 0://Sound Path
						DO_NOT_USE_CCFG(measure_data[index]).a_sound_path = (float)(((TMP(measure_data[index][1])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/20000000.0);//Time(A^)*声速/2
						break;
					case 1://Time
						DO_NOT_USE_CCFG(measure_data[index]).a_sound_path = (float)(((TMP(measure_data[index][1])) & 0xfffff)/100.0);//=A^
						break;
					case 2://True Depth
						DO_NOT_USE_CCFG(measure_data[index]).a_sound_path = (float)(((TMP(measure_data[index][1])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/(cos(a)*20000000.0) +TMP(field_distance[i]));//=DA^/cos(a)+distance				
						break;
				}
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).a_sound_path;
				break;
			case 34://SB^
				switch(GROUP_VAL(ut_unit))	
				{
					case 0://Sound Path
						DO_NOT_USE_CCFG(measure_data[index]).b_sound_path = (float)(((TMP(measure_data[index][2])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/20000000.0);//Time(A^)*声速/2
						break;
					case 1://Time
						DO_NOT_USE_CCFG(measure_data[index]).b_sound_path = (float)(((TMP(measure_data[index][2])) & 0xfffff)/100.0);//=A^
						break;
					case 2://True Depth
						DO_NOT_USE_CCFG(measure_data[index]).b_sound_path = (float)(((TMP(measure_data[index][2])) & 0xfffff)*get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/(cos(a)*20000000.0) +TMP(field_distance[i]));//=DA^/cos(a)+distance			
						break;
				}
				TMP(field[l]) = DO_NOT_USE_CCFG(measure_data[index]).b_sound_path;
				break;
			case 35://ViA
				break;
			case 36://ViB
				break;
			case 37://VsA
				break;
			case 38://VsB
				break;
			case 39://LA
				break;
			case 40://LB
				break;
			default :
				break;
		 }
	}
	
	

}



static void signal_scan_thread(void)
{
	while(1)
	{
		if(*DMA_MARK==0)
		{
                     pthread_mutex_lock(&draw_thread_mutex);
		             pthread_cond_signal(&draw_thread_signal);
            	     pthread_mutex_unlock(&draw_thread_mutex);
					//sem_post(&sem) ;
		}
		if(*DMA_MARK>2){ printf("DMA_MARK = %d \n", *DMA_MARK) ; *DMA_MARK=1; }
		usleep(40000);
	}
}

static void key_message_thread(void)
{
	char key = 0;
	char bar[3] = {0};
    while(1)
	{
	    if (read(pp->fd_key, &key, 1) > 0) 
	    {	
		     process_key_press (key);
			 //读取电池信息	
			 if(key == 0xaa)
			 {
				if( read(pp->fd_key, bar, 3) >0 )
				{
					if( (bar[0]==0xaa) && (bar[1]==0xaa) && (bar[2]==0xaa) )
					{
						//将所有电池信息全部读取出来
						read(pp->fd_key, &(pp->battery), 33);
						printf("read battery info successfully \n");
					}
				}
			 }
			//
	    }
		usleep(50000);
    }
}

static void draw_frame_thread(void)
{

	gint i, j, k, offset, offset1;
	guint temp2 = (pp->p_beam_data)+4;
    //unsigned int BeamInfoHeader;
	while (1)
	{
//		while(*DMA_MARK != 0)
			pthread_cond_wait( &draw_thread_signal, &draw_thread_mutex);
		for (i = 0 ; i < get_group_qty(pp->p_config); i++)
		{
			/* 获取数据 */
			/* 这里需要压缩数据 或者 插值数据 这里只有一个beam 同时最多处理256beam */
			for	(j = 0 ; j < TMP(beam_qty[i]); j++)
			{  
				for (offset = 0, offset1 = 0, k = 0 ; k < i; k++)
				{
					offset += (GROUP_VAL_POS(k, point_qty) + 32) * TMP(beam_qty[k]);
					offset1 += TMP(beam_qty[k]);
				}
				memcpy (TMP(measure_data[offset1 + j]), (void *)(temp2 + offset +
							(GROUP_VAL_POS(i, point_qty) + 32) * j + GROUP_VAL_POS(i, point_qty)), 32);

				//BeamInfoHeader = *((int*)(TMP(measure_data[offset1+j])));
				//BeamInfoHeader &= 0x1fff ;
				//printf("Beam info header is %x\n",BeamInfoHeader );
				
				if (GROUP_VAL_POS(i, point_qty) <= TMP(a_scan_dot_qty))
				{
					/* 只插值当前显示的A扫描 其余不插值 */
					interpolation_data (
							(DOT_TYPE *)(temp2 + offset +
								(GROUP_VAL_POS(i, point_qty) + 32) * j),
							TMP(scan_data[i] + TMP(a_scan_dot_qty) * j), 
							GROUP_VAL_POS(i, point_qty),
							TMP(a_scan_dot_qty));
				}
				else if (GROUP_VAL_POS(i, point_qty) > TMP(a_scan_dot_qty))
				{
					compress_data (
							(DOT_TYPE *)(temp2 + offset +
								(GROUP_VAL_POS(i, point_qty) + 32) * j),
							TMP(scan_data[i] + TMP(a_scan_dot_qty) * j), 
							GROUP_VAL_POS(i, point_qty),
							TMP(a_scan_dot_qty),
							get_group_val (get_group_by_id (pp->p_config, i), GROUP_RECTIFIER));
				}
			}
			for (k = 0; ((k < 16) && (TMP(scan_type[k]) != 0xff)); k++)
			{	
				if (TMP(scan_group[k]) == i)
					draw_scan(k, TMP(scan_type[k]), TMP(scan_group[k]), 
							TMP(scan_xpos[k]), TMP(scan_ypos[k]), dot_temp, 
						TMP(fb1_addr) + 768*400);
				//						dot_temp1);
			}
 		}
		*DMA_MARK = 1  ;
		calc_measure_data();//计算数据
		draw_field_value ();
	}
}


#endif

void change_language (guint lang, DRAW_UI_P p)
{
	switch (lang)
	{ 
		case ENGLISH_:
			p->con0_p	= content_en10;
			p->con1_p	= content1_en;
			p->con2_p	= content2_en;

			p->units	= units_en;
			p->menu_content	= all_menu_content_en;

			p->list		= list_en;
			p->list1	= list1_en;
			p->field1	= field1_en;
			p->field	= field_en;
			if(GROUP_VAL(ut_unit)==1)//Time
				p->field_unit	= field_unit_en;
			else//
				p->field_unit	= field_unit_en_mm;

			con0_p	= content_en10;
			con1_p	= content1_en;
			con2_p	= content2_en;

			units	= units_en;
			menu_content	= all_menu_content_en;

			list		= list_en;
			list1	= list1_en;
			field1	= field1_en;
			field	= field_en;
			break;
		default:break;
	}
}

void draw_field_name ()
{
	gchar	*markup;
	/* 4个测量值名字显示 */
	markup = 
		g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
				pp->field[get_reading_field1(pp->p_config)], pp->field_unit[get_reading_field1(pp->p_config)]);
	gtk_label_set_markup (GTK_LABEL(pp->label[8]),markup);
	g_free (markup);

	markup = 
		g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
				pp->field[get_reading_field2(pp->p_config)], pp->field_unit[get_reading_field2(pp->p_config)]);
	gtk_label_set_markup (GTK_LABEL(pp->label[10]),markup);
	g_free (markup);

	markup = 
		g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
				pp->field[get_reading_field3(pp->p_config)], pp->field_unit[get_reading_field3(pp->p_config)]);
	gtk_label_set_markup (GTK_LABEL(pp->label[12]),markup);
	g_free (markup);

	markup = 
		g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s\n(%s)</span>",
				pp->field[get_reading_field4(pp->p_config)], pp->field_unit[get_reading_field4(pp->p_config)]);
	gtk_label_set_markup (GTK_LABEL(pp->label[14]),markup);
	g_free (markup);

}

gboolean on_finish(gpointer p)
{
	gtk_label_set_markup (GTK_LABEL(pp->label[9]), p);
	return FALSE;
}

void draw_field_value ()
{
//	gint	offset, k;
	gchar	*markup0, *markup1 ,*markup2 ,*markup3;
//	for (offset = 0, k = 0 ; k < get_current_group (pp->p_config); k++)
//		offset += TMP(beam_qty[k]);
//	gint index = offset + TMP(beam_num[get_current_group(pp->p_config)]);
	/* 4个测量值显示 */
	markup0 = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%.2f</span>", TMP(field[0]));
	markup1 = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%.2f</span>", TMP(field[1]));
	markup2 = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%.2f</span>", TMP(field[2]));
	markup3 = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%.2f</span>", TMP(field[3]));
	gdk_threads_enter();
	gtk_label_set_markup (GTK_LABEL(pp->label[9]),  markup0);
	gtk_label_set_markup (GTK_LABEL(pp->label[11]), markup1);
	gtk_label_set_markup (GTK_LABEL(pp->label[13]), markup2);
	gtk_label_set_markup (GTK_LABEL(pp->label[15]), markup3);
	gdk_threads_leave();	
	g_free (markup0);
	g_free (markup1);
	g_free (markup2);
	g_free (markup3);

}

/*初始化界面*/
void init_ui(DRAW_UI_P p)				
{
	gint	i;
	gchar	*markup;
	pthread_t tid0 , tid1, tid2; 
	int ret;
	p_drawui_c = p;

	all_bg_pic_in_mem();
	/* 初始化语言 language init */
	change_language (get_language(pp->p_config), p);

	p->pos_pos		= MENU3_STOP;
	p->menu2_qty	= 5;
	p->pos			= 1;
	p->pos1[0]		= 0;
	p->pos2[0][0]	= 0;

	p->vbox			= gtk_vbox_new(FALSE, 0);	
	p->hbox1		= gtk_hbox_new(FALSE, 0);	
	p->vbox11		= gtk_vbox_new(FALSE, 0);
	p->vbox12		= gtk_vbox_new(FALSE, 0);
	p->hbox121		= gtk_hbox_new(FALSE, 0);	
	p->hbox111		= gtk_hbox_new(FALSE, 0);	
	p->vbox1111[0]	= gtk_vbox_new(FALSE, 0);	
	p->vbox1111[1]  = gtk_vbox_new(FALSE, 0);	
	p->vbox1111[2]  = gtk_vbox_new(FALSE, 0);	
	p->hbox112		= gtk_hbox_new(FALSE, 0);	

	p->hbox2		= gtk_hbox_new(FALSE, 0);	
	p->vbox21		= gtk_vbox_new(FALSE, 0);	
	p->hbox211		= gtk_hbox_new(FALSE, 0);
	p->vboxtable	= gtk_vbox_new(FALSE, 0);
	p->sw			= gtk_scrolled_window_new(NULL, NULL);
	//pp->entry 		= gtk_entry_new();

	p->hbox212		= gtk_hbox_new(FALSE, 0);	

	p->vbox22		= gtk_vbox_new(FALSE, 0);	
	for (i = 0; i < 6; i++)
		p->vbox221[i]	= gtk_vbox_new(FALSE, 0);	

	for (i = 0; i < 20; i++) 
	{
		pp->label[i]	= gtk_label_new ("");
		gtk_widget_modify_fg(pp->label[i], GTK_STATE_NORMAL, &color_white);	/* 字体颜色白色 */
		pp->event[i]	= gtk_event_box_new();
		gtk_container_add(GTK_CONTAINER(p->event[i]), p->label[i]);
		gtk_container_set_border_width( GTK_CONTAINER(pp->event[i]), 0);     /*设置边框大小，这个地方使用图片*/
	}

	/* 一级菜单的初始化 */
	p->menubar		= gtk_menu_bar_new();
	p->menu			= gtk_menu_new();
	p->menuitem_main	= gtk_menu_item_new_with_label(content_en10[1]);
	gtk_widget_set_size_request(GTK_WIDGET(p->menuitem_main), 114, 83);        
	gtk_menu_bar_append(GTK_MENU_BAR(p->menubar), p->menuitem_main);
	for (i = 0; i < 10; i++)
	{
		p->menuitem[i]	= gtk_menu_item_new_with_label(content_en10[i]);
		gtk_widget_set_size_request(GTK_WIDGET(p->menuitem[i]), 120, 35);
		g_signal_connect(G_OBJECT(p->menuitem[i]), "activate", 
				G_CALLBACK(menuitem_function), GUINT_TO_POINTER (i));
		gtk_menu_shell_append(GTK_MENU_SHELL(p->menu), p->menuitem[i]);
		gtk_widget_show(p->menuitem[i]);
		if ((i == 6) && GROUP_VAL (group_mode) == UT_SCAN)
			gtk_widget_set_sensitive(pp->menuitem[i] ,FALSE);
	}
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(p->menuitem_main), p->menu);
	gtk_box_pack_start(GTK_BOX(p->hbox212), p->menubar, FALSE, FALSE, 0);

	g_signal_connect (pp->menu, "key-press-event", 
			G_CALLBACK(key_press_handler), (gpointer) (MAIN_MENU_PRESS));

	g_signal_connect (pp->menuitem_main, "button-press-event", 
			G_CALLBACK(main_menu_press), NULL);
	gtk_widget_show(p->menubar);
	gtk_widget_show(p->menu);
	gtk_widget_show(p->menuitem_main);

	/* 二级菜单的初始化 */
	for (i = 0; i < 5; i++) 
	{
		p->eventbox2[i] = gtk_event_box_new();
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox2[i]), 113, 85);
		p->label2[i] = gtk_label_new("");
		gtk_container_add(GTK_CONTAINER(p->eventbox2[i]), p->label2[i]);
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox2[i]), 0);     
		update_widget_bg(p->eventbox2[i], /*backpic[1]*/ 1);
		gtk_widget_modify_fg(p->label2[i], GTK_STATE_NORMAL, &color_white);
		g_signal_connect(G_OBJECT(p->eventbox2[i]), "button-press-event", 
				G_CALLBACK(eventbox2_function0), GUINT_TO_POINTER (i));

		gtk_box_pack_start(GTK_BOX(p->hbox212), p->eventbox2[i], FALSE, FALSE, 0);
		gtk_widget_show(p->eventbox2[i]);
		gtk_widget_show(p->label2[i]);
	}


	/*三级菜单的初始化*/
	for (i = 0; i < 6; i++)
	{
		p->eventbox30[i] = gtk_event_box_new();
		p->eventbox31[i] = gtk_event_box_new();
		p->label3[i]     = gtk_label_new("");
		p->data3[i]      = gtk_label_new("");
		pp->adj = (GtkAdjustment *) gtk_adjustment_new (10.0, 0.0, 74.0, 0.1, 10.0, 0.0);
		pp->sbutton[i] = gtk_spin_button_new (pp->adj, 0, 1);
 		gtk_spin_button_set_numeric((GtkSpinButton*)pp->sbutton[i], 1);

		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox30[i]), 114, 60);
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox31[i]), 114, 25);       
		gtk_widget_set_size_request(GTK_WIDGET(p->sbutton[i]), 114, 25); 
		update_widget_bg(pp->eventbox30[i], /*backpic[7]*/ 7);
		update_widget_bg(pp->eventbox31[i], /*backpic[10]*/ 10);
		gtk_widget_modify_base(GTK_WIDGET (pp->sbutton[i]), GTK_STATE_NORMAL, &color_text_base);
		gtk_widget_modify_text(GTK_WIDGET (pp->sbutton[i]), GTK_STATE_NORMAL, &color_yellow);

		gtk_container_set_border_width(GTK_CONTAINER(p->eventbox30[i]), 0);   
		gtk_container_set_border_width(GTK_CONTAINER(p->eventbox31[i]), 0);
		gtk_container_add(GTK_CONTAINER(p->eventbox30[i]), p->label3[i]);
		gtk_container_add(GTK_CONTAINER(p->eventbox31[i]), p->data3[i]);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox30[i], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox31[i], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->sbutton[i], FALSE, FALSE, 0);
		gtk_widget_modify_fg(p->label3[i], GTK_STATE_NORMAL, &color_white);
		gtk_widget_modify_fg(p->data3[i], GTK_STATE_NORMAL, &color_white);
		g_signal_connect(G_OBJECT(p->eventbox30[i]), "button-press-event", 
				G_CALLBACK(data_function0), (GUINT_TO_POINTER (i)));
		g_signal_connect(G_OBJECT(p->eventbox31[i]), "button-press-event", 
				G_CALLBACK(data_function0), (GUINT_TO_POINTER (i)));
		gtk_widget_show(p->eventbox30[i]);
		gtk_widget_show(p->eventbox31[i]);
		gtk_widget_show(p->label3[i]);
	}

	/* 3级菜单的显示 */
	for (i = 0; i < 6; i++)
	{
		gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221[i], FALSE, FALSE, 0);
		gtk_widget_show(p->vbox221[i]);
	}

	/* 三级菜单 弹出的初始化 */
	pp->menu3 = gtk_menu_new();
	for ( i = 0; i < 6; i++)
	{
		pp->menu33[i] = gtk_menu_new();
		g_signal_connect (pp->menu33[i], "key-press-event", 
				G_CALLBACK(key_press_handler) ,(gpointer)(i + 2));
	}

	for ( i = 0; i < 64; i++)
		pp->menu_item3[i] = NULL;

	/* 各box的包含关系*/
	gtk_container_add(GTK_CONTAINER(p->window), p->vbox);
	gtk_widget_show(p->vbox);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox1, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox1);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox2, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox2);

	/* 上方数据显示  */
	gtk_box_pack_start (GTK_BOX (p->hbox1), pp->vbox11, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox1), pp->vbox12, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox111, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox112, FALSE, FALSE, 0);

	/*软键盘显示*/
	pp->drawing_area = gtk_drawing_area_new();
	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->drawing_area, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->drawing_area), 115, 65); 
	g_signal_connect (G_OBJECT (pp->drawing_area), "expose_event",
			G_CALLBACK (draw_other_info), NULL);		/* 电池信息 */

	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->hbox121, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[17], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[17]), 40, 25);
	update_widget_bg(pp->event[17], /*backpic[12]*/ 12); 
	g_signal_connect (G_OBJECT (pp->event[17]), "button-press-event", 
			G_CALLBACK (draw_keyboard), NULL);		/* 虚拟键盘 */
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[18], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[18]), 70, 25); 
	gtk_widget_modify_bg(pp->event[18], GTK_STATE_NORMAL, &color_black);
	/* 增益显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[0], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[0]), 60, 45);
	update_widget_bg(pp->event[0], /*backpic[3]*/ 3);
	if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
		tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
	else
		tt_label_show_string (pp->label[0], con2_p[1][0][0], "\n", "(dB)", "white", 10);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[1], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[1]), 112, 45);  
	update_widget_bg(pp->event[1], /*backpic[4]*/ 4);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>", 
			get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) / 100.0);
	gtk_label_set_markup (GTK_LABEL (pp->label[1]), markup);      /* 增益数值 */
	g_free (markup);

	/* 小状态栏  */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[2], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[2], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[2]), 172, 22);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>%s</span>", "current.cfg");
	gtk_label_set_markup (GTK_LABEL (pp->label[2]), markup); 
	g_free (markup);
	update_widget_bg(pp->event[2], /*backpic[5]*/ 5);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[3], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[3]), 172, 22);
	gtk_label_set_justify (GTK_LABEL (pp->label[3]), PANGO_ELLIPSIZE_START);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",
			GROUP_VAL(prf) / 10, GROUP_VAL(prf) / 10);
	gtk_label_set_markup (GTK_LABEL (pp->label[3]), markup); 
	g_free (markup);
	update_widget_bg(pp->event[3], /*backpic[5]*/ 5);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[4], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[4]), 172, 22);
	time_handler1(NULL);
	update_widget_bg(pp->event[4], /*backpic[5]*/ 5);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[5], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[5]), 172, 22);
	update_widget_bg(pp->event[5], /*backpic[5]*/ 5);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[6], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[6]), 172, 22);
	markup = g_markup_printf_escaped ("<span foreground='red' font_desc='10'>%s</span>", VERSION);
	gtk_label_set_markup (GTK_LABEL (pp->label[6]), markup); 
	g_free (markup);
	g_signal_connect(G_OBJECT(pp->event[6]), "button-press-event", 
			G_CALLBACK(save_config), NULL);

	update_widget_bg(pp->event[6], /*backpic[5]*/ 5);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",
			(gfloat)(GROUP_VAL(prf)));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;
	g_free (markup);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[7], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[7]), 172, 22);
	update_widget_bg(pp->event[7], /*backpic[5]*/ 5);
	if(get_inspec_source (pp->p_config)==0)
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f s</span>",
				(gfloat)(GROUP_VAL(prf)));
	else
		markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f mm</span>",
				(gfloat)(GROUP_VAL(prf)));
	gtk_label_set_markup (GTK_LABEL (pp->label[7]), markup); 
	g_free (markup);

	/* 4个测量值显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[8], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>%%A\n(%%)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[8]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[8]), 60, 45);
	update_widget_bg(pp->event[8], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[9], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[9]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[9]), 112, 45);
	update_widget_bg(pp->event[9], /*backpic[4]*/ 4);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[10], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>DA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[10]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[10]), 60, 34);
	update_widget_bg(pp->event[10], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[11], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[11]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[11]), 112, 34);
	update_widget_bg(pp->event[11], /*backpic[4]*/ 4);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[12], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>PA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[12]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[12]), 60, 34);
	update_widget_bg(pp->event[12], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[13], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[13]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[13]), 112, 34);
	update_widget_bg(pp->event[13], /*backpic[4]*/ 4);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[14], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>SA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[14]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[14]), 60, 34);
	update_widget_bg(pp->event[14], /*backpic[3]*/ 3);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[15], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[15]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[15]), 112, 34);
	update_widget_bg(pp->event[15], /*backpic[4]*/ 4);

	draw_field_name ();

	gtk_widget_show_all (pp->hbox1);   /* 上方显示信息  */

	/* 画图区域及各button */
	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox21, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox22, FALSE, FALSE, 0);
	gtk_widget_show(p->vbox21);
	gtk_widget_show(p->vbox22);

	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox211, FALSE, FALSE, 0);
	gtk_widget_set_size_request(p->sw,685,425);	/* 显示帮助文档 */
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->sw, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox212, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox211);
	gtk_widget_show(p->hbox212);

	/*帮助文档*/
	pp->web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());	
	webkit_web_view_set_custom_encoding (pp->web_view, "UTF-8");
	gtk_container_add(GTK_CONTAINER (pp->sw), GTK_WIDGET (pp->web_view));

	pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/UT_Settings_Menu.html";
	webkit_web_view_load_uri (pp->web_view, pp->file_path);

	gtk_box_pack_start (GTK_BOX (p->hbox211), p->vboxtable, FALSE, FALSE, 0);
	gtk_widget_show(p->vboxtable);

	draw_area_all ();
	gtk_widget_show_all (p->hbox2); /* 画图区域 及 button 的显示 */

	widget_window_class = GTK_WIDGET_GET_CLASS ((GtkObject*)(pp->window)); 
	window_keypress_event_orig = widget_window_class->key_press_event; 

	pp->dialog = gtk_dialog_new_with_buttons("TanDenghua", GTK_WINDOW (pp->window),
			GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	dialog_window_class = GTK_WIDGET_GET_CLASS ((GtkObject*)(pp->dialog)); 
	dialog_keypress_event_orig = dialog_window_class->key_press_event; 

	gtk_window_set_decorated (GTK_WINDOW (pp->dialog), FALSE);			/*不可以装饰*/
	gtk_container_set_border_width( GTK_CONTAINER (GTK_DIALOG(pp->dialog)->vbox), 0);     /* */
	gtk_widget_show (GTK_DIALOG(pp->dialog)->vbox);
	gtk_widget_hide (GTK_DIALOG(pp->dialog)->action_area);

	/* scale 上面的透明条 */
	pp->scale_drawarea = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(pp->scale_drawarea), 30, 425);
	gtk_widget_modify_bg(pp->scale_drawarea, GTK_STATE_NORMAL, &color_black);

	/* scale 快速调节数值 */
	pp->button_add = gtk_button_new_with_label ("+");    /* 加减数值这里功能数值加减会出现不准的时候 */
	g_signal_connect (pp->button_add, "clicked", 
			G_CALLBACK(add_click), NULL);
	pp->button_sub = gtk_button_new_with_label ("-");    
	g_signal_connect (pp->button_sub, "clicked", 
			G_CALLBACK(sub_click), NULL);

	pp->vscale = gtk_vscale_new_with_range(1.0, 100.0, 1.0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->vscale), 30, 370);

	/* vscalebox */
	pp->vscalebox = gtk_vbox_new (FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->vscalebox), 30, 425);
	gtk_widget_show (pp->vscalebox);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->scale_drawarea, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->button_add, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->vscale, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->button_sub, FALSE, FALSE, 0);
	gtk_widget_show (pp->scale_drawarea); 
	gtk_widget_show (pp->button_add);
	gtk_widget_show (pp->button_sub);
	gtk_widget_show (pp->vscale);
	gtk_widget_show (pp->vscalebox);
	gtk_widget_hide (pp->sw);

	gtk_widget_set_can_focus (pp->button_add, FALSE);
	gtk_widget_set_can_focus (pp->vscale, FALSE);
	gtk_widget_set_can_focus (pp->button_sub, FALSE);

	gtk_box_pack_start (GTK_BOX (p->hbox211), pp->vscalebox, FALSE, FALSE, 0);

	change_keypress_event (KEYPRESS_MAIN);

	draw_menu1 ();
	draw_menu2 (1);
	draw_menu3 (1, NULL);


	if (!g_thread_supported()) 
	{
		g_thread_init (NULL);
	}

	pp->mark3 = 1;
	pp->mark2 = 1;
	pp->scan_count = 1;
#if ARM
	DMA_MARK = (int*)(pp->p_beam_data + 0x800000)  ;
    printf("DMA_MAKR is %d \n", *DMA_MARK);	
	ret = pthread_create (&tid0, NULL, (void*)key_message_thread, NULL);
	if(ret){
		perror("in1:");
	    return;
	}
	ret = pthread_create (&tid1, NULL, (void*)draw_frame_thread, NULL);
    if(ret){
		perror("in1:");
	    return;
	}
	ret = pthread_create (&tid2, NULL, (void*)signal_scan_thread, NULL);	
	if(ret){
		perror("in1:");
	    return;
	}
#endif

	g_timeout_add (1000, (GSourceFunc) time_handler1, NULL);
}

void save_config (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	gint i;
	i	=	lseek (TMP(fd_config), 0, SEEK_SET);
	i	=	write (TMP(fd_config), pp->p_config, sizeof(CONFIG));
	close (TMP(fd_config));
	gtk_main_quit();
}

void draw_dac_curve(cairo_t *cr, int width, int height)
{
	  double point_x[18] ;
	  double point_y[18] ;
	  double scale_y[18] ;
	  int i, j ;
	  double tmpInterval ;
	  double interval;
      double tmp ;
      double ref_ampl_offset;
      double reference_db ;
      int point_count = (int)GROUP_VAL(dac_point_qty) ;
      printf("dac_point_qty %d \n", GROUP_VAL(dac_point_qty));
      printf("point_count %d \n", point_count);

      ref_ampl_offset = pow(10.0, GROUP_VAL(ref_ampl_offset)/2000.0) ;

      point_y[0] = (height * GROUP_VAL(ref_ampl)/1000.0) * ref_ampl_offset ;
      point_x[0] = 0 ;
      tmp = pow(10.0, GROUP_VAL(curve_step)/2000.0);
      interval = point_y[0]*(tmp-1) ;
      reference_db = (pow(10.0 , GROUP_VAL(ref_gain)/2000.0)-1)  ;
      reference_db = point_y[0] * reference_db  ;
      point_y[0] += reference_db ;

	  for( i = 1; i < point_count+1; i++ )
	  {
		  point_x [i] = point_x[i-1] + width * (GROUP_VAL(position[i-1])*200000.0) / ( ((double)(GROUP_VAL(range))) * ((double)(GROUP_VAL(velocity)))) ;
		  point_y [i] = reference_db + ref_ampl_offset * height * (GROUP_VAL( amplitude[i-1] ) / 100000.0) ;
	  }

	  point_x[point_count+1] = width ;
	  point_y[point_count+1] = point_y[point_count] ;

	  cairo_set_source_rgb (cr, 255, 0, 0 );
	  for(i = 1 ; i <= point_count ; i++)
	  {
			cairo_rectangle(cr , 17 + point_x[i], height - point_y[i] - 3, 6 ,6 );
			cairo_fill(cr);
	  }
	  scale_y[0] = 1.0 ;
	  scale_y[point_count+1] = 1.0 ;

	  for(i = 1; i<= point_count+1; i++ )
	  {
		  scale_y[i] = point_y[i]/ point_y[i-1];
	  }

	  cairo_set_source_rgb (cr, 0, 255 ,0);
	  cairo_set_line_width (cr, 0.5);

      tmp = interval ;
	  for(j = 0; j<=2 ; j++)
      {
		  interval = tmp ;
		  for(i = 0; i<= point_count; i++)
		  {
			  interval = interval*scale_y[i] ;
			  tmpInterval = interval*scale_y[i+1] ;
		      cairo_move_to(cr, 20 + point_x[i],   height - point_y[i] + j*interval);
		      cairo_line_to(cr, 20 + point_x[i+1], height - point_y[i+1] + j*tmpInterval);
		      if(j!=0)
		      {
		           cairo_move_to(cr, 20 + point_x[i],   height - point_y[i]- j* interval);
		           cairo_line_to(cr, 20 + point_x[i+1], height - point_y[i+1]-j* tmpInterval);
		      }
		  }
      }

      cairo_stroke(cr);
}

void draw_tcg_curve(cairo_t *cr , int width , int height )
{
	double point_x[18] ;
    double point_y[18] ;

	int i ;

    int point_count = (int)GROUP_VAL(dac_point_qty) ;
    //printf("dac_point_qty= %d\n",GROUP_VAL(dac_point_qty) );
	//printf("point_count %d \n", point_count);

    point_y[0] = height * GROUP_VAL(ref_ampl)/1000.0 ;  //+ GROUP_VAL(ref_ampl_offset)
    point_x[0] = 0 ;
    //printf("GROUP_VAL(ref_ampl) is %d \n", GROUP_VAL(ref_ampl));
    //printf(" point_y[0] is %f \n", point_y[0]);
    // set color width
	cairo_set_source_rgb (cr, 0, 255 , 0);
	cairo_set_line_width (cr, 0.5);
	  //draw five lines
      //printf("width is %d\n", width);
	for( i = 1; i < point_count+1; i++ )
	{
		  point_x [i] = point_x[i-1] + width * (GROUP_VAL(position[i-1])*200000.0) / ( ((double)(GROUP_VAL(range))) * ((double)(GROUP_VAL(velocity)))) ;
		  point_y [i] = height * (GROUP_VAL( amplitude[i-1] ) / 100000.0) ;
		  //printf("GROUP_VAL(position) is %d range %d  velocity %d \n", GROUP_VAL(position[i-1]),GROUP_VAL(range),GROUP_VAL(velocity));
		  //printf("x:%f y%f\n", point_x[i], point_y[i]) ;
		  //printf("GROUP_VAL(amplitude[i-1]) is %d\n", GROUP_VAL(amplitude[i-1]));
	}

	point_x[point_count+1] = width ;
	point_y[point_count+1] = point_y[point_count] ;

	for(i = point_count+1; i>0 ; i-- )
	{
		  point_y[i] = fabs(point_y[i] - point_y[0]) ;
	}
	point_y[0] = 0;

	for(i = 0; i<= point_count; i++)
	{
	      cairo_move_to(cr, 20 + point_x[i],   height - point_y[i] );
	      cairo_line_to(cr, 20 + point_x[i+1], height - point_y[i+1]);
	      cairo_stroke(cr);
	}

	cairo_set_source_rgb (cr, 255, 0, 0 );
	for(i = 1 ; i <= point_count ; i++)
	{
		cairo_rectangle(cr , 17 + point_x[i], height - point_y[i] - 3, 6 ,6 );
	    cairo_fill(cr);
	}


    cairo_stroke(cr);

}


void draw_dac_tcg_curve(cairo_t *cr,int width, int height)
{
	int i = GROUP_VAL(curve_pos);
	switch(i)
	{
		case 1:// DAC
			draw_dac_curve( cr, width, height);
			break;
		case 2:// Linear DAC
			draw_dac_curve( cr, width, height);
			break;
		case 3:// TCG
			draw_tcg_curve (cr , width , height );
			break;
		default:
			break;

	}

}

