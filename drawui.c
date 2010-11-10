
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#include "drawui.h"

void change_language();
void draw_1_menu(DRAW_UI_P p);
void draw_2_menu(DRAW_UI_P p);
void draw_3_menu(DRAW_UI_P p);
void init_ui(DRAW_UI_P p);				/*初始化界面,*/

/**/
static const gchar **con0_p		= content_en10;
static const gchar ***con1_p	= content1_en;
static const gchar ****con2_p	= content2_en;

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
	p->pos = 0, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem1_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 1, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem2_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	p->pos = 2, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem3_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 3, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem4_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main),
			gtk_menu_item_get_label(menuitem));
	p->pos = 4, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem5_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 5, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem6_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 6, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d \n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem7_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 7, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem8_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 8, p->pos1 = 0, p->pos2 = 0;
	g_print("pos = %d\n", p->pos);
	draw_2_menu(p);
	draw_3_menu(p);
}

void menuitem9_function(GtkMenuItem *menuitem, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	gtk_menu_item_set_label(GTK_MENU_ITEM (p->menuitem_main), 
			gtk_menu_item_get_label(menuitem));
	p->pos = 9, p->pos1 = 0, p->pos2 = 0;
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
	gtk_widget_modify_bg (p->button2[p->pos1], GTK_STATE_NORMAL, &color_button0);
}

/* 画三级菜单*/
void draw_3_menu(DRAW_UI_P p)
{
	gint i;

	for ( i = 0 ; i < 6 ; i++) 
	{
		if (con2_p[p->pos][p->pos1][i]) 
		{
			gtk_button_set_label (GTK_BUTTON (p->button3[i]), con2_p[p->pos][p->pos1][i]);
			gtk_widget_modify_bg (p->button3[i], GTK_STATE_NORMAL, &color_button1);
			gtk_widget_show (p->button3[i]);
		}
		else
		{
			gtk_button_set_label (GTK_BUTTON (p->button3[i]), " ");
			gtk_widget_hide (p->button3[i]);
		}
	}
	gtk_widget_modify_bg (p->button3[p->pos2], GTK_STATE_NORMAL, &color_button0);
}


void init_ui(DRAW_UI_P p)				/*初始化界面,*/
{
	gint i, j, k;
	GtkWidget *drawing_area;
	GtkWidget *window = p->window;

	p->pos	= 1;
	p->pos1	= 0;
	p->pos2	= 0;

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
		gtk_box_pack_start(GTK_BOX(p->hbox212), p->button2[i], FALSE, FALSE, 0);
		gtk_widget_show(p->button2[i]);
	}


	for (i = 0; i < 6; i++)
	{
		p->button3[i] = gtk_button_new();
		gtk_widget_set_size_request(GTK_WIDGET(p->button3[i]), 115, 87);
		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->button3[i], FALSE, FALSE, 0);
		/*		gtk_box_pack_start (GTK_BOX (p->vbox221[i]), p->data[i], FALSE, FALSE, 0);*/
		gtk_widget_show(p->button3[i]);
		/*		gtk_widget_show(p->data[i]);*/
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

	for (i = 0; i < 6; i++)
	{
		gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221[i], FALSE, FALSE, 0);
		gtk_widget_show(p->vbox221[i]);
	}

	draw_1_menu(p);
	draw_2_menu(p);
	draw_3_menu(p);


	/*
	   gtk_box_pack_start (GTK_BOX (p->vbox22), p->vbox221, FALSE, FALSE, 0);
	   gtk_widget_show(p->vbox221);
	   */

}
