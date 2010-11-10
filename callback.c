
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#include "drawui.h"

void button2_function0 (GtkButton *button, gpointer data);
void button2_function1 (GtkButton *button, gpointer data);
void button2_function2 (GtkButton *button, gpointer data);
void button2_function3 (GtkButton *button, gpointer data);
void button2_function4 (GtkButton *button, gpointer data);

void (*button2_fun[5])(GtkButton *button, gpointer data) = 
{
	button2_function0,	button2_function1,	button2_function2,	button2_function3,	button2_function4
};

void button2_function0 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->pos1 = 0, p->pos2 = 0;
	draw_2_menu(p);
	draw_3_menu(p);


}

void button2_function1 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->pos1 = 1, p->pos2 = 0;
	draw_2_menu(p);
	draw_3_menu(p);

}

void button2_function2 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->pos1 = 2, p->pos2 = 0;
	draw_2_menu(p);
	draw_3_menu(p);

}

void button2_function3 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->pos1 = 3, p->pos2 = 0;
	draw_2_menu(p);
	draw_3_menu(p);

}

void button2_function4 (GtkButton *button, gpointer data)
{
	DRAW_UI_P p = (DRAW_UI_P)(data);
	p->pos1 = 4, p->pos2 = 0;
	draw_2_menu(p);
	draw_3_menu(p);

}

