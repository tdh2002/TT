

/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#include "language.h"

const gchar *content_en10[] = {
	/* 一级菜单名称 */
	" Wizard  ",	" UT Settings",		" Gate/Alarm  ",	" Measurements  ",
	" Display  ",	" Probe/Part  ",	" Focal Law  ",		" Scan  ",
	" File  "	,	" Prefenrences  "
		/**
	"1 Wizard  ",	"2 UT Settings",	"3 Gate/Alarm  ",	"4 Measurements  ",
	"5 Display  ",	"6 Probe/Part  ",	"7 Focal Law  ",	"8 Scan  ",
	"9 File  "	,	"10 Prefenrences  "
	**/
};

static const gchar *content_en20[] = {
	/* 二级菜单第0页 */
	"  Group  ","  Focal Law  ", "  Calibration  ", NULL, NULL
};

static const gchar *content_en21[] = {
	/* 二级菜单第1页 */
	"General", "Pulser", "Receiver", "Beam", "Advanced"
};

static const gchar *content_en22[] = {
	/* 二级菜单第2页 */
	"  Gate  ","  Alarm  ", "  Output  ","  Sizing Curves", NULL
};

static const gchar *content_en23[] = {
	/* 二级菜单第3页 */
	"  Reading  ","  Cursors  ", "  Table  ","  Thickness  ", "  Export  "
};

static const gchar *content_en24[] = {
	/* 二级菜单第4页 */
	"  Selection  ","  Overlay  ", "  Zoom  ","  Color  ", "  Properties  "
};

static const gchar *content_en25[] = {
	/* 二级菜单第5页 */
	"  Select  ","  Position  ", "  Characterize  ","  Parts", NULL
};

static const gchar *content_en26[] = {
	/* 二级菜单第6页 */
	"  Configuration  ","  Aperature  ", "  Beam  ","  Laws", NULL
};

static const gchar *content_en27[] = {
	/* 二级菜单第7页 */
	"  Encoder  ","  Inspection  ", "  Area  ","  Start", "  Data  "
};

static const gchar *content_en28[] = {
	/* 二级菜单第8页 */
	"  File  ","  Report  ", "  Format  ","  User Field  ", "  Notes  "
};

static const gchar *content_en29[] = {
	/* 二级菜单第9页 */
	"  Pref.  ","  System", "  Service  ","  Options  ", "  Network  "
};








static const gchar *content_en200[] = {
	/* 二级菜单第0页下的第0页三级菜单 */
	"Back", "Start", NULL, NULL, NULL, NULL
};

static const gchar *content_en201[] = {
	/* 二级菜单第0页下的第1页三级菜单 */
	"Back", "Start", NULL, NULL, NULL, NULL
};

static const gchar *content_en202[] = {
	/* 二级菜单第0页下的第2页三级菜单 */
	"Back", "Start", "Type", "Mode", "Clear Calib", NULL
};

static const gchar *content_en203[] = {
	/* 二级菜单第0页下的第3页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};

static const gchar *content_en204[] = {
	/* 二级菜单第0页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};



static const gchar *content_en210[] = {
	/* 二级菜单第1页下的第0页三级菜单 */
	"Gain", "Start", "Range", "Wedge Delay" , "Velocity",  NULL
};

static const gchar *content_en211[] = {
	/* 二级菜单第1页下的第1页三级菜单 */
	"Pulser" , "Tx/Rx mode", "Freq.(MHz)", "Voltage(V)", "PW(ns)" , "Max PRF"
};

static const gchar *content_en212[] = {
	/* 二级菜单第1页下的第2页三级菜单 */
	"Receiver", "Filter", "Rectifier", "Video Filter", "Averaging", "Reject"
};

static const gchar *content_en213[] = {
	/* 二级菜单第1页下的第3页三级菜单 */
	"Scan Offset", "Index Offset", "Angle(deg.)", "Skew(deg.)", "Beam Delay", "Gain Offset"
};

static const gchar *content_en214[] = {
	/* 二级菜单第1页下的第4页三级菜单 */
	"Set 80%", "Set Ref.", "dB Ref.", "Points Qty.", "Scale Factor", "Sum Gain"
};



static const gchar *content_en220[] = {
	/* 二级菜单第2页下的第0页三级菜单 */
	"Gate", "Parameters", "Start", "Width", "Threshold", NULL
};

static const gchar *content_en221[] = {
	/* 二级菜单第2页下的第1页三级菜单 */
	"  Alarm  ","  Group A  ", "  Condition  ","  Operator  ", "  Group B  ", "  Condition  "
};

