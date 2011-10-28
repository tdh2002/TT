
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "language.h"

const gchar *content_ch10[] = {
	/* 一级菜单名称 */
	"向导",	"超声检测设置",		"闸门/报警",	"测量",
	"显示",	"探头/工件",	"聚焦法则",		"扫查",
	"文件"	,	"设置"
};

static const gchar *content_ch20[] = {
	/* 二级菜单第0页 */
	"组", "聚焦法则", "校准", "焊缝图示", NULL
};

static const gchar *content_ch21[] = {
	/* 二级菜单第1页 */
	"常规", "发射", "接收", "声束", "高级"
};

static const gchar *content_ch22[] = {
	/* 二级菜单第2页 */
	"闸门", "报警设置", "报警输出", "DAC/TCG", NULL
};

static const gchar *content_ch23[] = {
	/* 二级菜单第3页 */
	"检测结果", "光标", "检测记录表", "厚度设置", "输出报表"
};

static const gchar *content_ch24[] = {
	/* 二级菜单第4页 */
	"显示模式", "辅助线", "颜色", "属性", NULL
}; 

static const gchar *content_ch25[] = {
	/* 二级菜单第5页 */
	"选择", "位置", "特性", "工件", NULL
};

static const gchar *content_ch26[] = {
	/* 二级菜单第6页 */
	"配置", "声束角度", "焦点", "孔径", "计算"
};

static const gchar *content_ch27[] = {
	/* 二级菜单第7页 */
	"编码器", "扫查设置", "扫查区域", "开始", "数据"
};

static const gchar *content_ch28[] = {
	/* 二级菜单第8页 */
	"文件", "报告", "格式", "用户栏", "注释"
};

static const gchar *content_ch29[] = {
	/* 二级菜单第9页 */
	"配置", "系统设置", "服务", "网络设置",NULL
};









static const gchar *content_ch200[] = {
	/* 二级菜单第0页下的第0页三级菜单 */
	"返回", "开始", "Operation选择？？", "探头型号","自动识别", NULL, 
	"下一步", "组", "组模式","发射起始阵元", "选择",
	"扫查偏置","步进偏置", "转向角", "完成", "继续", 
	"收发模式","发射","接收","接收起始阵元"
};

static const gchar *content_ch201[] = {
	/* 二级菜单第0页下的第1页三级菜单 */
	"返回", "开始", "类型", "材料", "末阵元", 
	"阵元步进", "下一步","波型","阵元数", 
	"起始阵元", "最小角度", "最大角度", "角度步进", "完成"
};

static const gchar *content_ch202[] = {
	/* 二级菜单第0页下的第2页三级菜单 */
	"返回", "开始", "类型", "模式", NULL, "清除校准", "选择标准","重新设置", 
	"角度","增益", "开始", "范围", "回波类型", "半径1", "半径2", "深度1", 
	"深度2", "厚度1", "厚度",	"下一步", "宽度", "阈值","读取位置","半径 A", 
	"深度 A", "厚度 A", "公差","第一个角度", "最后一个角度", "清除包络", "校准", 
	"重新开始", "接受", "参考幅度", "增益补偿", "应用", "幅度", "增加点", "焊缝类型", 
	"厚度", "编码器", "参考点", "距离", "分辨率", "速度", "第一个波束", "最后一个波束",
	"读取位置1", "读取位置2"
};

static const gchar *content_ch203[] = {
	/* 二级菜单第0页下的第3页三级菜单 */
	"返回", "开始", "焊缝", "对称性", NULL, NULL
};

static const gchar *content_ch204[] = {
	/* 二级菜单第0页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};



static const gchar *content_ch210[] = {
	/* 二级菜单第1页下的第0页三级菜单 */
	"增益", "起始点", "范围", "楔块延迟" , "声速",  NULL, "参考增益", "增益(参考)"
};

static const gchar *content_ch211[] = {
	/* 二级菜单第1页下的第1页三级菜单 */
	"发射起始阵元" , "收发模式", "频率(MHz)", "发射电压(V)", "脉宽(ns)" , "脉冲重复频率"
};

static const gchar *content_ch212[] = {
	/* 二级菜单第1页下的第2页三级菜单 */
	"接收起始阵元", "滤波", "检波", "视频滤波", "平均", "抑制"
};

static const gchar *content_ch213[] = {
	/* 二级菜单第1页下的第3页三级菜单 */
	"扫查偏置", "步进偏置", "角度(°)", "转向角(°)", "声束延迟", "增益补偿"
};

static const gchar *content_ch214[] = {
	/* 二级菜单第1页下的第4页三级菜单 */
	"设为80%", "设定参考", "参考增益", "采样点", "压缩系数", "总增益"
};




