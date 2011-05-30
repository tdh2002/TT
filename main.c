
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include "drawfb.h"
#include "spi_d.h"

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <gdk/gdkkeysyms.h>

DRAW_UI_P	pp;
void init_group_spi (guint group);
void send_focal_spi (guint group);

/* 测试用的初始值 */
static void set_config (guint groupid)
{
	CFG(groupQty)	= 1;
	CFG(groupId)	= groupid;
	GROUP_VAL(group_mode) = PA_SCAN;
	CFG(probe_select)=0;
	GROUP_VAL(scan_offset)=0;
	GROUP_VAL(index_offset)=0;
	GROUP_VAL(skew)=0;
	GROUP_VAL(agate_start)=1000;
	GROUP_VAL(agate_width)=1000;
	CFG(part.Thickness)=100.0;
	CFG(part.Diameter)=10000.0;

	CFG(auto_program)	=	AUTO_FOCAL_ON;

	CFG(voltage_pa) = 0;
	CFG(voltage_ut) = 0;
	/* UT settings */
	GROUP_VAL(velocity)	= 592000;	/* 5920m/s */ 
	GROUP_VAL(gain)         = 10;
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
	GROUP_VAL(point_qty)	= 615;			/* 0是Auto */
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
	CFG(display_group) = DISPLAY_CURRENT_GROUP; /*0是All*/
	CFG(c_scan1)=0;   /*0是A%*/
	CFG(c_scan2)=0;   /*0是A%*/
	CFG(c_scan11)	=	C_SCAN_OFF;
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
	GROUP_VAL(ascan_envelope)	=	0; /*0 None*/
	GROUP_VAL(ascan_appearance)	=	0; /*0 Hollow*/
	GROUP_VAL(ascan_overlay)	=	0; /*0 None*/
	CFG(optimum)=0; /*0 off*/
	CFG(ratio)=0; /*0 off*/
	CFG(interpolation)=0; /*0 off*/
	CFG(fft_color)=0;/* 0 Yellow*/


	LAW_VAL (Elem_qty)		=	1;
	LAW_VAL (First_tx_elem)	=	1;
	LAW_VAL (Last_tx_elem)	=	2;
	LAW_VAL (Elem_step)		=	1;
	LAW_VAL (Wave_type)		=	LONGITUDINAL_WAVE;
	LAW_VAL (Angle_min)		=	0;
	LAW_VAL (Angle_max)		=	0;
	LAW_VAL	(Angle_step)	=	100;
	LAW_VAL (Angle_beam_skew_min)		=	0;
	LAW_VAL (Angle_beam_skew_max)		=	0;
	LAW_VAL	(Angle_beam_skew_step)		=	100;
	LAW_VAL (Focus_depth)	=	5000;
	LAW_VAL (Position_start)	=	5000;
	LAW_VAL (Position_end)	=	5000;
	LAW_VAL (Offset_start)	=	2000;
	LAW_VAL (Offset_end)	=	2000;
	LAW_VAL (Depth_start)	=	2000;
	LAW_VAL (Depth_end)	=	2000;

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
	LAW_VAL(Focal_type)	= AZIMUTHAL_SCAN;
	LAW_VAL(Focal_point_type)	= HALFPATH_P;
	LAW_VAL(Tx_connect)	= 1;
	LAW_VAL(Rx_connect)	= 1;
	LAW_VAL(Elem_qty)	= 16;
	LAW_VAL(First_tx_elem)	= 1;
	LAW_VAL(First_rx_elem)	= 1;
	LAW_VAL(Last_tx_elem)	= 17;
	LAW_VAL(Last_rx_elem)	= 17;
	LAW_VAL(Elem_step)	= 1;
	LAW_VAL(Angle_step)	= 100;
	LAW_VAL(Focus_depth)	= 50000;
	LAW_VAL(law_index_start)= 0;
	LAW_VAL(law_index_end)	= 1;

	TMP(velocity_data_p[0]) =  TMP(velocity_data[0]);
	TMP(velocity_data_p[1]) =  TMP(velocity_data[1]);
	TMP(velocity_data_p[2]) =  TMP(velocity_data[2]);

	set_part_geometry (pp->p_config, GUINT_TO_POINTER (PLATE_PART)	);

	pp->ctype_pos = 1;
}

