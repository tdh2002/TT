
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#ifndef	__DRAW_UI_H_
#define	__DRAW_UI_H_
#include "base.h"
#include "language.h"

extern DRAW_UI_P pp;					

extern void change_language();										/*选择语言*/
extern void draw_2_menu(gint pa);
extern void draw_3_menu(gint pa);
extern void init_ui(DRAW_UI_P p);									/*初始化界面,*/

extern void (*button3_fun[6])(GtkButton *button, gpointer data);
extern gboolean (*data_fun[6])(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
extern gboolean (*eventbox2_fun[5])(GtkWidget *widget, GdkEventButton *event, gpointer data);

extern void data_100 (GtkSpinButton *spinbutton, gpointer data);	/*100*/
extern void data_101 (GtkSpinButton *spinbutton, gpointer data);	/*101*/
extern void data_102 (GtkSpinButton *spinbutton, gpointer data);	/*102*/
extern void data_103 (GtkSpinButton *spinbutton, gpointer data);	/*103*/
extern void data_104 (GtkSpinButton *spinbutton, gpointer data);	/*104*/

extern void data_110 (GtkSpinButton *spinbutton, gpointer data);	/*110*/
extern void data_125 (GtkSpinButton *spinbutton, gpointer data);	/*125*/

extern void data_130 (GtkSpinButton *spinbutton, gpointer data);	/*130*/
extern void data_131 (GtkSpinButton *spinbutton, gpointer data);	/*131*/
extern void data_132 (GtkSpinButton *spinbutton, gpointer data);	/*132*/
extern void data_134 (GtkSpinButton *spinbutton, gpointer data);	/*134*/
extern void data_135 (GtkSpinButton *spinbutton, gpointer data);	/*135*/

extern gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);

extern gboolean key_press_handler (GtkWidget* pWidget,
		GdkEventKey* pEvent, gpointer pointerBunch);

#endif
