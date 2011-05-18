
/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 *
 *
 */

#include "drawui.h"
#include "file_op.h"
#include "drawfb.h"
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
#include <netinet/in.h>
#include <arpa/inet.h>

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS((GtkObject*)(i))->x
#define YOFFSET  26

//_my_ip_set entry_ip;
_my_ip_set entry_ip;
_my_mask_set entry_mask;


GdkColor	color_black     = {0x0, 0x0, 0x0, 0x0};
GdkColor	color_black1    = {0x0, 0x0, 0x0, 0x0800};
GdkColor	color_white     = {0x0, 0xffff, 0xffff, 0xffff};
GdkColor	color_yellow    = {0x0, 0xffff, 0xffff, 0x0};
GdkColor	color_green     = {0x0, 0x0, 0xffff, 0x0};
GdkColor	color_blue      = {0x0, 0x0, 0x0, 0xffff};
GdkColor	color_red       = {0x0, 0x0, 0x0, 0xffff};
GdkColor	color_text_base = {0x0, 0x1300, 0x4900, 0x7600};
GdkColor	color_rule      = {0x0, 0xc300, 0xf000, 0x1d00};

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


enum
{
	LIST_ITEM = 0,
	N_COLUMNS
};

static char buffer[32];
static guchar dot_temp[800];
static gushort dot_temp1[FB_WIDTH*400];
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

void change_language(guint lang, DRAW_UI_P p);
void draw_menu1();
void draw_menu2(gint pa);
void draw_menu3(gint pa, gpointer p);
void init_ui(DRAW_UI_P p);				/* 初始化界面 */
void draw_area_all();

void save_config (GtkWidget *widget, GdkEventButton *event,	gpointer data);

void draw3_data0(DRAW_UI_P p);
void draw3_data1(DRAW_UI_P p);
void draw3_data2(DRAW_UI_P p);
void draw3_data3(DRAW_UI_P p);
void draw3_data4(DRAW_UI_P p);
void draw3_data5(DRAW_UI_P p);

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

/* 设置控件的背景图片 */
void update_widget_bg(GtkWidget *widget, const gchar *img_file)
{    
	GtkStyle *style;   
	GdkPixmap *pixmap; 
	cairo_surface_t *image;
	cairo_t *cr;

	gint width, height;  

	image = cairo_image_surface_create_from_png(img_file);
	width = cairo_image_surface_get_width(image);
	height = cairo_image_surface_get_height(image);
	pixmap = gdk_pixmap_new(NULL, width, height, TRUE_DEPTH); 

	cr = gdk_cairo_create (GDK_DRAWABLE (pixmap));
	cairo_set_source_surface(cr, image, 0, 0);
	cairo_paint(cr);

	style = gtk_style_copy(widget->style);  

	if (style->bg_pixmap[GTK_STATE_NORMAL])      
		g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);  

	style->bg_pixmap[GTK_STATE_NORMAL] = (pixmap);  
	/**
	  style->bg_pixmap[GTK_STATE_ACTIVE] = g_object_ref(pixmap);
	  style->bg_pixmap[GTK_STATE_PRELIGHT] = g_object_ref(pixmap);
	  style->bg_pixmap[GTK_STATE_SELECTED] = g_object_ref(pixmap);
	  style->bg_pixmap[GTK_STATE_INSENSITIVE] = g_object_ref(pixmap);
	 **/
	gtk_widget_set_style( widget, style);

	g_object_unref(style);
	cairo_destroy(cr);
	cairo_surface_destroy(image);
}

void main_menu_pop(guint action)
{
	int i;
	pp->x_pos = 0, pp->y_pos = 200;
	if (MENU_POP == action)	/*弹出主菜单*/
	{
		gtk_menu_popup ( GTK_MENU (pp->menu), NULL, NULL, 
				(GtkMenuPositionFunc)set_menu_position_tdh,
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
			//g_print("pp->menu3_poppos1 = %d\n",pp->menu3_poppos);
		}
		else
		{
			pp->menu3_poppos++;
			while(!gtk_widget_get_sensitive(pp->menu_item3[pp->menu3_poppos]))
				pp->menu3_poppos++;
			//g_print("pp->menu3_poppos2 = %d\n",pp->menu3_poppos);
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
				pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/Wizard_Menu.html";
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
		case 4:
		case 6:
		case 7:
		case 8:
		case 9:
			p->menu2_qty = 5;
			break;
		case 0:
			p->menu2_qty = 3;
			break;
		case 2:
		case 5:
			p->menu2_qty = 3;
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
				update_widget_bg (p->eventbox2[i], backpic[1]);
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
		update_widget_bg(p->eventbox2[p->pos1[p->pos]], backpic[2]);
	else 
		update_widget_bg(p->eventbox2[p->pos1[p->pos]], backpic[0]);

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

/* Probe 选择探头2个按键的处理 一个是确认 一个是取消 */
static void da_call_probe (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;

	if (GTK_RESPONSE_OK == response_id)  /* 确认 */
	{
		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection1), &model, &iter)) /* 选中探头型号时 */
		{
			gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
			if (GROUP_VAL(group_mode) == PA_SCAN )
				file_path = g_strdup_printf ("%s%s/%s", PA_PROBE_PATH, pp->p_type, value);
			else if (GROUP_VAL(group_mode) == UT_SCAN )
				file_path = g_strdup_printf ("%s%s/%s", UT_PROBE_PATH, pp->p_type, value);

			read_probe_file (file_path, &GROUP_VAL(probe));
			GROUP_VAL (frequency) = GROUP_VAL(probe.Frequency);				/* 频率 */
			if (!GROUP_VAL(pw_pos))
				GROUP_VAL(pulser_width) = GROUP_VAL(frequency) * 2.0; /* 改变脉冲宽度 */
			g_free (file_path);
			gtk_label_set_text (GTK_LABEL (pp->data3[3]), GROUP_VAL(probe.Model));

			gtk_widget_destroy (GTK_WIDGET (dialog));
			change_keypress_event (KEYPRESS_MAIN);
		}
		else
		{
			if (pp->tag == 0)/*探头大类选择Unknow时*/
			{
				//				strcpy(GROUP_VAL(probe.Model), " Unknown");

				if (GROUP_VAL(group_mode) == PA_SCAN )
					read_probe_file (PA_UNKNOWN_PROBE, &GROUP_VAL(probe));
				else if (GROUP_VAL(group_mode) == UT_SCAN )
					read_probe_file (UT_UNKNOWN_PROBE, &GROUP_VAL(probe));

				GROUP_VAL (frequency) = GROUP_VAL(probe.Frequency);				/* 频率 */
				if (!GROUP_VAL(pw_pos))
					GROUP_VAL(pulser_width) = GROUP_VAL(frequency) * 2.0; /* 改变脉冲宽度 */
				gtk_label_set_text (GTK_LABEL (pp->data3[3]), GROUP_VAL(probe.Model));

				gtk_widget_destroy (GTK_WIDGET (dialog));			
				change_keypress_event (KEYPRESS_MAIN);
			}
			else 
			{	
				if (gtk_tree_model_get_iter_first (model, &iter))/*用model中的第一项初始化iter*/
				{
					gtk_tree_selection_select_iter(pp->selection1, &iter);/*选择&iter指定的那项*/
					gtk_widget_grab_focus (pp->sw1);
				}
				else 
				{
					gtk_widget_destroy (GTK_WIDGET (dialog));
					change_keypress_event (KEYPRESS_MAIN);
				}
			}
		}
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
	{
		gtk_widget_destroy (GTK_WIDGET (dialog));
		change_keypress_event (KEYPRESS_MAIN);
	}

}

/* Wedge 选择楔块2个按键的处理 一个是确认 一个是取消 */
static void da_call_wedge (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value = NULL;
	gchar *file_path = NULL;

	if (GTK_RESPONSE_OK == response_id)  /* 确认 */
	{

		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection1), &model, &iter)) /* 选中楔块型号时 */
		{
			gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
			if (GROUP_VAL(group_mode) == PA_SCAN )
				file_path = g_strdup_printf ("%s%s/%s", PA_WEDGE_PATH, pp->p_type, value);
			else if (GROUP_VAL(group_mode) == UT_SCAN )
				file_path = g_strdup_printf ("%s%s/%s", UT_WEDGE_PATH, pp->p_type, value);

			read_wedge_file (file_path, &GROUP_VAL(wedge));
			g_free(file_path);

			gtk_label_set_text (GTK_LABEL (pp->data3[4]), GROUP_VAL(wedge.Model));
			gtk_widget_destroy (GTK_WIDGET (dialog));
		}
		else
		{
			if (pp->tag == 0)/* 楔块大类选择Unknow时*/
			{
				strcpy(GROUP_VAL(wedge.Model), " Unknown");
				gtk_label_set_text (GTK_LABEL (pp->data3[4]), GROUP_VAL(wedge.Model));
				gtk_widget_destroy (GTK_WIDGET (dialog));			
			}
			else
			{	
				if (gtk_tree_model_get_iter_first (model, &iter))/*用model中的第一项初始化iter*/
					gtk_tree_selection_select_iter(pp->selection1, &iter);/*选择&iter指定的那项*/
				else
					gtk_widget_destroy (GTK_WIDGET (dialog));
			}
		}
	}

	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
		gtk_widget_destroy (GTK_WIDGET (dialog));
}

/* 调色板 选择2个按键的处理 一个是确认 一个是取消 */
static void da_call_palette (GtkDialog *dialog, gint response_id, gpointer user_data)      
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;
	gint i;

	if (GTK_RESPONSE_OK == response_id)  /* 确认 */
	{
		if (gtk_tree_selection_get_selected(
					GTK_TREE_SELECTION(pp->selection), &model, &iter)) /* 选中探头型号时 */
		{
			gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
			file_path = g_strdup_printf ("%s%s", PALETTE_PATH, value);

			read_palette_file (file_path, TMP(t_special_col), TMP(t_color));  /*   */
			switch (GROUP_VAL(col_select_pos))
			{
				case 0:
					if (TMP(t_special_col[0]) != 0x12345678)
					{
						TMP(special_col_amp[0]) = COL_24_TO_16(TMP(t_special_col)[0]);
						TMP(special_col_amp[1]) = COL_24_TO_16(TMP(t_special_col)[1]);
						TMP(special_col_amp[2]) = COL_24_TO_16(TMP(t_special_col)[2]);
					}
					for (i = 0; i < 256; i ++)
						TMP(color_amp[i]) = COL_24_TO_16(TMP(t_color)[i]);
					break;
				case 1:
					if (TMP(t_special_col[0]) != 0x12345678)
					{
						TMP(special_col_tofd[0]) = COL_24_TO_16(TMP(t_special_col)[0]);
						TMP(special_col_tofd[1]) = COL_24_TO_16(TMP(t_special_col)[1]);
						TMP(special_col_tofd[2]) = COL_24_TO_16(TMP(t_special_col)[2]);
					}
					for (i = 0; i < 256; i ++)
						TMP(color_tofd[i]) = COL_24_TO_16(TMP(t_color)[i]);
					break;
				case 2:
					if (TMP(t_special_col[0]) != 0x12345678)
					{
						TMP(special_col_depth[0]) = COL_24_TO_16(TMP(t_special_col)[0]);
						TMP(special_col_depth[1]) = COL_24_TO_16(TMP(t_special_col)[1]);
						TMP(special_col_depth[2]) = COL_24_TO_16(TMP(t_special_col)[2]);
					}
					for (i = 0; i < 256; i ++)
						TMP(color_depth[i]) = COL_24_TO_16(TMP(t_color)[i]);
					break;
				default:break;
			}

			g_free (file_path);
			gtk_widget_destroy (GTK_WIDGET (dialog));
		}
	}
	else if (GTK_RESPONSE_CANCEL == response_id) /* 取消 */
		gtk_widget_destroy (GTK_WIDGET (dialog));

	//	for ( i = 0 ; i < 256 ; i ++)
	//	{
	//		g_print("%x,\n", TMP(color_amp[i]));
	//	}
}

static void da_call_warning (GtkDialog *dialog, 
		gint response_id, gpointer user_data)      
{
	gtk_widget_destroy (GTK_WIDGET (dialog));
	return ;
}

/*
 * 警告信息 
 * btn_qty: 表示几个按钮 1-2; 1就是 确认 2是确认和取消
 * warn_info: 警告或者提示信息
 * 1 
 *
 *
 */

void draw_warning(guint btn_qty, const gchar *warn_info)
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog = NULL;
	GtkWidget *vbox1;	/* 指向dialog的vbox */
	GtkWidget *label;

	switch (btn_qty)
	{
		case 1:
			dialog = gtk_dialog_new_with_buttons("Dialog_Warning", win,
					GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
					GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
			break;
		case 2:
			dialog = gtk_dialog_new_with_buttons("Dialog_Warning", win,
					GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
					GTK_STOCK_OK, GTK_RESPONSE_OK,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
			break;
		default:break;
	}
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/

	/*	gtk_widget_set_size_request (GTK_WIDGET (dialog), 300, 300);*/
	vbox1 = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	label = gtk_label_new(NULL);
	gtk_box_pack_start(GTK_BOX(vbox1), label, TRUE, TRUE, 5);
	gtk_label_set_markup(GTK_LABEL(label), warn_info);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_warning), NULL);

	gtk_widget_show_all(dialog);
}

/* 向列表里添加东西 */
void add_to_list(GtkWidget *list, const gchar *str, guint count)
{
	GtkListStore *store;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *str_data;
	guint tt = count, pos;
	store = GTK_LIST_STORE(gtk_tree_view_get_model
			(GTK_TREE_VIEW(list)));
	model = (gtk_tree_view_get_model
			(GTK_TREE_VIEW(list)));

	if (!count) 
	{
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
	}
	else
	{
		gtk_tree_model_get_iter_first (model, &iter);
		gtk_tree_model_get (model, &iter, LIST_ITEM, &str_data, -1);

		while ( count-- )
		{
			if (strcmp(str, str_data) < 0)	
			{
				pos = tt - count - 1;
				break;
			}
			else 
				pos = tt - count ;
			if (count)
			{
				g_free (str_data);
				gtk_tree_model_iter_next (model, &iter);
				gtk_tree_model_get (model, &iter, LIST_ITEM, &str_data, -1);
			}
		}
		g_free (str_data);

		gtk_list_store_insert (store, &iter, pos);
		gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
	}
}

/* 初始化列表 */
static void init_file_list (GtkWidget *list,
		GtkTreeSelection *selection,
		const gchar *path, guint file_type)
{ 
	DIR* dir = opendir(path);
	GtkListStore *store;
	struct dirent* enump = NULL;
	size_t name_len;
	GtkTreeIter iter;
	GtkTreeModel *model;
	guint i = 0;
	/*	gchar temp[128];*/

	if (NULL == dir)
	{
		g_print("Open directory failed:%s \n", path);
		return ;
	}

	store = GTK_LIST_STORE(gtk_tree_view_get_model
			(GTK_TREE_VIEW(list)));
	gtk_list_store_clear(store);

	while ((enump = readdir(dir)))
	{
		name_len = strlen(enump->d_name);
		if ((name_len == 1 && enump->d_name[0] == '.')
				|| (name_len == 2 && !strncmp(enump->d_name, "..", 2)))
			continue;


		/*  DT_DIR 文件夹    DT_REG 文件 */
		if (file_type == enump->d_type)
		{
			/* 去掉文件按后面的名字 */
			/* 
			   if (DT_REG == file_type)
			   {
			   memcpy(temp, enump->d_name, strlen(enump->d_name) - 4);
			   memset(temp + strlen(enump->d_name) -4, 0, 1);
			   add_to_list(list, temp, i);
			   }
			   else
			   */
			add_to_list(list, enump->d_name, i);
			i++;
		}
	}
	if (selection)
	{
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (list));	
		if (gtk_tree_model_get_iter_first (model, &iter))
		{
			gtk_tree_selection_select_iter(selection, &iter);
			pp->tag = 0;
		}
	}

	closedir(dir);
}

/* Probe 大类(左边treeview)选择的处理函数 */
static void on_changed_probe(GtkTreeSelection *selection, gpointer p) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value = NULL;
	gchar *file_path = NULL;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		if (strcmp(value, " Unknown") == 0)
			pp->tag = 0;
		else if (strcmp(value, "user") == 0)
			pp->tag =2;
		else
		{
			pp->tag =1;
			strcpy(pp->p_type, value);
		}
		if (GROUP_VAL(group_mode) == PA_SCAN )
			file_path = g_strdup_printf ("%s%s/", PA_PROBE_PATH , value);	
		else if (GROUP_VAL(group_mode) == UT_SCAN )
			file_path = g_strdup_printf ("%s%s/", UT_PROBE_PATH , value);	
		init_file_list (GTK_WIDGET (p), NULL, file_path, DT_REG);
		g_free(file_path);
		g_free(value);
	}
	gtk_tree_model_unref_node (model, &iter);

	switch (CFG(language))
	{
		case ENGLISH_:
			if (GROUP_VAL(group_mode) == PA_SCAN ) 
			{
				if (pp->tag == 0)
					gtk_label_set_text (GTK_LABEL (pp->label_probe), "Selecting the \"Unknown\" probe will require you \n to manually configure aditional settings.");
				else if (pp->tag == 2)
					gtk_label_set_text (GTK_LABEL (pp->label_probe), "This group contains all user-defined ultrasonic \n phased array probes.");
				else 
					gtk_label_set_text (GTK_LABEL (pp->label_probe), "Ultrasonic phased array probe family.");
			}
			else
				;
			break;
		default:break;
	}
}

/* Wedge 大类(左边treeview)选择的处理函数 */
static void on_changed_wedge(GtkTreeSelection *selection, gpointer label) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value = NULL;
	gchar *file_path = NULL;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		if (strcmp(value, " Unknown") == 0)
			pp->tag = 0;
		else if (strcmp(value, "user") == 0)
			pp->tag =2;
		else
			pp->tag =1;
		strcpy(pp->p_type, value);
		if (GROUP_VAL(group_mode) == PA_SCAN )
			file_path = g_strdup_printf ("%s%s/", PA_WEDGE_PATH , value);	
		else if (GROUP_VAL(group_mode) == UT_SCAN )
			file_path = g_strdup_printf ("%s%s/", UT_WEDGE_PATH , value);	
		init_file_list (GTK_WIDGET (label), NULL, file_path, DT_REG);
		printf("file_path = %s\n", file_path);
		g_free(file_path);
		g_free(value);
	}
	gtk_tree_model_unref_node (model, &iter);

	switch (CFG(language))
	{
		case ENGLISH_:
			if (GROUP_VAL(group_mode) == PA_SCAN ) 
			{
				if (pp->tag == 0)
					gtk_label_set_text (GTK_LABEL (pp->label_probe), "Selecting the \"Unknown\" wedge will require you \n to manually configure aditional settings.");
				else if (pp->tag == 2)
					gtk_label_set_text (GTK_LABEL (pp->label_probe), "This group contains all user-defined ultrasonic \n phased array probes.");
				else 
					gtk_label_set_text (GTK_LABEL (pp->label_probe), "Ultrasonic phased array probe family.");
			}
			else
				;
			break;
		default:break;
	}
}

/* 画调色板 */
static gboolean draw_palette(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gint i;
	gfloat color_r, color_g, color_b;

	cairo_t *cr;        //声明一支画笔
	cr=gdk_cairo_create(widget->window);//创建画笔
	cairo_set_line_width(cr, 2);

	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*设置画笔颜色为黑色*/
	cairo_move_to(cr,10,15);
	cairo_save(cr);
	cairo_set_font_size(cr, 20);		/*设置字体大小*/
	cairo_show_text(cr,"Palette");   	/*最顶端标签 Palette*/
	cairo_restore(cr);

	cairo_rectangle(cr,10,30,265,70);	/*special colors 外框*/
	cairo_move_to(cr,13,45);
	cairo_show_text(cr,"Special colors");	/*标签  Special colors*/

	if (TMP(t_special_col[0]) != 0x12345678)  
	{
		cairo_move_to(cr,13,60);
		cairo_show_text(cr,"No data");		/*标签  No data*/
		cairo_move_to(cr,13,75);
		cairo_show_text(cr,"No detection");	/*标签  No detection*/
		cairo_move_to(cr,13,90);
		cairo_show_text(cr,"No synchro");	/*标签  No synchro*/
	}
	cairo_rectangle(cr,10,102,265,306);	/*Main colors 外框*/
	cairo_move_to(cr,13,120);
	cairo_show_text(cr,"Main colors");	/*标签  Main colors*/
	cairo_move_to(cr,120,120);
	cairo_show_text(cr,"Quantity");		/*标签  Quantity*/
	cairo_move_to(cr,120,135);
	cairo_show_text(cr,"256");		/*标签  256 */
	cairo_stroke(cr);
	/*
	   cairo_rectangle(cr,20,140,245,256);	
	   cairo_stroke(cr);
	   */
	if (TMP(t_special_col[0]) != 0x12345678)  
	{
		for (i = 0; i < 3; i++)
		{
			color_r = (TMP(t_special_col[i]) >> 16) / 256.0;
			color_g = ((TMP(t_special_col[i]) & 0xff00) >> 8) / 256.0;
			color_b = ((TMP(t_special_col[i]) & 0xff)) /  256.0;
			//			color_r = ((TMP(t_special_col[i]) >> 16) >> 3) / 32.0;
			//			color_g = (((TMP(t_special_col[i]) & 0xff00) >> 8) >> 2) / 64.0;
			//			color_b = (((TMP(t_special_col[i]) & 0xff)) >> 3) /  32.0;
			cairo_set_source_rgba(cr, color_r, color_g, color_b, 1.0);
			cairo_rectangle(cr,85,50 + 15 * i,185,10);
			cairo_fill(cr);
			cairo_stroke(cr);
		}
	}

	for (i = 0; i < 256; i++)
	{
		color_r = (TMP(t_color[i]) >> 16) / 256.0;
		color_g = ((TMP(t_color[i]) & 0xff00) >> 8) / 256.0;
		color_b = ((TMP(t_color[i]) & 0xff)) /  256.0;
		cairo_set_source_rgba(cr,color_r,color_g,color_b,1.0);
		cairo_set_line_width(cr, 1);
		cairo_move_to(cr,20,140+i);
		cairo_line_to(cr,265,140+i);
		cairo_stroke(cr);
	}

	cairo_destroy(cr);//销毁画笔
	return TRUE;
}

/* Palette 大类(左边treeview)选择的处理函数 */
static void on_changed_palette(GtkTreeSelection *selection, gpointer drawing_area) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value = NULL;
	gchar *file_path = NULL;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		file_path = g_strdup_printf ("%s%s", PALETTE_PATH , value);	
		read_palette_file (file_path, TMP(t_special_col), TMP(t_color));  /*   */
		g_free(file_path);
		g_free(value);
	}

	gtk_tree_model_unref_node (model, &iter);

	gtk_widget_queue_draw(GTK_WIDGET (drawing_area));

}


/* 简单获取探头的信息 */
static gchar* get_probe_info(const gchar *file_path)
{
	PROBE p1;
	gchar *probe_info = NULL;
	read_probe_file (file_path, &p1);

	switch (CFG(language))
	{
		case ENGLISH_:
			if (GROUP_VAL(group_mode) == PA_SCAN)
				probe_info = g_strdup_printf ("Model:%s           Frequency:%.2fMHz\nElement Quantity:%d      Element Pitch:%.3f mm\nReference Point:-%.3f mm", 
						p1.Model, p1.Frequency/1000.0,p1.Elem_qty, p1.Pitch/1000.0, p1.Reference_Point/1000.0 );
			else if (GROUP_VAL(group_mode) == UT_SCAN)
				probe_info = g_strdup_printf ("Model:%s           Frequency:%.2fMHz\nElement_size:%.3f", 
						p1.Model, p1.Frequency/1000.0, p1.Pitch / 1000.0 );
			break;
		default:break;
	}

	return probe_info;
}

/* 简单获取楔块的信息 */
static gchar* get_wedge_info(const gchar *file_path)
{
	WEDGE w1;
	gchar *wedge_info = NULL;
	read_wedge_file (file_path, &w1);

	switch (CFG(language))
	{
		case ENGLISH_:
			if (GROUP_VAL(group_mode) == UT_SCAN)
				wedge_info = g_strdup_printf ("Model:%s           Angle:%.1f°\nWave Type:%s      Probe Delay:%.2f mm\nReference Point:%.3f mm",
						w1.Model, w1.Angle/10.0, (w1.Wave_type == 2) ? "Shear" : "Longitudinal", (w1.Probe_delay / 1000.0) * (GROUP_VAL(velocity) / 1000.0) / 200.0, w1.Ref_point / 1000.0 );
			else if (GROUP_VAL(group_mode) == PA_SCAN)
				wedge_info = g_strdup_printf ("Model:%s         Angle:%.1f°\nOrientation:%s      Height:%.3f mm\n Velocity:%.4f m/s   Primary Offset:%.3f mm\nSecondary Offset:%.3f mm",
						w1.Model, w1.Angle/10.0, (w1.Orientation == 1) ? "Normal" : "reversal", w1.Height/1000.0, w1.Velocity_PA / 1000.0, w1.Primary_offset/1000.0, w1.Secondary_offset/1000.0 );
			break;
		default:break;
	}
	return wedge_info;
}


/* Probe 具体型号选择的处理函数 */
static void on_changed1_probe(GtkTreeSelection *selection, gpointer label) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;
	gchar *probe_info = NULL;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		if (GROUP_VAL(group_mode) == PA_SCAN)
			file_path = g_strdup_printf ("%s%s/%s", PA_PROBE_PATH, pp->p_type, value);	
		else if (GROUP_VAL(group_mode) == UT_SCAN)
			file_path = g_strdup_printf ("%s%s/%s", UT_PROBE_PATH, pp->p_type, value);	
		g_free(value);
		probe_info = get_probe_info(file_path);
		gtk_label_set_text (GTK_LABEL (pp->label_probe), probe_info);
	}
	gtk_tree_model_unref_node (model, &iter);

	if (file_path)
		g_free(file_path);
	if (probe_info)
		g_free(probe_info);
}

/* Wedge 具体型号选择的处理函数 */
static void on_changed1_wedge(GtkTreeSelection *selection, gpointer label) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path = NULL;
	gchar *wedge_info = NULL;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(selection), &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		if (GROUP_VAL(group_mode) == PA_SCAN)
			file_path = g_strdup_printf ("%s%s/%s", PA_WEDGE_PATH, pp->p_type, value);	
		else if (GROUP_VAL(group_mode) == UT_SCAN)
			file_path = g_strdup_printf ("%s%s/%s", UT_WEDGE_PATH, pp->p_type, value);	
		g_free(value);
		wedge_info = get_wedge_info(file_path);
		gtk_label_set_text (GTK_LABEL (pp->label_probe), wedge_info);
	}
	gtk_tree_model_unref_node (model, &iter);

	if (file_path)
		g_free(file_path);
	if (wedge_info)
		g_free(wedge_info);
}



