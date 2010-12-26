/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 *
 *
 */

#include "drawui.h"
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>

gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);
GtkWidgetClass *widget_window_class;

GdkPoint a[512] = {{0,0}, {240,200}, {400,200}};

const gchar *backpic[] = 
{
	"pic/10.png", "pic/11.png", "pic/12.png",		/* 二级菜单 0按下 1未选中 2 停留*/
	"pic/00.png", "pic/01.png", "pic/02.png",	    /* 00上方数值 标识栏 01 数值背景 02 比较小的背景 */
	"pic/20.png", "pic/21.png", "pic/22.png",		/* 三级菜单名称 0按下 1未选中 2 停留 */
	"pic/30.png", "pic/31.png", "pic/32.png"		/* 三级菜单数值 0按下 1未选中 2 停留 */
};

void change_language();
void draw_1_menu(DRAW_UI_P p);
void draw_2_menu(gint pa);
void draw_3_menu(gint pa, gpointer p);
void init_ui(DRAW_UI_P p);				/* 初始化界面 */

void draw3_pressed1(gfloat step, guint digit);
/**/
const gchar **con0_p	 = content_en10;
const gchar ***con1_p	 = content1_en;
const gchar ****con2_p	 = content2_en;

const gchar **units	 = units_en;

const gchar **type       = type_en;
const gchar **calibration_mode       = calibration_mode_en;

const gchar **tx_rx_mode = tx_rx_mode_en;
const gchar **freq	 = freq_en;
const gchar **voltage    =voltage_en;
const gchar **PW         =PW_en;
const gchar **PRF        =PRF_en;
const gchar **filter     =filter_en;
const gchar **rectifier  =rectifier_en;
const gchar **averaging  =averaging_en;
const gchar **points_qty =points_qty_en;
const gchar **sum_gain   =sum_gain_en;

const gchar **gate       = gate_en;
const gchar **parameters = parameters_en;
const gchar **gate_alarm = gate_alarm_en;
const gchar **groupA     = groupA_en;
const gchar **conditionA = conditionA_en;
const gchar **operator   = operator_en;
const gchar **groupB     = groupB_en;
const gchar **conditionB = conditionB_en;
const gchar **output     = output_en;
const gchar **output_alarm= output_alarm_en;
const gchar **sound      = sound_en;
const gchar **curves_mode= curves_mode_en;
const gchar **curve      = curve_en;

const gchar **list       =list_en;
const gchar *field1      =field1_en;
const gchar **selection  =selection_en;
const gchar **source     =source_en;

const gchar **display    =display_en;
const gchar **group      =group_en;
const gchar **utunit     =utunit_en;
const gchar **grid       =grid_en;
const gchar **zoom_display =zoom_display_en;
const gchar **select1     =select_en;
const gchar **scan       =scan_en;
const gchar **color      =color_en;
const gchar **envelope   =envelope_en;
const gchar **properties_source =properties_source_en;
const gchar **appearance =appearance_en;
const gchar **overlay    =overlay_en;

const gchar **group_mode =group_mode_en;
const gchar **probe_select =probe_select_en;
const gchar **probe_skew   =probe_skew_en;
const gchar **geometry   =geometry_en;
const gchar **material   =material_en;


const gchar **law_config        =law_config_en;
const gchar **wave_type         =wave_type_en;


const gchar **inspection_type   =inspection_type_en;
const gchar **inspection_scan   =inspection_scan_en;
const gchar **start_mode        =start_mode_en;
const gchar **storage           =storage_en;
const gchar **inspec_data       =inspec_data_en;


const gchar **file_storage      =file_storage_en;
const gchar **save_mode         =save_mode_en;
const gchar **template          =template_en;
const gchar **paper_size        =paper_size_en;
const gchar **view              =view_en;
const gchar **file_select       =file_select_en;

const gchar **pref_units        =pref_units_en;
const gchar **scheme            =scheme_en;
const gchar **gate_mode         =gate_mode_en;
const gchar **select_key        =select_key_en;
const gchar **assign_key        =assign_key_en;
const gchar **startup_mode      =startup_mode_en;
const gchar **mouse             =mouse_en;


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
	gpointer data;
	key_press_handler (widget, event, data);
	return 0;
}

void set_menu_position0_200(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 630; *y = 120;return;}
void set_menu_position0_210(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 603; *y = 118;return;}
void set_menu_position0_220(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 551; *y = 118;return;}
void set_menu_position0_230(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 605; *y = 118;return;}
void set_menu_position0_300(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 505; *y = 118;return;}
void set_menu_position0_310(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 575; *y = 118;return;}
void set_menu_position0_330(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 595; *y = 118;return;}
void set_menu_position0_400(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 480; *y = 118;return;}
void set_menu_position0_410(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 118;return;}
void set_menu_position0_420(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 515; *y = 118;return;}
void set_menu_position0_430(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 520; *y = 118;return;}
void set_menu_position0_440(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 575; *y = 118;return;}
void set_menu_position0_530(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 608; *y = 118;return;}
void set_menu_position0_600(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 400; *y = 118;return;}
void set_menu_position0_710(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 540; *y = 118;return;}
void set_menu_position0_730(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 549; *y = 120;return;}
void set_menu_position0_740(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 543; *y = 118;return;}
void set_menu_position0_800(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 500; *y = 118;return;}
void set_menu_position0_810(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 500; *y = 118;return;}
void set_menu_position0_830(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 623; *y = 118;return;}
void set_menu_position0_900(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 567; *y = 120;return;}
void set_menu_position0_930(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 560; *y = 120;return;}

void set_menu_position1(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 480; *y = 209;return;}
void set_menu_position1_121(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 511; *y = 168;return;}
void set_menu_position1_201(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 590; *y = 218;return;}
void set_menu_position1_211(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 632; *y = 210;return;}
void set_menu_position1_221(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 600; *y = 153;return;}
void set_menu_position1_231(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 575; *y = 213;return;}
void set_menu_position1_401(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 595; *y = 213;return;}
void set_menu_position1_411(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 600; *y = 210;return;}
void set_menu_position1_441(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 600; *y = 210;return;}
void set_menu_position1_501(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 500; *y = 210;return;}
void set_menu_position1_711(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 575; *y = 210;return;}
void set_menu_position1_741(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 541; *y = 210;return;}

void set_menu_position2(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 614; *y = 245;return;}
void set_menu_position2_022(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 295;return;}
void set_menu_position2_122(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 608; *y = 265;return;}
void set_menu_position2_212(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 530; *y = 265;return;}
void set_menu_position2_302(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 430; *y = 265;return;}
void set_menu_position2_442(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 600; *y = 300;return;}
void set_menu_position2_502(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 560; *y = 300;return;}
void set_menu_position2_512(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 295;return;}
void set_menu_position2_812(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 600; *y = 295;return;}
void set_menu_position2_912(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 607; *y = 295;return;}

void set_menu_position3(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 592; *y = 400;return;}
void set_menu_position3_023(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 560; *y = 380;return;}
void set_menu_position3_123(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 592; *y = 400;return;}
void set_menu_position3_143(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 380;return;}
void set_menu_position3_213(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 615; *y = 380;return;}
void set_menu_position3_223(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 430; *y = 380;return;}
void set_menu_position3_303(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 430; *y = 350;return;}
void set_menu_position3_443(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 565; *y = 380;return;}
void set_menu_position3_533(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 565; *y = 380;return;}
void set_menu_position3_903(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 588; *y = 380;return;}
void set_menu_position3_913(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 550; *y = 380;return;}

void set_menu_position4(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 470;return;}
void set_menu_position4_124(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 623; *y = 440;return;}
void set_menu_position4_214(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 633; *y = 470;return;}
void set_menu_position4_304(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 433; *y = 440;return;}
void set_menu_position4_444(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 545; *y = 470;return;}
void set_menu_position4_614(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 545; *y = 470;return;}
void set_menu_position4_804(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 540; *y = 470;return;}
void set_menu_position4_824(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 540; *y = 470;return;}
void set_menu_position4_904(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 470;return;}
void set_menu_position4_924(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 572; *y = 470;return;}

