
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include <glib/gprintf.h>

GdkPoint a[512] = {{0,0},{240,200},{400,200}};

void change_language();
void draw_1_menu(DRAW_UI_P p);
void draw_2_menu(DRAW_UI_P p);
void draw_3_menu(DRAW_UI_P p);
void init_ui(DRAW_UI_P p);				/*初始化界面,*/

/**/
const gchar **con0_p	= content_en10;
const gchar ***con1_p	= content1_en;
const gchar ****con2_p	= content2_en;

void menuitem0_function(GtkMenuItem *menuitem, gpointer data);
void menuitem1_function(GtkMenuItem *menuitem, gpointer data);
void menuitem2_function(GtkMenuItem *menuitem, gpointer data);
void menuitem3_function(GtkMenuItem *menuitem, gpointer data);
void menuitem4_function(GtkMenuItem *menuitem, gpointer data);
void menuitem5_function(GtkMenuItem *menuitem, gpointer data);
void menuitem6_function(GtkMenuItem *menuitem, gpointer data);
void menuitem7_function(GtkMenuItem *menuitem, gpointer data);
void menuitem8_function(GtkMenuItem *menuitem, gpointer data);
void menuitem9_function(GtkMenuItem *menuitem, gpointer data);

void (*menu_fun[10])(GtkMenuItem *menuitem, gpointer data) = 
{
	menuitem0_function,	menuitem1_function,
	menuitem2_function,	menuitem3_function,
	menuitem4_function,	menuitem5_function,
	menuitem6_function,	menuitem7_function,
	menuitem8_function,	menuitem9_function
};

/*1级菜单 回调函数*/
void menuitem0_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 0;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem1_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 1;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem2_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	p->pos = 2;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem3_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 3;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem4_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	p->pos = 4;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem5_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 5;
	//p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem6_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 6;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d \n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem7_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 7;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem8_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 8;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem9_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 9;
	// p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

/**/
void change_language()
{
	return ;
}

/* 画一级菜单*/
void draw_1_menu(DRAW_UI_P p)
{
	gint i;

	for (i = 0; i < 10; i++)
		gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem[i]),con0_p[i]);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), con0_p[p->pos]);
}

/* 画二级菜单*/
void draw_2_menu(DRAW_UI_P p)
{
	gint i;

	for ( i = 0 ; i < 5 ; i++) 
	{
		if (con1_p[p->pos][i]) 
		{
			gtk_button_set_label (GTK_BUTTON (p->button2[i]), con1_p[p->pos][i]);
			gtk_widget_modify_bg (p->button2[i], GTK_STATE_NORMAL, &color_button1);
			gtk_widget_show (p->button2[i]);
		}
		else
		{
			gtk_button_set_label (GTK_BUTTON (p->button2[i]), " ");
			gtk_widget_hide (p->button2[i]);
		}
	}
	gtk_widget_modify_bg (p->button2[p->pos1[p->pos]], GTK_STATE_NORMAL, &color_button0);
}

