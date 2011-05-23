
/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#ifndef	__DRAW_FB_H_
#define	__DRAW_FB_H_

#define FB_DEVICE "/dev/fb1"

#define FB_WIDTH 768

/*extern gfloat HEIGHT_TABLE[];*/

extern void init_fb ();
/*extern void change_fb ();*/
extern void init_mem ();
extern void fbdot(gushort *p, gint x, gint y, gushort col);
extern void fbliney(gushort *p, gint x, gint y1, gint y2, gushort col);
extern void fblinex(gushort *p, gint y, gint x1, gint x2, gushort col);
extern void fbline(gushort *p, gint x1, gint y1, gint x2, gint y2, gushort col);

extern void draw_scan(guchar scan_num, guchar scan_type, guchar group,
		guint xoff, guint yoff, guchar *dot_temp, gushort *dot_temp1);

#endif