/* 0 记事本 备注 等 */
static void draw_remark ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */
	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *view;
	GtkTextBuffer *TextBuffer;
	//	GtkWidgetClass *widget_window_class1;
	const gchar *buf = (const gchar *)(CFG(remark_info));


	dialog = gtk_dialog_new_with_buttons("Dialog_Remark", win,
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	//	widget_window_class1 = GTK_WIDGET_GET_CLASS (((GtkObject*)(dialog))); 
	//	widget_window_class1->key_press_event = gtk_entry_digit_only_keypress_event; /* 指定哪些字符输入 */

	gtk_widget_set_size_request(GTK_WIDGET (dialog), 300, 300);
	vbox1 = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);
	sw = gtk_scrolled_window_new(NULL, NULL);

	gtk_widget_set_size_request(sw, 300, 300);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	gtk_box_pack_start(GTK_BOX(vbox1), sw, TRUE, TRUE, 5);
	view = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (sw), view);
	TextBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (view), GTK_WRAP_WORD_CHAR);
	gtk_text_buffer_set_text (TextBuffer, buf, -1);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_remark), (gpointer) (TextBuffer));

	gtk_widget_show_all(dialog);
}

/* 1 探头 */
static void draw_probe ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */
	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *sw1;		/* 第二个scroll 探头 楔块 聚焦法则 setup 等需要2个sw */
	GtkWidget *list;	/* 2个treeview 用来放置 探头大类和名称 楔块也一样 */
	GtkWidget *list1;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	GtkCellRenderer *renderer1;
	GtkTreeViewColumn *column1;
	GtkListStore *store1;
	GtkWidget *hpaned;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Probe", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 400, 370);

	vbox1 = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	sw = gtk_scrolled_window_new (NULL, NULL);
	sw1 = gtk_scrolled_window_new ( NULL, NULL);
	pp->sw11 = sw;
	pp->sw1 = sw1;

	/* 目录名字 探头大类 */
	gtk_widget_set_size_request (sw, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);
	/* 探头名字 小类 */
	gtk_widget_set_size_request (sw1, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	/* 2个treeview 放置探头大类和名字 */
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	list1 = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list1), FALSE);
	/* 初始化list */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);
	/* 初始化list1 */
	renderer1 = gtk_cell_renderer_text_new();
	column1 = gtk_tree_view_column_new_with_attributes("List Items",
			renderer1, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list1), column1);

	store1 = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list1), 
			GTK_TREE_MODEL(store1));
	g_object_unref(store1);
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add(GTK_CONTAINER (sw), list);
	gtk_container_add(GTK_CONTAINER (sw1), list1);

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	pp->selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(list1));

	gtk_box_pack_start (GTK_BOX(vbox1), hpaned, FALSE, FALSE, 5);
	pp->label_probe = gtk_label_new("\n");
	gtk_box_pack_start (GTK_BOX(vbox1), pp->label_probe, TRUE, TRUE, 5);

	if (GROUP_VAL(group_mode) == UT_SCAN)
		init_file_list (list, pp->selection, UT_PROBE_PATH ,DT_DIR);
	else if (GROUP_VAL(group_mode) == PA_SCAN)
		init_file_list (list, pp->selection, PA_PROBE_PATH ,DT_DIR);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_probe), NULL);/*确定 or 取消*/

	g_signal_connect (G_OBJECT (pp->selection), "changed", 
			G_CALLBACK(on_changed_probe), (gpointer) (list1));

	g_signal_connect (G_OBJECT (pp->selection1), "changed", 
			G_CALLBACK(on_changed1_probe), (gpointer) NULL);

	gtk_widget_show_all(dialog);
}

/* 2 楔块 */
static void draw_wedge ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */

	GtkWidget *sw;		/* 第一个scroll 备注只要一个sw */
	GtkWidget *sw1;		/* 第二个scroll 探头 楔块 聚焦法则 setup 等需要2个sw */

	GtkWidget *list;	/* 2个treeview 用来放置 探头大类和名称 楔块也一样 */
	GtkWidget *list1;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	GtkCellRenderer *renderer1;
	GtkTreeViewColumn *column1;
	GtkListStore *store1;

	GtkWidget *hpaned;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Wedge", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 400, 370);

	vbox1 = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	sw = gtk_scrolled_window_new (NULL, NULL);
	sw1 = gtk_scrolled_window_new ( NULL, NULL);
	/* 目录名字 楔块大类 */
	gtk_widget_set_size_request (sw, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	/* 楔块名字 小类 */
	gtk_widget_set_size_request (sw1, 200, 230);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	/* 2个treeview 放置楔块大类和名字 */
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	list1 = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list1), FALSE);
	/* 初始化list */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);
	/* 初始化list1 */
	renderer1 = gtk_cell_renderer_text_new();
	column1 = gtk_tree_view_column_new_with_attributes("List Items",
			renderer1, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list1), column1);

	store1 = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list1), 
			GTK_TREE_MODEL(store1));
	g_object_unref(store1);
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);		
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add(GTK_CONTAINER (sw), list);
	gtk_container_add(GTK_CONTAINER (sw1), list1);

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	pp->selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(list1));

	gtk_box_pack_start(GTK_BOX(vbox1), hpaned, FALSE, FALSE, 5);
	pp->label_probe = gtk_label_new("\n");
	gtk_box_pack_start(GTK_BOX(vbox1), pp->label_probe, TRUE, TRUE, 5);

	if (GROUP_VAL(group_mode) == UT_SCAN)
		init_file_list (list, pp->selection, UT_WEDGE_PATH ,DT_DIR);
	else if (GROUP_VAL(group_mode) == PA_SCAN)
		init_file_list (list, pp->selection, PA_WEDGE_PATH ,DT_DIR);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_wedge), NULL);/*确定 or 取消*/

	g_signal_connect (G_OBJECT (pp->selection), "changed", 
			G_CALLBACK(on_changed_wedge), (gpointer) (list1));/*使用"changed"信号来与用户的选择信号进行关联起来*/

	g_signal_connect (G_OBJECT (pp->selection1), "changed", 
			G_CALLBACK(on_changed1_wedge), (gpointer) NULL);

	gtk_widget_show_all(dialog);

}

//作者 geniikid
//日期 2011-3-28
//函数 selection_file_type
//参数 char *dir_path
//参数 char *file_type
void selection_file_type(GtkWidget *list,char *dir_path,char *file_type)
{
	DIR *dir;      
	int i;
	struct dirent* enump = NULL;
	char *pos;
	int name_len;
	int suffix_len;

	assert(dir_path != NULL);
	assert(file_type != NULL);

	i = 0;    

	dir = opendir(dir_path);

	if (dir == NULL)
	{
		g_printf("Open directory failed:%s \n",dir_path);
		return ;
	}

	suffix_len = strlen(file_type);

	while((enump = readdir(dir)))
	{
		name_len = strlen(enump->d_name);

		if ((name_len == 1 && enump->d_name[0] == '.')|| (name_len == 2 && !strncmp(enump->d_name, "..", 2)))
			continue;

		if ( name_len <= suffix_len )
			continue;

		pos = strstr(enump->d_name,file_type);

		if (pos == NULL)
			continue;
		else
		{    
			add_to_list(list, enump->d_name, i);
			i++;
		}

	}

	closedir(dir);
}

//作者 geniikid
//日期 2011-4-1
//函数 open_config_file
//参数 char *path
int open_config_file(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	FILE *fp;

	int temp;    

	char file_name[1024] = "";

	int value; 
	char *value_name;

	GtkTreeIter source_iter;

	GtkTreeModel *source_model;
	GtkTreeSelection *source_selection; 

	GtkWidget *source_list = (GtkWidget *)data;

	char *source_file;

	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));

	source_model=gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));

	value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");

	while(value)   
	{
		if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
		{

			gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);

			g_printf("value_name = %s\n",value_name);

			memset(file_name,0,sizeof(file_name));

			source_file = Get_Source_File_Path();    

			strcpy(file_name,source_file);

			strcat(file_name,value_name);

			fp = fopen(file_name, "r+");

			if (fp < 0)
				return -1;

			temp = fread(pp->p_config, sizeof(CONFIG),1,fp);

			fclose(fp);

			gtk_tree_selection_unselect_iter(source_selection,&source_iter);

			g_free(value_name);

			g_printf("selection \n");

		}
		else
		{
			value = gtk_tree_model_iter_next(source_model,&source_iter);
		} 

	}

	return 0; 
}

/* 打开文件 */
static void draw_file_open_main()
{ 
	GtkWindow *window = GTK_WINDOW (pp->window);

	GtkWidget *dialog;

	GtkWidget *hbox_first;

	GtkWidget *hbox;

	GtkWidget *hbox_2;

	GtkWidget *vbox_2_1;

	GtkWidget *vbox_2_1_1[7];
	GtkWidget *label_2_1_1[7];
	char *char_2_1_1[7] = {"","","Close","File Type","listed files","",""};

	GtkWidget *hbox_1;

	GtkWidget *vbox_1_1;

	GtkWidget *vbox_1_1_1;

	GtkWidget *hbox_1_1_1_1;

	GtkWidget *sw_1_1_1_1_1;

	GtkWidget *sw_1_1_1_1_2;

	GtkWidget *vbox_1_1_2;

	GtkWidget *hbox_1_1_2_1;

	GtkWidget *hbox_1_1_2_1_1[6];
	GtkWidget *label_1_1_2_1_1[6];
	char *char_1_1_2_1_1[6] = {"Open","","","","",""};

	int i;

	GtkWidget *source_list;

	GtkTreeSelection *source_selection; 

	GtkTreeModel *source_model;

	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Wedge", window,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg(GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/

	hbox_first = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	//源文件列表
	source_list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(source_list), FALSE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",renderer, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(source_list), column);

	store = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(source_list), GTK_TREE_MODEL(store));

	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));

	source_model=gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));

	hbox = gtk_hbox_new(FALSE,0);

	hbox_2 = gtk_hbox_new(FALSE,0);

	vbox_2_1 = gtk_vbox_new(FALSE,0); 

	for(i=0;i<7;i++)
	{
		if ( (i>=2) && (i<=4))
		{
			vbox_2_1_1[i] = gtk_event_box_new();
		}
		else
		{
			vbox_2_1_1[i] = gtk_vbox_new(FALSE,0);
		}
		gtk_widget_set_size_request(GTK_WIDGET(vbox_2_1_1[i]),114,85);
		update_widget_bg(vbox_2_1_1[i], backpic[1]);

		label_2_1_1[i] = gtk_label_new(char_2_1_1[i]);
		gtk_widget_modify_fg (label_2_1_1[i], GTK_STATE_NORMAL, &color_black);
		gtk_label_set_justify(GTK_LABEL(label_2_1_1[i]), GTK_JUSTIFY_CENTER);

		gtk_container_add(GTK_CONTAINER(vbox_2_1_1[i]),label_2_1_1[i]);
		gtk_box_pack_start(GTK_BOX(vbox_2_1),vbox_2_1_1[i],FALSE,FALSE,0);   
	}

	hbox_1 = gtk_hbox_new(FALSE,0);

	vbox_1_1 = gtk_vbox_new(FALSE,0);

	vbox_1_1_1 = gtk_vbox_new(FALSE,0);

	hbox_1_1_1_1 = gtk_hbox_new(FALSE,0);

	vbox_1_1_2 = gtk_vbox_new(FALSE,0);

	hbox_1_1_2_1 = gtk_hbox_new(FALSE,0);

	//
	sw_1_1_1_1_1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_1_1_1_1_1, 343, 515);

	//
	sw_1_1_1_1_2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_1_1_1_1_2, 343, 515);



	for(i=0;i<6;i++)
	{
		if (i==0)
		{ 
			hbox_1_1_2_1_1[i] = gtk_event_box_new();
		}
		else
		{
			hbox_1_1_2_1_1[i] = gtk_hbox_new(FALSE,0);
		}
		gtk_widget_set_size_request(GTK_WIDGET(hbox_1_1_2_1_1[i]),114,85);
		update_widget_bg(hbox_1_1_2_1_1[i], backpic[1]);
		label_1_1_2_1_1[i] = gtk_label_new(char_1_1_2_1_1[i]);
		gtk_widget_modify_fg (label_1_1_2_1_1[i], GTK_STATE_NORMAL, &color_black);
		gtk_label_set_justify(GTK_LABEL(label_1_1_2_1_1[i]), GTK_JUSTIFY_CENTER);
		gtk_box_pack_start(GTK_BOX(hbox_1_1_2_1),hbox_1_1_2_1_1[i],FALSE,FALSE,0);
		gtk_container_add(GTK_CONTAINER(hbox_1_1_2_1_1[i]), label_1_1_2_1_1[i]);
	}



	Set_Source_File_Path(USER_CFG_PATH);

	selection_file_type(source_list, USER_CFG_PATH,	".cfg");

	gtk_container_add(GTK_CONTAINER(sw_1_1_1_1_1), source_list);

	//    
	pp->web_view_tmp = WEBKIT_WEB_VIEW (webkit_web_view_new());
	//web的编码方式
	webkit_web_view_set_custom_encoding (pp->web_view_tmp, "UTF-8");

	gtk_container_add(GTK_CONTAINER(sw_1_1_1_1_2),GTK_WIDGET(pp->web_view_tmp));

	gtk_box_pack_start(GTK_BOX(hbox_1_1_1_1),sw_1_1_1_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox_1_1_1_1),sw_1_1_1_1_2,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1_1),hbox_1_1_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1),vbox_1_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1_2),hbox_1_1_2_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1),vbox_1_1_2,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox_1),vbox_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox_2),vbox_2_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_2,FALSE,FALSE,0);

	gtk_container_add(GTK_CONTAINER(hbox_first), hbox);

	g_signal_connect(G_OBJECT (vbox_2_1_1[2]), "button-press-event",G_CALLBACK(dialog_destroy), dialog);

	g_signal_connect(G_OBJECT (hbox_1_1_2_1_1[0]), "button-press-event",G_CALLBACK(open_config_file), (gpointer)source_list);

	g_signal_connect (G_OBJECT (source_selection), "changed", G_CALLBACK(on_changed_config_file), (gpointer)source_list);

	gtk_widget_show_all(dialog);

	gtk_widget_hide (GTK_DIALOG(dialog)->action_area);

}


int on_changed_config_file(GtkTreeSelection *selection,	gpointer       data)
{
	FILE *fp;

	char *file_path = USER_CFG_PATH;

	char file_name[256];

	char *preview_file_name = "/tmp/zsh.htm";

	GtkTreeIter source_iter;

	GtkTreeSelection *source_selection; 

	GtkTreeModel *source_model;

	GtkWidget *source_list;

	int value;

	char *value_name;

	int temp;

	source_list = (GtkWidget *)data;

	source_model = gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));

	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));

	value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");

	while(value)   
	{
		if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
		{

			gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);

			g_printf("value_name = %s\n",value_name);

			memset(file_name,0,sizeof(file_name));    

			strcpy(file_name,file_path);

			strcat(file_name,value_name);

			fp = fopen(file_name, "r+");

			if (fp < 0)
				return -1;

			temp = fread(pp->p_config, sizeof(CONFIG),1,fp);

			fclose(fp);

			fp = fopen(preview_file_name,"w+");

			fprintf(fp,"<html>\n");

			fprintf(fp,"<body>\n");

			fprintf(fp,"<p>\n");

			fprintf(fp,"%s\n",value_name);

			fprintf(fp,"</p>\n");

			fprintf(fp,"</body>\n");

			fprintf(fp,"</html>\n");

			fclose(fp);

			memset(file_name,0,sizeof(file_name));

			strcpy(file_name,"file://");

			strcat(file_name,preview_file_name);

			webkit_web_view_load_uri (pp->web_view_tmp, file_name);  

			value = gtk_tree_model_iter_next(source_model,&source_iter);            

			g_free(value_name);

			g_printf("selection \n");

		}
		else
		{
			value = gtk_tree_model_iter_next(source_model,&source_iter);
		} 

	}

	g_printf("on_changed_config_file\n");

	return 0;
}

int save_config_file(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
	char *dir_path = USER_CFG_PATH;
	char *file_name = "tt.cfg";
	char path[256];
	FILE *fp;
	int temp;

	strcpy(path,dir_path);

	strcat(path,file_name);

	fp = fopen(path, "wb+");

	if (fp < 0)
		return -1;

	temp = fwrite(pp->p_config, sizeof(CONFIG),1,fp);

	fclose(fp);

	return 0;
}

static void draw_save_setup_as()
{
	GtkWindow *window = GTK_WINDOW (pp->window);

	GtkWidget *dialog;

	GtkWidget *hbox_first;

	GtkWidget *hbox;

	GtkWidget *hbox_2;

	GtkWidget *vbox_2_1;

	GtkWidget *vbox_2_1_1[7];
	GtkWidget *label_2_1_1[7];
	char *char_2_1_1[7] = {"","","Close","","","",""};

	GtkWidget *hbox_1;

	GtkWidget *vbox_1_1;

	GtkWidget *vbox_1_1_1;

	GtkWidget *hbox_1_1_1_1;

	GtkWidget *sw_1_1_1_1_1;

	GtkWidget *sw_1_1_1_1_2;

	GtkWidget *vbox_1_1_2;

	GtkWidget *hbox_1_1_2_1;

	//    GtkWidget *hbox_1_1_2_1_1[6];
	//   GtkWidget *label_1_1_2_1_1[6];
	//  char *char_1_1_2_1_1[6] = {"","Save","File name","Setup lock","",""};

	//	GtkWidget *drawing_area;
	GtkWidget *eventbox_save;
	GtkWidget *label[3];
	GtkWidget *vbox_filename;
	GtkWidget *eventbox_filename;
	//	GtkWidget *eventbox_entry;
	GtkWidget *vbox_lock;
	GtkWidget *eventbox_lock1;

	GtkWidget *entry_save_file_name;

	int i;

	GtkWidget *source_list;

	GtkTreeSelection *source_selection; 

	GtkTreeModel *source_model;

	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;



	dialog = gtk_dialog_new_with_buttons ("Dialog_Wedge", window,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg(GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/

	hbox_first = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	//源文件列表
	source_list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(source_list), FALSE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",renderer, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(source_list), column);

	store = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(source_list), GTK_TREE_MODEL(store));

	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));

	source_model=gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));

	hbox = gtk_hbox_new(FALSE,0);

	hbox_2 = gtk_hbox_new(FALSE,0);

	vbox_2_1 = gtk_vbox_new(FALSE,0); 

	for(i=0;i<7;i++)
	{
		if ( i == 2 )
		{
			vbox_2_1_1[i] = gtk_event_box_new();
		}
		else
		{
			vbox_2_1_1[i] = gtk_vbox_new(FALSE,0);
		}
		gtk_widget_set_size_request(GTK_WIDGET(vbox_2_1_1[i]),114,85);
		update_widget_bg(vbox_2_1_1[i], backpic[1]);

		label_2_1_1[i] = gtk_label_new(char_2_1_1[i]);
		gtk_widget_modify_fg (label_2_1_1[i], GTK_STATE_NORMAL, &color_black);
		gtk_label_set_justify(GTK_LABEL(label_2_1_1[i]), GTK_JUSTIFY_CENTER);

		gtk_container_add(GTK_CONTAINER(vbox_2_1_1[i]),label_2_1_1[i]);
		gtk_box_pack_start(GTK_BOX(vbox_2_1),vbox_2_1_1[i],FALSE,FALSE,0);   
	}

	hbox_1 = gtk_hbox_new(FALSE,0);

	vbox_1_1 = gtk_vbox_new(FALSE,0);

	vbox_1_1_1 = gtk_vbox_new(FALSE,0);

	hbox_1_1_1_1 = gtk_hbox_new(FALSE,0);

	vbox_1_1_2 = gtk_vbox_new(FALSE,0);

	hbox_1_1_2_1 = gtk_hbox_new(FALSE,0);

	//
	sw_1_1_1_1_1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_1_1_1_1_1, 343, 515);

	//
	sw_1_1_1_1_2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_1_1_1_1_2, 343, 515);



	/*   for(i=0;i<6;i++)
		 {
		 if ( (i==1) || (i==3)  )
		 { 
		 hbox_1_1_2_1_1[i] = gtk_event_box_new();
		 }
		 else
		 {
		 hbox_1_1_2_1_1[i] = gtk_vbox_new(FALSE,0);
		 }
		 gtk_widget_set_size_request(GTK_WIDGET(hbox_1_1_2_1_1[i]),114,85);
		 update_widget_bg(hbox_1_1_2_1_1[i], backpic[1]);
		 label_1_1_2_1_1[i] = gtk_label_new(char_1_1_2_1_1[i]);
		 gtk_widget_modify_fg (label_1_1_2_1_1[i], GTK_STATE_NORMAL, &color_white);
		 gtk_label_set_justify(GTK_LABEL(label_1_1_2_1_1[i]), GTK_JUSTIFY_CENTER);
		 gtk_box_pack_start(GTK_BOX(hbox_1_1_2_1),hbox_1_1_2_1_1[i],FALSE,FALSE,0); 
		 gtk_container_add(GTK_CONTAINER(hbox_1_1_2_1_1[i]), label_1_1_2_1_1[i]);

		 }*/

	/*	drawing_area = gtk_drawing_area_new();
		gtk_widget_set_size_request (GTK_WIDGET(drawing_area), 114, 85); 
		gtk_widget_modify_bg(GTK_WIDGET(drawing_area), GTK_STATE_NORMAL, &color_black);
		gtk_box_pack_start (GTK_BOX (hbox_1_1_2_1), drawing_area, FALSE, FALSE, 0);*/

	eventbox_save = gtk_event_box_new();
	gtk_widget_set_size_request (GTK_WIDGET(eventbox_save), 114, 85);
	update_widget_bg(eventbox_save, backpic[1]);
	label[0] = gtk_label_new("Save");
	gtk_container_add(GTK_CONTAINER(eventbox_save),label[0]);
	gtk_box_pack_start (GTK_BOX (hbox_1_1_2_1), eventbox_save, FALSE, FALSE, 0);

	vbox_filename = gtk_vbox_new(FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(vbox_filename), 342, 85);
	eventbox_filename = gtk_event_box_new();
	gtk_widget_set_size_request (GTK_WIDGET(eventbox_filename), 342, 57);
	update_widget_bg(eventbox_filename, backpic[1]);
	label[1] = gtk_label_new("File name");
	gtk_container_add(GTK_CONTAINER(eventbox_filename),label[1]);
	gtk_box_pack_start (GTK_BOX (vbox_filename), eventbox_filename, FALSE, FALSE, 0);
	entry_save_file_name = gtk_entry_new();
	gtk_widget_set_size_request (GTK_WIDGET(entry_save_file_name), 342, 28);
	gtk_box_pack_start(GTK_BOX(vbox_filename),entry_save_file_name,FALSE,FALSE,0);
	gtk_widget_modify_base(GTK_WIDGET (entry_save_file_name), GTK_STATE_NORMAL, &color_text_base);
	gtk_widget_modify_text(GTK_WIDGET (entry_save_file_name), GTK_STATE_NORMAL, &color_yellow);

	gtk_box_pack_start (GTK_BOX (hbox_1_1_2_1), vbox_filename, FALSE, FALSE, 0);

	vbox_lock = gtk_vbox_new(FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(vbox_lock), 114, 85);
	eventbox_lock1 = gtk_event_box_new();
	gtk_widget_set_size_request (GTK_WIDGET(eventbox_lock1), 114, 85);
	update_widget_bg(eventbox_lock1, backpic[1]);
	label[2] = gtk_label_new("Setup Lock");
	gtk_container_add(GTK_CONTAINER(eventbox_lock1),label[2]);
	gtk_box_pack_start (GTK_BOX (vbox_lock), eventbox_lock1, FALSE, FALSE, 0);
	/*	eventbox_lock2 = gtk_event_box_new();
		gtk_widget_set_size_request (GTK_WIDGET(eventbox_lock2), 114, 28);
		gtk_box_pack_start (GTK_BOX (vbox_lock), eventbox_lock2, FALSE, FALSE, 0);*/
	gtk_box_pack_start (GTK_BOX (hbox_1_1_2_1), vbox_lock, FALSE, FALSE, 0);

	Set_Source_File_Path(USER_CFG_PATH);

	selection_file_type(source_list, USER_CFG_PATH,	".cfg");

	gtk_container_add(GTK_CONTAINER(sw_1_1_1_1_1), source_list);

	source_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));

	//
	pp->web_view_tmp = WEBKIT_WEB_VIEW (webkit_web_view_new());
	//web的编码方式
	webkit_web_view_set_custom_encoding (pp->web_view_tmp, "UTF-8");

	gtk_container_add(GTK_CONTAINER(sw_1_1_1_1_2),GTK_WIDGET(pp->web_view_tmp));

	gtk_box_pack_start(GTK_BOX(hbox_1_1_1_1),sw_1_1_1_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox_1_1_1_1),sw_1_1_1_1_2,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1_1),hbox_1_1_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1),vbox_1_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1_2),hbox_1_1_2_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1),vbox_1_1_2,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox_1),vbox_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox_2),vbox_2_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_2,FALSE,FALSE,0);

	gtk_container_add(GTK_CONTAINER(hbox_first), hbox);

	g_signal_connect(G_OBJECT (vbox_2_1_1[2]), "button-press-event",G_CALLBACK(dialog_destroy), dialog);

	g_signal_connect(G_OBJECT (eventbox_save), "button-press-event",G_CALLBACK(save_config_file), (gpointer)source_list);

	g_signal_connect (G_OBJECT (source_selection), "changed", G_CALLBACK(on_changed_config_file), (gpointer)source_list);

	gtk_widget_show_all(dialog);

	gtk_widget_hide (GTK_DIALOG(dialog)->action_area);

}

