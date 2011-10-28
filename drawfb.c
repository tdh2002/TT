
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base.h"
#include "drawui.h"
#include "drawfb.h"
#include <termios.h>	//串口头文件 
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define MEM_DEVICE "/dev/mem"
#define TTY_DEVICE "/dev/ttyS1"
#define TTY_DEVICE1 "/dev/ttyS0"

#define COLOR_STEP 32     /*    4  8  16  32  64*/
#define COLOR_SHIFT 5     /*    2  3   4   5   6*/
#define MIN_DRAW_FAN_ANGLE_STEP    3
#define MAX_GROUP_QTY   8
#define MAX_BEAM_QTY    256

typedef struct _Point
{
	int x;
	int y;
}POINT, *P_POINT;
typedef struct _VERTICAL_LINE
{
	int start;
	int end  ;
}VERTICAL_LINE, *P_VERTICAL_LINE;

static POINT CurrentLine_top [MAX_GROUP_QTY][MAX_BEAM_QTY];
static POINT CurrentLine_bottom [MAX_GROUP_QTY][MAX_BEAM_QTY];
static VERTICAL_LINE AScanDrawRange[MAX_GROUP_QTY][MAX_BEAM_QTY]  ;

static gchar*	pDraw[MAX_GROUP_QTY]      = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 是否扇形区域
static guchar*	pAngleZoom[MAX_GROUP_QTY] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 处于哪个角度区间
static guchar*	pDrawRate[MAX_GROUP_QTY]  = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 填充比例
static gint*	pDataNo[MAX_GROUP_QTY]    = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 数据在数组中的列号


int calc_line_position (gdouble startAngle, gdouble endAngle, gdouble stepAngle, double StartFocusDepth,
		                     double FocusEnd, double JunctionOffset, int DataLength, int width, int height, guchar group)
{
       // 实际坐标
       gdouble xLeftmost              ;     //     扇形 最左点坐标
       gdouble xRightmost             ;     //     扇形 最右点坐标
       gdouble yTopmost               ;     //     最上点
       gdouble yBottonmost            ;     //     最下点
       double FocusRange              ;
       // 起始\终止\步进  角度
       gdouble a1, a2 , a3            ;
       int RowNo                      ;
       //
       // 实际窗口 宽 高
       gdouble tempW , tempH          ;
       // 实际窗口 入射点坐标
       gdouble xOrg , yOrg            ;
       // 拉伸窗口 入射点坐标
       gdouble xOrgS, yOrgS           ;
       // 拉伸窗口 信号起 止半径
       gdouble xScale                 ;
       gdouble yScale                 ;
       int i                          ;
       gdouble tempy                  ;

	   double TempLength              ;
	   double templength              ;
       // 为全局指针分配内存

       a1  = startAngle * G_PI/180.0                                       ;
       a2  = endAngle   * G_PI/180.0                                       ;
       a3  = stepAngle  * G_PI/180.0                                       ;

       //  扫描数据列数
	   RowNo = (int) ((endAngle-startAngle) / stepAngle +1)                ;

       // calculate the fan range in real coordinate
       FocusRange = FocusEnd - StartFocusDepth  ;
       yBottonmost = JunctionOffset +  FocusEnd  ;

       if( a1 * a2 <= 0)
	   {
			xLeftmost   = JunctionOffset * tan(a1) + (StartFocusDepth + FocusRange)*sin(a1)  ;
			xRightmost  = JunctionOffset * tan(a2) + (StartFocusDepth + FocusRange)*sin(a2)  ;

            if( fabs(a2) > fabs(a1) )
			   yTopmost = JunctionOffset + StartFocusDepth * cos(a2)  ;
			else
			   yTopmost = JunctionOffset + StartFocusDepth * cos(-a1) ;

            TempLength  = StartFocusDepth + FocusRange                ;
			templength  = StartFocusDepth                             ;
	   }
	   else if (a1 < 0 && a2 < 0)
	   {
		    yTopmost   = JunctionOffset + (StartFocusDepth / cos(-a2)) * cos(-a1)                    ;
		    xLeftmost  = JunctionOffset * tan(a1) + sin(a1) * (StartFocusDepth + FocusRange)/cos(a2) ;
		    xRightmost = (JunctionOffset + StartFocusDepth) * tan(a2)                                ;

            TempLength = (StartFocusDepth + FocusRange) / cos(-a2)								     ;
			templength = StartFocusDepth / cos(-a2)                                                  ;
	   }
	   else if (a1 > 0 && a2 > 0)
	   {
	        yTopmost   = JunctionOffset + (StartFocusDepth / cos(a1)) * cos(a2)						 ;
            xLeftmost  = (JunctionOffset + StartFocusDepth) * tan(a1)								 ;
		    xRightmost = JunctionOffset * tan(a2) + sin(a2) * (StartFocusDepth + FocusRange)/cos(a1) ;

            TempLength = (StartFocusDepth + FocusRange) / cos(a1)									 ;
			templength = StartFocusDepth / cos (a1)													 ;
	   }
       // 波型的实际宽度和高度
       // 虚拟发射点坐标
       tempW = (gdouble)(xRightmost - xLeftmost)           ;
       tempH = (gdouble)(yBottonmost- yTopmost)            ;

       xOrg  = 0 - xLeftmost                               ;
       yOrg  = 0 - yTopmost                                ;

       // 实际尺寸 转 显示尺寸 比例
       xScale = width/tempW                                ;
       yScale = height/tempH                               ;

       // 显示坐标系中 虚拟发射点坐标
       xOrgS  = xOrg*xScale                                ;
       yOrgS  = yOrg*yScale                                ;

       //求每一列波型的 比例尺
       //和拉伸后显示的扫描角度
       for(i = 0; i< RowNo ; i++)
       {
             tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+TempLength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
             AScanDrawRange[group][i].end = (int)tempy ;

			 tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+templength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
			 AScanDrawRange[group][i].start = (int)tempy ;
             //****************************************************************************
       }
       return 0;
}












static int fd_fb;
struct fb_var_screeninfo vinfo;

static gushort all_col_16[] =
{
	0x001f,				/* 蓝色 */
	0x03e0,				/* 绿色 */
	0xf800,				/* 红色 */
	0xffe0,				/* 黄色 */
	0x0000,				/* 黑色 */
	0xffff				/* 白色 */		
};

static gfloat HEIGHT_TABLE[256]=
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
	if ((fd_fb = open(FB_DEVICE, O_RDWR)) == -1)
	{
		perror (FB_DEVICE);
		return ;
	}

	TMP(fb1_addr) = (gushort *)
		mmap(NULL, 2 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);

	g_print ("fb1 video addr:%p %p", TMP(fb1_addr), TMP(virtual_add));

	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &vinfo))
	{
		printf("Error reading variable information\n");
		exit(3);
	}

	return ;
}

void init_mem ()
{
	int fd_mem;

	if ((fd_mem = open(MEM_DEVICE, O_RDWR | O_SYNC)) == -1) 
	{
		perror (MEM_DEVICE);
		return ;
	}
	
	TMP(dma_data_add) = (unsigned long int)
		mmap(0, 16 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, 0x8F000000);

	g_print ("mem init\n");
	return ;
}

void init_serial ()
{
	struct termios newtermios;
	pp->fd_key = open(TTY_DEVICE, O_RDWR | O_NOCTTY );
	if (pp->fd_key < 0) {
		perror(TTY_DEVICE); 
		return ;
	}
	bzero(&newtermios, sizeof(newtermios)); /* 清除结构体以放入新的序列埠设定值 */
	tcgetattr(pp->fd_key, &newtermios);
	newtermios.c_cc[VMIN] = 0;
	newtermios.c_iflag = 0;
	newtermios.c_oflag = 0;
	newtermios.c_lflag = 0; 
	newtermios.c_cflag	= B115200 | CS8 | PARENB | CLOCAL | CREAD;
	if (tcsetattr(pp->fd_key, TCSANOW, &newtermios)) {
		perror("tcsetattr   error");  
		exit(1);  
	}

	pp->fd_key1 = open(TTY_DEVICE1, O_RDWR | O_NOCTTY );
	if (pp->fd_key1 < 0) {
		perror(TTY_DEVICE1); 
		return ;
	}
	bzero(&newtermios, sizeof(newtermios)); /* 清除结构体以放入新的序列埠设定值 */
	tcgetattr(pp->fd_key1, &newtermios);
	newtermios.c_cc[VMIN] = 0;
	newtermios.c_iflag = 0;
	newtermios.c_oflag = 0;
	newtermios.c_lflag = 0; 
	newtermios.c_cflag	= B115200 | CS8 | PARENB | CLOCAL | CREAD;
	if (tcsetattr(pp->fd_key1, TCSANOW, &newtermios)) {
		perror("tcsetattr   error");  
		exit(1);  
	}
	return ;
}


