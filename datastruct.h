/************************************************* 
  定义各种基本数据结构
  工件，楔块，探头等等 
 ************************************************* */

#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H
#include <gtk/gtk.h>

/*保存聚焦法则所需变量*/
/*探头(Probe)*/
typedef struct ProbeF
{
	/*transmitter*/
	gint      D1_D2;	/* 0 1d 1 2d */
	gint      Pitch_Catch;
	gint      transmi_trans_longi;//设置发送探头 横波 OR 纵波 、
	gint      recev_trans_longi;//设置接收探头 横波 OR 纵波  
	gdouble   pb_skew_angle;
	gdouble   pb_frequency;
	gint      ele_num_pri;//主轴阵元数
	gint      ele_num_sec;//副轴阵元数 
	gdouble   pri_axis_pitch;//主轴间距
	gdouble   sec_axis_pitch;
	gdouble   pri_ele_size;
	gdouble   sec_ele_size;
	/*receiver*/ 
	gdouble   pb_skew_angle_r;
	gdouble   pb_frequency_r;
	gint      ele_num_pri_r;//阵元数
	gint      ele_num_sec_r;
	gdouble   pri_axis_pitch_r;//主轴间距
	gdouble   sec_axis_pitch_r;
	gdouble   pri_ele_size_r;
	gdouble   sec_ele_size_r;
} PROBEF, *PROBEF_P;

/*楔块(Wedge)*/
typedef struct WedgeF
{
	gdouble   wg_wedge_angle; /* 楔块角 */
	gdouble   wg_roof_angle;/* 顶角 */
	gdouble   wg_lon_vel;/*纵波声速*/
	gdouble   wg_trans_vel;/*横波声速*/
	gint      wg_density;/* 密度 */
	gdouble   wg_heigh_fir;/*第一阵元高度*/
	gdouble   wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	gdouble   wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	gdouble   wg_pri_axis_reference;/*主轴楔块参考位置*/
	gdouble   wg_sec_axis_reference;/*次轴楔块参考位置*/
	gdouble   wg_length;/*楔块长度*/
	gdouble   wg_width;/*楔块宽度*/
	gdouble   wg_height;/*楔块高度*/
	gdouble   wg_separation;
}WEDGEF, *WEDGEF_P;

/* 波束角选择(Beam angle selection) */
typedef struct Beam_angle
{
	gdouble   beam_pri_steer_angle_start;
	gdouble   beam_pri_steer_angle_stop;
	gdouble   beam_pri_steer_angle_resolution;
	//
	gdouble   beam_sec_steer_angle_start;
	gdouble   beam_sec_steer_angle_stop;
	gdouble   beam_sec_steer_angle_resolution;
	//
	gdouble   beam_refrac_angle_start;  
	gdouble   beam_refrac_angle_stop;
	gdouble   beam_refrac_angle_resolution; 
	// 
	gdouble   beam_skew_angle_start;  
	gdouble   beam_skew_angle_stop;
	gdouble   beam_skew_angle_resolution;
	gint      beam_type;
	gint      beam_angle_sel;
}BEAM_ANGLE, *BEAM_ANGLE_P;

/* 聚焦点选择(focal point selection) */
typedef struct Focal_point
{
	gint      focal_focus_type;
	gdouble   focal_focus_point_start;
	gdouble   focal_focus_point_stop; 
	gdouble   focal_focus_point_resolution; 
}FOCAL_POINT, *FOCAL_POINT_P;

/* 样本设定(specimen settings) */
typedef struct specimen
{
	guint   speci_longitudinal_wave;//样本纵波声速     
	guint   speci_transverse_wave;//样本横波声速 
	//
	gdouble speci_length_flat;
	gdouble speci_height_flat;
	gdouble speci_width_flat;  
	//
	gdouble speci_inside_cylindrical;
	gdouble speci_outside_cylindrical;
	gdouble speci_length_cylindrical; 
	gint    flat_cylindrical;
	//
	gchar   Inspection_od_id[2];  
}SPECIMEN,*SPECIMEN_P;

/*element selection*/
typedef struct element_sel
{   
	gdouble pri_axis_ape;
	gdouble sec_axis_ape;
}ELEMENT_SEL ,*ELEMENT_SEL_P;

#endif