void set_menu_position5(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 500;return;}
void set_menu_position5_145(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 570; *y = 558;return;}
void set_menu_position5_215(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 532; *y = 395;return;}
void set_menu_position5_305(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 433; *y = 525;return;}
void set_menu_position5_445(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer status_icon){*x = 575; *y = 555;return;}

/* 设置控件的背景图片 */
void update_widget_bg(GtkWidget *widget, const gchar *img_file)
{      
	GtkStyle *style;    
	GdkPixbuf *pixbuf;      
	GdkPixmap *pixmap;    
	gint width, height;   

	pixbuf = gdk_pixbuf_new_from_file(img_file, NULL);   
	width = gdk_pixbuf_get_width(pixbuf);  
	height = gdk_pixbuf_get_height(pixbuf);   
	pixmap = gdk_pixmap_new(NULL, width, height, 24);  
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, NULL, 0);
	style = gtk_style_copy(GTK_WIDGET (widget)->style);   

	if (style->bg_pixmap[GTK_STATE_NORMAL])       
		g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);   

	style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);   
/**
	style->bg_pixmap[GTK_STATE_ACTIVE] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_PRELIGHT] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_SELECTED] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_INSENSITIVE] = g_object_ref(pixmap);
**/
	gtk_widget_set_style(GTK_WIDGET (widget), style);
	g_object_unref(style);
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

	return ;
}

/*
 * 处理 三级菜单弹出菜单的画图
 * 
 *
 *
 */

static void draw3_pop (void (*fun)(GtkMenuItem*, gpointer),
		void (* set_menu_position)(GtkMenu *, gint *, gint *, gboolean *, gpointer ),
		const gchar *cur_value,
		const gchar *content[], guint qty, gint pos )
{

	gint i, x, y, z;
	GtkMenu *menu;
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	for (i = 0; i < 30; i++) 
	{
		if (pp->menu_item3[i]) 
		{
			gtk_widget_destroy(pp->menu_item3[i]);
			pp->menu_item3[i] = NULL;
		}
		else 
			break;;
	}

	for (i = 0; i < qty; i++ )
	{
		pp->menu_item3[i] = gtk_menu_item_new_with_label(content[i]);
		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu3), pp->menu_item3[i]);
		//		g_signal_connect_swapped (menu_items[i], "activate", 
		//				G_CALLBACK(menuitem_respnse), (gpointer) g_strdu(buf));
		gtk_widget_show(pp->menu_item3[i]);
	}

	menu = GTK_MENU (pp->menu3);
	gtk_menu_popup (menu, NULL, NULL, 
			(GtkMenuPositionFunc)set_menu_position,
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

	return ;
}

draw3_popdwon (gchar *cur_value, guint pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
	}
	else
	{
	}
	gtk_label_set_text (GTK_LABEL (pp->data3[z]), cur_value);
	if (str)
		g_free(str);
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	/*						gtk_widget_grab_focus (pp->button);*/
}

/*
 * 处理 三级菜单按下状态的画图  
 * fun   为回调函数 
 * cur_value 为菜单当前数值
 * lower 为菜单最小值
 * upper 为菜单最大值
 * step  为菜单步进
 * digit 为保留小数点数
 * pos 为第几个3级菜单
 */

static void draw3_pressed(void (*fun)(GtkSpinButton*, gpointer),const gchar *unit, 
		gfloat cur_value, gfloat lower, gfloat upper, gfloat step, guint digit, gpointer p, guint pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;
	GtkAdjustment *adj;		/*  */

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;
	/*	gtk_widget_destroy (pp->vscale);*/

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
		case 4:
			str = g_strdup_printf ("%s\n(%s) Δ%0.4f", con2_p[x][y][z], unit, step);			break;
		default:break;
	}
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	//	gtk_widget_modify_bg (pp->eventbox30[z], GTK_STATE_NORMAL, &color_button0);
	update_widget_bg(pp->eventbox30[z], backpic[6]);
	widget_window_class->key_press_event = window_keypress_event_orig;
	g_signal_connect (G_OBJECT(pp->sbutton[z]), "value-changed", G_CALLBACK(fun), (gpointer) (pp));

	/* 设置值的范围 */
	adj = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (pp->sbutton[z]));
	gtk_adjustment_configure (adj, cur_value, lower, upper, step , 10.0, 0.0);
	gtk_spin_button_set_digits (GTK_SPIN_BUTTON (pp->sbutton[z]), digit);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_hide (pp->eventbox31[z]);
	gtk_widget_show (pp->sbutton[z]);
	gtk_widget_grab_focus (pp->sbutton[z]);

	/* 设置scale */
	gtk_range_set_adjustment (GTK_RANGE (pp->vscale), adj);
	gtk_widget_set_size_request (GTK_WIDGET(pp->vscale), 30, 365);
	gtk_range_set_inverted (GTK_RANGE (pp->vscale), TRUE);
	gtk_scale_set_draw_value (GTK_SCALE (pp->vscale), FALSE);
//	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->vscale, TRUE, TRUE, 0);

	gtk_widget_show (pp->vscalebox);

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

static void draw3_stop(gfloat cur_value, const gchar *unit,  guint digit, guint pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	str = g_strdup_printf ("%s\n(%s)", con2_p[x][y][z], unit);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
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
		case 4:
			str = g_strdup_printf ("%0.4f", cur_value);			break;
		default:break;
	}
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
	gtk_widget_hide (pp->vscalebox);
	/*						gtk_widget_grab_focus (pp->button);*/
}

static void draw3_onoffstop(guint pos)               /* button 为 on/off 时的颜色变化*/
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_STOP))
	{
	}
	else
	{
	}

	if (str)
		g_free(str);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
}

static void draw3_onoffpressed(guint pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;
	gtk_widget_destroy (pp->vscale);

	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_PRESSED))
	{
	}
	else
	{
	}

	if (str)
		g_free(str);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
}