/* 系统信息的显示 */
static void draw_system_info ()
{ 
	GtkWindow *window = GTK_WINDOW (pp->window);

	GtkWidget *dialog;

	WebKitWebView* web_view;

	gchar *file_path = "file:///home/tt/TT/system_info.htm";
	//    gchar *file_path = "file:///home/geniikid/tmp/system_info.htm";

	GtkWidget *sw;

	GtkWidget *vbox_first;

	GtkWidget *vbox;

	GtkWidget *vbox_1;

	GtkWidget *vbox_2;

	GtkWidget *hbox_2_1;

	GtkWidget *hbox_2_1_1[7];

	GtkWidget *label_2_1_1[7];

	char *char_2_1_1[7] = {"","","","","Print","Save and close","Close"};

	int i;

	//window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	//gtk_widget_set_size_request(GTK_WIDGET (window), 800, 600);

	dialog = gtk_dialog_new_with_buttons ("Dialog_Wedge", window,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/

	vbox_first = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg(GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/

	vbox = gtk_vbox_new(FALSE,0); 

	vbox_1 = gtk_vbox_new(FALSE,0);

	vbox_2 = gtk_vbox_new(FALSE,0);

	hbox_2_1 = gtk_hbox_new(FALSE,0);

	for(i=0;i<7;i++)
	{
		hbox_2_1_1[i] = gtk_event_box_new();
		gtk_widget_set_size_request(GTK_WIDGET(hbox_2_1_1[i]),114,85);
		update_widget_bg(hbox_2_1_1[i], backpic[1]);
		label_2_1_1[i] = gtk_label_new(char_2_1_1[i]);
		gtk_widget_modify_fg (label_2_1_1[i], GTK_STATE_NORMAL, &color_black);
		gtk_label_set_justify(GTK_LABEL(label_2_1_1[i]), GTK_JUSTIFY_CENTER);
		gtk_container_add(GTK_CONTAINER(hbox_2_1_1[i]), label_2_1_1[i]);
		gtk_box_pack_start(GTK_BOX(hbox_2_1),hbox_2_1_1[i], FALSE, FALSE, 0);
	}

	sw = gtk_scrolled_window_new (NULL, NULL);

	gtk_widget_set_size_request(GTK_WIDGET(sw),800,515);

	//
	web_view = WEBKIT_WEB_VIEW (webkit_web_view_new());
	//web的编码方式
	webkit_web_view_set_custom_encoding (web_view, "UTF-8");    
	//
	webkit_web_view_load_uri (web_view, file_path);

	//g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(G_OBJECT (hbox_2_1_1[6]), "button-press-event",G_CALLBACK(dialog_destroy), dialog);

	gtk_container_add(GTK_CONTAINER(sw),(GtkWidget *)web_view);

	gtk_box_pack_start(GTK_BOX(vbox_1),sw, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),vbox_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_2),hbox_2_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),vbox_2, FALSE, FALSE, 0);

	//gtk_container_add(GTK_CONTAINER(window), vbox);

	gtk_box_pack_start(GTK_BOX(vbox_first), vbox,FALSE,FALSE,0);

	//gtk_widget_show_all(window);

	gtk_widget_show_all(dialog);

	return ;
}

/* 11 调色板 */
static void draw_color_palette ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;
	GtkWidget *vbox1;	/* 指向dialog的vbox */

	GtkWidget *sw;		/* 第一个scroll */
	GtkWidget *drawarea;

	GtkWidget *hbox_sw;	/* 用来装调色板信息*/

	GtkWidget *list;	/* 1个treeview 用来放置调色板名称 */
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	GtkWidget *hpaned;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Palette", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 480, 460);

	vbox1 = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	sw = gtk_scrolled_window_new (NULL, NULL);
	drawarea = gtk_drawing_area_new ();
	/* 调色板名字 */
	gtk_widget_set_size_request (sw, 165, 410);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	/* treeview 放置调色板名字 */
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	/* 初始化list */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);

	hbox_sw = gtk_hbox_new(FALSE,0);
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), drawarea);		
	gtk_container_add(GTK_CONTAINER (sw), list);

	gtk_box_pack_start(GTK_BOX(vbox1), hpaned, FALSE, FALSE, 5);
	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	init_file_list (list, pp->selection, PALETTE_PATH, DT_REG);

	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_palette), NULL);

	g_signal_connect (G_OBJECT(pp->selection), "changed",
			G_CALLBACK(on_changed_palette), (gpointer)(drawarea));

	g_signal_connect (G_OBJECT (drawarea), "expose_event",
			G_CALLBACK (draw_palette), NULL);

	gtk_widget_show_all(dialog);
}

/* 12 文件管理 */
static void draw_file_manage ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;

	GtkWidget *hbox_first;

	GtkWidget *hbox;  

	GtkWidget *hbox_1;

	GtkWidget *vbox_1_1;

	GtkWidget *vbox_1_1_1;

	GtkWidget *hbox_1_1_1_1;

	GtkWidget *vbox_1_1_2;

	GtkWidget *hbox_1_1_2_1;

	GtkWidget *hbox_1_1_2_1_1[6];
	GtkWidget *label_1_1_2_1_1[6];
	char *char_1_1_2_1_1[6] = {"Select","Select All","Copy","Move","Delect","Rename"};

	GtkWidget *hbox_2;

	GtkWidget *vbox_2_1;
	GtkWidget *vbox_2_1_1[7];
	GtkWidget *label_2_1_1[7];
	char *char_2_1_1[7] = {"","","Close","","","",""};

	GtkWidget *sw_1_1_1_1_1;
	GtkWidget *sw_1_1_1_1_2;

	GtkWidget *source_list;
	GtkWidget *target_list;

	GtkTreeSelection *source_selection; 
	GtkTreeSelection *target_selection; 

	GtkTreeModel *source_model;
	GtkTreeModel *target_model;

	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	MY_SIGNAL my_signal;

	int i;

	dialog = gtk_dialog_new_with_buttons ("Dialog_Wedge", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg(GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/

	hbox_first = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	hbox = gtk_hbox_new(FALSE,0);

	hbox_1 = gtk_hbox_new(FALSE,0);

	vbox_1_1 = gtk_vbox_new(FALSE,0);

	vbox_1_1_1 = gtk_vbox_new(FALSE,0);

	hbox_1_1_1_1 = gtk_hbox_new(FALSE,0);

	vbox_1_1_2 = gtk_vbox_new(FALSE,0);

	hbox_1_1_2_1 = gtk_hbox_new(FALSE,0);

	hbox_2 = gtk_hbox_new(FALSE,0);

	vbox_2_1 = gtk_vbox_new(FALSE,0);

	//源文件列表
	source_list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(source_list), FALSE);

	//目标文件列表
	target_list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(target_list),FALSE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(source_list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(source_list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(target_list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(target_list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);

	//源文件窗口，带滚动条
	sw_1_1_1_1_1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_1_1_1_1_1, 343, 515);

	//目标文件窗口，带滚动条
	sw_1_1_1_1_2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_1_1_1_1_2, 343, 515);

	//源文件路径
	Set_Source_File_Path("/");
	//目标文件路径    
	Set_Target_File_Path("/");
	//

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(source_list));
	pp->selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(target_list));

	source_selection = pp->selection;
	target_selection = pp->selection1;

	source_model=gtk_tree_view_get_model(GTK_TREE_VIEW(source_list));
	target_model=gtk_tree_view_get_model(GTK_TREE_VIEW(target_list));

	//源文件，文件视图
	init_file_list (source_list, pp->selection, "/" , DT_DIR);
	//目标文件，文件视图
	init_file_list (target_list, pp->selection1, "/" , DT_DIR);

	gtk_tree_selection_set_mode(source_selection,GTK_SELECTION_MULTIPLE);

	gtk_tree_selection_set_mode(target_selection,GTK_SELECTION_MULTIPLE);

	for(i=0;i<6;i++)
	{
		hbox_1_1_2_1_1[i] = gtk_event_box_new();
		gtk_widget_set_size_request(GTK_WIDGET(hbox_1_1_2_1_1[i]),114,85);
		update_widget_bg(hbox_1_1_2_1_1[i], backpic[1]);
		label_1_1_2_1_1[i] = gtk_label_new(char_1_1_2_1_1[i]);
		gtk_widget_modify_fg (label_1_1_2_1_1[i], GTK_STATE_NORMAL, &color_black);
		gtk_label_set_justify(GTK_LABEL(label_1_1_2_1_1[i]), GTK_JUSTIFY_CENTER);
		gtk_box_pack_start(GTK_BOX(hbox_1_1_2_1),hbox_1_1_2_1_1[i],FALSE,FALSE,0);
		gtk_container_add(GTK_CONTAINER(hbox_1_1_2_1_1[i]), label_1_1_2_1_1[i]);
	}   

	gtk_container_add(GTK_CONTAINER(sw_1_1_1_1_1),source_list);

	gtk_box_pack_start(GTK_BOX(hbox_1_1_1_1),sw_1_1_1_1_1,FALSE,FALSE,0);

	gtk_container_add(GTK_CONTAINER(sw_1_1_1_1_2),target_list);            

	gtk_box_pack_start(GTK_BOX(hbox_1_1_1_1),sw_1_1_1_1_2,FALSE,FALSE,0);


	for(i=0;i<7;i++)
	{
		if (i ==2)
		{ 
			vbox_2_1_1[i] = gtk_event_box_new();
		}  
		else
		{
			vbox_2_1_1[i] =gtk_vbox_new(FALSE,0);
		}

		gtk_widget_set_size_request(GTK_WIDGET(vbox_2_1_1[i]),114,85);
		update_widget_bg(vbox_2_1_1[i], backpic[1]);

		label_2_1_1[i] = gtk_label_new(char_2_1_1[i]);
		gtk_widget_modify_fg (label_2_1_1[i], GTK_STATE_NORMAL, &color_black);
		gtk_label_set_justify(GTK_LABEL(label_2_1_1[i]), GTK_JUSTIFY_CENTER);

		gtk_container_add(GTK_CONTAINER(vbox_2_1_1[i]),label_2_1_1[i]);
		gtk_box_pack_start(GTK_BOX(vbox_2_1),vbox_2_1_1[i],FALSE,FALSE,0); 
	}     

	gtk_box_pack_start(GTK_BOX(vbox_1_1_1),hbox_1_1_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1_2),hbox_1_1_2_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1),vbox_1_1_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_1_1),vbox_1_1_2, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox_1),vbox_1_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_1, FALSE, FALSE, 0); 

	gtk_box_pack_start(GTK_BOX(hbox_2),vbox_2_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_2, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox_first),hbox, FALSE, FALSE, 0);

	my_signal.source_model = source_model;
	my_signal.source_selection = source_selection;
	my_signal.source_list = source_list;
	my_signal.target_model = target_model;
	my_signal.target_selection = target_selection;
	my_signal.target_list = target_list;

	//关闭窗口的信号
	g_signal_connect(G_OBJECT (vbox_2_1_1[2]), "button-press-event",G_CALLBACK(dialog_destroy), dialog);

	g_signal_connect(G_OBJECT(hbox_1_1_2_1_1[1]), "button-press-event",G_CALLBACK(Select_All_File), (gpointer)source_list);

	g_signal_connect(G_OBJECT(hbox_1_1_2_1_1[2]), "button-press-event",G_CALLBACK(Copy_File), (gpointer)&my_signal);

	g_signal_connect(G_OBJECT(hbox_1_1_2_1_1[3]), "button-press-event",G_CALLBACK(Move_File), (gpointer)&my_signal);

	g_signal_connect(G_OBJECT(hbox_1_1_2_1_1[4]), "button-press-event",G_CALLBACK(Delect_File), (gpointer)&my_signal);

	g_signal_connect(G_OBJECT(hbox_1_1_2_1_1[5]), "button-press-event",G_CALLBACK(Rename_File), (gpointer)&my_signal);

	gtk_widget_show_all(dialog);

	gtk_widget_hide (GTK_DIALOG(dialog)->action_area);

	return ;  
}

void  on_changed_law_save(GtkWidget *widget, gpointer label) 
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	gchar *file_path;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(widget), &model, &iter)) {

		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		file_path = 
			(gchar*)malloc(strlen("file://") + strlen(label) + strlen(value) + 1);
		strcpy(file_path, "file://");
		strcat(file_path, label);
		strcat(file_path, value);
		webkit_web_view_load_uri (pp->web_view_tmp, file_path);   /* 本地文件路径前面加上file:// */
		printf("file_path = %s\n", file_path);
		g_free(file_path);
		g_free(value);
	}
	gtk_tree_model_unref_node (model, &iter);

}

gboolean law_save (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	gchar *file_path;
	gint offset, k;
	file_path = g_strdup_printf ("%s%s.law",
			USER_LAW_PATH, gtk_entry_get_text (GTK_ENTRY (data)));
	for (offset = 0, k = 0 ; k < CFG(groupId); k++)
		offset += TMP(beam_qty[k]);
	save_law_file(file_path, offset, CFG(groupId));
	g_free (file_path);
	//	widget_window_class->key_press_event = my_keypress_event;
	gtk_widget_destroy (gtk_widget_get_parent 
			(gtk_widget_get_parent
			 (gtk_widget_get_parent (widget))));
	return TRUE;
}

gboolean law_close (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	//	widget_window_class->key_press_event = my_keypress_event;
	gtk_widget_destroy (GTK_WIDGET (data));
	return TRUE;
}

gboolean law_name (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	gtk_widget_grab_focus (GTK_WIDGET (data));

	return 0;
}

/* 13 保存聚焦法则 */
static void draw_law_save ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	//	GtkWidgetClass *widget_window_class1;
	GtkWidget *dialog;
	GtkWidget *vbox1;

	GtkWidget *ebox_save;
	GtkWidget *label_save;

	GtkWidget *ebox_close;
	GtkWidget *label_close;

	GtkWidget *vbox_name;
	GtkWidget *entry_name;
	GtkWidget *ebox_name;
	GtkWidget *label_name;

	GtkWidget *sw;
	GtkWidget *sw1;
	GtkWidget *table;
	GtkWidget *hpaned;
	GtkWidget *list;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	dialog = gtk_dialog_new_with_buttons("Dialog_Law_Save", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg (GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/
	//	widget_window_class->key_press_event = window_keypress_event_orig;
	//	widget_window_class1 = GTK_WIDGET_GET_CLASS (((GtkObject*)(dialog))); 
	//	widget_window_class1->key_press_event = gtk_entry_full_screen_keypress_event;
	/* 这里处理按键 */


	vbox1	= GTK_WIDGET (GTK_DIALOG(dialog)->vbox);
	ebox_save	= gtk_event_box_new();
	label_save	= gtk_label_new("Save");
	update_widget_bg(ebox_save, backpic[7]);
	ebox_close	= gtk_event_box_new();
	label_close	= gtk_label_new("Close");
	update_widget_bg(ebox_close, backpic[7]);
	entry_name	= gtk_entry_new();
	vbox_name	= gtk_vbox_new(FALSE, 0);
	ebox_name	= gtk_event_box_new();
	label_name	= gtk_label_new("Name");

	gtk_container_add (GTK_CONTAINER (ebox_save), label_save);
	gtk_container_add (GTK_CONTAINER (ebox_close), label_close);
	gtk_container_add (GTK_CONTAINER (ebox_name), label_name);
	gtk_box_pack_start (GTK_BOX(vbox_name), entry_name, FALSE, FALSE, 5);
	gtk_box_pack_start (GTK_BOX(vbox_name), ebox_name, TRUE, TRUE, 5);

	g_signal_connect (G_OBJECT(ebox_save), "button-press-event", 
			G_CALLBACK(law_save), (gpointer) (entry_name));

	g_signal_connect (G_OBJECT(ebox_close), "button-press-event", 
			G_CALLBACK(law_close), (gpointer) (dialog));

	g_signal_connect (G_OBJECT(ebox_name), "button-press-event", 
			G_CALLBACK(law_name), (gpointer) (entry_name));

	sw		= gtk_scrolled_window_new(NULL, NULL);
	sw1		= gtk_scrolled_window_new(NULL, NULL);
	/* 文件的名字 */
	gtk_widget_set_size_request(sw, 200, 400);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);

	/* 文件的内容 */
	gtk_widget_set_size_request(sw1, 400, 400);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	pp->web_view_tmp = WEBKIT_WEB_VIEW (webkit_web_view_new ());
	webkit_web_view_set_custom_encoding (pp->web_view_tmp, "UTF-8");
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add (GTK_CONTAINER (sw), list);
	gtk_container_add (GTK_CONTAINER (sw1), GTK_WIDGET (pp->web_view_tmp));

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	init_file_list (list, pp->selection, USER_LAW_PATH, DT_REG);
	//	on_changed(GTK_WIDGET (pp->selection), NULL) ;/* 初始化 */

	g_signal_connect (G_OBJECT (pp->selection), "changed", 
			G_CALLBACK(on_changed_law_save), USER_LAW_PATH);

	table = gtk_table_new(6, 7, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 0);
	gtk_table_set_col_spacings(GTK_TABLE(table), 0);


	gtk_table_attach(GTK_TABLE(table), hpaned, 0, 6, 0, 5, 
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);

	gtk_table_attach(GTK_TABLE(table), ebox_save, 6, 7, 1, 2,		/* 0 5   */
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */

	gtk_table_attach(GTK_TABLE(table), ebox_close, 6, 7, 2, 3, 
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */

	gtk_table_attach(GTK_TABLE(table), vbox_name, 0, 7, 5, 6, 
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 下方栏 */


	gtk_box_pack_start (GTK_BOX(vbox1), table, TRUE, TRUE, 5);
	gtk_widget_show_all (dialog);
	gtk_widget_hide (GTK_DIALOG(dialog)->action_area);
}

gboolean law_read (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gint offset, k;
	gchar *value = NULL;
	gchar *file_path = NULL;

	if (gtk_tree_selection_get_selected(
				GTK_TREE_SELECTION(pp->selection), &model, &iter)) /* 选中探头型号时 */
	{
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		file_path = g_strdup_printf ("%s%s", USER_LAW_PATH, value);

		for (offset = 0, k = 0 ; k < CFG(groupId); k++)
			offset += TMP(beam_qty[k]);
		g_print("%s\n", file_path);
		read_law_file(file_path, offset, CFG(groupId));
		GROUP_VAL (frequency) = GROUP_VAL(probe.Frequency);				/* 频率 */

	}

	g_free (file_path);
	//	widget_window_class->key_press_event = my_keypress_event;
	gtk_widget_destroy (gtk_widget_get_parent 
			(gtk_widget_get_parent
			 (gtk_widget_get_parent (widget))));
	return TRUE;
}

/* 14 读取聚焦法则 */
static void draw_law_read ()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	//	GtkWidgetClass *widget_window_class1;
	GtkWidget *dialog;
	GtkWidget *vbox1;

	GtkWidget *ebox_read;
	GtkWidget *label_read;

	GtkWidget *ebox_close;
	GtkWidget *label_close;

	GtkWidget *sw;
	GtkWidget *sw1;
	GtkWidget *table;
	GtkWidget *hpaned;
	GtkWidget *list;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	dialog = gtk_dialog_new_with_buttons("Dialog_Law_Save", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 800, 600);
	gtk_widget_modify_bg (GTK_WIDGET (dialog), GTK_STATE_NORMAL, &color_black);	/*黑色背景*/
	//	widget_window_class->key_press_event = window_keypress_event_orig;
	//	widget_window_class1 = GTK_WIDGET_GET_CLASS (((GtkObject*)(dialog))); 
	//	widget_window_class1->key_press_event = gtk_entry_full_screen_keypress_event;
	/* 这里处理按键 */


	vbox1	= GTK_WIDGET (GTK_DIALOG(dialog)->vbox);
	ebox_read	= gtk_event_box_new();
	label_read	= gtk_label_new("Read");
	gtk_widget_set_size_request(GTK_WIDGET (ebox_read), 115, 85);
	//gtk_widget_modify_bg(GTK_WIDGET (ebox_read), GTK_STATE_NORMAL, &color_blue);
	update_widget_bg(ebox_read, backpic[7]);
	ebox_close	= gtk_event_box_new();
	label_close	= gtk_label_new("Close");
	gtk_widget_set_size_request(GTK_WIDGET (ebox_close), 115, 85);
	//gtk_widget_modify_bg(GTK_WIDGET (ebox_close), GTK_STATE_NORMAL, &color_blue);
	update_widget_bg(ebox_close, backpic[7]);

	gtk_container_add (GTK_CONTAINER (ebox_read), label_read);
	gtk_container_add (GTK_CONTAINER (ebox_close), label_close);

	g_signal_connect (G_OBJECT(ebox_read), "button-press-event", 
			G_CALLBACK(law_read), (gpointer) (NULL));

	g_signal_connect (G_OBJECT(ebox_close), "button-press-event", 
			G_CALLBACK(law_close), (gpointer) (dialog));

	sw		= gtk_scrolled_window_new(NULL, NULL);
	sw1		= gtk_scrolled_window_new(NULL, NULL);
	/* 文件的名字 */
	gtk_widget_set_size_request(sw, 200, 600);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
			GTK_SHADOW_ETCHED_IN);

	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
			GTK_TREE_MODEL(store));
	g_object_unref(store);

	/* 文件的内容 */
	gtk_widget_set_size_request(sw1, 400, 600);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
			GTK_SHADOW_ETCHED_IN);

	pp->web_view_tmp = WEBKIT_WEB_VIEW (webkit_web_view_new ());
	webkit_web_view_set_custom_encoding (pp->web_view_tmp, "UTF-8");
	/* 放置名字和内容 */
	hpaned = gtk_hpaned_new ();
	gtk_paned_add1 (GTK_PANED (hpaned), sw);
	gtk_paned_add2 (GTK_PANED (hpaned), sw1);
	gtk_container_add (GTK_CONTAINER (sw), list);
	gtk_container_add (GTK_CONTAINER (sw1), GTK_WIDGET (pp->web_view_tmp));

	pp->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	init_file_list (list, pp->selection, USER_LAW_PATH, DT_REG);
	//	on_changed(GTK_WIDGET (pp->selection), NULL) ;/* 初始化 */

	g_signal_connect (G_OBJECT (pp->selection), "changed", 
			G_CALLBACK(on_changed_law_save), USER_LAW_PATH);

	table = gtk_table_new(6, 7, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 0);
	gtk_table_set_col_spacings(GTK_TABLE(table), 0);


	gtk_table_attach(GTK_TABLE(table), hpaned, 0, 6, 0, 6, 
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);

	gtk_table_attach(GTK_TABLE(table), ebox_read, 6, 7, 1, 2,		/* 0 5   */
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */

	gtk_table_attach(GTK_TABLE(table), ebox_close, 6, 7, 2, 3, 
			GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);	/* 侧边栏 */

	gtk_box_pack_start (GTK_BOX(vbox1), table, TRUE, TRUE, 5);
	gtk_widget_show_all (dialog);
	gtk_widget_hide (GTK_DIALOG(dialog)->action_area);
}

static void da_call_ip (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	char ifconfig_buf[64] = "ifconfig eth0 ";
	_my_ip_get tmp_ip;        
	_my_ip_set *entry_ip_p = (_my_ip_set *)user_data;
	int i;
	unsigned char *tmp;
	char ip_string[256] = {0};
	struct in_addr addr;
	int inet_sock;
	struct ifreq ifr;
	int system_value;

	//用户点击了确认按钮
	if (GTK_RESPONSE_OK == response_id)
	{  
		tmp = (unsigned char *)&tmp_ip;

		//读取输入框里面的数据
		for(i=0;i<4;i++)
		{  
			*tmp = gtk_spin_button_get_value((GtkSpinButton *)entry_ip_p->entry[i]);
			g_printf("%d\n",*tmp);    
			tmp++;
		}

		//设置ip地址
		memcpy(&addr,&tmp_ip,sizeof(tmp_ip));
		sprintf(ip_string,"%s\n", inet_ntoa(addr));

		strcat(ifconfig_buf,ip_string);
		system_value = system(ifconfig_buf);

		//重新读取ip地址，并显示出来，因为有可能设置失败，所以要重新读取
		inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
		strcpy(ifr.ifr_name, "eth0");
		if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0)
			perror("ioctl");

		memcpy(&tmp_ip,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_ip));  

		memcpy(&addr,&tmp_ip,sizeof(tmp_ip));
		sprintf(ip_string,"%s\n", inet_ntoa(addr));

		gtk_label_set_text (GTK_LABEL (pp->data3[0]), ip_string);

		//重算子网掩码，并重画
		draw3_data1(pp);

		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
	}
	else if(GTK_RESPONSE_CANCEL == response_id)
	{
		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
	}


	printf("response\n");
}

static void draw_ip()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;

	GtkObject *adjustment[5];
	GtkWidget *label[5];
	char *char_label[5] = {"IP Address","","","",""};

	GtkWidget *hbox1;
	GtkWidget *hbox2;
	GtkWidget *hbox3;

	GtkWidget *vbox_first;	/* 指向dialog的vbox */

	int inet_sock;
	struct ifreq ifr;
	_my_ip_get tmp_ip;
	//_my_ip_set entry_ip;
	unsigned char *tmp;
	int i;

	//读取eth0 ip地址
	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, "eth0");
	if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0)
		perror("ioctl");

	memcpy(&tmp_ip,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_ip));

	//新建对话框
	dialog = gtk_dialog_new_with_buttons ("Dialog_Ip", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 300, 140);

	vbox_first = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	hbox1 = gtk_hbox_new(TRUE, 0);
	hbox2 = gtk_hbox_new(FALSE, 0);
	hbox3 = gtk_hbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_first), hbox3, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox2, FALSE, FALSE, 5);

	label[0] = gtk_label_new(char_label[0]);
	gtk_box_pack_start(GTK_BOX(hbox3), label[0], TRUE, TRUE, 5);

	tmp = (unsigned char *)&tmp_ip;

	//把ip在输入框那里显示出来
	for (i=1;i<5;i++)
	{
		label[i] = gtk_label_new(char_label[i]);
		gtk_box_pack_start(GTK_BOX(hbox1), label[i], FALSE, FALSE, 15);
		adjustment[i] = gtk_adjustment_new(0.0,0.0,255.0,1.0,0.0,0.0);
		entry_ip.entry[i-1] = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment[i]),0.01,0);
		gtk_spin_button_set_value((GtkSpinButton *)entry_ip.entry[i-1],*tmp);
		gtk_box_pack_start(GTK_BOX(hbox2), entry_ip.entry[i-1], TRUE, TRUE, 1);
		tmp++;
	}

	//用户点击对话上面的按钮
	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_ip), (gpointer)&entry_ip);/*确定 or 取消*/

	gtk_widget_show_all(dialog);
}

static void da_call_mask (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	char ifconfig_buf[64] = "ifconfig eth0 netmask ";
	_my_mask_get tmp_mask;        
	_my_mask_set *entry_mask_p = (_my_mask_set *)user_data;
	int i;
	unsigned char *tmp;
	char mask_string[256] = {0};
	struct in_addr addr;
	int inet_sock;
	struct ifreq ifr;
	int system_value;
	//用户点击了确认按钮
	if (GTK_RESPONSE_OK == response_id)
	{  
		tmp = (unsigned char *)&tmp_mask;

		//读取输入框里面的数据
		for(i=0;i<4;i++)
		{  
			*tmp = gtk_spin_button_get_value((GtkSpinButton *)entry_mask_p->entry[i]);
			g_printf("%d\n",*tmp);    
			tmp++;
		}

		//设置mask地址
		memcpy(&addr,&tmp_mask,sizeof(tmp_mask));
		sprintf(mask_string,"%s\n", inet_ntoa(addr));

		strcat(ifconfig_buf,mask_string);
		system_value = system(ifconfig_buf);

		//重新读取mask地址，并显示出来，因为有可能设置失败，所以要重新读取
		inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
		strcpy(ifr.ifr_name, "eth0");
		if (ioctl(inet_sock, SIOCGIFNETMASK, &ifr) < 0)
			perror("ioctl");

		memcpy(&tmp_mask,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_mask));  

		memcpy(&addr,&tmp_mask,sizeof(tmp_mask));
		sprintf(mask_string,"%s\n", inet_ntoa(addr));

		gtk_label_set_text (GTK_LABEL (pp->data3[1]), mask_string);

		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
	}
	else if(GTK_RESPONSE_CANCEL == response_id)
	{
		//关闭窗口
		gtk_widget_destroy (GTK_WIDGET (dialog));
	}


	printf("response\n");
}

