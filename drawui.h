
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#ifndef	__DRAW_UI_H_
#define	__DRAW_UI_H_
#include "base.h"
#include "language.h"

extern gint (*window_keypress_event_orig)(GtkWidget *widget, GdkEventKey *event);
extern gint my_keypress_event(GtkWidget *widget, GdkEventKey *event);
extern GtkWidgetClass *widget_window_class;

extern DRAW_UI_P pp;					

extern guint address[];
extern gfloat get_freq ();

extern void change_language();										/* 选择语言 */
extern void draw_2_menu(gint pa);									/* 更新二级菜单 */
extern void draw_3_menu(gint pa, gpointer p);									/* 更新三级菜单 */
extern void init_ui(DRAW_UI_P p);									/* 初始化界面, */

extern gboolean (*data_fun[6])(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
extern gboolean (*eventbox2_fun[5])(GtkWidget *widget, GdkEventButton *event, gpointer data);

extern void data_100 (GtkSpinButton *spinbutton, gpointer data);	/* 100 gain增益 */
extern void data_101 (GtkSpinButton *spinbutton, gpointer data);	/* 101 start 扫描延时 */
extern void data_102 (GtkSpinButton *spinbutton, gpointer data);	/* 102 range 范围 */
extern void data_103 (GtkSpinButton *spinbutton, gpointer data);	/* 103 wedget delay 楔款延时 */
extern void data_104 (GtkSpinButton *spinbutton, gpointer data);	/* 104 velocity 声速 */

extern void data_110 (GtkSpinButton *spinbutton, gpointer data);	/* 110 pulser 脉冲发射器 */
extern void data_111 (GtkMenuItem *menuitem, gpointer data);            /* 111 Tx/Rx Mode 收发模式 */
extern void data_1121 (GtkSpinButton *spinbutton, gpointer data);       /* 112 freq 频率 */
extern void data_112 (GtkMenuItem *menuitem, gpointer data);            /* 112 Freq 频率 */

extern void data_125 (GtkSpinButton *spinbutton, gpointer data);	/*125 */

extern void data_130 (GtkSpinButton *spinbutton, gpointer data);	/*130*/
extern void data_131 (GtkSpinButton *spinbutton, gpointer data);	/*131*/
extern void data_132 (GtkSpinButton *spinbutton, gpointer data);	/*132*/
extern void data_134 (GtkSpinButton *spinbutton, gpointer data);	/*134*/
extern void data_135 (GtkSpinButton *spinbutton, gpointer data);	/*135*/

extern void data_202 (GtkSpinButton *spinbutton, gpointer data);	/*202*/
extern void data_203 (GtkSpinButton *spinbutton, gpointer data);	/*203*/
extern void data_204 (GtkSpinButton *spinbutton, gpointer data);	/*204*/

extern void data_222 (GtkSpinButton *spinbutton, gpointer data);	/*222*/
extern void data_224 (GtkSpinButton *spinbutton, gpointer data);	/*224*/
extern void data_225 (GtkSpinButton *spinbutton, gpointer data);	/*225*/

extern void data_311 (GtkSpinButton *spinbutton, gpointer data);	/*311*/
extern void data_313 (GtkSpinButton *spinbutton, gpointer data);	/*313*/

extern void data_331 (GtkSpinButton *spinbutton, gpointer data);	/*331*/
extern void data_332 (GtkSpinButton *spinbutton, gpointer data);	/*332*/
extern void data_333 (GtkSpinButton *spinbutton, gpointer data);	/*333*/

extern void data_431 (GtkSpinButton *spinbutton, gpointer data);	/*431*/
extern void data_432 (GtkSpinButton *spinbutton, gpointer data);	/*432*/

extern void data_510 (GtkSpinButton *spinbutton, gpointer data);	/*510*/
extern void data_511 (GtkSpinButton *spinbutton, gpointer data);	/*511*/

extern void data_521 (GtkSpinButton *spinbutton, gpointer data);	/*521*/
extern void data_522 (GtkSpinButton *spinbutton, gpointer data);	/*522*/
extern void data_523 (GtkSpinButton *spinbutton, gpointer data);	/*523*/

extern void data_531 (GtkSpinButton *spinbutton, gpointer data);	/*531*/

extern void data_601 (GtkSpinButton *spinbutton, gpointer data);	/*601*/

extern void data_610 (GtkSpinButton *spinbutton, gpointer data);	/*610*/
extern void data_611 (GtkSpinButton *spinbutton, gpointer data);	/*611*/
extern void data_612 (GtkSpinButton *spinbutton, gpointer data);	/*612*/
extern void data_613 (GtkSpinButton *spinbutton, gpointer data);	/*613*/

extern void data_620 (GtkSpinButton *spinbutton, gpointer data);	/*620*/
extern void data_623 (GtkSpinButton *spinbutton, gpointer data);	/*623*/

extern void data_713 (GtkSpinButton *spinbutton, gpointer data);	/*713*/

extern void data_720 (GtkSpinButton *spinbutton, gpointer data);	/*720*/
extern void data_721 (GtkSpinButton *spinbutton, gpointer data);	/*721*/
extern void data_722 (GtkSpinButton *spinbutton, gpointer data);	/*722*/
extern void data_723 (GtkSpinButton *spinbutton, gpointer data);	/*723*/
extern void data_724 (GtkSpinButton *spinbutton, gpointer data);	/*724*/
extern void data_725 (GtkSpinButton *spinbutton, gpointer data);	/*725*/

extern void data_901 (GtkSpinButton *spinbutton, gpointer data);	/*901*/

extern gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

extern gboolean key_press_handler (GtkWidget* pWidget, GdkEventKey* pEvent, gpointer data);


extern void draw3_pressed1(gfloat step, guint digit);

#endif
