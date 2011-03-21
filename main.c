
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include "drawfb.h"

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <gdk/gdkkeysyms.h>

GdkColor	color_black     = {0x0, 0x0, 0x0, 0x0};
GdkColor	color_black1    = {0x0, 0x0, 0x0, 0x0800};
GdkColor	color_white     = {0x0, 0xffff, 0xffff, 0xffff};
GdkColor	color_yellow    = {0x0, 0xffff, 0xffff, 0x0};
GdkColor	color_green     = {0x0, 0x0, 0xffff, 0x0};
GdkColor	color_blue      = {0x0, 0x0, 0x0, 0xffff};
GdkColor	color_red       = {0x0, 0x0, 0x0, 0xffff};
GdkColor	color_text_base = {0x0, 0x1300, 0x4900, 0x7600};
GdkColor	color_rule      = {0x0, 0xc300, 0xf000, 0x1d00};

GdkColor	all_col[] = 
{
	{0x0, 0x0, 0x0, 0xffff},	/* blue */
	{0x0, 0x0, 0xffff, 0x0}, /* green*/
	{0x0, 0xffff, 0x0, 0x0},		/* red */
	{0x0, 0xffff, 0xffff, 0x0}, /* yellow */
	{0x0, 0x0, 0x0, 0x0},	/* black */
	{0x0, 0xffff, 0xffff, 0xffff} /* white */
};

gushort	all_col_16[] =
{
	0x001f,
	0x03e0,
	0xf800,
	0xffe0,
	0x0000,
	0xffff
};

DRAW_UI_P	pp;					