static void draw_mask()
{
	GtkWindow *win = GTK_WINDOW (pp->window);
	GtkWidget *dialog;

	GtkObject *adjustment[5];
	GtkWidget *label[5];
	char *char_label[5] = {"MASK Address","","","",""};

	GtkWidget *hbox1;
	GtkWidget *hbox2;
	GtkWidget *hbox3;

	GtkWidget *vbox_first;	/* 指向dialog的vbox */

	int inet_sock;
	struct ifreq ifr;
	_my_mask_get tmp_mask;
	//_my_mask_set entry_mask;
	unsigned char *tmp;
	int i;

	//读取eth0 mask 地址
	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, "eth0");
	if (ioctl(inet_sock, SIOCGIFNETMASK, &ifr) < 0)
		perror("ioctl");

	memcpy(&tmp_mask,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr,sizeof(tmp_mask));

	//新建对话框
	dialog = gtk_dialog_new_with_buttons ("Dialog_MASK", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	gtk_widget_set_size_request(GTK_WIDGET (dialog), 300, 140);

	vbox_first = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

	hbox1 = gtk_hbox_new(TRUE, 0);
	hbox2 = gtk_hbox_new(FALSE, 0);
	hbox3 = gtk_hbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_first), hbox3, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_first), hbox2, FALSE, FALSE, 5);

	label[0] = gtk_label_new(char_label[0]);
	gtk_box_pack_start(GTK_BOX(hbox3), label[0], TRUE, TRUE, 5);

	tmp = (unsigned char *)&tmp_mask;

	//把ip在输入框那里显示出来
	for (i=1;i<5;i++)
	{
		label[i] = gtk_label_new(char_label[i]);
		gtk_box_pack_start(GTK_BOX(hbox1), label[i], FALSE, FALSE, 15);
		adjustment[i] = gtk_adjustment_new(0.0,0.0,255.0,1.0,0.0,0.0);
		entry_mask.entry[i-1] = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment[i]),0.01,0);
		gtk_spin_button_set_value((GtkSpinButton *)entry_mask.entry[i-1],*tmp);
		gtk_box_pack_start(GTK_BOX(hbox2), entry_mask.entry[i-1], TRUE, TRUE, 1);
		tmp++;
	}

	//用户点击对话上面的按钮
	g_signal_connect (G_OBJECT(dialog), "response",
			G_CALLBACK(da_call_mask), (gpointer)&entry_mask);/*确定 or 取消*/

	gtk_widget_show_all(dialog);
}

/*
 * 弹出的dialog
 * 0 记事本 备注等等
 * 1 探头 
 * 2 楔块 
 * 3 自定义探头
 * 4 自定义楔块
 * 5 
 * 13 聚焦法则保存
 * 6 配置 数据 报告 图像 的读入
 * 7 保存 配置文件
 * 8 系统信息的显示 
 * 9 报告的显示
 * 10 Export Table
 * 11 调色板
 * 12 文件管理
 * 13 聚集法则的保存
 * 14 聚集法则的读入
 * 15 
 */
static void draw_dialog_all (guint type)
{

	change_keypress_event (KEYPRESS_DIALOG);
	pp->dialog_pos = type;
	switch (type)
	{
		case DIALOG_REMARK: draw_remark(); break;
		case DIALOG_PROBE:  draw_probe(); break;
		case DIALOG_WEDGE:  draw_wedge(); break;
		case DIALOG_FILE_OPEN:  draw_file_open_main(); break;

		case DIALOG_SAVE_SETUP_AS:  draw_save_setup_as(); break;



		case DIALOG_SYSTEM_INFO:  draw_system_info(); break;
		case DIALOG_COLOR_PALETTE:  draw_color_palette(); break;
		case DIALOG_FILE_MANAGE:	draw_file_manage(); break;
		case DIALOG_LAW_SAVE:	draw_law_save();break;
		case DIALOG_LAW_READ:	draw_law_read();break;
		case DIALOG_IP:		draw_ip();break;
		case DIALOG_MASK:	draw_mask();break;
		default:break;
	}
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
static void draw3_pop_tt (void (*fun)(GtkMenuItem*, gpointer),
		gpointer p,	const gchar *cur_value,	const gchar *content[],
		guint qty, gint pos, guint pop_pos, guint menu_status)
{
	gint i, x, y, z;
	GtkMenu *menu;
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	update_widget_bg(pp->eventbox30[z], backpic[6]);
	for (i = 0; i < 30; i++) 
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
	gtk_widget_hide (GTK_WIDGET (pp->entry));

	pp->menu3_qty = qty;
	pp->menu3_poppos = pop_pos;
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

	update_widget_bg(pp->eventbox30[z], backpic[6]);
	for (i = 0; i < 30; i++) 
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
	gtk_widget_hide (GTK_WIDGET (pp->entry));

	pp->menu3_qty = qty;
	pp->menu3_poppos = pop_pos;

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
		update_widget_bg(pp->eventbox30[z], backpic[6]);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox30[z], backpic[8]);
			update_widget_bg (pp->eventbox31[z], backpic[14]);
		}
		else
			update_widget_bg (pp->eventbox31[z], backpic[11]);
	}
	else if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg (pp->eventbox30[z], backpic[8]);
		if (big_menu)
			update_widget_bg (pp->eventbox31[z], backpic[14]);
		else
			update_widget_bg (pp->eventbox31[z], backpic[11]);
	}
	else
	{
		update_widget_bg (pp->eventbox30[z], backpic[7]);
		if (big_menu)
			update_widget_bg (pp->eventbox31[z], backpic[13]);
		else 
			update_widget_bg (pp->eventbox31[z], backpic[10]);
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
	gtk_widget_hide (GTK_WIDGET (pp->entry));


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
		update_widget_bg(pp->eventbox30[z], backpic[6]);
		if (big_menu)
		{
			update_widget_bg (pp->eventbox30[z], backpic[8]);
			update_widget_bg (pp->eventbox31[z], backpic[14]);
		}
		else
			update_widget_bg (pp->eventbox31[z], backpic[11]);
	}
	else if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg (pp->eventbox30[z], backpic[8]);
		if (big_menu)
			update_widget_bg (pp->eventbox31[z], backpic[14]);
		else
			update_widget_bg (pp->eventbox31[z], backpic[11]);
	}
	else
	{
		update_widget_bg (pp->eventbox30[z], backpic[7]);
		if (big_menu)
			update_widget_bg (pp->eventbox31[z], backpic[13]);
		else 
			update_widget_bg (pp->eventbox31[z], backpic[10]);
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
	gtk_widget_hide (GTK_WIDGET (pp->entry));


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
	update_widget_bg(pp->eventbox30[z], backpic[6]);
	//	widget_window_class->key_press_event = window_keypress_event_orig;
	/* 一个信号能对应多个回调函数，所以先把对应的回调函数取消 */
	if (g_signal_handler_is_connected (G_OBJECT (pp->sbutton[z]), pp->signal_id))
		g_signal_handler_disconnect (G_OBJECT (pp->sbutton[z]), pp->signal_id);
	pp->signal_id = g_signal_connect (G_OBJECT(pp->sbutton[z]), "value-changed", G_CALLBACK(fun), (gpointer) (pp));
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
	gtk_widget_hide (GTK_WIDGET (pp->entry));

	if (str)
		g_free(str);

	change_keypress_event (KEYPRESS_MAIN_SPINBUTTON);
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

	if (g_signal_handler_is_connected (G_OBJECT (pp->sbutton[z]), pp->signal_id))
		g_signal_handler_disconnect (G_OBJECT (pp->sbutton[z]), pp->signal_id);
	if (!unit )
		str = g_strdup_printf ("%s", con2_p[x][y][content_pos ? content_pos : pos]);	
	else
		str = g_strdup_printf ("%s\n%s", con2_p[x][y][content_pos ? content_pos : pos], unit);	

	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	gtk_label_set_justify (GTK_LABEL(pp->label3[z]),GTK_JUSTIFY_CENTER);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
		update_widget_bg(pp->eventbox30[z], backpic[8]);
		update_widget_bg(pp->eventbox31[z], backpic[11]);
	}
	else
	{
		update_widget_bg(pp->eventbox30[z], backpic[7]);
		update_widget_bg(pp->eventbox31[z], backpic[10]);
	}
	/* 更新当前增益值显示 */
	str = g_strdup_printf ("%0.*f", digit, cur_value);

	gtk_label_set_text (GTK_LABEL (pp->data3[z]), str);
	/*	gtk_label_set_text (GTK_LABEL (pp->label[1]), str);*/
	if (str)
		g_free(str);
	//	widget_window_class->key_press_event = my_keypress_event;
	/* 显示和隐藏控件 */
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
	gtk_widget_hide (GTK_WIDGET (pp->entry));

	change_keypress_event (KEYPRESS_MAIN);

	return ;
	/*						gtk_widget_grab_focus (pp->button);*/
}