int main (int argc, char *argv[])
{
	DRAW_UI_P		p_ui;					
	CONFIG_P		p_config;				
	TMP_CONFIG_P	p_tmp_config;					
	GtkWidget		*window;
	gint			i;

	g_thread_init(NULL);
	gdk_threads_init();
	gtk_init (&argc, &argv);

	g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));
	g_object_set (gtk_settings_get_default (), "gtk-menu-bar-accel", NULL, NULL); 

	p_ui		= (DRAW_UI_P)malloc(sizeof(DRAW_UI));
	p_config	= (CONFIG_P)malloc(sizeof(CONFIG));
	p_tmp_config	= (TMP_CONFIG_P)malloc(sizeof(TMP_CONFIG));

	if (!p_ui)	return 0;
	if (!p_config)	return 1;
	if (!p_tmp_config)	return 2;

	memset (p_ui, 0x0, sizeof(DRAW_UI));
	memset (p_config, 0x0, sizeof(CONFIG));
	memset (p_tmp_config, 0x0, sizeof(TMP_CONFIG));
	g_print ("DRAW_UI's size:%d xx = %d\n", sizeof(DRAW_UI), p_ui->mark3);
	g_print ("CONFIG's size:%d xx = %d\n", sizeof(CONFIG), p_config->time);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (window), FALSE);			/*不可以装饰*/
	gtk_window_set_default_size (GTK_WINDOW(window), 800, 600);		/*设置窗口大小*/
#if ARM
	gtk_window_fullscreen (GTK_WINDOW(window));						/*全屏幕*/
#endif
	gtk_widget_modify_bg (window, GTK_STATE_NORMAL, &color_black);	/*黑色背景*/
	g_signal_connect (G_OBJECT(window), "delete_event",
			G_CALLBACK(gtk_main_quit), NULL);			

	p_ui->disp = XOpenDisplay(NULL);
	if (p_ui->disp == NULL)
		return 1;
	p_ui->fk = fakekey_init(p_ui->disp);

	p_ui->p_config		= p_config;
	p_ui->p_tmp_config	= p_tmp_config;
	p_ui->window		= window;

	pp = p_ui;

	p_ui->p_tmp_config->fd_config = open ("default.cfg", O_RDWR);
	if (!argv[1])
	{
		set_config(0);
	} 
	else 
	{
		if (p_ui->p_tmp_config->fd_config < 0)
			g_print("error open config file\n");
		else 
		{ 
			i = read (TMP(fd_config), pp->p_config, sizeof(CONFIG));
			g_print("success open config file\n");
		}
	}

	memset (TMP(scan_type), 0xff, 16);
	TMP(beam_qty[0]) = 1;
	TMP(beam_qty[1]) = 0;
	TMP(beam_qty[2]) = 0;
	TMP(beam_qty[3]) = 0;
	TMP(beam_qty[4]) = 0;
	TMP(beam_qty[5]) = 0;
	TMP(beam_qty[6]) = 0;
	TMP(beam_qty[7]) = 0;

#if ARM
	init_fb ();					
	g_print ("fb inited\n");
	init_mem ();
	g_print ("mem inited\n");
	init_spi ();
	g_print ("spi inited\n");
	init_serial ();
	g_print ("serial inited\n");
	p_ui->p_beam_data = TMP(dma_data_add);		/* FPGA过来的数据 */

	/* 初始化要冲送给fpga的值 */
	for (i = CFG(groupQty) ; i != 0; i--)
	{
		init_group_spi (i - 1);
		write_group_data (&TMP(group_spi[i - 1]), i - 1);
		cal_focal_law(i - 1);
		send_focal_spi (i - 1);
		g_print ("group %d config init complete\n", i);
	}
#endif

	for (i = 0; i < setup_MAX_GROUP_QTY; i++)
		TMP(total_point_qty) += TMP(beam_qty[i]) * (GROUP_VAL_POS (i, point_qty) + 32);

	init_ui (p_ui);

	gtk_widget_show (window);

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();

	return 0;
}