static const gchar *content_en222[] = {
	/* 二级菜单第2页下的第2页三级菜单 */
	"  Output  ","  Alarm #  ", "  Count  ","  Sound  ", "  Delay  ", "  Hold Time  "
};

static const gchar *content_en223[] = {
	/* 二级菜单第2页下的第3页三级菜单 */
	"  Mode  ","  Curve  ", NULL, NULL, NULL, NULL
};

static const gchar *content_en224[] = {
	/* 二级菜单第2页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};



static const gchar *content_en230[] = {
	/* 二级菜单第3页下的第0页三级菜单 */
	"  List  ","  Group  ", "  Field 1  ","  Field 2  ", "  Field 3  ", "  Field 4  "
};

static const gchar *content_en231[] = {
	/* 二级菜单第3页下的第1页三级菜单 */
	"  Selection  ","  Angle  ", "  Scan  ","  Index  ", "  Add Entry  ", NULL
};

static const gchar *content_en232[] = {
	/* 二级菜单第3页下的第2页三级菜单 */
	"  Display Table  ","  Entry Image  ", "  Add Entry  ","  Delete Entry  ", "  Select Entry  ", "  Edit Comments  "
};

static const gchar *content_en233[] = {
	/* 二级菜单第3页下的第3页三级菜单 */
	"  Source  ","  Min.  ", "  Max.  ","  Echo Qty.  ", NULL, NULL
};

static const gchar *content_en234[] = {
	/* 二级菜单第3页下的第4页三级菜单 */
	"  Export Table  ", NULL, NULL ,NULL ,NULL, NULL
};





static const gchar *content_en240[] = {
	/* 二级菜单第4页下的第0页三级菜单 */
	"  Display  ","  C-Scan 1  ", NULL, NULL, NULL, NULL 
};

static const gchar *content_en241[] = {
	/* 二级菜单第4页下的第1页三级菜单 */
	"  UT Unit  ","  Grid  ", "  Sizing Curves  ","  Gate  ", "  Cursor  ", "  Overlay  "
};

static const gchar *content_en242[] = {
	/* 二级菜单第4页下的第2页三级菜单 */
	"  Display  ","  Type  ", NULL, NULL, NULL, NULL
};

static const gchar *content_en243[] = {
	/* 二级菜单第4页下的第3页三级菜单 */
	"  Select  ","  Start  ", "  End  ","  Load  ", NULL, NULL
};

static const gchar *content_en244[] = {
	/* 二级菜单第4页下的第4页三级菜单 */
	"  Scan  ","  Color  ", "  Envelope  ","  Source  ", "  Appearance  ", "  Overlay  "
};





static const gchar *content_en250[] = {
	/* 二级菜单第5页下的第0页三级菜单 */
	"  Group  ","  Group Mode  ", "  Select ","  Probe  ", "  Wedge  ", "  Auto Detect  "
};

static const gchar *content_en251[] = {
	/* 二级菜单第5页下的第1页三级菜单 */
	"  Scan Offset  ","  Index Offset  ", "  Skew(deg.) ", NULL ,NULL, NULL
};

static const gchar *content_en252[] = {
	/* 二级菜单第5页下的第2页三级菜单 */
	"  FFT  ","  Gain  ", "  Start ","  Width  ", "  Procedure  ", "  Block Name  "
};

static const gchar *content_en253[] = {
	/* 二级菜单第5页下的第3页三级菜单 */
	"  Geometry  ","  Thickness  ", "  Diameter  ","  Material  ", NULL, NULL
};

static const gchar *content_en254[] = {
	/* 二级菜单第5页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};





static const gchar *content_en260[] = {
	/* 二级菜单第6页下的第0页三级菜单 */
	"  Law Config.  ","  Connection P  ", "  Connection R ", NULL, NULL, NULL
};

static const gchar *content_en261[] = {
	/* 二级菜单第6页下的第1页三级菜单 */
	"  Element Qty.  ","  First Element  ", "  Last Element ","  Element Step  ", "  Wave Type  ", NULL
};

static const gchar *content_en262[] = {
	/* 二级菜单第6页下的第2页三级菜单 */
	"  Min.Angle  ","  Max.Angle  ", "  Angle Step ","  Focus Depth  ", NULL, NULL
};

static const gchar *content_en263[] = {
	/* 二级菜单第6页下的第3页三级菜单 */
	"  Auto Program  ","  Load Law File  ", "  Save Law File ", NULL, NULL, NULL
};

static const gchar *content_en264[] = {
	/* 二级菜单第6页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};





static const gchar *content_en270[] = {
	/* 二级菜单第7页下的第0页三级菜单 */
	"  Encoder  ","  Polarity  ", "  Type ","  Resolution  ", "  Origin  ", "  Preset  "
};