/**/
void draw3_data0(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0:/*增益 Gain */
					/*当前步进*/
					switch (p->p_tmp_config->db_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 0.5; break;
						case 2:	tmpf = 1.0; break;
						case 3:	tmpf = 2.0; break;
						case 4:	tmpf = 6.0; break;
						default:break;
					}
					if (p->pos2[p->pos][p->pos1[p->pos]] == 0)
						g_sprintf (temp,"%s\ndB Δ%.1f", con2_p[1][0][0], tmpf);
					else 
						g_sprintf (temp,"%s\ndB", con2_p[1][0][0]);

					gtk_button_set_label (GTK_BUTTON (p->button3[0]), temp);
					gtk_widget_modify_bg (p->button3[0], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[0]);

					p->adj = (GtkAdjustment *) gtk_adjustment_new
						(p->p_config->gain / 10.0, 0.0, 74.0, tmpf, 10.0, 0.0);
					p->data[0] = gtk_spin_button_new (p->adj, 0, 1);
					gtk_box_pack_start (GTK_BOX (p->vbox221[0]), p->data[0], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[0]), 115, 29);
					gtk_widget_show(p->data[0]);
					g_signal_connect(G_OBJECT(p->data[0]), "value-changed", 
							G_CALLBACK(data_100), (gpointer) (p));
					//					 gtk_widget_set_can_focus (p->data[0], FALSE);
					//g_signal_connect(G_OBJECT(p->data[0]), "button-press-event", 
				//		G_CALLBACK(data_fun[0]), (gpointer) (p));
					break;
				case 1:/*发射 Pulser*/
					/*当前步进*/
					switch (p->p_tmp_config->pulser_reg)
					{
						case 0:	tmpf = 0.0; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;
						default:break;
					}
					if (p->pos2[p->pos][p->pos1[p->pos]] == 0)
						g_sprintf (temp,"%s\n(1 to 100) Δ%d", con2_p[1][1][0], (guint)(tmpf));
					else 
						g_sprintf (temp,"%s\n(1 to 100)", con2_p[1][1][0]);

					gtk_button_set_label (GTK_BUTTON (p->button3[0]), temp);
					gtk_widget_modify_bg (p->button3[0], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[0]);

					p->adj = (GtkAdjustment *) gtk_adjustment_new
						(p->p_config->pulser , 0.0, 100.0, tmpf, 10.0, 0.0);
					p->data[0] = gtk_spin_button_new (p->adj, 0, 0);
					gtk_box_pack_start (GTK_BOX (p->vbox221[0]), p->data[0], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[0]), 115, 29);
					gtk_widget_show(p->data[0]);
					g_signal_connect(G_OBJECT(p->data[0]), "value-changed", 
							G_CALLBACK(data_110), (gpointer) (p));

					break;
				case 2:
					g_sprintf (temp,"%s\n(1 to 100)", con2_p[1][2][0]);

					gtk_button_set_label (GTK_BUTTON (p->button3[0]), temp);
					gtk_widget_modify_bg (p->button3[0], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[0]);

					p->adj = (GtkAdjustment *) gtk_adjustment_new
						(p->p_config->pulser , p->p_config->pulser, p->p_config->pulser, 1.0, 10.0, 0.0);
					p->data[0] = gtk_spin_button_new (p->adj, 0, 0);
					gtk_box_pack_start (GTK_BOX (p->vbox221[0]), p->data[0], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[0]), 115, 29);
					gtk_widget_show(p->data[0]);