/* 测试用的初始值 */
static void set_config (guint groupid)
{
	CFG(groupQty)	= 1;
	CFG(groupId)	= groupid;
	CFG(group_pos)	= 1;
	GROUP_VAL(group_mode) =0;
	CFG(probe_select)=0;
	GROUP_VAL(scan_offset)=0;
	GROUP_VAL(index_offset)=0;
	GROUP_VAL(skew)=0;
	GROUP_VAL(agate_start)=1000;
	GROUP_VAL(agate_width)=1000;
	CFG(part.Thickness)=100.0;
	CFG(part.Diameter)=10000.0;

	CFG(auto_program)=0;

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
	GROUP_VAL(frequency)	= 5000;			/* 频率 */
	GROUP_VAL(pw_pos)		= 0;			/* 0是Atuo	*/
	GROUP_VAL(prf_pos)		= 0;			/* 0是Atuo Max*/
	GROUP_VAL(pulser_width)	= 10000;		/* 脉冲宽度 30ns */
	GROUP_VAL(prf)			= 600;			/* 重复频率 60*/
	GROUP_VAL(filter)		= 0;			/* 接收滤波器 0是None */
	GROUP_VAL(rectifier)	= FULL_WAVE;	/* 检波 */
	GROUP_VAL(averaging)	= 1;	        /* 位置1 值为2*/
	GROUP_VAL(video_filter)	= VIDEO_FILTER_OFF;
	GROUP_VAL(point_qty)	= 160;			/* 0是Auto */
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
	GROUP_VAL(selection)=0;   /*0 是 A-scan*/
	GROUP_VAL(per_reference)=1000.0;
	GROUP_VAL(per_measure)=1000.0;
	GROUP_VAL(u_reference)=1000.0;
	GROUP_VAL(u_measure)=1000.0;
	GROUP_VAL(s_reference)=1000.0;
	GROUP_VAL(s_measure)=100.0;
	GROUP_VAL(data_link)=0;  /* 0是none */

	GROUP_VAL(probe.Model[0]) = 32;

	CFG(display_table)=0;  /*0是OFF*/
	CFG(entry_image)=0;    /*0是OFF*/
	GROUP_VAL(source)=0;   /*0是A^ */
	CFG(min_thickness)=50;
	CFG(max_thickness)=99900;
	CFG(echo_qty)=1;
	CFG(dis_group)=0; /*0是All*/
	CFG(c_scan1)=0;   /*0是A%*/
	CFG(c_scan2)=0;   /*0是A%*/
	CFG(c_scan11)=0;  /*0是A%*/
	CFG(data1)=0;     /*0是A%*/
	CFG(data2)=0;     /*0是A%*/
	CFG(dis_mode)=0;  /* 0是 All & A-Scan */
	CFG(dis_range)=100.0;
	CFG(avg_scan_speed)=1000.0;
	GROUP_VAL(ut_unit)=0;  /* 0 是 Sound Path */
	CFG(grid)=5;     /*5是 Off*/
	CFG(sizing_curves)=0;
	CFG(overlay_gate)=0;
	CFG(overlay_cursor)=0;
	CFG(overlay_overlay)=0;


	CFG(zoom_display_pos)=0;
	CFG_ZOOM_POS(zoom_type)=0;
	CFG_ZOOM_POS(start_usound)=1000;
	CFG_ZOOM_POS(end_usound)=1000;
	CFG_ZOOM_POS(range_usound)=1000;
	CFG_ZOOM_POS(center_usound)=1000;
	CFG_ZOOM_POS(start_amplitude)=10;
	CFG_ZOOM_POS(end_amplitude)=10;
	CFG_ZOOM_POS(range_amplitude)=10;
	CFG_ZOOM_POS(center_amplitude)=10;

	/**/
	GROUP_VAL(col_select_pos)=0;  /*0 Amplitude*/
	GROUP_VAL(col_start)   = 0.0;
	GROUP_VAL(col_end)     = 100.0;
	GROUP_VAL(col_contrast)= 0.0;
	GROUP_VAL(col_brightness)    = 50.0;
	GROUP_VAL(col_min)           = 5000.0;
	GROUP_VAL(col_max)           = 50000.0;
	GROUP_VAL(col_mode)    = 0;  /*0 Exclusion*/

	CFG(prop_scan)=0;/*0 A-Scan*/
	CFG(envelope)=0; /*0 None*/
	CFG(prop_app)=0; /*0 Hollow*/
	CFG(prop_overlay)=0; /*0 None*/
	CFG(optimum)=0; /*0 off*/
	CFG(ratio)=0; /*0 off*/
	CFG(interpolation)=0; /*0 off*/
	CFG(fft_color)=0;/* 0 Yellow*/

	/* focal law */
	LAW_VAL(Focal_type)	= ANGLE_SCAN;

	GROUP_VAL(element_qty)=1;
	GROUP_VAL(first_element)=1;
	GROUP_VAL(last_element)=1;
	GROUP_VAL(element_step)=1;
	GROUP_VAL(wave_type)=0;	/* 0纵波 与 1横波 */
	GROUP_VAL(min_angle)=0.0;
	GROUP_VAL(max_angle)=100.0;
	GROUP_VAL(angle_step)=100.0;
	GROUP_VAL(focus_depth)=5000.0;

	CFG(polarity)=0;  /* 0 Normal */
	CFG(e_type)=0;  /* 0 Clock/Dir */
	CFG(encoder_resolution)=1000;
	CFG(origin)=0;
	CFG(i_type)=0;  /* 0  One-Line Scan */
	CFG(i_scan)=0;  /* 0 Time */
	CFG(i_index)=1;  /* 1   Encoder 1 */
	CFG(scanspeed)=10.0;
	CFG(scanspeed_rpm)=10.0;
	CFG(indexspeed)=10.0;
	CFG(scan_start)=0.0;
	CFG(scan_end)=346.0;
	CFG(scan_resolution)=100.0;
	CFG(index_start)=1.0;
	CFG(index_end)=1.0;
	CFG(index_resolution)=100.0;

	CFG(start_mode)=0; /*0 Reset All*/
	CFG(pause)=0; 		/*0 off*/
	CFG(storage)=0;   /* 0 Last */

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
	LAW_VAL(Elem_step)	= 1;
	LAW_VAL(Angle_start)	= 0;
	LAW_VAL(Angle_end)	= 0;
	LAW_VAL(Angle_step)	= 100;
	LAW_VAL(Focus_depth)	= 50000;
	LAW_VAL(law_index_start)= 0;
	LAW_VAL(law_index_end)	= 1;

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
	gint			i;
	gint			tt;

	g_thread_init(NULL);
	gdk_threads_init();
	gtk_init (&argc, &argv);

	g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));
	g_object_set (gtk_settings_get_default (), "gtk-menu-bar-accel", NULL, NULL); 

	accel = gtk_accel_group_new();

	p_ui		= (DRAW_UI_P)malloc(sizeof(DRAW_UI));
	p_config	= (CONFIG_P)malloc(sizeof(CONFIG));
	p_tmp_config	= (TMP_CONFIG_P)malloc(sizeof(TMP_CONFIG));

	if (!p_ui)	return 0;
	if (!p_config)	return 1;
	if (!p_tmp_config)	return 2;

	memset (p_ui, 0x0, sizeof(DRAW_UI));
	memset (p_config, 0x0, sizeof(CONFIG));
	memset (p_tmp_config, 0x0, sizeof(TMP_CONFIG));
	g_print("DRAW_UI's size:%d xx = %d\n", sizeof(DRAW_UI), p_ui->mark3);
	g_print("CONFIG's size:%d xx = %d\n", sizeof(CONFIG), p_config->time);

	p_ui->p_beam_data = (guint)malloc(MAX_DOT_QTY * 2);		/* FPGA过来的数据 */
	for (i = 0; i < 4; i++);

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
	pp->a_scan_width = 615;
	pp->a_scan_height = 120;
	for (i = 0; i < 20480; i++)
	{
		tt = i % 511;
		if ( tt > 255)
			*(DOT_TYPE *)(pp->p_beam_data + i) = 511 - tt;
		else 
			*(DOT_TYPE *)(pp->p_beam_data + i) = tt;
//		g_print ("%d = %d\n", i, 
//				*(DOT_TYPE *)(pp->p_beam_data + i));
	}
#if ARM
	init_fb (); /* 初始化fb1 */
#endif
	init_ui (p_ui);

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
