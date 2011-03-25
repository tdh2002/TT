
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
#include <gdk/gdkkeysyms.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <webkit/webkit.h>

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS((GtkObject*)(i))->x
#define YOFFSET  26


enum
{
	LIST_ITEM = 0,
	N_COLUMNS
};

static char buffer[32];
static guchar dot_temp[800];
static gushort dot_temp1[FB_WIDTH*400];

gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);/* window 原始的按键处理 */
gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);			/* 自己的按键处理*/
GtkWidgetClass *widget_window_class;									/* 原始widnow class */
GdkPoint a[512] = {{0,0}, {240,200}, {400,200}};

const gchar *backpic[] = 
{
	"pic/10.png", "pic/11.png", "pic/12.png",		/* 二级菜单 0按下 1未选中 2 停留 */
	"pic/00.png", "pic/01.png", "pic/02.png",	        /* 00上方数值 标识栏 01 数值背景 02 比较小的背景 */
	"pic/20.png", "pic/21.png", "pic/22.png",		/* 三级菜单名称 0按下 1未选中 2 停留 */
	"pic/30.png", "pic/31.png", "pic/32.png",		/* 三级菜单数值 0按下 1未选中 2 停留 */
	"pic/tt.png", "pic/311.png", "pic/322.png"      	/* 软键盘图标 */
};

void change_language();
void draw_1_menu(DRAW_UI_P p);
void draw_2_menu(gint pa);
void draw_3_menu(gint pa, gpointer p);
void init_ui(DRAW_UI_P p);				/* 初始化界面 */
void draw_area_all();

/**/
const gchar **con0_p	 = content_en10;
const gchar ***con1_p	 = content1_en;
const gchar ****con2_p	 = content2_en;

const gchar **units	 = units_en;
const gchar **menu_content = all_menu_content_en;

const gchar **type       = type_en;
const gchar **calibration_mode       = calibration_mode_en;

const gchar **list       =list_en;
const gchar **list1      =list1_en;
const gchar **field1     =field1_en;
const gchar **field      =field_en;

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

	style = gtk_style_copy( widget->style);  

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
	if (MENU_POP == action)/*弹出主菜单*/
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
	else if (MENU_DIS == action) /*收回主菜单*/
	{
		gtk_menu_popdown( GTK_MENU (pp->menu));
	}
	 else if ( (MENU_CHANGE == action)||(MENU_DOWN == action) )	/*轮流切换主菜单*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		if((pp->pos) == 9)
			pp->pos = 0;
		else
			pp->pos++;
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
	}
	 else if (MENU_UP == action)/*向上切换主菜单*/
	{
		gtk_menu_item_deselect (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		if((pp->pos) == 0)
			pp->pos = 9;
		else
			pp->pos--;
		gtk_menu_item_select (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
	}
	else if (MENU_ENTER == action) /*选中当前菜单项，并收回主菜单*/
	{
		gtk_menu_item_activate (GTK_MENU_ITEM (pp->menuitem[pp->pos]));
		gtk_menu_popdown( GTK_MENU (pp->menu));
	}

//	pp->main_menu_pop_status = !pp->main_menu_pop_status ;
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
			gtk_menu_item_get_label(menuitem));	/*  */
	pp->pos_last = pp->pos; /*  */
	p->pos = 0;
	pp->menu2_qty = 3;
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
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
	MENU_STATUS = MENU3_STOP; /* */
	draw_2_menu(1);
	draw_3_menu(1, NULL);
}

/* 切换语言 保留 */
void change_language()
{
	return;
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

	if (pp->pos_pos != MENU3_PRESSED)
		pp->mark_pop_change = 0;
	for ( i = 0 ; i < 5 ; i++ )   /* 5个二级菜单 */
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
				update_widget_bg(pp->eventbox2[i], backpic[1]);
				gtk_widget_modify_fg(pp->label2[i], GTK_STATE_NORMAL, &color_white);
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
		update_widget_bg(pp->eventbox2[pp->pos1[pp->pos]], backpic[2]);
	else 
		update_widget_bg(pp->eventbox2[pp->pos1[pp->pos]], backpic[0]);

	gtk_widget_modify_fg(pp->label2[pp->pos1[pp->pos]], GTK_STATE_NORMAL, &color_yellow);

	if(pp->pos == 4)
		gtk_widget_set_sensitive(pp->eventbox2[2],FALSE);
	else
		gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);


	return ;
}

static gint gtk_entry_digit_only_keypress_event(GtkWidget *widget, GdkEventKey *event)
{
	gpointer data = NULL;
	if (event->keyval == GDK_F12)
		return key_press_handler (widget, event, data);
	else
		return window_keypress_event_orig(widget, event); 
	return FALSE;
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
		}
		else
		{
			if (pp->tag == 0)/*探头大类选择Unknow时*/
			{
				strcpy(GROUP_VAL(probe.Model), " Unknown");
				gtk_label_set_text (GTK_LABEL (pp->data3[3]), GROUP_VAL(probe.Model));

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
			if (pp->tag == 0)/*探头大类选择Unknow时*/
			{
				strcpy(GROUP_VAL(probe.Model), " Unknown");
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

/*
 * 警告信息 
 * btn_qty: 表示几个按钮 1-2; 1就是 确认 2是确认和取消
 * warn_info: 警告或者提示信息
 * 1 
 *
 *
 */
/**
static void draw_warning(guint btn_qty, gchar *warn_info)
{

}
**/

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
	cairo_show_text(cr,"Palette");   	/*最顶端标签 Palette*/

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
	GtkWidgetClass *widget_window_class1;
	const gchar *buf = (const gchar *)(CFG(remark_info));


	dialog = gtk_dialog_new_with_buttons("Dialog_Remark", win,
			GTK_DIALOG_MODAL |	GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);			/*不可以装饰*/
	widget_window_class1 = GTK_WIDGET_GET_CLASS (((GtkObject*)(dialog))); 
	widget_window_class1->key_press_event = gtk_entry_digit_only_keypress_event; /* 指定哪些字符输入 */

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

       GtkWidget *vbox_1_1_1[7];
	GtkWidget *label_1_1_1[7];
       char *char_1_1_1[7] = {"","","Close","","","",""};

	GtkWidget *hbox_2;

	GtkWidget *vbox_2_1;

	GtkWidget *vbox_2_1_1;
	GtkWidget *vbox_2_1_2;

	GtkWidget *hbox_2_1_1_1;

	GtkWidget *hbox_2_1_2_1;

       GtkWidget *hbox_2_1_2_1_1[6];
       GtkWidget *label_2_1_2_1_1[6];
       char *char_2_1_2_1_1[6] = {"Select","Select All","Copy","Move","Delect","Rename"};

	GtkWidget *sw_2_1_1_1_1;
	GtkWidget *sw_2_1_1_1_2;

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

	hbox_first = GTK_WIDGET (GTK_DIALOG(dialog)->vbox);

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
	sw_2_1_1_1_1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_2_1_1_1_1, 343, 515);

	//目标文件窗口，带滚动条
	sw_2_1_1_1_2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request(sw_2_1_1_1_2, 343, 515);

	hbox = gtk_hbox_new(FALSE,0);

	hbox_1 = gtk_hbox_new(FALSE,0);

	vbox_1_1 = gtk_vbox_new(FALSE,0); 




	hbox_2 = gtk_hbox_new(FALSE,0);

	vbox_2_1 = gtk_vbox_new(FALSE,0); 

	vbox_2_1_1 = gtk_vbox_new(FALSE,0);

	vbox_2_1_2 = gtk_vbox_new(FALSE,0);

	hbox_2_1_1_1 = gtk_hbox_new(FALSE,0);

	hbox_2_1_2_1 = gtk_hbox_new(FALSE,0);

        for(i=0;i<6;i++)
         {
            hbox_2_1_2_1_1[i] = gtk_event_box_new();
	     gtk_widget_set_size_request(GTK_WIDGET(hbox_2_1_2_1_1[i]),114,85);
            update_widget_bg(hbox_2_1_2_1_1[i], backpic[1]);
	     label_2_1_2_1_1[i] = gtk_label_new(char_2_1_2_1_1[i]);
	     gtk_widget_modify_fg (label_2_1_2_1_1[i], GTK_STATE_NORMAL, &color_black);
	     gtk_label_set_justify(GTK_LABEL(label_2_1_2_1_1[i]), GTK_JUSTIFY_CENTER);
	     gtk_box_pack_start(GTK_BOX(hbox_2_1_2_1),hbox_2_1_2_1_1[i],FALSE,FALSE,0);
            gtk_container_add(GTK_CONTAINER(hbox_2_1_2_1_1[i]), label_2_1_2_1_1[i]);
         }   
	
        for(i=0;i<7;i++)
        {
            vbox_1_1_1[i] = gtk_event_box_new();
            gtk_widget_set_size_request(GTK_WIDGET(vbox_1_1_1[i]),114,85);
            update_widget_bg(vbox_1_1_1[i], backpic[1]);

	     label_1_1_1[i] = gtk_label_new(char_1_1_1[i]);
	     gtk_widget_modify_fg (label_1_1_1[i], GTK_STATE_NORMAL, &color_black);
	     gtk_label_set_justify(GTK_LABEL(label_1_1_1[i]), GTK_JUSTIFY_CENTER);

	     gtk_container_add(GTK_CONTAINER(vbox_1_1_1[i]),label_1_1_1[i]);
	     gtk_box_pack_start(GTK_BOX(vbox_1_1),vbox_1_1_1[i],FALSE,FALSE,0);   
        }  
  

	gtk_box_pack_start(GTK_BOX(vbox_2_1_2),hbox_2_1_2_1,FALSE,FALSE,0);   

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
	
	gtk_container_add(GTK_CONTAINER(sw_2_1_1_1_1),source_list);

	gtk_container_add(GTK_CONTAINER(sw_2_1_1_1_2),target_list);    

	gtk_box_pack_start(GTK_BOX(hbox_2_1_1_1),sw_2_1_1_1_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox_2_1_1_1),sw_2_1_1_1_2, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_2_1_1),hbox_2_1_1_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_2_1), vbox_2_1_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_2_1), vbox_2_1_2, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox_2), vbox_2_1, FALSE, FALSE, 0);   

	gtk_box_pack_start(GTK_BOX(hbox_1),vbox_1_1,FALSE,FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox),hbox_2, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox_first),hbox, FALSE, FALSE, 0);

	my_signal.source_model = source_model;
	my_signal.source_selection = source_selection;
	my_signal.source_list = source_list;
	my_signal.target_model = target_model;
	my_signal.target_selection = target_selection;
	my_signal.target_list = target_list;

	//关闭窗口的信号
	g_signal_connect(G_OBJECT (vbox_1_1_1[2]), "button-press-event",G_CALLBACK(dialog_destroy), dialog);

	g_signal_connect(G_OBJECT(hbox_2_1_2_1_1[1]), "button-press-event",G_CALLBACK(Select_All_File), (gpointer)source_list);

	g_signal_connect(G_OBJECT(hbox_2_1_2_1_1[2]), "button-press-event",G_CALLBACK(Copy_File), (gpointer)&my_signal);

	g_signal_connect(G_OBJECT(hbox_2_1_2_1_1[3]), "button-press-event",G_CALLBACK(Move_File), (gpointer)&my_signal);

	g_signal_connect(G_OBJECT(hbox_2_1_2_1_1[4]), "button-press-event",G_CALLBACK(Delect_File), (gpointer)&my_signal);

	g_signal_connect(G_OBJECT(hbox_2_1_2_1_1[5]), "button-press-event",G_CALLBACK(Rename_File), (gpointer)&my_signal);

	gtk_widget_show_all(dialog);

	return ;  
}