//					g_signal_connect(G_OBJECT(p->data[0]), "value-changed", 
//							G_CALLBACK(data_110), (gpointer) (p));
					break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data1(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0:
					/*当前步进*/
					switch (p->p_tmp_config->start_reg)
					{
						case 0:	tmpf = p->p_config->range / (1 * 1000.0); break;
						case 1:	tmpf = p->p_config->range / (2 * 1000.0); break;
						case 2:	tmpf = p->p_config->range / (32 * 1000.0); break;
						default:break;
					}
					if (p->pos2[p->pos][p->pos1[p->pos]] == 1)
					{
						if (p->p_config->ut_unit == 0)
							g_sprintf (temp, "%s\n(%s) Δ%.2f", 
									con2_p[1][0][1], 
									!p->p_config->unit ? "mm" : "inch", tmpf * p->p_config->part.Velocity / 20000.0);
					}
					else 
					{
						if (p->p_config->ut_unit == 0)
							g_sprintf (temp, "%s\n(%s)",
									con2_p[1][0][1], !p->p_config->unit ? "mm" : "inch");
					}

					gtk_button_set_label (GTK_BUTTON (p->button3[1]), temp);
					gtk_widget_modify_bg (p->button3[1], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[1]);

					if (p->p_config->ut_unit == 0) 
					{
						p->adj = (GtkAdjustment *) gtk_adjustment_new
							((p->p_config->start / 100.0) * (p->p_config->part.Velocity / 20000.0),
							 -0.047 * (p->p_config->part.Velocity / 20000.0),
							 ((9896.1 - ((p->p_config->range > 0) ? (p->p_config->range / 100.0) : 0)) * (p->p_config->part.Velocity / 20000.0)),
							  tmpf * p->p_config->part.Velocity / 20000.0, 10.0, 0.0);
					}
					else;
					p->data[1] = gtk_spin_button_new (p->adj, 0, 2);
					gtk_box_pack_start (GTK_BOX (p->vbox221[1]), p->data[1], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[1]), 115, 29);
					gtk_widget_show(p->data[1]);
					g_signal_connect(G_OBJECT(p->data[1]), "value-changed", 
							G_CALLBACK(data_101), (gpointer) (p));
					break;
				case 1:/*Tx/Rx mode*/
					g_sprintf (temp, "%s", con2_p[1][1][1]);

					gtk_button_set_label (GTK_BUTTON (p->button3[1]), temp);
					gtk_widget_modify_bg (p->button3[1], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[1]);

					p->data[1] = gtk_combo_box_new_text();
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[1]), "PC Pitch-and-Catch");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[1]), "PE pulse-Echo");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[1]), "TT Through-Transmission");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[1]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[1]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[1]), p->data[1], FALSE, FALSE, 0);
					gtk_widget_show(p->data[1]);

					break;
				case 2:/*Filter 滤波*/
					g_sprintf (temp, "%s", con2_p[1][2][1]);

					gtk_button_set_label (GTK_BUTTON (p->button3[1]), temp);
					gtk_widget_modify_bg (p->button3[1], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[1]);

					p->data[1] = gtk_combo_box_new_text();
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[1]), "None (0.5-21.0 Mhz)");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[1]), "Auto");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[1]), "1 Mhz");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[1]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[1]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[1]), p->data[1], FALSE, FALSE, 0);
					gtk_widget_show(p->data[1]);

					break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data2(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0:
					/*当前步进*/
					switch (p->p_tmp_config->range_reg)
					{
						case 0:	tmpf = p->p_config->part.Velocity * 1.6 / 5000.0; break;
						case 1:	tmpf = p->p_config->part.Velocity * 8.0 / 5000.0; break;
						case 2:	tmpf = p->p_config->part.Velocity * 16.0 / 5000.0; break;
						default:break;
					}
					if (p->pos2[p->pos][p->pos1[p->pos]] == 2)
					{
						if (p->p_config->ut_unit == 0)
							g_sprintf (temp,"%s\n(%s) Δ%.1f", 
									con2_p[1][0][2], 
									!p->p_config->unit ? "mm" : "inch", tmpf * p->p_config->part.Velocity / 20000.0);
					}
					else 
					{
						if (p->p_config->ut_unit == 0)
							g_sprintf (temp,"%s\n(%s)",
									con2_p[1][0][2], !p->p_config->unit ? "mm" : "inch");
					}

					gtk_button_set_label (GTK_BUTTON (p->button3[2]), temp);
					gtk_widget_modify_bg (p->button3[2], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[2]);
					

					if (p->p_config->ut_unit == 0) 
					{
						p->adj = (GtkAdjustment *) gtk_adjustment_new
							((p->p_config->range / 100.0) * (p->p_config->part.Velocity / 20000.0),
							 3.2 * (p->p_config->part.Velocity / 20000.0),
							 (((9896.1 - ((p->p_config->start > 0) ? (p->p_config->start / 100.0) : 0)) > 6400.0 ? 6400.0  :
							 (9896.1 - ((p->p_config->start > 0) ? (p->p_config->start / 100.0) : 0)))
							  * (p->p_config->part.Velocity / 20000.0)),
							  tmpf * p->p_config->part.Velocity / 20000.0, 10.0, 0.0);
					}
					else;
					p->data[2] = gtk_spin_button_new (p->adj, 0, 2);
					gtk_box_pack_start (GTK_BOX (p->vbox221[2]), p->data[2], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[2]), 115, 29);
					gtk_widget_show(p->data[2]);
					g_signal_connect(G_OBJECT(p->data[2]), "value-changed", 
							G_CALLBACK(data_102), (gpointer) (p));
					break;
				case 1:/*Tx/Rx mode*/
					g_sprintf (temp, "%s", con2_p[1][1][2]);

					gtk_button_set_label (GTK_BUTTON (p->button3[2]), temp);
					gtk_widget_modify_bg (p->button3[2], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[2]);

					p->data[2] = gtk_combo_box_new_text();
					gtk_widget_modify_bg (gtk_bin_get_child(GTK_BIN (p->data[2])), 
							GTK_STATE_NORMAL, &color_button1);
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "1");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "1.5");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "2");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "20");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[2]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[2]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[2]), p->data[2], FALSE, FALSE, 0);
					gtk_widget_show(p->data[2]);

					break;
				case 2: /*检波*/
					g_sprintf (temp, "%s", con2_p[1][2][2]);

					gtk_button_set_label (GTK_BUTTON (p->button3[2]), temp);
					gtk_widget_modify_bg (p->button3[2], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[2]);

					p->data[2] = gtk_combo_box_new_text();
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "RF");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "HW +");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "HW -");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[2]), "FW");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[2]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[2]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[2]), p->data[2], FALSE, FALSE, 0);
					gtk_widget_show(p->data[2]);
					break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data3(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0:
					/*当前步进*/
					switch (p->p_tmp_config->wedge_delay_reg)
					{
						case 0:	tmpf = 0.01; break;
						case 1:	tmpf = 0.1; break;
						case 2:	tmpf = 1.0; break;
						default:break;
					}
					if (p->pos2[p->pos][p->pos1[p->pos]] == 3)
						g_sprintf (temp,"%s\n(%s) Δ%.2f", 
								con2_p[1][0][3], "μs", tmpf);
					else 
						g_sprintf (temp,"%s\n(%s)",
								con2_p[1][0][3], "μs");

					gtk_button_set_label (GTK_BUTTON (p->button3[3]), temp);
					gtk_widget_modify_bg (p->button3[3], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[3]);

					p->adj = (GtkAdjustment *) gtk_adjustment_new
						(p->p_config->wedge_delay / 100.0, 
						 0.0, 1000.0,
						 tmpf, 10.0, 0.0);
					p->data[3] = gtk_spin_button_new (p->adj, 0, 2);
					gtk_box_pack_start (GTK_BOX (p->vbox221[3]), p->data[3], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[3]), 115, 29);
					gtk_widget_show(p->data[3]);
					g_signal_connect(G_OBJECT(p->data[3]), "value-changed", 
							G_CALLBACK(data_103), (gpointer) (p));
					break;
				case 1: /*电压 功率 福特*/
					g_sprintf (temp, "%s", con2_p[1][1][3]);

					gtk_button_set_label (GTK_BUTTON (p->button3[3]), temp);
					gtk_widget_modify_bg (p->button3[3], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[3]);

					p->data[3] = gtk_combo_box_new_text();
					gtk_widget_modify_bg (gtk_bin_get_child(GTK_BIN (p->data[3])), 
							GTK_STATE_NORMAL, &color_button1);
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[3]), "45 Low");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[3]), "90 High");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[3]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[3]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[3]), p->data[3], FALSE, FALSE, 0);
					gtk_widget_show(p->data[3]);

					break;
				case 2: /*视频滤波 video filter*/
					g_sprintf (temp, "%s", con2_p[1][2][3]);

					gtk_button_set_label (GTK_BUTTON (p->button3[3]), temp);
					gtk_widget_modify_bg (p->button3[3], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[3]);
					if (!p->p_config->video_filter)
						p->data[3] = gtk_label_new("On");
					else
						p->data[3] = gtk_label_new("Off");

					gtk_widget_set_size_request(GTK_WIDGET(p->data[3]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[3]), p->data[3], FALSE, FALSE, 0);
					gtk_widget_show(p->data[3]);

					break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data4(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0:
					/*当前步进*/
					switch (p->p_tmp_config->velocity_reg)
					{
						case 0:	tmpf = 0.1; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;
						default:break;
					}
					if (p->pos2[p->pos][p->pos1[p->pos]] == 4)
						g_sprintf (temp,"%s\n(%s) Δ%.1f", 
								con2_p[1][0][4], !p->p_config->unit ? "m/s" : "in/μs", tmpf);
					else 
						g_sprintf (temp,"%s\n(%s)",
								con2_p[1][0][4], !p->p_config->unit ? "m/s" : "in/μs");

					gtk_button_set_label (GTK_BUTTON (p->button3[4]), temp);
					gtk_widget_modify_bg (p->button3[4], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[4]);

					p->adj = (GtkAdjustment *) gtk_adjustment_new
						(p->p_config->part.Velocity / 10.0, 
						 635, 15340,
						 tmpf, 10.0, 0.0);
					p->data[4] = gtk_spin_button_new (p->adj, 0, 1);
					gtk_box_pack_start (GTK_BOX (p->vbox221[4]), p->data[4], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[4]), 115, 29);
					gtk_widget_show(p->data[4]);
					g_signal_connect(G_OBJECT(p->data[4]), "value-changed", 
							G_CALLBACK(data_104), (gpointer) (p));
					break;
				case 1: /*脉冲宽度 pulser width*/
					g_sprintf (temp, "%s", con2_p[1][1][4]);

					gtk_button_set_label (GTK_BUTTON (p->button3[4]), temp);
					gtk_widget_modify_bg (p->button3[4], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[4]);

					p->data[4] = gtk_combo_box_new_text();
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[4]), "Auto");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[4]), "User define");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[4]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[4]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[4]), p->data[4], FALSE, FALSE, 0);
					gtk_widget_show(p->data[4]);

					break;
				case 2: /*平均 averaging*/
					g_sprintf (temp, "%s", con2_p[1][2][4]);

					gtk_button_set_label (GTK_BUTTON (p->button3[4]), temp);
					gtk_widget_modify_bg (p->button3[4], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[4]);

					p->data[4] = gtk_combo_box_new_text();
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[4]), "1");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[4]), "2");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[4]), "4");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[4]), "8");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[4]), "16");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[4]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[4]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[4]), p->data[4], FALSE, FALSE, 0);
					gtk_widget_show(p->data[4]);

					break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		default:break;
	}
}

