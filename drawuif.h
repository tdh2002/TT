/************************************************* 
  ����������������ݽṹ 
 ************************************************* */

#ifndef	__DRAW_UIF_H_
#define	__DRAW_UIF_H_
#include <gtk/gtk.h>
#include <string.h>
#include "datastruct.h"

#include <gdk/gdkkeysyms.h>

/*������ṹ��*/
typedef struct FocusParameter
{
	//�����趨����
	PROBEF_P        probe_p;
	WEDGEF_P        wedge_p;
	WEDGEF_P        wedge_r;
	BEAM_ANGLE_P   beam_angle;
	SPECIMEN_P     specimen;
	FOCAL_POINT_P  focal_point;
	ELEMENT_SEL_P  element_sel;

	gint G_Time[256];
	gint timedelay[256][32];
	gint k;//�۽�����ʱ������ 
	//����̽ͷ����
	gdouble  focus_x[512];/*�۽������� */
	gdouble focus_y[512];/*�۽�������*/
	gdouble focus_z[512];/*�۽�������*/	
	//����̽ͷ����------ 1D ˫��ʱ��
	gdouble focus_xr[512];
	gdouble focus_yr[512];
	gdouble focus_zr[512];
	//����̽ͷ
	gdouble probe_x[512][512];/*��Ԫ����*/
	gdouble probe_y[512][512];/*��Ԫ����*/
	gdouble probe_z[512][512];/*��Ԫ����*/
	gdouble horizontal_x[512][512];/*��������� */
	gdouble horizontal_y[512][512];/*��������� */
	gdouble horizontal_z[512][512];/*��������� */
	//����̽ͷ
	gdouble probe_xr[512][512];/*��Ԫ����*/
	gdouble probe_yr[512][512];/*��Ԫ����*/
	gdouble probe_zr[512][512];/*��Ԫ����*/
	gdouble horizontal_xr[512][512];/*��������� */
	gdouble horizontal_yr[512][512];/*��������� */
	gdouble horizontal_zr[512][512];/*��������� */

}PARAMETER, *PARAMETER_P;

#endif
