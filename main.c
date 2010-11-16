
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

GdkColor	color_black= {0x0, 0x0, 0x0, 0x0};
GdkColor	color_button0= {0x0, 0x9999, 0x9999, 0x9999};
GdkColor	color_button1= {0x0, 0x7e00, 0xb700, 0xf300};

/* You have to start somewhere */
int main (int argc, char *argv[])
{
	DRAW_UI_P	p_ui;					
	CONFIG_P	p_config;				
	CONFIG_P	p_tmp_config;					
	GtkWidget	*window;

	gtk_init (&argc, &argv);

	p_ui		= (DRAW_UI_P)g_malloc0(sizeof(DRAW_UI));
	p_config	= (CONFIG_P)g_malloc0(sizeof(CONFIG));
	p_tmp_config	= (TMP_CONFIG_P)g_malloc0(sizeof(TMP_CONFIG));
	g_print("DRAW_UI's size:%d xx = %d\n", sizeof(DRAW_UI), p_ui->xx);
	g_print("CONFIG's size:%d xx = %d\n", sizeof(CONFIG), p_config->time);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (window), FALSE);			/*不可以装饰*/
	gtk_window_set_default_size (GTK_WINDOW(window), 800, 600);		/*设置窗口大小*/
	gtk_widget_modify_bg (window, GTK_STATE_NORMAL, &color_black);	/*黑色背景*/
	g_signal_connect (G_OBJECT(window), "delete_event",
			G_CALLBACK(gtk_main_quit), NULL);			/**/

	p_ui->p_config		= p_config;
	p_ui->p_tmp_config	= p_tmp_config;
	p_ui->window		= window;
	init_ui(p_ui);
	
	gtk_widget_show (window);
	gtk_main();

#if 0	
	draw_area_init(&tan1, vbox30, 0, 0, 400,426);
#endif

	return 0;
}