void draw3_data5(DRAW_UI_P p) 
{
	gchar temp[52];
	gfloat tmpf;/**/

	switch (p->pos) 
	{
		case 0:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 1:
			switch (p->pos1[1])
			{
				case 0: /*空白的地方*/
					break;
				case 1: /*重复频率*/
					g_sprintf (temp, "%s", con2_p[1][1][5]);

					gtk_button_set_label (GTK_BUTTON (p->button3[5]), temp);
					gtk_widget_modify_bg (p->button3[5], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[5]);

					p->data[5] = gtk_combo_box_new_text();
					gtk_widget_modify_bg (gtk_bin_get_child(GTK_BIN (p->data[5])), 
							GTK_STATE_NORMAL, &color_button1);
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[5]), "Auto Max");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[5]), "Max/2");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[5]), "Optimum");
					gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[5]), "User define");
					gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[5]), 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[5]), 115, 29);
					gtk_box_pack_start (GTK_BOX (p->vbox221[5]), p->data[5], FALSE, FALSE, 0);
					gtk_widget_show(p->data[5]);

					break;
				case 2:
					switch (p->p_tmp_config->reject_reg)
					{
						case 0:	tmpf = 0.0; break;
						case 1:	tmpf = 1.0; break;
						case 2:	tmpf = 10.0; break;
						case 3:	tmpf = 100.0; break;
						default:break;
					}
					if (p->pos2[p->pos][p->pos1[p->pos]] == 5)
						g_sprintf (temp,"%s\n(1 to 100) Δ%d", con2_p[1][2][5], (guint)(tmpf));
					else 
						g_sprintf (temp,"%s\n(1 to 100)", con2_p[1][2][5]);

					gtk_button_set_label (GTK_BUTTON (p->button3[5]), temp);
					gtk_widget_modify_bg (p->button3[5], GTK_STATE_NORMAL, &color_button1);
					gtk_widget_show (p->button3[5]);

					p->adj = (GtkAdjustment *) gtk_adjustment_new
						(p->p_config->pulser , 0.0, 100.0, tmpf, 10.0, 0.0);
					p->data[5] = gtk_spin_button_new (p->adj, 0, 0);
					gtk_box_pack_start (GTK_BOX (p->vbox221[5]), p->data[5], FALSE, FALSE, 0);
					gtk_widget_set_size_request(GTK_WIDGET(p->data[5]), 115, 29);
					gtk_widget_show(p->data[5]);
					g_signal_connect(G_OBJECT(p->data[5]), "value-changed", 
							G_CALLBACK(data_110), (gpointer) (p));
					break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 2:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 4:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 5:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 7:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		case 9:
			switch (p->pos1[0])
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				default:break;
			}
			break;
		default:break;
	}
}

