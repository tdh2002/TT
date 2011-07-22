
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include "drawfb.h"
#include "spi_d.h"
#include "file_op.h"

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
	gint i;
	set_group_qty (pp->p_config, 1);
	set_current_group (pp->p_config, groupid);
	GROUP_VAL(group_mode) = PA_SCAN;
	set_probe_select (pp->p_config, CHOOSE_PROBE);
	set_probe_fft (pp->p_config, NORMAL_OFF);
	GROUP_VAL(scan_offset)=0;
	GROUP_VAL(index_offset)=0;
	GROUP_VAL(skew)=0;
	GROUP_VAL(agate_start)=1000;
	GROUP_VAL(agate_width)=1000;

	set_part_thickness (pp->p_config, GUINT_TO_POINTER (100));
	set_part_diameter (pp->p_config, GUINT_TO_POINTER (1000));
	set_auto_detect (pp->p_config, NORMAL_OFF);

	set_auto_focal (pp->p_config, NORMAL_ON);

	set_voltage (pp->p_config, get_current_group(pp->p_config), VOLTAGE_LOW);
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
	GROUP_VAL(prf_pos)		= 3;			/* 0是Atuo Max*/
	GROUP_VAL(pulser_width)	= 10000;		/* 脉冲宽度 30ns */
	GROUP_VAL(prf)			= 200;			/* 重复频率 60*/
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

	set_alarm_pos (pp->p_config, 0);
	set_alarm_groupa (pp->p_config, groupid);
	set_alarm_conditiona (pp->p_config, ALARM_NONE);
	set_alarm_operator (pp->p_config, OPERATOR_AND);
	set_alarm_groupb (pp->p_config, groupid);
	set_alarm_conditionb (pp->p_config, ALARM_NONE);

	set_output_pos (pp->p_config, OUTPUT1);
	for (i = 1; i < 17; i++) 
		set_output_alarm (pp->p_config, ALARM_OFF, i);

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

	set_display_table (pp->p_config, NORMAL_OFF);
	set_entry_image (pp->p_config, NORMAL_OFF);
	set_cur_entry (pp->p_config, 0);
	set_entry_qty (pp->p_config, 10);
	GROUP_VAL(source)=0;   /*0是A^ */
	set_min_thickness (pp->p_config, 50);
	set_max_thickness (pp->p_config, 99900);
	set_echo_qty (pp->p_config, 1);
	set_display_pos (pp->p_config, C_SCAN);
	set_cscan_source (pp->p_config, C_SCAN_A_HEIGHT, 0);

	set_display_pos (pp->p_config, S_SCAN);
 	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	set_display_pos (pp->p_config, A_S_CC_SCAN);
 	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	set_cscan_source (pp->p_config, C_SCAN_OFF, 0);

	set_display_pos (pp->p_config, A_C_CC_SCAN);
 	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	set_cscan_source (pp->p_config, C_SCAN_A_HEIGHT, 0);
	set_cscan_source (pp->p_config, C_SCAN_OFF, 1);

	set_display_pos (pp->p_config, A_SCAN);
 	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/

	set_stripscan_data1 (pp->p_config, STRIP_SCAN_A_HEIGHT);
	set_stripscan_data2 (pp->p_config, STRIP_SCAN_A_HEIGHT);
	set_stripscan_mode (pp->p_config, STRIP_SCAN_ALL_ASCAN);
	set_stripscan_disrange (pp->p_config, 100);
	GROUP_VAL(ut_unit)=0;  /* 0 是 Sound Path */
	set_overlay_grid(pp->p_config, 5);     /*5是 Off*/
	set_overlay_sizing_curves (pp->p_config, NORMAL_OFF);
	set_overlay_gate (pp->p_config, NORMAL_OFF);
	set_overlay_cursor (pp->p_config, NORMAL_OFF);
	set_overlay_overlay (pp->p_config, NORMAL_OFF);

	/**/
	GROUP_VAL(col_select_pos)=0;  /*0 Amplitude*/
	GROUP_VAL(col_start)   = 0.0;
	GROUP_VAL(col_end)     = 100.0;
	GROUP_VAL(col_contrast)= 0.0;
	GROUP_VAL(col_brightness)    = 50.0;
	GROUP_VAL(col_min)           = 5000.0;
	GROUP_VAL(col_max)           = 50000.0;
	GROUP_VAL(col_mode)    = 0;  /*0 Exclusion*/

	set_dis_prop_scan (pp->p_config, DIS_PROP_SCAN_A);/*0 A-Scan*/
	GROUP_VAL(ascan_envelope)	=	0; /*0 None*/
	GROUP_VAL(ascan_appearance)	=	0; /*0 Hollow*/
	GROUP_VAL(ascan_overlay)	=	0; /*0 None*/
	set_dis_prop_boptimum (pp->p_config, NORMAL_OFF);
	set_dis_prop_cratio (pp->p_config, NORMAL_OFF);
	set_dis_prop_sinterpolation (pp->p_config, NORMAL_OFF);
	set_dis_prop_strip_orientation (pp->p_config, NORMAL_OFF);

	set_fft_color (pp->p_config, FFT_YELLOW); /* 0 Yellow*/

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

	set_cur_encoder (pp->p_config, ENCODER_1);
	set_enc_polarity (pp->p_config, ENC_NORMAL, get_cur_encoder (pp->p_config));
	set_enc_type (pp->p_config, ENC_CLOCK_DIR, get_cur_encoder (pp->p_config));
	set_enc_resolution (pp->p_config, 1000, get_cur_encoder (pp->p_config));
	set_enc_origin (pp->p_config, 0, get_cur_encoder (pp->p_config));

	set_inspec_type (pp->p_config, TYPE_ONE_LINE);
	set_inspec_source (pp->p_config, SCAN_TIME);
	set_inspec_index (pp->p_config, INDEX_AUTO);
	set_inspec_speed (pp->p_config, 10);
	set_inspec_rpmspeed (pp->p_config, 10);
	set_inspec_indexspeed (pp->p_config, 10);

	set_area_scanstart (pp->p_config, 0);
	set_area_scanend (pp->p_config, 346);
	set_area_scanresolution (pp->p_config, 100);
	set_area_indexstart (pp->p_config, 1);
	set_area_indexend (pp->p_config, 1);
	set_area_indexresolution (pp->p_config, 100);

	set_start_mode (pp->p_config, RESET_ALL);
	set_start_pause (pp->p_config, NORMAL_OFF);

	set_data_storage (pp->p_config, STORAGE_LAST);
	set_data_inspec_data (pp->p_config, INSPEC_DATA_ALL_A_C);

	set_file_storage (pp->p_config, STORAGE_CARD);
	set_file_save_mode (pp->p_config, SAVE_MODE_INSPEC_DATA);

	set_report_format_userfield (pp->p_config, NORMAL_OFF);
	set_report_format_probe (pp->p_config, NORMAL_OFF);
	set_report_format_setup (pp->p_config, NORMAL_OFF);
	set_report_format_note (pp->p_config, NORMAL_OFF);
	set_report_format_view (pp->p_config, FILE_VIEW_TABLE);

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
 	pp->cmode_pos = 2;
	pp->cstart_qty = 1;

	set_report_template (pp->p_config, REPORT_COMPLETE);
	set_report_paper_size (pp->p_config, PAPER_A4);
	GROUP_GATE_POS(height) = 20; /*闸门默认高度为20*/

}

