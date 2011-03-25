
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

gfloat	HEIGHT_TABLE[256]=
{
	1.000000,0.996078,0.992157,0.988235,0.984314,0.980392,0.976471,0.972549,
	0.968627,0.964706,0.960784,0.956863,0.952941,0.949020,0.945098,0.941176,
	0.937255,0.933333,0.929412,0.925490,0.921569,0.917647,0.913725,0.909804,
	0.905882,0.901961,0.898039,0.894118,0.890196,0.886275,0.882353,0.878431,
	0.874510,0.870588,0.866667,0.862745,0.858824,0.854902,0.850980,0.847059,
	0.843137,0.839216,0.835294,0.831373,0.827451,0.823529,0.819608,0.815686,
	0.811765,0.807843,0.803922,0.800000,0.796078,0.792157,0.788235,0.784314,
	0.780392,0.776471,0.772549,0.768627,0.764706,0.760784,0.756863,0.752941,
	0.749020,0.745098,0.741176,0.737255,0.733333,0.729412,0.725490,0.721569,
	0.717647,0.713725,0.709804,0.705882,0.701961,0.698039,0.694118,0.690196,
	0.686275,0.682353,0.678431,0.674510,0.670588,0.666667,0.662745,0.658824,
	0.654902,0.650980,0.647059,0.643137,0.639216,0.635294,0.631373,0.627451,
	0.623529,0.619608,0.615686,0.611765,0.607843,0.603922,0.600000,0.596078,
	0.592157,0.588235,0.584314,0.580392,0.576471,0.572549,0.568627,0.564706,
	0.560784,0.556863,0.552941,0.549020,0.545098,0.541176,0.537255,0.533333,
	0.529412,0.525490,0.521569,0.517647,0.513725,0.509804,0.505882,0.501961,
	0.498039,0.494118,0.490196,0.486275,0.482353,0.478431,0.474510,0.470588,
	0.466667,0.462745,0.458824,0.454902,0.450980,0.447059,0.443137,0.439216,
	0.435294,0.431373,0.427451,0.423529,0.419608,0.415686,0.411765,0.407843,
	0.403922,0.400000,0.396078,0.392157,0.388235,0.384314,0.380392,0.376471,
	0.372549,0.368627,0.364706,0.360784,0.356863,0.352941,0.349020,0.345098,
	0.341176,0.337255,0.333333,0.329412,0.325490,0.321569,0.317647,0.313725,
	0.309804,0.305882,0.301961,0.298039,0.294118,0.290196,0.286275,0.282353,
	0.278431,0.274510,0.270588,0.266667,0.262745,0.258824,0.254902,0.250980,
	0.247059,0.243137,0.239216,0.235294,0.231373,0.227451,0.223529,0.219608,
	0.215686,0.211765,0.207843,0.203922,0.200000,0.196078,0.192157,0.188235,
	0.184314,0.180392,0.176471,0.172549,0.168627,0.164706,0.160784,0.156863,
	0.152941,0.149020,0.145098,0.141176,0.137255,0.133333,0.129412,0.125490,
	0.121569,0.117647,0.113725,0.109804,0.105882,0.101961,0.098039,0.094118,
	0.090196,0.086275,0.082353,0.078431,0.074510,0.070588,0.066667,0.062745,
	0.058824,0.054902,0.050980,0.047059,0.043137,0.039216,0.035294,0.031373,
	0.027451,0.023529,0.019608,0.015686,0.011765,0.007843,0.003922,0.000000
};

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

/* 画A扫描 data 原始数据 data1 data2 包络数据 */
void draw_a_scan (gushort *p, guint width, guint height, 
		DOT_TYPE *data, DOT_TYPE *data1, DOT_TYPE *data2,
		guint xoffset, guint yoffset, guchar groupId)
{
	gint	i;
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/* 画回波 */
	for (i = 0; i < width - 1; i++)
	{
		fbline (p, 
				xoffset + i,
				yoffset + height * HEIGHT_TABLE[data[i]],
				xoffset + i + 1,
				yoffset + height * HEIGHT_TABLE[data[i + 1]],
				all_col_16[GROUP_VAL_POS(groupId, ascan_color)]);
		/* 画包络 */
		if (GROUP_VAL_POS(groupId, ascan_envelope))
		{
			/* 未完成 */
		}
	}
}

/* 画B扫描 */
void draw_b_scan (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar mark)
{
	gint i, j;
	if (mark)
	{
		for (i = 0; i < height; i++)
			memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2);
		return ;
	}

//	for (i = 0; i < height - 1; i++)
//		memcpy(p + (i + yoffset) * FB_WIDTH, p + (i + yoffset + 1) * FB_WIDTH, FB_WIDTH * 2);

	memcpy(p + (yoffset) * FB_WIDTH, p + (yoffset + 1) * FB_WIDTH, FB_WIDTH * (height - 1) * 2);

	for (j = 0; j < width - 1; j++)
		fbdot (p, xoffset + j, yoffset + height - 1, TMP(color_amp[data1[j]]));
#if 0
	for (i = 0; i < height - 1; i++)
		for (j = 0; j < width - 1; j++)
			fbdot (p, xoffset + j, yoffset + i,
					TMP(color_amp[data[i * width + j]]));
#endif

}

/* 画S扫描 */
void draw_s_scan (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar ut_unit)
{
	gint i, temp;
	switch (ut_unit)	
	{
		case UT_UNIT_SOUNDPATH:
		case UT_UNIT_TIME:
				if (height < TMP(beam_qty[groupId]))
				{

				}
				else if (height = TMP(beam_qty[groupId]))
				{

				}
				else if (height > TMP(beam_qty[groupId]))
				{

				}
			break;
		case  UT_UNIT_TRUE_DEPTH:

			break;
		default:break;
	}
	return ;
}