/* 画三级菜单*/
void draw_3_menu(DRAW_UI_P p)
{
	gint i;

	for ( i = 0 ; i < 6 ; i++) 
	{
		if (p->data[i]) 
		{
			gtk_widget_destroy (p->data[i]);
			p->data[i] = NULL;
		}

		if (con2_p[p->pos][p->pos1[p->pos]][i]) 
		{

			/*
			   gtk_button_set_label (GTK_BUTTON (p->button3[i]), 
			   con2_p[p->pos][p->pos1[p->pos]][i]);
			   gtk_widget_modify_bg (p->button3[i], GTK_STATE_NORMAL, &color_button1);
			   gtk_widget_show (p->button3[i]);
			   */

			if (i == 0)
				draw3_data0(p);		/*第一个数值*/
			else if (i == 1)
				draw3_data1(p);
			else if (i == 2)
				draw3_data2(p);
			else if (i == 3)
				draw3_data3(p);
			else if (i == 4)
				draw3_data4(p);
			else if (i == 5)
				draw3_data5(p);

#if 0			
			else if (i == 1)
			{
				p->data[i] = gtk_combo_box_entry_new_text();
				gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[i]), "Law Wizard");
				gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[i]), "Law Wizard");
				gtk_combo_box_append_text(GTK_COMBO_BOX (p->data[i]), "Law Wizariiiiiiiiiiiiid");
				gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[i]), 0);
				gtk_widget_set_size_request(GTK_WIDGET(p->data[i]), 115, 29);
				//				gtk_combo_box_set_wrap_width (GTK_COMBO_BOX (p->data[i]), 115);
				gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->data[i], FALSE, FALSE, 0);
				gtk_widget_show(p->data[i]);

				//				gtk_widget_set_can_focus (p->data[i], FALSE);

				g_object_set (gtk_bin_get_child(GTK_BIN (p->data[i])), "editable", FALSE, NULL);
				g_signal_connect(G_OBJECT(p->data[i]), "button-press-event", 
						G_CALLBACK(data_fun[i]), (gpointer) (p));
				g_signal_connect(G_OBJECT(gtk_bin_get_child(GTK_BIN (p->data[i]))), 
						"button-press-event", G_CALLBACK(data_fun[i]), (gpointer) (p));
			}
			else 
			{
				p->data[i] = gtk_combo_box_entry_new_text();
				gtk_combo_box_set_active(GTK_COMBO_BOX (p->data[i]), 0);
				gtk_widget_set_size_request(GTK_WIDGET(p->data[i]), 115, 29);
				//				gtk_combo_box_set_wrap_width (GTK_COMBO_BOX (p->data[i]), 115);
				gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->data[i], FALSE, FALSE, 0);
				gtk_widget_show(p->data[i]);

				//				gtk_widget_set_can_focus (p->data[i], FALSE);

				g_signal_connect(G_OBJECT(p->data[i]), "button-press-event", 
						G_CALLBACK(data_fun[i]), (gpointer) (p));
			}