static void	compress_data (DOT_TYPE *source_data, DOT_TYPE *target_data,
		gint qty1, gint qty2, guint	rectify)
{
	guint	temp_1, temp_2, i, j;
	for (i = 0; i < qty2; i++)
	{
		temp_2 = qty1 / qty2;
		temp_1 = i * qty1 / qty2;
		target_data[i] = source_data[temp_1];
		for (j = 1; j < temp_2; j++) {
			if (rectify == RF_WAVE) {
				if (target_data[i] > 127) {
					if (source_data[temp_1 + j] > target_data[i])
						target_data[i] = source_data[temp_1 + j];
				} else {
					if (source_data[temp_1 + j] < target_data[i])
						target_data[i] = source_data[temp_1 + j];
				}
			}
			else
			{
				if (source_data[temp_1 + j] > target_data[i])
					target_data[i] = source_data[temp_1 + j];
			}
		}
	}
}


static void interpolation_data (DOT_TYPE *source_data, DOT_TYPE *target_data,
								  int qty1, int qty2)
{
	int	t1, t2, i, j;
	if (qty1 == qty2)
		memcpy ((void *)(target_data), (void *)(source_data), qty1 * sizeof(DOT_TYPE));
	else
	{
			for (i = 0 ; i < qty1 - 1; i++)
			{
				for (t1 = i * qty2 / (qty1 - 1), t2 = (i + 1) * qty2 / (qty1 - 1), j = t1; j < t2; j++)
				{
					target_data[j] = (unsigned char)((int)(source_data[i]) +
							((int)((source_data[i + 1]) - (int)(source_data[i])) * (j - t1) / (qty2 / (qty1 - 1))));
				}
			}
	}
}


/* draw a dot on coordinate (x,y) 在 (x,y) 画点 */
inline void fbdot(gushort *p, gint x, gint y, gushort col)
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
	/* 算出,两个点之间的y坐标的差值.*/
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

/* 画圆 */
void fbround(gushort *p, gint x0, gint y0, gint radius, gushort color)
{
	unsigned int x,y,y1=0;
	int s0,s1,s2;
	x=x0+radius+1;        
	y=y0;
	do
	{      
		s0=abs((x-x0-1)*(x-x0-1)+(y-y0)*(y-y0)-radius*radius);
		s1=abs((x-x0)*(x-x0)+(y-y0-1)*(y-y0-1)-radius*radius);
		s2=abs((x-x0-1)*(x-x0-1)+(y-y0-1)*(y-y0-1)-radius*radius);
		s0<=s1?(s0<=s2?x=x-1:(x=x-1,y=y-1)):(s1<=s2?y=y-1:(x=x-1,y=y-1));
		if((x!=2*x0-x)&&(y1!=y))
		{   
			fbdot(p,2*x0-x,y,color);
			fbdot(p,x,2*y0-y,color);
//			fbline(p,x-1,y,2*x0-x+1,y,color);
		}
		if((y!=2*y0-y)&&(y1!=y))
		{      
			fbdot(p,x,y,color);    
			fbdot(p,2*x0-x,2*y0-y,color);
			fbline(p,2*x0-x+1,2*y0-y,x-1,2*y0-y,color);
		}            
		y1=y;
	}while((x!=x0)&&(y!=(y0-radius)));

}


/* 画A扫描 data 原始数据 data1 data2 包络数据 */
void draw_a_scan (gushort *p, gint width, gint height, 
		DOT_TYPE *data, DOT_TYPE *data1, DOT_TYPE *data2,
		gint xoffset, gint yoffset, guchar groupId)
{
	gint	i, min, max;
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
//	for (i = 0; i < height; i++)
//		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/* 画回波 */

	for (i = 0; i < width - 1; i++)
	{
		min =	MIN (height * HEIGHT_TABLE[data[i]],	height * HEIGHT_TABLE[data[i + 1]]);
		max =	MAX (height * HEIGHT_TABLE[data[i]],	height * HEIGHT_TABLE[data[i + 1]]);
		if ((max - min) == 0)
		{
			if (min) 
				min -= 1;
			else
				max =+ 1;
		}

		// clear back ground
		fbliney(p,
				xoffset + i,
				yoffset    ,
				yoffset + height,
				0x0
			   );
#if 1		
		// draw line
		fbliney(
				p, 
				xoffset + i,
				yoffset + min, 
				yoffset + max, 
				all_col_16[GROUP_VAL_POS(groupId, ascan_color)]
				);
#endif

#if 0
		/* 画包络 */
		if (GROUP_VAL_POS(groupId, ascan_envelope))
		{
			/* 未完成 */
		}
#endif
	}
//	fbline(p, xoffset+0, yoffset+0, xoffset+400, yoffset+400, 0x001f);
}

#if 0
/* draw a scan back up */
void draw_a_scan (gushort *p, gint width, gint height, 
		DOT_TYPE *data, DOT_TYPE *data1, DOT_TYPE *data2,
		gint xoffset, gint yoffset, guchar groupId)
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
				xoffset + i,
				yoffset + height * HEIGHT_TABLE[data[i + 1]],
				all_col_16[GROUP_VAL_POS(groupId, ascan_color)]);
		/*
		fbline (p, 
				xoffset + i,
				yoffset + height * HEIGHT_TABLE[data[i]],
				xoffset + i + 1,
				yoffset + height * HEIGHT_TABLE[data[i + 1]],
				all_col_16[GROUP_VAL_POS(groupId, ascan_color)]);
		*/
		/* 画包络 */
		if (GROUP_VAL_POS(groupId, ascan_envelope))
		{
			/* 未完成 */
		}
	}
}
#endif

/* 画A扫描 data 原始数据 data1 data2 包络数据 */
void draw_a_scan_r (gushort *p, gint width, gint height, 
		DOT_TYPE *data, DOT_TYPE *data1, DOT_TYPE *data2,
		gint xoffset, gint yoffset, gushort point_qty, guchar groupId, int start, int end)
{
	gint	   i;
	float scale ;
	scale = point_qty / (end - start + 1.0) ;
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/* 画回波 */
	for (i = start; i < end ; i++)
	{
		fbline (p, 
				xoffset + width * HEIGHT_TABLE[255 - data[(int)(0.5 + (i-start) * scale)]],
				yoffset + i,
				xoffset + width * HEIGHT_TABLE[255 - data[(int)(0.5 + (i + 1 -start) * scale)]],
				yoffset + i + 1,
				all_col_16[GROUP_VAL_POS(groupId, ascan_color)]);
		/* 画包络 */
		if (GROUP_VAL_POS(groupId, ascan_envelope))
		{
			/* 未完成 */
		}
	}
}

/* 画B扫描 */
void draw_b_scan (gushort *p, gint width, gint height, DOT_TYPE *data, DOT_TYPE *data1,
		gint xoffset, gint yoffset, guchar groupId, guchar mark)
{
	gint i, j;
	if (mark)
	{
		for (i = 0; i < height; i++)
			memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2);
		return ;

	}

	for (i = 0; i < height - 1; i++)
		memcpy(p + (i + yoffset) * FB_WIDTH + xoffset, p + (i + yoffset + 1) * FB_WIDTH + xoffset, width * 2);

	/* 编码器同步的时候怎么画 */
	/*
	memcpy(p + (yoffset) * FB_WIDTH, p + (yoffset + 1) * FB_WIDTH, FB_WIDTH * (height - 1) * 2);
	*/

	for (j = 0; j < width - 1; j++)
		fbdot (p, xoffset + j, yoffset + height - 1, TMP(color_amp[data1[j]]));

#if 0
	g_print ("xoffset = %d yoffset = %d width =%d height =%d \n",
			xoffset, yoffset, width, height);
	for (i = 0; i < height - 1; i++)
		for (j = 0; j < width - 1; j++)
			fbdot (p, xoffset + j, yoffset + i,
					TMP(color_amp[data[i * width + j]]));
#endif

}

/* 画C扫描 */
void draw_c_scan (gushort *p, gint width, gint height, DOT_TYPE *data, DOT_TYPE *data1,
		gint xoffset, gint yoffset, guchar groupId, guchar mark, guchar source)
{
	gint i, j, k, offset;
	gint beam_qty = TMP(beam_qty[groupId]);
	if (mark)
	{
		for (i = 0; i < height; i++)
			memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2);
		return ;
	}

//	for (i = 0; i < height - 1; i++)
//		memcpy(p + (i + yoffset) * FB_WIDTH, p + (i + yoffset + 1) * FB_WIDTH, FB_WIDTH * 2);

	/* 编码器同步的时候怎么画 */
