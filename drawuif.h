/************************************************* 
            定义主界面相关数据结构 
 ************************************************* */
 
#ifndef	__DRAW_UIF_H_
#define	__DRAW_UIF_H_
#include <string.h>
#include "datastruct.h"


#define MAXNUM 256
#define ELE_NUM 32

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
     
    int G_Time[256];
    int G_delay[256];
    int timedelay[256][32];
    int k;//the num of calculator 
    int tmp_k ;
    int is_draw_focus;
    

}PARAMETER, *PARAMETER_P;

#endif
