
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#include "drawui.h"

void init_ui(DRAW_UI_P p, GtkWidget *window)				/*初始化界面,*/
{
	gint i, j, k;



	p->vbox			= gtk_vbox_new(FALSE, 0);	
	p->hbox1		= gtk_hbox_new(FALSE, 0);	
	p->vbox11		= gtk_vbox_new(FALSE, 0);	
	p->hbox111		= gtk_hbox_new(FALSE, 0);	
	p->hbox1111[0]	= gtk_hbox_new(FALSE, 0);	
	p->hbox1111[1]	= gtk_hbox_new(FALSE, 0);	
	p->hbox1111[2]	= gtk_hbox_new(FALSE, 0);	
	p->hbox112		= gtk_hbox_new(FALSE, 0);	

	p->hbox2		= gtk_hbox_new(FALSE, 0);	
	p->vbox21		= gtk_vbox_new(FALSE, 0);	
	p->hbox211		= gtk_hbox_new(FALSE, 0);	
	p->vbox2111		= gtk_vbox_new(FALSE, 0);	
	p->hbox2112		= gtk_hbox_new(FALSE, 0);	

	p->hbox212		= gtk_hbox_new(FALSE, 0);	
	p->vbox22		= gtk_vbox_new(FALSE, 0);	

	for (i = 0; i < 6; i++)
		p->vbox221[i]	= gtk_vbox_new(FALSE, 0);	

	for (i = 0; i < 20; i++)
		p->buffer[i]	= gtk_text_buffer_new (NULL);
		
	p->view			= gtk_text_view_new ();

	p->menubar		= gtk_menu_bar_new();
	p->menu			= gtk_menu_new();
	p->menuitem_main	= gtk_menu_item_new_with_label(content1[1]);
	for (i = 0; i < 10; i++)
		p->menuitem[i]	= gtk_menu_item_new_with_label(content1[i]);

}

