/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 *
 *
 */

#include "drawui.h"
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>
#include <time.h>

static char buffer[32];

gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);
GtkWidgetClass *widget_window_class;

GdkPoint a[512] = {{0,0}, {240,200}, {400,200}};

const gchar *backpic[] = 
{
	"pic/10.png", "pic/11.png", "pic/12.png",		/* 二级菜单 0按下 1未选中 2 停留*/
	"pic/00.png", "pic/01.png", "pic/02.png",	        /* 00上方数值 标识栏 01 数值背景 02 比较小的背景 */
	"pic/20.png", "pic/21.png", "pic/22.png",		/* 三级菜单名称 0按下 1未选中 2 停留 */
	"pic/30.png", "pic/31.png", "pic/32.png",		/* 三级菜单数值 0按下 1未选中 2 停留 */
	"pic/tt.png", "pic/311.png", "pic/322.png"      /*  软键盘图标 */
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
const gchar **selection  =selection_en;
const gchar **source     =source_en;

const gchar **display    =display_en;
const gchar **displ      =displ_en;
const gchar **group      =group_en;
const gchar **utunit     =utunit_en;
const gchar **grid       =grid_en;
const gchar **zoom_display =zoom_display_en;
const gchar **select1     =select_en;
const gchar **color_mode  =color_mode_en;
const gchar **scan       =scan_en;
const gchar **color      =color_en;
const gchar **envelope   =envelope_en;
const gchar **properties_source =properties_source_en;
const gchar **appearance =appearance_en;
const gchar **overlay    =overlay_en;
const gchar **fft_color  =fft_color_en;
const gchar **orientation =orientation_en;

const gchar **group_mode     =group_mode_en;
const gchar **group_mode1    =group_mode1_en;
const gchar **probe_select   =probe_select_en;
const gchar **probe_skew     =probe_skew_en;
const gchar **geometry       =geometry_en;
const gchar **material       =material_en;


const gchar **law_config        =law_config_en;
const gchar **law_config1       =law_config1_en;
const gchar **wave_type         =wave_type_en;
const gchar **wave_type1        =wave_type1_en;


const gchar **inspection_type   =inspection_type_en;
const gchar **inspection_scan   =inspection_scan_en;
const gchar **start_mode        =start_mode_en;
const gchar **storage           =storage_en;
const gchar **inspec_data       =inspec_data_en;


const gchar **file_storage      =file_storage_en;
const gchar **file_storage1     =file_storage1_en;
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

	for (i = 0; i < 11; i++)
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
		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu3), pp->menu_item3[i]);
		g_signal_connect (pp->menu_item3[i], "activate",
				G_CALLBACK(fun), (GUINT_TO_POINTER (i)));
		if (0x01 & (menu_status >> i))
			gtk_widget_set_sensitive (pp->menu_item3[i], FALSE);
		gtk_widget_show (pp->menu_item3[i]);
	}
	gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pop_pos]));

	menu = GTK_MENU (pp->menu3);

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

		gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu3), pp->menu_item3[i]);
		g_signal_connect (pp->menu_item3[i], "activate",
				G_CALLBACK(fun), (GUINT_TO_POINTER (i)));
		if (0x01 & (menu_status >> i))
			gtk_widget_set_sensitive (pp->menu_item3[i], FALSE);
		gtk_widget_show (pp->menu_item3[i]);
	}
	gtk_menu_item_select (GTK_MENU_ITEM (pp->menu_item3[pop_pos]));

	menu = GTK_MENU (pp->menu3);

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


/*旧版三级菜单弹出函数，仅弹出菜单，不可更换选项*/
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

	update_widget_bg(pp->eventbox30[z], backpic[6]);
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
	//gtk_widget_hide (pp->vscalebox);
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

	gtk_menu_popdown( GTK_MENU (pp->menu3));

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

# if 0
static void draw3_onoffstop(guint pos)               /* button 为 on/off 时的颜色变化*/
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;

	//update_widget_bg(pp->eventbox30[z], backpic[6]);/*被按下时的状态*/
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


	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);

	if (str)
		g_free(str);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	//gtk_widget_hide (pp->vscalebox);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
}


static void draw3_onoffpressed(guint pos)
{
	gint  x, y, z;       /* xyz 分别为123级菜单位置 */
	gchar *str = NULL;

	x = pp->pos;
	y = pp->pos1[x];
	z = pos;
	/*	gtk_widget_destroy (pp->vscale);*/
	// update_widget_bg(pp->eventbox30[z], backpic[6]);/*被按下时的状态*/
	if ((CUR_POS == z) && (pp->pos_pos == MENU3_PRESSED))
	{
		update_widget_bg(pp->eventbox30[z], backpic[6]);
		update_widget_bg(pp->eventbox31[z], backpic[11]);
	}
	else
	{
		update_widget_bg(pp->eventbox30[z], backpic[7]);
		update_widget_bg(pp->eventbox31[z], backpic[10]);
	}

	str = g_strdup_printf ("%s", con2_p[x][y][z]);	
	gtk_label_set_text (GTK_LABEL (pp->label3[z]), str);

	if (str)
		g_free(str);

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[z]);
	gtk_widget_show (pp->eventbox31[z]);
	gtk_widget_show (pp->data3[z]);
	gtk_widget_hide (pp->sbutton[z]);
	//gtk_widget_hide (pp->vscalebox);
	//	gtk_widget_show (pp->vscalebox);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
}
#endif
/*改变各图形显示区域的尺寸大小*/

static gboolean draw_colorbar(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	gint i;
	gdouble tmp;
	guint high ;

	high = GPOINTER_TO_UINT (data);

	cairo_t *cr;        //声明一支画笔
	cr=gdk_cairo_create(widget->window);//创建画笔
	cairo_set_source_rgba(cr,1.0,1.0,1.0,1.0);//设置画笔颜色，也就是红，绿，蓝，这里设置成白色。
	//cairo_set_line_width(cr,1);

	/* cairo_move_to (cr, 0, 0);
	   cairo_line_to (cr, 8, 0);
	   cairo_line_to (cr, 8, 390);
	   cairo_line_to (cr, 0, 390);
	   cairo_line_to (cr, 0, 0);*/

	cairo_rectangle(cr,0,0,8,390);//画一个方块，位置从坐标(10,10)开始，宽200，高200
	cairo_fill(cr);//填充，使用的颜色当然是上面设置的颜色。
	for ( i = 1; i < high; i++ ) {
		tmp = 1.0 - i / (gfloat)(high);
		cairo_set_source_rgba(cr, tmp, tmp, tmp, 1.0);
		cairo_move_to(cr,1,i);
		cairo_line_to(cr,7,i);
		cairo_stroke(cr);
	}

	cairo_destroy(cr);//销毁画笔

	return TRUE;
}


static void draw_area(GtkWidget *parent_box, DRAW_AREA *draw_area, guint width, guint height, const gchar *title, 
		gdouble v1s, gdouble v1e, gdouble v2s, gdouble v2e, gdouble h1s, gdouble h1e, guchar *other)
{

	draw_area->vbox = gtk_vbox_new (FALSE, 0);
	/* 窗口名字 */
	draw_area->ebox = gtk_event_box_new ();
	gtk_widget_modify_bg (draw_area->ebox, GTK_STATE_NORMAL, &color_text_base);
	draw_area->label = gtk_label_new (title);
	gtk_widget_modify_fg (draw_area->label, GTK_STATE_NORMAL, &color_white);
	gtk_container_add (GTK_CONTAINER (draw_area->ebox), draw_area->label);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->ebox, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET (draw_area->ebox), width, 20);
	//	gtk_widget_show(draw_area->label);
	//	gtk_widget_show(draw_area->ebox);

	/* 3个刻度尺 1个画图区 */
	draw_area->table = gtk_table_new (2, 4, FALSE);
	gtk_box_pack_start (GTK_BOX (draw_area->vbox), draw_area->table, FALSE, FALSE, 0);
	draw_area->drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(draw_area->drawing_area), width - 40, height - 35);
	gtk_widget_modify_bg (draw_area->drawing_area, GTK_STATE_NORMAL, &color_black1);
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->drawing_area, 1, 2, 0, 1,
			GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);

	draw_area->vruler1 = gtk_vruler_new ();
	gtk_ruler_set_metric (GTK_RULER (draw_area->vruler1), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (draw_area->vruler1), v1s, v1e, 0, 1);
	gtk_widget_modify_bg (draw_area->vruler1, GTK_STATE_NORMAL, &color_rule);
	g_signal_connect_swapped (G_OBJECT (draw_area->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD (draw_area->vruler1, motion_notify_event)),
			draw_area->vruler1);
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->vruler1, 0, 1, 0, 1,
			GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

	draw_area->vruler2 = gtk_vruler_new ();
	gtk_ruler_set_metric (GTK_RULER (draw_area->vruler2), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (draw_area->vruler2), v2s, v2e, 0, 1);
	gtk_widget_modify_bg(draw_area->vruler2, GTK_STATE_NORMAL, &color_rule);
	g_signal_connect_swapped (G_OBJECT (draw_area->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD (draw_area->vruler2, motion_notify_event)),
			draw_area->vruler2);			
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->vruler2, 3, 4, 0, 1,
			GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

	draw_area->drawarea_colorbar = gtk_drawing_area_new();
	gtk_widget_set_size_request (draw_area->drawarea_colorbar, 8, height - 35);
	g_signal_connect (G_OBJECT (draw_area->drawarea_colorbar), "expose_event",
			G_CALLBACK (draw_colorbar), GUINT_TO_POINTER (height - 36));
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->drawarea_colorbar, 2, 3, 0, 1,
			GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

	draw_area->hruler = gtk_hruler_new ();
	gtk_ruler_set_metric (GTK_RULER (draw_area->hruler), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (draw_area->hruler), h1s, h1e, 0, 60);
	gtk_widget_modify_bg(draw_area->hruler, GTK_STATE_NORMAL, &color_rule);
	g_signal_connect_swapped (G_OBJECT (draw_area->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD (draw_area->hruler, motion_notify_event)),
			draw_area->hruler);
	gtk_table_attach (GTK_TABLE (draw_area->table), draw_area->hruler, 1, 2, 1, 2,
			GTK_EXPAND|GTK_SHRINK|GTK_FILL, GTK_FILL, 0, 0);
	gtk_widget_show_all(draw_area->vbox);

	gtk_box_pack_start (GTK_BOX (parent_box), draw_area->vbox, FALSE, FALSE, 0);
	return ;
}


