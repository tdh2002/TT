/************************************************* 
  ������ֻ������ݽṹ
  ������Ш�飬̽ͷ�ȵ� 
 ************************************************* */

#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H
#include <gtk/gtk.h>

/*����۽������������*/
/*̽ͷ(Probe)*/
typedef struct ProbeF
{
	/*transmitter*/
	gint      D1_D2;	/* 0 1d 1 2d */
	gint      Pitch_Catch;
	gint      transmi_trans_longi;//���÷���̽ͷ �Შ OR �ݲ� ��
	gint      recev_trans_longi;//���ý���̽ͷ �Შ OR �ݲ�  
	gdouble   pb_skew_angle;
	gdouble   pb_frequency;
	gint      ele_num_pri;//������Ԫ��
	gint      ele_num_sec;//������Ԫ�� 
	gdouble   pri_axis_pitch;//������
	gdouble   sec_axis_pitch;
	gdouble   pri_ele_size;
	gdouble   sec_ele_size;
	/*receiver*/ 
	gdouble   pb_skew_angle_r;
	gdouble   pb_frequency_r;
	gint      ele_num_pri_r;//��Ԫ��
	gint      ele_num_sec_r;
	gdouble   pri_axis_pitch_r;//������
	gdouble   sec_axis_pitch_r;
	gdouble   pri_ele_size_r;
	gdouble   sec_ele_size_r;
} PROBEF, *PROBEF_P;

/*Ш��(Wedge)*/
typedef struct WedgeF
{
	gdouble   wg_wedge_angle; /* Ш��� */
	gdouble   wg_roof_angle;/* ���� */
	gdouble   wg_lon_vel;/*�ݲ�����*/
	gdouble   wg_trans_vel;/*�Შ����*/
	gint      wg_density;/* �ܶ� */
	gdouble   wg_heigh_fir;/*��һ��Ԫ�߶�*/
	gdouble   wg_pri_elem_offset_fir;/*��һ������Ԫƫ��*/
	gdouble   wg_sec_elem_offset_fir;/*��һ������Ԫƫ��*/
	gdouble   wg_pri_axis_reference;/*����Ш��ο�λ��*/
	gdouble   wg_sec_axis_reference;/*����Ш��ο�λ��*/
	gdouble   wg_length;/*Ш�鳤��*/
	gdouble   wg_width;/*Ш����*/
	gdouble   wg_height;/*Ш��߶�*/
	gdouble   wg_separation;
}WEDGEF, *WEDGEF_P;

/* ������ѡ��(Beam angle selection) */
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

/* �۽���ѡ��(focal point selection) */
typedef struct Focal_point
{
	gint      focal_focus_type;
	gdouble   focal_focus_point_start;
	gdouble   focal_focus_point_stop; 
	gdouble   focal_focus_point_resolution; 
}FOCAL_POINT, *FOCAL_POINT_P;

/* �����趨(specimen settings) */
typedef struct specimen
{
	guint   speci_longitudinal_wave;//�����ݲ�����     
	guint   speci_transverse_wave;//�����Შ���� 
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
