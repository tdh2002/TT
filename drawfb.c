
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base.h"
#include "drawui.h"
#include "drawfb.h"
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

void init_fb ()
{
	int fd_fb;
	if ((fd_fb = open(FB_DEVICE, O_RDWR)) == -1)
	{
		perror (FB_DEVICE);
		return ;
	}
	
	TMP(fb1_addr) = (gushort *)
		mmap(NULL, 10 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);

	g_print ("fb1 video addr:%p %p", TMP(fb1_addr), TMP(virtual_add));
	return ;
}

/* draw a dot on coordinate (x,y) 在 (x,y) 画点 */
void fbdot(gushort *p, gint x, gint y, gushort col)
{
	*(p + FB_WIDTH*y + x) = col;
}

/*draw a line from coordinate(x,y1) to coordinate (x,y2) 画线(x,y1) 到 (x,y2)*/
void fbliney(gushort *p, gint x, gint y1, gint y2, gushort col)
{
	gint yy1, yy2;
	yy1 = MAX(y1, y2);
	yy2 = MIN(y1, y2);
	for (;yy2 < yy1 + 1; yy2++)
		fbdot(p, x, yy2, col);
}

/*draw a line from coordinate(x1,y) to coordinate (x2,y) 画线(x1,y) 到 (x2,y)*/
void fblinex(gushort *p, gint y, gint x1, gint x2, gushort col)
{
	gint xx1, xx2;
	xx1 = MAX(x1, x2); 
	xx2 = MIN(x1, x2);
	for (;xx2 < xx1 + 1; xx2++)
		fbdot(p, xx2, y, col);
}

/*draw a line from coordinate(x1,y1) ro coordinate(x2,y2)  画线(x1,y1)到(x2,y2)*/
void fbline(gushort *p, gint x1, gint y1, gint x2, gint y2, gushort col)
{
	int M, N, n, r, s, t;

	if (x1 > x2) 
	{
		M = x1 ; x1 = x2; x2 = M;
		M = y1 ; y1 = y2; y2 = M;
	}

	/* 算出,两个点之间的x坐标的差值.*/
	M = ABS(x1 - x2);
	/*算出,两个点之间的y坐标的差值. */
	N = ABS(y1 - y2);
	/*如果两个点之间的x坐标间差值是0的话,则调用画竖线函数fbliney来完成*/
	if (M == 0) 
		fbliney(p, x1, y1, y2, col);

	/*如果两个点之间的y坐标间差值是0的话,则调用画横线函数fblinex来完成*/
	if (N == 0) 	
		fblinex(p, y1, x1, x2, col);

	/*如果两个点之间的斜率是1的话*/
	if ((M == N) && (M != 0)) 
	{
		for (t = x1; t <= x2; t++) 
		{
			s = (t - x1) * (y2 - y1)/M+y1;
			fbdot(p, t, s, col);
		}
	}
	if ((M < N) && (M != 0)) 
	{
		for (t = x1, r = y1; t <= x2; t++) 
		{
			s = (t - x1) * (y2 - y1)/M+y1;
			fbliney(p, t, r, s, col);
			r = s;
		}
	}
	if ((M > N) && (N != 0)) 
	{
		if (y1 > y2) 
		{
			n = x1; x1 = x2;x2 = n;
			n = y1; y1 = y2;y2 = n;
		}
		for (t = y1, r = x1; t <= y2; t++)
		{
			s = (t - y1) * (x2 - x1)/N+x1;
			fblinex(p, t, r, s, col);
			r=s;
		}
	}
}

/* 画A扫描 */
void draw_a_scan (gushort *p, guint width, guint height, gushort *data,
		guint xoffset, guint yoffset, gushort col)
{
	gint i;
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	for (i = 0; i < width - 1; i++)
		fbline (p, xoffset + i, yoffset + data[i], xoffset + i + 1,
				yoffset + data[i + 1], col);
}

/* 画B扫描 */
void draw_b_scan (gushort *p, guint width, guint height, gushort *data,
		guint xoffset, guint yoffset, gushort col, guchar mark)
{
	gint i;
	if (mark)
		for (i = 0; i < height; i++)
			memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	for (i = 0; i < width - 1; i++)
		fbline (p, xoffset + i, yoffset + data[i], xoffset + i + 1,
				yoffset + data[i + 1], col);
}