/* 画波形数据显示区 */
void draw_area_all()
{
	gint i;

	/* 把之前的区域释放 */
	for (i = 0; i < 4; i ++)
	{
		gtk_widget_destroy(pp->vbox_area[i]);
		gtk_widget_destroy(pp->hbox_area[i]);
		pp->vbox_area[i] = gtk_vbox_new(FALSE, 0);
		pp->hbox_area[i] = gtk_hbox_new(FALSE, 0);
	}

	//	CFG(display) = A_B_SCAN;

	if (1) 
	{
		switch (CFG(display))
		{
			case A_SCAN:
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "A-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
				break;
			case B_SCAN:
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 425, "B-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
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
				gtk_box_pack_start (GTK_BOX (pp->vboxtable), pp->vbox_area[0], FALSE, FALSE, 0);
				draw_area(pp->vbox_area[0], &(pp->draw_area[0]), 655, 150, "A-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				draw_area(pp->vbox_area[0], &(pp->draw_area[1]), 655, 275, "B-scan", 0.0, 100.0,
						0.0, 100.0, 0.0, 100.0, NULL);
				gtk_widget_show (pp->vbox_area[0]);
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
				if (CFG(ut_unit)==2)
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
	else ;

	return ;
}









/* 三级菜单第一个 */
void draw3_data0(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit, content_pos, menu_status;

	p = NULL;

	guint menu_on=0, i;

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
				case 0: /* 增益 Gain P100 */
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

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = (GROUP_VAL(gain) - GROUP_VAL(gainr) * GROUP_VAL(db_ref)) / 100.0; 
						lower = 0.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
						upper = 74.0 - GROUP_VAL(gainr) * GROUP_VAL(db_ref) / 100.0 ;
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

				case 1: /* 发射 Pulser P110 */
					/* 当前步进 */
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
						unit = UNIT_TO;    /* 1 to n 这个范围需要计算出来 waiting */
						g_sprintf (temp,"(1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_110, temp, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (GROUP_VAL(pulser));
						digit = 0;
						pos = 0;
						unit = UNIT_TO;
						g_sprintf (temp,"(1 to %d)", (guint)(upper));
						draw3_digit_stop (cur_value, temp, digit, pos, 0);
					}
					break;

				case 2: /* Reveiver 接收器  P120 */
					/* 当前步进 */
					switch (TMP(receiver_reg))
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;
						default:break;
					}

					if (GROUP_VAL(tx_rxmode) == PULSE_ECHO)
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
						unit = UNIT_TO;    /* 1 to n 这个范围需要计算出来 waiting */
						g_sprintf (temp,"(1 to %d)", (guint)(upper));
						draw3_digit_pressed (data_120, temp, cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = (gfloat) (GROUP_VAL(receiver));
						digit = 0;
						pos = 0;
						unit = UNIT_TO;
						g_sprintf (temp,"(1 to %d)", (guint)(upper));
						draw3_digit_stop (cur_value, temp, digit, pos, 0);
					}
					break;

				case 3:/*Scan Offset p130 */
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
				case 0:/* Gate 选择调节哪个闸门 P200 */
					pp->x_pos = 630, pp->y_pos = 120;
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_200, NULL, 
								menu_content[GATE_POS + GROUP_VAL(gate_pos)],
								menu_content + GATE_POS, 3, 0, GROUP_VAL(gate_pos), 0);
					else 
						draw3_popdown (menu_content[GATE_POS + GROUP_VAL(gate_pos)], 0, 0);
					break;

				case 1:/*Alarm  P210 */
					pp->x_pos = 603, pp->y_pos = 118;

					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						menu_on = 0;
						for ( i = 0 ; i < 16; i++)
							menu_on |= (CFG(alarm[i].conditiona) !=0 ) << i;
						draw3_pop_tt_on (data_210, NULL, menu_content[ALARM_POS + CFG(alarm_pos)],		menu_content + ALARM_POS, 16, 0, CFG(alarm_pos), 0, menu_on, 16);
					}
					else 
						draw3_popdown (menu_content[ALARM_POS + CFG(alarm_pos)], 0, 0);

					break;


				case 2:/*Output  p220  */
					pp->x_pos = 585, pp->y_pos = 148;

					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
					{
						menu_on = 0;
						for ( i = 0 ; i < 3; i++)/*alarm#的值决定前三项后面是否跟[On]*/
							menu_on |= (CFG(output[i].alarm1) !=0 ) << i;
						draw3_pop_tt_on (data_220, NULL, menu_content[OUTPUT_POS + CFG(output_pos)],		menu_content + OUTPUT_POS, 5, 0, CFG(output_pos), 0, menu_on, 5);

						for ( i = 3 ; i < 5; i++)/*data的值决定后两项后面是否跟[On]*/
							menu_on |= (CFG(output[i].data) !=0 ) << i;
						draw3_pop_tt_on (data_220, NULL, menu_content[OUTPUT_POS + CFG(output_pos)],		menu_content + OUTPUT_POS, 5, 0, CFG(output_pos), 0, menu_on, 5);
					}

					else 
						draw3_popdown (menu_content[OUTPUT_POS + CFG(output_pos)], 0, 0);

					break;

				case 3:/*Sizing Curves -> Setup  p230 */
					pp->x_pos = 605, pp->y_pos = 148;

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						if (!GROUP_VAL(curve_pos))
						{
							menu_status = 0x02;
						}
						else
							menu_status = 0;

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
					pp->x_pos = 505, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_300, NULL, 
								list1[CFG(list)],
								list, 2, 0, CFG(list), 0);
					else 
						draw3_popdown (list1[CFG(list)], 0, 0);

					break;


				case 1:/*Measurements -> Cursors -> Selection 310 */
					pp->x_pos = 575, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_310, NULL, 
								selection[CFG(select)],
								selection, 9, 0, CFG(select), 0);
					else 
						draw3_popdown (selection[CFG(select)], 0, 0);

					break;

				case 2:/*Measurements -> Table -> Display Table  p320 */
#if 0
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);

					if (!p->p_config->display_table)
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
#endif
					break;

				case 3:/*Measurements -> Thickness -> Source  p330 */
					pp->x_pos = 595, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_330, NULL, 
								source[CFG(source)],
								source, 9, 0, CFG(source), 0);
					else 
						draw3_popdown (source[CFG(source)], 0, 0);

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
				case 0:/*Display -> Selection -> Display  p400 */
					pp->x_pos = 480, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_400, NULL, 
								displ[CFG(display)],
								display, 11, 0, CFG(display), 0);
					else 
						draw3_popdown (displ[CFG(display)], 0, 0);
					break;

				case 1:/*Display -> Overlay -> UT Unit  P410 */
					pp->x_pos = 570, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_410, NULL, 
								utunit[CFG(ut_unit)],
								utunit, 3, 0, CFG(ut_unit), 0);
					else 
						draw3_popdown (utunit[CFG(ut_unit)], 0, 0);

					break;

				case 2:/*Display -> Zoom -> Display p420 */
					pp->x_pos = 515, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_420, NULL, 
								zoom_display[CFG(zoom_display)],
								zoom_display, 6, 0, CFG(zoom_display), 0);
					else 
						draw3_popdown (zoom_display[CFG(zoom_display)], 0, 0);

					break;

				case 3:/*Display -> Color -> Select  p430 */
					pp->x_pos = 520, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_430, NULL, 
								select1[CFG(color_select)],
								select1, 3, 0, CFG(color_select), 0);
					else 
						draw3_popdown (select1[CFG(color_select)], 0, 0);

					break;

				case 4:/*Display -> Properties -> Scan p440 */
					pp->x_pos = 575, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_440, NULL, 
								scan[CFG(prop_scan)],
								scan, 6, 0, CFG(prop_scan), 0);
					else 
						draw3_popdown (scan[CFG(prop_scan)], 0, 0);

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
						draw3_digit_pressed (data_510, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->scan_offset/10.0;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 2:/*Probe/Part -> characterize -> FFT p520 */
					/* 格式化字符串 */
#if 0
					g_sprintf (temp,"%s", con2_p[5][2][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);

					if (!p->p_config->fft)
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
#endif
					break;

				case 3:/*Probe/Part -> Parts -> Geometry  530 */
					pp->x_pos = 608, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_530, NULL, 
								geometry[pp->p_config->part.Geometry],
								geometry, 3, 0, pp->p_config->part.Geometry, 0);
					else 
						draw3_popdown (geometry[pp->p_config->part.Geometry], 0, 0);

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
				case 0:/*Focal Law -> configuration -> Law Config.  p600 */
					pp->x_pos = 400, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_600, NULL, 
								law_config1[CFG(law_config)],
								law_config, 4, 0,CFG(law_config), 0);
					else 
						draw3_popdown (law_config1[CFG(law_config)], 0, 0);

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

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						cur_value = pp->p_config->element_qty;
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
						cur_value = pp->p_config->element_qty;
						digit = 0;
						pos = 0;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;

				case 2:/*Focal Law -> beam -> Min.Angle p620 */
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
						draw3_digit_pressed (data_620, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->min_angle/10.0;
						digit = 1;
						pos = 0;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
					break;


				case 3:/*Focal Law -> laws -> auto program  p630 */
					/* 格式化字符串 */
#if 0
					g_sprintf (temp,"%s", con2_p[6][3][0]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);

					if (!p->p_config->auto_program)
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
#endif
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
					pp->x_pos = 540, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_710, NULL, 
								inspection_type[CFG(scan_type)],
								inspection_type, 3, 0, CFG(scan_type), 0);
					else 
						draw3_popdown (inspection_type[CFG(scan_type)], 0, 0);

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
						draw3_digit_pressed (data_720, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->scan_start;
						digit = 0;
						pos = 0;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 3:/*Scan -> start -> start mode  730 */
					pp->x_pos = 540, pp->y_pos = 120;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_730, NULL, 
								start_mode[CFG(start_mode)],
								start_mode, 3, 0, CFG(start_mode), 0);
					else 
						draw3_popdown (start_mode[CFG(start_mode)], 0, 0);

					break;

				case 4:/*Scan -> data -> storage  740 */
					pp->x_pos = 543, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_740, NULL, 
								storage[CFG(storage)],
								storage, 4, 0, CFG(storage), 0);
					else 
						draw3_popdown (storage[CFG(storage)], 0, 0);

					break;

				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 0:/*File -> File -> Storage  p800 */
					pp->x_pos = 500, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_800, NULL, 
								file_storage1[CFG(file_storage)],
								file_storage, 2, 0, CFG(file_storage), 0);
					else 
						draw3_popdown (file_storage1[CFG(file_storage)], 0, 0);

					break;

				case 1:/*File -> report -> template 810 */
					pp->x_pos = 500, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
					{
						draw3_pop (NULL, NULL, "Complete", template, 1, 0);
					}
					else 
						draw3_popdown ("Complete", 0, 0);
					break;

				case 2:/*File -> format -> user field  820 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][0]);
#if 0
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
#endif
					break;

				case 3:/*File -> user field -> select  p830 */
					pp->x_pos = 623, pp->y_pos = 118;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_830, NULL, 
								file_select[CFG(file_select)],
								file_select, 10, 0, CFG(file_select), 0);
					else 
						draw3_popdown (file_select[CFG(file_select)], 0, 0);

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
					pp->x_pos = 567, pp->y_pos = 120;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_900, NULL, 
								pref_units[CFG(unit)],
								pref_units, 2, 0, CFG(unit), 0);
					else 
						draw3_popdown (pref_units[CFG(unit)], 0, 0);

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
					pp->x_pos = 560, pp->y_pos = 120;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
						draw3_pop_tt (data_930, NULL, 
								mouse[CFG(mouse)],
								mouse, 3, 0, CFG(mouse), 0);
					else 
						draw3_popdown (mouse[CFG(mouse)], 0, 0);

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