void send_focal_spi (guint group)
{
	guint offset, beam_qty = TMP(beam_qty[group]), k, i;
	for (offset = 0, k = 0 ; k < group; k++)
		offset += TMP(beam_qty[k]);
	for (k = offset; k < beam_qty; k++)
	{
		TMP(focal_spi[group]).group	= group;
		TMP(focal_spi[group]).all_beam_info	= get_beam_qty();
		TMP(focal_spi[group]).gain_offset	= GROUP_VAL_POS(group, gain_offset);

		TMP(focal_spi[group]).beam_delay	= CFG(focal_law_all_beam[k].beam_delay) / 10;
		TMP(focal_spi[group]).rx_sel	= 
			channel_select(GROUP_VAL_POS(group, pulser) + LAW_VAL_POS(group, First_rx_elem));  
		TMP(focal_spi[group]).tx_sel	= 
			channel_select(GROUP_VAL_POS(group, receiver) + LAW_VAL_POS(group, First_tx_elem));  

		for (i = 0 ; i < CFG(focal_law_all_beam[k].N_ActiveElements); i++)
		{
			TMP(focal_spi[group]).tx_info[i]	= 
				((guint)(CFG(focal_law_all_elem[k][i].T_delay) / 2.5)) | 
				(((guint)(GROUP_VAL_POS(group, pulser_width) / 2.5)) << 16) | (0x3 << 30);	
			if (i < 16)
				TMP(focal_spi[group]).rx_info[i]	= 
					(TMP(focal_spi[group]).rx_info[i] & 0xffff0000) | 
					((guint)(CFG(focal_law_all_elem[k][i].R_delay) / 2.5)); 
			else
				TMP(focal_spi[group]).rx_info[i - 16]	= 
					(TMP(focal_spi[group]).rx_info[i] & 0x0000ffff) | 
					((guint)(CFG(focal_law_all_elem[k][i].R_delay) / 2.5) << 16); 
		}
		
		if (CFG(focal_law_all_beam[k].N_ActiveElements) < 32)
		{
			for (i = CFG(focal_law_all_beam[k].N_ActiveElements); i < 32; i++)
				TMP(focal_spi[group]).rx_info[i] &= 0x3fffffff;
		}

		write_focal_data (&TMP(focal_spi[k - 1]), k - 1);
	}
}