static const gchar *content_ch220[] = {
	/* 二级菜单第2页下的第0页三级菜单 */
	"闸门", "参数", "起点", "宽度", "阈值", NULL, "同步", "测量方式", "射频"
};

static const gchar *content_ch221[] = {
	/* 二级菜单第2页下的第1页三级菜单 */
	"报警", "组选择", "报警条件", "报警逻辑", "组选择", "报警条件"
};

static const gchar *content_ch222[] = {
	/* 二级菜单第2页下的第2页三级菜单 */
	"输出", "报警组", "报警次数", "蜂鸣", "报警延迟", "持续时间","Group", "Data"
};

static const gchar *content_ch223[] = {
	/* 二级菜单第2页下的第3页三级菜单 */
	"操作模式", "曲线",NULL, NULL, NULL, NULL, "参考幅度", "参考幅度补偿", "曲线增益差", "参考增益", "点", "位置", "幅度", "增加", "删除", 

"Mat.Attenuatior衰减系数", "Delay", "增益"
};

static const gchar *content_ch224[] = {
	/* 二级菜单第2页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};



static const gchar *content_ch230[] = {
	/* 二级菜单第3页下的第0页三级菜单 */
	"  List  ","测量结果", "结果1","结果2", "结果3", "结果4"
};

static const gchar *content_ch231[] = {
	/* 二级菜单第3页下的第1页三级菜单 */
	"选择","%(r)", "%(m)  ","  U(r)  ", "  U(m)  ", NULL , "S(r)", "S(m)", "Data Link", "I(r)", "I(m)", "S(r&m)",  "角度",  "幅度",  "UT",  "扫查",  "步进", "增加记录"
};

static const gchar *content_ch232[] = {
	/* 二级菜单第3页下的第2页三级菜单 */
	"显示报表","记录图像", "增加记录","删除记录", "选择记录", "备注"
};

static const gchar *content_ch233[] = {
	/* 二级菜单第3页下的第3页三级菜单 */
	"数据读取","最小厚度", "最大厚度","回波次数", NULL, NULL
};

static const gchar *content_ch234[] = {
	/* 二级菜单第3页下的第4页三级菜单 */
	"输出报表", NULL, NULL ,NULL ,NULL, NULL
};





static const gchar *content_ch240[] = {
	/* 二级菜单第4页下的第0页三级菜单 */
	"显示","  Group  ", NULL, NULL, NULL, NULL,"C-Scan 1" , "C-Scan 2","Data 1", "Data 2", "Mode", "Display Range", "Avg.Scan Speed"
};

static const gchar *content_ch241[] = {
	/* 二级菜单第4页下的第1页三级菜单 */
	"超声轴单位","栅格", "DAC/TCG","闸门", "光标", "参考线"
};

static const gchar *content_ch242[] = {
	/* 二级菜单第4页下的第2页三级菜单 */
	"选择","开始", "结束","加载", "模式", NULL, "对比度", "亮度", "最小值", "最大值", "模式"
};

static const gchar *content_ch243[] = {
	/* 二级菜单第4页下的第3页三级菜单 */
	"扫描模式","颜色", "包络","数据读取", "外观", "  波形对比  ", "压缩", "最佳","比率 1:1","Imterpolation插值","颜色","方向"
};

static const gchar *content_ch244[] = {
	/* 二级菜单第4页下的第4页三级菜单 */
	NULL, NULL, NULL ,NULL ,NULL, NULL
};





static const gchar *content_ch250[] = {
	/* 二级菜单第5页下的第0页三级菜单 */
	"组","检测模式", "选择","探头", "楔块", "自动检测","自定义探头", "自定义楔块"
};

static const gchar *content_ch251[] = {
	/* 二级菜单第5页下的第1页三级菜单 */
	"扫查偏置","步进偏置", "转向角(°) ", NULL ,NULL, NULL
};

static const gchar *content_ch252[] = {
	/* 二级菜单第5页下的第2页三级菜单 */
	"FFT","增益", "闸门起点","闸门宽度", "  Procedure  ", "  Block Name  ","Gain-r"
};

static const gchar *content_ch253[] = {
	/* 二级菜单第5页下的第3页三级菜单 */
	"工件形状","厚度", "直径","材料", "焊缝", NULL
};

static const gchar *content_ch254[] = {
	/* 二级菜单第5页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};





static const gchar *content_ch260[] = {
	/* 二级菜单第6页下的第0页三级菜单 */
	"聚焦法则类型", "发射起始阵元", "接收起始阵元", NULL, NULL, NULL
};

static const gchar *content_ch261[] = {
	/* 二级菜单第6页下的第1页三级菜单 */
	"起始角度", "终止角度", "步进", "最小声束转向角", "最大声束转向角", "转向角步进"
};

