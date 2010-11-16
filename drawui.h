
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#ifndef	__DRAW_UI_H_
#define	__DRAW_UI_H_
#include "base.h"
#include "content_en.h"

extern void change_language();										/*选择语言*/
extern void draw_2_menu(DRAW_UI_P p);
extern void draw_3_menu(DRAW_UI_P p);
extern void init_ui(DRAW_UI_P p);									/*初始化界面,*/

extern void (*button2_fun[5])(GtkButton *button, gpointer data);
extern gboolean (*button20_fun[5])(GtkWidget *widget, GdkEventFocus *event,	gpointer user_data);
extern void (*button3_fun[6])(GtkButton *button, gpointer data);

#endif