void init_group_spi (guint group)
{
	gint tmp = 0;
	if (GROUP_VAL_POS(group, filter) == 0)
	{
		TMP(group_spi[group]).freq_band	= 0;
	}
	else if (GROUP_VAL_POS(group, filter) == 1)
	{
		if (GROUP_VAL_POS(group, frequency) < 1250)
			TMP(group_spi[group]).freq_band	= 1;
		else if (GROUP_VAL_POS(group, frequency) < 1750)
			TMP(group_spi[group]).freq_band	= 2;
		else if (GROUP_VAL_POS(group, frequency) < 2125)
			TMP(group_spi[group]).freq_band	= 3;
		else if (GROUP_VAL_POS(group, frequency) < 3125)
			TMP(group_spi[group]).freq_band	= 4;
		else if (GROUP_VAL_POS(group, frequency) < 4500)
			TMP(group_spi[group]).freq_band	= 5;
		else if (GROUP_VAL_POS(group, frequency) < 6250)
			TMP(group_spi[group]).freq_band	= 6;
		else if (GROUP_VAL_POS(group, frequency) < 8750)
			TMP(group_spi[group]).freq_band	= 7;
		else if (GROUP_VAL_POS(group, frequency) < 11000)
			TMP(group_spi[group]).freq_band	= 8;
		else if (GROUP_VAL_POS(group, frequency) < 13500)
			TMP(group_spi[group]).freq_band	= 9;
		else if (GROUP_VAL_POS(group, frequency) < 17500)
			TMP(group_spi[group]).freq_band	= 10;
		else 
			TMP(group_spi[group]).freq_band	= 11;
	}
	else
		TMP(group_spi[group]).freq_band	= GROUP_VAL_POS(group, filter) - 1;
	TMP(group_spi[group]).video_filter	= GROUP_VAL_POS(group, video_filter);
	TMP(group_spi[group]).rectifier		= GROUP_VAL_POS(group, rectifier);
	TMP(group_spi[group]).compress_rato	=
		((GROUP_VAL_POS(group, range) / 10.0) / GROUP_VAL_POS(group, point_qty)) > 1 ? 
		((GROUP_VAL_POS(group, range) / 10.0) / GROUP_VAL_POS(group, point_qty)) : 1;
	TMP(group_spi[group]).gain			= GROUP_VAL_POS(group, gain) / 10;
	g_print ("gain=%d\n", TMP(group_spi[group]).gain);

	TMP(group_spi[group]).tcg_point_qty	= 0;		/* 未完成 */
	TMP(group_spi[group]).tcg_en		= 0;		/* 未完成 */
	TMP(group_spi[group]).UT2			= 0;		/* 未完成 */
	TMP(group_spi[group]).UT1			= 0;		/* 未完成 */
	TMP(group_spi[group]).PA			= GROUP_VAL_POS (group, group_mode);		
	TMP(group_spi[group]).sample_start	= 0;		/* 未完成 */

	TMP(group_spi[group]).sum_gain		= 4 * pow (10, GROUP_VAL_POS (group, sum_gain) / 10.0);	
	TMP(group_spi[group]).sample_range	= 0;

	TMP(group_spi[group]).beam_qty		= TMP (beam_qty[group]);	
	TMP(group_spi[group]).sample_offset	= 0;

	TMP(group_spi[group]).rx_time		= 0;
	TMP(group_spi[group]).gain1			= 0;

	TMP(group_spi[group]).idel_time		= 0;

	TMP(group_spi[group]).gate_a_height	= GROUP_VAL_POS(group, gate[0].height);
	TMP(group_spi[group]).gate_a_start	= GROUP_VAL_POS(group, gate[0].start) / 10;
	
	if (GROUP_VAL_POS(group, gate[0].synchro) == 0)
		tmp = (tmp & 0xfffffff3) | 0x00;
	else if (GROUP_VAL_POS(group, gate[0].synchro) == 1)
		tmp = (tmp & 0xfffffff3) | 0x0c;

	if (GROUP_VAL_POS(group, gate[0].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[0].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	TMP(group_spi[group]).gate_a_logic	= tmp;	
	/* 0-1 表示测量选择 00 波前 Edge 01 波峰 Peak 
	 * 2-3 表示同步选择 00 发射同步Pulse 01 AGate 10 BGate 11 IGATE
	 * */
	TMP(group_spi[group]).gate_a_end	= (GROUP_VAL_POS(group, gate[0].start) + 
		GROUP_VAL_POS (group, gate[0].width)) / 10;

	TMP(group_spi[group]).gate_b_height	= GROUP_VAL_POS(group, gate[1].height) * 40.96;
	TMP(group_spi[group]).gate_b_start	= GROUP_VAL_POS(group, gate[1].start) / 10;

	if (GROUP_VAL_POS(group, gate[1].synchro) == 0)
		tmp = (tmp & 0xfffffff3) | 0x00;
	else if (GROUP_VAL_POS(group, gate[1].synchro) == 1)
		tmp = (tmp & 0xfffffff3) | 0x0c;
	else if (GROUP_VAL_POS(group, gate[1].synchro) == 2)
		tmp = (tmp & 0xfffffff3) | 0x04;

	if (GROUP_VAL_POS(group, gate[1].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[1].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	TMP(group_spi[group]).gate_b_logic	= tmp;
	TMP(group_spi[group]).gate_b_end	= (GROUP_VAL_POS(group, gate[1].start) + 
		GROUP_VAL_POS (group, gate[1].width)) / 10;

	TMP(group_spi[group]).gate_i_height	= GROUP_VAL_POS(group, gate[2].height);
	TMP(group_spi[group]).gate_i_start	= GROUP_VAL_POS(group, gate[2].start) / 10;

	if (GROUP_VAL_POS(group, gate[2].synchro) == 0)
		tmp = (tmp & 0xfffffff3) | 0x00;

	if (GROUP_VAL_POS(group, gate[2].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[2].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	TMP(group_spi[group]).gate_i_logic	= tmp;	
	TMP(group_spi[group]).gate_i_end	= (GROUP_VAL_POS(group, gate[2].start) + 
		GROUP_VAL_POS (group, gate[2].width)) / 10;

	TMP(group_spi[group]).reject = CFG(reject) * 40.95;	

	if (GROUP_VAL_POS (group, group_mode) == UT_SCAN)
		TMP(group_spi[group]).voltage = CFG(voltage_ut);	
	else if (GROUP_VAL_POS (group, group_mode) == PA_SCAN)
	{	
		if CFG(voltage_pa)
			TMP(group_spi[group]).voltage = 0x2;	
		else
			TMP(group_spi[group]).voltage = 0x0;	
	}

}
