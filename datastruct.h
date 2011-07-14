/************************************************* 
  定义各种基本数据结构
  工件，楔块，探头等等 
 ************************************************* */

#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H

/*保存聚焦法则所需变量*/
/*探头(Probe)*/
typedef struct ProbeF
{
	/*transmitter*/
	int      D1_D2;	/* 0 1d 1 2d */
	int      Pitch_Catch;
    	int      transmi_trans_longi;//设置发送探头 横波 OR 纵波 、
    	int      recev_trans_longi;//设置接收探头 横波 OR 纵波  
	double   pb_skew_angle;
	double   pb_frequency;
	int      ele_num_pri;//主轴阵元数
	int      ele_num_sec;//副轴阵元数 
    	double   pri_axis_pitch;//主轴间距
    	double   sec_axis_pitch;
    	double   pri_ele_size;
    	double   sec_ele_size;
       /*receiver*/ 
       double   pb_skew_angle_r;
	double   pb_frequency_r;
	int      ele_num_pri_r;//阵元数
	int      ele_num_sec_r;
    	double   pri_axis_pitch_r;//主轴间距
    	double   sec_axis_pitch_r;
    	double   pri_ele_size_r;
    	double   sec_ele_size_r;
} PROBEF, *PROBEF_P;

/*楔块(Wedge)*/
typedef struct WedgeF
{
    double   wg_wedge_angle; /* 楔块角 */
    double   wg_roof_angle;/* 顶角 */
    double   wg_lon_vel;/*纵波声速*/
    double   wg_trans_vel;/*横波声速*/
    int      wg_density;/* 密度 */
    double   wg_heigh_fir;/*第一阵元高度*/
    double   wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
    double   wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
    double   wg_pri_axis_reference;/*主轴楔块参考位置*/
    double   wg_sec_axis_reference;/*次轴楔块参考位置*/
    double   wg_length;/*楔块长度*/
    double   wg_width;/*楔块宽度*/
    double   wg_height;/*楔块高度*/
    double   wg_separation;
}WEDGEF, *WEDGEF_P;

/* 波束角选择(Beam angle selection) */
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

/* 聚焦点选择(focal point selection) */
typedef struct Focal_point
{
    int      focal_focus_type;
    double   focal_focus_point_start;
    double   focal_focus_point_stop; 
    double   focal_focus_point_resolution; 

    double   offset_start;
}FOCAL_POINT, *FOCAL_POINT_P;

/* 样本设定(specimen settings) */
typedef struct specimen
{
    unsigned int   speci_longitudinal_wave;//样本纵波声速     
    unsigned int   speci_transverse_wave;//样本横波声速 
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