int main (int argc, char *argv[])
{
	DRAW_UI_P		p_ui;					
	CONFIG_P		p_config;				
	TMP_CONFIG_P	p_tmp_config;					
	GtkWidget		*window;
	GdkColor		color_black_     = {0x0, 0x0, 0x0, 0x0};
	gint			i;

	g_thread_init(NULL);
	gdk_threads_init();
	gtk_init (&argc, &argv);

	g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));
	g_object_set (gtk_settings_get_default (), "gtk-menu-bar-accel", NULL, NULL); 

	/* 封装后需要条用函数初始化 */
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
	g_print ("CONFIG's size:%d \n", sizeof(CONFIG));
	g_print ("GROUP size:%d \n", sizeof(GROUP));

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (window), FALSE);			/*不可以装饰*/
	gtk_window_set_default_size (GTK_WINDOW(window), 800, 600);		/*设置窗口大小*/
#if ARM
	gtk_window_fullscreen (GTK_WINDOW(window));						/*全屏幕*/
#endif
	gtk_widget_modify_bg (window, GTK_STATE_NORMAL, &color_black_);	/*黑色背景*/
	g_signal_connect (G_OBJECT(window), "delete_event",	G_CALLBACK(gtk_main_quit), NULL);

	/* 把屏幕按键函数改成内核驱动代发 */
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
	/* 读取颜色 amp toft depth */
	read_palette_file ("source/system/Sample/Palette/ONDT_Amplitude.pal",
			TMP(t_special_col), TMP(t_color));  /*   */
	if (TMP(t_special_col[0]) != 0x12345678)
	{
		TMP(special_col_amp[0]) = COL_24_TO_16(TMP(t_special_col)[0]);
		TMP(special_col_amp[1]) = COL_24_TO_16(TMP(t_special_col)[1]);
		TMP(special_col_amp[2]) = COL_24_TO_16(TMP(t_special_col)[2]);
	}
	for (i = 0; i < 256; i ++)
		TMP(color_amp[i]) = COL_24_TO_16(TMP(t_color)[i]);

	read_palette_file ("source/system/Sample/Palette/ONDT_RFTOFD.pal",
			TMP(t_special_col), TMP(t_color));  /*   */
	if (TMP(t_special_col[0]) != 0x12345678)
	{
		TMP(special_col_tofd[0]) = COL_24_TO_16(TMP(t_special_col)[0]);
		TMP(special_col_tofd[1]) = COL_24_TO_16(TMP(t_special_col)[1]);
		TMP(special_col_tofd[2]) = COL_24_TO_16(TMP(t_special_col)[2]);
	}
	for (i = 0; i < 256; i ++)
		TMP(color_tofd[i]) = COL_24_TO_16(TMP(t_color)[i]);

	read_palette_file ("source/system/Sample/Palette/ONDT_Corrosion.pal",
			TMP(t_special_col), TMP(t_color));  /*   */
	if (TMP(t_special_col[0]) != 0x12345678)
	{
		TMP(special_col_depth[0]) = COL_24_TO_16(TMP(t_special_col)[0]);
		TMP(special_col_depth[1]) = COL_24_TO_16(TMP(t_special_col)[1]);
		TMP(special_col_depth[2]) = COL_24_TO_16(TMP(t_special_col)[2]);
	}
	for (i = 0; i < 256; i ++)
		TMP(color_depth[i]) = COL_24_TO_16(TMP(t_color)[i]);



	memset (TMP(scan_type), 0xff, 16);
	TMP(beam_qty[0]) = 1;
	TMP(beam_qty[1]) = 0;
	TMP(beam_qty[2]) = 0;
	TMP(beam_qty[3]) = 0;
	TMP(beam_qty[4]) = 0;
	TMP(beam_qty[5]) = 0;
	TMP(beam_qty[6]) = 0;
	TMP(beam_qty[7]) = 0;

	TMP(range_step_min) = ((gint)(GROUP_VAL(point_qty) * 10)+ 5) / 10 * 10;

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
	for (i = get_group_qty(pp->p_config) ; i != 0; i--)
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
	guint offset, beam_qty = TMP(beam_qty[group]), k, i,enablet = 0, enabler = 0;
	guint tmp,index,channel_index_num,cnt;
	for (offset = 0, k = 0 ; k < group; k++)
		offset += TMP(beam_qty[k]);
	for (k = offset; k < beam_qty; k++)
	{   //g_print("k=%d\n",k);
		TMP(focal_spi[k]).group	= group;
		TMP(focal_spi[k]).all_beam_info	= get_beam_qty() - 1;
		TMP(focal_spi[k]).gain_offset	= GROUP_VAL_POS(group, gain_offset);

		TMP(focal_spi[k]).beam_delay	= TMP(focal_law_all_beam[k].G_delay) / 10;
		/*UT Settings->Pulser->Tx/Rx mode*/		
		if (GROUP_VAL(tx_rxmode) == PULSE_ECHO )//单个探头收发模式
		{  
			GROUP_VAL_POS(group, receiver) = GROUP_VAL_POS(group, pulser);
			TMP(focal_spi[k]).rx_sel	= 
				channel_select((guint)(GROUP_VAL_POS(group, pulser))+ (guint)(LAW_VAL_POS(group, First_tx_elem))-1 ); //何凡修改 
			TMP(focal_spi[k]).tx_sel	= 
				channel_select((guint)(GROUP_VAL_POS(group, receiver))+ (guint)(LAW_VAL_POS(group, First_tx_elem))-1 );//何凡修改
//g_print("TMP(focal_spi[k]).rx_sel = %x TMP(focal_spi[k]).tx_sel = %x\n",TMP(focal_spi[k]).rx_sel,TMP(focal_spi[k]).tx_sel);
			if(LAW_VAL_POS(group, Focal_type) == 1)
				tmp = (guint) (GROUP_VAL_POS(group, receiver))+(guint)(LAW_VAL_POS(group, First_tx_elem)+k*LAW_VAL_POS(group, Elem_step))-1 ;
			else
				tmp = (guint) (GROUP_VAL_POS(group, receiver))+(guint)(LAW_VAL_POS(group, First_tx_elem))-1 ;
			channel_index_num = (tmp % 32 == 0)? 31 : (tmp % 32 -1); 
			cnt = pow(2,LAW_VAL_POS(group, Elem_qty))-1; 
			for (i = 0;i < TMP(focal_law_all_beam[k].N_ActiveElements); i++)
			{
				index = (channel_index_num + i) & 0x0000001f; 
				TMP(focal_spi[k]).tx_info[index]	= 
					(4 + (guint)(TMP(focal_law_all_elem[k][index].T_delay) / 2.5)) | 
					((4 + (guint)(TMP(focal_law_all_elem[k][index].T_delay) / 2.5)) +
					((guint)(GROUP_VAL_POS(group, pulser_width) / (2.5*100)))) << 16;// | (0x3 << 30);//何凡修改	
				//enablet = enablet | (1 << (LAW_VAL_POS(group, First_tx_elem) + i - 1));
				//enabler = enabler | (1 << (LAW_VAL_POS(group, First_rx_elem) + i - 1));
//g_printf("TMP(focal_spi[%d]).tx_info[%d]=%x\n",k,index,TMP(focal_spi[k]).tx_info[index]);
				if (index < 16)
					TMP(focal_spi[k]).rx_info[index]	= 
						(TMP(focal_spi[group]).rx_info[index] & 0xffff0000) | 
						((guint)(TMP(focal_law_all_elem[k][index].R_delay) / 2.5)); 
				else
					TMP(focal_spi[k]).rx_info[index - 16]	= 
						(TMP(focal_spi[group]).rx_info[index - 16] & 0x0000ffff) | 
						((guint)(TMP(focal_law_all_elem[k][index - 16].R_delay) / 2.5) << 16);
				// g_print("index=%d \n",index);
			}		
			enablet = (cnt >> (32-channel_index_num)) | (cnt<<channel_index_num);//循环左移channel_index_num位 ，使能控制
			enabler = (cnt >> (32-channel_index_num)) | (cnt<<channel_index_num);//循环左移channel_index_num位 ，使能控制
//			g_print("enablet = %x enabler = %x\n",enablet,enabler);
		
		}	
		else //其他模式
		{

		}		
		TMP(focal_spi[k]).tx_enable	= enablet;
		TMP(focal_spi[k]).rx_enable	= enabler;
		
		if (TMP(focal_law_all_beam[k].N_ActiveElements) < 32)
		{
			for (i = TMP(focal_law_all_beam[k].N_ActiveElements); i < 32; i++)
				TMP(focal_spi[k]).rx_info[i] &= 0x3fffffff;
		}

		write_focal_data (&TMP(focal_spi[k]), k);
	}
}