/* 画栅格线 */
static gboolean draw_info(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gfloat color_r = 0, color_g = 0, color_b = 0;
	gint i,j,m,n,w,h;
	cairo_t *cr; 
	gfloat k;
	gchar *str = NULL;

	gtk_widget_get_size_request (widget, &w, &h);
	g_print("w=%d, h=%d\n",w,h);
	cr = gdk_cairo_create(widget->window);
	cairo_set_line_width(cr, 1);
	switch(CFG(grid))
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

	if (CFG(grid) != 5) 
	{
		for (j = 0; j < h; j += (h / 10.0))
		{
			for  (m = 0; m < w; m += (w / 50.0))
			{
				cairo_move_to (cr, (int)(m) , (int)(j) + 0.5);
				cairo_line_to (cr, (int)(m) +1 , (int)(j) + 0.5);
				cairo_stroke (cr);
			}
		}
		for ( i = 0; i < w; i += (w /10.0))
		{
			for (n = 0; n < h; n += (h / 50.0))
			{
				cairo_move_to (cr, (int)(i) , (int)(n) + 0.5);
				cairo_line_to (cr, (int)(i) +1 , (int)(n) + 0.5);
				cairo_stroke(cr);
			}
		}
	}

	/* 调色条信息 */
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 1.0);
	cairo_rectangle (cr, w-30, 0, 10, h-20);
	cairo_fill (cr);
	for ( i = 1; i < h - 21; i++ )
	{
		switch (((DRAW_AREA_P)(data))->scan_type)
		{
			case	A_SCAN:
			case	A_SCAN_R:
			case	B_SCAN:
			case	S_SCAN:
			case	S_SCAN_A:
				color_r = ((TMP(color_amp[i * 256 / h]) >> 11)) / 32.0;
				color_g = ((TMP(color_amp[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
				color_b = ((TMP(color_amp[i * 256 / h]) & 0x1f)) /  32.0;
				break;
			case	C_SCAN:
				break;
			default:break;
		}
		cairo_set_source_rgba (cr, color_r, color_g, color_b, 1.0);
		cairo_move_to (cr, w - 29, i);
		cairo_line_to (cr, w - 21, i);
		cairo_stroke (cr);
	}

	/* 画cursor */
	if( CFG(overlay_cursor)==1 )
	{
		//cairo_set_line_width(cr, 1.0);
		cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);
		cairo_move_to (cr, 20, (h-20)*(1.0-GROUP_VAL(per_reference)/10000.0));
		cairo_line_to (cr, w-30, (h-20)*(1.0-GROUP_VAL(per_reference)/10000.0));
		cairo_stroke (cr);
		cairo_move_to (cr, 20+(w-50)*GROUP_VAL(u_reference)/(GROUP_VAL(velocity)*10.0/20.85), 0);
		cairo_line_to (cr, 20+(w-50)*GROUP_VAL(u_reference)/(GROUP_VAL(velocity)*10.0/20.85), h-20);
		cairo_stroke (cr);

		cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);
		cairo_move_to (cr, 20, (h-20)*(1.0-GROUP_VAL(per_measure)/10000.0));
		cairo_line_to (cr, w-30, (h-20)*(1.0-GROUP_VAL(per_measure)/10000.0));
		cairo_stroke (cr);
		cairo_move_to (cr, 20+(w-50)*GROUP_VAL(u_measure)/(GROUP_VAL(velocity)*10.0/20.85), 0);
		/* 当前值除以最大值再乘以窗口宽度，即为百分比 */
		cairo_line_to (cr, 20+(w-50)*GROUP_VAL(u_measure)/(GROUP_VAL(velocity)*10.0/20.85), h-20);
		cairo_stroke (cr);
	}

	/* 画闸门 gate */
	if(CFG(overlay_gate)==1)
	{
		cairo_set_source_rgba(cr,1.0,0.0,0.0,1.0);	/* A闸门为红色 */
		cairo_move_to(cr, (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0), 
				(1.0-GROUP_VAL(gate[0].height) / 100.0)*(h-20) );

		cairo_line_to(cr, (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0)+(GROUP_VAL(gate[0].width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0)  ,(1.0-GROUP_VAL(gate[0].height) / 100.0)*(h-20) );

		cairo_stroke(cr);
		g_printf("A gate start = %f\n",(GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0));
		g_printf("A gate width = %f\n",(GROUP_VAL(gate[0].width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0));

		cairo_set_source_rgba(cr,0.0,1.0,0.0,1.0);	/* B闸门为绿色 */
		cairo_move_to(cr, (GROUP_VAL(gate[1].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0), 
				(1.0-GROUP_VAL(gate[1].height) / 100.0)*(h-20) );

		cairo_line_to(cr, (GROUP_VAL(gate[1].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0)+(GROUP_VAL(gate[1].width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0)  ,(1.0-GROUP_VAL(gate[1].height) / 100.0)*(h-20) );
		cairo_stroke(cr);

		cairo_set_source_rgba(cr,1.0,1.0,0.0,1.0);	/* C闸门为黄色 */
		cairo_move_to(cr, (GROUP_VAL(gate[2].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0), 
				(1.0-GROUP_VAL(gate[2].height) / 100.0)*(h-20) );

		cairo_line_to(cr, (GROUP_VAL(gate[2].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0)+(GROUP_VAL(gate[2].width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0)  ,(1.0-GROUP_VAL(gate[2].height) / 100.0)*(h-20) );

		cairo_stroke(cr);

	}

	/* 画ruler */
	cairo_set_source_rgba(cr,0.72,0.94,0.1,1.0);    /* ruler颜色为绿色 */
	cairo_rectangle(cr,0.0,0.0,20.0,h-20);		/* vruler1 */
	cairo_fill (cr);
	cairo_rectangle(cr,w-20.0,0.0,20.0,h-20.0);	/* vruler2 */
	cairo_fill (cr);
	cairo_rectangle(cr,20.0,h-20,w-40,20.0);	/* hruler */
	cairo_fill (cr);
	cairo_stroke(cr);

	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);	/* 标尺及单位颜色为黑色 */
	cairo_set_font_size(cr,8);

	cairo_save (cr);				/* 保存当前画笔 */
	cairo_move_to (cr, 12, (h-30));	/*vruler1 单位 ％*/
	cairo_rotate (cr, G_PI * 3/2);
	cairo_show_text (cr, "%");
	cairo_restore (cr);				/* 恢复当前保存的画笔 */

	cairo_save(cr);
	cairo_move_to(cr,(w-5),(h-30));/*vruler2 单位 ％*/
	cairo_rotate(cr, G_PI*3/2);
	cairo_show_text(cr,"%");
	cairo_restore(cr);

	cairo_move_to(cr,30,(h-5));/*hruler 单位 ％*/
	cairo_show_text(cr,"mm");

	i=0;
	for( k=0;k<(h-20.0);k=k+(h-20.0)/100.0)	/* vruler1 标尺 */
	{
		if(i%10 == 0)
		{
			cairo_save(cr);
			cairo_move_to(cr,5,(int)k+0.5);
			cairo_line_to(cr,20,(int)k+0.5);
			cairo_stroke(cr);
			cairo_move_to(cr,12,(int)k-2.5);
			//		cairo_set_font_size(cr, 7);		/*设置字体大小*/
			//		cairo_translate(cr,250,0);		/*平移*/
			cairo_rotate(cr, G_PI * 3/2);		/*旋转270度*/
			str=g_strdup_printf("%d",(100-i));
			cairo_show_text(cr,str);   		/*标签*/
			cairo_restore(cr);
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
	i=0;
	for( k=0;k<w-50;k=k+(w-50.0)/100.0 )	/* hruler 标尺 */
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
			str=g_strdup_printf("%d",i);
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
	i=0;
	for( k=0;k<h-20;k=k+(h-20.0)/100.0 )	/* vruler2 标尺 */
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
			str=g_strdup_printf("%d",(100-i));
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
	g_free(str);
	cairo_destroy(cr);
	return TRUE;
}


/* 画电池等信息 */
static gboolean draw_other_info (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{

	gint  y2 = 23;	//y1 = 3,

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

	//	if (y == 300)
	//		g_print("%d  move a move %d\n", x, y);
	//	return TRUE;

	pressed = (gint)g_object_get_data (G_OBJECT(progress), "pressed");
	//	g_print("%d  move a%d move %d\n", x, pressed, y);
	if (pressed)
	{
		GROUP_GATE_POS(height) = (390 - (y - 115)) / 3.90 ;
		g_print("%d  move a%d move %d\n", x, GROUP_GATE_POS(height), y);
		gtk_widget_queue_draw (progress);
	}

	return FALSE;
}

static void draw_area(GtkWidget *parent_box, DRAW_AREA *draw_area, guint width, guint height,
		const gchar *title, gdouble v1s, gdouble v1e, gdouble v2s, gdouble v2e, 
		gdouble h1s, gdouble h1e, gpointer *other)
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
 *
 */
void set_drawarea_property()
{

}

/* 画波形数据显示区 */
void draw_area_all()
{
	gint i;

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

	if (CFG (display_group) == DISPLAY_CURRENT_GROUP) 
	{
		switch (CFG(display))
		{
			case A_SCAN:
				pp->draw_area[0].scan_type	=	A_SCAN;
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area (pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "A-scan", 
						0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				set_scan_config (0, A_SCAN, 615, 615, 390, 0, 0, CFG(groupId));
				/* 显示的位置 偏移等等 */
				break;
			case B_SCAN:
				pp->draw_area[0].scan_type	=	B_SCAN;
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "B-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				set_scan_config (0, B_SCAN, 615, 615, 390, 0, 0, CFG(groupId));
				break;
			case C_SCAN:
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "C-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				break;
			case S_SCAN:
				if ((GROUP_VAL(ut_unit) == UT_UNIT_SOUNDPATH) ||
						(GROUP_VAL(ut_unit) == UT_UNIT_TIME))
				{
					pp->draw_area[0].scan_type	=	S_SCAN;
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
					draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "S-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					gtk_widget_show (pp->vbox_area[0]);
					set_scan_config (0, S_SCAN, 615, 615, 390, 0, 0, CFG(groupId));
				}
				else if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_A;
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
					draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "S-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					gtk_widget_show (pp->vbox_area[0]);
					set_scan_config (0, S_SCAN_A, 615, 615, 390, 0, 0, CFG(groupId));
				}
				else if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
				{
					pp->draw_area[0].scan_type	=	S_SCAN_L;
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
					draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "S-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					gtk_widget_show (pp->vbox_area[0]);
					set_scan_config (0, S_SCAN_L, 615, 615, 390, 0, 0, CFG(groupId));
				}
				break;
			case A_B_SCAN:
				pp->draw_area[0].scan_type	=	A_SCAN;
				pp->draw_area[1].scan_type	=	B_SCAN;
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 0.0, NULL);
				draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 295, "B-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				set_scan_config (0, A_SCAN, 615, 615, 115, 0, 0, CFG(groupId));
				set_scan_config (1, B_SCAN, 615, 615, 260, 0, 130, CFG(groupId));
				break;
			case A_B_C_SCAN:
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 125, "A-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 150, "B-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				draw_area(pp->vbox_area[0], &(pp->draw_area[2]), 655, 150, "C-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				break;
			case A_B_S_SCAN:
				if (GROUP_VAL(ut_unit)==1)
				{
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
					gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
					gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
					draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 300, 425, "S-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					draw_area(pp->vbox_area[1], &(pp->draw_area[1]), 355, 210, "A-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					draw_area(pp->vbox_area[1], &(pp->draw_area[2]), 355, 215, "B-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					gtk_widget_show (pp->hbox_area[0]);
					gtk_widget_show (pp->vbox_area[0]);
					gtk_widget_show (pp->vbox_area[1]);
				}
				else
				{
					gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
					draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 125, "A-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 150, "B-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					draw_area(pp->vbox_area[0], &(pp->draw_area[2]), 655, 150, "S-scan", 0.0, 100.0,
							0.0, 100.0, 0.0, 100.0, NULL);
					gtk_widget_show (pp->vbox_area[0]);

				}
				break;

			case A_C_CC_SCAN:
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 275, "C-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				break;

			case A_S_CC_SCAN:
				if (CFG(c_scan11) == C_SCAN_OFF)
				{
					if ((GROUP_VAL(ut_unit) == UT_UNIT_SOUNDPATH) ||
							(GROUP_VAL(ut_unit) == UT_UNIT_TIME))
					{
						pp->draw_area[0].scan_type	=	A_SCAN;
						pp->draw_area[1].scan_type	=	S_SCAN;
						gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
						draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
								0.0, 100.0, 0.0, 0.0, NULL);
						draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 295, "S-scan", 0.0, 100.0,
								0.0, 100.0, 0.0, 100.0, NULL);
						gtk_widget_show (pp->vbox_area[0]);
						set_scan_config (0, A_SCAN, 615, 615, 115, 0, 0, CFG(groupId));
						set_scan_config (1, S_SCAN, 615, 615, 260, 0, 130, CFG(groupId));
					}
					else if (GROUP_VAL(ut_unit) == UT_UNIT_TRUE_DEPTH)
					{
						if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
						{
							pp->draw_area[0].scan_type	=	A_SCAN_R;
							pp->draw_area[1].scan_type	=	S_SCAN_A;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							draw_area(pp->hbox_area[0], &(pp->draw_area[0]), 250, 425, "A-scan", 0.0,
									100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area(pp->hbox_area[0], &(pp->draw_area[1]), 405, 425, "S-scan", 0.0, 
									100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, A_SCAN_R, 390, 210, 390, 0, 0, CFG(groupId));
							set_scan_config (1, S_SCAN_A, 390, 365, 390, 250, 0, CFG(groupId));
						} 
						else if (LAW_VAL(Focal_type) == LINEAR_SCAN)
						{
							pp->draw_area[0].scan_type	=	A_SCAN_R;
							pp->draw_area[1].scan_type	=	S_SCAN_L;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							draw_area(pp->hbox_area[0], &(pp->draw_area[0]), 250, 425, "A-scan", 0.0,
									100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area(pp->hbox_area[0], &(pp->draw_area[1]), 405, 425, "S-scan", 0.0, 
									100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, A_SCAN_R, 390, 210, 390, 0, 0, CFG(groupId));
							set_scan_config (1, S_SCAN_L, 390, 365, 390, 250, 0, CFG(groupId));
						}
					}
				}
				else
				{
				}
				break;

			case Strip_Chart_AA:
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 275, "Strip Chart", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				break;

			default:
				break;
		}
	}
	else 
		if (CFG (display_group) == DISPLAY_ALL_GROUP) 
		{
			switch (CFG(groupQty))
			{
				case 2:
					switch (CFG(display))
					{
						case A_SCAN:
							pp->draw_area[0].scan_type	=	A_SCAN;
							pp->draw_area[1].scan_type	=	A_SCAN;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							draw_area (pp->vbox_area[0], &(pp->draw_area[0]), 655, 212, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area (pp->vbox_area[0], &(pp->draw_area[1]), 655, 213, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->vbox_area[0]);
							set_scan_config (0, A_SCAN, 615, 615, 178, 0, 0,  0);
							set_scan_config (1, A_SCAN, 615, 615, 178, 0, 212, 1);
							/* 显示的位置 偏移等等 */
							break;
						case S_SCAN:
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "S-scan", 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->vbox_area[0]);
							break;
						case A_C_CC_SCAN:
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, NULL);
							draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 275, "C-scan", 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->vbox_area[0]);
							break;
						case A_S_CC_SCAN:
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, NULL);
							draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 275, "S-scan", 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->vbox_area[0]);
							break;
						default:
							break;
					}
					break;
				case 3:
					switch (CFG(display))
					{
						case A_SCAN:
							pp->draw_area[0].scan_type	=	A_SCAN;
							pp->draw_area[1].scan_type	=	A_SCAN;
							pp->draw_area[2].scan_type	=	A_SCAN;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
							draw_area (pp->vbox_area[0], &(pp->draw_area[0]), 655, 141, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area (pp->vbox_area[0], &(pp->draw_area[1]), 655, 142, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area (pp->vbox_area[0], &(pp->draw_area[2]), 655, 142, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->vbox_area[0]);
							set_scan_config (0, A_SCAN, 615, 615, 106, 0, 0,  0);
							set_scan_config (1, A_SCAN, 615, 615, 107, 0, 141, 1);
							set_scan_config (2, A_SCAN, 615, 615, 107, 0, 283, 2);
							/* 显示的位置 偏移等等 */
							break;
						default:break;
					}
					break;
				case 4:
					switch (CFG(display))
					{
						case A_SCAN:
							pp->draw_area[0].scan_type	=	A_SCAN;
							pp->draw_area[1].scan_type	=	A_SCAN;
							pp->draw_area[2].scan_type	=	A_SCAN;
							pp->draw_area[4].scan_type	=	A_SCAN;
							gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->hbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[0], FALSE, FALSE, 0);
							gtk_box_pack_start (GTK_BOX (pp->hbox_area[0]), pp->vbox_area[1], FALSE, FALSE, 0);
							draw_area (pp->vbox_area[0], &(pp->draw_area[0]), 327, 212, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area (pp->vbox_area[0], &(pp->draw_area[1]), 327, 212, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area (pp->vbox_area[1], &(pp->draw_area[2]), 327, 213, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							draw_area (pp->vbox_area[1], &(pp->draw_area[3]), 327, 213, "A-scan", 
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, NULL);
							gtk_widget_show (pp->vbox_area[0]);
							gtk_widget_show (pp->vbox_area[1]);
							gtk_widget_show (pp->hbox_area[0]);
							set_scan_config (0, A_SCAN, 327, 327, 178, 0, 0, 0);
							set_scan_config (1, A_SCAN, 327, 327, 178, 327, 0, 1);
							set_scan_config (2, A_SCAN, 327, 327, 178, 0, 212, 2);
							set_scan_config (3, A_SCAN, 327, 327, 178, 327, 212, 3);
							/* 显示的位置 偏移等等 */
							break;
						default:break;
					}
					break;
					break;
				default:break;
			}
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
	guint digit, pos, unit, content_pos, menu_status = 0;

	guint menu_on=0, i, temp_beam;

	int inet_sock;
	struct ifreq ifr;
	char ip_temp[256];


	time_t timep;
	struct tm *q;

	int time_hour;/*时*/
	int time_min;/*分*/
	int time_sec;/*秒*/

	//	gchar date_temp[52];  /*日期存储*/
	gchar time_temp[52];  /*时间存储*/
	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> Back P000 */
					draw3_popdown (NULL, 0, 1);
					if ((pp->start_qty == 0)||(pp->start_qty == 1)||(pp->start_qty == 9))
					{
						gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
					}
					else
					{
						gtk_widget_set_sensitive(pp->eventbox30[0], TRUE);
						gtk_widget_set_sensitive(pp->eventbox31[0], TRUE);
					}
					break;
				case 1:/*Wizard -> Focal Law -> Back p010 */
					draw3_popdown (NULL, 0, 1);
					if ((pp->fstart_qty == 0)||(pp->fstart_qty == 1)||(pp->fstart_qty == 5))
					{
						gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
					}
					else
					{
						gtk_widget_set_sensitive(pp->eventbox30[0], TRUE);
						gtk_widget_set_sensitive(pp->eventbox31[0], TRUE);
					}
					break;
				case 2:/*Wizard -> Calibration -> Back*/
					draw3_popdown (NULL, 0, 1);
					break;
				case 3:
					if ( !con2_p[0][3][0] )
						gtk_widget_hide (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
					break;
				case 4:
					if ( !con2_p[0][4][0] )
						gtk_widget_hide (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
					break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* 增益 Gain P100 TAN1 只等硬件 */
					switch (TMP(db_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;
						default:break;
					}
					g_print("db_reg = %d \n",pp->p_tmp_config->db_reg);
					if (GROUP_VAL(db_ref))
						content_pos = 6;
					else
						content_pos = 0;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
						lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
						upper = GAIN_MAX - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
						step = tmpf;
						digit = 1;
						pos = 0;
						unit = UNIT_DB;
						draw3_digit_pressed (data_100, units[unit], cur_value ,
								lower, upper, step, digit, p, pos, content_pos);
					}
					else 
					{
						cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
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
						cur_value = (gfloat) (GROUP_VAL(pulser));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 0;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (GROUP_VAL(pulser));
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
					if (GROUP_VAL(tx_rxmode) == PULSE_ECHO)	/* 脉冲回波模式不可以调节 */
					{
						gtk_widget_set_sensitive (pp->eventbox30[0], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[0], FALSE);
					}
					upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = (gfloat) (GROUP_VAL(receiver));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 0;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (GROUP_VAL(receiver));
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
						gtk_widget_set_sensitive (pp->eventbox30[0], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[0], FALSE);
					}
					switch (TMP(scanoffset_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
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
			switch (pp->pos1[2])
			{
				case 0:/* Gate 选择调节哪个闸门 P200 TAN1 */
					pp->x_pos = 630, pp->y_pos = 119 - YOFFSET;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_200, NULL, 
								menu_content[GATE_POS + GROUP_VAL(gate_pos)],
								menu_content + GATE_POS, 3, 0, GROUP_VAL(gate_pos), 0);
					else 
						draw3_popdown (menu_content[GATE_POS + GROUP_VAL(gate_pos)], 0, 0);
					break;
				case 1:/* Alarm 选择哪个报警项 P210 TAN1 */
					pp->x_pos = 624, pp->y_pos = 119-YOFFSET;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						menu_on = 0;
						for (i = 0 ; i < 16; i++)
							menu_on |= (CFG(alarm[i].conditiona) !=0 ) << i;
						draw3_pop_tt_on(data_210, NULL,
								menu_content[ALARM_POS + CFG(alarm_pos)],
								menu_content + ALARM_POS, 16, 0, CFG(alarm_pos), 0, menu_on, 16);
					}
					else 
						draw3_popdown(menu_content[ALARM_POS + CFG(alarm_pos)], 0, 0);
					break;
				case 2:/* Output  P220  */
					pp->x_pos = 581, pp->y_pos = 120-YOFFSET;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						menu_on = 0x0;
						for (i = 0 ; i < 3; i++) /*alarm#的值决定前三项后面是否跟[On]*/
							menu_on |= (CFG(output[i].alarm1) != 0 ) << i;

						for (i = 3 ; i < 5; i++) /*data的值决定后两项后面是否跟[On]*/
							menu_on |= (CFG(analog[i - 3].data) != 0 ) << i;
						draw3_pop_tt_on (data_220, NULL, menu_content[OUTPUT_POS + CFG(output_pos)],
								menu_content + OUTPUT_POS, 5, 0, CFG(output_pos), 0, menu_on, 5);
					}
					else 
						draw3_popdown (menu_content[OUTPUT_POS + CFG(output_pos)], 0, 0);
					break;

				case 3:/* Sizing Curves -> Setup  p230 */
					pp->x_pos = 600, pp->y_pos = 119-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
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
						gtk_widget_hide (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
					break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> List  p300 */
					pp->x_pos = 398, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_300, NULL, 
								list1[CFG(list)],
								list, 2, 0, CFG(list), 0);
					else 
						draw3_popdown (list1[CFG(list)], 0, 0);

					break;


				case 1:/*Measurements -> Cursors -> Selection p310 */
					pp->x_pos = 569, pp->y_pos = 116-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_310, NULL, 
								menu_content[SELECTION + GROUP_VAL(selection)],
								menu_content + SELECTION, 9, 0, GROUP_VAL(selection), 0);
					else 
						draw3_popdown (menu_content[SELECTION + GROUP_VAL(selection)], 0, 0);

					break;

				case 2:/*Measurements -> Table -> Display Table  p320 */

					draw3_popdown (menu_content[OFF_ON + CFG(display_table)], 0, 0);
					break;

				case 3:/*Measurements -> Thickness -> Source  p330 */
					pp->x_pos = 602, pp->y_pos = 117-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_330, NULL, 
								menu_content[SOURCE + GROUP_VAL(source)],
								menu_content + SOURCE, 9, 0, GROUP_VAL(source), 0);
					else 
						draw3_popdown (menu_content[SOURCE + GROUP_VAL(source)], 0, 0);

					break;

				case 4:/*Measurements -> Export -> Export Table p340 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw_dialog_all (DIALOG_REMARK);
					else
						draw3_popdown(NULL,0,1);
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{ 
				case 0:/* 选择显示模式ABSC P400 */
					pp->x_pos = 415, pp->y_pos = 90;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						if (CFG(groupQty) != 3)
							menu_status = 0x200;
						if (GROUP_VAL(ascan_source) != 0)
							menu_status |= 0x72;
						draw3_pop_tt (data_400, NULL, 
								menu_content[DISPL + CFG(display)],
								menu_content + DISPLAY, 11, 0, CFG(display), menu_status);
					}
					else 
						draw3_popdown (menu_content[DISPL + CFG(display)], 0, 0);
					break;
				case 1:/*Display -> Overlay -> UT Unit  P410 */
					pp->x_pos = 566, pp->y_pos = 120-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_410, NULL, 
								menu_content[UTUNIT + GROUP_VAL(ut_unit)],
								menu_content + UTUNIT, 3, 0, GROUP_VAL(ut_unit), 0);
					else 
						draw3_popdown (menu_content[UTUNIT + GROUP_VAL(ut_unit)], 0, 0);
					gtk_widget_queue_draw(pp->draw_area->drawing_area);

					break;

				case 2:/*Display -> Zoom -> Display p420 */
					pp->x_pos = 511, pp->y_pos = 117-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_420, NULL, 
								menu_content[ZOOM_DISPLAY + CFG(zoom_display_pos)],
								menu_content +ZOOM_DISPLAY, 6, 0, CFG(zoom_display_pos), 0x18);
					else 
						draw3_popdown (menu_content[ZOOM_DISPLAY + CFG(zoom_display_pos)], 0, 0);


					break;

				case 3:/*Display -> Color -> Select  p430 */
					pp->x_pos = 516, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_430, NULL, 
								menu_content[COL_SELECT + GROUP_VAL(col_select_pos)],
								menu_content +COL_SELECT, 3, 0, GROUP_VAL(col_select_pos), 0);
					else 
						draw3_popdown (menu_content[COL_SELECT + GROUP_VAL(col_select_pos)], 0, 0);

					break;

				case 4:/*Display -> Properties -> Scan P440 */
					pp->x_pos = 570, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_440, NULL, 
								menu_content[PROP_SCAN + CFG(prop_scan)],
								menu_content + PROP_SCAN, 6, 0, CFG(prop_scan), 0);
					else 
						draw3_popdown (menu_content[PROP_SCAN + CFG(prop_scan)], 0, 0);
					break;
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/* 组设置 添加删除选择组 P500 */
					pp->x_pos = 590, pp->y_pos = 90;	/* 位置ok */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						switch (CFG(groupQty))
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
								menu_content[GROUP_P + CFG(groupId) + 1],
								menu_content + GROUP_P, 10, 0, CFG(groupId) + 1, menu_status);
					}
					else 
						draw3_popdown (menu_content[GROUP_P + CFG(groupId) + 1], 0, 0);
					break;
				case 1:/* Scan Offset  P510 */
					switch (TMP(scanoffset_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						if(UNIT_MM == CFG(unit))
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
						if(UNIT_MM == CFG(unit))
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
					draw3_popdown (menu_content[OFF_ON + CFG(fft)], 0, 0);
					break;

				case 3: /* 被检则工件的几何形状  P530 */
					pp->x_pos = 608, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_530, NULL, 
								menu_content[GEOMETRY + get_part_geometry(pp->p_config)],
								menu_content+GEOMETRY, 3, 0, get_part_geometry(pp->p_config), 0);
					else 
						draw3_popdown (menu_content[GEOMETRY + get_part_geometry(pp->p_config)], 0, 0);
					break;
				case 4:
					if ( !con2_p[5][4][0] )
						gtk_widget_hide (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
					break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:/* 聚焦法则类型 线性扫查 角度(扇形)扫查
						  深度扫查 静态扫查  P600 TAN1 */
					pp->x_pos = 450, pp->y_pos = 116 - YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
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
					if(CFG(auto_program) == AUTO_FOCAL_ON)
						/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							/* 最大不能超过最大Angle_max */
							cur_value = LAW_VAL (Angle_min) / 100.0;
							/* 计算lower为妙 */
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
							TMP(beam_skew_num)	= (LAW_VAL(Angle_beam_skew_max) - 
									LAW_VAL(Angle_beam_skew_min)) /
								LAW_VAL(Angle_beam_skew_step) + 1;
							temp_beam = temp_beam / TMP(beam_skew_num);
							lower = MAX (((gint)(LAW_VAL (Angle_max)) - 
										(gint)(temp_beam * LAW_VAL(Angle_step))) / 100.0, -89.9);
							upper = LAW_VAL (Angle_max) / 100.0;
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
						gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					}
					break;
				case 2:/* 聚焦点的计算方式 0halfpath 1truedepth 2projection 3focalplane 4automatic P620 */
					pp->x_pos = 555, pp->y_pos = 116 - YOFFSET;
					if(CFG(auto_program) == AUTO_FOCAL_ON)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							if (GROUP_VAL (tx_rxmode) == PITCH_CATCH)
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
						gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					}
					break;
				case 3:/* 聚焦 阵元数量 P630 */
					switch (pp->p_tmp_config->element_qty_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if (CFG(auto_program) == AUTO_FOCAL_ON) /* 聚焦法则自动计算为 on 时，Element Qty 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
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
						gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					}
					break;
				case 4:/* 自动计算聚焦法则 P640 */
					draw3_popdown (menu_content[OFF_ON + CFG(auto_program)], 0, 0);
					break;
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:/*Scan -> Encoder -> Encoder p700 */
					pp->x_pos = 638, pp->y_pos = 130-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_700, NULL, 
								menu_content[ENCODER+CFG(encoder)],
								menu_content+ENCODER, 2, 0,CFG(encoder), 0);
					else 
						draw3_popdown (menu_content[ENCODER+CFG(encoder)], 0, 0);

					gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					break;

				case 1:/*Scan -> Inspection -> type  p710 */

					pp->x_pos = 546, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_710, NULL, 
								menu_content[I_TYPE+CFG(i_type)],
								menu_content+I_TYPE, 3, 0,CFG(i_type), 0);
					else 
						draw3_popdown (menu_content[I_TYPE+CFG(i_type)], 0, 0);
					break;

				case 2:/*Scan -> Area -> scan start p720 */
					/* 当前步进 */
					switch (TMP(scan_start_reg))
					{
						case 0:	tmpf = 0.001 * CFG(scan_resolution); break;
						case 1:	tmpf = 0.01 * CFG(scan_resolution); break;
						case 2:	tmpf = 0.1 * CFG(scan_resolution); break;
						default:break;
					}
					if(CFG(i_type)==0 || CFG(i_type)==1)
						/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							if(CFG(unit) == UNIT_MM)
							{
								cur_value = CFG(scan_start)/1000.0;
								lower = -99999.0;
								upper = 99999.0;
								step = tmpf;
								digit = 2;
								pos = 0;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(scan_start)/1000.0*0.03937;
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
							if(CFG(unit) == UNIT_MM)
							{
								cur_value = CFG(scan_start)/1000.0;
								digit = 2;
								pos = 0;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(scan_start)/1000.0*0.03937;
								digit = 3;
								pos = 0;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else  /* Inspection -> Type 选择 Helicoidal Scan  时 */
					{
						if(CFG(unit) == UNIT_MM)
						{
							CFG(scan_start) = 0;
							digit = 0;
							pos = 0;
							unit = UNIT_MM;
						}
						else
						{
							CFG(scan_start) = 0;
							digit = 0;
							pos = 0;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

						gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					}
					break;


				case 3:/*Scan -> start -> start mode  p730 */
					pp->x_pos = 550, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_730, NULL, 
								menu_content[START_MODE+CFG(start_mode)],
								menu_content+START_MODE, 3, 0,CFG(start_mode), 0);
					else 
						draw3_popdown (menu_content[START_MODE+CFG(start_mode)], 0, 0);

					break;

				case 4:/*Scan -> data -> storage  p740 */
					pp->x_pos = 550, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_740, NULL, 
								menu_content[STORAGE+CFG(storage)],
								menu_content+STORAGE, 4, 0,CFG(storage), 0);
					else 
						draw3_popdown (menu_content[STORAGE+CFG(storage)], 0, 0);

					break;

				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/*File -> File -> Storage  p800 */
					pp->x_pos = 442, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_800, NULL, 
								menu_content[F_STORAGE_P+CFG(file_storage)],
								menu_content+F_STORAGE, 2, 0,CFG(file_storage), 0);
					else 
						draw3_popdown (menu_content[F_STORAGE_P+CFG(file_storage)], 0, 0);

					break;

				case 1:/*File -> report -> template p810 */
					pp->x_pos = 502, pp->y_pos = 100;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_810, NULL, 
								menu_content[TEMPLA_P+CFG(templa)],
								menu_content+TEMPLA, 1, 0,CFG(templa), 0);
					else 
						draw3_popdown (menu_content[TEMPLA_P+CFG(templa)], 0, 0);


					break;

				case 2:/*File -> format -> user field  p820 */
					draw3_popdown (menu_content[OFF_ON + CFG(format_userfield)], 0, 0);
					break;

				case 3:/*File -> user field -> select  p830 */
					pp->x_pos = 630, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_830, NULL, 
								menu_content[F_SELECT+CFG(file_select)],
								menu_content+F_SELECT, 10, 0,CFG(file_select), 0);
					else 
						draw3_popdown (menu_content[F_SELECT+CFG(file_select)], 0, 0);


					break;

				case 4:/*File -> notes-> edit notes  p840 */
					draw3_popdown(NULL,0,1);
					gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					break;

				default:break;
			}
			break;
		case 9:
			switch (pp->pos1[9])
			{
				case 0:/*Preferences -> pref.-> units p900 */
					pp->x_pos = 570, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_900, NULL, 
								menu_content[P_UNITS+CFG(unit)],
								menu_content+P_UNITS, 2, 0,CFG(unit), 0);
					else 
						draw3_popdown (menu_content[P_UNITS+CFG(unit)], 0, 0);


					break;

				case 1:/*Preferences -> system -> clock set p910*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n(HH:MM:SS)", con2_p[9][1][0]);
					//					gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
					//					gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					//gtk_label_set_text (GTK_LABEL (pp->data3[0]), "02:55:06 PM");

					/***********/
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0)){
						//draw_dialog_all (DIALOG_TIME);
					}else{
						time(&timep);
						q = localtime(&timep);
						time_hour = q->tm_hour;
						time_min = q->tm_min;
						time_sec = q->tm_sec;
						memset(time_temp, 0, sizeof(time_temp));
						g_print("%d\n",time_hour);
						g_sprintf(time_temp,"%d:%d:%d",time_hour,time_min,time_sec);
						draw3_popdown(time_temp, 0, 0);
						gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);


					}
					/***********/


					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "02:55:06 PM");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
					break;

				case 2:/* 系统信息  P920*/
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw_dialog_all(DIALOG_SYSTEM_INFO);
					draw3_popdown(NULL,0,1);
					break;
				case 3:/*Preferences -> options -> mouse  p930 */
					pp->x_pos = 560, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_930, NULL, 
								menu_content[MOUSE+CFG(mouse)],
								menu_content+MOUSE, 3, 0,CFG(mouse), 0);
					else 
						draw3_popdown (menu_content[MOUSE+CFG(mouse)], 0, 0);
					gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);

					break;

				case 4:/*Preferences -> network -> IP Address  p940*/
					/* 格式化字符串 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw_dialog_all (DIALOG_IP);
						//gtk_label_set_text (GTK_LABEL (pp->data3[0]), "192.168.1.95");
					}
					else
					{
						inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
						strcpy(ifr.ifr_name, "eth0");
						ioctl(inet_sock, SIOCGIFADDR, &ifr);
						sprintf(ip_temp,"%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
						gtk_label_set_text (GTK_LABEL (pp->data3[0]), ip_temp);
					}

					g_sprintf (temp,"%s", con2_p[9][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					//gtk_label_set_text (GTK_LABEL (pp->data3[0]), ip_temp);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);

					//gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
					//gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
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
	guint digit, pos, unit, content_pos, menu_status = 0, temp_beam;

	int inet_sock;
	struct ifreq ifr;
	static char mask_temp[256];

	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> start  p001 */
					draw3_popdown (NULL, 1, 1);
					g_print("start_qty = %d\n",pp->start_qty);
					if( (pp->start_qty >0) && (pp->start_qty < 8) )
						draw3_popdown_offset (NULL, 1, 1, 6 );
					else if (pp->start_qty == 8)
						draw3_popdown_offset (NULL, 1, 1, 14 );
					break;

				case 1:/*Wizard -> Focal Law -> start p011 */
					draw3_popdown (NULL, 1, 1);
					g_print("fstart_qty = %d\n",pp->fstart_qty);
					if( (pp->fstart_qty >0) && (pp->fstart_qty < 5) )
						draw3_popdown_offset (NULL, 1, 1, 6 );
					break;

				case 2:/*Wizard -> Calibration -> start p021 */
					//draw3_popdown (NULL, 1, 1);
					g_print("cstart_qty = %d\n",pp->cstart_qty);
					if(pp->ctype_pos == 1)
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
							if (pp->cmode_pos == 1)
								draw3_popdown_offset (NULL, 1, 1, 32 );
							else if (pp->cmode_pos == 3)
								draw3_popdown_offset (NULL, 1, 1, 37 );
							else
								draw3_popdown_offset (NULL, 1, 1, 19 );
						}
						else if (pp->cstart_qty == 6)
						{
							if (pp->cmode_pos == 2)
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

				case 3:
					if ( !con2_p[0][3][1] )
						gtk_widget_hide (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
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
						case 0:	tmpf = (GROUP_VAL(range) / 1000.0) / (gfloat)(GROUP_VAL(point_qty)); break;
						case 1:	tmpf = (GROUP_VAL(range) / 1000.0) / 20.0 ; break;
						case 2:	tmpf = (GROUP_VAL(range) / 1000.0) / 10.0 ; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								lower = (BEAM_INFO(0,beam_delay) /1000.0) * GROUP_VAL(velocity) / 200000.0;
								upper =	(MAX_RANGE_US - GROUP_VAL(range) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * (GROUP_VAL(velocity) / 200000.0);
								(step < 0.01) ? (step = 0.01) : (step = step);
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (GROUP_VAL(start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								upper =	(MAX_RANGE_US - GROUP_VAL(range) / 1000.0 ) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								digit = 3;
								pos = 1;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = GROUP_VAL(start) / 1000.0 ;
							lower =	BEAM_INFO(0,beam_delay) / 1000.0;
							upper =	(MAX_RANGE_US - GROUP_VAL(range) / 1000.0);
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
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 1;
							}
							else
							{
								cur_value = (GROUP_VAL(start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								unit = UNIT_INCH;
								digit = 3;
								pos = 1;
							}
						}
						else
						{
							cur_value = GROUP_VAL(start) / 1000.0 ;
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
									menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)],
									menu_content + TX_RX_MODE, 3, 1, GROUP_VAL(tx_rxmode), 0x05);
						else if (GROUP_VAL (group_mode) == UT_SCAN)
							draw3_pop_tt (data_111, NULL, 
									menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)],
									menu_content + TX_RX_MODE, 4, 1, GROUP_VAL(tx_rxmode), 0x00);
					}
					else 
						draw3_popdown (menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)], 1, 0);
					break;
				case 2: /* 接收滤波 P121 TAN1 */
					pp->x_pos = 516, pp->y_pos = 201-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if (GROUP_VAL(filter))
							draw3_pop_tt (data_121, NULL, 
									menu_content[FILTER + GROUP_VAL(filter)],
									menu_content + FILTER , 15, 1, GROUP_VAL(filter), 0);
						else
							draw3_pop_tt (data_121, NULL, "None",
									menu_content + FILTER , 15, 1, GROUP_VAL(filter), 0);
					}
					else 
					{
						if (GROUP_VAL(filter))
							draw3_popdown (menu_content[FILTER + GROUP_VAL(filter)], 1, 0);
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
						draw3_pop_tt (data_211, NULL,menu_content[GROUPA + CFG_ALARM_POS(groupa)],
								menu_content + GROUPA, 4, 1, CFG_ALARM_POS(groupa), 0);
					else 
						draw3_popdown (menu_content[GROUPA + CFG_ALARM_POS(groupa)], 1, 0);
					break;
				case 2:/*  Gate -> Output -> Alarm or Group #  P221 */
					pp->x_pos = 604, pp->y_pos = 202-YOFFSET;
					if (CFG(output_pos) < 3)
					{
						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 1))
						{
							if ((CFG_OUTPUT_POS(alarm1_qty) == 0) ||
									(CFG_OUTPUT_POS(alarm1_qty) == 1))
								draw3_pop_tt_on (data_221, NULL, menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)],
										menu_content + ALARM1, 18, 1, CFG_OUTPUT_POS(alarm1),
										0, CFG_OUTPUT_POS(alarm1_status) << 2, 19);
							else
								draw3_pop_tt_on (data_221, NULL, menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)],
										menu_content + ALARM1, 19, 1, CFG_OUTPUT_POS(alarm1),
										0, CFG_OUTPUT_POS(alarm1_status) << 2, 19);
						}
						else
						{ 
							if ((CFG_OUTPUT_POS(alarm1_qty) == 0) ||
									(CFG_OUTPUT_POS(alarm1_qty) == 1) ||
									(CFG_OUTPUT_POS(alarm1_qty) == 16))
								draw3_popdown (menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)], 1, 0);
							else
								draw3_popdown (menu_content[ALARM1 + 18], 1, 0);
						}
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							draw3_pop_tt (data_2211, NULL, 
									menu_content[GROUPA + CFG_ANALOG_POS(group)],
									menu_content + GROUPA, 4, 1, CFG_ANALOG_POS(group), 0);
						}
						else 
						{
							draw3_popdown (menu_content[GROUPA+CFG_ANALOG_POS(group)], 1, 0);
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
										menu_content + ALARM_POS, 16, 1, GROUP_VAL(point_pos), 0);
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
									if (UNIT_MM == CFG(unit))
									{
										cur_value = GROUP_VAL(mat_atten) / 1000.0;
										lower = 0.00;
										upper = 4002000.0/GROUP_VAL(velocity);
										step = tmpf;
										digit = 2;
										pos = 1;
										unit = UNIT_DB_MM;
									}
									else
									{
										cur_value = GROUP_VAL(mat_atten) /1000.0 / 0.03937;
										lower = 0.00;
										upper = 4002000.0/GROUP_VAL(velocity)/0.03937;
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
									if (UNIT_MM == CFG(unit))
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
					if(CFG(list)==0)
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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

					draw3_popdown(menu_content[OFF_ON + CFG(entry_image)],1,0);
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
						if (CFG(unit)==UNIT_MM)
						{
							cur_value = CFG(min_thickness)/1000.0;
							lower = 0.05;
							upper = (CFG(max_thickness)/1000.0)-0.01;
							step = tmpf;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(min_thickness)/1000.0*0.03937;
							lower = 0.002;
							upper = (CFG(max_thickness)/1000.0*0.03937)-0.005;
							step = tmpf/2.0;
							digit = 3;
							pos = 1;
							unit = UNIT_INCH;
						}

						draw3_digit_pressed (data_331, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if (CFG(unit)==UNIT_MM)
						{
							cur_value = CFG(min_thickness)/1000.0;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(min_thickness)/1000.0*0.03937;
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
						if(CFG(display) == A_SCAN || CFG(display) == S_SCAN)
						{	
							if (CFG(groupQty) == 1)
								menu_status = 0x01;
							draw3_pop_tt (data_401, NULL, 
									menu_content[GROUP + CFG(display_group)],
									menu_content + GROUP, 2, 1, CFG(display_group), menu_status);
						}
						else if ((CFG(display) == C_SCAN) || 
								(CFG(display) == A_B_C_SCAN) ||
								(CFG(display) == A_C_CC_SCAN))
						{
							draw3_pop_tt (data_4011, NULL, 
									menu_content[C_SCAN1 + CFG(c_scan1)],
									menu_content + C_SCAN1, 4, 1, CFG(c_scan1), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if (CFG(display) == A_S_CC_SCAN)
						{
							draw3_pop_tt (data_4012, NULL, 
									menu_content[C_SCAN1+CFG(c_scan11)],
									menu_content + C_SCAN1, 5, 1, CFG(c_scan11), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_pop_tt (data_4013, NULL, 
									menu_content[C_SCAN1+CFG(data1)],
									menu_content + C_SCAN1, 3, 1, CFG(data1), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}
						else if(CFG(display)==1 || CFG(display)==4 || CFG(display)==6)
						{
							gtk_widget_hide (pp->eventbox30[1]);
							gtk_widget_hide (pp->eventbox31[1]);
						}
					}
					else 
					{
						if(CFG(display) == A_SCAN || CFG(display) == S_SCAN)
						{
							draw3_popdown (menu_content[GROUP+CFG(display_group)], 1, 0);
						}
						else if ((CFG(display) == C_SCAN) || 
								(CFG(display) == A_B_C_SCAN) ||
								(CFG(display) == A_C_CC_SCAN))
						{
							draw3_popdown (menu_content[C_SCAN1+CFG(c_scan1)], 1, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if(CFG(display)==8)
							/*Display 为 A-S-[C]*/
						{
							draw3_popdown (menu_content[C_SCAN1+CFG(c_scan11)], 1, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_popdown (menu_content[C_SCAN1+CFG(data1)], 1, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}
						else if(CFG(display)==1 || CFG(display)==4 || CFG(display)==6)
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
								menu_content[GRID + CFG(grid)],
								menu_content + GRID, 6, 1, CFG(grid), 0);
					else 
						draw3_popdown (menu_content[GRID + CFG(grid)], 1, 0);
					break;
				case 2:/* Display -> Zoom -> Type p421 */
					pp->x_pos = 583, pp->y_pos = 200-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_421, NULL, 
								menu_content[TYPE + CFG_ZOOM_POS(zoom_type)],
								menu_content + TYPE, 6, 1, CFG_ZOOM_POS(zoom_type), 0x08);
					else 
						draw3_popdown (menu_content[TYPE + CFG_ZOOM_POS(zoom_type)], 1, 0);

					break;

				case 3:/*Display -> Color -> start p431 */
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

				case 4:/* Ascan->Color  p441 */
					switch(CFG(prop_scan))
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
								cur_value = CFG(compress)/1000.0;
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
								cur_value = CFG(compress)/1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_NONE;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 6);

							}
							break;
						case 2:
							draw3_popdown(menu_content[OFF_ON + CFG(ratio)],1,0);
							g_sprintf (temp,"%s", con2_p[4][4][8]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

							break;
						case 3:
							draw3_popdown(menu_content[OFF_ON + CFG(interpolation)],1,0);
							g_sprintf (temp,"%s", con2_p[4][4][9]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
							break;
						case 4:
							pp->x_pos = 600, pp->y_pos = 210;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_4414, NULL, 
										menu_content[FFT_COLOR+CFG(fft_color)],
										menu_content+FFT_COLOR, 4, 1, CFG(fft_color), 0x08);
							else 
								draw3_popdown (menu_content[FFT_COLOR+CFG(fft_color)], 1, 0);
							g_sprintf (temp,"%s", con2_p[4][4][10]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
							break;
						case 5:
							pp->x_pos = 575, pp->y_pos = 230;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_4415, NULL, 
										menu_content[ORIENTATION + CFG(orientation)],
										menu_content+ORIENTATION, 2, 1, CFG(orientation), 0);
							else 
								draw3_popdown (menu_content[ORIENTATION + CFG(orientation)], 1, 0);
							g_sprintf (temp,"%s", con2_p[4][4][11]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
							break;

						default:break;
					}

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
						if(UNIT_MM == CFG(unit))
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
						if(UNIT_MM == CFG(unit))
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
					if (GROUP_VAL(db_ref))
						content_pos = 6;
					else
						content_pos = 0;

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
						lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
						upper = 86.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
						step = tmpf;
						digit = 1;
						pos = 1;
						unit = UNIT_DB;
						draw3_digit_pressed (data_100, units[unit], cur_value ,
								lower, upper, step, digit, p, pos, content_pos);
					}
					else 
					{
						cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
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
						if(UNIT_MM == CFG(unit))
						{
							cur_value = get_part_thickness(pp->p_config) / 1000.0;
							lower = 0.05;
							if (get_part_geometry(pp->p_config) == PLATE_PART)
								upper = 1000.0;
							else
								upper = get_part_diameter (pp->p_config) / 2000.0;
							step = tmpf;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = get_part_thickness(pp->p_config) / 1000.0*0.03937;
							lower = 0.002;
							upper = get_part_diameter(pp->p_config) / 2000.0*0.03937;
							step = tmpf*0.03937;
							digit = 3;
							pos = 1;
							unit = UNIT_INCH;
						}
						draw3_digit_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						if(UNIT_MM == CFG(unit))
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
						cur_value = (gfloat) (GROUP_VAL(pulser));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (GROUP_VAL(pulser));
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
							(CFG(auto_program) == AUTO_FOCAL_ON))
						/* 聚焦法则自动计算开启时并且是角度扫查, Max Angle 才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							/* 计算lower为妙 */
							cur_value = LAW_VAL(Angle_max) / 100.0;
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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

					if(CFG(auto_program) == AUTO_FOCAL_ON)
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
								digit = 2;
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
								digit = 2;
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
					if(CFG(auto_program) == AUTO_FOCAL_ON)/* 聚焦法则自动计算开启时, First Element才可调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = LAW_VAL(First_tx_elem);
							lower = 1.0;
							/* 计算最大值 */
							upper = GROUP_VAL(probe.Elem_qty) - LAW_VAL(Elem_qty) + 1;
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
					if (CFG(auto_program) == AUTO_FOCAL_ON )
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
								menu_content[POLARITY + CFG(polarity)],
								menu_content + POLARITY, 2, 1, CFG(polarity), 0);
					else 
						draw3_popdown (menu_content[POLARITY + CFG(polarity)], 1, 0);
					gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					break;

				case 1:/*Scan -> Inspection -> scan  p711 */
					pp->x_pos = 580, pp->y_pos = 201-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if(CFG(i_type)==1)
							draw3_pop_tt (data_711, NULL, 
									menu_content[I_SCAN+CFG(i_scan)],
									menu_content+I_SCAN, 3, 1, CFG(i_scan), 0x01);
						else
							draw3_pop_tt (data_711, NULL, 
									menu_content[I_SCAN+CFG(i_scan)],
									menu_content+I_SCAN, 3, 1, CFG(i_scan), 0);
					}
					else 
						draw3_popdown (menu_content[I_SCAN+CFG(i_scan)], 1, 0);
					break;

				case 2:/*Scan -> Area -> scan end  p721 */
					/* 当前步进 */
					switch (TMP(scan_end_reg))
					{
						case 0:	tmpf = 0.001 * CFG(scan_resolution); break;
						case 1:	tmpf = 0.01 * CFG(scan_resolution); break;
						case 2:	tmpf = 0.1 * CFG(scan_resolution); break;
						default:break;
					}

					if(CFG(i_type)==0 || CFG(i_type)==1)
						/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							if(CFG(unit) == UNIT_MM)
							{
								cur_value = CFG(scan_end)/1000.0;
								lower = CFG(scan_start)/1000.0;
								upper = 100000.0;
								step = tmpf;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(scan_end)/1000.0 * 0.03937;
								lower = CFG(scan_start)/1000.0 * 0.03937;
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
							if(CFG(unit) == UNIT_MM)
							{
								cur_value = CFG(scan_end)/1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(scan_end)/1000.0*0.03937;
								digit = 3;
								pos = 1;
								unit = UNIT_INCH;
							}
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else  /* Inspection -> Type 选择 Helicoidal Scan  时  ????CFG(scan_start)的值有待确定 */
					{
						if(CFG(unit) == UNIT_MM)
						{
							CFG(scan_start) = 346.0;
							digit = 2;
							pos = 1;
							unit = UNIT_MM;
						}
						else
						{
							CFG(scan_start) = 346.0*0.03937;
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
					draw3_popdown (menu_content[OFF_ON + CFG(pause)], 1, 0);
					break;

				case 4:/*Scan -> data -> inspec.data  p741 */
					pp->x_pos = 545, pp->y_pos = 202;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_741, NULL, 
								menu_content[INSPEC_DATA + CFG(inspec_data)],
								menu_content + INSPEC_DATA, 2, 1, CFG(inspec_data), 0);
					else 
						draw3_popdown (menu_content[INSPEC_DATA + CFG(inspec_data)], 1, 0);


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
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][1][1]);
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						update_widget_bg(pp->eventbox30[1], backpic[6]);
					else
					{
						update_widget_bg(pp->eventbox30[1], backpic[7]);
					}

					gtk_widget_set_size_request (GTK_WIDGET(pp->entry), 115, 29);
					gtk_box_pack_start(GTK_BOX (pp->vbox221[1]),pp->entry,FALSE,FALSE,0);
					gtk_widget_modify_base(GTK_WIDGET (pp->entry), GTK_STATE_NORMAL, &color_text_base);
					//gtk_widget_modify_text(GTK_WIDGET (pp->entry), GTK_STATE_NORMAL, &color_yellow);
					gtk_widget_modify_fg(GTK_WIDGET (pp->entry), GTK_STATE_NORMAL, &color_white);



					//gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					//gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Report####");
					//gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
					gtk_widget_show (GTK_WIDGET (pp->entry));
					//gtk_widget_show (pp->data3[1]);
					break;

				case 2:/*File -> format -> probe  p821 */
					draw3_popdown (menu_content[OFF_ON + CFG(format_probe)], 1, 0);
					break;

				case 3:/*File -> user field -> enable  p831 */
					draw3_popdown (menu_content[OFF_ON + CFG(enable)], 1, 0);
					break;

				case 4:/*File -> notes-> edit header p841 */
					draw3_popdown(NULL,1,1);
					gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
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
						cur_value = pp->p_config->bright;
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
						cur_value = pp->p_config->bright;
						digit = 0;
						pos = 1;
						unit = UNIT_BFH;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 1:/*Preferences -> system -> data set p911*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n(YYYY/MM/DD)", con2_p[9][1][1]);
					gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "2010/12/09");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				case 2:/*Preferences -> service -> File Manager  P921 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw_dialog_all(DIALOG_FILE_MANAGE);
					draw3_popdown(NULL,1,1);
					break;
				case 3:/*Preferences -> options -> Option Key  p931 */

					draw3_popdown(NULL,1,1);
					gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					break;

				case 4:/*Preferences -> Network -> IP Address  p941*/
					/* 格式化字符串 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw_dialog_all (DIALOG_MASK);
					}
					else
					{
						inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
						strcpy(ifr.ifr_name, "eth0");
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
				default:break;
			}
			break;
		default:break;
	}

}

void draw3_data2(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf = 0.0;
	gchar *str = NULL;
	guint menu_status = 0;

	gfloat cur_value=0.0, lower=0, upper=0, step=0, max_tmp=0, max_tmp1=0, temp_beam=0;
	guint digit, pos, unit, temp_qty =0, temp_pos = 0;

	//	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:	/* p002 */
					pp->x_pos = 590, pp->y_pos = 330;
					if ( pp->start_qty == 1 )
					{
						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_002, NULL, 
									menu_content[OPERATION + pp->operation_pos],
									menu_content + OPERATION, 2, 2, pp->operation_pos, 0);
						else 
							draw3_popdown (menu_content[OPERATION + pp->operation_pos], 2, 0);
					}
					else if ( pp->start_qty == 2 )
					{
						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_0021, NULL, 
									menu_content[WGROUP + pp->wgroup_pos],
									menu_content + WGROUP, 3, 2, pp->wgroup_pos, 0);
						else 
							draw3_popdown (menu_content[WGROUP + pp->wgroup_pos], 2, 0);

						str = g_strdup_printf ("%s", con2_p[0][0][7]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						if (str)
							g_free (str);
					}
					else if ( pp->start_qty == 3 )
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_501, NULL, 
									menu_content[GROUP_MODE_P + GROUP_VAL(group_mode)],
									menu_content + GROUP_MODE, 2, 2, GROUP_VAL(group_mode), 0);
						else 
							draw3_popdown (menu_content[GROUP_MODE_P + GROUP_VAL(group_mode)], 2, 0);

						str = g_strdup_printf ("%s", con2_p[0][0][8]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						if (str)
							g_free (str);
					}
					else if (pp->start_qty == 4)
					{
						if(GROUP_VAL(group_mode) == PA_SCAN)
						{
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
								cur_value = (gfloat) (GROUP_VAL(pulser));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 2;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, p, pos, 9);
							}
							else 
							{
								cur_value = (gfloat) (GROUP_VAL(pulser));
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
								if (GROUP_VAL (group_mode) == PA_SCAN)
									draw3_pop_tt (data_111, NULL, 
											menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)],
											menu_content + TX_RX_MODE, 3, 2, GROUP_VAL(tx_rxmode), 0x05);
								else if (GROUP_VAL (group_mode) == UT_SCAN)
									draw3_pop_tt (data_111, NULL, 
											menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)],
											menu_content + TX_RX_MODE, 4, 2, GROUP_VAL(tx_rxmode), 0x00);
							}
							else 
								draw3_popdown (menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)], 2, 0);

							str = g_strdup_printf ("%s", con2_p[0][0][16]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							if (str)
								g_free (str);							
						}
					}
					else if (( pp->start_qty == 5 )||( pp->start_qty == 6 ))
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_502, NULL, 
									menu_content[PROB_SELECT + CFG(probe_select)],
									menu_content + PROB_SELECT, 2, 2, CFG(probe_select), 0);
						else 
							draw3_popdown (menu_content[PROB_SELECT + CFG(probe_select)], 2, 0);

						str = g_strdup_printf ("%s", con2_p[0][0][10]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						if (str)
							g_free (str);
					}

					else if (pp->start_qty == 7)
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
							cur_value = GROUP_VAL(scan_offset)/10.0;
							lower = -100000.0;
							upper = 9999999.0;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_510, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);
						}
						else 
						{
							cur_value = GROUP_VAL(scan_offset)/10.0;
							digit = 1;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
						}
					}
					else if (pp->start_qty == 8)
					{
						draw3_popdown_offset (NULL, 2, 1, 15 );
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[2]);
						gtk_widget_hide (pp->eventbox31[2]);
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
						g_sprintf (TMP (velocity_data[2]), "%d m/s", GROUP_VAL(velocity) / 100);
						const gchar **tp = (const gchar **)(TMP(velocity_data_p));
						if (GROUP_VAL (velocity) == get_material_lw(pp->p_config )) 
						{
							temp_pos = 0;
							temp_qty = 2;
						}
						else if (GROUP_VAL (velocity) == get_material_sw(pp->p_config)) 
						{
							temp_pos = 1;
							temp_qty = 2;
						}
						else
						{
							temp_pos = 2;
							temp_qty = 3;
						}

						if (CFG(auto_program) == AUTO_FOCAL_ON)
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
						if (CFG(auto_program) == AUTO_FOCAL_ON) /* 聚焦法则自动计算为 on 时，Element Qty 才可调节 */
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
						if(CFG(auto_program) == AUTO_FOCAL_ON)
							/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								/* 最大不能超过最大Angle_max */
								cur_value = LAW_VAL (Angle_min) / 100.0;
								/* 计算lower为妙 */
								temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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
						case 1:
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								draw3_pop_tt (data_022, NULL, 
										menu_content[CTYPE + pp->ctype_pos],
										menu_content+CTYPE, 3, 2, pp->ctype_pos, 0x0);
							else 
								draw3_popdown (menu_content[CTYPE + pp->ctype_pos], 2, 0);
							break;
						case 2:
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
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
									cur_value =GROUP_VAL(ref_ampl)/100.0;
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
									cur_value = GROUP_VAL(ref_ampl)/100.0;
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
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
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
										if (UNIT_MM == CFG(unit)) 
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的起位数值mm */
											lower = (BEAM_INFO(0,beam_delay) /1000.0) * (GROUP_VAL(velocity) / 200000.0);
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);
											step = tmpf * (GROUP_VAL(velocity) / 200000.0);
											digit = 2;
											pos = 2;
											unit = UNIT_MM;
										}
										else 
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											digit = 3;
											pos = 2;
											unit = UNIT_INCH;
										}
									}
									draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
								}
								else 
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == CFG(unit))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 2;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 2;
										}
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 10);
								}
							}
							break;
						case 4:
							switch (TMP(cstart_reg))
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:	tmpf = 10.0; break;
								default:break;
							}

							if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 0)||(pp->cmode_pos == 1)||(pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
							{
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == CFG(unit)) 
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的起位数值mm */
											lower = (BEAM_INFO(0,beam_delay) /1000.0) * (GROUP_VAL(velocity) / 200000.0);
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);
											step = tmpf * (GROUP_VAL(velocity) / 200000.0);
											digit = 2;
											pos = 2;
											unit = UNIT_MM;
										}
										else 
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											digit = 3;
											pos = 2;
											unit = UNIT_INCH;
										}
									}
									draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
								}
								else 
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == CFG(unit))
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 2;
										}
										else
										{
											cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 2;
										}
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
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
									lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									upper = GAIN_MAX - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_pressed (data_100, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 9);
								}
								else 
								{
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
								}
							}
							break;
						case 5:
							if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 3)) )
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
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
									lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									upper = GAIN_MAX - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_pressed (data_100, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 9);
								}
								else 
								{
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
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
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
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
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
									lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									upper = GAIN_MAX - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									step = tmpf;
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_pressed (data_100, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 9);
								}
								else 
								{
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
									digit = 1;
									pos = 2;
									unit = UNIT_DB;
									draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
								}
							}
							break;

					}
					break;

				case 3:
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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						max_tmp = (MAX_RANGE_US - GROUP_VAL(start) / 1000.0);
						max_tmp1 = GROUP_VAL(point_qty) * 20.0;
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(range) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								lower = (GROUP_VAL(point_qty) / 100.0) * GROUP_VAL(velocity) / 200000.0;
								upper = MIN(max_tmp, max_tmp1) * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * (GROUP_VAL(velocity) / 200000.0);
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (GROUP_VAL(range) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								lower =	(GROUP_VAL(point_qty) / 100.0) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								upper = MIN(max_tmp, max_tmp1) * 0.03937 * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = GROUP_VAL(range) / 1000.0 ;
							lower =	GROUP_VAL(point_qty) / 100.0;
							upper = MIN(max_tmp, max_tmp1);										
							step = tmpf;
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
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(range) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 2;
							}
							else
							{
								cur_value = (GROUP_VAL(range) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								unit = UNIT_INCH;
								digit = 3;
								pos = 2;
							}
						}
						else
						{
							cur_value = GROUP_VAL(range) / 1000.0 ;
							unit = UNIT_US;
							pos = 2;
							digit = 2;
						}
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
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
							cur_value = GROUP_VAL(frequency) / 1000.0;
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
							str = g_strdup_printf ("%0.2f", GROUP_VAL(frequency) / 1000.0);
							draw3_pop_tt (data_112, NULL, str,
									menu_content + FREQUENCY, 13, 2, GROUP_VAL(freq_pos), 0);
							g_free (str);
						}
					}
					else 
					{
						cur_value = GROUP_VAL(frequency) / 1000.0;
						unit = UNIT_NULL;
						pos = 2;
						digit = 2;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 2:/* Rectifier 检波 P122 */
					pp->x_pos = 611, pp->y_pos = 288-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_122, NULL, 
								menu_content[RECTIFIER + GROUP_VAL(rectifier)],
								menu_content + RECTIFIER, 4, 2, GROUP_VAL(rectifier), 
								((GROUP_VAL(video_filter) == GAINR_ON) ? 0x01 : 0x0));
					else 
						draw3_popdown (menu_content[RECTIFIER + GROUP_VAL(rectifier)], 2, 0);
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
					draw3_popdown (menu_content[OFF_ON + GROUP_VAL(db_ref)], 2, 0);
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
						switch (TMP(agate_start_reg))
						{
							case 0:	tmpf = (GROUP_VAL(range) / 1000.0) / 320.0; break;
							case 1:	tmpf = (GROUP_VAL(range) / 1000.0) / 20.0 ; break;
							case 2:	tmpf = (GROUP_VAL(range) / 1000.0) / 10.0 ; break;
							case 3:	tmpf = (GROUP_VAL(range) / 1000.0) / 1.0  ; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if (UNIT_MM == CFG(unit)) 
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的起位数值mm */
									lower = (BEAM_INFO(0,beam_delay) /1000.0) * GROUP_VAL(velocity) / 200000.0;
									upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);
									step = tmpf * (GROUP_VAL(velocity) / 200000.0);
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else 
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
									lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
									upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
									step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
							}
							else 
							{
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
								if (UNIT_MM == CFG(unit))
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
									unit = UNIT_MM;
									digit = 2;
									pos = 2;
								}
								else
								{
									cur_value = (GROUP_GATE_POS(start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
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
							if (GROUP_VAL(gate_pos) != GATE_B) /* 闸门选择B时候 有3个同步选项 */
								draw3_pop_tt (data_2021, NULL,
										menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
										menu_content + SYNCHRO, 2, 2, GROUP_GATE_POS(synchro), 0);
							else
								draw3_pop_tt (data_2021, NULL, 
										menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
										menu_content + SYNCHRO, 3, 2, GROUP_GATE_POS(synchro), 0);
						}
						else 
							draw3_popdown (menu_content[SYNCHRO +GROUP_GATE_POS(synchro)], 2, 0);
						str = g_strdup_printf ("%s", con2_p[2][0][6]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						if (str)
							g_free (str);
					}
					gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;
				case 1:/* Condition GroupA P212 TAN1 */
					pp->x_pos = 535, pp->y_pos = 285-YOFFSET;
					if ((CFG_ALARM_POS(groupa) == CFG_ALARM_POS(groupb)) &&
							CFG_ALARM_POS(conditionb))
						menu_status = 1<<(CFG_ALARM_POS(conditionb));
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_212, NULL, menu_content[CONDITIONA +	CFG_ALARM_POS(conditiona)],
								menu_content + CONDITIONA, 9, 2, CFG_ALARM_POS(conditiona), menu_status);
					else 
						draw3_popdown (menu_content[CONDITIONA + CFG_ALARM_POS(conditiona)], 2, 0);
					break;
				case 2:/* Output->Count or Data P222 */
					pp->x_pos = 578, pp->y_pos = 300;
					/* 当前步进 */
					if (CFG(output_pos) < 3)
					{
						switch (TMP(count_reg))
						{
							case 0:	tmpf = 1.00; break;
							case 1:	tmpf = 10.00; break;						
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = CFG_OUTPUT_POS(count);
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
							cur_value = CFG_OUTPUT_POS(count);
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
									menu_content[DATA + CFG_ANALOG_POS(data)],
									menu_content + DATA, 4, 2, CFG_ANALOG_POS(data), 0);
						}
						else 
						{
							draw3_popdown (menu_content[DATA + CFG_ANALOG_POS(data)], 2, 0);
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
								cur_value =GROUP_VAL(ref_ampl)/100.0;
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
								cur_value = GROUP_VAL(ref_ampl)/100.0;
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
									if (UNIT_MM == CFG(unit))
									{
										cur_value = GROUP_VAL(position) / 1000.0;
										lower = 0.0;
										upper = GROUP_VAL(velocity)/100.0 * 4.95;
										step = tmpf;
										digit = 2;
										pos = 2;
										unit = UNIT_MM;
									}
									else
									{
										cur_value = GROUP_VAL(position) / 1000.0 * 0.03937;
										lower = 0.0;
										upper = GROUP_VAL(velocity)/100.0 * 4.95 * 0.03937;
										step = tmpf/2.0;
										digit = 3;
										pos = 2;
										unit = UNIT_INCH;
									}
								}
								else
								{
									cur_value = GROUP_VAL(position) / 1000.0 * 0.03937/0.1159;
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
									if (UNIT_MM == CFG(unit))
									{
										cur_value = GROUP_VAL(position)/1000.0;
										digit = 2;
										pos = 2;
										unit = UNIT_MM;
									}
									else
									{
										cur_value = GROUP_VAL(position)/1000.0 * 0.03937;
										digit = 3;
										pos = 2;
										unit = UNIT_INCH;
									}
								}
								else
								{
									cur_value = GROUP_VAL(position) / 1000.0 * 0.03937/0.1159;
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
									if (UNIT_MM == CFG(unit))
									{
										cur_value =GROUP_VAL(delay)/1000.0;
										lower = 0.0;
										upper = GROUP_VAL(velocity) / 2000.0;
										step = tmpf;
										digit = 2;
										pos = 2;
										unit = UNIT_MM;
									}
									else
									{
										cur_value =GROUP_VAL(delay)/1000.0*0.03937;
										lower = 0.0;
										upper = GROUP_VAL(velocity) / 2000.0*0.03937;
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
									if (UNIT_MM == CFG(unit))
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
				case 0:/*Measurements -> Reading -> Field 1 p302 */
					pp->x_pos = 314, pp->y_pos = 287-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_302, NULL, 
								field[CFG(field1)],
								field1, 4, 2, CFG(field1), 0);
					else 
						draw3_popdown (field[CFG(field1)], 2, 0);

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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0;
									lower = 0.0;
									upper = 422.92;
									step = tmpf;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference)/1000.0*0.03937;
									lower = 0.0;
									upper = 422.92*0.03937;
									step = tmpf/2.0;
									digit = 3;
									pos = 2;
									unit = UNIT_INCH;
								}
								draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, p, pos, 3);
							}
							else	/* ???? */
							{
								cur_value = GROUP_VAL(u_reference)/1000.0;
								lower = 0.0;
								upper = 422.92;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_US;
							}
						}
						else 
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0;
									digit = 2;
									pos = 2;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference)/1000.0*0.03937;
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
						if(UNIT_MM == CFG(unit))
						{
							cur_value = CFG(max_thickness)/1000.0;
							lower = (CFG(min_thickness)/1000.0) + 0.01;
							upper = 99999.00;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(max_thickness)/1000.0 * 0.03937;
							lower = (CFG(min_thickness)/1000.0 * 0.03937) + 0.005;
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
						if(UNIT_MM == CFG(unit))
						{
							cur_value = CFG(max_thickness)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(max_thickness)/1000.0 * 0.03937;
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
				case 0:/* C-Scan2  p402*/
					pp->x_pos = 580, pp->y_pos = 330;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))/*选中402这个位置*/
					{
						if(CFG(display)==7)
							/*Display 为 A-C-[C]*/
						{
							draw3_pop_tt (data_402, NULL, 
									menu_content[C_SCAN1+CFG(c_scan2)],
									menu_content + C_SCAN1, 5, 2, CFG(c_scan2), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

						}
						else if(CFG(display)==8)
							/*Display 为 A-S-[C]*/
						{
							draw3_pop_tt (data_401, NULL, 
									menu_content[GROUP+CFG(display_group)],
									menu_content + GROUP, 2, 2, CFG(display_group), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

						}
						else if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_pop_tt (data_4021, NULL, 
									menu_content[DATA2+CFG(data2)],
									menu_content + DATA2, 4, 2, CFG(data2), 0);
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
						if(CFG(display)==7)/* Display 为 A-C-[C] */
						{
							draw3_popdown (menu_content[C_SCAN1+CFG(c_scan2)], 2, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}

						else if(CFG(display)==8)/* Display 为 A-S-[C] */
						{
							draw3_popdown (menu_content[GROUP+CFG(display_group)], 2, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}

						else if(CFG(display)==10)/* Display 为 Strip Chart-[A] */
						{
							draw3_popdown (menu_content[DATA2+CFG(data2)], 2, 0);
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
					draw3_popdown(menu_content[OFF_ON + CFG(sizing_curves)],2,0);
					break;
				case 2:/* p422 */
					if(CFG_ZOOM_POS(zoom_type) == 0 || CFG_ZOOM_POS(zoom_type) == 1)
					{
						/* 当前步进 */
						switch (TMP(start_usound_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = CFG_ZOOM_POS(start_usound)/1000.0;
							lower = 0.0;
							upper = 85.25;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_422, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(start_usound)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else if(CFG_ZOOM_POS(zoom_type) == 2)
					{
						/* 当前步进 */
						switch (TMP(center_usound_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = CFG_ZOOM_POS(center_usound)/1000.0;
							lower = 0.0;
							upper = 100.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_4221, units[unit], cur_value , lower, upper, step, digit, p, pos, 8);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(center_usound)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
						}
					}
					break;


				case 3:/*Display -> Color -> end p432 */
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

				case 4:/* Display -> Properties -> Envelope p442 */

					switch(CFG(prop_scan))
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
						case 1:
							draw3_popdown(menu_content[OFF_ON + CFG(optimum)],2,0);
							g_sprintf (temp,"%s", con2_p[4][4][7]);
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
								menu_content[PROB_SELECT + CFG(probe_select)],
								menu_content + PROB_SELECT, 2, 2, CFG(probe_select), 0);
					else 
						draw3_popdown (menu_content[PROB_SELECT + CFG(probe_select)], 2, 0);
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
						case 0:	tmpf = (GROUP_VAL(range) / 1000.0) / 320.0; break;
						case 1:	tmpf = (GROUP_VAL(range) / 1000.0) / 20.0 ; break;
						case 2:	tmpf = (GROUP_VAL(range) / 1000.0) / 10.0 ; break;
						case 3:	tmpf = (GROUP_VAL(range) / 1000.0) / 1.0  ; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
						{
							if (UNIT_MM == CFG(unit)) 
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的起位数值mm */
								lower = (BEAM_INFO(0,beam_delay) /1000.0) * (GROUP_VAL(velocity) / 200000.0);
								upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * (GROUP_VAL(velocity) / 200000.0);
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
							}
							else 
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								lower = (BEAM_INFO(0,beam_delay) / 1000.0) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
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
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 2;
							}
							else
							{
								cur_value = (GROUP_VAL(gate[0].start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
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
							if(UNIT_MM == CFG(unit))
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
								if(UNIT_MM == CFG(unit))
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
								if(UNIT_MM == CFG(unit))
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
					if (GROUP_VAL(tx_rxmode) == PULSE_ECHO)	/* 脉冲回波模式不可以调节 */
					{
						gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
					}
					upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = (gfloat) (GROUP_VAL(receiver));
						lower = 1.0;
						step = tmpf;
						digit = 0;
						pos = 2;
						str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (GROUP_VAL(receiver));
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
							(CFG(auto_program) == AUTO_FOCAL_ON))
						/* 角度扫查时开始自动计算聚焦法则时候可以调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = LAW_VAL (Angle_step) / 100.0;
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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
					if(CFG(auto_program) == AUTO_FOCAL_ON)
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
					if ((CFG(auto_program) == AUTO_FOCAL_ON) &&
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
								menu_content[E_TYPE + CFG(e_type)],
								menu_content + E_TYPE, 9, 2, CFG(e_type), 0);
					else 
						draw3_popdown (menu_content[E_TYPE + CFG(e_type)], 2, 0);
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					break;

				case 1:/*Scan -> Inspection -> Index p712 */
					pp->x_pos = 570, pp->y_pos = 305;
					if(CFG(i_type)==0)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_712, NULL, 
									menu_content[I_INDEX + 4],
									menu_content + I_INDEX, 5, 2, 4, 0);
						else 
							draw3_popdown (menu_content[I_INDEX + 4], 2, 0);

						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}

					else if(CFG(i_type)==1)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_712, NULL, 
									menu_content[I_INDEX + CFG(i_index)],
									menu_content + I_INDEX, 5, 2, CFG(i_index), 0);
						else 
							draw3_popdown (menu_content[I_INDEX + CFG(i_index)], 2, 0);

						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							draw3_pop_tt (data_712, NULL, 
									menu_content[I_INDEX + CFG(i_index)],
									menu_content + I_INDEX, 5, 2, CFG(i_index), 0x10);
						else 
							draw3_popdown (menu_content[I_INDEX + CFG(i_index)], 2, 0);
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
						if(CFG(unit) == UNIT_MM)
						{
							cur_value = CFG(scan_resolution)/1000.0;
							lower = 1.0;
							upper = 357.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(scan_resolution)/1000.0*0.03937;
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
						if(CFG(unit) == UNIT_MM)
						{
							cur_value = CFG(scan_resolution)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(scan_resolution)/1000.0*0.03937;
							digit = 3;
							pos = 2;
							unit = UNIT_INCH;
						}
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 3:/*Scan -> start -> start  p732 */
					if (CFG(pause)==0)
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
								menu_content[PAPER_SIZE+CFG(paper_size)],
								menu_content+PAPER_SIZE, 4, 2,CFG(paper_size), 0);
					else 
						draw3_popdown (menu_content[PAPER_SIZE+CFG(paper_size)], 2, 0);
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);

					break;

				case 2:/* File -> format -> setup  p822 */
					draw3_popdown (menu_content[OFF_ON + CFG(format_setup)], 2, 0);
					break;

				case 3:/*File -> user field -> label p832 */
					draw3_popdown (menu_content[U_LABEL + CFG(file_select)], 2, 0);
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
					draw3_popdown (NULL, 2, 1);
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					break;

				case 1:/*Preferences -> system -> select key  p912 */
					pp->x_pos = 611, pp->y_pos = 295-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_912, NULL, 
								menu_content[SELECT_K + CFG(select_key)],
								menu_content+SELECT_K, 9, 2,CFG(select_key), 0);
					else 
						draw3_popdown (menu_content[SELECT_K+CFG(select_key)], 2, 0);
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);


					break;


				case 2:/*Preferences -> service -> Import/Export p922 */
					draw3_popdown(NULL,2,1);
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					break;

				case 3:/*Preferences -> Options -> Remote Desktop p932 */
					draw3_popdown (menu_content[OFF_ON+CFG(remote_desktop)], 2, 0);
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					break;

				case 4:/*Preferences -> network -> Apply p942 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw_warning(1, "test");
					draw3_popdown(NULL,2,1);
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
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

	gfloat cur_value=0.0, lower, upper, step;
	guint digit, pos, unit, content_pos, menu_status = 0, temp_beam, temp_beam1;


	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/* p003 */

					if (pp->start_qty == 4)
					{
						if(GROUP_VAL(group_mode) == PA_SCAN)
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
								cur_value = (gfloat) (GROUP_VAL(receiver));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 3;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, p, pos, 19);
							}
							else 
							{
								cur_value = (gfloat) (GROUP_VAL(receiver));
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
								cur_value = (gfloat) (GROUP_VAL(pulser));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 3;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, p, pos, 17);
							}
							else 
							{
								cur_value = (gfloat) (GROUP_VAL(pulser));
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
						if(CFG(auto_detect))
						{
							if(CFG(probe_select)==0)
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
							else if(CFG(probe_select)==1)
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
							if(CFG(probe_select)==0)
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
							else if( CFG(probe_select)==1 )
							{
								draw3_popdown(NULL,3,1);
								g_sprintf (temp,"%s", con2_p[5][0][6]);
								gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
							}
						}
						if( GROUP_VAL(group_mode) && CFG(auto_detect))	/* 自动检测开启时同时又在PA模式时，探头不可选 */
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
						if (CFG(probe_select)==0)
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								draw_dialog_all (DIALOG_WEDGE);
							else
								draw3_popdown(GROUP_VAL(wedge.Model), 3, 0);

							g_sprintf (temp,"%s", con2_p[5][0][4]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}
						else if(CFG(probe_select)==1)
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
							cur_value = GROUP_VAL(index_offset)/10.0;
							lower = -1000000.0;
							upper =  9999999.0;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_511, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
						}
						else 
						{
							cur_value =GROUP_VAL(index_offset)/10.0;
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
									menu_content + MATERIAL, 24, 3, CFG(part.Material_pos), 0);
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
						if(CFG(auto_program) == AUTO_FOCAL_ON)/* 聚焦法则自动计算开启时, First Element才可调节 */
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
								(CFG(auto_program) == AUTO_FOCAL_ON))
							/* 聚焦法则自动计算开启时并且是角度扫查, Max Angle 才可调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								/* 计算lower为妙 */
								cur_value = LAW_VAL(Angle_max) / 100.0;
								temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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
						case 1:
							if(pp->ctype_pos == 0)
							{
								gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
								gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
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
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
									lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									upper = GAIN_MAX - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
									step = tmpf;
									digit = 1;
									pos = 3;
									unit = UNIT_DB;
									draw3_digit_pressed (data_100, units[unit], cur_value ,
											lower, upper, step, digit, p, pos, 9);
								}
								else 
								{
									cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
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
										if (UNIT_MM == CFG(unit))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
											lower = 3.2 * GROUP_VAL(velocity) / 200000.0;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 : 
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (GROUP_VAL(velocity) / 200000.0);
											step = tmpf * (GROUP_VAL(velocity) / 200000.0);
											digit = 2;
											pos = 3;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											lower =	3.2 * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (GROUP_VAL(velocity) / 200000.0);
											step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											digit = 3;
											pos = 3;
											unit = UNIT_INCH;
										}
									}
									draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos, 20);
								}
								else 
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == CFG(unit))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 3;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 3;
										}
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
										if (UNIT_MM == CFG(unit))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
											lower = 3.2 * GROUP_VAL(velocity) / 200000.0;
											upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 : 
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (GROUP_VAL(velocity) / 200000.0);
											step = tmpf * (GROUP_VAL(velocity) / 200000.0);
											digit = 2;
											pos = 3;
											unit = UNIT_MM;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											lower =	3.2 * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
													(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (GROUP_VAL(velocity) / 200000.0);
											step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
											digit = 3;
											pos = 3;
											unit = UNIT_INCH;
										}
									}
									draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos, 20);
								}
								else 
								{
									if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
									{
										if (UNIT_MM == CFG(unit))
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
											unit = UNIT_MM;
											digit = 2;
											pos = 3;
										}
										else
										{
											cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
											unit = UNIT_INCH;
											digit = 3;
											pos = 3;
										}
									}
									draw3_digit_stop (cur_value , units[unit], digit, pos, 20);
								}
							}
							else if((pp->ctype_pos == 2)&&(pp->scode_pos == 1))
							{
								draw3_popdown_offset(NULL, 3,1,29);
							}
							break;
						case 5:
							if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 3)) )
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
						cur_value = GROUP_VAL(wedge_delay) / 1000.0;
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
						cur_value = GROUP_VAL(wedge_delay) / 1000.0;
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
						if (GROUP_VAL(group_mode) == PA_SCAN)
							draw3_pop_tt (data_113, NULL, 
									menu_content[VOLTAGE + 3 + CFG(voltage_pa)],
									menu_content + VOLTAGE + 6, 2, 3, CFG(voltage_pa), 0);
						else if (GROUP_VAL (group_mode) == UT_SCAN)
							draw3_pop_tt (data_113, NULL, 
									menu_content[VOLTAGE + 3 + CFG(voltage_ut)],
									menu_content + VOLTAGE, 3, 3, CFG(voltage_ut), 0);
					}
					else 
					{
						if (GROUP_VAL(group_mode) == PA_SCAN)
							draw3_popdown (menu_content[VOLTAGE + 3 + CFG(voltage_pa)], 3, 0);
						else if (GROUP_VAL (group_mode) == UT_SCAN)
							draw3_popdown (menu_content[VOLTAGE + 3 + CFG(voltage_ut)], 3, 0);
					}
					break;
				case 2: /* Video Filter  P123 TAN1 */
					if (GROUP_VAL(rectifier) == RF_WAVE)
					{
						gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
					}
					draw3_popdown (menu_content[OFF_ON + GROUP_VAL(video_filter)], 3, 0);
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
							upper =	8192.0;	/* 最大值需要计算 TAN1 */
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
						switch (pp->p_tmp_config->gate_width_reg)
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
								if (UNIT_MM == CFG(unit))
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
									lower = 3.2 * GROUP_VAL(velocity) / 200000.0;
									upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 : 
											(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (GROUP_VAL(velocity) / 200000.0);
									step = tmpf * (GROUP_VAL(velocity) / 200000.0);
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
									lower =	3.2 * 0.03937 * GROUP_VAL(velocity) / 200000.0;
									upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
											(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (GROUP_VAL(velocity) / 200000.0);
									step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else 
							{
								cur_value = GROUP_GATE_POS(width) / 1000.0 ;
								lower =	3.2;
								upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) /1000.0 ) > 6400.0 ? 6400.0 : (MAX_RANGE_US - GROUP_VAL(start) / 1000.0));
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
								if (UNIT_MM == CFG(unit))
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
									unit = UNIT_MM;
									digit = 2;
									pos = 3;
								}
								else
								{
									cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
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
					gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;
				case 1:/*Operator  P213*/
					pp->x_pos = 609, pp->y_pos = 371-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_213, NULL,menu_content[OPERAT + CFG_ALARM_POS(operat)],
								menu_content + OPERAT, 2, 3, CFG_ALARM_POS(operat), 0);
					else 
						draw3_popdown (menu_content[OPERAT + CFG_ALARM_POS(operat)], 3, 0);
					break;

				case 2:/* Sound  p223 */
					pp->x_pos = 424, pp->y_pos = 370-YOFFSET;
					if (CFG(output_pos) < 3)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							draw3_pop_tt (data_223, NULL,menu_content[SOUND + 5 + CFG_OUTPUT_POS(sound)],
									menu_content + SOUND, 5, 3, CFG_OUTPUT_POS(sound), 0);
						else 
							draw3_popdown (menu_content[SOUND + 5 + CFG_OUTPUT_POS(sound)], 3, 0);
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
								cur_value =GROUP_VAL(amplitude)/1000.0;
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
								cur_value = GROUP_VAL(amplitude)/1000.0;
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
				case 0:/*Measurements -> reading -> field2  p303*/
					pp->x_pos = 314, pp->y_pos = 371-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_303, NULL, 
								field[CFG(field2)],
								field1, 4, 3, CFG(field2), 0);
					else 
						draw3_popdown (field[CFG(field2)], 3, 0);

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
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0;
									lower = 0.0;
									upper = GROUP_VAL(velocity)/100.0/20.85;
									step = tmpf;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference) / 1000.0 * 0.03937;
									lower = 0.0;
									upper = GROUP_VAL(velocity) / 100.0 / 20.85 * 0.03937;
									step = tmpf/2.0;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else	/* ????? us */
							{
								cur_value = GROUP_VAL(u_reference)/(GROUP_VAL(velocity))*200.0;
								lower = -0.1;
								upper = 95.91;
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
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_reference)/1000.0;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_reference)/1000.0*0.03937;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else
							{
								cur_value = GROUP_VAL(u_reference)/(GROUP_VAL(velocity))*200.0;
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0;
									lower = 0.0;
									upper = 422.92;
									step = tmpf;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937;
									lower = 0.0;
									upper = 422.92*0.03937;
									step = tmpf/2.0;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else
							{
								cur_value = GROUP_VAL(u_measure)/1000.0;
								lower = 0.0;
								upper = 422.92;
								step  = tmpf/2.0;
								digit = 3;
								pos   = 3;
								unit  = UNIT_US;

							}
							draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, p, pos, 4);
						}
						else 
						{
							if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
							{
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0;
									digit = 2;
									pos = 3;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937;
									digit = 3;
									pos = 3;
									unit = UNIT_INCH;
								}
							}
							else
							{
								cur_value = GROUP_VAL(u_measure)/1000.0;
								digit = 3;
								pos   = 3;
								unit  = UNIT_US;
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
								if(CFG(unit) == UNIT_MM)
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
								if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
						cur_value = CFG(echo_qty);
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
						cur_value = CFG(echo_qty);
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
						if(CFG(display)==7)
							/*Display 为 A-C-[C]*/
						{
							draw3_pop_tt (data_401, NULL, 
									menu_content[GROUP+CFG(display_group)],
									menu_content + GROUP, 2, 3, CFG(display_group), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);

						}
						else if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_pop_tt (data_403, NULL, 
									menu_content[DIS_MODE+CFG(dis_mode)],
									menu_content + DIS_MODE, 4, 3, CFG(dis_mode), 0);
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
						if(CFG(display)==7)/* Display 为 A-C-[C] */
						{
							draw3_popdown (menu_content[GROUP+CFG(display_group)], 3, 0);
							str = g_strdup_printf ("%s", con2_p[4][0][1]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
						}
						else if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							draw3_popdown (menu_content[DIS_MODE+CFG(dis_mode)], 3, 0);
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
					draw3_popdown(menu_content[OFF_ON + CFG(overlay_gate)],3,0);
					break;
				case 2:/* p423 */
					if( CFG_ZOOM_POS(zoom_type) == 0 )
					{
						/* 当前步进 */
						switch (TMP(end_usound_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							cur_value = CFG_ZOOM_POS(end_usound)/1000.0;
							lower = 30.5;
							upper = 89.04;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_423, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(end_usound)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					if( CFG_ZOOM_POS(zoom_type) == 1 || CFG_ZOOM_POS(zoom_type) == 2 )
					{
						/* 当前步进 */
						switch (TMP(range_usound_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							cur_value = CFG_ZOOM_POS(range_usound)/1000.0;
							lower = 30.5;
							upper = 89.04;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_4231, units[unit], cur_value , lower, upper, step, digit, p, pos, 6);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(range_usound)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
						}
					}
					break;


				case 3:/* Load color 调色板 P433 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw_dialog_all (DIALOG_COLOR_PALETTE);
					draw3_popdown(NULL, 3, 1);
					break;
				case 4:/* A_SCAN -> Source  P443 */
					pp->x_pos = 567, pp->y_pos =368-YOFFSET;
					switch(CFG(prop_scan))
					{
						case 0:	/* Ascan 时候的source */
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							{
								if ((CFG(display) == B_SCAN) ||	(CFG(display) == A_B_SCAN)||
										(CFG(display) == A_B_C_SCAN)|| (CFG(display) == A_B_S_SCAN))
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
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/* Probe/Part -> Select -> Probe 探头选择  P503 */
					if(CFG(auto_detect))
					{
						if(CFG(probe_select)==0)
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
						else if(CFG(probe_select)==1)
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
						if(CFG(probe_select)==0)
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
						else if( CFG(probe_select)==1 )
						{
							draw3_popdown(NULL,3,1);
							g_sprintf (temp,"%s", con2_p[5][0][6]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}
					}
					if( GROUP_VAL(group_mode) && CFG(auto_detect))	/* 自动检测开启时同时又在PA模式时，探头不可选 */
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
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								lower = 3.2 * GROUP_VAL(velocity) / 200000.0;
								upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 : 
										(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * (GROUP_VAL(velocity) / 200000.0);
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								lower =	3.2 * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
										(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								digit = 3;
								pos = 3;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = GROUP_VAL(gate[0].width) / 1000.0 ;
							lower =	3.2;
							upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) /1000.0 ) > 6400.0 ? 6400.0 : (MAX_RANGE_US - GROUP_VAL(start) / 1000.0));
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
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								unit = UNIT_MM;
								digit = 2;
								pos = 3;
							}
							else
							{
								cur_value = (GROUP_VAL(gate[0].width) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
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
								//								menu_content[MATERIAL + CFG(part.Material_pos)],
								get_material_name(pp->p_config),
								menu_content + MATERIAL, 24, 3, CFG(part.Material_pos), 0);
					else 
						//						draw3_popdown (menu_content[MATERIAL + CFG(part.Material_pos)], 3, 0);
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
							(CFG(auto_program) == AUTO_FOCAL_ON) 
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
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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
					if(CFG(auto_program) == AUTO_FOCAL_ON)
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
					if ((CFG(auto_program) == AUTO_FOCAL_ON))
						/* 自动计算聚焦法则时候可以调节 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(UNIT_MM == CFG(unit))
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
							if(UNIT_MM == CFG(unit))
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
						if(UNIT_MM == CFG(unit))
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
					if ((CFG(auto_program) == AUTO_FOCAL_ON) &&
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
					if (CFG(auto_program) == AUTO_FOCAL_OFF)
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
						cur_value = CFG(encoder_resolution)/1000.0;
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
						cur_value = CFG(encoder_resolution)/1000.0;
						digit = 3;
						pos = 3;
						unit = UNIT_STEP_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					break;
				case 1:/* 扫查移动速度 P713 */
					switch (TMP(scanspeed_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if(CFG(i_scan)==0)
						content_pos=0;
					else
						content_pos=6;

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						if(UNIT_MM == CFG(unit))
						{
							cur_value = CFG(scanspeed) / 1000.0;
							lower = 1.0;
							upper = 6000.0;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM_S;
						}
						else
						{
							cur_value = CFG(scanspeed) / 1000.0 * 0.03937;
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
						if(UNIT_MM == CFG(unit))
						{
							cur_value = CFG(scanspeed)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM_S;
						}
						else
						{
							cur_value = CFG(scanspeed)/1000.0 * 0.03937;
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
						case 0:	tmpf = 0.001 * CFG(index_resolution); break;
						case 1:	tmpf = 0.01 * CFG(index_resolution); break;
						case 2:	tmpf = 0.1 * CFG(index_resolution); break;
						default:break;
					}
					if(CFG(i_type)==1 || CFG(i_type)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							if(CFG(unit) == UNIT_MM)
							{
								cur_value = CFG(index_start)/1000.0;
								lower = -99999.0;
								upper = 99999.0;
								step = tmpf;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(index_start) / 1000.0 * 0.03937;
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
							if(CFG(unit) == UNIT_MM)
							{
								cur_value = CFG(index_start)/1000.0;
								digit = 2;
								pos = 3;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(index_start)/1000*0.03937;
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
						if(CFG(unit) == UNIT_MM)
						{
							cur_value = CFG(index_start);
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(index_start)/1000*0.03937;
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
				case 0:/*File -> File -> save data p803 */
					if(!CFG(file_storage))
						draw3_popdown(NULL,3,1);
					else
					{
						draw3_popdown(NULL,3,1);
						gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
					}
					gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					break;

				case 1:/*File -> report -> build p813 */
					draw3_popdown(NULL,3,1);
					gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					break;

				case 2:/*File -> format -> Note  p823 */
					draw3_popdown (menu_content[OFF_ON + CFG(format_note)], 3, 0);
					break;
				case 3:/*File -> user field -> content  p833*/
					draw3_popdown (menu_content[U_CONTENT + CFG(file_select)], 3, 0);
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
					pp->x_pos = 588, pp->y_pos = 392-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_903, NULL, 
								menu_content[SCHEME + CFG(scheme)],
								menu_content+SCHEME, 2, 3, CFG(scheme), 0);
					else 
						draw3_popdown (menu_content[SCHEME + CFG(scheme)], 3, 0);

					gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);

					break;

				case 1:/*Preferences -> system -> assign key  p913 */
					pp->x_pos = 547, pp->y_pos = 260-YOFFSET;
					if(CFG(select_key)<5)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							draw3_pop_tt (data_913, NULL, 
									menu_content[ASSIGN_K + CFG(assign_key)],
									menu_content+ASSIGN_K, 16, 3, CFG(assign_key), 0);
						else 
							draw3_popdown (menu_content[ASSIGN_K + CFG(assign_key)], 3, 0);
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							draw3_pop_tt (data_9131, NULL, 
									menu_content[ASSIGN_K_P + CFG(assign_key_p)],
									menu_content+ASSIGN_K_P, 5, 3, CFG(assign_key_p), 0);
						else 
							draw3_popdown (menu_content[ASSIGN_K_P + CFG(assign_key_p)], 3, 0);
					}
					gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
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

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0: /* p004 */
					if (pp->start_qty == 4)
					{
						if(GROUP_VAL(group_mode) == PA_SCAN)
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
								cur_value = (gfloat) (GROUP_VAL(receiver));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 4;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, p, pos, 18);
							}
							else 
							{
								cur_value = (gfloat) (GROUP_VAL(receiver));
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
							if (GROUP_VAL(tx_rxmode) == PULSE_ECHO)	/* 脉冲回波模式不可以调节 */
							{
								gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
								gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);
							}
							upper = (gfloat) (128 + 1 - LAW_VAL(Last_rx_elem));
							if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = (gfloat) (GROUP_VAL(receiver));
								lower = 1.0;
								step = tmpf;
								digit = 0;
								pos = 4;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, p, pos, 18);
							}
							else 
							{
								cur_value = (gfloat) (GROUP_VAL(receiver));
								digit = 0;
								pos = 4;
								str = g_strdup_printf ("( 1 to %d)", (guint)(upper));
								draw3_digit_stop (cur_value, str, digit, pos, 18);
							}
							g_free(str);
						}
					}

					else if (( pp->start_qty == 5 )&&(GROUP_VAL(group_mode)==1))/*PA模式时才有Auto Detect键*/
					{
						draw3_popdown (menu_content[OFF_ON + CFG(auto_detect)], 4, 0);
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
								draw3_digit_pressed (data_5121, units[unit], cur_value , lower, upper, step, digit, p, pos, 13);
							}
							else
							{
								str = g_strdup_printf ("%.1f", GROUP_VAL(skew)/100.0);
								draw3_pop_tt (data_512, NULL, 
										str, menu_content+PROB_SKEW, 5, 4, GROUP_VAL(skew_pos), 0);
								g_sprintf (temp,"%s", con2_p[0][0][13]);
								gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
								g_free(str);
							}
						}
						else 
						{
							cur_value = GROUP_VAL(skew)/100.0 ;
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
						if ((CFG(auto_program) == AUTO_FOCAL_ON) &&
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
								(CFG(auto_program) == AUTO_FOCAL_ON))
							/* 角度扫查时开始自动计算聚焦法则时候可以调节 */
						{
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							{
								cur_value = LAW_VAL (Angle_step) / 100.0;
								temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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
						if((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
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
					}
					else if (pp->cstart_qty == 5)
					{
						if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
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
								cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
								lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
								upper = GAIN_MAX - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
								step = tmpf;
								digit = 1;
								pos = 4;
								unit = UNIT_DB;
								draw3_digit_pressed (data_100, units[unit], cur_value ,
										lower, upper, step, digit, p, pos, 9);
							}
							else 
							{
								cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
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
						if (CFG(unit) == 0)
						{
							cur_value = GROUP_VAL(velocity) / 100.0;
							lower = 635.0;
							upper = 15240.0;
							step = tmpf;
							digit = 1;
							unit = UNIT_M_S;
						}
						else
						{
							cur_value = GROUP_VAL(velocity) / (100.0 * 25400 );
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
						if (CFG(unit) == 0)
						{
							cur_value = GROUP_VAL(velocity) / 100.0;
							digit = 1;
							unit = UNIT_M_S;
						}
						else
						{
							cur_value = GROUP_VAL(velocity) * 0.000000394;
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
							cur_value = GROUP_VAL(pulser_width) / 100.0;
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
							if (!GROUP_VAL(pw_pos))
							{
								/* 更新当前增益值显示 */
								str = g_strdup_printf ("%s %0.1f", 
										menu_content[PULSER_WIDTH + GROUP_VAL(pw_pos)], GROUP_VAL(pulser_width) / 100.0);
								draw3_pop_tt (data_114, NULL, 
										str, menu_content + PULSER_WIDTH, 2, 4, GROUP_VAL(pw_pos), 0);
								g_free(str);
							}
							else
							{
								str = g_strdup_printf ("%0.1f", GROUP_VAL(pulser_width) / 100.0);
								draw3_pop_tt (data_114, NULL, 
										str, menu_content + PULSER_WIDTH, 2, 4, GROUP_VAL(pw_pos), 0);
								g_free(str);
							}
						}
					}
					else 
					{
						/* 这个选中Auto 时候显示 Auto + 数值 */
						if (!GROUP_VAL(pw_pos))
						{
							/* Auto 时候计算脉冲宽度 */
							str = g_strdup_printf ("%s %0.1f", 
									menu_content[PULSER_WIDTH + GROUP_VAL(pw_pos)], GROUP_VAL(pulser_width) / 100.0);
							draw3_popdown (str, 4, 0);
							g_free(str);
						}
						else 
						{
							cur_value = GROUP_VAL(pulser_width) / 100.0;
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
								menu_content[AVERAGING + GROUP_VAL(averaging)],
								menu_content + AVERAGING, 5, 4, GROUP_VAL(averaging), 0);
					else 
						draw3_popdown (menu_content[AVERAGING + GROUP_VAL(averaging)], 4, 0);
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
							cur_value = BEAM_INFO(0,beam_delay)/1000.0;
							lower = 1.0;
							upper = 1000.0;
							step = tmpf;
							digit = 2;
							pos = 4;
							unit = UNIT_US;
							draw3_digit_pressed (data_134, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = BEAM_INFO(0,beam_delay)/1000.0;
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

					cur_value = (GROUP_VAL(range) / 10.0) / GROUP_VAL(point_qty);
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
						if (GROUP_VAL(rectifier) != RF_WAVE)
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
					gtk_widget_queue_draw(pp->draw_area->drawing_area);
					break;
				case 1:/*Group B   p214 */
					pp->x_pos = 632, pp->y_pos = 456-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_214, NULL,menu_content[GROUPA + CFG_ALARM_POS(groupb)],
								menu_content + GROUPA, 4, 4, CFG_ALARM_POS(groupb), 0);
					else 
						draw3_popdown (menu_content[GROUPA + CFG_ALARM_POS(groupb)], 4, 0);
					break;

				case 2:/* Output->delay P224 */
					if (CFG(output_pos) < 3)
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
							cur_value = CFG_OUTPUT_POS(delay) / 1000.0;
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
							cur_value = CFG_OUTPUT_POS(delay)/1000.0;
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
				case 0:/*Measurements -> Reading -> Field 3  p304 */
					pp->x_pos = 314, pp->y_pos = 455-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_304, NULL, 
								field[CFG(field3)],
								field1, 4, 4, CFG(field3), 0);
					else 
						draw3_popdown (field[CFG(field3)], 4, 0);

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
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0;
									lower = 0.0;
									upper = 422.92;
									step = tmpf;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937;
									lower = 0.0;
									upper = 422.92*0.03937;
									step = tmpf;
									digit = 2;
									pos = 4;
									unit = UNIT_INCH;
								}
							}
							else	/*????*/
							{
								cur_value = GROUP_VAL(u_measure)/1000.0;
								lower = 0.0;
								upper = 422.92;
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
								if(CFG(unit) == UNIT_MM)
								{
									cur_value = GROUP_VAL(u_measure)/1000.0;
									digit = 2;
									pos = 4;
									unit = UNIT_MM;
								}
								else
								{
									cur_value = GROUP_VAL(u_measure)/1000.0*0.03937;
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
# if 0
					else if( GROUP_VAL(selection)==4)
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
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = GROUP_VAL(s_reference)/100.0;
							lower = -1000000.0;
							upper =  1000000.0;
							step = tmpf;
							digit = 1;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3111, units[unit], cur_value , lower, upper, step, digit, p, pos, 6);
						}
						else 
						{
							cur_value = GROUP_VAL(s_reference)/100.0;
							digit = 1;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
						}
					}
#endif
					else if (GROUP_VAL(selection)==4)
					{
						gtk_widget_hide(pp->eventbox30[4]);
						gtk_widget_hide(pp->eventbox31[4]);
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
							if(CFG(unit) == UNIT_MM)
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
							if(CFG(unit) == UNIT_MM)
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
				case 2:/*Measurements -> Table -> Select Entry  P324*/
					/* 当前步进 */
					switch (pp->p_tmp_config->entry_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;

						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						cur_value = pp->p_config->entry_qty;
						lower = 1.0;
						upper = 46.0;
						step = tmpf;
						digit = 0;
						pos = 4;
						unit = UNIT_NONE;
						draw3_digit_pressed (data_324, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->entry_qty;
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
						if(CFG(display)==10)
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

							cur_value = CFG(dis_range)/100.0;
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
						if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							cur_value = CFG(dis_range)/100.0;
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
					draw3_popdown(menu_content[OFF_ON + CFG(overlay_cursor)],4,0);
					break;
				case 2:/* p424 */
					if(CFG_ZOOM_POS(zoom_type) == 0 || CFG_ZOOM_POS(zoom_type) == 1)
					{
						/* 当前步进 */
						switch (TMP(start_amplitude_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = CFG_ZOOM_POS(start_amplitude);
							lower = 0.0;
							upper = 99.0;
							step = tmpf;
							digit = 0;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_424, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(start_amplitude);
							digit = 0;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else if(CFG_ZOOM_POS(zoom_type) == 2)
					{
						/* 当前步进 */
						switch (TMP(center_amplitude_reg))
						{
							case 0:	tmpf = 5.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = CFG_ZOOM_POS(center_amplitude);
							lower = 0.0;
							upper = 100.0;
							step = tmpf;
							digit = 2;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_4241, units[unit], cur_value , lower, upper, step, digit, p, pos, 9);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(center_amplitude);
							digit = 2;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
						}
					}
					break;

				case 3:/*Display -> color -> Mode  p434 */
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


				case 4:/*Display -> properties -> appearence  p444 */
					pp->x_pos = 536, pp->y_pos = 455-YOFFSET;
					switch(CFG(prop_scan))
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
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/* Probe/Part -> Select -> Wedge P504*/
					if (CFG(probe_select)==0)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							draw_dialog_all (DIALOG_WEDGE);
						else
							draw3_popdown(GROUP_VAL(wedge.Model), 4, 0);

						g_sprintf (temp,"%s", con2_p[5][0][4]);
						gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					}
					else if(CFG(probe_select)==1)
					{
						draw3_popdown(NULL,4,1);
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
					if(CFG(auto_program) == AUTO_FOCAL_ON)
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
							(CFG(auto_program) == AUTO_FOCAL_ON) 
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
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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
						if ((CFG(auto_program) == AUTO_FOCAL_ON)&&(LAW_VAL(Focal_point_type)==FOCALPLANE_P))
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
					g_sprintf (TMP (velocity_data[2]), "%d m/s", GROUP_VAL(velocity) / 100);
					gchar **tp = TMP(velocity_data_p);
					if (GROUP_VAL (velocity) == get_material_lw(pp->p_config)) 
					{
						temp_pos = 0;
						temp_qty = 2;
					}
					else if (GROUP_VAL (velocity) == get_material_sw(pp->p_config))
					{
						temp_pos = 1;
						temp_qty = 2;
					}
					else
					{
						temp_pos = 2;
						temp_qty = 3;
					}

					if (CFG(auto_program) == AUTO_FOCAL_ON)
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
						cur_value = CFG(origin)/1000.0;
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
						cur_value = CFG(origin)/1000.0;
						digit = 2;
						pos = 4;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
					break;

				case 1: /* Max.Scan Speed p714 */
					if(CFG(i_type)==2)
					{
						/* 当前步进 */
						switch (TMP(scanspeed_rpm_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}

						if(CFG(i_scan)==0)
							content_pos=0;
						else
							content_pos=6;

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = CFG(scanspeed_rpm)/10.0;
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
							cur_value = CFG(scanspeed_rpm)/10.0;
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
						case 0:	tmpf = 0.001 * CFG(index_resolution); break;
						case 1:	tmpf = 0.01 * CFG(index_resolution); break;
						case 2:	tmpf = 0.1 * CFG(index_resolution); break;
						default:break;
					}

					if(CFG(i_type)==1 || CFG(i_type)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							if( CFG(unit) == UNIT_MM )
							{
								cur_value = CFG(index_end)/1000.0;
								lower = CFG(index_start)/1000.0;
								upper = 100000.0;
								step = tmpf;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(index_end)/1000.0*0.03937;
								lower = CFG(index_start)/1000.0*0.03937;
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
							if( CFG(unit) == UNIT_MM )
							{
								cur_value = CFG(index_end)/1000.0;
								digit = 2;
								pos = 4;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(index_end)/1000.0*0.03937;
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
						if( CFG(unit) == UNIT_MM )
						{
							cur_value = CFG(index_end);
							digit = 2;
							pos = 4;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(index_end)/1000.0*0.03937;
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
					if(!CFG(file_storage))
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							draw3_pop_tt (data_804, NULL, 
									menu_content[SAVE_MODE+CFG(save_mode)],
									menu_content+SAVE_MODE, 4, 4, CFG(save_mode), 0);
						else 
							draw3_popdown (menu_content[SAVE_MODE+CFG(save_mode)], 4, 0);
					}

					else
					{
						draw3_popdown (menu_content[SAVE_MODE+CFG(save_mode)], 4, 0);
						gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);
					}
					gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);

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
								menu_content[VIEW+CFG(view)],
								menu_content+VIEW, 3, 4, CFG(view), 0);
					else 
						draw3_popdown (menu_content[VIEW+CFG(view)], 4, 0);

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
					pp->x_pos = 572, pp->y_pos = 484-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_904, NULL, 
								menu_content[GATE_M + CFG(gate_mode)],
								menu_content+GATE_M, 2, 4, CFG(gate_mode), 0);
					else 
						draw3_popdown (menu_content[GATE_M + CFG(gate_mode)], 4, 0);
					gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);

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
	gfloat tmpf = 0.0;
	gchar *str;
	guint menu_status  = 0;

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit, content_pos, temp_beam;

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
						if ((CFG(auto_program) == AUTO_FOCAL_ON) &&
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
							if ((pp->ctype_pos == 1)&&(pp->cmode_pos == 2))
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
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
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
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
							{
								draw3_popdown_offset(NULL, 5,1,31);
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
							//							upper = (gfloat)(TMP(max_prf));	/* 最大值需要计算出来 */
							upper = 100.0;
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
								str = g_strdup_printf ("%d", GROUP_VAL(prf) / 10);
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
						if (GROUP_VAL(prf))
						{
							str = g_strdup_printf ("%d", GROUP_VAL(prf) / 10);
							draw3_popdown (str, 5, 0);
							g_free(str);
						}
						else 
						{
							cur_value = GROUP_VAL(prf) / 10.0;
							unit = UNIT_NULL;
							pos = 5;
							digit = 0;
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
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
						cur_value = CFG(reject);
						lower = 1.0;
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
						cur_value = CFG(reject);
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
							upper = 74.0;
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
					if ( CFG_ALARM_POS(groupa) == CFG_ALARM_POS(groupb) )
					{
						/* 当 groupa 的值不为 None 时*/
						if (CFG(alarm[CFG(alarm_pos)].conditiona))
						{
							menu_status = 1<<(CFG_ALARM_POS(conditiona));
						}

						/* 当groupa与groupb的值相同,且condition a 与 condition b 的值相同时 */
						if ((CFG_ALARM_POS(conditiona)==CFG_ALARM_POS(conditionb)) &&
								CFG_ALARM_POS(conditiona))
						{
							menu_status = 1<<(CFG_ALARM_POS(conditiona));
							CFG_ALARM_POS(conditionb) = 0;  /* conditionb 变为 None */
							gtk_label_set_text (GTK_LABEL (pp->data3[5]), menu_content[CONDITIONA +
									CFG_ALARM_POS(conditionb)]);
						}
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						draw3_pop_tt (data_215, NULL,menu_content[CONDITIONA + CFG_ALARM_POS(conditionb)],
								menu_content + CONDITIONA, 9, 5, CFG_ALARM_POS(conditionb), menu_status);
					else 
						draw3_popdown (menu_content[CONDITIONA + CFG_ALARM_POS(conditionb)], 5, 0);

					if (CFG_ALARM_POS(conditiona) == 0)/*conditiona为None时，conditionb不可用*/
					{
						gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					}
					break;

				case 2:/* Hold Time P225 */
					if (CFG(output_pos) < 3)
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
							cur_value = CFG_OUTPUT_POS(holdtime)/1000.0;
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
							cur_value = CFG_OUTPUT_POS(holdtime)/1000.0;
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
					pp->x_pos = 314, pp->y_pos = 534-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						draw3_pop_tt (data_305, NULL, 
								field[CFG(field4)],
								field1, 4, 5, CFG(field4), 0);
					else 
						draw3_popdown (field[CFG(field4)], 5, 0);

					break;

				case 1:/* p315 */
					if( GROUP_VAL(selection)==0||GROUP_VAL(selection)==8||GROUP_VAL(selection)==4)
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
							if(CFG(unit)==UNIT_MM)
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
							if(CFG(unit)==UNIT_MM)
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
# if 0
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))/*选中p405这个位置*/
					{
						if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							/* 当前步进 */
							switch (TMP(avg_scan_speed_reg))
							{
								case 0:	tmpf = 0.1; break;
								case 1:	tmpf = 1.0; break;
								case 2:	tmpf = 10.0; break;
								case 3:	tmpf = 100.0; break;
								default:break;
							}

							if(UNIT_MM == CFG(unit))
							{
								cur_value = GROUP_VAL(prf)/10.0;//CFG(avg_scan_speed)/100.0;
								lower = 0.01;
								upper = 100.0;	/* 与prf一样 */
								step = tmpf;
								digit = 2;
								pos = 5;
								unit = UNIT_MM_S;
							}
							else
							{
								cur_value =  GROUP_VAL(prf)/10.0*0.03937;
								lower = 0.01;
								upper = 100.0*0.03937;	/* 与prf一样 */
								step = tmpf;
								digit = 2;
								pos = 5;
								unit = UNIT_INCH_S;

							}
							draw3_digit_pressed (data_405, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
							gtk_widget_set_sensitive (pp->eventbox30[5],FALSE);
							gtk_widget_set_sensitive (pp->eventbox31[5],FALSE);

						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[5]);
							gtk_widget_hide (pp->eventbox31[5]);
						}
					}
					else 
					{
						if(CFG(display)==10)
							/*Display 为 Strip Chart-[A]*/
						{
							cur_value = CFG(avg_scan_speed)/100.0;
							digit = 2;
							pos = 5;
							unit = UNIT_MM_S;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
						}
						else 
						{
							gtk_widget_hide (pp->eventbox30[5]);
							gtk_widget_hide (pp->eventbox31[5]);
						}
					}
#endif
					if(CFG(display)==10)
						/*Display 为 Strip Chart-[A]*/
					{
						if(UNIT_MM == CFG(unit))
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
					draw3_popdown(menu_content[OFF_ON + CFG(overlay_overlay)],5,0);
					break;
				case 2:/* p425 */
					if( CFG_ZOOM_POS(zoom_type) == 0 )
					{
						/* 当前步进 */
						switch (TMP(end_amplitude_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							cur_value = CFG_ZOOM_POS(end_amplitude);
							lower = 0.0;
							upper = 100.0;
							step = tmpf;
							digit = 0;
							pos = 5;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_425, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(end_amplitude);
							digit = 0;
							pos = 5;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					if( CFG_ZOOM_POS(zoom_type) == 1 || CFG_ZOOM_POS(zoom_type) == 2 )
					{
						/* 当前步进 */
						switch (TMP(range_amplitude_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							cur_value = CFG_ZOOM_POS(range_amplitude);
							lower = 0.0;
							upper = 100.0;
							step = tmpf;
							digit = 0;
							pos = 5;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_4251, units[unit], cur_value , lower, upper, step, digit, p, pos, 7);
						}
						else 
						{
							cur_value = CFG_ZOOM_POS(range_amplitude);
							digit = 0;
							pos = 5;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
						}
					}
					break;

				case 3:
					if ( !con2_p[4][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:/* Display -> properties -> overlay  p445 */
					pp->x_pos = 575, pp->y_pos = 541-YOFFSET;
					switch(CFG(prop_scan))
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
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Auto Detect  p505 */
					draw3_popdown (menu_content[OFF_ON + CFG(auto_detect)], 5, 0);
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
							(CFG(auto_program) == AUTO_FOCAL_ON) 
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
							temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[CFG(groupId)]);
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
					gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					break;
				case 1: /* Max.Index  Speed  p715 */
					if(CFG(i_type)==2)
					{
						/* 当前步进 */
						switch (TMP(indexspeed_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							case 2:	tmpf = 100.0; break;
							default:break;
						}

						if(CFG(i_scan)==0)
							content_pos=0;
						else
							content_pos=7;

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							cur_value = CFG(indexspeed)/10.0;
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
							cur_value = CFG(indexspeed)/10.0;
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

					if(CFG(i_type)==1 || CFG(i_type)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							if( CFG(unit) == UNIT_MM )
							{
								cur_value = CFG(index_resolution)/1000.0;
								lower = 0.01;
								upper = 99999.0;
								step = tmpf;
								digit = 2;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(index_resolution)/1000.0*0.03937;
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
							if( CFG(unit) == UNIT_MM )
							{
								cur_value = CFG(index_resolution)/1000.0;
								digit = 2;
								pos = 5;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = CFG(index_resolution)/1000.0 * 0.03937;
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
						if( CFG(unit) == UNIT_MM )
						{
							cur_value = CFG(index_resolution)/1000.0;
							digit = 2;
							pos = 5;
							unit = UNIT_MM;
						}
						else
						{
							cur_value = CFG(index_resolution)/1000.0 * 0.03937;
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
				case 0:/*File -> File -> file name  p805 */
					if(!CFG(file_storage))
					{
						/* 格式化字符串 */
						g_sprintf (temp,"%s", con2_p[8][0][5]);

						/* 设置label */
						gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
						gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Data####");

						/* 显示和隐藏控件 */
						gtk_widget_show (pp->eventbox30[5]);
						gtk_widget_show (pp->eventbox31[5]);
						gtk_widget_show (pp->data3[5]);

					}
					else
					{						
						gtk_widget_set_sensitive (pp->eventbox30[5], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[5], FALSE);
					}
					gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					break;

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
			for (t1 = i * qty2 / qty1, t2 = (i + 1) * qty2 /qty1, j = t1; j < t2; j++)
			{
				target_data[j] = (guchar)((gint)(source_data[i]) + 
						(j - t1) * ((gint)(source_data[i + 1]) - (gint)(source_data[i])) / (qty2 / qty1));
			}
		}
	}
}

static void interpolation_data1 (DOT_TYPE *source_data, DOT_TYPE *target_data,
		gint qty1, gint qty2)
{
	gint i;
	for (i = 0 ; i < qty1 ; i++)
	{
		target_data[i] = 0x80;
	}
}

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

	g_print ("draw keyboard\n");
	//	gtk_main_quit();
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
			fakekey_press_keysym(pp->fk, XK_BackSpace, 0);
			fakekey_release(pp->fk);
			break;
		case 0xef:
			fakekey_press_keysym(pp->fk, XK_BackSpace, 0);
			fakekey_release(pp->fk);
			break;
			/*
			   case 0xd3:
			   fakekey_press_keysym(pp->fk, XK_80%, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xda:
			   fakekey_press_keysym(pp->fk, XK_DISP, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xd4:
			   fakekey_press_keysym(pp->fk, XK_Repeat, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xdb:
			   fakekey_press_keysym(pp->fk, XK_Save, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xd5:
			   fakekey_press_keysym(pp->fk, XK_Open, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xdc:
			   fakekey_press_keysym(pp->fk, XK_GATE, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xde:
			   fakekey_press_keysym(pp->fk, XK_DB, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xe0:
			   fakekey_press_keysym(pp->fk, XK_Freeze, 0);
			   fakekey_release(pp->fk);
			   break;
			   case 0xe9:
			   fakekey_press_keysym(pp->fk, XK_Help, 0);
			   fakekey_release(pp->fk);
			   break;
			   */
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

/* 读取波形数据 并画出来 */
static gboolean time_handler2 (GtkWidget *widget)
{
	gchar key = 0;
	gint i, j, k, prf_count, offset;
	guint *temp = (guint *)(TMP(kernel_config_add));
	DRAW_UI_P p = p_drawui_c;

	if (temp[0])
	{
		temp[0] = 0;
	}

	for (i = 0 ; i < CFG(groupQty); i++)
	{
		p->refresh_mark = 1;
		/* 获取数据 */
		/* 这里需要压缩数据 或者 插值数据 这里只有一个beam 同时最多处理256beam */
		for	(j = 0 ; j < TMP(beam_qty[i]); j++)
		{  
			for (offset = 0, k = 0 ; k < i; k++)
				offset += GROUP_VAL_POS(k, point_qty) * TMP(beam_qty[k]);
			if (GROUP_VAL_POS(i, point_qty) <= TMP(a_scan_dot_qty))
			{
				interpolation_data (
						(DOT_TYPE *)(pp->p_beam_data + offset +
							GROUP_VAL_POS(i, point_qty) * j),
						TMP(scan_data[i] + TMP(a_scan_dot_qty) * j), 
						GROUP_VAL_POS(i, point_qty),
						TMP(a_scan_dot_qty));
			}
			else if (GROUP_VAL_POS(i, point_qty) > TMP(a_scan_dot_qty))
				compress_data (
						(DOT_TYPE *)(pp->p_beam_data + offset +
							GROUP_VAL_POS(i, point_qty) * j),
						TMP(scan_data[i] + TMP(a_scan_dot_qty) * j), 
						GROUP_VAL_POS(i, point_qty),
						TMP(a_scan_dot_qty), 
						GROUP_VAL_POS(i, rectifier));
		}
		for (k = 0; ((k < 16) && (TMP(scan_type[k]) != 0xff)); k++)
		{
			if (TMP(scan_group[k]) == i)
				draw_scan(k, TMP(scan_type[k]), TMP(scan_group[k]), 
						TMP(scan_xpos[k]), TMP(scan_ypos[k]), dot_temp, NULL);
		}
	}

	/* 复制波形到显存 */
	if (p->refresh_mark )
	{
		memcpy (TMP(fb1_addr), dot_temp1, FB_WIDTH*400*2);	/* 如果用dma更快啊 */
		pp->refresh_mark = 0;
	}

	return TRUE;
}

/* 用来用户按键信息 */
gpointer signal_thread(gpointer arg) 
{
	gchar key;
	if (read(pp->fd_key, &key, 1) > 0 ) 
	{	
		process_key_press (key);
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

/*初始化界面*/
void init_ui(DRAW_UI_P p)				
{
	gint	i;
	gchar	*markup;

	p_drawui_c = p;
	/* 初始化语言 language init */
	change_language (CFG(language), p);

	pp->pos_pos		= MENU3_STOP;
	pp->menu2_qty	= 5;
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
	pp->entry 		= gtk_entry_new();

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
		update_widget_bg(p->eventbox2[i], backpic[1]);
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

		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox30[i]), 114, 60);
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox31[i]), 114, 25);       
		gtk_widget_set_size_request(GTK_WIDGET(p->sbutton[i]), 114, 25); 
		update_widget_bg(pp->eventbox30[i], backpic[7]);
		update_widget_bg(pp->eventbox31[i], backpic[10]);
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

	for ( i = 0; i < 30; i++)
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
	update_widget_bg(pp->event[17], backpic[12]); 
	g_signal_connect (G_OBJECT (pp->event[17]), "button-press-event", 
			G_CALLBACK (draw_keyboard), NULL);		/* 虚拟键盘 */
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[18], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[18]), 70, 25); 
	gtk_widget_modify_bg(pp->event[18], GTK_STATE_NORMAL, &color_black);

	/* 增益显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[0], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[0]), 60, 45);
	update_widget_bg(pp->event[0], backpic[3]);
	if (GROUP_VAL(db_ref))
		tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
	else
		tt_label_show_string (pp->label[0], con2_p[1][0][0], "\n", "(dB)", "white", 10);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[1], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[1]), 112, 45);  
	update_widget_bg(pp->event[1], backpic[4]);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>", 
			GROUP_VAL(gain) / 100.0);
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
	update_widget_bg(pp->event[2], backpic[5]);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[3], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[3]), 172, 22);
	gtk_label_set_justify (GTK_LABEL (pp->label[3]), PANGO_ELLIPSIZE_START);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",
			GROUP_VAL(prf), GROUP_VAL(prf));
	gtk_label_set_markup (GTK_LABEL (pp->label[3]), markup); 
	g_free (markup);
	update_widget_bg(pp->event[3], backpic[5]);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[4], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[4]), 172, 22);
	time_handler1(NULL);
	update_widget_bg(pp->event[4], backpic[5]);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[5], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[5]), 172, 22);
	update_widget_bg(pp->event[5], backpic[5]);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[6], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[6]), 172, 22);
	markup = g_markup_printf_escaped ("<span foreground='red' font_desc='10'>%s</span>", VERSION);
	gtk_label_set_markup (GTK_LABEL (pp->label[6]), markup); 
	g_free (markup);
	g_signal_connect(G_OBJECT(pp->event[6]), "button-press-event", 
			G_CALLBACK(save_config), NULL);

	update_widget_bg(pp->event[6], backpic[5]);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",
			(gfloat)(GROUP_VAL(prf)));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;
	g_free (markup);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[7], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[7]), 172, 22);
	update_widget_bg(pp->event[7], backpic[5]);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f s</span>",
			(gfloat)(GROUP_VAL(prf)));
	gtk_label_set_markup (GTK_LABEL (pp->label[7]), markup); 
	g_free (markup);

	/* 4个测量值显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[8], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>%%A\n(%%)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[8]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[8]), 60, 45);
	update_widget_bg(pp->event[8], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[9], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[9]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[9]), 112, 45);
	update_widget_bg(pp->event[9], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[10], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>DA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[10]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[10]), 60, 34);
	update_widget_bg(pp->event[10], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[11], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[11]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[11]), 112, 34);
	update_widget_bg(pp->event[11], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[12], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>PA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[12]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[12]), 60, 34);
	update_widget_bg(pp->event[12], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[13], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[13]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[13]), 112, 34);
	update_widget_bg(pp->event[13], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[14], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>SA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[14]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[14]), 60, 34);
	update_widget_bg(pp->event[14], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[15], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[15]),markup);
	g_free (markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[15]), 112, 34);
	update_widget_bg(pp->event[15], backpic[4]);

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

#if ARM
	g_thread_create (signal_thread, NULL, FALSE, NULL);
	g_timeout_add (50, (GSourceFunc) time_handler2, NULL);
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
