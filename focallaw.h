/************************************************* 
  定义各种基本数据结构
  工件，楔块，探头等等 
 ************************************************* */

#ifndef FOCALLAW_H
#define FOCALLAW_H
#include <string.h>
#include <math.h>
#include <malloc.h>

#define MAXNUM 256
#define ELE_NUM 32


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
    double   offset_end;
    double   depth_start;
    double   depth_end;
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
    unsigned char   Inspection_od_id[2];    
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
}ELEMENT_SEL ,*ELEMENT_SEL_P;

/*location*/
typedef struct location
{
    double rotation_x;
    double rotation_y;
    double rotation_z;
}LOCATION ,*LOCATION_P;

/*data structer*/
typedef struct FocusParameter
{
    //Define paramenters
    PROBEF_P        probe_p;
    WEDGEF_P        wedge_p;
    WEDGEF_P        wedge_r;
    BEAM_ANGLE_P   beam_angle;
    SPECIMEN_P     specimen;
    FOCAL_POINT_P  focal_point;
    ELEMENT_SEL_P  element_sel;
    LOCATION_P     location;
      
    //*** 坐标信息用于以后opengl显示  ***//
	//Beam Information
    double   BeamExitPoint_x[MAXNUM];
    double   BeamExitPoint_y[MAXNUM];
    double   BeamExitPoint_z[MAXNUM];
    double   BeamRefAngle[MAXNUM] ;
    double   BeamSkewAngle[MAXNUM];
    double   BeamPriSteeringAngle[MAXNUM];
    double   BeamSecSteeringAngle[MAXNUM]; 
    //发射探头
    double focus_x[MAXNUM];/*聚焦点坐标*/
    double focus_y[MAXNUM];/*聚焦点坐标*/
    double focus_z[MAXNUM];/*聚焦点坐标*/
    double probe_x[MAXNUM][MAXNUM];/*阵元坐标*/
    double probe_y[MAXNUM][MAXNUM];/*阵元坐标*/
    double probe_z[MAXNUM][MAXNUM];/*阵元坐标*/
    double horizontal_x[MAXNUM][MAXNUM];/*入射点坐标 */
    double horizontal_y[MAXNUM][MAXNUM];/*入射点坐标 */
    double horizontal_z[MAXNUM][MAXNUM];/*入射点坐标 */
    //接收探头
    double focus_xr[MAXNUM];/*聚焦点坐标 */
    double focus_yr[MAXNUM];/*聚焦点坐标 */
    double focus_zr[MAXNUM];/*聚焦点坐标 */
    double probe_xr[MAXNUM][MAXNUM];/*阵元坐标*/
    double probe_yr[MAXNUM][MAXNUM];/*阵元坐标*/
    double probe_zr[MAXNUM][MAXNUM];/*阵元坐标*/
    double horizontal_xr[MAXNUM][MAXNUM];/*入射点坐标 */
    double horizontal_yr[MAXNUM][MAXNUM];/*入射点坐标 */
    double horizontal_zr[MAXNUM][MAXNUM];/*入射点坐标 */
    
    double G_Time[MAXNUM];//保存波束时间
    int G_delay[MAXNUM];//波束延时
    int timedelay[MAXNUM][ELE_NUM];//阵元延时
    int k;//the num of calculator 
    int tmp_k ;//临时变量
    int is_draw_focus;//标志位
    

}PARAMETER, *PARAMETER_P;

#endif