void draw3_data1(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;
	gchar *str;

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;
	guint menu_on,i;

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
				case 0: /* start 扫描延时 P101 */
					/* 当前步进 */
					switch (TMP(start_reg))
					{
						case 0:	tmpf = (GROUP_VAL(range) / 1000.0) / 320.0; break;
						case 1:	tmpf = (GROUP_VAL(range) / 1000.0) / 20.0 ; break;
						case 2:	tmpf = (GROUP_VAL(range) / 1000.0) / 10.0 ; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
						{
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(start) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								//lower = (CFG(beam_delay) /1000.0) * GROUP_VAL(velocity) / 200000.0;
								lower = (BEAM_INFO(0,beam_delay) /1000.0) * GROUP_VAL(velocity) / 200000.0;
								upper =	(MAX_RANGE_US - GROUP_VAL(range) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * (GROUP_VAL(velocity) / 200000.0);
								digit = 2;
								pos = 1;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (GROUP_VAL(start) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								/*lower = (CFG(beam_delay) / 1000.0) * 0.03937 * GROUP_VAL(velocity) / 200000.0;*/
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
							//lower =	CFG(beam_delay) / 1000.0;
							lower =	BEAM_INFO(0,beam_delay) / 1000.0;
							upper =	(MAX_RANGE_US - GROUP_VAL(range) / 1000.0);
							step = tmpf;
							pos = 1;
							digit = 2;
							unit = UNIT_US;
						}
						draw3_digit_pressed (data_101, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else
					{
						if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
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
				case 1: /* 收发模式 Tx/Rx Mode P111 */
					pp->x_pos = 480, pp->y_pos = 209; 	
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_111, NULL, 
								menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)],
								menu_content + TX_RX_MODE, 3, 1, GROUP_VAL(tx_rxmode), 0x05);
					else 
						draw3_popdown (menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)], 1, 0);
					break;
				case 2: /* 接收滤波 P121 */
					pp->x_pos = 511, pp->y_pos = 168;
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

				case 3:/*index Offset P131*/
					g_sprintf (temp,"%s", con2_p[1][3][1]);
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

				case 4:/*Set Ref P141*/
					draw3_popdown (NULL, 1, 1);
					break;
				default:break;
			}
			break;

		case 2:
			switch (pp->pos1[2])
			{
				case 0:/* Gate->Parameters 选择 Mode还是Position  P201 */
					pp->x_pos = 590, pp->y_pos = 218;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_201, NULL, 
								menu_content[PARAMETERS + GROUP_GATE_POS(parameters)],
								menu_content + PARAMETERS, 2, 1, GROUP_GATE_POS(parameters), 0);
					else 
						draw3_popdown (menu_content[PARAMETERS + GROUP_GATE_POS(parameters)], 1, 0);

					break;

				case 1:/* Gate -> Alarm -> Group A P211 */
					pp->x_pos = 623, pp->y_pos = 210;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_211, NULL,menu_content[GROUPA + CFG_ALARM_POS(groupa)], menu_content + GROUPA, 4, 1, CFG_ALARM_POS(groupa), 0);
					else 
						draw3_popdown (menu_content[GROUPA + CFG_ALARM_POS(groupa)], 1, 0);

					break;

				case 2:/*  Gate -> Output -> Alarm #  P221 */
					/*  Gate -> Output -> Group    P221 */
					pp->x_pos = 628, pp->y_pos = 203;
					if (CFG(output_pos)==0 || CFG(output_pos)==1 || CFG(output_pos)==2)
					{
						/*	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
							draw3_pop_tt (data_221, NULL, 
							menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)],
							menu_content + ALARM1, 18, 1, CFG_OUTPUT_POS(alarm1), 0);
							else 
							draw3_popdown (menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)], 1, 0);*/

						/*if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
						  {
						  menu_on = 0;
						  for ( i = 0 ; i < 3; i++)
						  menu_on |= (CFG(output[i].alarm1) !=0 ) << i;
						  draw3_pop_tt_on (data_220, NULL, menu_content[OUTPUT_POS + CFG(output_pos)],		menu_content + OUTPUT_POS, 5, 0, CFG(output_pos), 0, menu_on, 5);

						  }*/


						if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 1))
						{
							if(CFG_OUTPUT_POS(alarm1)==0)
								draw3_pop_tt (data_221, NULL, 
										menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)],
										menu_content + ALARM1, 18, 1, CFG_OUTPUT_POS(alarm1), 0);
							else if(CFG_OUTPUT_POS(alarm1)==1)
								draw3_pop_tt (data_221, NULL, 
										menu_content[ALARM1+18+CFG_OUTPUT_POS(alarm1)],
										menu_content + ALARM1+18, 19, 1, CFG_OUTPUT_POS(alarm1), 0);
							else
							{
								if(CFG_OUTPUT_POS(alarm1)==CFG_OUTPUT_POS(alarm1))
									CFG(output[CFG(output_pos)].alarm1_value[CFG_OUTPUT_POS(alarm1)])=!CFG(output[CFG(output_pos)].alarm1_value[CFG_OUTPUT_POS(alarm1)]);
								else
									CFG(output[CFG(output_pos)].alarm1_value[CFG_OUTPUT_POS(alarm1)])=1;

								menu_on = 0;
								for ( i = 0 ; i < 16; i++)
									//menu_on = CFG(output[CFG(output_pos)].alarm1);
									menu_on |= (CFG(output[CFG(output_pos)].alarm1_value[CFG_OUTPUT_POS(alarm1)]) !=0 ) << i;
								draw3_pop_tt_on (data_221, NULL, menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)],	menu_content + ALARM1, 18, 1, CFG_OUTPUT_POS(alarm1), 0, menu_on, 18);



							}


						}
						else 
							draw3_popdown (menu_content[ALARM1+CFG_OUTPUT_POS(alarm1)], 1, 0);



					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							draw3_pop_tt (data_2211, NULL, 
									menu_content[GROUPA+CFG_OUTPUT_POS(group)],
									menu_content + GROUPA, 4, 1, CFG_OUTPUT_POS(group), 0);
						}

						else 
						{
							draw3_popdown (menu_content[GROUPA+CFG_OUTPUT_POS(group)], 1, 0);
						}
						str = g_strdup_printf ("%s", con2_p[2][2][6]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
					}
					break;


				case 3:/*Sizing Curves -> Curve  p231 */
					pp->x_pos = 575, pp->y_pos = 213;
					if (GROUP_VAL(mode_pos)==0 )
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
							draw3_pop_tt (data_231, NULL, 
									menu_content[CURVE_POS+GROUP_VAL(curve_pos)],
									menu_content + CURVE_POS, 4, 1, GROUP_VAL(curve_pos), 0);
						else 
							draw3_popdown (menu_content[CURVE_POS+GROUP_VAL(curve_pos)], 1, 0);
					}

					if (GROUP_VAL(mode_pos)==1 )
					{

						if (GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==3)
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
						else if (GROUP_VAL(curve_pos)==2)
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
								cur_value = GROUP_VAL(mat_atten)/1000.0;
								lower = 0.00;
								upper = 6.76;
								step = tmpf;
								digit = 2;
								pos = 1;
								unit = UNIT_DB_MM;
								draw3_digit_pressed (data_2312, units[unit], cur_value , lower, upper, step, digit, p, pos, 15);
							}
							else 
							{
								cur_value = GROUP_VAL(mat_atten)/1000.0;
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
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					if(CFG(list)==0)
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Manual Weld 1");
					else
						gtk_label_set_text (GTK_LABEL (pp->data3[1]), "Auto. Weld 1");
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
					break;

				case 2:/*Measurements -> Table -> Entry Image p321*/

					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[3][2][1]);