/* 三级菜单第一个 */
void draw3_data0(gpointer p) 
{
	gchar temp[52];
	gchar *str;
	gfloat tmpf;/**/

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;


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
					{
						cur_value = pp->p_config->gain;
						lower = 0.0;
						upper = 74.0;
						step = tmpf;
						digit = 1;
						pos = 0;
						unit = UNIT_DB;
						draw3_pressed (data_100, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->gain;
						digit = 1;
						pos = 0;
						unit = UNIT_DB;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 1: /*发射 Pulser 110 */
					/* 当前步进 */
					switch (pp->p_tmp_config->pulser_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = pp->p_config->pulser;
						lower = 1.0;
						upper = 97.0;
						step = tmpf;
						digit = 0;
						pos = 0;
						unit = UNIT_TO;
						draw3_pressed (data_110, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->pulser;
						digit = 0;
						pos = 0;
						unit = UNIT_TO;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
					g_sprintf (temp,"%s", con2_p[1][3][0]);
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
				case 0:/*Gate 200 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_200, "A", gate, 3, 0);
					}
					else 
						draw3_popdwon ("A", 0);

					break;


				case 1:/*Alarm  210 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_210, "1", gate_alarm, 16, 0);
					}
					else 
						draw3_popdwon ("1", 0);
					break;

				case 2:/*Output  220 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_220, "Output 1", output, 5, 0);
					}
					else 
						draw3_popdwon ("Output 1", 0);
					break;


				case 3:/*Sizing Curves -> Setup  230 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_230, "Setup", curves_mode, 2, 0);
					}
					else 
						draw3_popdwon ("Setup", 0);
					break;

				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> List  300 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_300, "1", list, 2, 0);
					}
					else 
						draw3_popdwon ("1", 0);
					break;

				case 1:/*Measurements -> Cursors -> Selection 310 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_310, "Data", selection, 9, 0);
					}
					else 
						draw3_popdwon ("Data", 0);
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
				case 3:/*Measurements -> Thickness -> Source  330 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_330, "A^", source, 9, 0);
					}
					else 
						draw3_popdwon ("A^", 0);
					break;
					;
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
				case 0:/*Display -> Selection -> Display  400 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_400, "A", display, 11, 0);
					}
					else 
						draw3_popdwon ("A", 0);
					break;


				case 1:/*Display -> Overlay -> UT Unit  410 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_410, "True Depth", utunit, 3, 0);
					}
					else 
						draw3_popdwon ("True Depth", 0);
					break;


				case 2:/*Display -> Zoom -> Display 420 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_420, "Off", zoom_display, 6, 0);
					}
					else 
						draw3_popdwon ("Off", 0);
					break;


				case 3:/*Display -> Color -> Select  430 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_430, "Amplitude", select1, 3, 0);
					}
					else 
						draw3_popdwon ("Amplitude", 0);
					break;


				case 4:/*Display -> Properties -> Scan 440 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_440, "A-Scan", scan, 6, 0);
					}
					else 
						draw3_popdwon ("A-Scan", 0);
					break;
					;
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
				case 1:/*probe/part -> position -> Scan Offset 510 */
					/* 当前步进 */
					switch (pp->p_tmp_config->scanoffset_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = pp->p_config->scan_offset/10.0;
						lower = -100000.0;
						upper = 9999999.0;
						step = tmpf;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_pressed (data_510, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->scan_offset/10.0;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 3:/*Probe/Part -> Parts -> Geometry  530 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_530, "Plate", geometry, 3, 0);
					}
					else 
						draw3_popdwon ("Plate", 0);
					break;


				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:/*Focal Law -> configuration -> Law Config.  600 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_600, "Linear", law_config, 4, 0);
					}
					else 
						draw3_popdwon ("Linear", 0);
					break;

				case 1:/*Focal Law -> aperture -> element qty.  610 */
					/* 当前步进 */
					switch (pp->p_tmp_config->element_qty_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = pp->p_config->element_qty;
						lower = 1.0;
						upper = 32.0;
						step = tmpf;
						digit = 0;
						pos = 0;
						unit = UNIT_NONE;
						draw3_pressed (data_610, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->element_qty;
						digit = 0;
						pos = 0;
						unit = UNIT_NONE;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 2:/*Focal Law -> beam -> Min.Angle 620 */
					/* 当前步进 */
					switch (pp->p_tmp_config->min_angle_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = pp->p_config->min_angle/10.0;
						lower = -89.9;
						upper = 89.9;
						step = tmpf;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_pressed (data_620, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->min_angle/10.0;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;


				case 3:/*Focal Law -> laws -> auto program  630 */
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
				case 0:/*Scan -> Encoder -> Encoder 700 */
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
				case 1:/*Scan -> Inspection -> type  710 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_710, "One-Line Scan", inspection_type, 3, 0);
					}
					else 
						draw3_popdwon ("One-Line Scan", 0);
					break;


				case 2:/*Scan -> Area -> scan start 720 */
					/* 当前步进 */
					switch (pp->p_tmp_config->scan_start_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = pp->p_config->scan_start;
						lower = -99999.0;
						upper = 99999.0;
						step = tmpf;
						digit = 0;
						pos = 0;
						unit = UNIT_MM;
						draw3_pressed (data_720, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->scan_start;
						digit = 0;
						pos = 0;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;


				case 3:/*Scan -> start -> start mode  730 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_730, "Reset All", start_mode, 3, 0);
					}
					else 
						draw3_popdwon ("Reset All", 0);
					break;


				case 4:/*Scan -> data -> storage  740 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_740, "Last", storage, 4, 0);
					}
					else 
						draw3_popdwon ("Last", 0);
					break;


				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/*File -> File -> Storage  800 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_800, "Storage Card", file_storage, 2, 0);
					}
					else 
						draw3_popdwon ("Storage Card", 0);
					break;


				case 1:/*File -> report -> template 810 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_810, "Complete", template, 1, 0);
					}
					else 
						draw3_popdwon ("Complete", 0);
					break;

				case 2:/*File -> format -> user field  820 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);

					if (!pp->p_config->format_userfield)
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[0]), "On");
					}
					else
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[0]), "Off");
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_onoffpressed (0);
					}
					else 
						draw3_onoffstop (0);
					break;

				case 3:/*File -> user field -> select  830 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_830, "1", file_select, 10, 0);
					}
					else 
						draw3_popdwon ("1", 0);
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
				case 0:/*Preferences -> pref.-> units 900 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_900, "Millimeters", pref_units, 2, 0);
					}
					else 
						draw3_popdwon ("Millimeters", 0);
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
				case 3:/*Preferences -> options -> mouse  930 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, set_menu_position0_930, "Mouse", mouse, 3, 0);
					}
					else 
						draw3_popdwon ("Mouse", 0);
					break;

				case 4:/*Preferences -> network -> DHCP  940*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
					gtk_widget_modify_bg (pp->eventbox30[0], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[0]), "192.168.1.2");
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

void draw3_data1(gpointer p) 
{
	gchar temp[52];
	gfloat tmpf;
	gchar *str;
	gint x, y;
	GtkAdjustment *adj;

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

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
			switch (pp->pos1[1])
			{
				case 0: /* start 扫描延时 101 */
					/* 当前步进 */
					switch (pp->p_tmp_config->start_reg)
					{
						case 0:	tmpf = pp->p_config->range / 320.0; break;
						case 1:	tmpf = pp->p_config->range / 20.0 ; break;
						case 2:	tmpf = pp->p_config->range / 10.0 ; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if (pp->p_config->ut_unit == 0)
						{
							if (pp->p_config->unit == 0)
							{
								cur_value = pp->p_config->start * (pp->p_config->part.Velocity / 2000.0);
								lower = 0.0 * (pp->p_config->part.Velocity / 2000.0);
								upper =	(9900.8 - pp->p_config->range) * (pp->p_config->part.Velocity / 2000.0);
								step = tmpf * (pp->p_config->part.Velocity / 2000.0);
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = pp->p_config->start * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								lower =	0.0 * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								upper =	(9900.8 - pp->p_config->range) * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								step = tmpf * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = pp->p_config->start;
							lower =	0.0;
							upper =	(9900.8 - pp->p_config->range);
							step = tmpf;
							digit = 2;
							pos = 1;
							unit = UNIT_US;
						}
						draw3_pressed (data_101, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else
					{
						if (pp->p_config->ut_unit == 0)
						{
							if (pp->p_config->unit == 0)
							{
								cur_value = pp->p_config->start * pp->p_config->part.Velocity / 2000.0;
								unit = UNIT_MM;
								pos = 1;
								digit = 2;
							}
							else
							{
								cur_value = pp->p_config->start * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								unit = UNIT_INCH;
								pos = 1;
								digit = 2;
							}
						}
						else
						{
							cur_value = pp->p_config->start;
							unit = UNIT_US;
							pos = 1;
							digit = 2;
						}
						draw3_stop (cur_value , units[unit], digit, pos);
					}
					break;
				case 1: /* 收发模式 Tx/Rx Mode 111 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1, "PE", tx_rx_mode, 3, 1);
					}
					else 
						draw3_popdwon ("PE", 1);

					break;
				case 2: /* UT Settings -> Receiver -> Filter  121 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_121, "None", filter, 15, 1);
					}
					else 
						draw3_popdwon ("None", 1);

					break;
				case 3:/*index Offset*/
					g_sprintf (temp,"%s", con2_p[1][3][1]);
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "(mm)");

					gtk_widget_modify_bg (pp->eventbox31[10], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					break;
				default:break;
			}
			break;

		case 2:
			switch (pp->pos1[2])
			{
				case 0:/*Gate->Parameters  201 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_201, "Position", parameters, 2, 1);
					}
					else 
						draw3_popdwon ("Position", 1);

					break;

				case 1:/*Group A 211 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_211, "1", groupA, 3, 1);
					}
					else 
						draw3_popdwon ("1", 1);
					break;

				case 2:/*Alarm #  221 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_221, "None", gate_alarm, 18, 1);
					}
					else 
						draw3_popdwon ("None", 1);

					break;


				case 3:/*Sizing Curves -> Curve  231 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_231, "TCG", curve, 4, 1);
					}
					else 
						draw3_popdwon ("TCG", 1);
					break;

				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (pp->pos1[3])
			{
				case 0:/*Measurements -> Reading -> Group 301 */
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
				case 1:/*Measurements -> Cursors -> Angle 311 */
					/* 当前步进 */
					switch (pp->p_tmp_config->VPA_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
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
						draw3_pressed (data_311, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->VPA;
						digit = 0;
						pos = 1;
						unit = UNIT_NONE;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 3:/*Measurements -> Thickness -> Min  331 */
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
						cur_value = pp->p_config->min_thickness/100.0;
						lower = 0.05;
						upper = 49.99;
						step = tmpf;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
						draw3_pressed (data_331, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->min_thickness/100.0;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;


				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 0:/*Display -> Selection -> group  401 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_401, "Current", group, 2, 1);
					}
					else 
						draw3_popdwon ("Current", 1);
					break;

				case 1:/*Display -> Overlay -> grid  411 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_411, "Off", grid, 6, 1);
					}
					else 
						draw3_popdwon ("Off", 1);
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
				case 3:/*Display -> Color -> start 431 */
					/* 当前步进 */
					switch (pp->p_tmp_config->color_start_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->color_start;
						lower = 0.0;
						upper = 99.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						unit = UNIT_BFH;
						draw3_pressed (data_431, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->color_start;
						digit = 0;
						pos = 1;
						unit = UNIT_BFH;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

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
				case 4:/*Display -> Color -> Color  441 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_441, "Red", color, 6, 1);
					}
					else 
						draw3_popdwon ("Red", 1);
					break;

				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Group Mode  501 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_501, "PA", group_mode, 2, 1);
					}
					else 
						draw3_popdwon ("PA", 1);
					break;

				case 1:/*probe/part -> position -> Index Offset 511 */
					/* 当前步进 */
					switch (pp->p_tmp_config->indexoffset_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 5.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->index_offset/10.0;
						lower = -100000000.0;
						upper = 9999999999.0;
						step = tmpf;
						digit = 1;
						pos = 1;
						unit = UNIT_MM;
						draw3_pressed (data_511, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->index_offset/10.0;
						digit = 1;
						pos = 1;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 2:/*probe/part -> characterize -> gain 521 */
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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->gain/10.0;
						lower = 0.0;
						upper = 74.0;
						step = tmpf;
						digit = 1;
						pos = 1;
						unit = UNIT_MM;
						draw3_pressed (data_521, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->gain/10.0;
						digit = 1;
						pos = 1;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 3:/*probe/part -> parts -> thickness 531 */
					/* 当前步进 */
					switch (pp->p_tmp_config->part_thickness_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->part_thickness/100.0;
						lower = 0.05;
						upper = 1000000.0;
						step = tmpf;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
						draw3_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->part_thickness/100.0;
						digit = 2;
						pos = 1;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 0:/*Focal Law -> configuration -> connection P 601 */
					/* 当前步进 */
					switch (pp->p_tmp_config->connection_P_reg)
					{
						case 0:	tmpf = 0.0; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->connection_P;
						lower = 1.0;
						upper = 113.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						unit = UNIT_TO1;
						draw3_pressed (data_601, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->connection_P;
						digit = 0;
						pos = 1;
						unit = UNIT_TO1;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;


				case 1:/*Focal Law -> aperture -> first element 611 */
					/* 当前步进 */
					switch (pp->p_tmp_config->first_element_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->first_element;
						lower = 1.0;
						upper = 97.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						unit = UNIT_NONE;
						draw3_pressed (data_611, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->first_element;
						digit = 0;
						pos = 1;
						unit = UNIT_NONE;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
			switch (pp->pos1[7])
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
				case 1:/*Scan -> Inspection -> scan  711 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_711, "Time", inspection_scan, 3, 1);
					}
					else 
						draw3_popdwon ("Time", 1);
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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						cur_value = pp->p_config->scan_end;
						lower = -100000.0;
						upper = 100000.0;
						step = tmpf;
						digit = 0;
						pos = 1;
						unit = UNIT_MM;
						draw3_pressed (data_721, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->scan_end;
						digit = 0;
						pos = 1;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 4:/*Scan -> data -> inspec.data  741 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, set_menu_position1_741, "All A & C scans", inspec_data, 2, 1);
					}
					else 
						draw3_popdwon ("All A & C scans", 1);
					break;


				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
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

				case 2:/*File -> format -> probe  821 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

					if (!pp->p_config->format_probe)
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), "On");
					}
					else
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Off");
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_onoffpressed (1);
					}
					else 
						draw3_onoffstop (1);
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
						draw3_pressed (data_901, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->bright;
						digit = 0;
						pos = 1;
						unit = UNIT_BFH;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 4:/*Preferences -> Network -> IP Address  941*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][4][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), "255.255.255.0");
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

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:/*Wizard -> Calibration -> Type  022 */

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					   {
						   draw3_pop (NULL, set_menu_position2_022, "Ultrasound", type, 3, 2);
					   }
					   else 
						   draw3_popdwon ("Ultrasound", 2);
					   break;

				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0: /* range范围 101 */
					/*当前步进*/
					switch (p->p_tmp_config->range_reg)
					{
						case 0:	tmpf = 3.2; break;
						case 1:	tmpf = 16.0; break;
						case 2:	tmpf = 32.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if (pp->p_config->ut_unit == 0)
						{
							if (pp->p_config->unit == 0)
							{
								cur_value = pp->p_config->range * (pp->p_config->part.Velocity / 2000.0);
								lower = 3.2 * (pp->p_config->part.Velocity / 2000.0);
								upper = ((9900.8 - pp->p_config->start) > 6400.0 ? 6400.0 : (9900.8 - pp->p_config->start)) *
									(pp->p_config->part.Velocity / 2000.0);
								step = tmpf * (pp->p_config->part.Velocity / 2000.0);
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = pp->p_config->range * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								lower =	3.2 * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								upper =	((9900.8 - pp->p_config->start) > 6400.0 ? 6400.0 : (9900.8 - pp->p_config->start)) *
									0.03937 * pp->p_config->part.Velocity / 2000.0;
								step = tmpf * 0.03937 * pp->p_config->part.Velocity / 2000.0;
								digit = 2;
								pos = 2;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = pp->p_config->range;
							lower =	3.2;
							upper =	((9900.8 - pp->p_config->start) > 6400.0 ? 6400.0 : (9900.8 - pp->p_config->start));
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_US;
						}
						draw3_pressed (data_102, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else
					{
						if (pp->p_config->ut_unit == 0)
						{
							if (pp->p_config->unit == 0)
							{
								cur_value = pp->p_config->range * (pp->p_config->part.Velocity / 2000.0);
								unit = UNIT_MM;
								pos = 2;
								digit = 2;
							}
							else
							{
								cur_value = pp->p_config->range * 0.03937 * (pp->p_config->part.Velocity / 2000.0);
								unit = UNIT_INCH;
								pos = 2;
								digit = 2;
							}
						}
						else
						{
							cur_value = pp->p_config->range;
							unit = UNIT_US;
							pos = 2;
							digit = 2;
						}
						draw3_stop (cur_value , units[unit], digit, pos);
					}
					break;
				case 1: /* Freq频带(Mhz)  112 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2, "5", freq, 12, 2);
					}
					else 
						draw3_popdwon ("5", 2);
					break;

				case 2:/*Rectifier 122 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2_122, "FW", rectifier, 4, 2);
					}
					else 
						draw3_popdwon ("FW", 2);
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
				case 4:/*dB Ref.  142 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][4][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);

					if (!p->p_config->db_ref)
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[2]), "On");
					}
					else
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Off");
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_onoffpressed (2);
					}
					else 
						draw3_onoffstop (2);
					break;


				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*Gate->Start 202 */
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_start_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.10; break;
						case 2:	tmpf = 1.00; break;
						case 3:	tmpf = 10.00; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->agate_start/100.0;
						lower = -3000.0;
						upper = 27000.0;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->agate_start/100.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 1:/*Condition  212 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2_212, "None", conditionA, 9, 2);
					}
					else 
						draw3_popdwon ("None", 2);
					break;



				case 2:/*Output->Count 222 */
					/* 当前步进 */
					switch (pp->p_tmp_config->count_reg)
					{

						case 0:	tmpf = 1.00; break;
						case 1:	tmpf = 10.00; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->count;
						lower = 1.0;
						upper = 100.0;
						step = tmpf;
						digit = 0;
						pos = 2;
						unit = UNIT_NONE;
						draw3_pressed (data_222, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->count;
						digit = 0;
						pos = 2;
						unit = UNIT_NONE;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 0:/*Measurements -> Reading -> Field 1 302 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2_302, "A%", field1, 3, 2);
					}
					else 
						draw3_popdwon ("A%", 2);
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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->cursors_scan;
						lower = 1.0;
						upper = 100.0;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_pressed (data_222, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->cursors_scan;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 3:/*Measurements -> Thickness -> Max 332 */
					/* 当前步进 */
					switch (pp->p_tmp_config->max_thickness_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->max_thickness/100.0;
						lower = 0.06;
						upper = 99999999.00;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_pressed (data_332, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->max_thickness/100.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;


				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{
				case 0:break;
				case 1:/*Display -> Overlay -> sizing curves  412 */

					   /* 格式化字符串 */
					   g_sprintf (temp,"%s", con2_p[4][1][2]);

					   /* 设置label */
					   gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);

					   if (!p->p_config->sizing_curves)
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[2]), "On");
					   }
					   else
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Off");
					   }

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					   {
						   draw3_onoffpressed (2);
					   }
					   else 
						   draw3_onoffstop (2);
					   break;


				case 2:break;
				case 3:/*Display -> Color -> end 432 */
					   /* 当前步进 */
					   switch (pp->p_tmp_config->color_end_reg)
					   {
						   case 0:	tmpf = 1.0; break;
						   case 1:	tmpf = 10.0; break;
						   default:break;
					   }
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					   {
						   cur_value = pp->p_config->color_end;
						   lower = 1.0;
						   upper = 100.0;
						   step = tmpf;
						   digit = 0;
						   pos = 2;
						   unit = UNIT_BFH;
						   draw3_pressed (data_432, units[unit], cur_value , lower, upper, step, digit, p, pos);
					   }
					   else 
					   {
						   cur_value = pp->p_config->color_end;
						   digit = 0;
						   pos = 2;
						   unit = UNIT_BFH;
						   draw3_stop (cur_value, units[unit], digit, pos);
					   }
					   break;

				case 4:/*Display -> Properties -> Envelope 442 */
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					   {
						   draw3_pop (NULL, set_menu_position2_442, "None", envelope, 2, 2);
					   }
					   else 
						   draw3_popdwon ("None", 2);
					   break;


				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Select  502 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2_502, "Select Tx/Rx", probe_select, 2, 2);
					}
					else 
						draw3_popdwon ("Select Tx/Rx", 2);
					break;


				case 1:/*Probe/Part -> position -> skew  512 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2_512, "0.0", probe_skew, 5, 2);
					}
					else 
						draw3_popdwon ("0.0", 2);
					break;

				case 2:/*Probe/Part -> Characterize -> start 522 */
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_start_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->agate_start/100.0;
						lower = 0.00;
						upper = 29127.88;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_pressed (data_522, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->agate_start/100.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 1:/*Focal Law -> aperture -> last element 612 */
					/* 当前步进 */
					switch (pp->p_tmp_config->last_element_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->last_element;
						lower = 32.0;
						upper = 128.0;
						step = tmpf;
						digit = 0;
						pos = 2;
						unit = UNIT_NONE;
						draw3_pressed (data_612, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->last_element;
						digit = 0;
						pos = 2;
						unit = UNIT_NONE;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 2:/*Scan -> Area -> scan resolution 722 */
					/* 当前步进 */
					switch (pp->p_tmp_config->scan_resolution_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->scan_resolution;
						lower = 0.01;
						upper = 36.6;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_pressed (data_722, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->scan_resolution;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 1:/*File -> report -> paper size  812 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2_812, "A4", paper_size, 4, 2);
					}
					else 
						draw3_popdwon ("A4", 2);
					break;


				case 2:/*File -> format -> setup  822 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][2]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);

					if (!pp->p_config->format_setup)
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[2]), "On");
					}
					else
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[2]), "Off");
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_onoffpressed (2);
					}
					else 
						draw3_onoffstop (2);
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
				case 1:/*Preferences -> system -> select key  912 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						draw3_pop (NULL, set_menu_position2_912, "F2", select_key, 9, 2);
					}
					else 
						draw3_popdwon ("F2", 2);
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
# if 0
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
# endif
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

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

	switch (pp->pos) 
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:/*Wizard -> Calibration -> Mode  023 */

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					   {
						   draw3_pop (NULL, set_menu_position3_023, "Sensitivity", calibration_mode, 4, 3);
					   }
					   else 
						   draw3_popdwon ("Sensitivity", 3);
					   break;

				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 0:/* wedge delay 103 */
					/* 当前步进 */
					switch (pp->p_tmp_config->wedge_delay_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = pp->p_config->wedge_delay;
						lower = 0.0;
						upper = 1000.0;
						step = tmpf;
						digit = 2;
						pos = 3;
						unit = UNIT_US;
						draw3_pressed (data_103, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->wedge_delay;
						digit = 2;
						pos = 3;
						unit = UNIT_US;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;
				case 1: /* 电压高低 功率?  113 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop (NULL, set_menu_position3, "45", voltage, 2, 3);
					}
					else 
						draw3_popdwon ("45", 3);

					break;


				case 2:/*Video Filter  123 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[1][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);

					if (!p->p_config->video_filter)
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[3]), "On");
					}
					else
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Off");
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_onoffpressed (3);
					}
					else 
						draw3_onoffstop (3);
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

				case 4:/*Points Qty.  143*/

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop (NULL, set_menu_position3_143, "Auto", points_qty, 5, 3);
					}
					else 
						draw3_popdwon ("Auto", 3);

					break;

				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*Gate->Width 203 */
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_width_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = pp->p_config->agate_width/100.0;
						lower = 0.12;
						upper = 29000.0;
						step = tmpf;
						digit = 2;
						pos = 3;
						unit = UNIT_MM;
						draw3_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->agate_width/100.0;
						digit = 2;
						pos = 3;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 1:/*Operator  213*/

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop (NULL, set_menu_position3_213, "AND", operator, 2, 3);
					}
					else 
						draw3_popdwon ("AND", 3);

					break;


				case 2:/*Sound  223 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop (NULL, set_menu_position3_223, "Off", sound, 5, 3);
					}
					else 
						draw3_popdwon ("Off", 3);

					break;

			}
		case 3:
			switch (p->pos1[3]) {
				case 0:/*Measurements -> reading -> field2  303*/

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop (NULL, set_menu_position3_303, "A%", field1, 3, 3);
					}
					else 
						draw3_popdwon ("A%", 3);
					break;

				case 1:/*Measurements -> Cursors -> Index 313 */
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
						draw3_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->cursors_index;
						digit = 1;
						pos = 3;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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

						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = pp->p_config->echo_qty;
						lower = 1.0;
						upper = 10.0;
						step = tmpf;
						digit = 0;
						pos = 3;
						unit = UNIT_NONE;
						draw3_pressed (data_333, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->echo_qty;
						digit = 0;
						pos = 3;
						unit = UNIT_NONE;
						draw3_stop (cur_value, units[unit], digit, pos);
					}

					break;


				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[4])
			{
				case 0:break;
				case 1:/*Display -> Overlay -> gate  413 */
					   /* 格式化字符串 */
					   g_sprintf (temp,"%s", con2_p[4][1][3]);

					   /* 设置label */
					   gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);

					   if (!p->p_config->overlay_gate)
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[3]), "On");
					   }
					   else
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Off");
					   }

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					   {
						   draw3_onoffpressed (3);
					   }
					   else 
						   draw3_onoffstop (3);
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
				case 4:/*Display -> Properties -> Source  443 */
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					   {
						   draw3_pop (NULL, set_menu_position3_443, "Normal", properties_source, 4, 3);
					   }
					   else 
						   draw3_popdwon ("Normal", 3);
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
				case 2:/*Probe/Part -> Characterize -> width 522 */
					   /* 当前步进 */
					   switch (pp->p_tmp_config->agate_width_reg)
					   {
						   case 0:	tmpf = 0.01; break;
						   case 1:	tmpf = 0.1; break;
						   case 2:	tmpf = 1.0; break;
						   case 3:	tmpf = 10.0; break;						
						   default:break;
					   }
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					   {
						   cur_value = pp->p_config->agate_width/100.0;
						   lower = 0.12;
						   upper = 29.98;
						   step = tmpf;
						   digit = 2;
						   pos = 3;
						   unit = UNIT_MM;
						   draw3_pressed (data_522, units[unit], cur_value , lower, upper, step, digit, p, pos);
					   }
					   else 
					   {
						   cur_value = pp->p_config->agate_width/100.0;
						   digit = 2;
						   pos = 3;
						   unit = UNIT_MM;
						   draw3_stop (cur_value, units[unit], digit, pos);
					   }
					   break;


				case 3:/*Probe/Part -> parts -> material  533 */

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					   {
						   draw3_pop (NULL, set_menu_position3_533, "STEEL.MILD", material, 7, 3);
					   }
					   else 
						   draw3_popdwon ("STEEL.MILD", 3);
					   break;

				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 0:break;
				case 1:/*Focal Law -> aperture -> element step 613*/
					   /* 当前步进 */
					   switch (pp->p_tmp_config->element_step_reg)
					   {
						   case 0:	tmpf = 1.0; break;
						   case 1:	tmpf = 10.0; break;
						   case 2:	tmpf = 100.0; break;
						   default:break;
					   }

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					   {
						   cur_value = pp->p_config->element_step;
						   lower = 0.5;
						   upper = 96.0;
						   step = tmpf;
						   digit = 1;
						   pos = 3;
						   unit = UNIT_NONE;
						   draw3_pressed (data_613, units[unit], cur_value , lower, upper, step, digit, p, pos);
					   }
					   else 
					   {
						   cur_value = pp->p_config->element_step;
						   digit = 1;
						   pos = 3;
						   unit = UNIT_NONE;
						   draw3_stop (cur_value, units[unit], digit, pos);
					   }
					   break;


				case 2:/*Focal law -> Beam -> focus depth 623 */
					   /* 当前步进 */
					   switch (pp->p_tmp_config->focus_depth_reg)
					   {
						   case 0:	tmpf = 0.1; break;
						   case 1:	tmpf = 1.0; break;
						   case 2:	tmpf = 10.0; break;
						   default:break;
					   }
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					   {
						   cur_value = pp->p_config->focus_depth/10.0;
						   lower = 0.10;
						   upper = 100.00;
						   step = tmpf;
						   digit = 1;
						   pos = 3;
						   unit = UNIT_MM;
						   draw3_pressed (data_623, units[unit], cur_value , lower, upper, step, digit, p, pos);
					   }
					   else 
					   {
						   cur_value = pp->p_config->focus_depth/10.0;
						   digit = 1;
						   pos = 3;
						   unit = UNIT_MM;
						   draw3_stop (cur_value, units[unit], digit, pos);
					   }
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
				case 1:/*Scan -> Inspection -> scan speed 713 */
					/* 当前步进 */
					switch (pp->p_tmp_config->scan_speed_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = pp->p_config->scan_speed/10.0;
						lower = 0.0;
						upper = 1000.0;
						step = tmpf;
						digit = 1;
						pos = 3;
						unit = UNIT_MM_S;
						draw3_pressed (data_713, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->scan_speed/10.0;
						digit = 1;
						pos = 3;
						unit = UNIT_MM_S;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;


				case 2:/*Scan -> area -> index start 723 */
					/* 当前步进 */
					switch (pp->p_tmp_config->index_start_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						cur_value = pp->p_config->index_start;
						lower = -99999.0;
						upper = 99999.0;
						step = tmpf;
						digit = 0;
						pos = 3;
						unit = UNIT_MM;
						draw3_pressed (data_723, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->index_start;
						digit = 0;
						pos = 3;
						unit = UNIT_MM;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 2:/*File -> format -> Note  823 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][3]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);

					if (!pp->p_config->format_note)
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[3]), "On");
					}
					else
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[3]), "Off");
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_onoffpressed (3);
					}
					else 
						draw3_onoffstop (3);
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
				case 0:/*preferences -> pref. -> scheme  903 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop (NULL, set_menu_position3_903, "Indoor", scheme, 2, 3);
					}
					else 
						draw3_popdwon ("Indoor", 3);
					break;

				case 1:/*Preferences -> system -> assign key  913 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						draw3_pop (NULL, set_menu_position3_913, "Gain", assign_key, 16, 3);
					}
					else 
						draw3_popdwon ("Gain", 3);
					break;

				case 2:/*Preferences -> service -> Import/Export  923 */
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
# if 0
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
# endif
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

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

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
				case 0:/* velocity 声速 104 */
					/* 当前步进 */
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
						if (pp->p_config->unit == 0)
						{
							cur_value = pp->p_config->part.Velocity;
							lower = 635.0;
							upper = 15240.0;
							step = tmpf;
							digit = 1;
							unit = UNIT_M_S;
						}
						else
						{
							cur_value = pp->p_config->part.Velocity;
							lower = 0.025;
							upper = 0.6;
							step = tmpf / 10000.0;
							digit = 4;
							unit = UNIT_IN_US;
						}
						pos = 4;
						draw3_pressed (data_104, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						if (pp->p_config->unit == 0)
						{
							cur_value = pp->p_config->part.Velocity;
							digit = 1;
							unit = UNIT_M_S;
						}
						else
						{
							cur_value = pp->p_config->wedge_delay * 0.0000394;
							digit = 4;
							unit = UNIT_IN_US;
						}
						pos = 4;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;
				case 1: /* 脉冲宽度 pulser width  114 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						draw3_pop (NULL, set_menu_position4, "Auto", PW, 3, 4);
					}
					else 
						draw3_popdwon ("Auto", 4);

					break;

				case 2:/*Averaging 124 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						draw3_pop (NULL, set_menu_position4_124, "2", averaging, 5, 4);
					}
					else 
						draw3_popdwon ("2", 4);

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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						cur_value = pp->p_config->beam_delay/100.0;
						lower = 1.0;
						upper = 1000.0;
						step = tmpf;
						digit = 2;
						pos = 4;
						unit = UNIT_US;
						draw3_pressed (data_134, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->beam_delay/100.0;
						digit = 2;
						pos = 4;
						unit = UNIT_US;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 0:/*Gate->Threshold  204 */
					/* 当前步进 */
					switch (pp->p_tmp_config->agate_height_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						cur_value = pp->p_config->agate_height;
						lower = 0.0;
						upper = 98.0;
						step = tmpf;
						digit = 0;
						pos = 4;
						unit = UNIT_BFH;
						draw3_pressed (data_204, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->agate_height;
						digit = 0;
						pos = 4;
						unit = UNIT_BFH;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 1:/*Group B   214 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						draw3_pop (NULL, set_menu_position4_214, "2", groupB, 3, 4);
					}
					else 
						draw3_popdwon ("2", 4);

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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						cur_value = pp->p_config->active_delay/100.0;
						lower = 0.0;
						upper = 5000.0;
						step = tmpf;
						digit = 2;
						pos = 4;
						unit = UNIT_MS;
						draw3_pressed (data_224, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->active_delay/100.0;
						digit = 2;
						pos = 4;
						unit = UNIT_MS;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
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
				case 0:/*Measurements -> Reading -> Field 3  304 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						draw3_pop (NULL, set_menu_position4_304, "A%", field1, 3, 4);
					}
					else 
						draw3_popdwon ("A%", 4);
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
				case 1:/*Display -> Overlay -> cursor  414 */
					   /* 格式化字符串 */
					   g_sprintf (temp,"%s", con2_p[4][1][4]);

					   /* 设置label */
					   gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);

					   if (!p->p_config->overlay_cursor)
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[4]), "On");
					   }
					   else
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Off");
					   }

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					   {
						   draw3_onoffpressed (4);
					   }
					   else 
						   draw3_onoffstop (4);
					   break;


				case 2:break;
				case 3:break;
				case 4:/*Display -> properties -> appearence  444 */
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					   {
						   draw3_pop (NULL, set_menu_position4_444, "Hollow", appearance, 3, 4);
					   }
					   else 
						   draw3_popdwon ("Hollow", 4);
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
				case 1:/*Focal Law -> aperture -> wave type  614 */

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					   {
						   draw3_pop (NULL, set_menu_position4_614, "LW", wave_type, 2, 4);
					   }
					   else 
						   draw3_popdwon ("LW", 4);
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
				case 2:/*Scan -> area -> index end  724 */
					   /* 当前步进 */
					   switch (pp->p_tmp_config->index_end_reg)
					   {
						   case 0:	tmpf = 1.0; break;
						   case 1:	tmpf = 10.0; break;
						   case 2:	tmpf = 100.0; break;
						   default:break;
					   }
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					   {
						   cur_value = pp->p_config->index_end;
						   lower = -100000.0;
						   upper = 100000.0;
						   step = tmpf;
						   digit = 0;
						   pos = 4;
						   unit = UNIT_MM;
						   draw3_pressed (data_724, units[unit], cur_value , lower, upper, step, digit, p, pos);
					   }
					   else 
					   {
						   cur_value = pp->p_config->index_end;
						   digit = 0;
						   pos = 4;
						   unit = UNIT_MM;
						   draw3_stop (cur_value, units[unit], digit, pos);
					   }
					   break;

				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 0:/*File -> File -> save mode  804 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						draw3_pop (NULL, set_menu_position4_804, "Inspection Data", save_mode, 4, 4);
					}
					else 
						draw3_popdwon ("Inspection Data", 4);
					break;


				case 1:break;
				case 2:/*File -> format -> view  824 */
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					   {
						   draw3_pop (NULL, set_menu_position4_824, "Current Layout", view, 3, 4);
					   }
					   else 
						   draw3_popdwon ("Current Layout", 4);
					   break;

				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[9])
			{
				case 0:/*preferences -> pref. -> gate mode   904 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					{
						draw3_pop (NULL, set_menu_position4_904, "By Group", gate_mode, 2, 4);
					}
					else 
						draw3_popdwon ("By Group", 4);
					break;


				case 1:break;
				case 2:/*Preferences -> service -> startup mode  924*/
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
					   {
						   draw3_pop (NULL, set_menu_position4_924, "Automatic", startup_mode, 2, 4);
					   }
					   else 
						   draw3_popdwon ("Automatic", 4);
					   break;


				case 3:break;
				case 4:/*Preferences -> Network -> Remote PC*/
# if 0
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
# endif
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

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

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
				case 1: /* 重复频率 PRF  115  */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						draw3_pop (NULL, set_menu_position5, "60", PRF, 5, 5);
					}
					else 
						draw3_popdwon ("60", 5);

					break;


				case 2:/*Reject  125 */
					/* 当前步进 */
					switch (pp->p_tmp_config->reject_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;                                               				
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						cur_value = pp->p_config->reject;
						lower = 1.0;
						upper = 100.0;
						step = tmpf;
						digit = 0;
						pos = 5;
						unit = UNIT_BFH;
						draw3_pressed (data_125, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->reject;
						digit = 0;
						pos = 5;
						unit = UNIT_BFH;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 3:/*Gain Offset  135 */
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
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						cur_value = pp->p_config->gain_offset/10.0;
						lower = 1.0;
						upper = 74.0;
						step = tmpf;
						digit = 1;
						pos = 5;
						unit = UNIT_DB;
						draw3_pressed (data_135, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->gain_offset/10.0;
						digit = 1;
						pos = 5;
						unit = UNIT_DB;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

				case 4:/*Sum Gain   145 */

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						draw3_pop (NULL, set_menu_position5_145, "Auto", sum_gain, 3, 5);
					}
					else 
						draw3_popdwon ("Auto", 5);

					break;


				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*NULL*/
					break;
				case 1:/*Condition   215 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						draw3_pop (NULL, set_menu_position5_215, "None", conditionB, 9, 5);
					}
					else 
						draw3_popdwon ("None", 5);

					break;


				case 2:/*Hold Time 225 */
					/* 当前步进 */
					switch (pp->p_tmp_config->holdtime_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 10.0; break;						
						default:break;
					}
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						cur_value = pp->p_config->holdtime/100.0;
						lower = 0.0;
						upper = 5000.0;
						step = tmpf;
						digit = 2;
						pos = 5;
						unit = UNIT_MS;
						draw3_pressed (data_225, units[unit], cur_value , lower, upper, step, digit, p, pos);
					}
					else 
					{
						cur_value = pp->p_config->holdtime/100.0;
						digit = 2;
						pos = 5;
						unit = UNIT_MS;
						draw3_stop (cur_value, units[unit], digit, pos);
					}
					break;

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
				case 0:/*Measurements -> Reading -> Field 4 305 */
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						draw3_pop (NULL, set_menu_position5_305, "A%", field1, 3, 5);
					}
					else 
						draw3_popdwon ("A%", 5);
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
				case 1:/*Display -> Overlay -> overlay  415 */
					   /* 格式化字符串 */
					   g_sprintf (temp,"%s", con2_p[4][1][5]);

					   /* 设置label */
					   gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);

					   if (!p->p_config->overlay_overlay)
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[5]), "On");
					   }
					   else
					   {
						   gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Off");
					   }

					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					   {
						   draw3_onoffpressed (5);
					   }
					   else 
						   draw3_onoffstop (5);
					   break;


				case 2:break;
				case 3:break;
				case 4:/*Display -> properties -> overlay  445 */
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					   {
						   draw3_pop (NULL, set_menu_position5_445, "None", overlay, 3, 5);
					   }
					   else 
						   draw3_popdwon ("None", 5);
					   break;


				default:break;
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 0:/*Probe/Part -> Select -> Auto Detect  505 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][5]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);

					if (!p->p_config->auto_detect)
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[5]), "On");
					}
					else
					{
						gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Off");
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					{
						draw3_onoffpressed (5);
					}
					else 
						draw3_onoffstop (5);
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
					   /* 当前步进 */
					   switch (pp->p_tmp_config->index_resolution_reg)
					   {
						   case 0:	tmpf = 0.1; break;
						   case 1:	tmpf = 0.5; break;
						   case 2:	tmpf = 1.0; break;
						   default:break;
					   }
					   if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
					   {
						   cur_value = pp->p_config->index_resolution;
						   lower = 0.01;
						   upper = 36.6;
						   step = tmpf;
						   digit = 2;
						   pos = 5;
						   unit = UNIT_MM;
						   draw3_pressed (data_722, units[unit], cur_value , lower, upper, step, digit, p, pos);
					   }
					   else 
					   {
						   cur_value = pp->p_config->index_resolution;
						   digit = 2;
						   pos = 5;
						   unit = UNIT_MM;
						   draw3_stop (cur_value, units[unit], digit, pos);
					   }
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
# if 0
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
# endif
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
				(pp->pos_last2 == i) || (pp->pos2[pp->pos][pp->pos1[pp->pos]] == i) || pa) 
		{
			gtk_widget_set_sensitive (pp->eventbox30[i], TRUE);
			gtk_widget_set_sensitive (pp->eventbox31[i], TRUE);
			gtk_widget_set_size_request(GTK_WIDGET(pp->eventbox30[i]), 114, 60);            /* */


			/*
			 * 如果三级菜单存在, 显示出来, 并显示 选项或者数值
			 * 不存在就隐藏
			 * */
			if ( con2_p[pp->pos][pp->pos1[pp->pos]][i] ) 
			{
				if (CUR_POS != i) {
					/* 0-5 表示6个3三级菜单 */
					switch (i)
					{
						case 0:	draw3_data0(p);break;
						case 1:	draw3_data1(p);break;
						case 2:	draw3_data2(pp);break;
						case 3:	draw3_data3(pp);break;
						case 4:	draw3_data4(pp);break;
						case 5:	draw3_data5(pp);break;
						default:break;
					}
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
	switch (CUR_POS)
	{
		case 0:	draw3_data0(p);break;
		case 1:	draw3_data1(p);break;
		case 2:	draw3_data2(pp);break;
		case 3:	draw3_data3(pp);break;
		case 4:	draw3_data4(pp);break;
		case 5:	draw3_data5(pp);break;
		default:break;
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
			a[i].y = g_random_int_range(128, 384);

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
	gchar buf[128];
	GtkWidget *drawing_area;
	GtkWidget *drawing_area1;
	GtkWidget *window = p->window;
	pp->pos_pos = MENU3_STOP;
	pp->menu2_qty = 5;
	GtkWidgetClass *widget_window_class1;

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
		gtk_container_set_border_width( GTK_CONTAINER(pp->event[i]), 0);     /*设置边框大小，这个地方使用图片*/
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

	/* 一级菜单的初始化 */
	p->menubar		= gtk_menu_bar_new();
	p->menu			= gtk_menu_new();
	p->menuitem_main	= gtk_menu_item_new_with_label(content_en10[1]);
	gtk_widget_set_size_request(GTK_WIDGET(p->menuitem_main), 116, 83);          /*刚好合适的大小*/
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
	}
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(p->menuitem_main), p->menu);/*最后把菜单menu1粘到菜单项menuitem1上*/
	gtk_box_pack_start(GTK_BOX(p->hbox212), p->menubar, FALSE, FALSE, 0);
	/*	g_object_set (p->menubar, "pack-direction", GTK_PACK_DIRECTION_LTR, NULL); */
	gtk_widget_show(p->menubar);
	gtk_widget_show(p->menu);
	gtk_widget_show(p->menuitem_main);



	/*二级菜单的初始化*/
	for (i = 0; i < 5; i++) 
	{
		p->eventbox2[i] = gtk_event_box_new();
		gtk_widget_set_size_request(GTK_WIDGET(p->eventbox2[i]), 114, 85);
		p->label2[i] = gtk_label_new("<^_^>");
		gtk_container_add(GTK_CONTAINER(p->eventbox2[i]), p->label2[i]);
		gtk_container_set_border_width( GTK_CONTAINER(p->eventbox2[i]), 0);     /*设置边框大小，这个地方使用图片做背景*/
		update_widget_bg(p->eventbox2[i], backpic[1]);
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

	/* 三级菜单 弹出的初始化 */

	pp->menu3 = gtk_menu_new();
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

	pp->menu_bar3 = gtk_menu_bar_new ();
	gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu_bar3) , pp->root_menu3);

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

	/* 增益显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[0], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[0]), 60, 45);
	update_widget_bg(pp->event[0], backpic[3]);
	gtk_label_set_text (GTK_LABEL (pp->label[0]), "Gain\n(dB)");      /* 增益单位以及标识 */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->event[1], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[1]), 112, 45);  
	update_widget_bg(pp->event[1], backpic[4]);
	gtk_label_set_text (GTK_LABEL (pp->label[1]), " ");      /* 增益数值 */

	/* 小状态栏  */
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[0], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[1], FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox111), pp->vbox1111[2], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[2], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[2]), 172, 22);
	update_widget_bg(pp->event[2], backpic[5]);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[0]), pp->event[3], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[3]), 172, 22);
	update_widget_bg(pp->event[3], backpic[5]);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[4], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[4]), 172, 22);
	update_widget_bg(pp->event[4], backpic[5]);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[1]), pp->event[5], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[5]), 172, 22);
	update_widget_bg(pp->event[5], backpic[5]);

	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[6], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[6]), 172, 22);
	update_widget_bg(pp->event[6], backpic[5]);
	gtk_box_pack_start (GTK_BOX (p->vbox1111[2]), pp->event[7], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[7]), 172, 22);
	update_widget_bg(pp->event[7], backpic[5]);

	/* 4个测量值显示 */
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[8], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[8]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[8]), 60, 45);
	update_widget_bg(pp->event[8], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[9], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[9]), 112, 45);
	update_widget_bg(pp->event[9], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[10], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[10]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[10]), 60, 34);
	update_widget_bg(pp->event[10], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[11], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[11]), 112, 34);
	update_widget_bg(pp->event[11], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[12], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[12]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[12]), 60, 34);
	update_widget_bg(pp->event[12], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[13], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[13]), 112, 34);
	update_widget_bg(pp->event[13], backpic[4]);

	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[14], FALSE, FALSE, 0);
	gtk_label_set_text (GTK_LABEL (pp->label[14]), "%A\n(%)");
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[14]), 60, 34);
	update_widget_bg(pp->event[14], backpic[3]);
	gtk_box_pack_start (GTK_BOX (p->hbox112), pp->event[15], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[15]), 112, 34);
	update_widget_bg(pp->event[15], backpic[4]);


	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox21, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox22, FALSE, FALSE, 0);
	gtk_widget_show(p->vbox21);
	gtk_widget_show(p->vbox22);

	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox211, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox212, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox211);
	gtk_widget_show(p->hbox212);

	gtk_widget_show_all (pp->hbox1);   /* 上方显示信息  */

	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(drawing_area), 658, 425);
	gtk_box_pack_start (GTK_BOX (p->hbox211), drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x0, p->col.green = 0x0, p->col.blue = 0x0;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show (drawing_area);
	g_signal_connect (G_OBJECT (drawing_area), "expose_event", G_CALLBACK(draw_gtk), NULL);




	pp->dialog = gtk_dialog_new_with_buttons("TanDenghua", GTK_WINDOW (pp->window),
			/*			GTK_DIALOG_MODAL| */  /* 独占窗口 */
			// GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT |
			GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

	widget_window_class1 = GTK_WIDGET_GET_CLASS (((GtkObject*)(pp->dialog))); 
	// 取代原來的處理函式
	widget_window_class1->key_press_event =	my_keypress_event;

	gtk_window_set_decorated (GTK_WINDOW (pp->dialog), FALSE);			/*不可以装饰*/
	gtk_container_set_border_width( GTK_CONTAINER (GTK_DIALOG(pp->dialog)->vbox), 0);     /*设置边框大小，这个地方使用图片*/
	gtk_widget_show (GTK_DIALOG(pp->dialog)->vbox);

	pp->button_add = gtk_button_new_with_label ("+");    /* 加减数值 */
	pp->button_sub = gtk_button_new_with_label ("-");    /*  */
	pp->vscale = gtk_vscale_new_with_range(1.0, 100.0, 1.0);

	/* vscalebox */
	pp->vscalebox = gtk_vbox_new (FALSE, 0);
	//	gtk_widget_set_size_request (GTK_WIDGET(pp->vscalebox), 30, 400);
	gtk_widget_set_size_request (GTK_WIDGET(pp->dialog), 30, 425);
	gtk_widget_set_size_request (GTK_WIDGET(pp->vscalebox), 30, 425);
//	gtk_box_pack_start (GTK_BOX (GTK_DIiLOG(pp->dialog)->vbox), pp->vscalebox, FALSE, FALSE, 0);
	gtk_widget_show (pp->vscalebox);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->button_add, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->vscale, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->vscalebox), pp->button_sub, FALSE, FALSE, 0);
	gtk_widget_show (pp->button_add);
	gtk_widget_show (pp->button_sub);
	gtk_widget_show (pp->vscale);

	gtk_widget_set_can_focus (pp->button_add, FALSE);
	gtk_widget_set_can_focus (pp->vscale, FALSE);
	gtk_widget_set_can_focus (pp->button_sub, FALSE);
	gtk_widget_set_can_focus (pp->dialog, FALSE);

	gtk_widget_modify_base (GTK_WIDGET (pp->dialog), GTK_STATE_NORMAL, &color_button0);
	gtk_widget_modify_bg (GTK_WIDGET (pp->dialog), GTK_STATE_NORMAL, &color_button0);

	gtk_window_move (GTK_WINDOW (pp->dialog), 650, 90);
//	gtk_widget_show (pp->dialog);
	gtk_widget_hide (GTK_DIALOG(pp->dialog)->action_area);

	gtk_box_pack_start (GTK_BOX (p->hbox211), pp->vscalebox, FALSE, FALSE, 0);
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

#if 0
	pp->window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (pp->window1), FALSE);			/*不可以装饰*/
	pp->window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (pp->window2), FALSE);			/*不可以装饰*/
#endif

	/*	pp->p_config->unit = 1;*/

	g_thread_create((GThreadFunc)(time_handler), (gpointer) (drawing_area), FALSE, NULL);
	/*
	   gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221, FALSE, FALSE, 0);
	   gtk_widget_show(p->vbox221);
	   */

}