/*	memcpy(p + (yoffset) * FB_WIDTH, p + (yoffset + 1) * FB_WIDTH, FB_WIDTH * (height - 1) * 2);*/
	for (i = 0; i < height; i++)
		for (j = width - 1; j > 0; j--)
		{
			*((p + (i + yoffset) * FB_WIDTH) + xoffset + j) = 
				*((p + (i + yoffset) * FB_WIDTH) + xoffset + j -1);
		}


	for (offset = 0, k = 0 ; k < get_current_group(pp->p_config); k++)
		offset += TMP(beam_qty[k]);

	/* 不同的C-source measure_data 后面取值不一样  */
	if (height < beam_qty)
	{
		/* 压缩C扫描 */
		for (i = 0 ; i < height ; i++)		
			{
				fbdot (p, xoffset, yoffset + i,
						TMP(color_amp[(int)((TMP(measure_data[offset + i * beam_qty / height][source + 1]) >> 24) / 40.95)]));
			}
	}
	else if (height == beam_qty)
	{
		for (i = 0; i < beam_qty; i++)
				fbdot (p, xoffset, yoffset + i,
						TMP(color_amp[(int)((TMP(measure_data[offset + i][source + 1]) >> 24) / 40.95)]));
	}
	else if (height > beam_qty)
	{
		/* 拉伸 */
		for (i = 0; i < beam_qty; i++)
			for (j = 0; j <= height / beam_qty; j++)
				fbdot (p, xoffset, yoffset + 
						i * height / beam_qty + j,
						TMP(color_amp[(int)((TMP(measure_data[offset + i][source + 1]) >> 24) / 40.95)]));

	}

}