#endif

		}
		else
		{
			gtk_button_set_label (GTK_BUTTON (p->button3[i]), " ");
			gtk_widget_hide (p->button3[i]);
		}
	}
	gtk_widget_modify_bg (p->button3[p->pos2[p->pos][p->pos1[p->pos]]],
			GTK_STATE_NORMAL, &color_button0);
	gtk_widget_modify_bg (p->data[p->pos2[p->pos][p->pos1[p->pos]]],
			GTK_STATE_NORMAL, &color_button0);
#if 0
	g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],
			0, "is-focus", TRUE,	NULL); 
	g_object_set ( p->data[p->pos2[p->pos][p->pos1[p->pos]]],
				0, "editable", FALSE, NULL); 
#endif
}


static void travel_path (cairo_t *cr)
{
	gint i;

	//   cairo_set_source_rgb (cr, 1, 1, 1);
	//   cairo_paint (cr);

	/* Use IBM Blue Pen Color with no ALPHA */
	cairo_set_source_rgba (cr, 1, 0, 0, 1);
	cairo_set_line_width (cr, 1);
	//   cairo_move_to (cr, 10, 10);
	//   cairo_line_to (cr, 11, 200);
	//   cairo_set_line_width (cr, 1);
	cairo_stroke (cr);

	//   cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

	//	   cairo_move_to (cr, a3[i].x, a3[i].y);
	//	   cairo_move_to (cr, a3[0].x, a3[0].y);
	for (i = 0; i < 500; i++) {
		cairo_move_to (cr, a[i].x + 0.5, a[i].y );
		cairo_line_to (cr, a[i].x + 0.5, (a[i + 1].y + a[i].y)/2); 
		cairo_move_to (cr, a[i + 1].x + 0.5, (a[i + 1].y + a[i].y)/2); 
		cairo_line_to (cr, a[i + 1].x + 0.5, a[i + 1].y); 
	}

	cairo_stroke(cr);
}

