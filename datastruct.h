/************************************************* 
  ������ֻ������ݽṹ
  ������Ш�飬̽ͷ�ȵ� 
 ************************************************* */

#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H

/*����۽������������*/
/*̽ͷ(Probe)*/
typedef struct ProbeF
{
	/*transmitter*/
	int      D1_D2;	/* 0 1d 1 2d */
	int      Pitch_Catch;
    	int      transmi_trans_longi;//���÷���̽ͷ �Შ OR �ݲ� ��
    	int      recev_trans_longi;//���ý���̽ͷ �Შ OR �ݲ�  
	double   pb_skew_angle;
	double   pb_frequency;
	int      ele_num_pri;//������Ԫ��
	int      ele_num_sec;//������Ԫ�� 
    	double   pri_axis_pitch;//������
    	double   sec_axis_pitch;
    	double   pri_ele_size;
    	double   sec_ele_size;
       /*receiver*/ 
       double   pb_skew_angle_r;
	double   pb_frequency_r;
	int      ele_num_pri_r;//��Ԫ��
	int      ele_num_sec_r;
    	double   pri_axis_pitch_r;//������
    	double   sec_axis_pitch_r;
    	double   pri_ele_size_r;
    	double   sec_ele_size_r;
} PROBEF, *PROBEF_P;

/*Ш��(Wedge)*/
typedef struct WedgeF
{
    double   wg_wedge_angle; /* Ш��� */
    double   wg_roof_angle;/* ���� */
    double   wg_lon_vel;/*�ݲ�����*/
    double   wg_trans_vel;/*�Შ����*/
    int      wg_density;/* �ܶ� */
    double   wg_heigh_fir;/*��һ��Ԫ�߶�*/
    double   wg_pri_elem_offset_fir;/*��һ������Ԫƫ��*/
    double   wg_sec_elem_offset_fir;/*��һ������Ԫƫ��*/
    double   wg_pri_axis_reference;/*����Ш��ο�λ��*/
    double   wg_sec_axis_reference;/*����Ш��ο�λ��*/
    double   wg_length;/*Ш�鳤��*/
    double   wg_width;/*Ш����*/
    double   wg_height;/*Ш��߶�*/
    double   wg_separation;
}WEDGEF, *WEDGEF_P;

/* ������ѡ��(Beam angle selection) */
typedef struct Beam_angle
{
    double   beam_pri_steer_angle_start;
    double   beam_pri_steer_angle_stop;
    double   beam_pri_steer_angle_resolution;
    //
    double   beam_sec_steer_angle_start;
    double   beam_sec_steer_angle_stop;
    double   beam_sec_steer_angle_resolution;
    //
    double   beam_refrac_angle_start;  
    double   beam_refrac_angle_stop;
    double   beam_refrac_angle_resolution; 
    // 
    double   beam_skew_angle_start;  
    double   beam_skew_angle_stop;
    double   beam_skew_angle_resolution;
    int      beam_type;
    int      beam_angle_sel;
}BEAM_ANGLE, *BEAM_ANGLE_P;

/* �۽���ѡ��(focal point selection) */
typedef struct Focal_point
{
    int      focal_focus_type;
    double   focal_focus_point_start;
    double   focal_focus_point_stop; 
    double   focal_focus_point_resolution; 

    double   offset_start;
}FOCAL_POINT, *FOCAL_POINT_P;

/* �����趨(specimen settings) */
typedef struct specimen
{
    unsigned int   speci_longitudinal_wave;//�����ݲ�����     
    unsigned int   speci_transverse_wave;//�����Შ���� 
    //
    double speci_length_flat;
    double speci_height_flat;
    double speci_width_flat;  
    //
    double speci_inside_cylindrical;
    double speci_outside_cylindrical;
    double speci_length_cylindrical; 
    int    flat_cylindrical;
    //
    char   Inspection_od_id[2];    
}SPECIMEN,*SPECIMEN_P;

/*element selection*/
typedef struct element_sel
{   
    int pri_axis_ape;
    int sec_axis_ape;

    int primary_axis_s;
    int primary_axis_e;
    int primary_axis_r;

    int seconary_axis_s;
}ELEMENT_SEL ,*ELEMENT_SEL_P
;



#endif
