/************************************************* 
            定义主界面相关数据结构 
 ************************************************* */
 
#ifndef	__DRAW_UIF_H_
#define	__DRAW_UIF_H_
#include <gtk/gtk.h>
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
    
    gint G_Time[256];
    gint timedelay[256][32];
    gint k;//the num of calculator 
    

}PARAMETER, *PARAMETER_P;

#endif
