
#ifndef __DRAW_DIALOG_
#define __DRAW_DIALOG_

#include <gtk/gtk.h>

/* 弹出的对话框的类型 */
#define DIALOG_REMARK	0
#define DIALOG_PROBE	1
#define DIALOG_WEDGE	2
#define DIALOG_EDIT_NOTES	3
#define DIALOG_EDIT_HEADER	4
#define DIALOG_FILE_OPEN	6
#define DIALOG_SAVE_SETUP_AS	7

#define DIALOG_SYSTEM_INFO	8

#define DIALOG_COLOR_PALETTE	11
#define DIALOG_FILE_MANAGE		12
#define DIALOG_LAW_SAVE			13
#define DIALOG_LAW_READ			14
#define DIALOG_IP				15
#define DIALOG_MASK				16
#define DIALOG_TIME				17
#define DIALOG_DATE				18
#define DIALOG_REPORT_BUILD		19
#define DIALOG_DEFINE_PROBE		20

#define DIALOG_FILE_NAME		21
#define DIALOG_USERFIELD_LABEL	22
#define DIALOG_USERFIELD_CONTENT	23
#define DIALOG_DEFINE_WEDGE		24
#define DIALOG_FILE_NAME_ALL	25

extern void draw_dialog_all (guint type);

#endif
