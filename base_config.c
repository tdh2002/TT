/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base_config.h"
#include <string.h>

/* 材料 (Material) 28Byte OK */
typedef struct _Material 
{
	guint	Velocity_LW;	/* 声速 单位 0.01m/s 纵波 快点 */
	guint	Velocity_SW;	/* 声速 单位 0.01m/s 横波 慢点 */
	gchar	Name[20];		/* 材料名字 */
} st_MATERIAL;

static const st_MATERIAL data[] =
{
	{626000, 308000, "Aluminum"},			/* 0 */
	{592000, 323000, "Steel common"},		/* 1 */
	{566400, 0, "Steel stainless"},			/* 2 */
	{439400,0 , "Brass"},					/* 3 */
	{470000, 226000, "Copper"},				/* 4 */
	{5893, 323000, "Iron"},					/* 5 */
	{215900, 0, "Lead"},					/* 6 */
	{220000, 0, "Nylon"},					/* 7 */
	{360700, 0, "Silver"},					/* 8 */
	{325100, 0, "Gold"},					/* 8 */
	{421600, 0, "Zinc"},					/* 10 */
	{609600, 0, "Titanium"},				/* 11 */
	{332700, 0, "Tin"},						/* 12 */
	{254000, 110000, "Epoxy resin"},		/* 13 */
	{398800, 0, "Ice"},						/* 14 */
	{563900, 0, "Nickel"},					/* 15 */
	{273000, 146000, "Plexiglass"},			/* 16 */
	{235000, 115000, "Polystyrene"},		/* 17 */
	{584200, 0, "Porcelain"},				/* 18 */
	{238800, 0, "PVC"},						/* 19 */
	{563900, 0, "Quartz glass"},			/* 20 */
	{231100, 0, "Rubber vulcanized"},		/* 21 */
	{142200, 0, "Teflon"},					/* 22 */
	{148000, 0, "Water"}					/* 23 */
};

static inline gboolean get_bit_value (gint val, gint bit)
{
	g_assert (bit < 32);
	g_assert (bit >= 0);
	return ((val & (0x01 << bit)) != 0);
}

static inline gint set_bit_value (gint val, gint bit, gboolean val1)
{
	g_assert (bit < 32);
	g_assert (bit >= 0);
	return ((val & ~(0x1 << bit)) | (val1 << bit));

}

gchar *get_material_name (CONFIG *p)
{
	return (gchar *)(data[p->part.Material_pos].Name);
}

guint get_material_lw (CONFIG *p)
{
	return data[p->part.Material_pos].Velocity_LW;
}

guint get_material_sw (CONFIG *p)
{
	return data[p->part.Material_pos].Velocity_SW;
}

guchar get_part_geometry (CONFIG *p)
{
	return p->part.Geometry;
}

void set_part_geometry (CONFIG *p, gpointer data)
{
	p->part.Geometry = (guchar)(GPOINTER_TO_UINT (data));
}

guchar get_part_material (CONFIG *p)
{
	return p->part.Material_pos;
}

void set_part_material (CONFIG *p, gpointer data)
{
	p->part.Material_pos = (guchar)(GPOINTER_TO_UINT (data));
}

guint get_part_thickness (CONFIG *p)
{
	return p->part.Thickness;
}

void set_part_thickness (CONFIG *p, gpointer data)
{
	p->part.Thickness = (guint)(GPOINTER_TO_UINT (data));
}

guint get_part_diameter (CONFIG *p)
{
	return p->part.Diameter;
}

void set_part_diameter (CONFIG *p, gpointer data)
{
	p->part.Diameter = (guint)(GPOINTER_TO_UINT (data));
}

/* 配置信息的操作 */
guchar get_current_group (CONFIG *p)	/* 当前p活动的group 0-7 */
{
	return	p->groupId;
}

void set_current_group (CONFIG *p, guchar data)	/* 设置p当前活动的group */
{
	g_assert (data < 8);
	p->groupId = data;
}

guchar get_group_qty (CONFIG *p)	/* 取得p当前group数量 */
{
	return	p->groupQty;
}

void set_group_qty (CONFIG *p, guchar data)	/* 设置p当前group数量 */
{
	g_assert (data < 9);
	p->groupQty = data;
}

guchar get_voltage (CONFIG *p, guint group_num)	/* 取得p发射电压,发射与group的模式相关 */
{
	switch ((p->group[group_num].group_mode))
	{
		case UT_SCAN:
		case UT1_SCAN:	return p->voltage_ut;	break;
		case PA_SCAN:	return p->voltage_pa;	break;
		default:g_print("Voltage get value error\n");
				return 0;break;
	}
}

