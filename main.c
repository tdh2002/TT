
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <gdk/gdkkeysyms.h>

GdkColor	color_black     = {0x0, 0x0, 0x0, 0x0};
GdkColor	color_black1     = {0x0, 0x0, 0x0, 0x0800};
GdkColor	color_white     = {0x0, 0xffff, 0xffff, 0xffff};
GdkColor	color_yellow    = {0x0, 0xffff, 0xffff, 0x0};
GdkColor	color_text_base = {0x0, 0x1300, 0x4900, 0x7600};
GdkColor	color_rule      = {0x0, 0xc300, 0xf000, 0x1d00};

#if 0
GdkColor	color_button0= {0x0, 0x3100, 0x0900, 0x9f00}; /*按下*/
GdkColor	color_button1= {0x0, 0x2200, 0x5f00, 0xe700}; /*未选中*/
GdkColor	color_button2= {0x0, 0x7e00, 0xb700, 0xf300}; /*停留*/
#endif
GdkColor	color_button0= {0x0, 0x1f00, 0x2b00, 0x6b00}; /*按下*/
GdkColor	color_button1= {0x0, 0x7100, 0x8200, 0xde00}; /*未选中*/
GdkColor	color_button2= {0x0, 0x4c00, 0x5a00, 0xa100}; /*停留*/
DRAW_UI_P	pp;					

/* 测试用的初始值 */
static void set_config ()
{
	CFG(part.Velocity) = 592000; 
	CFG(gain)          = 10.0;
	CFG(start)         = 0.0;
	CFG(range)         = 10.0;
	CFG(wedge_delay)   = 0;
	CFG(color_end)     = 100.0;
	CFG(brightness)    = 50.0;
	CFG(min)           = 50.0;
	CFG(max)           = 50000.0;

}

/* You have to start somewhere */
int main (int argc, char *argv[])
{
	DRAW_UI_P		p_ui;					
	CONFIG_P		p_config;				
	TMP_CONFIG_P	p_tmp_config;					
	GtkWidget		*window;
	GtkAccelGroup	*accel;
	GClosure		*closure;

	g_thread_init(NULL);
	gdk_threads_init();
	gtk_init (&argc, &argv);

	g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));
	g_object_set (gtk_settings_get_default (), "gtk-menu-bar-accel", NULL, NULL); 

	accel = gtk_accel_group_new();

	p_ui		= (DRAW_UI_P)g_malloc0(sizeof(DRAW_UI));
	p_config	= (CONFIG_P)g_malloc0(sizeof(CONFIG));
	p_tmp_config	= (TMP_CONFIG_P)g_malloc0(sizeof(TMP_CONFIG));
	g_print("DRAW_UI's size:%d xx = %d\n", sizeof(DRAW_UI), p_ui->xx);
	g_print("CONFIG's size:%d xx = %d\n", sizeof(CONFIG), p_config->time);

	/*	window = gtk_window_new (GTK_WINDOW_POPUP);*/
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (window), FALSE);			/*不可以装饰*/
	gtk_window_set_default_size (GTK_WINDOW(window), 800, 600);		/*设置窗口大小*/
	//	gtk_window_fullscreen (GTK_WINDOW(window));						/*全屏幕*/
	gtk_widget_modify_bg (window, GTK_STATE_NORMAL, &color_black);	/*黑色背景*/
	g_signal_connect (G_OBJECT(window), "delete_event",
			G_CALLBACK(gtk_main_quit), NULL);			/**/

	p_ui->p_config		= p_config;
	p_ui->p_tmp_config	= p_tmp_config;
	p_ui->window		= window;

	p_ui->p_tmp_config->fd_config = open ("default.cfg", O_RDWR);
	if (p_ui->p_tmp_config->fd_config < 0)
		g_print("error open config file\n");
	else 
		g_print("success open config file\n");

	/*	write(p_ui->p_tmp_config->fd_config, (void*)(p_ui->p_config), sizeof(CONFIG));*/

	pp = p_ui;
	set_config();
	init_ui(p_ui);

	gtk_widget_show (window);

	g_print("float = %d, double= %d\n", sizeof(gfloat), sizeof(gdouble));

	/*
	   g_signal_connect (G_OBJECT (window), "key-press-event",
	   G_CALLBACK (key_press_handler), NULL);
	   */

	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F1, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F2, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F3, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F4, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F5, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F6, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F7, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F8, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F9, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F10, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F11, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_F12, 0, GTK_ACCEL_VISIBLE, closure);
	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_Return, 0, GTK_ACCEL_VISIBLE, closure);

	closure = g_cclosure_new(G_CALLBACK(foo), (gpointer) NULL, NULL);
	gtk_accel_group_connect(accel, GDK_Escape, 0, GTK_ACCEL_VISIBLE, closure);

	gtk_window_add_accel_group(GTK_WINDOW(window), accel);

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();

#if 0	
	draw_area_init(&tan1, vbox30, 0, 0, 400,426);
#endif

	return 0;
}