static void draw_gtk (GtkWidget      *widget, GdkEventExpose *eev, gpointer        data)
{
	cairo_t *cr;
	cr = gdk_cairo_create (widget->window);
	travel_path (cr);
	cairo_destroy (cr);
}

static gboolean time_handler(GtkWidget *widget)
{
	gint32 i;


	while (1) {
		for ( i = 0; i < 512; i++)
			a[i].y = g_random_int_range(1, 512);

		gdk_threads_enter();
		gtk_widget_queue_draw(widget);
		gdk_threads_leave();
		g_usleep(100000);
	}
	g_thread_exit(NULL);
	return TRUE;
}

void init_ui(DRAW_UI_P p)				/*初始化界面,*/
{
	gint i;
	GtkWidget *drawing_area;
	GtkWidget *window = p->window;

	for (i = 0; i < 512; i++) {
		a[i].x =  i;
		a[i].y =  i;
	}


	/* New a window */
	p->pos	= 1;
	p->pos1[0]	= 0;
	p->pos2[0][0]	= 0;

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

	/*一级菜单的初始化*/
	p->menubar		= gtk_menu_bar_new();
	p->menu			= gtk_menu_new();
	p->menuitem_main	= gtk_menu_item_new_with_label(content_en10[1]);
	gtk_widget_set_size_request(GTK_WIDGET(p->menuitem_main), 110, 60);
	/*	gtk_widget_modify_fg(p->menuitem_main, GTK_STATE_NORMAL, &color_red);
		gtk_widget_modify_bg(p->menuitem_main, GTK_STATE_NORMAL, &color_green);*/
	gtk_menu_bar_append(GTK_MENU_BAR(p->menubar), p->menuitem_main);
	for (i = 0; i < 10; i++)
	{
		p->menuitem[i]	= gtk_menu_item_new_with_label(content_en10[i]);
		gtk_widget_set_size_request(GTK_WIDGET(p->menuitem[i]), 110, 45);
		/*		gtk_widget_modify_bg(p->menuitem[i], GTK_STATE_NORMAL, &color_green);*/
		g_signal_connect(G_OBJECT(p->menuitem[i]), "activate", 
				G_CALLBACK(menu_fun[i]), (gpointer) (p));
		gtk_menu_shell_append(GTK_MENU_SHELL(p->menu), p->menuitem[i]);
		gtk_widget_show(p->menuitem[i]);
	}
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(p->menuitem_main), p->menu);/*最后把菜单menu1粘到菜单项menuitem1上*/
	gtk_box_pack_start(GTK_BOX(p->hbox212), p->menubar, FALSE, FALSE, 0);
	g_object_set (p->menubar, "pack-direction", GTK_PACK_DIRECTION_LTR, NULL); 
	gtk_widget_show(p->menubar);
	gtk_widget_show(p->menu);
	gtk_widget_show(p->menuitem_main);



	/*二级菜单的初始化*/
	for (i = 0; i < 5; i++) 
	{
		p->button2[i] = gtk_button_new();
		gtk_widget_set_size_request(GTK_WIDGET(p->button2[i]), 114, 60);
		/*		color_green.red = 0x7e00, color_green.green = 0xb700, color_green.blue = 0xf300;      
				gtk_widget_modify_bg(p->button2[i], GTK_STATE_NORMAL, &color_green); */
		g_signal_connect(G_OBJECT(p->button2[i]), "clicked", 
				G_CALLBACK(button2_fun[i]), (gpointer) (p));

		g_signal_connect(G_OBJECT(p->button2[i]), "focus-in-event", 
				G_CALLBACK(button20_fun[i]), (gpointer) (p));
		gtk_box_pack_start(GTK_BOX(p->hbox212), p->button2[i], FALSE, FALSE, 0);
		gtk_widget_show(p->button2[i]);
	}


	/*三级菜单的初始化*/
	for (i = 0; i < 6; i++)
	{
		p->button3[i] = gtk_button_new();
		p->adj = (GtkAdjustment *) gtk_adjustment_new (21.0, 0.0, 74.0, 0.1, 10.0, 0.0);
		p->data[i] = gtk_spin_button_new (p->adj, 0, 1);
		gtk_widget_set_size_request(GTK_WIDGET(p->button3[i]), 115, 58);
		gtk_widget_set_size_request(GTK_WIDGET(p->data[i]), 115, 29);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->button3[i], FALSE, FALSE, 0);
		g_signal_connect(G_OBJECT(p->button3[i]), "clicked", 
				G_CALLBACK(button3_fun[i]), (gpointer) (p));
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->data[i], FALSE, FALSE, 0);
		gtk_widget_show(p->button3[i]);
		gtk_widget_show(p->data[i]);
	}




	gtk_container_add(GTK_CONTAINER(window), p->vbox);
	gtk_widget_show(p->vbox);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox1, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox1);
	gtk_box_pack_start (GTK_BOX (p->vbox), p->hbox2, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox2);



	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 800, 78);
	gtk_box_pack_start (GTK_BOX (p->hbox1), drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x5555, p->col.green = 0x5555, p->col.blue = 0x5555;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show(drawing_area);

	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox21, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->hbox2), p->vbox22, FALSE, FALSE, 0);
	gtk_widget_show(p->vbox21);
	gtk_widget_show(p->vbox22);

	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox211, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (p->vbox21), p->hbox212, FALSE, FALSE, 0);
	gtk_widget_show(p->hbox211);
	gtk_widget_show(p->hbox212);

	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 685, 460);
	gtk_box_pack_start (GTK_BOX (p->hbox211), drawing_area, FALSE, FALSE, 0);
	p->col.red = 0x5555, p->col.green = 0x0, p->col.blue = 0x5555;
	gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &(p->col));
	gtk_widget_show(drawing_area);
	g_signal_connect(G_OBJECT (drawing_area), "expose_event", G_CALLBACK(draw_gtk), NULL);

	for (i = 0; i < 6; i++)
	{
		gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221[i], FALSE, FALSE, 0);
		gtk_widget_show(p->vbox221[i]);
	}

	draw_1_menu(p);
	draw_2_menu(p);
	draw_3_menu(p);


	g_thread_create((GThreadFunc)(time_handler), (gpointer) (drawing_area), FALSE, NULL);
	/*
	   gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221, FALSE, FALSE, 0);
	   gtk_widget_show(p->vbox221);
	   */

}