/*
 * 弹出的dialog
 * 0 记事本 备注等等
 * 1 探头 
 * 2 楔块 
 * 3 自定义探头
 * 4 自定义楔块
 * 5 聚焦法则读入
 * 5 聚焦法则保存
 * 6 配置 数据 报告 图像 的读入
 * 7 保存 配置文件
 * 8 系统信息的显示 
 * 9 报告的显示
 * 10 Export Table
 * 11 调色板
 *
 */
static void draw_dialog_all (guint type)
{
	switch (type)
	{
		case DIALOG_REMARK: draw_remark(); break;
		case DIALOG_PROBE:  draw_probe(); break;
		case DIALOG_WEDGE:  draw_wedge(); break;

		case DIALOG_COLOR_PALETTE:  draw_color_palette(); break;
		case DIALOG_FILE_MANAGE:	draw_file_manage(); break;
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
	widget_window_class->key_press_event = window_keypress_event_orig;
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

	if (str)
		g_free(str);
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
	widget_window_class->key_press_event = my_keypress_event;
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

	/*						gtk_widget_grab_focus (pp->button);*/
}

/* 画栅格线 */
static gboolean draw_info(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gfloat color_r = 0, color_g = 0, color_b = 0;
	gint i,j,m,n,w,h;
	cairo_t *cr; 

	gtk_widget_get_size_request (widget, &w, &h);

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

	/* 调色板信息 */
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 1.0);
	cairo_rectangle (cr, w, 0, 10, h);
	cairo_fill (cr);
	for ( i = 1; i < h - 1; i++ )
	{
		switch (((DRAW_AREA_P)(data))->scan_type)
		{
			case	A_SCAN:
			case	B_SCAN:
				color_r = ((TMP(color_amp[i * 256 / h]) >> 11)) / 32.0;
				color_g = ((TMP(color_amp[i * 256 / h]) & 0x07e0) >> 5) / 64.0;
				color_b = ((TMP(color_amp[i * 256 / h]) & 0x1f)) /  32.0;
				break;
			case	C_SCAN:
			case	S_SCAN:
				break;
			default:break;
		}
		cairo_set_source_rgba (cr, color_r, color_g, color_b, 1.0);
		cairo_move_to (cr, w + 1, i);
		cairo_line_to (cr, w + 9, i);
		cairo_stroke (cr);
	}

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
	//	gtk_widget_show(draw_area->label);
	//	gtk_widget_show(draw_area->ebox);

	/* 3个刻度尺 1个画图区 */
	draw_area->table = gtk_table_new (2, 4, FALSE);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->table, FALSE, FALSE, 0);
	draw_area->drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(draw_area->drawing_area), width - 40, height - 35);
	/* 调用 draw_info 函数 */
	g_signal_connect (G_OBJECT (draw_area->drawing_area), "expose_event",
			G_CALLBACK (draw_info), (gpointer)(draw_area));

	gtk_widget_modify_bg (draw_area->drawing_area, GTK_STATE_NORMAL, &color_black1);
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->drawing_area, 1, 2, 0, 1,
			GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
	gtk_widget_set_events (draw_area->drawing_area, GDK_POINTER_MOTION_MASK |
			GDK_POINTER_MOTION_HINT_MASK);

	draw_area->vruler1 = gtk_vruler_new ();
	gtk_ruler_set_metric (GTK_RULER (draw_area->vruler1), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (draw_area->vruler1), v1s, v1e, 0, 1);
	gtk_widget_modify_bg (draw_area->vruler1, GTK_STATE_NORMAL, &color_rule);
	gtk_widget_set_size_request (GTK_WIDGET (draw_area->vruler1), 15, height - 35);
	g_signal_connect_swapped ((draw_area->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD ((draw_area->vruler1), motion_notify_event)),
			draw_area->vruler1);
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->vruler1, 0, 1, 0, 1,
			GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

	draw_area->vruler2 = gtk_vruler_new ();
	gtk_ruler_set_metric (GTK_RULER (draw_area->vruler2), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (draw_area->vruler2), v2e, v2s, 0, 1);
	gtk_widget_modify_bg(draw_area->vruler2, GTK_STATE_NORMAL, &color_rule);
	gtk_widget_set_size_request (GTK_WIDGET (draw_area->vruler2), 15, height - 35);
	g_signal_connect_swapped (G_OBJECT (draw_area->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD (draw_area->vruler2, motion_notify_event)),
			draw_area->vruler2);			
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->vruler2, 3, 4, 0, 1,
			GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

	if (h1s != h1e) 
	{
		draw_area->hruler = gtk_hruler_new ();
		gtk_ruler_set_metric (GTK_RULER (draw_area->hruler), GTK_PIXELS);
		gtk_ruler_set_range (GTK_RULER (draw_area->hruler), h1s, h1e, 0, 60);
		gtk_widget_modify_bg(draw_area->hruler, GTK_STATE_NORMAL, &color_rule);
		gtk_widget_set_size_request (GTK_WIDGET (draw_area->hruler), width - 40, 20);
		g_signal_connect_swapped (G_OBJECT (draw_area->drawing_area), "motion_notify_event",
				G_CALLBACK (EVENT_METHOD (draw_area->hruler, motion_notify_event)),
				draw_area->hruler);
		gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->hruler, 1, 2, 1, 2,
				GTK_EXPAND|GTK_SHRINK|GTK_FILL, GTK_FILL, 0, 0);
	}
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
	switch (scan_type)
	{
		case A_SCAN:
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
			TMP(s_scan_width)	=	w;
			TMP(s_scan_height)	=	h;
			break;
		default:break;
	}
	TMP(a_scan_width)	=	aw;
	return ;
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
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "S-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
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
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 275, "S-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
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

	guint menu_on=0, i;

	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> Back*/
					g_sprintf (temp,"%s", con2_p[0][0][0]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Group Wizard");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
					break;
				case 1:/*Wizard -> Focal Law -> Back*/
					g_sprintf (temp,"%s", con2_p[0][1][0]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Law Wizard");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
					break;
				case 2:/*Wizard -> Calibration -> Back*/
					g_sprintf (temp,"%s", con2_p[0][2][0]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Select Calibration");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
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
								menu_content[GROUP_P + CFG(group_pos)],
								menu_content + GROUP_P, 10, 0, CFG(group_pos), menu_status);
					}
					else 
						draw3_popdown (menu_content[GROUP_P + CFG(group_pos)], 0, 0);
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
				case 2:/*Probe/Part -> characterize -> FFT p520 */
					draw3_popdown (menu_content[OFF_ON + CFG(fft)], 0, 0);
					break;

				case 3:/*Probe/Part -> Parts -> Geometry  p530 */
					pp->x_pos = 608, pp->y_pos = 118-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_530, NULL, 
								menu_content[GEOMETRY + CFG(part.Geometry_pos)],
								menu_content+GEOMETRY, 3, 0, CFG(part.Geometry_pos), 0);
					else 
						draw3_popdown (menu_content[GEOMETRY + CFG(part.Geometry_pos)], 0, 0);
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
				case 0:/* 聚焦法则类型 线性扫查 角度(扇形)扫查 P600 TAN1 */
					pp->x_pos = 394, pp->y_pos = 116 - YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_600, NULL, 
								menu_content[L_CONFIG + LAW_VAL(Focal_type)],
								menu_content+LAW_CONFIG, 4, 0, LAW_VAL(Focal_type), 0x0c);
					else 
						draw3_popdown (menu_content[L_CONFIG + LAW_VAL(Focal_type)], 0, 0);
					break;
				case 1:/* 聚焦 阵元数量 P610 */
					/* 当前步进 */
					switch (pp->p_tmp_config->element_qty_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if(CFG(auto_program))/*auto program 为 on 时，Element Qty 才可用*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							cur_value = GROUP_VAL(element_qty);
							lower = 1.0;
							upper = 32.0;
							step = tmpf;
							digit = 0;
							pos = 0;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_610, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(element_qty);
							digit = 0;
							pos = 0;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else/*auto program 为 off 时，Element Qty 为 unsensitive*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							cur_value = GROUP_VAL(element_qty);
							lower = 1.0;
							upper = 32.0;
							step = tmpf;
							digit = 0;
							pos = 0;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_610, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(element_qty);
							digit = 0;
							pos = 0;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					}
					break;

				case 2:/*Focal Law -> beam -> Min.Angle p620 */
					/* 当前步进 */
					switch (TMP(min_angle_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if(CFG(auto_program))/*auto program 为 on 时，Element Qty 才可用*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							cur_value = GROUP_VAL(min_angle)/100.0;
							lower = -89.9;
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 0;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_620, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(min_angle)/100.0;
							digit = 1;
							pos = 0;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else/*auto program 不为 on 时，Element Qty 不可用*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							cur_value = GROUP_VAL(min_angle)/100.0;
							lower = -89.9;
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 0;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_620, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(min_angle)/100.0;
							digit = 1;
							pos = 0;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}

						gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					}
					break;

				case 3:/*Focal Law -> laws -> auto program  p630 */
					draw3_popdown (menu_content[OFF_ON + CFG(auto_program)], 0, 0);
					break;

				case 4:
					if ( !con2_p[6][4][0] )
						gtk_widget_hide (pp->eventbox30[0]);
					gtk_widget_hide (pp->eventbox31[0]);
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
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if(CFG(i_type)==0 || CFG(i_type)==1)
						/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						{
							cur_value = CFG(scan_start);
							lower = -99999.0;
							upper = 99999.0;
							step = tmpf;
							digit = 0;
							pos = 0;
							unit = UNIT_MM;
							draw3_digit_pressed (data_720, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG(scan_start);
							digit = 0;
							pos = 0;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else  /* Inspection -> Type 选择 Helicoidal Scan  时 */
					{
						CFG(scan_start) = 0;
						digit = 0;
						pos = 0;
						unit = UNIT_MM;
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

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "02:55:06 PM");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[0]);
					gtk_widget_show (pp->eventbox31[0]);
					gtk_widget_show (pp->data3[0]);
					break;

				case 2:/*Preferences -> service -> system info  p920*/
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

					break;

				case 4:/*Preferences -> network -> IP Address  p940*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "192.168.1.2");

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
	gfloat tmpf = 0.0;
	gchar *str;

	gfloat cur_value=0.0, lower, upper, step;
	guint digit, pos, unit, content_pos, menu_status = 0;

	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:/*Wizard -> Group -> start*/
					g_sprintf (temp,"%s", con2_p[0][0][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Group Wizard");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				case 1:/*Wizard -> Focal Law -> start*/
					g_sprintf (temp,"%s", con2_p[0][1][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Law Wizard");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				case 2:/*Wizard -> Calibration -> start*/
					g_sprintf (temp,"%s", con2_p[0][2][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Select Calibration");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
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
								cur_value = GROUP_VAL(mat_atten) / 1000.0;
								lower = 0.00;
								upper = 6.76;
								step = tmpf;
								digit = 2;
								pos = 1;
								unit = UNIT_DB_MM;
								draw3_digit_pressed (data_2312, units[unit], cur_value , lower, 
										upper, step, digit, p, pos, 15);
							}
							else 
							{
								cur_value = GROUP_VAL(mat_atten) / 1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_DB_MM;
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
							cur_value = GROUP_VAL(s_reference)/100.0;
							lower = -1000000.0;
							upper =  1000000.0;
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3111, units[unit], cur_value , lower, upper, step, digit, p, pos, 6);
						}
						else 
						{
							cur_value = GROUP_VAL(s_reference)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_MM;
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
							unit = UNIT_NONE;
							draw3_digit_pressed (data_3112, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
						}
						else 
						{
							cur_value = GROUP_VAL(cursors_angle)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_NONE;
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
							unit = UNIT_NONE;
							draw3_digit_pressed (data_3113, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);
						}
						else 
						{
							cur_value = GROUP_CURSORS_POS(angle)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
						}
					}



#if 0
					/* 当前步进 */
					switch (pp->p_tmp_config->VPA_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->VPA;
						lower = 1.0;
						upper = 128.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						unit = UNIT_NONE;
						draw3_digit_pressed (data_311, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->VPA;
						digit = 0;
						pos = 1;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
#endif
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
						cur_value = CFG(min_thickness)/1000.0;
						lower = 0.05;
						upper = (CFG(max_thickness)/1000.0)-0.01;
						step = tmpf;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
						draw3_digit_pressed (data_331, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = CFG(min_thickness)/1000.0;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
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
						else if(CFG(display)==2 || CFG(display)==5 || CFG(display)==7)
							/*Display 为 C-Scan 或 A-B-C 或 A-C-[C]*/
						{
							draw3_pop_tt (data_4011, NULL, 
									menu_content[C_SCAN1+CFG(c_scan1)],
									menu_content + C_SCAN1, 4, 1, CFG(c_scan1), 0);
							str = g_strdup_printf ("%s", con2_p[4][0][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}
						else if(CFG(display)==8)
							/*Display 为 A-S-[C]*/
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
						if(CFG(display)==0 || CFG(display)==3)/*Display 为 A-Scan 或 S-Scan*/
						{
							draw3_popdown (menu_content[GROUP+CFG(display_group)], 1, 0);
						}
						else if(CFG(display)==2 || CFG(display)==5 || CFG(display)==7)
							/*Display 为 C-Scan 或 A-B-C 或 A-C-[C]*/
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
						cur_value = CFG(part.Thickness)/100.0;
						lower = 0.05;
						upper = 1000000.0;
						step = tmpf;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
						draw3_digit_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = CFG(part.Thickness)/100.0;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
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
				case 1:/*Focal Law -> aperture -> first element p611 */
					/* 当前步进 */
					switch (TMP(first_element_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if(CFG(auto_program))/*auto program 为 on 时，Element Qty 才可用*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(first_element);
							lower = 1.0;
							upper = 97.0;
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_611, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(first_element);
							digit = 0;
							pos = 1;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /*auto program 为 off 时，Element Qty   unsensitive*/
					{

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(first_element);
							lower = 1.0;
							upper = 97.0;
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_611, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(first_element);
							digit = 0;
							pos = 1;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					break;


				case 2:/*focal law -> beam -> max angle  p621*/
					/* 当前步进 */
					switch (TMP(max_angle_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}

					if( LAW_VAL(Focal_type)!=1 && CFG(auto_program))
						/*Law Config 不为linear ,同时auto program 为 on 时，Element Qty 才可调*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(max_angle)/100.0;
							lower = -89.9;
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_621, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(max_angle)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
						/*Law Config 为linear ,或auto program 为 off 时，Element Qty 不可调*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = GROUP_VAL(max_angle)/100.0;
							lower = -89.9;
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_621, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(max_angle)/100.0;
							digit = 1;
							pos = 1;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					break;

				case 3:/*Focal Law -> laws -> load law file P631*/
					if(CFG(auto_program))
					{
						draw3_popdown(NULL, 1, 1);
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
					else
						draw3_popdown(NULL, 1, 1);
					break;

				case 4:
					if ( !con2_p[6][4][1] )
						gtk_widget_hide (pp->eventbox30[1]);
					gtk_widget_hide (pp->eventbox31[1]);
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
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if(CFG(i_type)==0 || CFG(i_type)==1)
						/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = pp->p_config->scan_end;
							lower = -100000.0;
							upper = 100000.0;
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_MM;
							draw3_digit_pressed (data_721, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = pp->p_config->scan_end;
							digit = 0;
							pos = 1;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else  /* Inspection -> Type 选择 Helicoidal Scan  时 */
					{
						CFG(scan_start) = 346.0;
						digit = 0;
						pos = 1;
						unit = UNIT_MM;
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
				case 0:/*File -> File -> Open  p801 */
					draw3_popdown(NULL,1,1);
					break;

				case 1:/*File -> report -> file name p811 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][1][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Report####");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;

				case 2:/*File -> format -> probe  p821 */
					draw3_popdown (menu_content[OFF_ON + CFG(format_probe)], 1, 0);
					break;

				case 3:/*File -> user field -> enable  p831 */
					draw3_popdown (menu_content[OFF_ON + CFG(enable)], 1, 0);
					break;

				case 4:/*File -> notes-> edit header p841 */
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


				case 1:/*Preferences -> system -> data set*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s\n(YYYY/MM/DD)", con2_p[9][1][1]);

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

				case 3:/*Preferences -> options -> Option Key  931 */

					draw3_popdown(NULL,1,1);
					break;

				case 4:/*Preferences -> Network -> IP Address  941*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "255.255.255.0");

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

	gfloat cur_value=0.0, lower, upper, step, max_tmp, max_tmp1;
	guint digit, pos, unit;

	//	p = NULL;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:
					if ( !con2_p[0][0][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;
				case 1:
					if ( !con2_p[0][1][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;

				case 2:/*Wizard -> Calibration -> Type  022 */
					pp->x_pos = 570, pp->y_pos = 295;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_022, NULL, 
								type[CFG(type)],
								type, 3, 2, CFG(type), 0);
					else 
						draw3_popdown (type[CFG(type)], 2, 0);

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
							unit = UNIT_NULL;
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
							case ANGLE_SCAN:
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
								cur_value = LAW_VAL(Angle_start)/100.0 ;
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
							cur_value = LAW_VAL(Angle_start)/100.0 ;
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
							switch (pp->p_tmp_config->position_reg)
							{
								case 0:	tmpf = 0.01; break;
								case 1:	tmpf = 0.1; break;
								case 2:	tmpf = 1.0; break;
								case 3:tmpf = 10.0;break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								cur_value = GROUP_VAL(position) / 1000.0;
								lower = 0.0;
								upper = 29306.37;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_pressed (data_2321, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);
							}
							else 
							{
								cur_value = GROUP_VAL(position)/1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
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
								case 3:tmpf=10.0;break;						
								default:break;
							}
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
							{
								cur_value =GROUP_VAL(delay)/1000.0;
								lower = 0.0;
								upper = 296.0;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_pressed (data_2322, units[unit], cur_value , lower, upper, step, digit, p, pos, 16);
							}
							else 
							{

								cur_value = GROUP_VAL(delay)/1000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
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
							cur_value = GROUP_VAL(s_measure)/100.0;
							lower = -1000000.0;
							upper =  1000000.0;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3121, units[unit], cur_value , lower, upper, step, digit, p, pos, 7);
						}
						else 
						{
							cur_value = GROUP_VAL(s_measure)/100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_MM;
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
							cur_value = GROUP_VAL(u_reference)/1000.0;
							lower = 0.0;
							upper = 422.92;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, p, pos, 3);
						}
						else 
						{
							cur_value = GROUP_VAL(u_reference)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
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
							cur_value = GROUP_CURSORS_POS(scan)/100.0;
							lower = -100000.0;
							upper = 100000;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3142, units[unit], cur_value , lower, upper, step, digit, p, pos, 15);
						}
						else 
						{
							cur_value = GROUP_CURSORS_POS(scan)/100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
						}
					}

# if 0
					/* 当前步进 */
					switch (pp->p_tmp_config->cursors_scan_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1;  break;
						case 2:	tmpf = 1.0;  break;
						case 3:	tmpf = 10.0;  break;					
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->cursors_scan;
						lower = 1.0;
						upper = 100.0;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_pressed (data_222, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->cursors_scan;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
#endif
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
						cur_value = CFG(max_thickness)/1000.0;
						lower = (CFG(min_thickness)/1000.0) + 0.01;
						upper = 99999.00;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_pressed (data_332, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = CFG(max_thickness)/1000.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
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
					break;

				case 3:/*Probe/Part -> Parts -> diameter p532 */
					switch(CFG(part.Geometry_pos))
					{
						case 0:
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
								cur_value = pp->p_config->part.Diameter/100.0;
								lower = 100.00;
								upper = 1000000.00;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_pressed (data_532, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							}
							else 
							{
								cur_value = pp->p_config->part.Diameter/100.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							}
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
								cur_value = pp->p_config->part.Diameter/100.0;
								lower = 100.00;
								upper = 1000000.00;
								step = tmpf;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
								draw3_digit_pressed (data_532, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							}
							else 
							{
								cur_value = pp->p_config->part.Diameter/100.0;
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
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
				case 1:/*Focal Law -> aperture -> last element p612 */
					/* 当前步进 */
					switch (TMP(last_element_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if ( LAW_VAL(Focal_type)==1)/*Law Config 为 Linear 时，Last Element可调*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(last_element);
							lower = 32.0;
							upper = 128.0;
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_612, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(last_element);
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}

					else /*Law Config 不为 Linear 时，Last Element  Unsensitive*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(last_element);
							lower = 32.0;
							upper = 128.0;
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_612, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(last_element);
							digit = 0;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}

					break;


				case 2:/*focal law -> beam -> angel step p622 */
					/* 当前步进 */
					switch (pp->p_tmp_config->angle_step_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}

					if( LAW_VAL(Focal_type)!=1 && CFG(auto_program))
						/*Law Config 不为linear ,同时auto program 为 on 时，Element Qty 才可调*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(angle_step)/100.0;
							lower = 0.1;
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_622, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(angle_step)/100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
						/*Law Config 为linear ,或auto program 为 off 时，Element Qty 不可调*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = GROUP_VAL(angle_step)/100.0;
							lower = 0.1;
							upper = 89.9;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_DEG;
							draw3_digit_pressed (data_621, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(angle_step)/100.0;
							digit = 1;
							pos = 2;
							unit = UNIT_DEG;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					}
					break;
				case 3:/*focal law -> laws -> save law file 632*/
					draw3_popdown(NULL, 2, 1);
					break;
				case 4:
					if ( !con2_p[6][4][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
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
						cur_value = CFG(scan_resolution)/100.0;
						lower = 1.0;
						upper = 357.0;
						step = tmpf;
						digit = 1;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_pressed (data_722, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = CFG(scan_resolution)/100.0;
						digit = 1;
						pos = 2;
						unit = UNIT_MM;
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
					break;

				case 1:/*Preferences -> system -> select key  p912 */
					pp->x_pos = 611, pp->y_pos = 295-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_912, NULL, 
								menu_content[SELECT_K + CFG(select_key)],
								menu_content+SELECT_K, 9, 2,CFG(select_key), 0);
					else 
						draw3_popdown (menu_content[SELECT_K+CFG(select_key)], 2, 0);


					break;


				case 2:/*Preferences -> service -> Import/Export p922 */
					draw3_popdown(NULL,2,1);
					break;

				case 3:/*Preferences -> Options -> Remote Desktop p932 */
					draw3_popdown (menu_content[OFF_ON+CFG(remote_desktop)], 2, 0);
					break;

				case 4:/*Preferences -> network -> Apply p942 */
					draw3_popdown(NULL,2,1);
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
	guint digit, pos, unit, content_pos, menu_status = 0;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:
					if ( !con2_p[0][0][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 1:
					if ( !con2_p[0][1][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 2:/*Wizard -> Calibration -> Mode  023 */
					pp->x_pos = 560, pp->y_pos = 380;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_023, NULL, 
								calibration_mode[CFG(calibration_mode)],
								calibration_mode, 4, 3, CFG(calibration_mode), 0);
					else 
						draw3_popdown (calibration_mode[CFG(calibration_mode)], 3, 0);

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
							cur_value = GROUP_VAL(u_reference)/1000.0;
							lower = 0.0;
							upper = 422.92;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(u_reference)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
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
							cur_value = GROUP_VAL(i_reference)/100.0;
							lower = -999999.0;
							upper = 999999.0;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3131, units[unit], cur_value , lower, upper, step, digit, p, pos, 9);
						}
						else 
						{
							cur_value = GROUP_VAL(i_reference)/100.0;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
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
							cur_value = GROUP_VAL(s_refmeas)/100.0;
							lower = 12.0;
							upper = 400.5;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3132, units[unit], cur_value , lower, upper, step, digit, p, pos, 11);
						}
						else 
						{
							cur_value = GROUP_VAL(s_refmeas)/100.0;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
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
							cur_value = GROUP_VAL(u_measure)/1000.0;
							lower = 0.0;
							upper = 422.92;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, p, pos, 4);
						}
						else 
						{
							cur_value = GROUP_VAL(u_measure)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
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
							cur_value = GROUP_CURSORS_POS(UT)/100.0;
							lower = -0.07;
							upper = 58.3;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3133, units[unit], cur_value , lower, upper, step, digit, p, pos, 14);
						}
						else 
						{
							cur_value = GROUP_CURSORS_POS(UT)/100.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
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
							cur_value = GROUP_CURSORS_POS(index)/100.0;
							lower = -100000.0;
							upper = 100000;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3151, units[unit], cur_value , lower, upper, step, digit, p, pos, 16);
						}
						else 
						{
							cur_value = GROUP_CURSORS_POS(index)/100.0;
							digit = 1;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
						}

					}
# if 0
					/* 当前步进 */
					switch (pp->p_tmp_config->cursors_index_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0;  break;
						case 2:	tmpf = 100.0;  break;				
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = pp->p_config->cursors_index;
						lower = 0.5;
						upper = 127.5;
						step = tmpf;
						digit = 1;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->cursors_index;
						digit = 1;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
#endif
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
				case 0:/*Probe/Part -> Select -> Probe p503 */
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
								draw3_popdown(GROUP_VAL(probe.Model),3,0);

							g_sprintf (temp,"%s", con2_p[5][0][3]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}


						else if( CFG(probe_select)==1 )
						{
							draw3_popdown(NULL,3,1);
							g_sprintf (temp,"%s", con2_p[5][0][6]);
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
						}
					}

					if( GROUP_VAL(group_mode) && CFG(auto_detect) )	/* PA模式时，探头不可选 */
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
					break;


				case 3:/*Probe/Part -> parts -> material  p533 */
					pp->x_pos = 563, pp->y_pos = 370-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_533, NULL, 
								menu_content[MATERIAL + CFG(part.Material)],
								menu_content+MATERIAL, 7, 3, CFG(part.Material), 0);
					else 
						draw3_popdown (menu_content[MATERIAL + CFG(part.Material)], 3, 0);

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
				case 1:/*Focal Law -> aperture -> element step p613*/
					/* 当前步进 */
					switch (pp->p_tmp_config->element_step_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if (LAW_VAL(Focal_type)==1)/*Law Config 为 Linear 时，Element Step 可调*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							cur_value = GROUP_VAL(element_step);
							lower = 0.5;
							upper = 96.0;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_613, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(element_step);
							digit = 1;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else /*Law Config 不为 Linear 时，Element Step  Unsensitive*/
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							cur_value = GROUP_VAL(element_step);
							lower = 0.5;
							upper = 96.0;
							step = tmpf;
							digit = 1;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_613, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(element_step);
							digit = 1;
							pos = 3;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
						gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
					}
					break;


				case 2:/*Focal law -> Beam -> focus depth p623 */
					/* 当前步进 */
					switch (TMP(focus_depth_reg))
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = GROUP_VAL(focus_depth)/100.0;
						lower = 0.10;
						upper = 100.00;
						step = tmpf;
						digit = 1;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_pressed (data_623, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = GROUP_VAL(focus_depth)/100.0;
						digit = 1;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 3:
					if ( !con2_p[6][3][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 4:
					if ( !con2_p[6][4][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
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
						cur_value = CFG(scanspeed) / 10.0;
						lower = 1.0;
						upper = 100.0;
						step = tmpf;
						digit = 1;
						pos = 3;
						unit = UNIT_MM_S;
						draw3_digit_pressed (data_713, units[unit], cur_value , lower, upper, step, digit, p, pos, content_pos);
					}
					else 
					{
						cur_value = CFG(scanspeed)/10.0;
						digit = 1;
						pos = 3;
						unit = UNIT_MM_S;
						draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
					}
					break;
				case 2:/*Scan -> area -> index start p723 */
					/* 当前步进 */
					switch (TMP(index_start_reg))
					{
						case 0:	tmpf = 0.01 * CFG(index_resolution); break;
						case 1:	tmpf = 0.1 * CFG(index_resolution); break;
						case 2:	tmpf = 1.0 * CFG(index_resolution); break;
						default:break;
					}
					if(CFG(i_type)==1 || CFG(i_type)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							cur_value = CFG(index_start);
							lower = -99999.0;
							upper = 99999.0;
							step = tmpf;
							digit = 0;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_723, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG(index_start);
							digit = 0;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
						/* Inspection -> Type 选择 One-Line Scan 时 */
					{
						cur_value = CFG(index_start);
						digit = 0;
						pos = 3;
						unit = UNIT_MM;
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
					break;

				case 1:/*File -> report -> build p813 */
					draw3_popdown(NULL,3,1);
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
	guint digit, pos, unit, content_pos;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:
					if ( !con2_p[0][0][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 1:
					if ( !con2_p[0][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 2:
					if ( !con2_p[0][2][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
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
							cur_value = GROUP_VAL(u_measure)/1000.0;
							lower = 0.0;
							upper = 422.92;
							step = tmpf;
							digit = 2;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_VAL(u_measure)/1000.0;
							digit = 2;
							pos = 4;
							unit = UNIT_MM;
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
							cur_value = GROUP_VAL(i_measure)/100.0;
							lower = -999999.0;
							upper = 999999.0;
							step = tmpf;
							digit = 1;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3141, units[unit], cur_value , lower, upper, step, digit, p, pos, 10);
						}
						else 
						{
							cur_value = GROUP_VAL(i_measure)/100.0;
							digit = 1;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
						}
					}
					else if( GROUP_VAL(selection)==3)
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
					}
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
							cur_value = GROUP_CURSORS_POS(scan)/100.0;
							lower = -100000.0;
							upper = 100000;
							step = tmpf;
							digit = 1;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3142, units[unit], cur_value , lower, upper, step, digit, p, pos, 15);
						}
						else 
						{
							cur_value = GROUP_CURSORS_POS(scan)/100.0;
							digit = 1;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
						}
					}
					else if (GROUP_VAL(selection)==8)
					{
						draw3_popdown(NULL,4,1);
						g_sprintf (temp,"%s", con2_p[3][1][17]);
						gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					}

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
				case 0:/* Probe/Part -> Select -> Wedge p504*/
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

				case 1:
					if ( !con2_p[5][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 2:/*Probe/Part -> characterize -> procedure*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][2][4]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Undefined");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[4]);
					gtk_widget_show (pp->eventbox31[4]);
					gtk_widget_show (pp->data3[4]);
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
				case 1:/*Focal Law -> aperture -> wave type  p614 */
					pp->x_pos = 544, pp->y_pos = 456-YOFFSET;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_614, NULL, 
								menu_content[W_TYPE+GROUP_VAL(wave_type)],
								menu_content+WAVE_TYPE, 3, 4, GROUP_VAL(wave_type), 0);
					else 
						draw3_popdown (menu_content[W_TYPE+GROUP_VAL(wave_type)], 4, 0);

					break;


				case 2:
					if ( !con2_p[6][2][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 3:
					if ( !con2_p[6][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
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
						case 0:	tmpf = 0.01 * CFG(index_resolution); break;
						case 1:	tmpf = 0.1 * CFG(index_resolution); break;
						case 2:	tmpf = 1.0 * CFG(index_resolution); break;
						default:break;
					}

					if(CFG(i_type)==1 || CFG(i_type)==2)
						/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							cur_value = CFG(index_end);
							lower = -100000.0;
							upper = 100000.0;
							step = tmpf;
							digit = 0;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_pressed (data_724, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG(index_end);
							digit = 0;
							pos = 4;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}

					else
						/* Inspection -> Type 选择 One-Line Scan 时 */
					{
						cur_value = CFG(index_end);
						digit = 0;
						pos = 4;
						unit = UNIT_MM;
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
	guint digit, pos, unit, content_pos;

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
				case 1:
					if ( !con2_p[0][1][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 2:/* Wizard -> Calibration -> clear calib p025*/
					g_sprintf (temp,"%s", con2_p[0][2][5]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
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
							else							/* Auto ==时候 显示当前选项 */
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
					else if( GROUP_VAL(selection)==4 )
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
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						{
							cur_value = GROUP_VAL(s_measure)/100.0;
							lower = -1000000.0;
							upper =  1000000.0;
							step = tmpf;
							digit = 1;
							pos = 5;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3121, units[unit], cur_value , lower, upper, step, digit, p, pos, 7);
						}
						else 
						{
							cur_value = GROUP_VAL(s_measure)/100.0;
							digit = 1;
							pos = 5;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
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
							cur_value = GROUP_CURSORS_POS(index)/100.0;
							lower = -100000.0;
							upper = 100000;
							step = tmpf;
							digit = 1;
							pos = 5;
							unit = UNIT_MM;
							draw3_digit_pressed (data_3151, units[unit], cur_value , lower, upper, step, digit, p, pos, 16);
						}
						else 
						{
							cur_value = GROUP_CURSORS_POS(index)/100.0;
							digit = 1;
							pos = 5;
							unit = UNIT_MM;
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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))/*选中405这个位置*/
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

							cur_value = CFG(avg_scan_speed)/100.0;
							lower = 0.01;
							upper = 100.0;	/* 与prf一样 */
							step = tmpf;
							digit = 2;
							pos = 5;
							unit = UNIT_MM_S;
							draw3_digit_pressed (data_405, units[unit], cur_value , lower, upper, step, digit, p, pos, 12);							

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
				case 2:/*Probe/Part -> characterize -> procedure*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][2][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Undefined");

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[5]);
					gtk_widget_show (pp->eventbox31[5]);
					gtk_widget_show (pp->data3[5]);
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
				case 1:
					if ( !con2_p[6][1][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
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
							cur_value = CFG(index_resolution)/100.0;
							lower = 1.0;
							upper = 100.0;
							step = tmpf;
							digit = 1;
							pos = 5;
							unit = UNIT_MM;
							draw3_digit_pressed (data_725, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = CFG(index_resolution)/100.0;
							digit = 1;
							pos = 5;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
						/* Inspection -> Type 选择 One-Line Scan 时 */
					{
						cur_value = CFG(index_resolution)/100.0;
						digit = 0;
						pos = 5;
						unit = UNIT_MM;
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
				(pp->pos_last2 == i) || (CUR_POS == i) || pa) 
		{
			gtk_widget_set_sensitive (pp->eventbox30[i], TRUE);
			gtk_widget_set_sensitive (pp->eventbox31[i], TRUE);
			gtk_widget_set_size_request(GTK_WIDGET(pp->eventbox30[i]), 114, 60);            /* */


			/*
			 * 如果三级菜单存在, 显示出来, 并显示 选项或者数值
			 * 不存在就隐藏
			 * */
			//if ( con2_p[pp->pos][pp->pos1[pp->pos]][i] ) 
			//{
			if (CUR_POS != i) {
				/* 0-5 表示6个3三级菜单 */
				switch (i)
				{
					case 0:	draw3_data0(pp);break;
					case 1:	draw3_data1(pp);break;
					case 2:	draw3_data2(pp);break;
					case 3:	draw3_data3(pp);break;
					case 4:	draw3_data4(pp);break;
					case 5:	draw3_data5(pp);break;
					default:break;
				}
			}
			//}
			//else
			//{
			//	gtk_label_set_text (GTK_LABEL (pp->label3[i]), " ");
			//	gtk_widget_hide (pp->eventbox30[i]);	/**/
			//	gtk_widget_hide (pp->eventbox31[i]);	/**/
			//	gtk_widget_hide (pp->sbutton[i]);	/**/
			//}

		}
		gtk_widget_hide (pp->sbutton[i]);
		//gtk_widget_hide (pp->vscalebox);
		//		gtk_widget_show (pp->vscalebox);
		gtk_widget_show (pp->scale_drawarea);
		gtk_widget_hide (pp->button_add);
		gtk_widget_hide (pp->button_sub);
		gtk_widget_hide (pp->vscale);
		//gtk_widget_set_size_request (GTK_WIDGET(pp->drawing_area), 658, 390);
	}
	switch (CUR_POS)
	{
		case 0:	draw3_data0(pp);break;
		case 1:	draw3_data1(pp);break;
		case 2:	draw3_data2(pp);break;
		case 3:	draw3_data3(pp);break;
		case 4:	draw3_data4(pp);break;
		case 5:	draw3_data5(pp);break;
		default:break;
	}

	return ;
}

/*  */
static gboolean time_handler1(GtkWidget *widget)
{
	time_t curtime;
	struct tm *loctime;
	gchar *markup;

	curtime = time(NULL);
	loctime = localtime(&curtime);
	strftime(buffer, 32, "%F %T", loctime);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>%s</span>", buffer);
	gtk_label_set_markup (GTK_LABEL(pp->label[4]),markup);

	g_free (markup);
	return TRUE;
}

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

/*  */
static void draw_scan(guchar scan_num, guchar scan_type, guchar group,
		guint xoff, guint yoff)
{
	switch (scan_type)
	{
		case A_SCAN:
			draw_a_scan(dot_temp1, TMP(a_scan_width), TMP(a_scan_height),
					TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
					dot_temp, dot_temp, 
					xoff, yoff, group);
			break;
		case B_SCAN:
			if (pp->bscan_mark)
			{
				draw_b_scan(dot_temp1, TMP(b_scan_width), TMP(b_scan_height),dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 1);
				pp->bscan_mark = 0;
			}
			else
				draw_b_scan(dot_temp1, TMP(b_scan_width), TMP(b_scan_height),dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 0);
			break;
		case S_SCAN:


			break;
		case C_SCAN:
			break;
		default:break;
	}
	return ;
}


/*   */
static gboolean time_handler2(GtkWidget *widget)
{
	gint i, j, k, prf_count, offset;
	pp->scan_count++;

	/* 第一个GROUP */
	(GROUP_VAL(prf) > 250) ? (prf_count = 25) : (prf_count = (GROUP_VAL(prf) / 10));
	prf_count = 25 / prf_count;

	for (i = 0 ; i < CFG(groupQty); i++)
	{
		(GROUP_VAL_POS(i, prf) > 250) ? 
			(prf_count = 25) : (prf_count = (GROUP_VAL_POS(i, prf) / 10));
		prf_count = 25 / prf_count;
		if ((pp->scan_count % prf_count) == 0)
		{
			/* 获取数据 */
			/* 这里需要压缩数据 或者 插值数据 这里只有一个beam 同时最多处理256beam */
			for	(j = 0 ; j < TMP(beam_qty[i]); j++)
			{  
				for (offset = 0, k = 0 ; k < i; k++)
					offset += GROUP_VAL_POS((k), point_qty) * TMP(beam_qty[k]);
				if (GROUP_VAL_POS(i, point_qty) <= TMP(a_scan_width))
				{
					interpolation_data (
							(DOT_TYPE *)(pp->p_beam_data + offset +
								GROUP_VAL_POS(i, point_qty) * j),
							TMP(scan_data[i] + TMP(a_scan_width) * j), 
							GROUP_VAL_POS(i, point_qty),
							TMP(a_scan_width));
				}
				else if (GROUP_VAL_POS(i, point_qty) > TMP(a_scan_width))
					compress_data (
							(DOT_TYPE *)(pp->p_beam_data + offset +
								GROUP_VAL_POS(i, point_qty) * j),
							TMP(scan_data[i] + TMP(a_scan_width) * j), 
							GROUP_VAL_POS(i, point_qty),
							TMP(a_scan_width), 
							GROUP_VAL_POS(i, rectifier));
			}
			for (k = 0; ((k < 16) && (TMP(scan_type[k]) != 0xff)); k++)
			{
				if (TMP(scan_group[k]) == i)
					draw_scan(k, TMP(scan_type[k]), TMP(scan_group[k]), 
							TMP(scan_xpos[k]), TMP(scan_ypos[k]));
			}
		} 
	}


//	for (i = 0; (i < 16) && (TMP(scan_type[i]) != 0xff); i++)
//	{
//		draw_scan(i, TMP(scan_type[i]), TMP(scan_group[i]), TMP(scan_xpos[i]), TMP(scan_ypos[i]));
//	}

	/* 复制波形到显存 */
	memcpy (TMP(fb1_addr), dot_temp1, FB_WIDTH*400*2);	/* 如果用dma更快啊 */

	return TRUE;
}

/*  */
void init_ui(DRAW_UI_P p)				/*初始化界面,*/
{
	gint i;
	gchar buf[128];
	//GtkWidget *drawing_area;
	GtkWidget *window = p->window;
	pp->pos_pos = MENU3_STOP;
	pp->menu2_qty = 5;
	GtkWidgetClass *widget_window_class1;
	gchar *markup;
	//WebKitWebView* web_view;


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
	p->vbox12		= gtk_vbox_new(FALSE, 0);
	p->hbox121		= gtk_hbox_new(FALSE, 0);	
	p->hbox111		= gtk_hbox_new(FALSE, 0);	
	p->vbox1111[0]	        = gtk_vbox_new(FALSE, 0);	
	p->vbox1111[1]   	= gtk_vbox_new(FALSE, 0);	
	p->vbox1111[2]  	= gtk_vbox_new(FALSE, 0);	
	p->hbox112		= gtk_hbox_new(FALSE, 0);	

	p->hbox2		= gtk_hbox_new(FALSE, 0);	
	p->vbox21		= gtk_vbox_new(FALSE, 0);	
	p->hbox211		= gtk_hbox_new(FALSE, 0);
	p->vboxtable            = gtk_vbox_new(FALSE, 0);
	p->sw			= gtk_scrolled_window_new(NULL, NULL);


	//	p->vbox2111		= gtk_vbox_new(FALSE, 0);	
	//	p->hbox2112		= gtk_hbox_new(FALSE, 0);	
	p->hbox212		= gtk_hbox_new(FALSE, 0);	

	p->vbox22		= gtk_vbox_new(FALSE, 0);	

	for (i = 0; i < 6; i++)
		p->vbox221[i]	= gtk_vbox_new(FALSE, 0);	

	for (i = 0; i < 20; i++) {
		pp->label[i]	= gtk_label_new ("");
		gtk_widget_modify_fg(pp->label[i], GTK_STATE_NORMAL, &color_white);	/* 字体颜色白色 */
		pp->event[i]  =  gtk_event_box_new();
		gtk_container_add(GTK_CONTAINER(p->event[i]), p->label[i]);
		gtk_container_set_border_width( GTK_CONTAINER(pp->event[i]), 0);     /*设置边框大小，这个地方使用图片*/
	}

	widget_window_class = GTK_WIDGET_GET_CLASS (((GtkObject*)(pp->window))); 
	// 取代原來的處理函式
	window_keypress_event_orig = widget_window_class->key_press_event; 

	/*
	   pp->button = gtk_button_new_with_label(" ");
	   gtk_box_pack_start(GTK_BOX(pp->hbox1), pp->button, FALSE, FALSE, 0);
	   gtk_widget_show(pp->button);
	   */

	/* 一级菜单的初始化 */
	p->menubar		= gtk_menu_bar_new();
	p->menu			= gtk_menu_new();
	p->menuitem_main	= gtk_menu_item_new_with_label(content_en10[1]);
	gtk_widget_set_size_request(GTK_WIDGET(p->menuitem_main), 114, 83);          /*刚好合适的大小*/
	/*	gtk_widget_modify_fg(p->menuitem_main, GTK_STATE_NORMAL, &color_red);
		gtk_widget_modify_bg(p->menuitem_main, GTK_STATE_NORMAL, &color_green);*/
	gtk_menu_bar_append(GTK_MENU_BAR(p->menubar), p->menuitem_main);
	for (i = 0; i < 10; i++)
	{
		p->menuitem[i]	= gtk_menu_item_new_with_label(content_en10[i]);
		gtk_widget_set_size_request(GTK_WIDGET(p->menuitem[i]), 120, 35);
		g_signal_connect(G_OBJECT(p->menuitem[i]), "activate", 
				G_CALLBACK(menu_fun[i]), (gpointer) (p));
		gtk_menu_shell_append(GTK_MENU_SHELL(p->menu), p->menuitem[i]);
		gtk_widget_show(p->menuitem[i]);
		if ((i == 6) && GROUP_VAL (group_mode) == UT_SCAN)
			gtk_widget_set_sensitive(pp->menuitem[i] ,FALSE);
	}
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(p->menuitem_main), p->menu);/*最后把菜单menu1粘到菜单项menuitem1上*/
	gtk_box_pack_start(GTK_BOX(p->hbox212), p->menubar, FALSE, FALSE, 0);
	/*g_object_set (p->menubar, "pack-direction", GTK_PACK_DIRECTION_LTR, NULL); */
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
		p->label2[i] = gtk_label_new("<^_^>");
		gtk_container_add(GTK_CONTAINER(p->eventbox2[i]), p->label2[i]);
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox2[i]), 0);     /*设置边框大小，这个地方使用图片做背景*/
		update_widget_bg(p->eventbox2[i], backpic[1]);
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

		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox30[i]), 114, 60);           /* 配置名称*/
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox31[i]), 114, 25);           /* 标签 */
		gtk_widget_set_size_request(GTK_WIDGET(p->sbutton[i]), 114, 25);           /* 标签 */
		update_widget_bg(pp->eventbox30[i], backpic[7]);
		update_widget_bg(pp->eventbox31[i], backpic[10]);
		gtk_widget_modify_base(GTK_WIDGET (pp->sbutton[i]), GTK_STATE_NORMAL, &color_text_base);
		gtk_widget_modify_text(GTK_WIDGET (pp->sbutton[i]), GTK_STATE_NORMAL, &color_yellow);
		//		update_widget_bg(pp->sbutton[i], backpic[10]);

		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox30[i]), 0);     /*设置边框大小，这个地方使用图片*/
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox31[i]), 0);     /*设置边框大小，这个地方使用图片*/
		gtk_container_add(GTK_CONTAINER(p->eventbox30[i]), p->label3[i]);
		gtk_container_add(GTK_CONTAINER(p->eventbox31[i]), p->data3[i]);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox30[i], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->eventbox31[i], FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->sbutton[i], FALSE, FALSE, 0);
		gtk_widget_modify_fg(p->label3[i], GTK_STATE_NORMAL, &color_white);	/* 字体颜色白色 */
		gtk_widget_modify_fg(p->data3[i], GTK_STATE_NORMAL, &color_white);
		g_signal_connect(G_OBJECT(p->eventbox30[i]), "button-press-event", 
				G_CALLBACK(data_fun[i]), (GUINT_TO_POINTER (i)));
		g_signal_connect(G_OBJECT(p->eventbox31[i]), "button-press-event", 
				G_CALLBACK(data_fun[i]), (GUINT_TO_POINTER (i)));
		gtk_widget_show(p->eventbox30[i]);
		gtk_widget_show(p->eventbox31[i]);
		gtk_widget_show(p->label3[i]);
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

	for ( i = 0; i < 4; i++)
	{
		sprintf (buf, "Test-doppler- %d", i);

		pp->menu_item3[i] = gtk_menu_item_new_with_label(buf);

		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu3), pp->menu_item3[i]);

		//		g_signal_connect_swapped (menu_items[i], "activate", 
		//				G_CALLBACK(menuitem_respnse), (gpointer) g_strdu(buf));
		gtk_widget_show(pp->menu_item3[i]);
	}
	pp->root_menu3 = gtk_menu_item_new_with_label ("TAN");

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (pp->root_menu3), pp->menu3);
//	g_signal_connect (pp->menu3, "key-press-event", 
//			G_CALLBACK(key_press_handler) ,(gpointer)(MENU3_PRESS));

	pp->menu_bar3 = gtk_menu_bar_new ();
	gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu_bar3) , pp->root_menu3);


	/* 各box的包含关系*/
	gtk_container_add(GTK_CONTAINER(window), p->vbox);
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
			G_CALLBACK (draw_other_info), NULL);


	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->hbox121, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[17], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[17]), 40, 25);
	update_widget_bg(pp->event[17], backpic[12]); 
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
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>", GROUP_VAL(gain) / 100.0);
	gtk_label_set_markup (GTK_LABEL (pp->label[1]), markup);      /* 增益数值 */

	/* 小状态栏  */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[2], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[2], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[2]), 172, 22);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>%s</span>", "current.cfg");
	gtk_label_set_markup (GTK_LABEL (pp->label[2]), markup); 
	update_widget_bg(pp->event[2], backpic[5]);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[3], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[3]), 172, 22);
	gtk_label_set_justify (GTK_LABEL (pp->label[3]), PANGO_ELLIPSIZE_START);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>PRF: %d(%d)</span>",
			GROUP_VAL(prf), GROUP_VAL(prf));
	gtk_label_set_markup (GTK_LABEL (pp->label[3]), markup); 
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
	update_widget_bg(pp->event[6], backpic[5]);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",
			(gfloat)(GROUP_VAL(prf)));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup); ;
	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[7], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[7]), 172, 22);
	update_widget_bg(pp->event[7], backpic[5]);
	markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f s</span>",
			(gfloat)(GROUP_VAL(prf)));
	gtk_label_set_markup (GTK_LABEL (pp->label[7]), markup); 

	/* 4个测量值显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[8], FALSE, FALSE, 0);
	//gtk_label_set_text (GTK_LABEL (pp->label[8]), "%A\n(%)");
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>%%A\n(%%)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[8]),markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[8]), 60, 45);
	update_widget_bg(pp->event[8], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[9], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[9]),markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[9]), 112, 45);
	update_widget_bg(pp->event[9], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[10], FALSE, FALSE, 0);
	//gtk_label_set_text (GTK_LABEL (pp->label[10]), "%A\n(%)");
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>DA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[10]),markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[10]), 60, 34);
	update_widget_bg(pp->event[10], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[11], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[11]),markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[11]), 112, 34);
	update_widget_bg(pp->event[11], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[12], FALSE, FALSE, 0);
	//gtk_label_set_text (GTK_LABEL (pp->label[12]), "%A\n(%)");
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>PA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[12]),markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[12]), 60, 34);
	update_widget_bg(pp->event[12], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[13], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[13]),markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[13]), 112, 34);
	update_widget_bg(pp->event[13], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[14], FALSE, FALSE, 0);
	//gtk_label_set_text (GTK_LABEL (pp->label[14]), "%A\n(%)");
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='10'>SA^\n(mm)</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[14]),markup);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[14]), 60, 34);
	update_widget_bg(pp->event[14], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[15], FALSE, FALSE, 0);
	markup=g_markup_printf_escaped("<span foreground='white' font_desc='24'>ND</span>");
	gtk_label_set_markup (GTK_LABEL(pp->label[15]),markup);
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
	pp->web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());	/*???*/
	webkit_web_view_set_custom_encoding (pp->web_view, "UTF-8");	/*???????????*/
	gtk_container_add(GTK_CONTAINER (pp->sw), GTK_WIDGET (pp->web_view));

	//gchar *file_path="/home/gll/arraytop/TT/source/system/Help/Contextual/Wizard_Menu.html";
	//webkit_web_view_load_uri (web_view, "file:///home/tt/TT/source/system/Help/Contextual/UT_Settings_Menu.html");
	pp->file_path = "file:///home/gll/arraytop/TT/source/system/Help/Contextual/UT_Settings_Menu.html";
	webkit_web_view_load_uri (pp->web_view, pp->file_path);

	gtk_box_pack_start (GTK_BOX (p->hbox211), p->vboxtable, FALSE, FALSE, 0);
	gtk_widget_show(p->vboxtable);

	draw_area_all ();
	gtk_widget_show_all (p->hbox2); /* 画图区域 及 button 的显示 */

	/* 弹出菜单 是否透明 waiting */
	pp->dialog = gtk_dialog_new_with_buttons("TanDenghua", GTK_WINDOW (pp->window),
			/*			GTK_DIALOG_MODAL| */  /* 独占窗口 */
			// GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT |
			GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

	widget_window_class1 = GTK_WIDGET_GET_CLASS (((GtkObject*)(pp->dialog))); 
	widget_window_class1->key_press_event =	my_keypress_event;

	gtk_window_set_decorated (GTK_WINDOW (pp->dialog), FALSE);			/*不可以装饰*/
	gtk_container_set_border_width( GTK_CONTAINER (GTK_DIALOG(pp->dialog)->vbox), 0);     /* */
	gtk_widget_show (GTK_DIALOG(pp->dialog)->vbox);
	//	gtk_widget_show (pp->dialog);
	gtk_widget_hide (GTK_DIALOG(pp->dialog)->action_area);


	/* scale 上面的透明条 */
	pp->scale_drawarea = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(pp->scale_drawarea), 30, 425);
	gtk_widget_modify_bg(pp->scale_drawarea, GTK_STATE_NORMAL, &color_black);
	//gtk_widget_modify_fg(pp->scale_drawarea, GTK_STATE_NORMAL, &color_black);

	/* scale 快速调节数值 */
	pp->button_add = gtk_button_new_with_label ("+");    /* 加减数值 */
	g_signal_connect (pp->button_add, "clicked", 
			G_CALLBACK(add_click), NULL);
	pp->button_sub = gtk_button_new_with_label ("-");    /*  */
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

	for (i = 0; i < 6; i++)
	{
		gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221[i], FALSE, FALSE, 0);
		gtk_widget_show(p->vbox221[i]);
	}

	draw_1_menu(p);
	draw_2_menu(1);
	draw_3_menu(1, NULL);

#if ARM
	g_timeout_add(40, (GSourceFunc) time_handler2, NULL);
#endif
	//	g_thread_create((GThreadFunc)(time_handler), (gpointer) (pp->drawing_area), FALSE, NULL);
}