///////////////////////////////
//  CalcFanScan
//  功能         : 计算各个像素点的插值参数
//  image        : 像素缓存
//  startAngle   : 扫描起始角度
//  endAngle     : 扫描终止角度
//  StartWave    : 波形起始
//  WaveLength   : 波形长度
int CalcFanScan_new (gdouble startAngle, gdouble endAngle, gdouble stepAngle, double StartFocusDepth,
		                     double FocusEnd, double JunctionOffset, int DataLength, int width, int height, guchar group)
{
       
       //int endWave =   StartWave + WaveLength                ;
       // 实际坐标
       gdouble xLeftmost              ;     //     扇形 最左点坐标
       gdouble xRightmost             ;     //     扇形 最右点坐标
       gdouble yTopmost               ;     //     最上点
       gdouble yBottonmost            ;     //     最下点
       double FocusRange              ;
       // 起始\终止\步进  角度
       gdouble a1, a2 , a3            ;
       int RowNo                      ;
       //
       gdouble _a3                    ;
       int     _RowNo                 ;
       // 实际窗口 宽 高
       gdouble tempW , tempH          ;
       // 实际窗口 入射点坐标
       gdouble xOrg , yOrg            ;
       // 拉伸窗口 入射点坐标
       gdouble xOrgS, yOrgS           ;
       // 拉伸窗口 信号起 止半径
       gdouble startWaveS , endWaveS  ;
       gdouble xxx  , yyy             ;
       gdouble tLength                ;
       gdouble xScale                 ;
       gdouble yScale                 ;
       int i , j                      ;

       gdouble* pScales = NULL        ;
       gdouble* pScalesAngle =  NULL  ;
       gdouble* _pScalesAngle=  NULL  ;
       gdouble tempx, tempy           ;
       gdouble tempScale              ;
       gdouble tAngle                 ;
       int iAngle                     ;
      
	   double TempLength              ; 
	   double templength              ;
	   double BeamStartLength[256]    ;
	   double BeamEndLength[256]      ;
       // 为全局指针分配内存 

	   if(pDraw[group] != NULL)       { free(pDraw[group]);  pDraw[group] = NULL ;}
	   if(pAngleZoom[group] != NULL)  { free(pAngleZoom[group]);  pAngleZoom[group] = NULL ;}
	   if(pDrawRate[group] != NULL)   { free(pDrawRate[group]);  pDrawRate[group] = NULL ;}
	   if(pDataNo[group] != NULL)     { free(pDataNo[group]);  pDataNo[group] = NULL ;}

       pDraw[group]        = (char*) malloc (width*height)                        ;
       pAngleZoom[group]   = (unsigned char*)malloc(width*height)                 ;
       pDrawRate[group]    = (unsigned char*)malloc(width*height)                 ;
       pDataNo[group]      = (int*) malloc(sizeof(int)*width*height)              ;

       a1  = startAngle * G_PI/180.0                                       ;
       a2  = endAngle   * G_PI/180.0                                       ;
       a3  = stepAngle  * G_PI/180.0                                       ;
       _a3 = MIN_DRAW_FAN_ANGLE_STEP * G_PI/180.0                          ;

       //  扫描数据列数
	   RowNo = (int) ((endAngle-startAngle) / stepAngle +1)                ;
	   _RowNo= (int) ((endAngle-startAngle) / MIN_DRAW_FAN_ANGLE_STEP + 1) ;
	   
       //  临时比例队列
       pScales = (gdouble*)malloc(sizeof(gdouble)*RowNo)                   ;
       pScalesAngle = (gdouble*)malloc(sizeof(gdouble)*RowNo)              ;

       // calculate the fan range in real coordinate
       FocusRange = FocusEnd - StartFocusDepth  ;
       yBottonmost = JunctionOffset +  FocusEnd  ;
	   
       if( a1 * a2 <= 0)
	   {
			xLeftmost   = JunctionOffset * tan(a1) + (StartFocusDepth + FocusRange)*sin(a1)  ;
			xRightmost  = JunctionOffset * tan(a2) + (StartFocusDepth + FocusRange)*sin(a2)  ;
			
            if( fabs(a2) > fabs(a1) )
			   yTopmost = JunctionOffset + StartFocusDepth * cos(a2)  ;
			else 
			   yTopmost = JunctionOffset + StartFocusDepth * cos(-a1) ;
			
            TempLength  = StartFocusDepth + FocusRange                ;
			templength  = StartFocusDepth                             ;
	   }
	   else if (a1 < 0 && a2 < 0)
	   {
		    yTopmost   = JunctionOffset + (StartFocusDepth / cos(-a2)) * cos(-a1)                    ; 
		    xLeftmost  = JunctionOffset * tan(a1) + sin(a1) * (StartFocusDepth + FocusRange)/cos(a2) ;
		    xRightmost = (JunctionOffset + StartFocusDepth) * tan(a2)                                ;
			
            TempLength = (StartFocusDepth + FocusRange) / cos(-a2)								     ;
			templength = StartFocusDepth / cos(-a2)                                                  ;
	   }
	   else if (a1 > 0 && a2 > 0)
	   {
	        yTopmost   = JunctionOffset + (StartFocusDepth / cos(a1)) * cos(a2)						 ; 
            xLeftmost  = (JunctionOffset + StartFocusDepth) * tan(a1)								 ;
		    xRightmost = JunctionOffset * tan(a2) + sin(a2) * (StartFocusDepth + FocusRange)/cos(a1) ;
			
            TempLength = (StartFocusDepth + FocusRange) / cos(a1)									 ;
			templength = StartFocusDepth / cos (a1)													 ;
	   }
       // 波型的实际宽度和高度
       // 虚拟发射点坐标
       tempW = (gdouble)(xRightmost - xLeftmost)           ;
       tempH = (gdouble)(yBottonmost- yTopmost)            ;

       xOrg  = 0 - xLeftmost                               ;
       yOrg  = 0 - yTopmost                                ;
       
       // 实际尺寸 转 显示尺寸 比例
       xScale = width/tempW                                ;
       yScale = height/tempH                               ;
  
       // 显示坐标系中 虚拟发射点坐标
       xOrgS  = xOrg*xScale                                ;
       yOrgS  = yOrg*yScale                                ;
       
       //求每一列波型的 比例尺
       //和拉伸后显示的扫描角度
       for(i = 0; i< RowNo ; i++)
       {
             tempx = xScale * (sin(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+TempLength)+xOrg)                  ;
             tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+TempLength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
             CurrentLine_bottom[group][i].x = (int)tempx ;
             CurrentLine_bottom[group][i].y = (int)tempy ;
             //****************************************************************************
             tempx = tempx  -  xOrgS																		;
             tempy = tempy  -  yOrgS																		;
             if(tempy != 0)
                    pScalesAngle[i] = atan(tempx/tempy)														;
             else pScalesAngle[i] = tempx>0 ? G_PI/2.0 : -G_PI/2.0											;
             pScales[i] = sqrt(tempx*tempx+tempy*tempy)/(JunctionOffset/cos(a1+i*a3)+TempLength)            ;
            
             BeamEndLength[i]   = sqrt(tempx*tempx+tempy*tempy)                                             ;
			 tempx = xScale * (sin(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+templength)+xOrg)                  ;
			 tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+templength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
             CurrentLine_top[group][i].x = (int)tempx ;
             CurrentLine_top[group][i].y = (int)tempy ;
             //****************************************************************************
             tempx = tempx  -  xOrgS																		;
             tempy = tempy  -  yOrgS																		;
			 BeamStartLength[i] = sqrt(tempx*tempx+tempy*tempy)                                             ;
       }
       // 当最小 步进角度 小于  MIN_DRAW_FAN_ANGLE_STEP   的时候
       // 取  MIN_DRAW_FAN_ANGLE_STEP 为最小步进角度
       // 实现 各个扇区 平稳过度  美观
       if( stepAngle > MIN_DRAW_FAN_ANGLE_STEP )
       {
                free( pScales);
                pScales = (gdouble*) malloc(  sizeof(gdouble) * _RowNo )         ;
                _pScalesAngle = ( gdouble* ) malloc( sizeof( gdouble) * _RowNo)  ;
                for(i = 0; i< _RowNo ; i++)
                {
                         tempx = xScale * (sin(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+TempLength)+xOrg)                  ;
                         tempy = yScale * (cos(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+TempLength)+yOrg)                  ;
                        
                         tempx = tempx  -  xOrgS																		  ;
                         tempy = tempy  -  yOrgS																		  ;
                         if(tempy != 0)
                                _pScalesAngle[i] = atan(tempx/tempy)												      ;
                         else pScalesAngle[i] = tempx>0 ? G_PI/2.0 : -G_PI/2.0											  ;
                         pScales[i] = sqrt(tempx*tempx+tempy*tempy)/(JunctionOffset/cos(a1+i*_a3)+TempLength)             ;
                        
                         BeamEndLength[i]   = sqrt(tempx*tempx+tempy*tempy)                                               ;
            			 tempx = xScale * (sin(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+templength)+xOrg)                  ;
            			 tempy = yScale * (cos(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+templength)+yOrg)                  ;
                         tempx = tempx  -  xOrgS																		  ;
                         tempy = tempy  -  yOrgS																		  ;
            			 BeamStartLength[i] = sqrt(tempx*tempx+tempy*tempy)                                               ;
                }
                _pScalesAngle[_RowNo - 1] = pScalesAngle[ RowNo - 1]                                                      ;
       }

       a1 = pScalesAngle[0]          ;
       a2 = pScalesAngle[RowNo -1]   ;

	   for(i = 0 ; i< height ; i++)
       {
             for(j = 0; j< width; j++)
             {

				  xxx = j-xOrgS      ;
                  yyy = i-yOrgS      ;
                  tLength = sqrt(xxx*xxx + yyy*yyy)     ;
                  tAngle  = asin(xxx/tLength)           ;
                  if(tAngle > a2|| tAngle < a1)  
                    {  pDraw[group][i*width+ j] = 0; continue ;}
                  iAngle = 0                            ;
                  while(tAngle>=pScalesAngle[iAngle]&&tAngle<a2)
                  {
                        iAngle++                        ;
                  }
                  iAngle--                              ;
                  pAngleZoom[group][i*width+j] = iAngle        ;
                  tempScale = (tAngle - pScalesAngle[iAngle])/(pScalesAngle[iAngle+1] - pScalesAngle[iAngle])         ;
                  if(tempScale > 1) tempScale = 1                                                                     ;
                  pDrawRate[group][i*width+j] = (unsigned char)( tempScale * COLOR_STEP )                             ;
                  if(stepAngle <= MIN_DRAW_FAN_ANGLE_STEP)
                  {
                              startWaveS = BeamStartLength[iAngle] * ( 1 - tempScale )  + BeamStartLength[iAngle+1] * tempScale      ;
                              endWaveS   = BeamEndLength[iAngle]   * ( 1 - tempScale )  + BeamEndLength[iAngle+1] * tempScale        ;
                              tempScale = pScales[iAngle]*(1-tempScale)+pScales[iAngle+1]*tempScale                                  ;
                              pDataNo[group][i*width+j] = (int)( DataLength * (tLength-startWaveS)/(endWaveS - startWaveS) )         ;
                              if(tLength > endWaveS || tLength<startWaveS) 
                              { 
                                      pDraw[group][i*width + j]= 0 ;
                                      continue               ;
                              }
                              pDraw[group][i*width + j]= 255       ;
                  }
                  else
                  {
                              iAngle = 0     ;
                              while(tAngle>=_pScalesAngle[iAngle] && tAngle<a2)
                              {
                                    iAngle++ ;
                              }
                              iAngle--       ;
                              tempScale = (tAngle - _pScalesAngle[iAngle]) / (_pScalesAngle[iAngle+1] - _pScalesAngle[iAngle])  ;
                              if (tempScale > 1)     tempScale = 1                                                              ;
                              startWaveS = BeamStartLength[iAngle] * ( 1 - tempScale )  + BeamStartLength[iAngle+1] * tempScale      ;
                              endWaveS   = BeamEndLength[iAngle]   * ( 1 - tempScale )  + BeamEndLength[iAngle+1] * tempScale          ;
                              tempScale =  pScales[iAngle] * (1- tempScale) +  pScales[iAngle+1]*tempScale                      ;
                              pDataNo[group][i*width+j] = (int)(DataLength * (tLength-startWaveS)/(endWaveS - startWaveS) )            ;
                              if(tLength > endWaveS || tLength<startWaveS) { pDraw[group][ i * width + j]= 0 ; continue  ;}
                              pDraw [group][i*width + j]= 255                                                                          ;
                  }
             }
       }



       free(pScales);
	   free(pScalesAngle);
	   free(_pScalesAngle) ;
	   return 0;
}
#if 0
int CalcFanScan(gdouble startAngle, gdouble endAngle,
		gdouble stepAngle, gint StartWave, gint endWave,
	    gint width, gint height)
{
       // 实际坐标
       gdouble xLeftmost               ;     //      扇形 最左点坐标
       gdouble xRightmost             ;     //     扇形 最右点坐标
       gdouble yTopmost                ;      //     最上点
       gdouble yBottonmost         ;    //     最下点
       // 起始\终止\步进  角度
       gdouble a1, a2 , a3                ;
       int RowNo                              ;
       //
       gdouble _a3                            ;
       int _RowNo                             ;
       // 实际窗口 宽 高
       gdouble tempW , tempH      ;
       // 实际窗口 入射点坐标
       gdouble xOrg , yOrg             ;
       // 拉伸窗口 入射点坐标
       gdouble xOrgS, yOrgS         ;
       // 拉伸窗口 信号起 止半径
       gdouble startWaveS , endWaveS ;
       gdouble xxx  , yyy                 ;
       gdouble tLength                  ;
       gdouble xScale                     ;
       gdouble yScale                     ;
       int i , j                                      ;

       gdouble* pScales                 ;
       gdouble* pScalesAngle       ;
       gdouble* _pScalesAngle     ;
       gdouble tempx, tempy        ;
       gdouble tempScale              ;
       gdouble tAngle                    ;
       int iAngle                               ;
       // 为全局指针分配内在
       pDraw             = (char*) malloc (width*height)                 ;
       pAngleZoom = (unsigned char*)malloc(width*height)  ;
       pDrawRate    = (unsigned char*)malloc(width*height)  ;
       pDataNo        = (int*) malloc(sizeof(int)*width*height) ;

       a1 = startAngle * G_PI/180.0 ;
       a2 = endAngle   * G_PI/180.0   ;
       a3 = stepAngle  * G_PI/180.0  ;
       _a3 = MIN_DRAW_FAN_ANGLE_STEP * G_PI/180.0                       ;

       //  扫描数据列数
	   RowNo = (int)  ((endAngle-startAngle) / stepAngle +1)            ;
	   _RowNo= (int) ((endAngle-startAngle) / MIN_DRAW_FAN_ANGLE_STEP + 1)            ;
	   //  临时比例队列
       pScales = (gdouble*)malloc(sizeof(gdouble)*RowNo)                   ;
       pScalesAngle = (gdouble*)malloc(sizeof(gdouble)*RowNo)        ;

       // calculate the fan in real coordinate
       xLeftmost = a1 >= 0  ?  sin(a1) * StartWave : sin(a1) * endWave ;
       xRightmost = a2 >= 0  ?  sin(a2) * endWave : sin(a2) * StartWave;

       if(fabs(a1)>fabs(a2))
       {
              yTopmost = cos(a1)*StartWave                 ;
              yBottonmost = cos(a2)*endWave            ;
       }
       else
       {
              yTopmost = cos(a2)*StartWave                 ;
              yBottonmost= cos(a1)*endWave              ;
       }
       if(a1<=0&&a2>=0) yBottonmost = endWave ;
       // 波型的实际宽度和高度.
       // 虚拟发射点坐标
       tempW = (gdouble)(xRightmost - xLeftmost)           ;
       tempH = (gdouble)(yBottonmost- yTopmost)          ;
       xOrg = 0 - xLeftmost                       ;
       yOrg = 0 - yTopmost                        ;
       // 实际尺寸 转 显示尺寸 比例
       xScale = width/tempW                     ;
       yScale = height/tempH                    ;
       // 显示坐标系中 虚拟发射点坐标
       xOrgS = (int)xOrg*xScale               ;
       yOrgS = (int)yOrg*yScale               ;
       //求每一列波型的 比例尺
       //和拉伸后显示的扫描角度
       for(i = 0; i< RowNo ; i++)
       {
             tempx = xScale*(sin(a1+i*a3)*endWave+xOrg)     ;
             tempy = yScale*(cos(a1+i*a3)*endWave+yOrg)     ;
             tempx = tempx - xOrgS                                             ;
             tempy = tempy - yOrgS                                             ;
             if(tempy != 0)
                    pScalesAngle[i] = atan(tempx/tempy)             ;
            else pScalesAngle[i] = tempx>0 ? G_PI/2.0 : -G_PI/2.0      ;
             pScales[i] = sqrt(tempx*tempx+tempy*tempy)/endWave    ;
       }
       //当最小 步进角度 小于  MIN_DRAW_FAN_ANGLE_STEP   的时候
       // 取  MIN_DRAW_FAN_ANGLE_STEP 为最小步进角度
       // 实现 各个扇区 平稳过度
       if( stepAngle > MIN_DRAW_FAN_ANGLE_STEP )
       {
                free( pScales);
                pScales = (gdouble*) malloc(  sizeof(gdouble) * _RowNo ) ;
                _pScalesAngle = ( gdouble* ) malloc( sizeof( gdouble) * _RowNo)  ;
                for(i = 0; i< _RowNo ; i++)
                {
                         tempx = xScale*(sin(a1+i*_a3)*endWave +xOrg)     ;
                         tempy = yScale*(cos(a1+i*_a3)*endWave+yOrg)    ;
                         tempx = tempx - xOrgS                                             ;
                         tempy = tempy - yOrgS                                              ;
                         if(tempy != 0)
                                  _pScalesAngle[i] = atan(tempx/tempy)             ;
                         else _pScalesAngle[i] = tempx>0 ? G_PI/2.0 : -G_PI/2.0      ;
                         pScales[i] = sqrt(tempx*tempx+tempy*tempy)/endWave    ;
                }
                  _pScalesAngle[_RowNo - 1] = pScalesAngle[ RowNo - 1]             ;
       }

       a1 = pScalesAngle[0];
       a2 = pScalesAngle[RowNo -1];

	   for(i = 0 ; i< height ; i++)
       {
             for(j = 0; j< width; j++)
             {
                  xxx = j-xOrgS  ;
                  yyy = i-yOrgS  ;
                  tLength = sqrt(xxx*xxx + yyy*yyy);
                  tAngle  = asin(xxx/tLength)           ;
                  if(tAngle > a2|| tAngle < a1)  {  pDraw[i*width+ j] = 0;  continue ;}
                  iAngle = 0;
                  while(tAngle>pScalesAngle[iAngle]&&tAngle<a2)
                  {
                        iAngle++ ;
                  }
                  iAngle--;
                  pAngleZoom[i*width+j] = iAngle  ;
                  tempScale = (tAngle - pScalesAngle[iAngle])/(pScalesAngle[iAngle+1] - pScalesAngle[iAngle])  ;
                  if(tempScale > 1) tempScale = 1;
                  pDrawRate[i*width+j] = (unsigned char)(tempScale*COLOR_STEP);
                  if(stepAngle <= MIN_DRAW_FAN_ANGLE_STEP)
                  {
                                  tempScale = pScales[iAngle]*(1-tempScale)+pScales[iAngle+1]*tempScale  ;
                                  startWaveS = StartWave*tempScale   ;
                                  endWaveS   = endWave*tempScale     ;
                                  pDataNo[i*width+j] = (int)((tLength-startWaveS)/tempScale )   ;
                                  if(tLength > endWaveS || tLength<startWaveS) { pDraw [i*width + j]= 0 ; continue ;}
                                  pDraw [i*width + j]= 255 ;
                  }
                  else
                  {
                                  iAngle = 0;
                                  while(tAngle>_pScalesAngle[iAngle]&&tAngle<a2)
                                  {
                                        iAngle++ ;
                                  }
                                  iAngle--;
                                  tempScale = (tAngle - _pScalesAngle[iAngle]) / (_pScalesAngle[iAngle+1] - _pScalesAngle[iAngle])  ;
                                  if (tempScale > 1)     tempScale = 1;
                                  tempScale =  pScales[iAngle] * (1- tempScale) +  pScales[iAngle+1]*tempScale  ;
                                  startWaveS = StartWave * tempScale   ;
                                  endWaveS   = endWave   * tempScale   ;
                                  pDataNo[i*width+j] = (int)((tLength-startWaveS)/tempScale )   ;
                                  if(tLength > endWaveS || tLength<startWaveS) { pDraw [i*width + j]= 0 ; continue ;}
                                  pDraw [i*width + j]= 255 ;
                  }
             }
       }
       free(pScales);
	   free(pScalesAngle);
	   return 0;
}