static const gchar *content_en271[] = {
	/* 二级菜单第7页下的第1页三级菜单 */
	"  Type  ","  Scan  ", "  Index ","  Scan Speed  ", NULL, NULL,
};

static const gchar *content_en272[] = {
	/* 二级菜单第7页下的第2页三级菜单 */
	"  Scan Start  ","  Scan End  ", "  Scan Resolution ","  Index Start  ", "  Index End  ", "  Index Resolution  "
};

static const gchar *content_en273[] = {
	/* 二级菜单第7页下的第3页三级菜单 */
	"  Start Mode  ","  Pause  ", "  Start ", NULL, NULL, NULL
};

static const gchar *content_en274[] = {
	/* 二级菜单第7页下的第4页三级菜单 */
	"  Storage  ","  Inspec.Data  ", NULL, NULL, NULL, NULL
};





static const gchar *content_en280[] = {
	/* 二级菜单第8页下的第0页三级菜单 */
	"  Storage  ","  Open  ", "  Save Setup As ","  Save Data  ", "  Save Mode  ", "  File Name  "
};

static const gchar *content_en281[] = {
	/* 二级菜单第8页下的第1页三级菜单 */
	"  Template  ","  File Name  ", "  Paper Size ","  Build  ", NULL, NULL
};

static const gchar *content_en282[] = {
	/* 二级菜单第8页下的第2页三级菜单 */
	"  User field  ","  Probe  ", "  Setup ","  Note  ", "  View  ", NULL
};

static const gchar *content_en283[] = {
	/* 二级菜单第8页下的第3页三级菜单 */
	"  Select  ","  Enable  ", "  Label ","  Content  ", NULL, NULL
};

static const gchar *content_en284[] = {
	/* 二级菜单第8页下的第4页三级菜单 */
	"  Edit Notes  ","  Edit Header  ", NULL, NULL, NULL, NULL
};





static const gchar *content_en290[] = {
	/* 二级菜单第9页下的第0页三级菜单 */
	"  Units  ","  Bright  ", "  Admin Password ","  Scheme  ", "  Gate Mode  ", NULL
};

static const gchar *content_en291[] = {
	/* 二级菜单第9页下的第1页三级菜单 */
	"  Clock Set  ","  Date Set  ", "  Select Key ","  Assign Key  ", NULL, NULL
};

static const gchar *content_en292[] = {
	/* 二级菜单第9页下的第2页三级菜单 */
	"  System Info  ","  Win CE  ", "  File Manager ","  Import/Export  ", "  Startup Mode  ", NULL
};

static const gchar *content_en293[] = {
	/* 二级菜单第9页下的第3页三级菜单 */
	"  Mouse  ","  EZView  ", "  Option Key ","  Remote Desktop  ", NULL, NULL
};

static const gchar *content_en294[] = {
	/* 二级菜单第9页下的第4页三级菜单 */
	"  DHCP  ","  IP Address  ", "  Subnet Mask ","  Apply  ", "  Remote Pc  ", "  Connect  "
};

const gchar **content1_en[] = 
{
	content_en20,	content_en21,	content_en22,	content_en23,	content_en24,
	content_en25,	content_en26,	content_en27,	content_en28,	content_en29
};

static const gchar **content2_en0[] = {	content_en200,	content_en201,	content_en202,	content_en203,	content_en204 };
static const gchar **content2_en1[] = {	content_en210,	content_en211,	content_en212,	content_en213,	content_en214 };
static const gchar **content2_en2[] = {	content_en220,	content_en221,	content_en222,	content_en223,	content_en224 };
static const gchar **content2_en3[] = {	content_en230,	content_en231,	content_en232,	content_en233,	content_en234 };
static const gchar **content2_en4[] = {	content_en240,	content_en241,	content_en242,	content_en243,	content_en244 };
static const gchar **content2_en5[] = {	content_en250,	content_en251,	content_en252,	content_en253,	content_en254 };
static const gchar **content2_en6[] = {	content_en260,	content_en261,	content_en262,	content_en263,	content_en264 };
static const gchar **content2_en7[] = {	content_en270,	content_en271,	content_en272,	content_en273,	content_en274 };
static const gchar **content2_en8[] = {	content_en280,	content_en281,	content_en282,	content_en283,	content_en284 };
static const gchar **content2_en9[] = {	content_en290,	content_en291,	content_en292,	content_en293,	content_en294 };

const gchar ***content2_en[] = 
{
	content2_en0,	content2_en1,	content2_en2,	content2_en3,	content2_en4,
	content2_en5,	content2_en6,	content2_en7,	content2_en8,	content2_en9
};

