
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
static void set_config (guint groupid)
{
	CFG(groupQty)	= 1;
	CFG(groupId)	= groupid;
	CFG(voltage_pa) = 0;
	CFG(voltage_ut) = 0;
	/* UT settings */
	GROUP_VAL(velocity) = 592000;	/* 5920m/s */ 
	GROUP_VAL(gain)         = 0;
	GROUP_VAL(gainr)        = 0;
	GROUP_VAL(db_ref)	= GAINR_OFF;
	GROUP_VAL(wedge_delay)  = 0;	/*  */
	GROUP_VAL(range)        = 10000;		/* 10μs */
	GROUP_VAL(start)        = 0.0;
	GROUP_VAL(pulser)       = 1;			/* 1表示第一个探头接口 1-128 */
	GROUP_VAL(receiver)     = 1;			/* 1表示第一个探头接口 1-128 */
	GROUP_VAL(tx_rxmode)	= PULSE_ECHO;	/* 收发模式 */
	GROUP_VAL(freq_pos)		= 0;			/* 0是1Mhz	*/
	GROUP_VAL(frequence)	= 1000;			/* 频率 */
	GROUP_VAL(pw_pos)		= 0;			/* 0是Atuo	*/
	GROUP_VAL(prf_pos)		= 0;			/* 0是Atuo Max*/
	GROUP_VAL(pulser_width)	= 3000;			/* 脉冲宽度 30ns */
	GROUP_VAL(prf)			= 600;			/* 重复频率 60*/
	GROUP_VAL(filter)		= 0;			/* 接收滤波器 0是None */
	GROUP_VAL(rectifier)	= FULL_WAVE;	/* 检波 */
	GROUP_VAL(averaging)	= 1;	        /* 位置1 值为2*/
	GROUP_VAL(video_filter)	= VIDEO_FILTER_OFF;
	GROUP_VAL(point_qty)	= 100;			/* 0是Auto */
	GROUP_VAL(sum_gain)	= 10;			/* 0是Auto */
	GROUP_VAL(gate_pos)	= GATE_A;		  
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].start) = 0;  
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].width) = 10000;  
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].parameters) = 0;
	GROUP_VAL(gate[GROUP_VAL(gate_pos)].synchro) = 0;  
	CFG(alarm_pos)	= 0;                             /* alarm的值，1～16 */
	CFG(alarm[CFG(alarm_pos)].groupa) = 0;           /* alarm中Group A 的值，1～4 */
	CFG(alarm[CFG(alarm_pos)].conditiona) = 0;       /* alarm中Condition A 的值，0是 NONE */
	CFG(alarm[CFG(alarm_pos)].operat) = 0;		 /* alarm中Operator 的值，AND OR */
	CFG(alarm[CFG(alarm_pos)].groupb) = 0;		 /* alarm中Group B 的值，1～4 */
	CFG(alarm[CFG(alarm_pos)].conditionb) = 0;	 /* alarm中Condition B 的值，0是 NONE */
	CFG(output_pos) = 0;				 /* output 的值，1～5 */
	CFG(output[CFG(output_pos)].alarm1) = 0;
	CFG(output[CFG(output_pos)].alarm1_status) = 0;
	CFG(output[CFG(output_pos)].alarm1_qty) = 0;
	CFG(analog[CFG(output_pos)].group) = 0;          /* output中group 的值，0是 1*/
	CFG(analog[CFG(output_pos)].data) = 0;           /* output中data 的值，0是 Off*/
	GROUP_VAL(mode_pos)= 0;				 /* 0是Setup */
	GROUP_VAL(curve_pos)= 0;			/* 0是NOne */
	GROUP_VAL(ref_ampl)=10; 
	GROUP_VAL(ref_ampl_offset)=10; 
	GROUP_VAL(curve_step)=10; 
	GROUP_VAL(ref_gain)=10;
	GROUP_VAL(point_pos)=0;
	GROUP_VAL(amplitude)=1000;
	GROUP_VAL(position)=1000; 
	GROUP_VAL(mat_atten)=1000; 
	GROUP_VAL(delay)=1000; 
	GROUP_VAL(tcg_gain)=500;

	/* 探头信息 */
	GROUP_VAL(probe.Elem_qty)	= 32; /*  */
/*	memcpy (GROUP_VAL(probe.Name), "5L64-A2", 8);*/ /* 探头名字 */
	/* 聚焦法则信息 */
	LAW_VAL(Focal_type)	= LINEAR_SCAN;
	LAW_VAL(Tx_connect)	= 1;
	LAW_VAL(Rx_connect)	= 1;
	LAW_VAL(Elem_qty)	= 16;
	LAW_VAL(First_tx_elem)	= 1;
	LAW_VAL(First_rx_elem)	= 1;
	LAW_VAL(Last_tx_elem)	= 17;
	LAW_VAL(Last_rx_elem)	= 17;
	LAW_VAL(Elem_step)		= 1;
	LAW_VAL(Angle_start)	= 0;
	LAW_VAL(Angle_end)		= 0;
	LAW_VAL(Angle_step)		= 100;
	LAW_VAL(Focus_depth)	= 50000;
	LAW_VAL(law_index_start)= 0;
	LAW_VAL(law_index_end)	= 1;

	/*  */
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
#if ARM
	gtk_window_fullscreen (GTK_WINDOW(window));						/*全屏幕*/
#endif
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
	set_config(0);
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
