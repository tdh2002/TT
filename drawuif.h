/************************************************* 
  定义主界面相关数据结构 
 ************************************************* */

#ifndef	__DRAW_UIF_H_
#define	__DRAW_UIF_H_
#include <gtk/gtk.h>
#include <string.h>
#include "datastruct.h"

#include <gdk/gdkkeysyms.h>

/*画界面结构体*/
typedef struct FocusParameter
{
	//各种设定参数
	PROBEF_P        probe_p;
	WEDGEF_P        wedge_p;
	WEDGEF_P        wedge_r;
	BEAM_ANGLE_P   beam_angle;
	SPECIMEN_P     specimen;
	FOCAL_POINT_P  focal_point;
	ELEMENT_SEL_P  element_sel;

	gint G_Time[256];
	gint timedelay[256][32];
	gint k;//聚焦计算时点的序号 
	//发射探头焦点
	gdouble  focus_x[512];/*聚焦点坐标 */
	gdouble focus_y[512];/*聚焦点坐标*/
	gdouble focus_z[512];/*聚焦点坐标*/	
	//接收探头焦点------ 1D 双晶时用
	gdouble focus_xr[512];
	gdouble focus_yr[512];
	gdouble focus_zr[512];
	//发射探头
	gdouble probe_x[512][512];/*阵元坐标*/
	gdouble probe_y[512][512];/*阵元坐标*/
	gdouble probe_z[512][512];/*阵元坐标*/
	gdouble horizontal_x[512][512];/*入射点坐标 */
	gdouble horizontal_y[512][512];/*入射点坐标 */
	gdouble horizontal_z[512][512];/*入射点坐标 */
	//接收探头
	gdouble probe_xr[512][512];/*阵元坐标*/
	gdouble probe_yr[512][512];/*阵元坐标*/
	gdouble probe_zr[512][512];/*阵元坐标*/
	gdouble horizontal_xr[512][512];/*入射点坐标 */
	gdouble horizontal_yr[512][512];/*入射点坐标 */
	gdouble horizontal_zr[512][512];/*入射点坐标 */

}PARAMETER, *PARAMETER_P;

#endif