void set_voltage (CONFIG *p, guint group_num, guchar data)	/* 取得p发射电压,发射与group的模式相关 */
{
	g_assert (data < 3);
	switch ((p->group[group_num].group_mode))
	{
		case UT_SCAN:
		case UT1_SCAN:	p->voltage_ut = data;	break;
		case PA_SCAN:	p->voltage_pa = data;	break;
		default:g_print("Voltage set value error\n");	break;
	}
}

guchar get_language (CONFIG *p)		/* 获取语言 */
{
	return p->language;
}

void set_language (CONFIG *p, guchar data)	/* 设置语言 */
{
	g_assert (data < 2);			/* 支持语言的总数量 */
	p->language = data;
}

guchar get_reject (CONFIG *p)		/* 获取抑制 */
{
	return p->reject;
}

void set_reject (CONFIG *p, guchar data)	/* 设置抑制 */
{
	g_assert (data < 101);			/* 支持语言的总数量 */
	p->reject = data;
}

guchar get_display_pos (CONFIG *p)		/* 获取当前显示模式 ABSC... */
{
	return p->display_pos;
}

void set_display_pos (CONFIG *p, guchar data)	/* 设置当前显示模式 ABSC */
{
	g_assert (data < 11);
	p->display_pos = data;
}

guchar get_display_group (CONFIG *p)		/* 获取显示是全部还是当前 返回1是当前 0是全部 */
{
	switch (get_display_pos(p))
	{
		case A_SCAN: 
			return (p->display_group & 0x01); break;		/**/
		case S_SCAN:
			return (p->display_group & 0x02) >> 1; break;	/**/
		case A_C_CC_SCAN:
			return (p->display_group & 0x04) >> 2; break;	/**/
		case A_S_CC_SCAN:
			return (p->display_group & 0x08) >> 3; break;	/**/
		case B_SCAN:
		case C_SCAN:
		case A_B_SCAN:
		case A_B_C_SCAN:
		case A_B_S_SCAN:
		case PA_TOFD:
		case Strip_Chart_AA: return 1;break;
		default:break;
	}
	return 1;
}

void set_display_group (CONFIG *p, guchar data)	/* 设置显示是当前还是全部 */
{
	g_assert (data < 2);
	switch (get_display_pos(p))
	{
		case A_SCAN:
			p->display_group = (p->display_group & ~0x01) | data; break;
		case S_SCAN:
			p->display_group = (p->display_group & ~0x02) | (data << 1); break;
		case A_C_CC_SCAN:
			p->display_group = (p->display_group & ~0x04) | (data << 2); break;
		case A_S_CC_SCAN:
			p->display_group = (p->display_group & ~0x08) | (data << 3); break;
		case B_SCAN:
		case C_SCAN:
		case A_B_SCAN:
		case A_B_C_SCAN:
		case A_B_S_SCAN:
		case PA_TOFD:
		case Strip_Chart_AA: return ;break;
		default:break;
	}
}

guchar	get_cscan_source (CONFIG *p, guint pos)	/* 获取c scan source */
{
	g_assert (pos < 2);
	switch (get_display_pos(p))
	{
		case C_SCAN:
		case A_B_C_SCAN:
			return (p->c_scan_pos & 0x03); break;			/* c_scan1 第0~1位 0 1 2 3 */
		case A_C_CC_SCAN:
			if (pos == 0)
				return (p->c_scan_pos & 0x03);				/* c_scan1 第0~1位 0 1 2 3 */
			else if (pos == 1)
				return (p->c_scan_pos & 0x1c) >> 2;			/* c_scan2 第2-4位 0 1 2 3 4 */
			break;
		case A_S_CC_SCAN:
			return (p->c_scan_pos & 0xe0) >> 5; break;		/* c_scan3 5-7位  0 1 2 3 4*/
		case A_SCAN: 
		case B_SCAN:
		case S_SCAN:
		case A_B_SCAN:
		case A_B_S_SCAN:
		case PA_TOFD:
		case Strip_Chart_AA: return 4;break;
		default:break;
	}
	return 4;
}

