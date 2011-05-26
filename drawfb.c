
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

#define COLOR_STEP 32     //    4  8  16  32  64
#define COLOR_SHIFT 5     //    2  3   4   5   6
static gchar*	pDraw  = NULL;          // 是否扇形区域 
static guchar*	pAngleZoom = NULL; // 处于哪个角度区间 
static guchar*	pDrawRate = NULL; // 填充比例 
static gint*	pDataNo = NULL; // 数据在数组中的列号 
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

void change_fb ()
{
	if (!vinfo.yoffset)
		vinfo.yoffset = 400;
	else
		vinfo.yoffset = 0;

	if (ioctl(fd_fb, FBIOPAN_DISPLAY, &vinfo) < 0)
	{
		printf("Error oppandisplay \n");
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

/* 画A扫描 data 原始数据 data1 data2 包络数据 */
void draw_a_scan_r (gushort *p, guint width, guint height, 
		DOT_TYPE *data, DOT_TYPE *data1, DOT_TYPE *data2,
		guint xoffset, guint yoffset, guchar groupId)
{
	gint	i;
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/* 画回波 */
	for (i = 0; i < height - 1; i++)
	{
		fbline (p, 
				xoffset + width * HEIGHT_TABLE[data[i]],
				yoffset + i,
				xoffset + width * HEIGHT_TABLE[data[i + 1]],
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

	for (i = 0; i < height - 1; i++)
		memcpy(p + (i + yoffset) * FB_WIDTH + xoffset, p + (i + yoffset + 1) * FB_WIDTH + xoffset, width * 2);

	/* 编码器同步的时候怎么画 */
/*	memcpy(p + (yoffset) * FB_WIDTH, p + (yoffset + 1) * FB_WIDTH, FB_WIDTH * (height - 1) * 2);*/

	for (j = 0; j < width - 1; j++)
		fbdot (p, xoffset + j, yoffset + height - 1, TMP(color_amp[data1[j]]));

//	g_print ("xoffset = %d yoffset = %d width =%d height =%d \n",
//			xoffset, yoffset, width, height);
#if 0
	for (i = 0; i < height - 1; i++)
		for (j = 0; j < width - 1; j++)
			fbdot (p, xoffset + j, yoffset + i,
					TMP(color_amp[data[i * width + j]]));
#endif

}

/* 画C扫描 */
void draw_c_scan (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar mark, guchar source)
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


	for (offset = 0, k = 0 ; k < CFG(groupId); k++)
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



int CalcFanScan(gdouble startAngle, gdouble endAngle,
		gdouble stepAngle, gint startWave, gint endWave,
		gint widstep, gint width, gint height)
{
       // 实际坐标 
       gdouble xLeftmost             ;    //      扇形 最左点坐标 
       gdouble xRightmost            ;    //     扇形 最右点坐标 
       gdouble yTopmost              ;    //     最上点 
       gdouble yBottonmost           ;    //     最下点 
       // 起始\终止\步进  角度 
       gdouble a1, a2 , a3       ;
       // 实际窗口 宽 高 
       gdouble tempW , tempH         ;
       // 实际窗口 入射点坐标 
       gdouble xOrg , yOrg           ;
       // 拉伸窗口 入射点坐标 
       gdouble xOrgS, yOrgS          ;
       // 拉伸窗口 信号起 止半径 
       gdouble startWaveS , endWaveS ;
       gdouble xxx  , yyy ;
       gdouble tLength    ;
       gdouble xScale ;
       gdouble yScale ;
       int i , j ;
       int RowNo ;
       gdouble* pScales ;
       gdouble* pScalesAngle ;
       gdouble tempx, tempy ;
       gdouble tempScale;
       gdouble tAngle ;
       int iAngle ;
       // 为全局指针分配内在
	   if (pDraw)
	   {
		   free(pDraw);
		   free(pAngleZoom);
		   free(pDrawRate);
		   free(pDataNo);
		   pDraw = NULL;
		   pAngleZoom = NULL;
		   pDrawRate = NULL;
		   pDataNo = NULL;
	   }
       pDraw      = (char*)malloc(width*height);
       pAngleZoom = (unsigned char*)malloc(width*height);
       pDrawRate  = (unsigned char*)malloc(width*height);
       pDataNo    = (int*) malloc(sizeof(int)*width*height) ;      
       
       a1 = startAngle*G_PI/180.0 ;
       a2 = endAngle*G_PI/180.0   ;
       a3 = stepAngle*G_PI/180.0  ;
       
       //  扫描数据列数
	   RowNo = (int)((endAngle-startAngle)/stepAngle +1)   ;
	   //  临时比例队列
       pScales = (gdouble*)malloc(sizeof(gdouble)*RowNo)   ;
       pScalesAngle = (gdouble*)malloc(sizeof(gdouble)*RowNo);

       // calculate the fan in real coordinate
       xLeftmost = a1>=0 ? sin(a1)*startWave:sin(a1)*endWave ;
       xRightmost = a2>=0 ? sin(a2)*endWave:sin(a2)*startWave;
       if(fabs(a1)>fabs(a2))
       {
              yTopmost = cos(a1)*startWave    ;
              yBottonmost = cos(a2)*endWave   ;
       }
       else
       {
              yTopmost = cos(a2)*startWave    ;
              yBottonmost= cos(a1)*endWave    ;
       }
       if(a1<=0&&a2>=0) yBottonmost = endWave ;
       // 波型的实际宽度和高度. 
       // 虚拟发射点坐标 
       tempW = (gdouble)(xRightmost - xLeftmost)         ;
       tempH = (gdouble)(yBottonmost- yTopmost)          ;
       xOrg = 0 - xLeftmost                   ;
       yOrg = 0 - yTopmost                    ;
       // 实际尺寸 转 显示尺寸 比例 
       xScale = width/tempW                   ;
       yScale = height/tempH                  ;
       // 显示坐标系中 虚拟发射点坐标 
       xOrgS = (int)xOrg*xScale               ;
       yOrgS = (int)yOrg*yScale               ;
       //求每一列波型的 比例尺
       //和拉伸后显示的扫描角度 
       for(i = 0; i< RowNo ; i++)
       {
             tempx = xScale*(sin(a1+i*a3)*endWave+xOrg)     ;
             tempy = yScale*(cos(a1+i*a3)*endWave+yOrg)     ;
             tempx = tempx - xOrgS                                 ;
             tempy = tempy - yOrgS                                 ;
             if(tempy != 0)
                    pScalesAngle[i] = atan(tempx/tempy)            ;
             else   pScalesAngle[i] = G_PI/2                         ;    // 折射角不可能等于 90 度 
             pScales[i] = sqrt(tempx*tempx+tempy*tempy)/endWave    ;
             g_print("pScales = %f ; pScalesAngle = %f \n",pScales[i], pScalesAngle[i]*180/G_PI) ;
       }
       g_print("yScale = %f,\n", yScale) ;
  
       if(a1<0 && a2>0)
       {
               for(i = 1; i<RowNo - 1; i++)
               {
                     if(pScalesAngle[i]!=0)
                     {
                        if(pScalesAngle[i]<0)
                            {
                                       tempScale = (pScales[0]*fabs(pScalesAngle[i]) + yScale*fabs(pScalesAngle[i]-pScalesAngle[0]))/fabs(pScalesAngle[0]); 
                                       if(pScales[i]<tempScale) 
                                                  pScales[i] = tempScale;
                            }
                        else
                            {
                                       tempScale = (pScales[RowNo - 1]*fabs(pScalesAngle[i]) + 
                                                      yScale*fabs(pScalesAngle[i]-pScalesAngle[RowNo - 1]))/fabs(pScalesAngle[RowNo - 1]);
                                       if(pScales[i]<tempScale)
                                                  pScales[i] = tempScale;
                            }
                     }
               }
       }
       else
       {
               for(i = 1; i<RowNo - 1; i++)
               {
                   tempScale = (pScales[0]*fabs(pScalesAngle[i]-pScalesAngle[RowNo-1]) + pScales[RowNo-1]*fabs(pScalesAngle[i]-pScalesAngle[0]))/fabs(pScalesAngle[0]-pScalesAngle[RowNo -1]); 
                   if(pScales[i]<tempScale) 
                              pScales[i] = tempScale;
                   g_print("tempScale = %f \n ******" , pScales[i]);           
                   g_print("pScales = %f ; pScalesAngle = %f \n",pScales[i], pScalesAngle[i]*180/G_PI) ;
               }
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
                  if(tAngle > a2|| tAngle < a1) {  pDraw[i*width+ j] = 0;  continue ;}
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
                  tempScale = pScales[iAngle]*(1-tempScale)+pScales[iAngle+1]*tempScale  ;     
                  startWaveS = startWave*tempScale   ;
                  endWaveS   = endWave*tempScale     ;    
                  pDataNo[i*width+j] = (int)((tLength-startWaveS)/tempScale )   ;
                  if(tLength > endWaveS || tLength<startWaveS) { pDraw [i*width + j]= 0 ; continue ;}    
                  pDraw [i*width + j]= 255 ;
             }
       }
       free(pScales);
	   free(pScalesAngle);
	   return 0;
}
/*
{
       // 实际坐标 
       int xLeftmost             ;    //     扇形 最左点坐标 
       int xRightmost            ;    //     扇形 最右点坐标 
       int yTopmost              ;    //     最上点 
       int yBottonmost           ;    //     最下点 
       //  起始\终止\步进  角度 
       gdouble a1, a2 , a3       ;
       //  实际窗口 宽 高 
       gdouble tempW , tempH         ;
       //  实际窗口 入射点坐标 
       int xOrg , yOrg           ;
       //  拉伸窗口 入射点坐标 
       int xOrgS, yOrgS          ;
       //  拉伸窗口 信号起 止半径 
       int startWaveS , endWaveS ;
       
       int xxx  , yyy ;
       gdouble tLength    ;
       
       gdouble xScale ;
       gdouble yScale ;
       
       int i , j ;
       int RowNo ;
       gdouble* pScales ;
       gdouble* pScalesAngle ;
       gdouble tempx, tempy ;
       gdouble tempScale;
       
       gdouble tAngle ;
       int iAngle ;
	   if (pDraw)
	   {
		   free(pDraw);
		   free(pAngleZoom);
		   free(pDrawRate);
		   free(pDataNo);
	   }
       // 为全局指针分配内在
       pDraw      = (gchar*)  malloc(width*height);
       pAngleZoom = (guchar*) malloc(width*height);
       pDrawRate  = (guchar*) malloc(width*height);
       pDataNo    = (gint*) malloc(sizeof(gint)*width*height) ;      
       
       a1 = startAngle*G_PI/180.0 ;
       a2 = endAngle*G_PI/180.0   ;
       a3 = stepAngle*G_PI/180.0  ;
       
       //  扫描数据列数
	   RowNo = (int)((endAngle-startAngle)/stepAngle +1)   ;
	   //  临时比例队列
       pScales = (gdouble*)malloc(sizeof(gdouble)*RowNo)   ;
       pScalesAngle = (gdouble*)malloc(sizeof(gdouble)*RowNo);

       // calculate the fan in real coordinate
       xLeftmost = a1>=0 ? sin(a1)*startWave:sin(a1)*endWave ;
       xRightmost = a2>=0 ? sin(a2)*endWave:sin(a2)*startWave;
       if(fabs(a1)>fabs(a2))
       {
              yTopmost = cos(a1)*startWave    ;
              yBottonmost = cos(a2)*endWave   ;
       }
       else
       {
              yTopmost = cos(a2)*startWave    ;
              yBottonmost= cos(a1)*endWave    ;
       }
       if(a1<=0&&a2>=0) yBottonmost = endWave ;
       // 波型的实际宽度和高度. 
       // 虚拟发射点坐标 
       tempW = (gdouble)(xRightmost - xLeftmost)         ;
       tempH = (gdouble)(yBottonmost- yTopmost)          ;
       xOrg = 0 - xLeftmost                   ;
       yOrg = 0 - yTopmost                    ;
       // 实际尺寸 转 显示尺寸 比例 
       xScale = width/tempW                   ;
       yScale = height/tempH                  ;
       // 显示坐标系中 虚拟发射点坐标 
       xOrgS = (int)xOrg*xScale               ;
       yOrgS = (int)yOrg*yScale               ;
       //求每一列波型的 比例尺
       //和拉伸后显示的扫描角度 
       for(i = 0; i< RowNo ; i++)
       {
             tempx = xScale*(sin(a1+i*a3)*endWave+xOrg)     ;
             tempy = yScale*(cos(a1+i*a3)*endWave+yOrg)     ;
             tempx = tempx - xOrgS                                 ;
             tempy = tempy - yOrgS                                 ;
             if(tempy != 0)
                    pScalesAngle[i] = atan(tempx/tempy)            ;
             else   pScalesAngle[i] = G_PI/2                         ;    // 折射角不可能等于 90 度 
             pScales[i] = sqrt(tempx*tempx+tempy*tempy)/endWave    ;
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
                  if(tAngle > a2|| tAngle < a1) {  pDraw[i*width+ j] = 0;  continue ;}
                  iAngle = 0;
                  while(tAngle>pScalesAngle[iAngle]&&tAngle<a2)
                  {
                        iAngle++ ;
                  }
                  iAngle--;
                  pAngleZoom[i*width+j] = iAngle  ;
                  tempScale = (tAngle - pScalesAngle[iAngle])/(pScalesAngle[iAngle+1] - pScalesAngle[iAngle])  ;
                  if(tempScale > 1) tempScale = 1;
                  pDrawRate[i*width+j] = (unsigned char)(tempScale*16);
                  tempScale = pScales[iAngle]*(1-tempScale)+pScales[iAngle+1]*tempScale                      ;     
                  startWaveS = startWave*tempScale   ;
                  endWaveS   = endWave*tempScale     ;    
                  pDataNo[i*width+j] = (int)((tLength-startWaveS)/tempScale )                ;
                  if(tLength > endWaveS || tLength<startWaveS) { pDraw [i*width + j]= 0 ; continue ;}    
                  pDraw [i*width + j]= 255 ;
             }
       }

       free (pScales);
	   free (pScalesAngle);
	   return 0;
}
*/

int DrawPixbuff(gushort *p,
		guint xoffset, guint yoffset,
		gint widstep, gint width, gint height,
		gint startWave, gint endWave, DOT_TYPE *WaveData)
{   
       gint	i, j;
       gint pointer ;
       gint pointer2;
       gint tempData ;
       gint waveLength  = endWave - startWave + 1;
       
       for(i = 0; i< height; i++)
       {
             for(j = 0; j < width; j++)
             {
                      pointer = i*width +j ;
                      if(pDraw[pointer] != 0)
                      { 
                           pointer2 = (int)(pAngleZoom[pointer] * waveLength + pDataNo[pointer]);
                           tempData = (int)(WaveData[pointer2] * (COLOR_STEP - pDrawRate[pointer]) +
								   WaveData[pointer2 + waveLength]*pDrawRate[pointer] ) ;
						   fbdot (p, xoffset + j, yoffset + i - 1, TMP(color_amp[tempData>>COLOR_SHIFT]));
					  }
             }
       }
		
	   //	   g_print("pDraw[1] = %d", pDraw[pointer]);
	   return 0;
}

/* 画S扫描 角度扫查 */
void draw_s_scan (gushort *p, guint width, guint height, DOT_TYPE *data, DOT_TYPE *data1,
		guint xoffset, guint yoffset, guchar groupId, guchar ut_unit)
{
	gint i, j, k, temp;
	gint beam_qty = TMP(beam_qty[groupId]);
	switch (ut_unit)	
	{
		case UT_UNIT_SOUNDPATH:
		case UT_UNIT_TIME:
				if (height < beam_qty)
				{
					/* 压缩s扫描 */
					for (i = 0 ; i < height ; i++)		
						for (k = 0; k < width - 1; k++)
						{
							temp = width * i * beam_qty / height;
								fbdot (p, xoffset + k, yoffset + i,
										TMP(color_amp[data1[temp + k]]));
						}
				}
				else if (height == beam_qty)
				{
					/* 不变 */
					for (i = 0; i < beam_qty; i++)
							for (k = 0; k < width - 1; k++)
								fbdot (p, xoffset + k, yoffset + i,
										TMP(color_amp[data1[width * i + k]]));
				}
				else if (height > beam_qty)
				{
					/* 拉伸 */
					for (i = 0; i < beam_qty; i++)
						for (j = 0; j <= height / beam_qty; j++)
							for (k = 0; k < width - 1; k++)
							{
								fbdot (p, xoffset + k, yoffset + i * height / beam_qty + j,
										TMP(color_amp[data1[width * i + k]]));
							}
				}
				break;
		case  UT_UNIT_TRUE_DEPTH:
				if ((!pp->sscan_mark) && pDraw)
				{
					DrawPixbuff(p, xoffset, yoffset, width, width, height,
							0, TMP(a_scan_dot_qty),  data1);
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
						for (k = 0; k < width - 1; k++)
						{
							temp = width * i * TMP(beam_qty[groupId]) / height;
								fbdot (p, xoffset + k, yoffset + i,
										TMP(color_amp[data1[temp + k]]));
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


void draw_scan(guchar scan_num, guchar scan_type, guchar group,
		guint xoff, guint yoff, guchar *dot_temp, gushort *dot_temp1 )
{
	gint i;
	switch (scan_type)
	{
		case A_SCAN:
			draw_a_scan(dot_temp1, TMP(a_scan_width), TMP(a_scan_height),
					TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
					dot_temp, dot_temp, 
					xoff, yoff, group);
			break;
		case A_SCAN_R:
			draw_a_scan_r(dot_temp1, TMP(a_scan_width), TMP(a_scan_height),
					TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
					dot_temp, dot_temp, 
					xoff, yoff, group);
			break;
		case B_SCAN:
			if (pp->bscan_mark)
			{
				draw_b_scan(dot_temp1, TMP(b_scan_width), TMP(b_scan_height),dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 1);
				pp->bscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
				draw_b_scan(dot_temp1, TMP(b_scan_width), TMP(b_scan_height),dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 0);
			break;
		case S_SCAN:
			draw_s_scan(dot_temp1, TMP(s_scan_width), TMP(s_scan_height),dot_temp,
					TMP(scan_data[group]),
					xoff, yoff, group, GROUP_VAL_POS(group, ut_unit));
			break;
		case S_SCAN_L:
			draw_s_scan(dot_temp1, TMP(s_scan_width), TMP(s_scan_height),dot_temp,
					TMP(scan_data[group]),
					xoff, yoff, group, GROUP_VAL_POS(group, ut_unit));
			break;
		case S_SCAN_A:
			/* 计算查找表 */
			if ((GROUP_VAL_POS(group, ut_unit) == UT_UNIT_TRUE_DEPTH) &&
					pp->sscan_mark)
			{
				/* 初始化扇型查找表 */
				CalcFanScan (
						LAW_VAL(Angle_min) / 100.0,
						LAW_VAL(Angle_max) /100.0,	/*  */
						LAW_VAL(Angle_step) / 100.0, 0, TMP(a_scan_dot_qty),
						TMP(s_scan_width), TMP(s_scan_width), TMP(s_scan_height));
				pp->sscan_mark = 0;
				/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
				for (i = 0; i < TMP(s_scan_height); i++)
					memset (dot_temp1 + FB_WIDTH * (i + yoff) + xoff, 0x0, TMP(s_scan_width) * 2 );
			}
			draw_s_scan(dot_temp1, TMP(s_scan_width), TMP(s_scan_height), dot_temp,
					TMP(scan_data[group]),
					xoff, yoff, group, GROUP_VAL_POS(group, ut_unit));
			break;
		case C_SCAN:
			if (pp->cscan_mark)
			{
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height), dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 1, CFG(c_scan1));
				pp->cscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height),dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 0, CFG(c_scan1));
			break;
		case CC_SCAN:
			if (pp->ccscan_mark)
			{
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height), dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 1, CFG(c_scan1));
				pp->ccscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
			draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height),dot_temp,
					TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
					xoff, yoff, group, 0, CFG(c_scan2));
			break;
		case CCC_SCAN:
			if (pp->cccscan_mark)
			{
				draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height), dot_temp,
						TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
						xoff, yoff, group, 1, CFG(c_scan11));
				pp->cccscan_mark = 0;	/* mark 的时候把画图区清空 */
			}
			else
			draw_c_scan(dot_temp1, TMP(c_scan_width), TMP(c_scan_height),dot_temp,
					TMP(scan_data[group]) + TMP(a_scan_width) * TMP(beam_num[group]),
					xoff, yoff, group, 0, CFG(c_scan11));
			break;
		default:break;
	}
	return ;
}