#endif

int DrawPixbuff(gushort *p,
		guint xoffset, guint yoffset,
		gint widstep, gint width, gint height,
		gint startWave, gint endWave, DOT_TYPE *WaveData, guchar group)
{   
       gint	i, j;
       gint pointer ;
       gint pointer2;
       gint tempData ;
       gint waveLength  = endWave - startWave;
       i= pp->p_tmp_config->beam_num[group] ;
//       fbline(p, xoffset+CurrentLine_top[group][i].x, yoffset+CurrentLine_top[group][i].y, xoffset+CurrentLine_bottom[group][i].x,
//       	                              yoffset+CurrentLine_bottom[group][i].y,  0x001f );

       
       for(i = 0; i< height; i++)
       {
             for(j = 0; j < width; j++)
             {
                      pointer = i*width +j ;
                      if(pDraw[group][pointer] != 0)
                      { 
                           pointer2 = (gint)(pAngleZoom[group][pointer] * (waveLength + 32) + pDataNo[group][pointer]);
//						   printf("pointer2 = %d \n", pointer2);
                           tempData = (gint)(WaveData[pointer2] * (COLOR_STEP - pDrawRate[group][pointer]) +
								   WaveData[pointer2 + waveLength + 32]*pDrawRate[group][pointer] ) ;
						   fbdot (p, xoffset + j, yoffset + i - 1, TMP(color_amp[tempData>>COLOR_SHIFT]));
					  }
             }
       }

       //draw current angle line
       i= pp->p_tmp_config->beam_num[group] ;
       fbline(p, xoffset+CurrentLine_top[group][i].x, yoffset+CurrentLine_top[group][i].y, xoffset+CurrentLine_bottom[group][i].x,
       	                              yoffset+CurrentLine_bottom[group][i].y,  0x001f );
       //fbline(p , xoffset + 100, yoffset, xoffset + 100, yoffset + height, 0x001f);
	   //	   g_print("pDraw[1] = %d", pDraw[pointer]);
	   return 0;
}

// draw S scan when Linear model and Display as true depth
//  range:  real range in mm (range from several tens to several hundreds, or else will cause drawing not good)
//  width  height :  draw area width  and height
//  DataLength  :   wave data length of each beam
void CalcLinearScan(int start_element, int stop_element, int element_step, int element_qty, double _angle, int DataLength, double range, int width, int height, guchar group)
{

	   //printf("\n**************\n");
	   //printf("start = %d, stop_element = %d, element_step=%d, element_qty=%d \n", start_element, stop_element ,element_step, element_qty);
	   //printf("angle = %f, data length=%d, range=%f \n", _angle , DataLength, range);
	   //printf("width =%d , height = %d, group %d", width, height, group);
	   //printf("**************\n");
	   double real_width ;         // real coordinate width     ***  set element pitch to be 1 mm
	   double real_height;         // real coordinate height
	   double xScale     ;
	   double yScale     ;         // x y axis scale factor
	   double tan_angle_scaled ;   // tan value of reflect angle after scaled
	   int i ,  j   ;              // circle element
	   double angle ;              // reflect angle (arc)
	   double xVacc ;              // x axis vacant distance

	   double tmpX  ;

	   double xxx   ;
	   int offset   ;

	   double tmpDrawRate ;
	   double tmpDataNo   ;
	   double beam_width  ;
	   int beam_qty       ;

	   angle = fabs(_angle * G_PI /180.0);

       beam_qty  = (stop_element  - start_element - element_qty + 1 ) / element_step ;
	   real_height  = range ;

	   real_width   = stop_element - start_element + real_height * tan(angle) ;

	   xScale = width / real_width ;
	   yScale = height/ real_height ;

	   xVacc = tan(angle) * real_height * xScale ;
	   beam_width = width - xVacc ;
	   tan_angle_scaled =  xVacc / height ;

       if(pDraw[group] != NULL)       { free(pDraw[group]);  pDraw[group] = NULL ;}
	   if(pAngleZoom[group] != NULL)  { free(pAngleZoom[group]);  pAngleZoom[group] = NULL ;}
	   if(pDrawRate[group] != NULL)   { free(pDrawRate[group]);  pDrawRate[group] = NULL ;}
	   if(pDataNo[group] != NULL)     { free(pDataNo[group]);  pDataNo[group] = NULL ;}

	   pDraw[group]        = (char*) malloc (width*height)                         ;
	   pAngleZoom[group]   = (unsigned char*) malloc(width*height)                 ;
	   pDrawRate[group]    = (unsigned char*) malloc(width*height)                 ;
	   pDataNo[group]      = (int*) malloc(sizeof(int)*width*height)               ;

	   for( i = 0 ; i< height ; i++)
	   {
		   tmpX = i * tan_angle_scaled ;
		   tmpDataNo = ((double)i)/ height ;
		   for( j = 0 ; j< width ; j++ )
		   {
			   xxx = j - tmpX ;
			   if (_angle >=0 )offset  = j + i * width ;
			   else offset = xxx + (height - i) * tan_angle_scaled + i * width ;
			   if( xxx >= 0 && xxx < (width - xVacc)) *(pDraw[group]+offset) = 255 ;
			   else{
				   *(pDraw[group]+offset) = 0 ;
				   continue ;
			   }
			   tmpDrawRate = ( xxx / beam_width ) *  beam_qty ;
			   pAngleZoom[group][offset] = (unsigned char)tmpDrawRate ;
			   tmpDrawRate =  tmpDrawRate - (int)tmpDrawRate  ;
			   pDrawRate[group][offset] = (unsigned char)( tmpDrawRate * COLOR_STEP ) ;
			   pDataNo[group][offset]   = (int) DataLength * tmpDataNo ;
		   }
	   }


	   //keep the points for drawing current beam line
	   beam_width = beam_width / beam_qty ;
	   for(i = 0 ; i<= beam_qty ; i++)
	   {
		   if(_angle >=0 )
		   {
		        CurrentLine_top[group][i].x = (int) ( beam_width * i  + 0.5) ;
		        CurrentLine_bottom[group][i].x = (int)( beam_width * i  + 0.5 + xVacc );
		   }
		   else
		   {
		        CurrentLine_bottom[group][i].x =  (int) ( beam_width * i + 0.5);
		        CurrentLine_top[group][i].x   = (int)( beam_width * i  + xVacc + 0.5);
		   }
		   CurrentLine_top[group][i].y = 0 ;
		   CurrentLine_bottom[group][i].y =	height - 1 ;
	   }
	   if(beam_qty)
	   {
            CurrentLine_top[group][beam_qty].x--  ;
            CurrentLine_bottom[group][beam_qty].x-- ;
	   }
}

