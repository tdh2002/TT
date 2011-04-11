
/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#ifndef	__DRAW_FB_H_
#define	__DRAW_FB_H_

#define FB_DEVICE "/dev/fb1"

#define FB_WIDTH 768

extern gfloat HEIGHT_TABLE[];

extern void init_fb();
extern void fbdot(gushort *p, gint x, gint y, gushort col);
extern void fbliney(gushort *p, gint x, gint y1, gint y2, gushort col);
extern void fblinex(gushort *p, gint y, gint x1, gint x2, gushort col);
extern void fbline(gushort *p, gint x1, gint y1, gint x2, gint y2, gushort col);

/*  */
extern void draw_a_scan (gushort *p, guint width, guint height, 
		DOT_TYPE *data, DOT_TYPE *data1, DOT_TYPE *data2,
		guint xoffset, guint yoffset, guchar groupId);
extern void draw_a_scan_r (gushort *p, guint width, guint height, 
		DOT_TYPE *data, DOT_TYPE *data1, DOT_TYPE *data2,
		guint xoffset, guint yoffset, guchar groupId);
/* */
extern void draw_b_scan (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar mark);
extern int CalcFanScan(gdouble startAngle, gdouble endAngle,
		gdouble stepAngle, gint startWave, gint endWave,
		gint widstep, gint width, gint height);
extern void draw_s_scan (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar ut_unit);
#endif