static const gchar *content_ch262[] = {
	/* 二级菜单第6页下的第2页三级菜单 */
	"Type.", "Position Start", "Position End",  "Position Step", "Depth End", NULL, "Offset Start", "Offset End","Depth Start"
};

static const gchar *content_ch263[] = {
	/* 二级菜单第6页下的第3页三级菜单 */
	"阵元数", "第一个阵元", "最后一个阵元", "阵元步进", "波型", NULL
};

static const gchar *content_ch264[] = {
	/* 二级菜单第6页下的第4页三级菜单 */
	"自动计算", "加载法则文件", "保存法则文件",  "计算聚焦法则", NULL, NULL
};





static const gchar *content_ch270[] = {
	/* 二级菜单第7页下的第0页三级菜单 */
	"编码器","极性", "类型","分辨率", "编码器起点", "预置编码器"
};

static const gchar *content_ch271[] = {
	/* 二级菜单第7页下的第1页三级菜单 */
	"扫查方式","扫查轴数据源", "步进轴数据源","扫查速度", "扫查轴速度", "步进轴速度","扫查轴最大速度", "步进轴最大速度",
};

static const gchar *content_ch272[] = {
	/* 二级菜单第7页下的第2页三级菜单 */
	"扫查起点","扫查终点", "扫查轴分辨率","步进起点", "步进终点", "步进分辨率"
};

static const gchar *content_ch273[] = {
	/* 二级菜单第7页下的第3页三级菜单 */
	"开始模式","暂停", "开始", NULL, NULL, NULL
};

static const gchar *content_ch274[] = {
	/* 二级菜单第7页下的第4页三级菜单 */
	"存储","检测数据", NULL, NULL, NULL, NULL
};





static const gchar *content_ch280[] = {
	/* 二级菜单第8页下的第0页三级菜单 */
	"存储","打开", "设置另存为","保存数据", "保存模式", "文件名"
};

static const gchar *content_ch281[] = {
	/* 二级菜单第8页下的第1页三级菜单 */
	"模板 ","文件名", "纸张类型","报告预览", NULL, NULL
};

static const gchar *content_ch282[] = {
	/* 二级菜单第8页下的第2页三级菜单 */
	"用户栏","探头", "设置","注释", "查看", NULL
};

static const gchar *content_ch283[] = {
	/* 二级菜单第8页下的第3页三级菜单 */
	"选择","启用", "标签","内容", NULL, NULL
};

static const gchar *content_ch284[] = {
	/* 二级菜单第8页下的第4页三级菜单 */
	"编辑注释","编辑页眉", NULL, NULL, NULL, NULL
};





static const gchar *content_ch290[] = {
	/* 二级菜单第9页下的第0页三级菜单 */
	"单位","亮度", NULL, NULL, NULL, NULL
};

static const gchar *content_ch291[] = {
	/* 二级菜单第9页下的第1页三级菜单 */
	"时间", "日期", "语言", NULL, NULL, NULL
};

static const gchar *content_ch292[] = {
	/* 二级菜单第9页下的第2页三级菜单 */
	"系统信息", "文件管理",NULL, NULL, NULL, NULL
};

static const gchar *content_ch293[] = {
	/* 二级菜单第9页下的第3页三级菜单 */
	" IP地址", "子网掩码",NULL, NULL, NULL,NULL
};

static const gchar *content_ch294[] = {
	/* 二级菜单第9页下的第4页三级菜单 */
	/*"  DHCP  ","IP地址", "子网掩码","应用", "  Remote Pc  ", "  Connect  "*/
	NULL, NULL, NULL, NULL, NULL, NULL
};

const gchar **content1_ch[] = 
{
	content_ch20,	content_ch21,	content_ch22,	content_ch23,	content_ch24,
	content_ch25,	content_ch26,	content_ch27,	content_ch28,	content_ch29
};

static const gchar **content2_ch0[] = {	content_ch200,	content_ch201,	content_ch202,	content_ch203,	content_ch204 };
static const gchar **content2_ch1[] = {	content_ch210,	content_ch211,	content_ch212,	content_ch213,	content_ch214 };
static const gchar **content2_ch2[] = {	content_ch220,	content_ch221,	content_ch222,	content_ch223,	content_ch224 };
static const gchar **content2_ch3[] = {	content_ch230,	content_ch231,	content_ch232,	content_ch233,	content_ch234 };
static const gchar **content2_ch4[] = {	content_ch240,	content_ch241,	content_ch242,	content_ch243,	content_ch244 };
static const gchar **content2_ch5[] = {	content_ch250,	content_ch251,	content_ch252,	content_ch253,	content_ch254 };
static const gchar **content2_ch6[] = {	content_ch260,	content_ch261,	content_ch262,	content_ch263,	content_ch264 };
static const gchar **content2_ch7[] = {	content_ch270,	content_ch271,	content_ch272,	content_ch273,	content_ch274 };
static const gchar **content2_ch8[] = {	content_ch280,	content_ch281,	content_ch282,	content_ch283,	content_ch284 };
static const gchar **content2_ch9[] = {	content_ch290,	content_ch291,	content_ch292,	content_ch293,	content_ch294 };