void set_cscan_source (CONFIG *p, guchar data, guint pos)	/* 设置c scan source */
{
	g_assert (pos < 2);
	g_assert (data < 5);
	switch (get_display_pos(p))
	{
		case C_SCAN:
		case A_B_C_SCAN:
			p->c_scan_pos = (p->c_scan_pos & ~0x03) | data; break;			/* c_scan1 第0~1位 0 1 2 3 */
		case A_C_CC_SCAN:
			if (pos == 0)
				p->c_scan_pos = (p->c_scan_pos & ~0x03) | data;		/* c_scan1 第0~1位 0 1 2 3 */
			else if (pos == 1)
				p->c_scan_pos = (p->c_scan_pos & ~0x1c) | (data << 2);		/* c_scan2 第2-4位 0 1 2 3 4 */
			break;
		case A_S_CC_SCAN:
			p->c_scan_pos = (p->c_scan_pos & ~0xe0) | (data << 5); break;	/* c_scan3 5-7位  0 1 2 3 4*/
		case A_SCAN: 
		case B_SCAN:
		case S_SCAN:
		case A_B_SCAN:
		case A_B_S_SCAN:
		case PA_TOFD:
		case Strip_Chart_AA: return ;break;
		default:break;
	}
}

guchar get_stripscan_data1 (CONFIG *p)
{
	return p->data1;
}

guchar get_stripscan_data2 (CONFIG *p)
{
	return p->data2;
}

guchar get_stripscan_mode (CONFIG *p)
{
	return p->dis_mode;
}

guint get_stripscan_disrange (CONFIG *p)
{
	return p->dis_range;
}

void set_stripscan_data1 (CONFIG *p, guchar data)
{
	g_assert (data < 3);
	p->data1 = data;
}

void set_stripscan_data2 (CONFIG *p, guchar data)
{
	g_assert (data < 4);
	p->data2 = data;
}

void set_stripscan_mode (CONFIG *p, guchar data)
{
	g_assert (data < 4);
	p->dis_mode = data;
}

void set_stripscan_disrange (CONFIG *p, guint data)
{
	g_assert (data > 0 );
	p->dis_range = data;
}

guchar get_alarm_pos (CONFIG *p)				/* 16中不同的报警信息 */
{
	return p->alarm_pos;
}

void set_alarm_pos (CONFIG *p, guchar data)
{
	g_assert (data < 16);
	p->alarm_pos = data;
}

/* alarm_info 分布 3 4 1 3 4 groupa conditiona operator groupb conditionb */
guchar get_alarm_groupa (CONFIG *p)
{
	return p->alarm_info[get_alarm_pos(p)] & 0x7;
}

void set_alarm_groupa (CONFIG *p, guchar data)
{
	g_assert (data < 8);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x07) | data; 		
}

guchar get_alarm_conditiona (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x78) >> 3;
}

void set_alarm_conditiona (CONFIG *p, guchar data)
{
	g_assert (data < 9);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x78) | (data << 3); 		
}

guchar get_alarm_operator (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x80) >> 7;
}

void set_alarm_operator (CONFIG *p, guchar data)
{
	g_assert (data < 2);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x80) | (data << 7); 
}

guchar get_alarm_groupb (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x700) >> 8;
}

void set_alarm_groupb (CONFIG *p, guchar data)
{
	g_assert (data < 8);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x700) | (data << 8); 
}

guchar get_alarm_conditionb (CONFIG *p)
{
	return (p->alarm_info[get_alarm_pos(p)] & 0x7800) >> 11;
}

void set_alarm_conditionb (CONFIG *p, guchar data)
{
	g_assert (data < 9);
	p->alarm_info[get_alarm_pos(p)] = (p->alarm_info[get_alarm_pos(p)] & ~0x7800) | (data << 11);
}

/* OUPUT */
guchar	get_output_pos (CONFIG *p)
{
	return p->output_pos;
}

void set_output_pos (CONFIG *p, guchar data)
{
	g_assert (data < 5);
	p->output_pos = data;
}

gushort	get_output_alarm (CONFIG *p)
{
	return p->output1[get_output_pos(p)].alarm_info;
}

gushort	get_1output_alarm_pos (CONFIG *p)
{
	gint i;
	g_assert (get_output_alarm_qty (p) == 1);
	for (i = 0; i < 16; i++)
	{
		if (get_output_alarm_pos (p, i + 1))
			return (i + 1);
	}
	return 0;
}

guchar get_output_alarm_qty (CONFIG *p)
{
	gint qty = 0, i;
	for (i = 0; i < 16; i++)
	{
		if (get_output_alarm_pos (p, i + 1))
			qty += 1;
	}
	return (guchar)(qty);
}