/* 画S扫描 角度扫查 */
void draw_s_scan (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar ut_unit)
{
	gint i, j, k, temp;
	gint beam_qty = TMP(beam_qty[groupId]);
	gint dot_qty  = GROUP_VAL_POS(groupId, point_qty);
    unsigned char* pData ;
    unsigned char* buff  ;
	switch (ut_unit)	
	{
		case UT_UNIT_SOUNDPATH:
		case UT_UNIT_TIME:
		        for(i = 0; i< beam_qty; i++)
		        {
		    	    pData = data1 + i * (dot_qty + 32) ;
					buff  = TMP(scan_data[groupId])+width*i ;
		    	    if (dot_qty <= width)
					{
						interpolation_data ( pData, buff, dot_qty, width);
					}
					else if (dot_qty  > width)
					{
						compress_data ( pData, buff, dot_qty, width,
								get_group_val (get_group_by_id (pp->p_config, groupId), GROUP_RECTIFIER));
					}

		        }

			    if (height < beam_qty)
				{
					/* 压缩s扫描 */
					for (i = 0 ; i < height ; i++)
					{
						temp = width * i * beam_qty / height;
						for (k = 0; k < width - 1; k++)
						{

						   fbdot (p, xoffset + k, yoffset + i,
										TMP(color_amp[TMP(scan_data[groupId])[temp + k]]));
						}
					}

				}
				else if (height == beam_qty)
				{
					/* 不变 */
					for (i = 0; i < beam_qty; i++)
							for (k = 0; k < width - 1; k++)
								fbdot (p, xoffset + k, yoffset + i,
										TMP(color_amp[TMP(scan_data[groupId])[width * i + k]]));
				}
				else if (height > beam_qty)
				{
					/* 拉伸 */
					for (i = 0; i < beam_qty; i++)
						for (j = 0; j <= height / beam_qty; j++)
							for (k = 0; k < width - 1; k++)
							{
								fbdot (p, xoffset + k, yoffset + i * height / beam_qty + j,
										TMP(color_amp[TMP(scan_data[groupId])[width * i + k]]));
							}
				}
				//*********************************
				// draw current angle line (VPA)
				temp = (pp->p_tmp_config->beam_num[groupId] + 0.5) * height / beam_qty ;
				fblinex(p, yoffset + temp, xoffset, xoffset+width , 0x001f);
				//***********************************
				break;
		case  UT_UNIT_TRUE_DEPTH:
				if ((!pp->sscan_mark) && pDraw[groupId])
				{
					DrawPixbuff(p, xoffset, yoffset, width, width, height,
							0,GROUP_VAL_POS(groupId, point_qty) ,  data1, groupId);
				}
				break;
		default:break;
	}
	return ;
}

/* 画S扫描 */
void draw_s_scan_r (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar ut_unit)
{
	gint i, j, k, temp;
	switch (ut_unit)	
	{
		case UT_UNIT_SOUNDPATH:
		case UT_UNIT_TIME:
				if (height < TMP(beam_qty[groupId]))
				{
					/* 压缩s扫描 */
					for (i = 0 ; i < height ; i++)		
					{
						temp = width * i * TMP(beam_qty[groupId]) / height;
						for (k = 0; k < width - 1; k++)
						{
							fbdot (p, xoffset + k, yoffset + i,
										TMP(color_amp[data1[temp + k]]));
						}
					}
				}
				else if (height == TMP(beam_qty[groupId]))
				{
					/* 不变 */
					for (i = 0; i < TMP(beam_qty[groupId]); i++)
						for (k = 0; k < width - 1; k++)
								fbdot (p, xoffset + k, yoffset + i,
										TMP(color_amp[data1[width * i + k]]));
				}
				else if (height > TMP(beam_qty[groupId]))
				{
					/* 拉伸 */
					for (i = 0; i < TMP(beam_qty[groupId]); i++)
						for (j = 0; j < height / TMP(beam_qty[groupId]); j++)
							for (k = 0; k < width - 1; k++)
								fbdot (p, xoffset + k, yoffset + i * height / TMP(beam_qty[groupId]) + j
										, TMP(color_amp[data1[width * i + k]]));
				}
				break;
		case  UT_UNIT_TRUE_DEPTH:

			break;
		default:break;
	}
	return ;
}

void draw_clb_wedge_delay (gushort *p, gint width, gint height, DOT_TYPE *data, DOT_TYPE *data1, 
							DOT_TYPE *data2,gint xoffset, gint yoffset, guchar groupId)
{
	gint	i, step;
	gint y1, y2;
	gint clb_x1, clb_x2;
	gint clb_y1, clb_y2;
	gfloat s, s_1, vel, wedge_delay;

	gint count = 0;
	gfloat clb_tmp_max_data = 0;
	gint grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	vel = (get_group_val (p_grp, GROUP_VELOCITY) / 100.0);// m/s
	wedge_delay = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_WEDGE_DELAY) / 1000.0;

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
	{
		y1 = (gint)(height*(1- (TMP_CBA(distance) - pp->tolerance/100.0 - pp->gate_start_clb/2000.0)*2000.0/(pp->gate_width_clb) )) + yoffset;
		y2 = (gint)(height*(1- (TMP_CBA(distance) + pp->tolerance/100.0 - pp->gate_start_clb/2000.0)*2000.0/(pp->gate_width_clb) )) + yoffset;
	}
	else
	{
		y1 = (gint)(height*(1- (TMP_CBA(distance) - pp->tolerance/100.0 - pp->gate_start_clb/1000.0)*1000.0/(pp->gate_width_clb) )) + yoffset;
		y2 = (gint)(height*(1- (TMP_CBA(distance) + pp->tolerance/100.0 - pp->gate_start_clb/1000.0)*1000.0/(pp->gate_width_clb) )) + yoffset;
	}
	if(y1 < yoffset)
			y1 = yoffset;
	if(y2 < yoffset)
			y2 = yoffset;

	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/*画参考线*/
	fbline (p,0, y1, width, y1,all_col_16[1]);
	fbline (p,0, y2, width, y2,all_col_16[1]);
	/* 画包络线*/
	for (i = 0; i < step; i++)
	{
		TMP(clb_real_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;
		if(TMP(clb_real_data[i]) > 100.0)
				TMP(clb_real_data[i]) = 100.0;
		if( clb_tmp_max_data < TMP(clb_real_data[i]) )
		{
				count = i;//记录最大值时的beam_num
				clb_tmp_max_data = TMP(clb_real_data[i]);//保存每次循环的最大值
		}
//		if( clb_tmp_max_data > GROUP_GATE_POS(height) )
			TMP(clb_wedge_data[i]) = (TMP(measure_data[i][1]) & 0xfffff) * 10;
//		else
//			TMP(clb_wedge_data[i]) = 0;
	}
    pp->p_tmp_config->beam_num[groupId] = count;
//	if( clb_tmp_max_data > GROUP_GATE_POS(height) )
//		TMP(clb_wedge_data[count]) = (TMP(measure_data[count][1]) & 0xfffff) * 10;

	for (i = 0; i < step - 1; i++)
	{
		if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
		{
			clb_x1 = (gint)( LAW_VAL(Angle_step)*i*width/(LAW_VAL(Angle_max)-LAW_VAL(Angle_min)) );
			clb_x2 = (gint)( LAW_VAL(Angle_step)*(i+1)*width/(LAW_VAL(Angle_max)-LAW_VAL(Angle_min)) );
		}
		else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
		{
			clb_x1 = (gint)( LAW_VAL(Elem_step)*i*width / 
						(LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1));
			clb_x2 = (gint)( LAW_VAL(Elem_step)*(i+1)*width /
						(LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1));
		}

		s   = (TMP(clb_wedge_data[i])   - wedge_delay - pp->G_delay[i]   - get_pw()) * vel / 2000000;//mm
		s_1 = (TMP(clb_wedge_data[i+1]) - wedge_delay - pp->G_delay[i+1] - get_pw()) * vel / 2000000;//mm
		if(s<0) s = 0;
		if(s_1<0) s_1 = 0;
//		printf("vel=%f s=%f \n beam_delay=%d pw=%d \n", vel, s, pp->G_delay[count], get_pw() );
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL(ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL(ut_unit)))
		{
			clb_y1 = (gint)(height*(1- (s   - pp->gate_start_clb/2000.0)*2000.0/pp->gate_width_clb)) + yoffset;
			clb_y2 = (gint)(height*(1- (s_1 - pp->gate_start_clb/2000.0)*2000.0/pp->gate_width_clb)) + yoffset;
		}
		else
		{
			clb_y1 = (gint)(height*(1- (s   - pp->gate_start_clb/1000.0)*1000.0/pp->gate_width_clb)) + yoffset;
			clb_y2 = (gint)(height*(1- (s_1 - pp->gate_start_clb/1000.0)*1000.0/pp->gate_width_clb)) + yoffset;
		}
		if(clb_y1 < yoffset)
				clb_y1 = yoffset;
		if(clb_y2 < yoffset)
				clb_y2 = yoffset;
		fbline (p, clb_x1, clb_y1, clb_x2, clb_y2, all_col_16[2]);//包络线
	}   

}