const gchar ***content2_ch[] = 
{
	content2_ch0,	content2_ch1,	content2_ch2,	content2_ch3,	content2_ch4,
	content2_ch5,	content2_ch6,	content2_ch7,	content2_ch8,	content2_ch9
};

/* 0mm 1英寸 2微秒  */
const gchar *units_ch[] = 
{
	"(mm)", "(in)", "(μs)", NULL, "(m/s)", "(in/μs)", "(dB)", 
	"(1 to 97)", "(%)", "(ms)", "(mm/s)", "(1 to 113)", "(dB/mm)",
	"(s)", "(°)", "(Step/mm)", "(rpm)", "(dB/in)","(dB/μs)","(in/s)","VPA"," "
};

const gchar *all_menu_content_ch[] = 
{
	"PC 一发一收", "PE 脉冲回波", "TT  穿透式", "TOFD 衍射声时",		/* 0 TX_RX_MODE*/
	"PC", "PE", "TT", "TOFD",								/* 4 tx_rxmode */
	"1","1.5","2","2.25","3.5","4","5","7.5","10","12","15","20","UserDef","",	/* 8 frequence */
	"50 低", "100 中", "200 高",			               			/* 22 */
	"50", "100", "200",								/* 25 */
	"50 低", "100 高",								/* 28 Voltage  */
	"自动", "自定义",								/* 30 PW       */	
	"最大", "Max/2", "最佳","自定义",					/* 32 PRF      */
	"无  0.5-21.0 MHz", "Auto", "1 MHz", "1.5MHz", "2MHz", 
	"2.25MHz", "4MHz", "5MHz", "7.5MHz", "10MHz", "12MHz", 
	"15MHz", "20MHz", "HR", "HR 10 MHz", "无",					/* 36 filter   */
	"射频", "正半波", "负半波", "全波",							/* 52 rectifier*/
	"1", "2", "4", "8", "16",								/* 56 averaging*/
	"关", "开",									/* 61 db_ref   */
	"自动", "160", "320", "640", "自定义",						/* 63 points_qty*/
	"自动", "自定义",								/* 68 sum_gain  */
	"A", "B","I",									/* 70 gate_pos  */
	"位置", "模式",								/* 73 parameters*/
	"脉冲", "I/","A/",								/* 75 synchro */
	"Peak(^)", "Edge(/)",								/* 78 measure */
	"Absolute", "Positive", "Negative",						/* 80 rectifier_freq*/
	"1", "2","3","4","5","6","7","8","9","10","11","12","13","14","15","16",	/* 83 Alarm_pos*/
	"1 [开]", "2 [开]","3 [开]","4 [开]","5 [开]","6 [开]","7 [开]","8 [开]","9 [开]","10 [开]","11 [开]","12 [开]","13 [开]","14 [开]","15 [开]","16 [开]",	/* 99 Alarm [开] */
	"1", "2", "3", "4", "5", "6", "7", "8",						/* 115 groupA */
	"无","闸门 A","闸门 B","闸门 I","非闸门A","非闸门B","非闸门I",">最大厚度","<最小厚度",/*123 conditiona*/
	"与","或",									/* 132 operator */
	"输出 1", "输出 2","输出 3", "Analog 1", "Analog 2",			/* 134 Output_pos */
	"Output 1  [开]","Output 2  [开]","Output 3  [开]","Analog 1  [开]","Analog 2  [开]", 		/*139 Output_pos+5 */
	"None", "All", "1", "2", "3", "4", "5", "6", "7", "8", "9","10","11","12","13","14","15","16", "Multi",	/*144 alarm1*/
	"None", "All", "1  [开]", "2  [开]", "3  [开]", "4  [开]", "5  [开]", "6  [开]", "7  [开]", "8  [开]", "9  [开]","10  [开]","11  [开]","12  [开]","13  [开]","14  [开]","15  [开]","16  [开]",						/*162 alarm1*/
	"关", "A%", "B%", "厚度",									/* 181 data */ 
	"关  静音", "300Hz  音频输出","600Hz  音频输出","1000Hz  音频输出","5000Hz  音频输出",/* 185 sound*/
	"关", "300Hz","600Hz","1000Hz","5000Hz",							/* 190 sound */
	"设置", "编辑",										/* 195 mode  */
	"无","DAC","线性DAC","TCG",								/* 197 curve */
	"1 Manual Weld 1: A%. DA^. PA^. SA^", "2 Auto. Weld 1: A%. DA^. ViA^. VsA^",			/* 201 list */
	"A%  闸门A内信号峰值幅度", "AdBA  闸门A内信号幅度与阈值之差（dB）", 
	"AdBr  闸门A内峰值幅度与参考信号幅度之差（dB）", "B%  闸门B内信号峰值幅度",
	"BdBB 闸门A内信号幅度与阈值之差（dB）", "BdBr  闸门B内峰值幅度与参考信号幅度之差（dB）", 
	"A^  闸门A内信号峰值位置", "B^  闸门B内信号峰值位置", "I/  穿过闸门I时信号的位置", "I(w)/  Position in gate I in water", 
	"T(A^)  厚度", "ML  材料损失百分比", "%(r)  参考光标位置的幅度值",
	"%(m)  测量光标位置的幅度值", "%(m-r) 测量光标的幅度值减去参考信号的幅度值)", 
	"U(r)  参考光标在超声轴上的位置", "U(m)  测量光标在超声轴上的位置", 
	"U(m-r)  超声轴上测量光标位置与参考光标位置之差", "P(r)  相对于参考光标的探头位置", 
	"P(m)  相对于测量光标的探头位置", "P(m-r)  探头相对于测量光标与参考光标差值的位置", 
	"S(r)  扫查轴上参考光标的位置", "S(m)  扫查轴上测量光标的位置", 
	"S(m-r) 扫查轴上测量光标与参考光标的距离", "I(r)  步进轴上参考光标的位置",
	"I(m)  步进轴上测量光标的位置", "I(m-r)  步进轴上测量光标与参考光标的距离", 
	"RA^  声束出射点与闸门A内检测到的缺陷之间的距离", "RB^  声束出射点与闸门B内检测到的缺陷之间的距离",
	"PA^  探头前表面与闸门A内检测到的缺陷的距离", "PB^  探头前表面与闸门B内检测到的缺陷的距离", 
	"DA^  闸门A内检测到的缺陷在工件中的深度", "DB^  闸门B内检测到的缺陷在工件中的深度", 
	"SA^  声束出射点到闸门A内检测到的缺陷之间的声程", "SB^  声束出射点到闸门B内检测到的缺陷之间的声程",
	"ViA^  闸门A内检测到的缺陷相对于步进轴的体积定位", "ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位",
	"VsA^  闸门A内检测到的缺陷相对于扫查轴的体积定位", "VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位",
	"LA^  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数", "LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数", 
	"E%  闸门A中包络线的峰值幅度", "E^  闸门A内包络线的峰值位置", "E-3dB  闸门A内信号在-3 dB处包络线的宽度",
	"E-6dB  闸门A内信号在-6dB处包络线的宽度", "E-12dB  闸门A内信号在-12dB处包络线的宽度", 
	"E-20dB  闸门A内信号在-20dB处包络线的宽度", "API-DL根据API标准规定的缺陷长度", 
	"API-HW  根据API 标准规定的半波宽度", "AWS-D A  根据AWS-D1.5 A标准的指示电平", 
	"AWS-D B  根据AWS-D1.5 B标准的零点参考电平", "AWS-D C  根据AWS-D1.5C标准的衰减系数", 
	"AWS-D D  根据AWS-D1.5 D标准的指征率", "AWS-D 45  根据AWS-D1.5 45标准的不连续性严重等级", 
	"AWS-D 60  根据AWS-D1.5 60标准的不连续性严重等级", "AWS-D 70  根据AWS-D1.5 70标准的不连续性严重等级", 
	"AWS-D CL  根据AWS-D1.5标准的不连续性严重等级", "比例  比例系数", 
	"%(U(r))  参考光标在超声轴上对应的信号幅度", 
	"%(U(m))  测量光标在超声轴上对应的信号幅度", "None  None",		  /* 203 Field */
	"A-扫描", "B-扫描", "C-扫描", "Strip Chart", "S-扫描", "R and M", "Reference", "Measure", "数据", /* 263 Selection */
	"None", "Reference", "Measurement", "Auto",							  /* 272 Data Link */
	"A^", "A^-I^", "A^-I/", "B^", "B^-I^", "B^-I/", "B^-A^", "I^", "I/",				  /* 276 Source*/
	"A  A-扫描", "B  B-扫描", "C  C-扫描", "S  S-扫描", "A-B  A -扫描B-扫描", "A-B-C  A-扫描B-扫描C-扫描", 
	"A-B-S  A-扫描B-扫描S-扫描", "A-C-[C] A-扫描C-扫描[C-扫描]", "A-S-[C]  A扫描-S扫描- C扫描[C-扫描]", "PA-TOFD  PA-TOFD", 
	"Strip Chart-[A]  Strip Chart[A-Scan]",								 /* 285 Display */
	"A", "B", "C", "S", "A-B", "A-B-C", "A-B-S", "A-C-[C]", "A-S-[C]", "PA-TOFD", "Strip Chart-[A]", /* 296 Displ   */
	"所有组","当前组",										 /* 307 group   */
	"A%", "B%", "厚度","I/","关",								 /* 309 C-Scan1  */
	"A%", "B%", "厚度","关",									 /* 314 Data2 */
	"All & A-Scan", "Current & A-Scan", "所有组","当前组",						 /* 318 Mode  */
	"声程", "时间", "真实深度",								 /* 322 UT Unit */
	"蓝色", "绿色", "红色", "黄色", "黑色", "关",						 /* 325 Grid */
	" A-扫描", "B-扫描", "S-扫描", "Strip Chart", "未校正的C-扫描", "关",			 /* 331 display*/
	"Absolute", "Start", "Center", "光标", "闸门 A", "闸门 B", 					 /* 337  Type*/
	"幅度", "校正-TOFD", "深度",							 /*343 Select*/
	"扩展", "压缩",									/* 346 Mode */
	"A-扫描", "B-扫描", "C-扫描","S-扫描", "FFT", "Strip Chart",		/* 348 Scan */
	"蓝色", "绿色", "红色", "黄色", "黑色", "白色",						/* 354 Color*/
	"无", "Infinite",										/* 360 Envelope*/
	"常规", "峰值(%)", "最小厚度", "全部法则",							/* 362 Source*/
	"中空", "填充", "中空 双色", "填充 双色",						/* 366 Appearance*/
	"无", "峰值", "参考",									/* 370 Overlay*/
	"黄色", "蓝色", "白色","黑色",								/* 373 FFT->Color*/
	"垂直", "水平",									/* 377 Orientation*/
	"增加", "1", "2", "3", "4", "5", "6", "7", "8", "删除",	/* 379 Group */
	"UT (常规超声)", "PA (相控阵)",	"UT1(独立通道1)","UT2(独立通道2)",						/* 385 Group Mode*/
	"UT", "PA",	 "UT1", "UT2",										/* 387 */
	"选择系列", "自定义",									/* 389 Select*/
	"0.0", "90.0", "180.0", "270.0", "自定义",							/* 391 Skew  */
	"板", "凸面", "凹面",										/* 396 Geometry */
/*	"氧化铝", "铝", "BERYLIUM铍", "黄铜", "镉", "铜", "低碳钢",	*/		/* 399 Material */
	"铝", "普通钢", "不锈钢", "黄铜", "铜",
	"铁", "铅", "尼龙", "银", "金", "锌", "钛", 
	"锡", "环氧树脂", "冰", "镍", "树脂玻璃", "聚苯乙烯",
	"瓷器", "聚氯乙烯", "石英玻璃", "硫化橡胶", "聚四氟乙烯", "水",	/* 24 个材料 */
	
	"扇形扫查", "线性扫查", 
	"深度扫查", "静态扫查",				
	"扇形", "线性", "深度", "静态",				/* 410 Law config*/
	"半声程", "真实深度", "投影 ", "任意面", "自动",
	"半声程", "真实深度", "投影 ", "任意面", "自动",	/*  focalpoint type */
	"纵波  5890.0m/s", "横波  3240.0m/s"," ",						/* 414 Wave Type*/
	"纵波", "横波", " "	,										/* 417 Wave Type*/
	"1", "2",											/* 420 Encoder*/
	"常规", "反转",										/* 422 Polarity */
	"时钟方向", "正交", "向上", "向下", "时钟方向 向上", "时钟方向 向下", "正交向上", "正交向下", "Clicker",/* 424 Type */
	"单线扫查", "栅格扫查", "螺旋扫查",						/* 433 Type */
	"时间", "编码器 1", "编码器 2",								/* 436 Inspection -> Scan*/
	"自动", "编码器 1", "编码器 2", "Top Turn", "关",						/* 439 Index */
	"全部重置", "重置编码器", "数据清零",							/* 444 Start Mode */
	"Last", "A%", "最大厚度", "最小厚度",							/* 447 Storage */
	"全部 A & C 扫描", "全部C扫描",								/* 451 Inspec.Data */
	"存储卡   \"\\存储卡\"", "内存  \"\\User\"",				/* 453 Storage */
	"存储卡", "内存",								/* 455 Storage */
	"检测数据", "缺陷记录表", "屏幕", "报告",					/* 457 Save Mode */
	"完整  (标准)",										/* 461 Template*/
	"完整",											/* 462 Template*/
	"Letter", "Legal", "A4", "B4",									/* 463 Paper Size */
	"表格", "当前布局", "关",								/* 467 View*/
	"1", "2","3", "4", "5", "6", "7", "8", "9", "10",						/* 470 File Select */
	"ADJML", "技术员", "客户", "项目", "地点", "工件编号", "工序", "","","",	/* 480 Label */
	"姓名", "Name","Name","Name","Name","Name","Name","","", "",					/* 490 Content */
	"毫米", "英寸",									/* 500 Units   */
	"室内", "户外",										/* 502 Scheme  */
	"By Group", "By Context",									/* 504 Gate Mode*/
	"F2", "F3", "F4", "F5", "F6", "DIN1", "DIN2", "DIN3", "DIN4",					/* 506 Select Key */
	"增益", "范围", "起始", "开始/停止", "暂停/重新开始", "校准", "报告", "设置另存为", "缩放", "Add Entry", "显示", "Context Select", 

"闸门", "用户栏", "蜂鸣器", "注释",						/* 515 Assign Key */
	"无", "重置编码器 2", "暂停/重新开始", "保存数据", "清除全部",				/* 531 Assign Key */
	"关", "Mouse", "Control Area",									/* 536 Mouse */
	"增加", "更改",				/* Wizard->Group->Operation  OPERATION */
	"组 1", "组2", "组3", "组4", "组5", "组6", "组7", "组8",	/* Wizard->Group->Group  WGROUP */
	"编码器", "超声", "校准",	/* Wizard->Calibration->Type */
	"速度", "延迟", "灵敏度", "TCG",	/* Wizard->Calibration->Mode */
	"API-DL", "AWS-D1.5",				/* Select Code */
	"半径", "深度", "厚度",		/* Echo type */
	"Compressive", "Tensile",			/* Weld type */
	"Custom", "Angle Beam", "Contact", "Immersion",	/* Probe Type */
	"50 Ω","500 Ω",/*Damping*/
	//"None","I", "V", "V Offset", "Double V", "Double V Offset", "J", "U",	/* Weld */
	"I","V","Double V", "U",
	"对称","左单边坡口","右单边坡口",
	"English","中文",
	""
};