/* 初始化需要发给fpga的group参数 */
void init_group_spi (guint group)
{
	gint tmp = 0, tt[4];
	gint temp_prf;
	get_prf();
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

	TMP(group_spi[group]).tcg_point_qty	= 0;		/* 未完成 */
	TMP(group_spi[group]).tcg_en		= 0;		/* 未完成 */
	TMP(group_spi[group]).UT2			= (GROUP_VAL_POS (group, group_mode) == 2) ? 1 : 0;		
	TMP(group_spi[group]).UT1			= (GROUP_VAL_POS (group, group_mode) == 0) ? 1 : 0;		
	TMP(group_spi[group]).PA			= (GROUP_VAL_POS (group, group_mode) == 1) ? 1 : 0;		
	TMP(group_spi[group]).sample_start	= (GROUP_VAL_POS (group, start) + 
		GROUP_VAL_POS(group, wedge_delay)) / 10;		

//	if (GROUP_VAL_POS(group, probe.Elem_qty) == 1)	
	if (LAW_VAL_POS(group, Elem_qty) == 1)	
		TMP(group_spi[group]).sum_gain	= 4095;	
	else 
		TMP(group_spi[group]).sum_gain	= 
			4096 / GROUP_VAL_POS(group, probe.Elem_qty);	
	TMP(group_spi[group]).sample_range	= TMP(group_spi[group]).sample_start + 
		GROUP_VAL_POS(group, range) / 10;		

	TMP(group_spi[group]).beam_qty		= TMP(beam_qty[group]) - 1; 
	TMP(group_spi[group]).sample_offset	= 0;

	tt[0] = (GROUP_VAL_POS(group, gate[0].start) +	GROUP_VAL_POS (group, gate[0].width));
	tt[1] = (GROUP_VAL_POS(group, gate[1].start) +	GROUP_VAL_POS (group, gate[1].width));
	tt[2] = (GROUP_VAL_POS(group, gate[2].start) +	GROUP_VAL_POS (group, gate[2].width));

	tt[3] = MAX(tt[0], (MAX(tt[1],tt[2]))) / 10;

	TMP(group_spi[group]).rx_time		= MAX (tt[3], TMP(group_spi[group]).sample_range);
	TMP(group_spi[group]).gain1			= 0;

	if (GROUP_VAL_POS(group, prf)  >= 400)
			GROUP_VAL_POS(group, prf)  = 400;
/*		TMP(group_spi[group]).idel_time	= 
			100000000 / (GROUP_VAL_POS(group, prf) / (10.0 * CFG(prf_compress))) - 2048 - TMP(group_spi[group]).rx_time;
			*/
	temp_prf = TMP(beam_qty[group]) * GROUP_VAL_POS(group, prf);
	TMP(group_spi[group]).idel_time		= 
/*		100000000 / (GROUP_VAL_POS(group, prf) / 10) - 2048 - TMP(group_spi[group]).rx_time;*/
		100000000 / (temp_prf / (10)) - 2048 - TMP(group_spi[group]).rx_time;

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

	TMP(group_spi[group]).reject = get_reject(pp->p_config) * 40.95;	

	TMP(group_spi[group]).voltage = get_voltage (pp->p_config, group);	

}