/* 画灵敏度校准包络线 */
void draw_clb_sensitivity (gushort *p, gint width, gint height, DOT_TYPE *data, DOT_TYPE *data1, 
							DOT_TYPE *data2,gint xoffset, gint yoffset, guchar groupId)
{
	gint	i, step;
	gint clb_x1, clb_x2;
	gint clb_y1, clb_y2;
	gint clb_y1_m, clb_y2_m;

	gint count = 0;
	gfloat clb_tmp_max_data = 0;
	gint y1 = (gint)(height*(1- pp->ref_amplitude/10000.0 - pp->tolerance_t/10000.0)) + yoffset;
	gint y2 = (gint)(height*(1- pp->ref_amplitude/10000.0 + pp->tolerance_t/10000.0)) + yoffset;
	if(y1 < yoffset)
			y1 = yoffset;
	if(y2 < yoffset)
			y2 = yoffset;
	if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
	{
		step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	}
	else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
	{
		step = (gint)( ( LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1 ) /
				LAW_VAL(Elem_step) ) + 1;
	}
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/*画参考线*/
	fbline (p,0, y1, width, y1,all_col_16[1]);
	fbline (p,0, y2, width, y2,all_col_16[1]);
	/* 画包络线 */
	for (i = 0; i < step; i++)
	{
		TMP(clb_real_data[i]) = ((TMP(measure_data[i][1])>>20) & 0xfff)/20.47;
		if(TMP(clb_real_data[i]) > 100.0)
				TMP(clb_real_data[i]) = 100.0;
		if( clb_tmp_max_data < TMP(clb_real_data[i]) )
		{
				count = i;//记录最大值时的beam_num
				clb_tmp_max_data = TMP(clb_real_data[i]);//保存每次循环的最大值
		}

		if(TMP(clb_max_data[i]) < TMP(clb_real_data[i]))
				TMP(clb_max_data[i]) = TMP(clb_real_data[i]);
	}

	if(TMP(clb_max_data[count]) > 100.0)
			TMP(clb_max_data[count]) = 100.0;
    pp->p_tmp_config->beam_num[groupId] = count;

	for (i = 0; i < step - 1; i++)
	{
		if (LAW_VAL (Focal_type) == AZIMUTHAL_SCAN)
		{
			clb_x1 = (gint)( LAW_VAL(Angle_step)*i*width/(LAW_VAL(Angle_max)-LAW_VAL(Angle_min)) );
			clb_x2 = (gint)( LAW_VAL(Angle_step)*(i+1)*width/(LAW_VAL(Angle_max)-LAW_VAL(Angle_min)) );
		}
		else if(LAW_VAL (Focal_type) == LINEAR_SCAN) 
		{
			clb_x1 = (gint)( LAW_VAL(Elem_step)*i*width / 
						(LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1));
			clb_x2 = (gint)( LAW_VAL(Elem_step)*(i+1)*width /
						(LAW_VAL (Last_tx_elem)-LAW_VAL(First_tx_elem) - LAW_VAL(Elem_qty) + 1));
		}
		clb_y1 = (gint)(height*(1 - TMP(clb_real_data[i])/100.0) + yoffset);
		clb_y2 = (gint)(height*(1 - TMP(clb_real_data[i+1])/100.0) + yoffset);
		clb_y1_m = (gint)(height*(1 - TMP(clb_max_data[i])/100.0) + yoffset);
		clb_y2_m = (gint)(height*(1 - TMP(clb_max_data[i+1])/100.0) + yoffset);
		if(clb_y1_m < yoffset)
				clb_y1_m = yoffset;
		if(clb_y2_m < yoffset)
				clb_y2_m = yoffset;

		fbline (p, clb_x1, clb_y1, clb_x2, clb_y2, all_col_16[0]);//实测线
		fbline (p, clb_x1, clb_y1_m, clb_x2, clb_y2_m, all_col_16[2]);//包络线
	}       

}

void draw_clb_tcg (gushort *p, gint width, gint height, DOT_TYPE *data, DOT_TYPE *data1, 
							DOT_TYPE *data2,gint xoffset, gint yoffset, guchar groupId)
{
	gint	i;
	//	gint clb_x1, clb_x2;
	//	gint clb_y1, clb_y2;
	//	gint clb_y1_m, clb_y2_m;

	//	gint count = 0;
	//	gfloat clb_tmp_max_data = 0;
	//	static gfloat clb_his_max_data = 0;
	gint y1 = (gint)(height*(1- GROUP_VAL(amplitude[0])/10000.0 - pp->tolerance_t/10000.0)) + yoffset;
	gint y2 = (gint)(height*(1- GROUP_VAL(amplitude[0])/10000.0 + pp->tolerance_t/10000.0)) + yoffset;
	if(y1 < yoffset)
			y1 = yoffset;
	if(y2 < yoffset)
			y2 = yoffset;
    //gint step = (gint)( (LAW_VAL(Angle_max) - LAW_VAL(Angle_min)) / LAW_VAL(Angle_step) + 1);
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/*画参考线*/
	fbline (p,0, y1, width, y1,all_col_16[1]);
	fbline (p,0, y2, width, y2,all_col_16[1]);
	/* 画包络线 */
	      

}