/*  0  */
//const gchar *type_en[] = {"编码器", "超声", "标准"};
//const gchar *calibration_mode_en[] = {"速度", "延迟", "灵敏度", "TCG"};

/*  3  */
const gchar *list_ch[]		= {"1 Manual Weld 1: A%. DA^. PA^. SA^", "2 Auto. Weld 1: A%. DA^. ViA^. VsA^"};
const gchar *list1_ch[]		= {"1", "2"};

const gchar *field1_ch[]	= 
{
	"A% 闸门A内峰值幅度", "AdBA  闸门A内峰值幅度与闸门阈值幅度之差（dB）", 
	"AdBr  闸门A内峰值幅度与参考信号幅度之差（dB）", "B%  闸门B内峰值幅度",
	"BdBB闸门B内峰值幅度与闸门阈值幅度之差（dB）", "BdBr  闸门B内峰值幅度与参考信号幅度之差（dB）", 
	"A^ 闸门A内峰值位置", "B^ 闸门B内峰值位置", "I/ 闸门I的前沿位置", "I(w)/  Position in gate I in water", 
	"T(A^)  厚度", "ML  材料损失百分比", "%(r)  参考光标位置的幅度值",
	"%(m)  测量光标位置的幅度值", "%(m-r) 测量光标的幅度值减去参考信号的幅度值)", 
	"U(r)  参考光标在超声轴上的位置", "U(m)  测量光标在超声轴上的位置", 
	"U(m-r)  超声轴上测量光标位置与参考光标位置之差", "P(r)  相对于参考光标的探头位置",
	"P(m)  相对于测量光标的探头位置", "P(m-r)  探头相对于测量光标与参考光标差值的位置", 
	"S(r)  扫查轴上参考光标的位置", "S(m)  扫查轴上测量光标的位置",
	"S(m-r) 扫查轴上测量光标与参考光标的距离", "I(r)  步进轴上参考光标的位置",
	"I(m)  步进轴上测量光标的位置", "I(m-r)  步进轴上测量光标与参考光标的距离",
	"RA^  声束出射点与闸门A内检测到的缺陷之间的距离", "RB^  声束出射点与闸门B内检测到的缺陷之间的距离",
	"PA^  探头前表面与闸门A内检测到的缺陷的距离", "PB^  探头前表面与闸门B内检测到的缺陷的距离", 
	"DA^  闸门A内检测到的缺陷在工件中的深度", "DB^  闸门B内检测到的缺陷在工件中的深度", 
	"SA^  声束出射点到闸门A内检测到的缺陷之间的声程", "SB^  声束出射点到闸门B内检测到的缺陷之间的声程",
	"ViA^  闸门A内检测到的缺陷相对于步进轴的体积定位", "ViB^  闸门B内检测到的缺陷相对于步进轴的体积定位",
	"VsA^  闸门A内检测到的缺陷相对于扫查轴的体积定位", "VsB^  闸门B内检测到的缺陷相对于扫查轴的体积定位",
	"LA^  声波从探头到闸门A探测到的缺陷在材料中的路径改变次数", "LB^  声波从探头到闸门B探测到的缺陷在材料中的路径改变次数",
	"E%  闸门A中包络线的峰值幅度", "E^  闸门A内包络线的峰值位置", "E-3dB  闸门A内信号在-3 dB处包络线的宽度",
	"E-6dB  闸门A内信号在-6dB处包络线的宽度", "E-12dB  闸门A内信号在-12dB处包络线的宽度", 
	"E-20dB  闸门A内信号在-20dB处包络线的宽度", "API-DL根据API标准规定的缺陷长度", 
	"API-HW  根据API 标准规定的半波宽度", "AWS-D A  根据AWS-D1.5 A标准的指示电平", 
	"AWS-D B  根据AWS-D1.5 B标准的零点参考电平", "AWS-D C  根据AWS-D1.5C标准的衰减系数", 
	"AWS-D D  根据AWS-D1.5 D标准的指征率", "AWS-D 45  根据AWS-D1.5 45标准的不连续性严重等级", 
	"AWS-D 60  根据AWS-D1.5 60标准的不连续性严重等级", "AWS-D 70  根据AWS-D1.5 70标准的不连续性严重等级", 
	"AWS-D CL  根据AWS-D1.5标准的不连续性严重等级", "比例  比例系数", 
	"%(U(r))  参考光标在超声轴上对应的信号幅度", 
	"%(U(m))  测量光标在超声轴上对应的信号幅度",	  /* 203 Field */
};
/*{"A%  闸门A内峰值位置", "AdBA  闸门A内峰值幅度与闸门阈值幅度之差（dB）", "AdBr  闸门A内峰值幅度与参考信号幅度之差（dB）","B%  闸门B内峰值位置"};
 */