#if 0
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

					if (!p->p_config->entry_image)
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
#endif
					break;
# if 0
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
# endif
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
						draw3_digit_pressed (data_331, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->min_thickness/100.0;
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
				case 0:/*Display -> Selection -> group  401 */
					pp->x_pos = 595, pp->y_pos = 213;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					{
						draw3_pop (NULL, NULL, "Current", group, 2, 1);
					}
					else 
						draw3_popdown ("Current", 1, 0);
					break;

				case 1:/*Display -> Overlay -> grid  p411 */
					pp->x_pos = 600, pp->y_pos = 210;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_411, NULL, 
								grid[CFG(grid)],
								grid, 6, 1, CFG(grid), 0);
					else 
						draw3_popdown (grid[CFG(grid)], 1, 0);

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
				case 3:/*Display -> Color -> start p431 */
					if(CFG(color_select == 0))
					{
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
							upper = CFG(color_end)-1.0;
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_431, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = pp->p_config->color_start;
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}

					else if(CFG(color_select == 1))
					{

						/* 当前步进 */
						switch (pp->p_tmp_config->color_contrast_reg)
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = pp->p_config->color_contrast;
							lower = 0.0;
							upper = 99.0;
							step = tmpf;
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_4311, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							str = g_strdup_printf ("%s\n (%%) Δ%0.0f", con2_p[4][3][6],tmpf);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else 
						{
							cur_value = pp->p_config->color_contrast;
							digit = 0;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							str = g_strdup_printf ("%s\n    (%%)", con2_p[4][3][6]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}

					}
					else if (CFG(color_select == 2))
					{

						/* 当前步进 */
						switch (pp->p_tmp_config->min_reg)
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf =  1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						{
							cur_value = pp->p_config->min/1000.0;
							lower = 0.05;
							upper = CFG(max)/1000.0-0.01;
							step = tmpf;
							digit = 2;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_4312, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							str = g_strdup_printf ("   %s\n(mm) Δ%0.2f", con2_p[4][3][8],tmpf);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
						}
						else 
						{
							cur_value = pp->p_config->min/1000.0;
							digit = 2;
							pos = 1;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							str = g_strdup_printf ("%s\n(mm)", con2_p[4][3][8]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						}

					}

					break;

				case 4:/*Display -> Color -> Color  p441 */

					switch(CFG(prop_scan))
					{
						case 0:
							pp->x_pos = 600, pp->y_pos = 210;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_441, NULL, 
										color[CFG(color)],
										color, 6, 1, CFG(color), 0);
							else 
								draw3_popdown (color[CFG(color)], 1, 0);
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
								cur_value = pp->p_config->compress/1000.0;
								lower = 0.10;
								upper = 65536.0;
								step = tmpf;
								digit = 2;
								pos = 1;
								unit = UNIT_NONE;
								draw3_digit_pressed (data_4411, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
								str = g_strdup_printf ("%s\n      Δ%0.2f", con2_p[4][4][6],tmpf);	
								gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
							}
							else 
							{
								cur_value = pp->p_config->compress/1000.0;
								digit = 2;
								pos = 1;
								unit = UNIT_NONE;
								draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
								str = g_strdup_printf ("%s", con2_p[4][4][6]);	
								gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

							}
							break;
						case 2:
#if 0
							if (!p->p_config->ratio)
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
							/* 格式化字符串 */
							g_sprintf (temp,"%s", con2_p[4][4][8]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
#endif
							break;
						case 3:
#if 0
							if (!p->p_config->interpolation)
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
							/* 格式化字符串 */
							g_sprintf (temp,"%s", con2_p[4][4][9]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
#endif
							break;
						case 4:
							pp->x_pos = 600, pp->y_pos = 210;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_4414, NULL, 
										fft_color[CFG(fft_color)],
										fft_color, 4, 1, CFG(fft_color), 0);
							else 
								draw3_popdown (fft_color[CFG(fft_color)], 1, 0);
							g_sprintf (temp,"%s", con2_p[4][4][10]);
							gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
							break;
						case 5:
							pp->x_pos = 600, pp->y_pos = 210;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
								draw3_pop_tt (data_4415, NULL, 
										orientation[CFG(orientation)],
										orientation, 2, 1, CFG(orientation), 0);
							else 
								draw3_popdown (orientation[CFG(orientation)], 1, 0);
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
					pp->x_pos = 500, pp->y_pos = 210;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_501, NULL, 
								group_mode1[CFG(group_mode)],
								group_mode, 2, 1, CFG(group_mode), 0);
					else 
						draw3_popdown (group_mode1[CFG(group_mode)], 1, 0);

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
						draw3_digit_pressed (data_511, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->index_offset/10.0;
						digit = 1;
						pos = 1;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;

				case 2:/*probe/part -> characterize -> gain P521 */
					/* 当前步进 */
#if 0
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
						cur_value = GROUP_VAL(gain) / 10.0;
						lower = 0.0;
						upper = 74.0;
						step = tmpf;
						digit = 1;
						pos = 1;
						unit = UNIT_DB;
						draw3_digit_pressed (data_521, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = GROUP_VAL(gain) / 10.0;
						digit = 1;
						pos = 1;
						unit = UNIT_DB;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
#endif
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
						draw3_digit_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->part_thickness/100.0;
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
				case 0:/*Focal Law -> configuration -> connection P p601 */
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
						draw3_digit_pressed (data_601, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->connection_P;
						digit = 0;
						pos = 1;
						unit = UNIT_TO1;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
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
						draw3_digit_pressed (data_611, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->first_element;
						digit = 0;
						pos = 1;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 2:/*focal law -> beam -> max angle  p621*/
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

					gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					break;
				case 3:/*Focal Law -> laws -> load law file P631*/
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[6][3][1]);
#if 0
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
					gtk_widget_modify_bg (pp->eventbox30[1], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[1]), " ");
					gtk_widget_modify_bg (pp->eventbox31[1], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[1]);
					gtk_widget_show (pp->eventbox31[1]);
					gtk_widget_show (pp->data3[1]);
					if (!p->p_config->auto_program)
					{
						gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
					}
#endif

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
					pp->x_pos = 575, pp->y_pos = 210;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_711, NULL, 
								inspection_scan[CFG(inspec_scan)],
								inspection_scan, 3, 1, CFG(inspec_scan), 0);
					else 
						draw3_popdown (inspection_scan[CFG(inspec_scan)], 1, 0);

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
					break;


				case 3:/*Scan -> start -> pause  p731 */

					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[7][3][1]);

					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
#if 0
					if (!p->p_config->pause)
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
						i	draw3_onoffstop (1);
#endif
					break;

				case 4:/*Scan -> data -> inspec.data  741 */
					pp->x_pos = 541, pp->y_pos = 210;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_741, NULL, 
								inspec_data[CFG(inspec_data)],
								inspec_data, 2, 1, CFG(inspec_data), 0);
					else 
						draw3_popdown (inspec_data[CFG(inspec_data)], 1, 0);

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
#if 0
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
#endif
					break;

				case 3:/*File -> user field -> enable  831 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][3][1]);
#if 0
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

					if (!pp->p_config->enable)
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
#endif
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
				case 3:/*Preferences -> options -> EZView  931 */

					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][3][1]);