void draw_scan(guchar scan_num, guchar scan_type, guchar group,
		guint xoff, guint yoff, guchar *dot_temp, gushort *dot_temp1 )
{
	gint i, k ;
	double RANGE    ;
	double VELOCITY ;
    double START ;
    double start ;
	double range ;
	unsigned char* pData ;
	int offset;
	//guint buff_addr = (pp->p_beam_data) + 256 * 1024 ;
	guint temp2 = (pp->p_beam_data) + 3 ;
	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset += (GROUP_VAL_POS(k, point_qty) + 32) * TMP(beam_qty[k]);
	}
	//pData = (unsigned char *)(temp2 + offset + (pp->p_config->virtual_focallaw -1) * TMP(beam_qty[group]) *  (GROUP_VAL_POS( group, point_qty) + 32) ) ;
	pData = (unsigned char *)(temp2 + offset );
	switch (scan_type)
	{
		case A_SCAN:
			// beam data start address
			pData = pData + TMP(beam_num[group]) * (GROUP_VAL_POS( group, point_qty) + 32);
			// scale data to display width
			if (GROUP_VAL_POS(group, point_qty) <= TMP(a_scan_dot_qty))
			{
				interpolation_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty));
			}
			else if (GROUP_VAL_POS(group, point_qty) > TMP(a_scan_dot_qty))
			{
				compress_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty),
						get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER));
			}
			draw_a_scan (dot_temp1, TMP(a_scan_width), TMP(a_scan_height),
					TMP(scan_data[group]), dot_temp, dot_temp, xoff, yoff, group);
			break;
		case A_SCAN_R:
			// draw a scan line position need the result calculated in the s scan calculation
			if(pp->sscan_mark)
			{
				if (LAW_VAL(Focal_type) == LINEAR_SCAN)
				{
					for(i = 0; i< TMP(beam_qty[group]); i++ )
					{
						AScanDrawRange[group][i].start = 0;
						AScanDrawRange[group][i].end   = TMP(a_scan_height);
					}
				}
				else if (LAW_VAL(Focal_type) == AZIMUTHAL_SCAN)
				{
					RANGE = get_group_val (get_group_by_id (pp->p_config, group), GROUP_RANGE) / 1000.0      ;
					VELOCITY = get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100.0 ;
					START  = get_group_val (get_group_by_id (pp->p_config, group), GROUP_START) / 1000.0     ;
					start = START * VELOCITY / 2000.0 ;
				    range = RANGE * VELOCITY / 2000.0 ;
				    calc_line_position(LAW_VAL(Angle_min)/100.0, LAW_VAL(Angle_max)/100.0, LAW_VAL(Angle_step)/100.0,
						start, range, TMP(Junction), GROUP_VAL_POS(group, point_qty),TMP(s_scan_width),TMP(s_scan_height), group);
				}
			}
			pData = pData + TMP(beam_num[group]) * (GROUP_VAL_POS( group, point_qty) + 32) ;
			draw_a_scan_r (dot_temp1, TMP(a_scan_width), TMP(a_scan_height),
					pData, dot_temp, dot_temp, xoff, yoff, GROUP_VAL_POS( group, point_qty) ,group, AScanDrawRange[group][TMP(beam_num[group])].start ,AScanDrawRange[group][TMP(beam_num[group])].end );
			break;
		case B_SCAN:
			if (pp->bscan_mark)
			{
				draw_b_scan(dot_temp1, TMP(b_scan_width), TMP(b_scan_height), dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 1);
				pp->bscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
			{
				pData = pData + TMP(beam_num[group]) * (GROUP_VAL_POS( group, point_qty) + 32);
				if (GROUP_VAL_POS(group, point_qty) <= TMP(a_scan_dot_qty))
				{
					interpolation_data ( pData,
							TMP(scan_data[group]),
							GROUP_VAL_POS(group, point_qty),
							TMP(a_scan_dot_qty));
				}
				else if (GROUP_VAL_POS(group, point_qty) > TMP(a_scan_dot_qty))
				{
					compress_data ( pData,
							TMP(scan_data[group]),
							GROUP_VAL_POS(group, point_qty),
							TMP(a_scan_dot_qty),
							get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER));
				}
				draw_b_scan(dot_temp1, TMP(b_scan_width), TMP(b_scan_height), dot_temp,
						TMP(scan_data[group]), xoff, yoff, group, 0);
			}
			break;
		case S_SCAN:
			draw_s_scan(dot_temp1, TMP(s_scan_width), TMP(s_scan_height),dot_temp,
					pData, xoff, yoff, group, GROUP_VAL_POS(group, ut_unit));
			break;
		case S_SCAN_A:
			pData = (unsigned char *)(temp2 + offset) ;
			/* 计算查找表 */
			if ((GROUP_VAL_POS(group, ut_unit) == UT_UNIT_TRUE_DEPTH) &&
					pp->sscan_mark)
			{
				/* 初始化扇型查找表 */
				//CalcFanScan (
				//		LAW_VAL(Angle_min) / 100.0,
				//		LAW_VAL(Angle_max) /100.0,	/*  */
				//		LAW_VAL(Angle_step) / 100.0, TMP(a_scan_dot_qty),
				//	    TMP(s_scan_width), TMP(s_scan_height));	
				RANGE = get_group_val (get_group_by_id (pp->p_config, group), GROUP_RANGE) / 1000.0      ;
				VELOCITY = get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100.0 ;
				START  = get_group_val (get_group_by_id (pp->p_config, group), GROUP_START) / 1000.0     ;
			    	start = START * VELOCITY / 2000.0 ;
				range = RANGE * VELOCITY / 2000.0 ;

				CalcFanScan_new (LAW_VAL(Angle_min)/100.0, LAW_VAL(Angle_max)/100.0, LAW_VAL(Angle_step)/100.0,
								start, range, TMP(Junction), GROUP_VAL_POS(group, point_qty),TMP(s_scan_width),TMP(s_scan_height), group);
				/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
				pp->sscan_mark = 0;
				for (i = 0; i < TMP(s_scan_height); i++)
						memset (dot_temp1 + FB_WIDTH * (i + yoff) + xoff, 0x0, TMP(s_scan_width) * 2 );

			}  
			draw_s_scan(dot_temp1, TMP(s_scan_width), TMP(s_scan_height), dot_temp,
					pData, xoff, yoff, group, GROUP_VAL_POS(group, ut_unit));

			break;
		case S_SCAN_L: // Linear -- true depth
			if ((GROUP_VAL_POS(group, ut_unit) == UT_UNIT_TRUE_DEPTH) &&
					pp->sscan_mark)
			{
				RANGE = get_group_val (get_group_by_id (pp->p_config, group), GROUP_RANGE) / 1000.0 ;
				VELOCITY = get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100.0 ;
			    range = RANGE * VELOCITY / 2000.0 ;

				CalcLinearScan(LAW_VAL(First_tx_elem), LAW_VAL (Last_tx_elem), LAW_VAL(Elem_step),
						        LAW_VAL (Elem_qty), LAW_VAL(Angle_min)/100.0, GROUP_VAL_POS(group, point_qty),
					    		range , TMP(s_scan_width), TMP(s_scan_height),  group);

			    pp->sscan_mark = 0;
				for (i = 0; i < TMP(s_scan_height); i++)
						memset (dot_temp1 + FB_WIDTH * (i + yoff) + xoff, 0x0, TMP(s_scan_width) * 2 );
			}
			draw_s_scan(dot_temp1, TMP(s_scan_width), TMP(s_scan_height), dot_temp,
					pData, xoff, yoff, group, GROUP_VAL_POS(group, ut_unit));
			break;
		case C_SCAN:
			pData = pData + TMP(beam_num[group]) * (GROUP_VAL_POS( group, point_qty) + 32);
			if (GROUP_VAL_POS(group, point_qty) <= TMP(a_scan_dot_qty))
			{
				interpolation_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty));
			}
			else if (GROUP_VAL_POS(group, point_qty) > TMP(a_scan_dot_qty))
			{
				compress_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty),
						get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER));
			}

			if (pp->cscan_mark)
			{
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height), dot_temp,
						TMP(scan_data[group]),
						xoff, yoff, group, 1, get_cscan_source(pp->p_config, 0));
				pp->cscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height),dot_temp,
						TMP(scan_data[group]),
						xoff, yoff, group, 0, get_cscan_source(pp->p_config, 0));
			break;
		case CC_SCAN:

			pData = pData + TMP(beam_num[group]) * (GROUP_VAL_POS( group, point_qty) + 32);
			if (GROUP_VAL_POS(group, point_qty) <= TMP(a_scan_dot_qty))
			{
				interpolation_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty));
			}
			else if (GROUP_VAL_POS(group, point_qty) > TMP(a_scan_dot_qty))
			{
				compress_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty),
						get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER));
			}

			if (pp->ccscan_mark)
			{
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height), dot_temp,
						TMP(scan_data[group]),
						xoff, yoff, group, 1, get_cscan_source(pp->p_config, 1));
				pp->ccscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height),dot_temp,
						TMP(scan_data[group]) ,
						xoff, yoff, group, 0, get_cscan_source(pp->p_config, 1));
			break;
		case CCC_SCAN:
			pData = pData + TMP(beam_num[group]) * (GROUP_VAL_POS( group, point_qty) + 32);
			if (GROUP_VAL_POS(group, point_qty) <= TMP(a_scan_dot_qty))
			{
				interpolation_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty));
			}
			else if (GROUP_VAL_POS(group, point_qty) > TMP(a_scan_dot_qty))
			{
				compress_data ( pData,
						TMP(scan_data[group]),
						GROUP_VAL_POS(group, point_qty),
						TMP(a_scan_dot_qty),
						get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER));
			}


			if (pp->cccscan_mark)
			{
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height), dot_temp,
						TMP(scan_data[group]),
						xoff, yoff, group, 1, get_cscan_source(pp->p_config, 0));
				pp->cccscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height),dot_temp,
						TMP(scan_data[group]) ,  xoff, yoff, group, 0, get_cscan_source (pp->p_config, 0));
			break;
		case WEDGE_DELAY:
			draw_clb_wedge_delay(dot_temp1, TMP(clb_width), TMP(clb_height),
					NULL,
					dot_temp, dot_temp, 
					xoff, yoff, group);
			break;
		case SENSITIVITY:
			draw_clb_sensitivity(dot_temp1, TMP(clb_width), TMP(clb_height),
					NULL,
					dot_temp, dot_temp, 
					xoff, yoff, group);
			break;
		case TCG:
			break;
		default:break;
	}
	return ;
}