const gchar *field_ch[]		=
{
	"A%", "AdBA", "AdBr", "B%", "BdBB", "BdBr", "A^", "B^", "I/", "I(w)/", 
	"T(A^)", "ML", "%(r)", "%(m)", "%(m-r)", 
	"U(r)", "U(m)", "U(m-r)", "P(r)", "P(m)", "P(m-r)", "S(r)", "S(m)", 
	"S(m-r)", "I(r)", "I(m)", "I(m-r)", "RA^", "RB^", "PA^", "PB^", 
	"DA^", "DB^", "SA^", "SB^", "ViA^", "ViB^",	"VsA^", "VsB^",	"LA^", "LB^", 
	"E%", "E^", "E-3dB", "E-6dB", "E-12dB", "E-20dB", "API-DL", 
	"API-HW", "AWS-D A", "AWS-D B", "AWS-D C", "AWS-D D", "AWS-D 45", 
	"AWS-D 60", "AWS-D 70", "AWS-D CL", "Scale", "%(U(r))", "%(U(m))"
};
const gchar *field_unit_ch[]		=
{
	"%", "dB", "dB", "%", "dB", "dB", "%", "%", "%", "%", 
	"%", "dB", "dB", "%", "dB", "dB", "%", "%", "%", "%", 
	"%", "dB", "dB", "%", "dB", "dB", "%", "%", "%", "%", 
	"%", "dB", "dB", "%", "dB", "dB", "%", "%", "%", "%", 
	"%", "dB", "dB", "%", "dB", "dB", "%", "%", "%", "%", 
	"%", "dB", "dB", "%", "dB", "dB", "%", "%", "%", "%" 
};
const gchar *field_unit_ch_mm[]		=
{
	"%", "dB", "dB",  "%", "dB",  "dB", "mm", "mm", "mm", "mm", 
	"mm", "%",  "%",  "%", "%",   "mm", "mm", "mm", "mm", "mm", 
	"mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", 
	"mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", 
	"  ", "  ", "%",  "%",  "mm", "mm", "mm", "mm", "mm", "dB", 
	"dB", "dB", "dB", "  ", "  ", "  ", "  ", "X",  "%",  "%" 
};
/*{"A%", "AdBA", "AdBr", "B%"};
 * */
//const gchar *selection_en[] = {"A-scan", "B-scan", "C-scan", "Strip Chart", "S-scan", "R and M", "Reference", "Measure", "Data"};
//const gchar *source_en[] = {"A^", "A^-I^", "A^-I/", "B^", "B^-I^", "B^-I/", "B^-A^", "I^", "I/"};


const gchar *warning_info_ch[] = {
	"max laws is 256!"
};