#if 0
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

					if (!pp->p_config->ezview)
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
#endif
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
	gchar *str = NULL;
	guint menu_status=0;

	gfloat cur_value, lower, upper, step;
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
				case 0: /* range范围 P102 */
					/*当前步进*/
					switch (TMP(range_reg))
					{
						case 0:	tmpf = 3.2; break;
						case 1:	tmpf = 16.0; break;
						case 2:	tmpf = 32.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
						{
							if (UNIT_MM == CFG(unit))
							{
								cur_value = (GROUP_VAL(range) / 1000.0) * (GROUP_VAL(velocity) / 200000.0);   /* 当前显示的范围数值mm */
								lower = 3.2 * GROUP_VAL(velocity) / 200000.0;
								upper = ((MAX_RANGE_US - GROUP_VAL(start) / 1000.0) > 6400.0 ? 6400.0 : 
										(MAX_RANGE_US - GROUP_VAL(start) / 1000.0)) * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * (GROUP_VAL(velocity) / 200000.0);
								digit = 2;
								pos = 2;
								unit = UNIT_MM;
							}
							else
							{
								cur_value = (GROUP_VAL(range) / 1000.0) * 0.03937 * (GROUP_VAL(velocity) / 200000.0); /* 当前显示的范围inch */
								lower =	3.2 * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								upper =	((MAX_RANGE_US - GROUP_VAL(start) / 1000.0) > 6400.0 ? 6400.0 :
										(MAX_RANGE_US - GROUP_VAL(start) / 1000.0)) * 0.03937 * (GROUP_VAL(velocity) / 200000.0);
								step = tmpf * 0.03937 * GROUP_VAL(velocity) / 200000.0;
								digit = 3;
								pos = 2;
								unit = UNIT_INCH;
							}
						}
						else 
						{
							cur_value = GROUP_VAL(range) / 1000.0 ;
							lower =	3.2;
							upper =	((MAX_RANGE_US - GROUP_VAL(start) /1000.0 ) > 6400.0 ? 6400.0 : (MAX_RANGE_US - GROUP_VAL(start) / 1000.0));
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_US;
						}
						draw3_digit_pressed (data_102, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else
					{
						if ((UT_UNIT_TRUE_DEPTH == CFG(ut_unit)) || (UT_UNIT_SOUNDPATH == CFG(ut_unit)))
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
				case 1: /* Freq频带(Mhz)  P112 */
					pp->x_pos = 614, pp->y_pos = 245;	
					/*当前步进*/
					switch (TMP(frequence_reg))
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}

					if (GROUP_VAL(probe.Name[0]) != 0)
					{
						gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
						gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
					}

					/*					if (GRPUP_VAL(probe.Name[0]) == 0)*/ /*	选择探头的时候需要同时修改GROUP_VAL(frequecne) */
					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2) && (GROUP_VAL(probe.Name[0]) == 0))
					{
						if (pp->mark_pop_change)
						{
							cur_value = GROUP_VAL(frequence) / 1000.0;
							lower =	1.0;
							upper =	20.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_NULL;
							draw3_digit_pressed (data_1121, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							/* 更新当前增益值显示 */
							str = g_strdup_printf ("%0.2f", GROUP_VAL(frequence) / 1000.0);
							draw3_pop_tt (data_112, NULL, 
									str, menu_content + FREQUENCE, 13, 2, GROUP_VAL(freq_pos), 0);
							g_free(str);
						}
					}
					else 
					{
						cur_value = GROUP_VAL(frequence) / 1000.0;
						unit = UNIT_NULL;
						pos = 2;
						digit = 2;
						draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
					}
					break;
				case 2:/* Rectifier 检波 P122 */
					pp->x_pos = 608, pp->y_pos = 265;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_122, NULL, 
								menu_content[RECTIFIER + GROUP_VAL(rectifier)],
								menu_content + RECTIFIER, 4, 2, GROUP_VAL(rectifier), 0);
					else 
						draw3_popdown (menu_content[RECTIFIER + GROUP_VAL(rectifier)], 2, 0);
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

				case 4:/* dB Ref 开关 P142 */
					draw3_popdown (menu_content[OFF_ON + GROUP_VAL(db_ref)], 2, 0);
					break;
				default:break;
			}
			break;
		case 2:
			switch (pp->pos1[2])
			{
				case 0:/*Gate->Start P202 */
					pp->x_pos = 608, pp->y_pos = 295;
					//if(CFG(parameter)==0)
					if(GROUP_GATE_POS(parameters)==0)
					{
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
							cur_value = GROUP_GATE_POS(start)/1000.0;
							lower = -3000.0;
							upper = 27000.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_GATE_POS(start)/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							if(GROUP_VAL(gate_pos)!=1)
							{
								draw3_pop_tt (data_2021, NULL, menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],menu_content + SYNCHRO, 2, 2, GROUP_GATE_POS(synchro), 0);
								// str = g_strdup_printf ("%s", con2_p[2][0][6]);	
								// gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							}
							else
							{
								draw3_pop_tt (data_2021, NULL, menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],menu_content + SYNCHRO, 3, 2, GROUP_GATE_POS(synchro), 0);
								// str = g_strdup_printf ("%s", con2_p[2][0][6]);	
								//gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
							}
						}
						else 
						{
							draw3_popdown (menu_content[SYNCHRO +GROUP_GATE_POS(synchro)], 2, 0);
							//str = g_strdup_printf ("%s", con2_p[2][0][6]);	
							//gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}
						str = g_strdup_printf ("%s", con2_p[2][0][6]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
					}

					break;

				case 1:/*Condition  P212 */
					pp->x_pos = 530, pp->y_pos = 265;

					/* groupa 与 groupb 的值相等时，conditiona 与 conditionb 不能相同*/
					if ( CFG_ALARM_POS(groupa) == CFG_ALARM_POS(groupb) )
					{
						if (CFG_ALARM_POS(conditionb))
						{
							menu_status = 1<<(CFG_ALARM_POS(conditionb));
						}

					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{         
						draw3_pop_tt (data_212, NULL,menu_content[CONDITIONA + CFG_ALARM_POS(conditiona)], menu_content + CONDITIONA, 9, 2, CFG_ALARM_POS(conditiona), menu_status);
					}

					else 
						draw3_popdown (menu_content[CONDITIONA + CFG_ALARM_POS(conditiona)], 2, 0);

					break;

				case 2:/*Output->Count p222 */
					pp->x_pos = 578, pp->y_pos = 300;
					/* 当前步进 */

					if (CFG(output_pos)==0 || CFG(output_pos)==1 || CFG(output_pos)==2)
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
									menu_content[DATA+CFG_OUTPUT_POS(data)],
									menu_content + DATA, 4, 2, CFG_OUTPUT_POS(data), 0);
						}

						else 
						{
							draw3_popdown (menu_content[DATA+CFG_OUTPUT_POS(data)], 2, 0);
						}
						str = g_strdup_printf ("%s", con2_p[2][2][7]);	
						gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

					}

					break;

				case 3:/* Gate/Alarm->Sizing Curves-> ref.amplitude  p232 */
					if( GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 || GROUP_VAL(curve_pos)==3 )
						/* Curve选项为 DAC 、 Linear DAC 或 TCG 时 */
					{
						if(( GROUP_VAL(mode_pos)==0 )&&(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2))
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
						else if(( GROUP_VAL(mode_pos)==1 )&&(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2|| GROUP_VAL(curve_pos)==3))
						{
							if( GROUP_VAL(curve_pos)==1|| GROUP_VAL(curve_pos)==3)
								/* Curve选项为 DAC或TCG 时 */
							{
								/* 当前步进 */
								switch (pp->p_tmp_config->position_reg)
								{
									case 0:	tmpf = 0.01; break;
									case 1:	tmpf = 0.1; break;
									case 2:	tmpf = 1.0; break;
									case 3:tmpf=10.0;break;						
									default:break;
								}
								if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								{
									cur_value =GROUP_VAL(position)/1000.0;
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
							else  /* Curve选项为 Linear DAC 时 */
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
						}
						else
						{
							gtk_widget_hide (pp->eventbox30[2]);
							gtk_widget_hide (pp->eventbox31[2]);
							gtk_widget_hide (pp->sbutton[2]);

						}


					}

					else
					{
						gtk_widget_hide (pp->eventbox30[2]);
						gtk_widget_hide (pp->eventbox31[2]);
						gtk_widget_hide (pp->sbutton[2]);

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
					pp->x_pos = 430, pp->y_pos = 265;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_302, NULL, 
								field[CFG(field1)],
								field1, 4, 2, CFG(field1), 0);
					else 
						draw3_popdown (field[CFG(field1)], 2, 0);

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
					break;


				case 2:/*Measurements -> Table -> Add Entry p332 */
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
						draw3_digit_pressed (data_332, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->max_thickness/100.0;
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
				case 0:
					if ( !con2_p[4][0][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;

				case 1:/*Display -> Overlay -> sizing curves  412 */

					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][2]);
#if 0
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
#endif
					break;


				case 2:
					if ( !con2_p[4][2][2] )
						gtk_widget_hide (pp->eventbox30[2]);
					gtk_widget_hide (pp->eventbox31[2]);
					break;
				case 3:/*Display -> Color -> end 432 */
					if(CFG(color_select == 0))
					{
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
							lower = CFG(color_start)+1;
							upper = 100.0;
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_432, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = pp->p_config->color_end;
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else if(CFG(color_select == 1))
					{
						//  str = g_strdup_printf ("%s", con2_p[4][3][6]);	
						//  gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

						/* 当前步进 */
						switch (pp->p_tmp_config->brightness_reg)
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = pp->p_config->brightness;
							lower = (int)((100.0-CFG(color_contrast))/2.0);
							upper = 100.0-lower;
							step = tmpf;
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_4321, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							str = g_strdup_printf ("%s\n    (%%)", con2_p[4][3][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}
						else 
						{
							cur_value = pp->p_config->brightness;
							digit = 0;
							pos = 2;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							str = g_strdup_printf ("%s\n    (%%)", con2_p[4][3][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

						}
					}
					else if (CFG(color_select == 2))
					{

						/* 当前步进 */
						switch (pp->p_tmp_config->max_reg)
						{
							case 0:	tmpf = 0.01; break;
							case 1:	tmpf =  1.0; break;
							case 2:	tmpf = 10.0; break;
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						{
							cur_value = pp->p_config->max/1000.0;
							lower = CFG(min)/1000.0 + 0.01;
							upper = 50.0;
							step = tmpf;
							digit = 2;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_4322, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
							str = g_strdup_printf ("   %s\n(mm) Δ%0.2f", con2_p[4][3][9],tmpf);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
						}
						else 
						{
							cur_value = pp->p_config->max/1000.0;
							digit = 2;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
							str = g_strdup_printf ("%s\n(mm)", con2_p[4][3][9]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

						}

					}

					break;

				case 4:/*Display -> Properties -> Envelope p442 */

					switch(CFG(prop_scan))
					{
						case 0:
							pp->x_pos = 600, pp->y_pos = 300;
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
								draw3_pop_tt (data_442, NULL, 
										envelope[CFG(envelope)],
										envelope, 2, 2, CFG(envelope), 0);
							else 
								draw3_popdown (envelope[CFG(envelope)], 2, 0);
							break;
						case 1:
#if 0
							if (!p->p_config->optimum)
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
							/* 格式化字符串 */
							g_sprintf (temp,"%s", con2_p[4][4][7]);
							gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
#endif

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
					pp->x_pos = 570, pp->y_pos = 300;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_502, NULL, 
								probe_select[CFG(probe_select)],
								probe_select, 2, 2, CFG(probe_select), 0);
					else 
						draw3_popdown (probe_select[CFG(probe_select)], 2, 0);

					break;

				case 1:/*Probe/Part -> position -> skew  p512 */
					pp->x_pos = 570, pp->y_pos = 295;
					/* 当前步进 */
					switch (pp->p_tmp_config->skew_reg)
					{
						case 0:	tmpf = 1.0; break;
						case 1:	tmpf = 10.0; break;
						case 2:	tmpf = 100.0; break;						
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						if (pp->mark_pop_change)
						{
							cur_value = CFG(skew)/100.0 ;
							lower =	0.0;
							upper =	359.9;
							step = tmpf;
							digit = 1;
							pos = 2;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_5121, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else
						{
							if (CFG(skew) > 5)
							{
								/* 更新当前增益值显示 */
								str = g_strdup_printf ("%.1f", CFG(skew)/100.0);
								draw3_pop_tt (data_512, NULL, 
										str, probe_skew, 5, 2, 4, 0);
								g_free(str);
							}
							else
								draw3_pop_tt (data_512, NULL, 
										probe_skew[CFG(skew)], probe_skew, 5, 2, CFG(skew), 0);
						}
					}
					else 
					{
						if (CFG(skew) < 5)
							draw3_popdown (probe_skew[CFG(skew)], 2, 0);
						else 
						{
							cur_value = CFG(skew)/100.0 ;
							unit = UNIT_NONE;
							pos = 2;
							digit = 1;
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
					}
					break;

				case 2:/*Probe/Part -> Characterize -> start p522 */
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
						draw3_digit_pressed (data_522, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->agate_start/100.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;

				case 3:/*Probe/Part -> Parts -> diameter p532 */
					switch(CFG(part.Geometry))
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
				case 0:/*Focal Law -> Configuration -> connection R  p602 */

					/* 当前步进 */
					switch (pp->p_tmp_config->connection_R_reg)
					{
						case 0:	tmpf = 0.0; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;
						default:break;
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
					{
						cur_value = pp->p_config->connection_R;
						lower = 1.0;
						upper = 113.0;
						step = tmpf;
						digit = 0;
						pos = 2;
						unit = UNIT_TO1;
						draw3_digit_pressed (data_601, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->connection_P;
						digit = 0;
						pos = 2;
						unit = UNIT_TO1;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
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
						draw3_digit_pressed (data_612, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->last_element;
						digit = 0;
						pos = 2;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
					break;


				case 2:/*focal law -> beam -> angel step 622 */
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

					gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
					gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
					break;
				case 3:/*focal law -> laws -> save law file 632*/

					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[6][3][2]);
#if 0
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
					gtk_widget_modify_bg (pp->eventbox30[2], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[2]), " ");
					gtk_widget_modify_bg (pp->eventbox31[2], GTK_STATE_NORMAL, &color_button1);

					/* 显示和隐藏控件 */
					gtk_widget_show (pp->eventbox30[2]);
					gtk_widget_show (pp->eventbox31[2]);
					gtk_widget_show (pp->data3[2]);
#endif
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
						draw3_digit_pressed (data_722, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->scan_resolution;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
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
					pp->x_pos = 600, pp->y_pos = 295;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_812, NULL, 
								paper_size[CFG(paper_size)],
								paper_size, 4, 2, CFG(paper_size), 0);
					else 
						draw3_popdown (paper_size[CFG(paper_size)], 2, 0);

					break;

				case 2:/*File -> format -> setup  822 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[8][2][2]);
#if 0
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
#endif
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
					pp->x_pos = 607, pp->y_pos = 295;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
						draw3_pop_tt (data_912, NULL, 
								select_key[CFG(select_key)],
								select_key, 9, 2, CFG(select_key), 0);
					else 
						draw3_popdown (select_key[CFG(select_key)], 2, 0);

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

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

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
				case 0:/* wedge delay P103 */
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
				case 1: /* 发射电压高低  P113 */
					/* PA 与 UT 的电压不一样 100 50 200 100 50 */
					pp->x_pos = 592, pp->y_pos = 400;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					{
						if (CFG(groupId) != UT_CHANNEL)
							draw3_pop_tt (data_113, NULL, 
									menu_content[VOLTAGE + 3 + CFG(voltage_pa)],
									menu_content + VOLTAGE + 6, 2, 3, CFG(voltage_pa), 0);
						else
							draw3_pop_tt (data_113, NULL, 
									menu_content[VOLTAGE + 3 + CFG(voltage_ut)],
									menu_content + VOLTAGE, 3, 3, CFG(voltage_ut), 0);
					}
					else 
					{
						if (CFG(groupId) != UT_CHANNEL)
							draw3_popdown (menu_content[VOLTAGE + 3 + CFG(voltage_pa)], 3, 0);
						else
							draw3_popdown (menu_content[VOLTAGE + 3 + CFG(voltage_ut)], 3, 0);
					}
					break;
				case 2:/*Video Filter  P123 */
					/* 格式化字符串 */
					draw3_popdown (menu_content[OFF_ON + GROUP_VAL(video_filter)], 3, 0);
					//g_sprintf (temp,"%s", con2_p[1][2][3]);
#if 0
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
#endif
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

				case 4:/*Points Qty.  P143*/

					pp->x_pos = 570, pp->y_pos = 380;
					/* 当前步进 */
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
							lower =	160.0;
							upper =	8192.0;	
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
						if (GROUP_VAL(point_qty))
						{
							str = g_strdup_printf ("%d", GROUP_VAL(point_qty));
							draw3_popdown (str, 3, 0);
							g_free(str);
						}
						else 
						{
							cur_value = GROUP_VAL(point_qty) ;
							unit = UNIT_NULL;
							pos = 3;
							digit = 0;
							draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
						}
					}
					break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[2])
			{
				case 0:/*Gate->Width P203 */
					pp->x_pos = 580, pp->y_pos = 380;
					//if(CFG(parameter)==0)
					if(GROUP_GATE_POS(parameters)==0)
					{
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
							cur_value = GROUP_GATE_POS(width)/1000.0;
							lower = 0.12;
							upper = 29000.0;
							step = tmpf;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							cur_value = GROUP_GATE_POS(width)/1000.0;
							digit = 2;
							pos = 3;
							unit = UNIT_MM;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						{
							draw3_pop_tt (data_2031, NULL, menu_content[MEASURE + GROUP_GATE_POS(measure)],	menu_content + MEASURE, 2, 3, GROUP_GATE_POS(measure), 0);
							str = g_strdup_printf ("%s", con2_p[2][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
						}
						else 
						{
							draw3_popdown (menu_content[MEASURE + GROUP_GATE_POS(measure)], 3, 0);
							str = g_strdup_printf ("%s", con2_p[2][0][7]);	
							gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
						}
					}
					break;

				case 1:/*Operator  P213*/
					pp->x_pos = 615, pp->y_pos = 380;

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_213, NULL,menu_content[OPERAT + CFG_ALARM_POS(operat)], menu_content + OPERAT, 2, 3, CFG_ALARM_POS(operat), 0);
					else 
						draw3_popdown (menu_content[OPERAT + CFG_ALARM_POS(operat)], 3, 0);

					break;


				case 2:/*Sound  p223 */
					pp->x_pos = 430, pp->y_pos = 380;

					if (CFG(output_pos)==0 || CFG(output_pos)==1 || CFG(output_pos)==2)
					{
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
							draw3_pop_tt (data_223, NULL,menu_content[SOUND + 5 + CFG_OUTPUT_POS(sound)],menu_content + SOUND, 5, 3, CFG_OUTPUT_POS(sound), 0);
						else 
							draw3_popdown (menu_content[SOUND + 5 + CFG_OUTPUT_POS(sound)], 3, 0);
					}

					/*	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
						draw3_pop_tt (data_111, NULL, 
						menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)],
						menu_content + TX_RX_MODE, 3, 1, GROUP_VAL(tx_rxmode), 0x05);
						else 
						draw3_popdown (menu_content[TX_RX_MODE + 4 + GROUP_VAL(tx_rxmode)], 1, 0);*/

					else
					{
						gtk_widget_hide (pp->eventbox30[3]);
						gtk_widget_hide (pp->eventbox31[3]);
					}

					break;

				case 3:/* Gate/Alarm->Sizing Curves->Ref.Ampl.Offset  p233*/
					if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 || GROUP_VAL(curve_pos)==3)
					{
						if(( GROUP_VAL(mode_pos)==0 ) && (GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2))
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
						else if(( GROUP_VAL(mode_pos)==1 ) && (GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2|| GROUP_VAL(curve_pos)==3))
						{

							if(GROUP_VAL(curve_pos)==1)
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
							else if(GROUP_VAL(curve_pos)==2)
							{
								gtk_widget_hide (pp->eventbox30[3]);
								gtk_widget_hide (pp->eventbox31[3]);
								gtk_widget_hide (pp->sbutton[3]);
							}
							else/*Mode 选择 Edit , Curve 选择 TCG 时*/
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

						}
						else
						{
							gtk_widget_hide (pp->eventbox30[3]);
							gtk_widget_hide (pp->eventbox31[3]);
							gtk_widget_hide (pp->sbutton[3]);

						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[3]);
						gtk_widget_hide (pp->eventbox31[3]);
						gtk_widget_hide (pp->sbutton[3]);

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
					pp->x_pos = 430, pp->y_pos = 350;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_303, NULL, 
								field[CFG(field2)],
								field1, 4, 3, CFG(field2), 0);
					else 
						draw3_popdown (field[CFG(field2)], 3, 0);

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
				case 3:/*Measurements -> Thickness -> Echo Qty.  p333*/
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
						draw3_digit_pressed (data_333, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->echo_qty;
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
				case 0:
					if ( !con2_p[4][0][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 1:/*Display -> Overlay -> gate  413 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][3]);
#if 0
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
#endif
					break;


				case 2:
					if ( !con2_p[4][2][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
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
				case 4:/*Display -> Properties -> Source  p443 */
					pp->x_pos = 565, pp->y_pos = 380;
					switch(CFG(prop_scan))
					{
						case 0:
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
								draw3_pop_tt (data_443, NULL, 
										properties_source[CFG(prop_source)],
										properties_source, 4, 3, CFG(prop_source), 0);
							else 
								draw3_popdown (properties_source[CFG(prop_source)], 3, 0);
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
				case 1:
					if ( !con2_p[5][1][3] )
						gtk_widget_hide (pp->eventbox30[3]);
					gtk_widget_hide (pp->eventbox31[3]);
					break;
				case 2:/*Probe/Part -> Characterize -> width 523 */
					/* 当前步进 */
#if 0
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
						draw3_digit_pressed (data_522, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->agate_width/100.0;
						digit = 2;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
					}
#endif
					break;


				case 3:/*Probe/Part -> parts -> material  p533 */
					pp->x_pos = 565, pp->y_pos = 380;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_533, NULL, 
								material[pp->p_config->part.Material],
								material, 7, 3, pp->p_config->part.Material, 0);
					else 
						draw3_popdown (material[pp->p_config->part.Material], 3, 0);

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
						draw3_digit_pressed (data_613, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->element_step;
						digit = 1;
						pos = 3;
						unit = UNIT_NONE;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
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
						draw3_digit_pressed (data_623, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->focus_depth/10.0;
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
						draw3_digit_pressed (data_713, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->scan_speed/10.0;
						digit = 1;
						pos = 3;
						unit = UNIT_MM_S;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
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
						draw3_digit_pressed (data_723, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->index_start;
						digit = 0;
						pos = 3;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
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
#if 0
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
#endif
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
				case 0:/*preferences -> pref. -> scheme  903 */
					pp->x_pos = 585, pp->y_pos = 380;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_903, NULL, 
								scheme[CFG(scheme)],
								scheme, 2, 3, CFG(scheme), 0);
					else 
						draw3_popdown (scheme[CFG(scheme)], 3, 0);

					break;

				case 1:/*Preferences -> system -> assign key  913 */
					pp->x_pos = 550, pp->y_pos = 380;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
						draw3_pop_tt (data_913, NULL, 
								assign_key[CFG(assign_key)],
								assign_key, 16, 3, CFG(assign_key), 0);
					else 
						draw3_popdown (assign_key[CFG(assign_key)], 3, 0);

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
				case 3:/*Preferences -> Options -> remote desktop  933 */

					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[9][3][3]);
#if 0
					/* 设置label */
					gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);

					if (!pp->p_config->remote_desktop)
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
#endif
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

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

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
				case 0:/* velocity 声速 P104 */
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
				case 1: /* 脉冲宽度 pulser width  P114 */
					pp->x_pos = 570, pp->y_pos = 470;
					/*当前步进*/
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
							g_print("pw_pos=%d\n", GROUP_VAL(pw_pos));
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


				case 2:/*Averaging  P124 */
					pp->x_pos = 623, pp->y_pos = 440;
					/*	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_124, NULL, 
						averaging[CFG(averaging)],
						averaging, 5, 4, CFG(averaging), 0);
						else 
						draw3_popdown (averaging[CFG(averaging)], 4, 0);

						break;*/

					if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_124, NULL, 
								menu_content[AVERAGING + GROUP_VAL(averaging)],
								menu_content + AVERAGING, 5, 4, GROUP_VAL(averaging), 0);
					else 
						draw3_popdown (menu_content[AVERAGING + GROUP_VAL(averaging)], 4, 0);
					break;

				case 3:/*Beam Delay  p134 */
					/* 当前步进 */
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
					break;

				case 4:/*Scale Factor  p144 */
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
			switch (pp->pos1[2])
			{
				case 0:/*Gate->Threshold  P204 */
					pp->x_pos = 590, pp->y_pos = 470;
					//if(CFG(parameter)==0)
					if(GROUP_GATE_POS(parameters)==0)
					{
						/* 当前步进 */
						switch (TMP(agate_height_reg))
						{
							case 0:	tmpf = 1.0; break;
							case 1:	tmpf = 10.0; break;						
							default:break;
						}
						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						{
							//cur_value = pp->p_config->agate_height;
							cur_value = GROUP_GATE_POS(height);
							lower = 0.0;
							upper = 98.0;
							step = tmpf;
							digit = 0;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_pressed (data_204, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
						}
						else 
						{
							//cur_value = pp->p_config->agate_height;
							cur_value = GROUP_GATE_POS(height);
							digit = 0;
							pos = 4;
							unit = UNIT_BFH;
							draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
						}
					}
					else
					{
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
					}
					break;

				case 1:/*Group B   p214 */
					pp->x_pos = 633, pp->y_pos = 470;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_214, NULL,menu_content[GROUPA + CFG_ALARM_POS(groupb)], menu_content + GROUPA, 4, 4, CFG_ALARM_POS(groupb), 0);
					else 
						draw3_popdown (menu_content[GROUPA + CFG_ALARM_POS(groupb)], 4, 0);

					break;




				case 2:/*Output->delay P224 */
					if (CFG(output_pos)==0 || CFG(output_pos)==1 || CFG(output_pos)==2)
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
							cur_value =CFG_OUTPUT_POS(delay)/1000.0;
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
					if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 || GROUP_VAL(curve_pos)==3 )
					{
						if((GROUP_VAL(mode_pos)==0) && (GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2))
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
						else if((GROUP_VAL(mode_pos)==1) && (GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 || GROUP_VAL(curve_pos)==3))
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
						else
						{
							gtk_widget_hide (pp->eventbox30[4]);
							gtk_widget_hide (pp->eventbox31[4]);
							gtk_widget_hide (pp->sbutton[4]);

						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
						gtk_widget_hide (pp->sbutton[4]);

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
					pp->x_pos = 433, pp->y_pos = 440;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_304, NULL, 
								field[CFG(field3)],
								field1, 4, 4, CFG(field3), 0);
					else 
						draw3_popdown (field[CFG(field3)], 4, 0);

					break;

				case 1:/*Measurements -> Cursors -> Add Entry p314 */
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
				case 0:
					if ( !con2_p[4][0][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 1:/*Display -> Overlay -> cursor  414 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][4]);
#if 0
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
#endif
					break;


				case 2:
					if ( !con2_p[4][2][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 3:/*Display -> color -> Mode  p434 */
					pp->x_pos = 560, pp->y_pos = 550;
					if (CFG(color_select == 2))
					{

						if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
							draw3_pop_tt (data_434, NULL, 
									color_mode[CFG(color_mode)],
									color_mode, 2, 4, CFG(color_mode), 0);
						else 
							draw3_popdown (color_mode[CFG(color_mode)], 4, 0);

					}
					else
					{
						gtk_widget_hide (pp->eventbox30[4]);
						gtk_widget_hide (pp->eventbox31[4]);
						//gtk_widget_hide (pp->data3[4]);
					}

					break;


				case 4:/*Display -> properties -> appearence  p444 */
					pp->x_pos = 545, pp->y_pos = 470;
					switch(CFG(prop_scan))
					{
						case 0:
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
								draw3_pop_tt (data_444, NULL, 
										appearance[CFG(prop_app)],
										appearance, 4, 4, CFG(prop_app), 0);
							else 
								draw3_popdown (appearance[CFG(prop_app)], 4, 0);

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
					gtk_widget_modify_bg (pp->eventbox30[4], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Undefined");
					gtk_widget_modify_bg (pp->eventbox31[4], GTK_STATE_NORMAL, &color_button1);

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
					pp->x_pos = 590, pp->y_pos = 470;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_614, NULL, 
								wave_type1[CFG(wave_type)],
								wave_type, 2, 4, CFG(wave_type), 0);
					else 
						draw3_popdown (wave_type1[CFG(wave_type)], 4, 0);

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
				case 1:
					if ( !con2_p[7][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
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
						draw3_digit_pressed (data_724, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->index_end;
						digit = 0;
						pos = 4;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
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
				case 0:/*File -> File -> save mode  804 */
					pp->x_pos = 545, pp->y_pos = 470;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_804, NULL, 
								save_mode[CFG(save_mode)],
								save_mode, 4, 4, CFG(save_mode), 0);
					else 
						draw3_popdown (save_mode[CFG(save_mode)], 4, 0);

					break;

				case 1:
					if ( !con2_p[8][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 2:/*File -> format -> view  824 */
					pp->x_pos = 545, pp->y_pos = 470;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_824, NULL, 
								view[CFG(view)],
								view, 3, 4, CFG(view), 0);
					else 
						draw3_popdown (view[CFG(view)], 4, 0);

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
				case 0:/*preferences -> pref. -> gate mode   904 */
					pp->x_pos = 590, pp->y_pos = 470;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_904, NULL, 
								gate_mode[CFG(gate_mode)],
								gate_mode, 2, 4, CFG(gate_mode), 0);
					else 
						draw3_popdown (gate_mode[CFG(gate_mode)], 4, 0);

					break;

				case 1:
					if ( !con2_p[9][1][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
				case 2:/*Preferences -> service -> startup mode  924*/
					pp->x_pos = 570, pp->y_pos = 472;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
						draw3_pop_tt (data_924, NULL, 
								startup_mode[CFG(startup_mode)],
								startup_mode, 2, 4, CFG(startup_mode), 0);
					else 
						draw3_popdown (startup_mode[CFG(startup_mode)], 4, 0);

					break;

				case 3:
					if ( !con2_p[9][3][4] )
						gtk_widget_hide (pp->eventbox30[4]);
					gtk_widget_hide (pp->eventbox31[4]);
					break;
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
	guint menu_status  = 0;

	gfloat cur_value, lower, upper, step;
	guint digit, pos, unit;

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
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

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

				case 1: /* 重复频率 PRF P115  Tandenghua 选择auto max max/2 optimum 时候需要更改界面上的PRF 等的显示 */
					pp->x_pos = 570, pp->y_pos = 500;
					/*当前步进*/
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
							cur_value = GROUP_VAL(prf) / 10.0;
							lower =	1.0;
							upper =	20000.0;	/* 最大值需要计算出来 */
							step = tmpf;
							digit = 0;
							pos = 5;
							unit = UNIT_NONE;
							draw3_digit_pressed (data_1151, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
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
				case 2:/*Reject  P125 */

					/* 当前步进 */
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
						draw3_digit_pressed (data_125, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
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



				case 3:/*Gain Offset  135 */
					/* 当前步进 */
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
					break;

				case 4:/*Sum Gain   P145 */
					pp->x_pos = 570, pp->y_pos = 558;
					/* 当前步进 */
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
					pp->x_pos = 532, pp->y_pos = 395;
					/* 当groupa与groupb的值相同时，condition互相制约 */
					if ( CFG_ALARM_POS(groupa) == CFG_ALARM_POS(groupb) )
					{
						/* 当 groupa 的值不为 None 时*/
						if (CFG(alarm[CFG(alarm_pos)].conditiona))
						{
							menu_status = 1<<(CFG_ALARM_POS(conditiona));
						}

						/* 当groupa与groupb的值相同,且condition a 与 condition b 的值相同时 */
						if ((CFG_ALARM_POS(conditiona)==CFG_ALARM_POS(conditionb)) && CFG_ALARM_POS(conditiona))
						{
							menu_status = 1<<(CFG_ALARM_POS(conditiona));
							CFG_ALARM_POS(conditionb) = 0;  /* conditionb 变为 None */
							gtk_label_set_text (GTK_LABEL (pp->data3[5]), menu_content[CONDITIONA + CFG_ALARM_POS(conditionb)]);
						}
					}

					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						draw3_pop_tt (data_215, NULL,menu_content[CONDITIONA + CFG_ALARM_POS(conditionb)], menu_content + CONDITIONA, 9, 5, CFG_ALARM_POS(conditionb), menu_status);
					else 
						draw3_popdown (menu_content[CONDITIONA + CFG_ALARM_POS(conditionb)], 5, 0);


					if (CFG_ALARM_POS(conditiona)== 0)/*conditiona为None时，conditionb不可用*/
					{
						gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
						gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
					}


					break;



				case 2:/*Hold Time P225 */
					if (CFG(output_pos)==0 || CFG(output_pos)==1 || CFG(output_pos)==2)
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
				case 3:/*NULL*/

					if(GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 || GROUP_VAL(curve_pos)==3 )
					{
						if((GROUP_VAL(mode_pos)==0) && ( GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 ))
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

						else if((GROUP_VAL(mode_pos)==1) && ( GROUP_VAL(curve_pos)==1 || GROUP_VAL(curve_pos)==2 || GROUP_VAL(curve_pos)==3 ))
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
						else
						{
							gtk_widget_hide (pp->eventbox30[5]);
							gtk_widget_hide (pp->eventbox31[5]);
							gtk_widget_hide (pp->sbutton[5]);

						}
					}
					else
					{
						gtk_widget_hide (pp->eventbox30[5]);
						gtk_widget_hide (pp->eventbox31[5]);
						gtk_widget_hide (pp->sbutton[5]);

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
					pp->x_pos = 433, pp->y_pos = 525;
					if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
						draw3_pop_tt (data_305, NULL, 
								field[CFG(field4)],
								field1, 4, 5, CFG(field4), 0);
					else 
						draw3_popdown (field[CFG(field4)], 5, 0);

					break;

				case 1:
					if ( !con2_p[3][1][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
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
				case 0:
					if ( !con2_p[4][0][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 1:/*Display -> Overlay -> overlay  415 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[4][1][5]);
#if 0
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
#endif
					break;


				case 2:
					if ( !con2_p[4][2][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 3:
					if ( !con2_p[4][3][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 4:/*Display -> properties -> overlay  445 */
					pp->x_pos = 575, pp->y_pos = 555;
					switch(CFG(prop_scan))
					{
						case 0:
							if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
								draw3_pop_tt (data_445, NULL, 
										overlay[CFG(prop_overlay)],
										overlay, 3, 5, CFG(prop_overlay), 0);
							else 
								draw3_popdown (overlay[CFG(prop_overlay)], 5, 0);

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
				case 0:/*Probe/Part -> Select -> Auto Detect  505 */
					/* 格式化字符串 */
					g_sprintf (temp,"%s", con2_p[5][0][5]);
#if 0
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
#endif
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
					gtk_widget_modify_bg (pp->eventbox30[5], GTK_STATE_NORMAL, &color_button1);
					gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Undefined");
					gtk_widget_modify_bg (pp->eventbox31[5], GTK_STATE_NORMAL, &color_button1);

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
				case 1:
					if ( !con2_p[7][1][5] )
						gtk_widget_hide (pp->eventbox30[5]);
					gtk_widget_hide (pp->eventbox31[5]);
					break;
				case 2:/*Scan -> area -> index resolution p725 */
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
						draw3_digit_pressed (data_722, units[unit], cur_value , lower, upper, step, digit, p, pos, 0);
					}
					else 
					{
						cur_value = pp->p_config->index_resolution;
						digit = 2;
						pos = 5;
						unit = UNIT_MM;
						draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
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

#if 0
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
#endif

#if 0
static void draw_gtk (GtkWidget      *widget, GdkEventExpose *eev, gpointer        data)
{
	cairo_t *cr;
	cr = gdk_cairo_create (widget->window);
	travel_path (cr);
	cairo_destroy (cr);
}
#endif

#if 0
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
#endif

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
		gtk_widget_modify_bg (pp->event[i], GTK_STATE_NORMAL, &color_button1);
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
	/*g_object_set (p->menubar, "pack-direction", GTK_PACK_DIRECTION_LTR, NULL); */
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
	g_signal_connect (pp->menu3, "key-press-event", 
			G_CALLBACK(key_press_handler), NULL);

	pp->menu_bar3 = gtk_menu_bar_new ();
	gtk_menu_shell_append (GTK_MENU_SHELL (pp->menu_bar3) , pp->root_menu3);


	/* 各box的包含关系*/
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
	gtk_box_pack_start (GTK_BOX (p->hbox1), pp->vbox12, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox111, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox11), pp->hbox112, FALSE, FALSE, 0);

	/*软键盘显示*/
	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->event[16], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[16]), 115, 52); 
	gtk_widget_modify_bg(pp->event[16], GTK_STATE_NORMAL, &color_black);
	gtk_box_pack_start (GTK_BOX (pp->vbox12), pp->hbox121, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[17], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[17]), 40, 38);
	update_widget_bg(pp->event[17], backpic[12]); 
	gtk_box_pack_start (GTK_BOX (pp->hbox121), pp->event[18], FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->event[18]), 70, 38); 
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
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox212, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox211);
	gtk_widget_show(p->hbox212);

	gtk_box_pack_start (GTK_BOX (p->hbox211), p->vboxtable, FALSE, FALSE, 0);
	gtk_widget_show(p->vboxtable);

# if 0
	pp->drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(pp->drawing_area), 658, 425);
	gtk_box_pack_start (GTK_BOX (p->hbox211), pp->drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x0, p->col.green = 0x0, p->col.blue = 0x0;
	gtk_widget_modify_bg(pp->drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show (pp->drawing_area);
	g_signal_connect (G_OBJECT (pp->drawing_area), "expose_event", G_CALLBACK(draw_gtk), NULL);
# endif


	/* 画图区域 */

# if 0
	pp->title = gtk_event_box_new();
	gtk_widget_modify_bg(pp->title, GTK_STATE_NORMAL, &color_text_base);
	pp->drawlabel = gtk_label_new_with_mnemonic("A-Scan");
	gtk_widget_modify_fg(pp->drawlabel, GTK_STATE_NORMAL, &color_white);
	gtk_container_add(GTK_CONTAINER(pp->title), pp->drawlabel);
	gtk_box_pack_start (GTK_BOX (p->vboxtable1[0][0]), pp->title, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET(pp->title), 658, 20);
	gtk_widget_show(pp->drawlabel);

	pp->table = gtk_table_new (2, 3, FALSE);
	gtk_box_pack_start (GTK_BOX (p->vboxtable1[0][0]), pp->table, FALSE, FALSE, 0);
	pp->drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(pp->drawing_area), 628, 390);
	//gtk_widget_modify_bg(pp->drawing_area, GTK_STATE_NORMAL, &color_black);
	gtk_table_attach (GTK_TABLE (pp->table), pp->drawing_area, 1, 2, 0, 1,
			GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);

	pp->vrule1 = gtk_vruler_new ();
	gtk_ruler_set_metric (GTK_RULER (pp->vrule1), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (pp->vrule1), 100, -100, 0, 1);
	//color_blue.red = 0x9999, color_blue.green = 0xeeee, color_blue.blue = 0x0;
	gtk_widget_modify_bg(pp->vrule1, GTK_STATE_NORMAL, &color_rule);
	g_signal_connect_swapped (G_OBJECT (pp->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD (pp->vrule1, motion_notify_event)),
			pp->vrule1);
	gtk_table_attach (GTK_TABLE (pp->table), pp->vrule1, 0, 1, 0, 1,
			GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

	pp->vrule2 = gtk_vruler_new ();
	gtk_ruler_set_metric (GTK_RULER (pp->vrule2), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (pp->vrule2), -100, 100, 0, 1);
	//color_blue.red = 0x9999, color_blue.green = 0xeeee, color_blue.blue = 0x0;
	gtk_widget_modify_bg(pp->vrule2, GTK_STATE_NORMAL, &color_rule);
	g_signal_connect_swapped (G_OBJECT (pp->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD (pp->vrule2, motion_notify_event)),
			pp->vrule2);			
	gtk_table_attach (GTK_TABLE (pp->table), pp->vrule2, 2, 3, 0, 1,
			GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

	pp->hrule = gtk_hruler_new ();
	gtk_ruler_set_metric (GTK_RULER (pp->hrule), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (pp->hrule), 0, 60, 0, 60);
	//color_blue.red = 0x9999, color_blue.green = 0x5555, color_blue.blue = 0x9999;
	gtk_widget_modify_bg(pp->hrule, GTK_STATE_NORMAL, &color_rule);
	g_signal_connect_swapped (G_OBJECT (pp->drawing_area), "motion_notify_event",
			G_CALLBACK (EVENT_METHOD (pp->hrule, motion_notify_event)),
			pp->hrule);
	gtk_table_attach (GTK_TABLE (pp->table), pp->hrule, 1, 2, 1, 2,
			GTK_EXPAND|GTK_SHRINK|GTK_FILL, GTK_FILL, 0, 0);
# endif

	//	change_area_size (p->vboxtable1[0][0],"A-Scan",628,390);


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

#if 0
	pp->window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (pp->window1), FALSE);			/*不可以装饰*/
	pp->window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (pp->window2), FALSE);			/*不可以装饰*/
#endif

	/*	pp->p_config->unit = 1;*/

	//	g_timeout_add(1000, (GSourceFunc) time_handler1, NULL);
	//	g_thread_create((GThreadFunc)(time_handler), (gpointer) (pp->drawing_area), FALSE, NULL);
	/*
	   gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221, FALSE, FALSE, 0);
	   gtk_widget_show(p->vbox221);
	   */

}
