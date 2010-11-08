
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#include "drawui.h"

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

GdkColor	color_black= {0x0, 0x0, 0x0};

/* You have to start somewhere */
int main(int argc, char *argv[])
{
	DRAW_UI_P	p_ui;
	gint	i;
	GtkWidget	*window;

	gtk_init (&argc, &argv);

	p_ui = (DRAW_UI_P)g_malloc0(sizeof(DRAW_UI));
	g_print("DRAW_UI's size:%d xx = %d\n", sizeof(DRAW_UI), p_ui->xx);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (window), FALSE);	/*不可以装饰*/
	gtk_window_set_default_size (GTK_WINDOW(window), 800, 600);		/*设置窗口大小*/
	gtk_widget_modify_bg (window, GTK_STATE_NORMAL, &color_black);	/*黑色背景*/
	g_signal_connect (G_OBJECT(window), "delete_event",
			G_CALLBACK(gtk_main_quit), NULL);	/**/

	init_ui(p_ui, window);
	
	for ( i = 0; i< 10; i++)
		g_print("%s\n", content1[i]);

	gtk_widget_show (window);
	gtk_main();

#if 0	
	/*	gtk_container_set_border_width(GTK_CONTAINER(window), 2);*//*设置边框宽度*/

	//--------vbox10--------
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 110, 120);
	gtk_box_pack_start (GTK_BOX (vbox10), drawing_area, FALSE, FALSE, 0);
	color_green.red = 0x5555, color_green.green = 0x5555, color_green.blue = 0x5555;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &color_green);
	gtk_widget_show(drawing_area);
	gtk_widget_show(vbox10);



	vbox12 = gtk_vbox_new(FALSE, 0);
	toolbutton2[0] = gtk_button_new_with_label("Gain\n(dB)");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton2[0]), 110, 42);
	color_green.red = 0x7e00, color_green.green = 0xb700, color_green.blue = 0xf300;
	gtk_widget_modify_bg(toolbutton2[0], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start (GTK_BOX (vbox12), toolbutton2[0], FALSE, FALSE, 0);

	label[0]=gtk_label_new("Group Wizard");
	gtk_box_pack_start (GTK_BOX (vbox12), label[0], FALSE, FALSE, 0);
	gtk_widget_show(label[0]);


	adj = (GtkAdjustment *) gtk_adjustment_new (21.0, 0.0, 74.0, 0.1, 10.0, 0.0);
	spinner[0] = gtk_spin_button_new (adj, 0, 1);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner[0], FALSE, FALSE, 0);
	gtk_widget_show(toolbutton2[0]);
	gtk_widget_show(spinner[0]);
	gtk_widget_show(vbox12);

	/*gtk_combo_box_append_text(GTK_COMBO_BOX(combo1[0]), "Group Wizard");
	  gtk_combo_box_set_active(GTK_COMBO_BOX(combo1[0]), 0);
	  gtk_box_pack_start (GTK_BOX (vbox12), combo1[0], FALSE, FALSE, 0);*/


	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone2[0]), "Law Wizard");
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboone2[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), comboone2[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[0]), "Select Calibration");
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboone3[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), comboone3[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo3[0]), "A");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo3[0]), "B");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo3[0]), "I");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo3[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo3[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "A");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "B");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "C");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "S");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "A-B");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "A-B-C");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "A-B-S");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "A-C-[C]");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[0]), "A-S-[C]");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo5[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo5[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[0]), "1");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[0]), "2");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[0]), "3");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo6[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo6[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo7[0]), "Sectorial");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo7[0]), "Linear");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo7[0]), "Linear at 0");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo7[0]), "Custom");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo7[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo7[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo8[0]), "1");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo8[0]), "2");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo8[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo8[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo9[0]), "Storage Card");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo9[0]), "Internal Memory");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo9[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo9[0], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo10[0]), "Millmeters");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo10[0]), "Inches");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo10[0]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo10[0], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (vbox10), vbox12, FALSE, FALSE, 5);
	gtk_widget_show(vbox10);




	vbox12 = gtk_vbox_new(FALSE, 0);
	toolbutton2[1] = gtk_button_new_with_label("Start\n(mm)");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton2[1]), 110, 42);
	gtk_widget_modify_bg(toolbutton2[1], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start (GTK_BOX (vbox12), toolbutton2[1], FALSE, FALSE, 0);

	label[1]=gtk_label_new("Group Wizard");
	gtk_box_pack_start (GTK_BOX (vbox12), label[1], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (50.0, -37.25, 250.0, 0.01, 10.0, 0.0);
	spinner[1] = gtk_spin_button_new (adj, 0, 2);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner[1], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (25, 1, 97, 1, 10.0, 0.0);
	spinner7[1] = gtk_spin_button_new (adj, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner7[1], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (25, 1, 100, 1, 10.0, 0.0);
	spinner10[1] = gtk_spin_button_new (adj, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner10[1], FALSE, FALSE, 0);

	gtk_widget_show(toolbutton2[1]);
	gtk_widget_show(spinner[1]);
	gtk_widget_show(vbox12);

	/*gtk_combo_box_append_text(GTK_COMBO_BOX(combo1[1]), "Group Wizard");
	  gtk_combo_box_set_active(GTK_COMBO_BOX(combo1[1]), 0);
	  gtk_box_pack_start (GTK_BOX (vbox12), combo1[1], FALSE, FALSE, 0);*/


	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone2[1]), "Law Wizard");
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboone2[1]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), comboone2[1], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[1]), "Select Calibration");
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboone3[1]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), comboone3[1], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo3[1]), "Position");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo3[1]), "Mode");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo3[1]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo3[1], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[1]), "Current");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo5[1]), "All");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo5[1]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo5[1], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[1]), "UT");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[1]), "PA");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo6[1]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo6[1], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo8[1]), "Normal");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo8[1]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo8[1], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo9[1]), "   ");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo9[1]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo9[1], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (vbox10), vbox12, FALSE, FALSE, 5);
	gtk_widget_show(vbox10);





	vbox12 = gtk_vbox_new(FALSE, 0);
	toolbutton2[2] = gtk_button_new_with_label("Range\n(mm)");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton2[2]), 110, 42);
	gtk_widget_modify_bg(toolbutton2[2], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start (GTK_BOX (vbox12),toolbutton2[2], FALSE, FALSE, 0);
	adj = (GtkAdjustment *) gtk_adjustment_new (250.0, 0.0, 500.0, 0.1, 10.0, 0.0);
	spinner[2] = gtk_spin_button_new (adj, 0, 1);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner[2], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (19.97, -47.65, 500.0, 0.01, 10.0, 0.0);
	spinner3[2] = gtk_spin_button_new (adj, 0, 2);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner3[2], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (20, 1, 97, 1, 10.0, 0.0);
	spinner7[2] = gtk_spin_button_new (adj, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner7[2], FALSE, FALSE, 0);

	gtk_widget_show(toolbutton2[2]);
	gtk_widget_show(spinner[2]);
	gtk_widget_show(vbox12);

	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[2]), "Encoder");
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[2]), "Ultrasound");
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[2]), "Code");
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboone3[2]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), comboone3[2], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[2]), "Select Tx/Rx");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo6[2]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo6[2], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo8[2]), "Quad");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo8[2]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo8[2], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo9[2]), "   ");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo9[2]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo9[2], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo10[2]), "   ");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo10[2]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo10[2], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (vbox10), vbox12, FALSE, FALSE, 5);
	gtk_widget_show(vbox10);





	vbox12 = gtk_vbox_new(FALSE, 0);
	toolbutton2[3] = gtk_button_new_with_label("Wedge Delay\n        (us)");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton2[3]), 110, 42);
	gtk_widget_modify_bg(toolbutton2[3], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start (GTK_BOX (vbox12), toolbutton2[3], FALSE, FALSE, 0);
	adj = (GtkAdjustment *) gtk_adjustment_new (0.5, 0.0, 5.0, 0.1, 10.0, 0.0);
	spinner[3] = gtk_spin_button_new (adj, 0, 1);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner[3], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (60.00, 0.10, 150.00, 0.01, 10.0, 0.0);
	spinner3[3] = gtk_spin_button_new (adj, 0, 2);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner3[3], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (1.000, 0.001, 5.000, 0.001, 10.0, 0.0);
	spinner8[3] = gtk_spin_button_new (adj, 0, 3);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner8[3], FALSE, FALSE, 0);

	gtk_widget_show(toolbutton2[3]);
	gtk_widget_show(spinner[3]);
	gtk_widget_show(vbox12);

	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[3]), "Velocity");
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[3]), "Wedge Delay");
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[3]), "Sensitivity");
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboone3[3]), "TCG");
	gtk_combo_box_set_active(GTK_COMBO_BOX(comboone3[3]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), comboone3[3], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[3]), "5L64-A2");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo6[3]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo6[3], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo9[3]), "   ");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo9[3]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo9[3], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo10[3]), "Indoor");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo10[3]), "Outdoor");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo10[3]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo10[3], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (vbox10), vbox12, FALSE, FALSE, 5);
	gtk_widget_show(vbox10);





	vbox12 = gtk_vbox_new(FALSE, 0);
	toolbutton2[4] = gtk_button_new_with_label("Velocity\n  (m/s)");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton2[4]), 110, 42);
	gtk_widget_modify_bg(toolbutton2[4], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start (GTK_BOX (vbox12), toolbutton2[4], FALSE, FALSE, 0);
	adj = (GtkAdjustment *) gtk_adjustment_new (1000.0, 500.0, 5000.0, 1.0, 10.0, 0.0);
	spinner[4] = gtk_spin_button_new (adj, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner[4], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (50, 0.1, 98, 1.0, 10.0, 0.0);
	spinner3[4] = gtk_spin_button_new (adj, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner3[4], FALSE, FALSE, 0);

	adj = (GtkAdjustment *) gtk_adjustment_new (0.00, -5.00, 5.00, 0.01, 10.0, 0.0);
	spinner8[4] = gtk_spin_button_new (adj, 0, 2);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner8[4], FALSE, FALSE, 0);

	gtk_widget_show(toolbutton2[4]);
	gtk_widget_show(spinner[4]);
	gtk_widget_show(vbox12);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[4]), "SA2-OL 2L64");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo6[4]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo6[4], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo9[4]), "Inspection Data");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo9[4]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo9[4], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo10[4]), "ByGroup");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo10[4]), "ByContext");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo10[4]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo10[4], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (vbox10), vbox12, FALSE, FALSE, 5);
	gtk_widget_show(vbox10);




	vbox12 = gtk_vbox_new(FALSE, 0);
	toolbutton2[5] = gtk_button_new_with_label("  ");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton2[5]), 110, 42);
	gtk_widget_modify_bg(toolbutton2[5], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start (GTK_BOX (vbox12), toolbutton2[5], FALSE, FALSE, 0);
	adj = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 0.0, 1.0, 10.0, 0.0);
	spinner[5] = gtk_spin_button_new (adj, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox12), spinner[5], FALSE, FALSE, 0);
	gtk_widget_show(vbox12);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[5]), "Off");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo6[5]), "ON");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo6[5]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo6[5], FALSE, FALSE, 0);

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo9[5]), "   ");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo9[5]), 0);
	gtk_box_pack_start (GTK_BOX (vbox12), combo9[5], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (vbox10), vbox12, FALSE, FALSE, 5);
	gtk_widget_show(vbox10);






	//------------vbox11----------------
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 690, 80);
	g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(gll_expose_event), NULL);
	gtk_box_pack_start (GTK_BOX (vbox11), drawing_area, FALSE, FALSE, 0);
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &color_red);
	gtk_widget_show(drawing_area);


	gtk_box_pack_start (GTK_BOX (vbox11), vbox30, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET (vbox30), 690, 461);
	gtk_box_pack_start (GTK_BOX (vbox11), hbox20, FALSE, FALSE, 0);
	gtk_widget_set_size_request (GTK_WIDGET (hbox20), 690, 60);
	gtk_widget_modify_bg(hbox20, GTK_STATE_NORMAL, &color_red);

	menubar=gtk_menu_bar_new();
	menuitem1=gtk_menu_item_new_with_label(" UT Settings");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1), 110, 60);
	gtk_widget_modify_fg(menuitem1, GTK_STATE_NORMAL, &color_red);
	gtk_widget_modify_bg(menuitem1, GTK_STATE_NORMAL, &color_green);
	gtk_menu_bar_append(GTK_MENU_BAR(menubar),menuitem1);

	menu1=gtk_menu_new();//建立菜单
	menuitem1_1=gtk_menu_item_new_with_label("  Wizard  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_1), 110, 45);
	gtk_widget_modify_bg(menuitem1_1, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_1), "activate", G_CALLBACK(menuitem1_function), "  Wizard");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_1);

	menuitem1_2=gtk_menu_item_new_with_label("  UT Settings  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_2), 110, 45);
	gtk_widget_modify_bg(menuitem1_2, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_2), "activate", G_CALLBACK(menuitem2_function), "  UT Settings");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_2);
	menuitem1_3=gtk_menu_item_new_with_label("  Gate/Alarm  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_3), 110, 45);
	gtk_widget_modify_bg(menuitem1_3, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_3), "activate", G_CALLBACK(menuitem3_function), "  Gate/Alarm");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_3);
	menuitem1_4=gtk_menu_item_new_with_label("  Measurements  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_4), 110, 45);
	gtk_widget_modify_bg(menuitem1_4, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_4), "activate", G_CALLBACK(menuitem4_function), "  Measurements");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_4);
	menuitem1_5=gtk_menu_item_new_with_label("  Display  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_5), 110, 45);
	gtk_widget_modify_bg(menuitem1_5, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_5), "activate", G_CALLBACK(menuitem5_function), "  Display");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_5);
	menuitem1_6=gtk_menu_item_new_with_label("  Probe/part  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_6), 110, 45);
	gtk_widget_modify_bg(menuitem1_6, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_6), "activate", G_CALLBACK(menuitem6_function), "  Probe/part");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_6);
	menuitem1_7=gtk_menu_item_new_with_label("  Focal Law  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_7), 110, 45);
	gtk_widget_modify_bg(menuitem1_7, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_7), "activate", G_CALLBACK(menuitem7_function), "  Focal Law");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_7);
	menuitem1_8=gtk_menu_item_new_with_label("  Scan  ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_8), 110, 45);
	gtk_widget_modify_bg(menuitem1_8, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_8), "activate", G_CALLBACK(menuitem8_function), "  Scan");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_8);
	menuitem1_9=gtk_menu_item_new_with_label("  File ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_9), 110, 45);
	gtk_widget_modify_bg(menuitem1_9, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_9), "activate", G_CALLBACK(menuitem9_function), "  File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_9);
	menuitem1_10=gtk_menu_item_new_with_label("  Preferences ");
	gtk_widget_set_size_request(GTK_WIDGET(menuitem1_10), 110, 45);
	gtk_widget_modify_bg(menuitem1_10, GTK_STATE_NORMAL, &color_green);
	g_signal_connect(G_OBJECT(menuitem1_10), "activate", G_CALLBACK(menuitem10_function), "  Preferences");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu1),menuitem1_10);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem1),menu1);//最后把菜单menu1粘到菜单项menuitem1上

	gtk_box_pack_start(GTK_BOX(hbox20),menubar,FALSE,FALSE,0);
	g_object_set (menubar, 
			"pack-direction", GTK_PACK_DIRECTION_LTR,
			NULL); 


	gtk_widget_show_all(vbox11);

	color_green.red = 0x7e00, color_green.green = 0xb700, color_green.blue = 0xf300;      

	toolbutton1[0] = gtk_button_new_with_label("General");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton1[0]), 115, 60);
	gtk_widget_modify_bg(toolbutton1[0], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start(GTK_BOX(hbox20), toolbutton1[0],FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(toolbutton1[0]), "clicked", G_CALLBACK(tollbutton1_function), "A");
	gtk_widget_show(toolbutton1[0]);

	toolbutton1[1] = gtk_button_new_with_label("Pulser");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton1[1]), 115, 60);
	gtk_widget_modify_bg(toolbutton1[1], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start(GTK_BOX(hbox20), toolbutton1[1], FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(toolbutton1[1]), "clicked", G_CALLBACK(tollbutton2_function), "B");
	gtk_widget_show(toolbutton1[1]);

	toolbutton1[2] = gtk_button_new_with_label("Receiver");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton1[2]), 115, 60);
	gtk_widget_modify_bg(toolbutton1[2], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start(GTK_BOX(hbox20), toolbutton1[2], FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(toolbutton1[2]), "clicked", G_CALLBACK(tollbutton3_function), "C");
	gtk_widget_show(toolbutton1[2]);

	toolbutton1[3] = gtk_button_new_with_label("Beam");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton1[3]), 115, 60);
	gtk_widget_modify_bg(toolbutton1[3], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start(GTK_BOX(hbox20),toolbutton1[3],FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(toolbutton1[3]), "clicked", G_CALLBACK(tollbutton4_function), "D");
	gtk_widget_show(toolbutton1[3]);

	toolbutton1[4] = gtk_button_new_with_label("Advanced");
	gtk_widget_set_size_request(GTK_WIDGET(toolbutton1[4]), 115, 60);
	gtk_widget_modify_bg(toolbutton1[4], GTK_STATE_NORMAL, &color_green);
	gtk_box_pack_start(GTK_BOX(hbox20),toolbutton1[4],FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(toolbutton1[4]), "clicked", G_CALLBACK(tollbutton5_function), "E");
	gtk_widget_show(toolbutton1[4]);


	draw_area_init(&tan1, vbox30, 0, 0, 400,426);

#endif

	return 0;
}
