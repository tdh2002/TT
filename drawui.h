
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#ifndef	__DRAW_UI_H_
#define	__DRAW_UI_H_
#include "base.h"

static const gchar *content1[] = {
	/* 一级菜单名称 */
	"  Wizard  ","  UT Settings", "  Gate/Alarm  ", "  Measurements  ",
	"  Display  ", "  Probe/Part  ",  "  Focal Law  ",  "  Scan  ",
	"  File  "	, "  Prefenrences  ",
	"","","","",
	"","","","",
	"","","",""	
};



extern void init_ui(DRAW_UI_P p, GtkWidget *window);		/*初始化界面,*/

#endif