guchar get_output_alarm_pos (CONFIG *p, guint pos)
{
	g_assert (pos < 17);
	return (p->output1[get_output_pos(p)].alarm_info >> (pos - 1)) & 0x01;
}

void set_output_alarm (CONFIG *p, guchar data, guint pos)
{
	g_assert (data < 2);
	g_assert (data < 17);
	p->output1[get_output_pos(p)].alarm_info = 
		(p->output1[get_output_pos(p)].alarm_info & ~(0x01 << (pos - 1))) | (data << (pos -1));
}

guchar get_output_count (CONFIG *p)
{
	return p->output1[get_output_pos(p)].count;
}

guchar get_output_sound (CONFIG *p)
{
	return p->output1[get_output_pos(p)].sound;
}

void set_output_count (CONFIG *p, guchar data)
{
	g_assert (data < 101);
	p->output1[get_output_pos(p)].count = data;
}

void set_output_sound (CONFIG *p, guchar data)
{
	g_assert (data < 5);
	p->output1[get_output_pos(p)].sound = data;
}

guint get_output_delay (CONFIG *p)
{
	return p->output1[get_output_pos(p)].delay;
}

guint get_output_holdtime (CONFIG *p)
{
	return p->output1[get_output_pos(p)].holdtime;
}

void set_output_delay (CONFIG *p, guint data)
{
	p->output1[get_output_pos(p)].delay = data;
}

void set_output_holdtime (CONFIG *p, guint data)
{
	p->output1[get_output_pos(p)].holdtime = data;
}

guchar get_output_group (CONFIG *p)
{
	return (p->analog1[get_output_pos(p) - 3] & 0x7);
}

guchar get_output_data (CONFIG *p)
{
	return ((p->analog1[get_output_pos(p) - 3] & 0x18) >> 3);
}

void set_output_group (CONFIG *p, guchar data)
{
	g_assert (data < 8);
	p->analog1[get_output_pos(p) - 3] = 
		(p->analog1[get_output_pos(p) - 3] & ~(0x07)) | data;
}

void set_output_data (CONFIG *p, guchar data)
{
	g_assert (data < 4);
	p->analog1[get_output_pos(p) - 3] = 
		(p->analog1[get_output_pos(p) - 3] & ~(0x18)) | (data << 3);
}

/* 设置获取亮度 */
guchar get_bright (CONFIG *p)
{
	return	p->bright;
}

void set_bright (CONFIG *p, guchar data)
{
	g_assert (data < 101);
	p->bright = data;
}

/* Reading list field 设置当前读取的数值 */
guchar get_reading_list (CONFIG *p)
{
	return p->list;
}

void set_reading_list (CONFIG *p, guchar data)
{
	g_assert (data < 2);
	p->list = data;
}

guchar get_reading_field1 (CONFIG *p)
{
	return p->field[0];
}

void set_reading_field1 (CONFIG *p, guchar data)
{
	g_assert (data < 128);
	p->field[0] = data;
}

guchar get_reading_field2 (CONFIG *p)
{
	return p->field[1];
}

void set_reading_field2 (CONFIG *p, guchar data)
{
	g_assert (data < 128);
	p->field[1] = data;
}

guchar get_reading_field3 (CONFIG *p)
{
	return p->field[2];
}

void set_reading_field3 (CONFIG *p, guchar data)
{
	g_assert (data < 128);
	p->field[2] = data;
}

guchar get_reading_field4 (CONFIG *p)
{
	return p->field[3];
}

void set_reading_field4 (CONFIG *p, guchar data)
{
	g_assert (data < 128);
	p->field[3] = data;
}

/* 单位 */
guchar get_unit (CONFIG *p)
{
	return p->unit;
}

void set_unit (CONFIG *p, guchar data)
{
	g_assert (data < 2);
	p->unit = data;
}

guchar get_display_table (CONFIG *p)
{
	return p->display_table;
}

void set_display_table (CONFIG *p, guchar data)
{
	g_assert (data < 2);
	p->display_table = data;
}

guchar get_entry_image (CONFIG *p)
{
	return p->entry_image;
}

void set_entry_image (CONFIG *p, guchar data)
{
	g_assert (data < 2);
	p->entry_image = data;
}

guint get_entry_qty (CONFIG *p)
{
	return p->entry_qty;
}

void set_entry_qty (CONFIG *p, guint data)
{
	p->entry_qty = data;
}

guint get_cur_entry (CONFIG *p)
{
	return p->cur_entry;
}

void set_cur_entry (CONFIG *p, guint data)
{
	p->cur_entry = data;
}

guint get_min_thickness (CONFIG *p)
{
	return p->min_thickness;
}

void set_min_thickness (CONFIG *p, guint data)
{
	p->min_thickness = data;
}

guint get_max_thickness (CONFIG *p)
{
	return p->max_thickness;
}

void set_max_thickness (CONFIG *p, guint data)
{
	p->max_thickness = data;
}

guchar get_echo_qty (CONFIG *p)
{
	return p->echo_qty;
}

void set_echo_qty (CONFIG *p, guchar data)
{
	g_assert (data < 11);
	g_assert (data != 0);
	p->echo_qty = data;
}

/* Display Overlay 菜单数值的保存 */
guchar get_overlay_grid (CONFIG *p)
{
	return p->grid;
}

void set_overlay_grid (CONFIG *p, guchar data)
{
	g_assert (data < 6);
	p->grid = data;
}

/* 0位是overlay sizing curves */
gboolean get_overlay_sizing_curves (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 0);		
}

void set_overlay_sizing_curves (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 0, data);
}

/* 1位是overlay gate */
gboolean get_overlay_gate (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 1);		
}

void set_overlay_gate (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 1, data);
}

/* 2位是overlay cursor */
gboolean get_overlay_cursor (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 2);		
}

void set_overlay_cursor (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 2, data);
}

/* 3位是overlay overlay */
gboolean get_overlay_overlay (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 3);		
}

void set_overlay_overlay (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 3, data);
}

/* Display Properties 操作 */
guchar get_dis_prop_scan (CONFIG *p)
{
	return p->dis_prop_scan;
}

void set_dis_prop_scan (CONFIG *p, guchar data)
{
	g_assert (data < 6);
	p->dis_prop_scan = data;
}

guint get_b_compress (CONFIG *p)
{
	return p->bcompress;
}

void set_b_compress (CONFIG *p, guint data)
{
	p->bcompress = data;
}

/* 4位是bscan optimum */
gboolean get_dis_prop_boptimum (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 4);		
}

void set_dis_prop_boptimum (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 4, data);
}

/* 5位是cscan ratio */
gboolean get_dis_prop_cratio (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 5);		
}

void set_dis_prop_cratio (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 5, data);
}

/* 6位是sscan interpolation */
gboolean get_dis_prop_sinterpolation (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 6);		
}

void set_dis_prop_sinterpolation (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 6, data);
}

/* 7位是strip chart orientation */
gboolean get_dis_prop_strip_orientation (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 7);		
}

void set_dis_prop_strip_orientation (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 7, data);
}

guchar get_fft_color (CONFIG *p)
{
	return p->fft_color;
}

void set_fft_color (CONFIG *p, guchar data)
{
	g_assert (data < 4);
	p->fft_color = data;
}

/* 8位自动检测探头 */
gboolean get_auto_detect (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 8);		
}

void set_auto_detect (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 8, data);
}

/* 9位选择探头或者是自定义探头 */
gboolean get_probe_select (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 9);		
}

void set_probe_select (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 9, data);
}

/* 10位选择探头FFT开关 */
gboolean get_probe_fft (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 10);		
}

void set_probe_fft (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 10, data);
}

/* 11位选择聚焦法则自动计算开关 */
gboolean get_auto_focal (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 11);		
}

void set_auto_focal (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 11, data);
}

/* 选择当前encoder 1或者2 */
gboolean get_cur_encoder (CONFIG *p)
{
	return get_bit_value (p->on_off_status, 12);		
}

void set_cur_encoder (CONFIG *p, gboolean data)
{
	g_assert ((data == 1) || (data == 0));
	p->on_off_status = set_bit_value (p->on_off_status, 12, data);
}

/* group操作*/
void grpcpy (CONFIG *p, guint dst, guint src)		/* 把src group 配置复制到 dst group */
{
	g_assert (src < 8);
	g_assert (dst < 8);
	g_assert (src != dst);
	memcpy (&p->group[dst], &p->group[src] , sizeof(GROUP));
}







/***
void *get_cfg_val (CONFIG *p, gint val_type, gint grp)
{
	g_assert (p);
	g_assert (grp < 8);
	g_assert (val_type > 0);
	g_assert (val_type < 200);
	switch (val_type)
	{
		case VAL_GRP_ID:	
			return	(gpointer)(p->groupId);break;
		default:break;
	}
}
*****/

