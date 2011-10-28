
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "language.h"

const gchar *content_en10[] = {
	/* 一级菜单名称 */
	" Wizard  ",	" UT Settings",		" Gate/Alarm  ",	" Measurements  ",
	" Display  ",	" Probe/Part  ",	" Focal Law  ",		" Scan  ",
	" File  "	,	" Preferences  "
};

static const gchar *content_en20[] = {
	/* 二级菜单第0页 */
	"Group", "Focal Law", "Calibration", "Weld", NULL
};

static const gchar *content_en21[] = {
	/* 二级菜单第1页 */
	"General", "Pulser", "Receiver", "Beam", "Advanced"
};

static const gchar *content_en22[] = {
	/* 二级菜单第2页 */
	"Gate", "Alarm", "Output", "Sizing Curves", NULL
};

static const gchar *content_en23[] = {
	/* 二级菜单第3页 */
	"Reading", "Cursors", "Table", "Thickness", "Export"
};

static const gchar *content_en24[] = {
	/* 二级菜单第4页 */
	"Selection", "Overlay", "Color", "Properties", NULL
};

static const gchar *content_en25[] = {
	/* 二级菜单第5页 */
	"Select", "Position", "Characterize", "Parts", NULL
};

static const gchar *content_en26[] = {
	/* 二级菜单第6页 */
	"Configuration", "Beam Angle", "Focal Point", "Aperture", "Laws"
};

static const gchar *content_en27[] = {
	/* 二级菜单第7页 */
	"Encoder", "Inspection", "Area", "Start", "Data"
};

static const gchar *content_en28[] = {
	/* 二级菜单第8页 */
	"File", "Report", "Format", "User Field", "Notes"
};

static const gchar *content_en29[] = {
	/* 二级菜单第9页 */
	"Pref.", "System", "Service", "Network", NULL
};









static const gchar *content_en200[] = {
	/* 二级菜单第0页下的第0页三级菜单 */
	"Back", "Start", "Operation", "Probe","Auto Detect", NULL, 
	"Next", "Source", "Group Mode","Connection P", "Select",
	"Scan Offset","Index Offet", "Skew", "Finish", "Continue", 
	"Tx/Rx mode","Pulser","Receiver","Connection R"
};

static const gchar *content_en201[] = {
	/* 二级菜单第0页下的第1页三级菜单 */
	"Back", "Start", "Law Type", "Material", "Last Element", 
	"Element Step", "Next","Wave Type","Element Qty.", 
	"First Element", "Min.Angle", "Max.Angle", "Angle Step", "Generate"
};

static const gchar *content_en202[] = {
	/* 二级菜单第0页下的第2页三级菜单 */
	"Back", "Start", "Type", "Mode", NULL, "Clear Calib", "Select Code","Reset All",
	"Angle","Gain", "Start", "Range", "Echo Type", "Radius 1", "Radius 2", "Depth 1",
	"Depth 2", "Thickness 1", "Thickness 2",	"Next", "Width", "Threshold","Get Position","Radius A",
	"Depth A", "Thickness A", "Tolerance","First Angle", "Last Angle", "Clear Env.", "Calibrate",
	"Restart", "Accept", "Ref.Amplitude", "Comp.Gain", "Apply", "Amplitude", "Add Point", "Weld Type",
	"Thickness", "Encoder", "Origin", "Distance", "Resolution", "Velocity","First VPA","Last VPA",
	"Get Position1", "Get Position2"
};

static const gchar *content_en203[] = {
	/* 二级菜单第0页下的第3页三级菜单 */
	"Back", "Start", "Weld", "Symmetry", NULL, NULL,"Next","Weld land height", "Weld land offset","Finish","Fill zone height", "Fill zone angle","Fill zone radius"
};

static const gchar *content_en204[] = {
	/* 二级菜单第0页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};



static const gchar *content_en210[] = {
	/* 二级菜单第1页下的第0页三级菜单 */
	"Gain", "Start", "Range", "Wedge Delay" , "Velocity",  NULL, "Gain-r", "Gain(Ref.)"
};

static const gchar *content_en211[] = {
	/* 二级菜单第1页下的第1页三级菜单 */
	"Pulser" , "Tx/Rx mode", "Freq.(MHz)", "Voltage(V)", "PW(ns)" , "PRF"
};

static const gchar *content_en212[] = {
	/* 二级菜单第1页下的第2页三级菜单 */
	"Receiver", "Filter", "Rectifier", "Video Filter", "Averaging", "Reject","Damping"
};

static const gchar *content_en213[] = {
	/* 二级菜单第1页下的第3页三级菜单 */
	"Scan Offset", "Index Offset", "Angle(°)", "Skew", "Beam Delay", "Gain Offset"
};

static const gchar *content_en214[] = {
	/* 二级菜单第1页下的第4页三级菜单 */
	"Set 80%", "Set Ref.", "dB Ref.", "Points Qty.", "Scale Factor", "Sum Gain"
};




static const gchar *content_en220[] = {
	/* 二级菜单第2页下的第0页三级菜单 */
	"Gate", "Parameters", "Start", "Width", "Threshold", NULL, "Synchro.", "Measure", "RF"
};

static const gchar *content_en221[] = {
	/* 二级菜单第2页下的第1页三级菜单 */
	"Alarm", "Group A", "Condition", "Operator", "Group B", "Condition"
};

static const gchar *content_en222[] = {
	/* 二级菜单第2页下的第2页三级菜单 */
	"Output", "Alarm #", "Count", "Sound", "Delay", "Hold Time","Group", "Data"
};

static const gchar *content_en223[] = {
	/* 二级菜单第2页下的第3页三级菜单 */
	"Mode", "Curve",NULL, NULL, NULL, NULL, "Ref.Amplitude", "Ref.Ampl.Offset", "Curve Step", "Reference Gain", "Point", "Position", "Amplitude", "Add", "Delete", "Mat.Attenuatior", "Delay", "Gain"
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
	"  Selection  ","  %(r)  ", "  %(m)  ","  U(r)  ", "  U(m)  ", NULL , "S(r)", "S(m)", "Data Link", "I(r)", "I(m)", "S(r&m)",  "Angle",  "Amplitude",  "UT",  "Scan",  "Index", "Add Entry"
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
	"  Display  ","  Group  ", NULL, NULL, NULL, NULL,"C-Scan 1" , "C-Scan 2","Data 1", "Data 2", "Mode", "Display Range", "Avg.Scan Speed"
};

static const gchar *content_en241[] = {
	/* 二级菜单第4页下的第1页三级菜单 */
	"  UT Unit  ","  Grid  ", "  Sizing Curves  ","  Gate  ", "  Cursor  ", "  Overlay  "
};

static const gchar *content_en242[] = {
	/* 二级菜单第4页下的第3页三级菜单 */
	"  Select  ","  Start  ", "  End  ","  Load  ", "Mode", NULL, "Contrast", "Brightness", "Min", "Max", "Mode"
};

static const gchar *content_en243[] = {
	/* 二级菜单第4页下的第4页三级菜单 */
	"  Scan  ","  Color  ", "  Envelope  ","  Source  ", "  Appearance  ", "  Overlay  ", "Compression", "Optimum","Ratio 1:1","Interpolation","Color","Orientation"
};

static const gchar *content_en244[] = {
	/* 二级菜单第3页下的第4页三级菜单 */
	NULL, NULL, NULL ,NULL ,NULL, NULL
};





static const gchar *content_en250[] = {
	/* 二级菜单第5页下的第0页三级菜单 */
	"  Group  ","  Group Mode  ", "  Select ","  Probe  ", "  Wedge  ", "  Auto Detect  ","Define Probe", "Define Wedge"
};

static const gchar *content_en251[] = {
	/* 二级菜单第5页下的第1页三级菜单 */
	"  Scan Offset  ","  Index Offset  ", "  Skew(°) ", NULL ,NULL, NULL
};

static const gchar *content_en252[] = {
	/* 二级菜单第5页下的第2页三级菜单 */
	"  FFT  ","  Gain  ", "  Start ","  Width  ", "  Procedure  ", "  Block Name  ","Gain-r"
};

static const gchar *content_en253[] = {
	/* 二级菜单第5页下的第3页三级菜单 */
	"  Geometry  ","  Thickness  ", "  Diameter  ","  Material  ", "Weld", NULL
};

static const gchar *content_en254[] = {
	/* 二级菜单第5页下的第4页三级菜单 */
	NULL, NULL, NULL, NULL, NULL, NULL
};





static const gchar *content_en260[] = {
	/* 二级菜单第6页下的第0页三级菜单 */
	"Law Type", "Connection P", "Connection R", NULL, NULL, NULL
};

static const gchar *content_en261[] = {
	/* 二级菜单第6页下的第1页三级菜单 */
	"Min.Angle", "Max.Angle", "Angle Step", "Min.Beamskew", "Max.Beamskew", "BeamskewStep"
};

static const gchar *content_en262[] = {
	/* 二级菜单第6页下的第2页三级菜单 */
	"Type.", "Position Start", "Position End",  "Position Step", "Depth End", NULL, "Offset Start", "Offset End","Depth Start"
};

static const gchar *content_en263[] = {
	/* 二级菜单第6页下的第3页三级菜单 */
	"Element Qty.", "First Element", "Last Element", "Element Step", "Wave Type", NULL
};

static const gchar *content_en264[] = {
	/* 二级菜单第6页下的第4页三级菜单 */
	"Auto Program", "Load Law File", "Save Law File",  "Cal Focallaw", NULL, NULL
};





static const gchar *content_en270[] = {
	/* 二级菜单第7页下的第0页三级菜单 */
	"  Encoder  ","  Polarity  ", "  Type ","  Resolution  ", "  Origin  ", "  Preset  "
};

static const gchar *content_en271[] = {
	/* 二级菜单第7页下的第1页三级菜单 */
	"  Type  ","  Scan  ", "  Index ","  Scan Speed  ", "Scan Speed", "Index Speed","  Max.Scan Speed  ", "Max.Index Speed",
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
	"Units", "Bright", NULL, NULL, NULL, NULL
};

static const gchar *content_en291[] = {
	/* 二级菜单第9页下的第1页三级菜单 */
	"Clock Set \n(HH:MM:SS)", "Date Set", "Language", NULL, NULL, NULL
};

static const gchar *content_en292[] = {
	/* 二级菜单第9页下的第2页三级菜单 */
	"  System Info  ", "  File Manager ", NULL, NULL, NULL, NULL
};

static const gchar *content_en293[] = {
	/* 二级菜单第9页下的第3页三级菜单 */
/*	"  Mouse  ", "  Option Key ","  Remote Desktop  ", NULL, NULL,NULL*/
	"  IP Address  ", "  Subnet Mask ", NULL, NULL, NULL, NULL
};

static const gchar *content_en294[] = {
	/* 二级菜单第9页下的第4页三级菜单 */
	/*"  DHCP  ","  IP Address  ", "  Subnet Mask ","  Apply  ", "  Remote Pc  ", "  Connect  "*/
	NULL, NULL, NULL, NULL, NULL, NULL
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

/* 0mm 1英寸 2微秒  */
const gchar *units_en[] = 
{
	"(mm)", "(in)", "(μs)", NULL, "(m/s)", "(in/μs)", "(dB)", 
	"(1 to 97)", "(%)", "(ms)", "(mm/s)", "(1 to 113)", "(dB/mm)",
	"(s)", "(°)", "(Step/mm)", "(rpm)", "(dB/in)","(dB/μs)","(in/s)","VPA"," "
};

const gchar *all_menu_content_en[] = 
{
	"PC Pitch-and-Catch", "PE Pulse-Echo", "TT Through-Transmission", "TOFD Time-of-Flight Diffraction",		/* 0 TX_RX_MODE*/
	"PC", "PE", "TT", "TOFD",								/* 4 tx_rxmode */
	"1","1.5","2","2.25","3.5","4","5","7.5","10","12","15","20","UserDef","",	/* 8 frequence */
	"50 Low", "100 Middle", "200 High",						/* 22 */
	"50", "100", "200",								/* 25 */
	"50 Low", "100 High",								/* 28 Voltage  */
	"Auto", "UserDef",								/* 30 PW       */	
	"Auto Max", "Max/2", "Optimum","UserDef",					/* 32 PRF      */
	"None  0.5-21.0 MHz", "Auto", "1 MHz", "1.5MHz", "2MHz", 
	"2.25MHz", "4MHz", "5MHz", "7.5MHz", "10MHz", "12MHz", 
	"15MHz", "20MHz", "HR", "HR 10 MHz", "None",					/* 36 filter   */
	"RF", "HW+", "HW-", "FW",							/* 52 rectifier*/
	"1", "2", "4", "8", "16",								/* 56 averaging*/
	"Off", "On",									/* 61 db_ref   */
	"Auto", "160", "320", "640", "UserDef",						/* 63 points_qty*/
	"Auto", "UserDef",								/* 68 sum_gain  */
	"A", "B","I",									/* 70 gate_pos  */
	"Position", "Mode",								/* 73 parameters*/
	"Pulse", "I/","A^",								/* 75 synchro */
	"Peak(^)", "Edge(/)",								/* 78 measure */
	"Absolute", "Positive", "Negative",						/* 80 rectifier_freq*/
	"1", "2","3","4","5","6","7","8","9","10","11","12","13","14","15","16",	/* 83 Alarm_pos*/
	"1 [On]", "2 [On]","3 [On]","4 [On]","5 [On]","6 [On]","7 [On]","8 [On]","9 [On]","10 [On]","11 [On]","12 [On]","13 [On]","14 [On]","15 [On]","16 [On]",	/* 99 Alarm [On] */
	"1", "2", "3", "4", "5", "6", "7", "8",						/* 115 groupA */
	"None","Gate A","Gate B","Gate I","Not Gate A","Not Gate B","Not Gate I",">Max.Thickness","<Min.Thickness",/*123 conditiona*/
	"AND","OR",									/* 132 operator */
	"Output 1", "Output 2","Output 3", "Analog 1", "Analog 2",			/* 134 Output_pos */
	"Output 1  [On]","Output 2  [On]","Output 3  [On]","Analog 1  [On]","Analog 2  [On]", 		/*139 Output_pos+5 */
	"None", "All", "1", "2", "3", "4", "5", "6", "7", "8", "9","10","11","12","13","14","15","16", "Multi",	/*144 alarm1*/
	"None", "All", "1  [On]", "2  [On]", "3  [On]", "4  [On]", "5  [On]", "6  [On]", "7  [On]", "8  [On]", "9  [On]","10  [On]","11  [On]","12  [On]","13  [On]","14  [On]","15  [On]","16  [On]",						/*162 alarm1*/
	"Off", "A%", "B%", "Thickness",									/* 181 data */ 
	"Off  No Sound", "300Hz  Audio output at 300 Hz","600Hz  Audio output at 600 Hz","1000Hz  Audio output at 1000 Hz","5000Hz  Audio output at 5000 Hz",											/* 185 sound*/
	"Off", "300Hz","600Hz","1000Hz","5000Hz",							/* 190 sound */
	"Setup", "Edit",										/* 195 mode  */
	"None","DAC","Linear DAC","TCG",								/* 197 curve */
	"1 Manual Weld 1: A%. DA^. PA^. SA^", "2 Auto. Weld 1: A%. DA^. ViA^. VsA^",			/* 201 list */

	"A%  Peak amplitude in gate A", "AdBA  Signal in gate minus gate level for gate A", 
	"AdBr  Diff. between signal and reference in gate A", "B%  Peak amplitude in gate B",
	"BdBB Signal in gate minus gate level for gate B", "BdBr  Diff. between signal and reference in gate B", 
	"A^  Position in gate A", "B^  Position in gate B", "I/  Position in gate I", "I(w)/  Position in gate I in water", 
	"T(A^)  Thickness", "ML  Percentage of material loss", "%(r)  Amplitude value for the reference cursor",
	"%(m)  Amplitude value of the measurement cursor", "%(m-r)  Amplitude (measurement cursor - reference cursor)", 
	"U(r)  Ultrasonic position of the reference cursor", "U(m)  Ultrasonic position of the measurement cursor", 
	"U(m-r)  Ultrasonic axis (measurement cursor - reference cursor)", "P(r)  Probe position of the reference cursor", 
	"P(m)  Probe position of the measurement cursor", "P(m-r)  Probe (measurement cursor - reference cursor)", 
	"S(r)  Scan position of the reference cursor", "S(m)  Scan position of the measurement cursor", 
	"S(m-r) Scan axis (measurement cursor - reference cursor)", "I(r)  Index position of the reference cursor",
	"I(m)  Index position of the measurement cursor", "I(m-r)  Index axis (measurement cursor - reference cursor)", 
	"RA^  Reference point to the indication in gate A", "RB^  Reference point to the indication in gate B",
	"PA^  Probe front face to the indication in gate A", "PB^  Probe front face to the indication in gate B", 
	"DA^  Depth of the indication in gate A", "DB^  Depth of the indication in gate B", 
	"SA^  Sound path of the indication in gate A", "SB^  Sound path of the indication in gate B",
	"ViA^  Volumetric position in gate A on the index axis", "ViB^  Volumetric position in gate B on the index axis",
	"VsA^  Volumetric position in gate A on the scan axis", "VsB^  Volumetric position in gate B on the scan axis",
	"LA^  Leg (skip) of the indication in gate A", "LB^  Leg (skip) of the indication in gate B", 
	"E%  Peak amplitude of the envelope in gate A", "E^  Position of envelop in gate A", "E-3dB  Envelop signal at -3 dB in gate A",
	"E-6dB  Envelop signal at -6 dB in gate A", "E-12dB  Envelop signal at -12 dB in gate A", 
	"E-20dB  Envelop signal at -20 dB in gate A", "API-DL  Defect length extension according to API code", 
	"API-HW  Half-wave width according to API code", "AWS-D A  Indication level for AWS-D1.5 A", 
	"AWS-D B  Zero reference level for AWS-D1.5 B", "AWS-D C  Attenuation factor for AWS-D1.5 C", 
	"AWS-D D  Indication rating for AWS-D1.5 D", "AWS-D 45  Discontinuity Severity Class for AWS-D1.5 45", 
	"AWS-D 60  Discontinuity Severity Class for AWS-D1.5 60", "AWS-D 70  Discontinuity Severity Class for AWS-D1.5 70", 
	"AWS-D CL  Discontinuity Severity Class for AWS-D1.5", "Scale  Scale factor", 
	"%(U(r))  Signal amplitude at reference ultrasonic cursor position", 
	"%(U(m))  Signal amplitude at measurement ultrasonic cursor position", "None  None",		  /* 203 Field */
	"A-scan", "B-scan", "C-scan", "Strip Chart", "S-scan", "R and M", "Reference", "Measure", "Data", /* 263 Selection */
	"None", "Reference", "Measurement", "Auto",							  /* 272 Data Link */
	"A^", "A^-I^", "A^-I/", "B^", "B^-I^", "B^-I/", "B^-A^", "I^", "I/",				  /* 276 Source*/
	"A  A-Scan", "B  B-Scan", "C  C-Scan", "S  S-Scan", "A-B  A-Scan B-Scan", "A-B-C  A-Scan B-Scan C-Scan", 
	"A-B-S  A-Scan B-Scan S-Scan", "A-C-[C] A-Scan C-Scan [C-Scan]", "A-S-[C]  A-Scan S-Scan [C-Scan]", "PA-TOFD  PA-TOFD", 
	"Strip Chart-[A]  Strip Chart[A-Scan]",								 /* 285 Display */
	"A", "B", "C", "S", "A-B", "A-B-C", "A-B-S", "A-C-[C]", "A-S-[C]", "PA-TOFD", "Strip Chart-[A]", /* 296 Displ   */
	"All","Current",										 /* 307 group   */
	"A%", "B%", "Thickness","I/","Off",								 /* 309 C-Scan1  */
	"A%", "B%", "Thickness","Off",									 /* 314 Data2 */
	"All & A-Scan", "Current & A-Scan", "All", "current",						 /* 318 Mode  */
	"Sound Path", "Time", "True Depth",								 /* 322 UT Unit */
	"Blue", "Green", "Red", "Yellow", "Black", "Off",						 /* 325 Grid */
	"A-Scan", "B-Scan", "S-Scan", "Strip Chart", "Uncorrected C-Scan", "Off",			 /* 331 display*/
	"Absolute", "Start", "Center", "Cursor", "Gate A", "Gate B", 					 /* 337  Type*/
	"Amplitude", "Rectification-TOFD", "Depth",							 /*343 Select*/
	"Exclusion", "Compression",									/* 346 Mode */
	"A-Scan", "B-Scan", "C-Scan", "S-Scan", "FFT", "Strip Chart",					/* 348 Scan */
	"Blue", "Green", "Red", "Yellow", "Black", "White",						/* 354 Color*/
	"None", "Infinite",										/* 360 Envelope*/
	"Normal", "Highest(%)", "Thinnest", "All Laws",							/* 362 Source*/
	"Hollow", "Fill", "Hollow 2 Colors", "Fill 2 Colors",						/* 366 Appearance*/
	"None", "Peak", "Reference",									/* 370 Overlay*/
	"Yellow", "Blue", "White","Black",								/* 373 FFT->Color*/
	"Vertical", "Horizontal",									/* 377 Orientation*/
	"Add", "1", "2", "3", "4", "5", "6", "7", "8", "Remove",	/* 379 Group */
	"UT (PA Connector)", "PA (Phassed Array)",	"UT1(Conventional UT)","UT2(Conventional UT)",					/* 385 Group Mode*/
	"UT", "PA", "UT1", "UT2",											/* 387 */
	"Select Tx/Rx", "Defines",									/* 389 Select*/
	"0.0", "90.0", "180.0", "270.0", "UserDef",							/* 391 Skew  */
	"Plate", "OD", "ID",										/* 396 Geometry */
/*	"AL OXIDE", "ALUMINUM", "BERYLIUM", "BRASS", "CADMIUM", "COPPER", "STEEL.MILD",	*/		/* 399 Material */
	"Aluminum", "Steel common", "Steel stainless", "Brass", "Copper",
	"Iron", "Lead", "Nylon", "Silver", "Gold", "Zinc", "Titanium", 
	"Tin", "Epoxy resin", "Ice", "Nickel", "Plexiglass", "Polystyrene",
	"Porcelain", "PVC", "Quartz glass", "Rubber vulcanized", "Teflon", "Water",	/* 24 个材料 */
	
	"Azimuthal    Angular sector", "Linear          Linear sector", 
	"Depth          Depth sector", "Static          Static sector",				
	"Azimuthal", "Linear", "Depth", "Static",				/* 410 Law config*/
	"Half Path", "True Depth", "Projection ", "Focal Plane", "Automatic",
	"Half Path", "True Depth", "Projection ", "Focal Plane", "Automatic",	/*  focalpoint type */
	"LW  5890.0m/s", "SW  3240.0m/s"," ",						/* 414 Wave Type*/
	"LW", "SW", " "	,										/* 417 Wave Type*/
	"1", "2",											/* 420 Encoder*/
	"Normal", "Inverse",										/* 422 Polarity */
	"Clock/Dir", "Quad", "Up", "Down", "Clock/Dir Up", "Clock/Dir Down", "Quad Up", "Quad Down", "Clicker",/* 424 Type */
	"One-Line Scan", "Raster Scan", "Helicoidal Scan",						/* 433 Type */
	"Time", "Encoder 1", "Encoder 2",								/* 436 Inspection -> Scan*/
	"Auto", "Encoder 1", "Encoder 2", "Top Turn", "Off",						/* 439 Index */
	"Reset All", "Reset Encoder", "Reset Data",							/* 444 Start Mode */
	"Last", "A%", "Max.Thickness", "Min.Thickness",							/* 447 Storage */
	"All A & C scans", "All C scans",								/* 451 Inspec.Data */
	"Storage Card   \"\\Storage Card\"", "Internal Memory  \"\\User\"",				/* 453 Storage */
	"Storage Card", "Internal Memory",								/* 455 Storage */
	"Inspection Data", "Indication Table", "Screen", "Report",					/* 457 Save Mode */
	"Complete  (Standard)",										/* 461 Template*/
	"Complete",											/* 462 Template*/
	"Letter", "Legal", "A4", "B4",									/* 463 Paper Size */
	"Table", "Current Layout", "Off",								/* 467 View*/
	"1", "2","3", "4", "5", "6", "7", "8", "9", "10",						/* 470 File Select */
	"ADJML", "Technician04", "Customer", "Project", "Site", "Part Number", "Procedure", "","","",	/* 480 Label */
	"NAME", "Name","Name","Name","Name","Name","Name","","", "",					/* 490 Content */
	"Millimeters", "Inches",									/* 500 Units   */
	"Indoor", "Outdoor",										/* 502 Scheme  */
	"By Group", "By Context",									/* 504 Gate Mode*/
	"F2", "F3", "F4", "F5", "F6", "DIN1", "DIN2", "DIN3", "DIN4",					/* 506 Select Key */
	"Gain", "Range", "Start", "Start/Stop", "Pause/Resume", "Calibrate", "Report", "Save Setup As", "Zoom", "Add Entry", "Display", "Context Select", "Gate", "User Field", "Buzzer", "Notes",						/* 515 Assign Key */
	"None", "Preset Encoder 2", "Pause/Resume", "Save Data", "Clear All",				/* 531 Assign Key */
	"Off", "Mouse", "Control Area",									/* 536 Mouse */
	"Add", "Modify",				/* Wizard->Group->Operation  OPERATION */
	"Group 1", "Group 2", "Group 3", "Group 4", "Group 5", "Group 6", "Group 7", "Group 8",	/* Wizard->Group->Group  WGROUP */
	"Encoder", "Ultrasound", "Code",	/* Wizard->Calibration->Type */
	"Velocity", "Wedge Delay", "Sensitivity", "TCG",	/* Wizard->Calibration->Mode */
	"API-DL", "AWS-D1.5",				/* Select Code */
	"Radius", "Depth", "Thickness",		/* Echo type */
	"Compressive", "Tensile",			/* Weld type */
	"Custom", "Angle Beam", "Contact", "Immersion",	/* Probe Type */
	"50 Ω","500 Ω",/*Damping*/
	//"None","I", "V", "V Offset", "Double V", "Double V Offset", "J", "U",	/* Weld */
	"I","V","Double V", "U",
	"Symmetric","Straight  Right","Straight  Left",
	"English","Chinese",
	""
};

/*  0  */
//const gchar *type_en[] = {"Encoder", "Ultrasound", "Code"};
//const gchar *calibration_mode_en[] = {"Velocity", "Wedge Delay", "Sensitivity", "TCG"};

/*  3  */
const gchar *list_en[]		= {"1 Manual Weld 1: A%. DA^. PA^. SA^", "2 Auto. Weld 1: A%. DA^. ViA^. VsA^"};
const gchar *list1_en[]		= {"1", "2"};
const gchar *field1_en[]	= 
{
	"A%  Peak amplitude in gate A", "AdBA  Signal in gate minus gate level for gate A", 
	"AdBr  Diff. between signal and reference in gate A", "B%  Peak amplitude in gate B",
	"BdBB Signal in gate minus gate level for gate B", "BdBr  Diff. between signal and reference in gate B", 
	"A^  Position in gate A", "B^  Position in gate B", "I/  Position in gate I", "I(w)/  Position in gate I in water", 
	"T(A^)  Thickness", "ML  Percentage of material loss", "%(r)  Amplitude value for the reference cursor",
	"%(m)  Amplitude value of the measurement cursor", "%(m-r)  Amplitude (measurement cursor - reference cursor)", 
	"U(r)  Ultrasonic position of the reference cursor", "U(m)  Ultrasonic position of the measurement cursor", 
	"U(m-r)  Ultrasonic axis (measurement cursor - reference cursor)", "P(r)  Probe position of the reference cursor", 
	"P(m)  Probe position of the measurement cursor", "P(m-r)  Probe (measurement cursor - reference cursor)", 
	"S(r)  Scan position of the reference cursor", "S(m)  Scan position of the measurement cursor", 
	"S(m-r) Scan axis (measurement cursor - reference cursor)", "I(r)  Index position of the reference cursor",
	"I(m)  Index position of the measurement cursor", "I(m-r)  Index axis (measurement cursor - reference cursor)", 
	"RA^  Reference point to the indication in gate A", "RB^  Reference point to the indication in gate B",
	"PA^  Probe front face to the indication in gate A", "PB^  Probe front face to the indication in gate B", 
	"DA^  Depth of the indication in gate A", "DB^  Depth of the indication in gate B", 
	"SA^  Sound path of the indication in gate A", "SB^  Sound path of the indication in gate B",
	"ViA^  Volumetric position in gate A on the index axis", "ViB^  Volumetric position in gate B on the index axis",
	"VsA^  Volumetric position in gate A on the scan axis", "VsB^  Volumetric position in gate B on the scan axis",
	"LA^  Leg (skip) of the indication in gate A", "LB^  Leg (skip) of the indication in gate B", 
	"E%  Peak amplitude of the envelope in gate A", "E^  Position of envelop in gate A", "E-3dB  Envelop signal at -3 dB in gate A",
	"E-6dB  Envelop signal at -6 dB in gate A", "E-12dB  Envelop signal at -12 dB in gate A", 
	"E-20dB  Envelop signal at -20 dB in gate A", "API-DL  Defect length extension according to API code", 
	"API-HW  Half-wave width according to API code", "AWS-D A  Indication level for AWS-D1.5 A", 
	"AWS-D B  Zero reference level for AWS-D1.5 B", "AWS-D C  Attenuation factor for AWS-D1.5 C", 
	"AWS-D D  Indication rating for AWS-D1.5 D", "AWS-D 45  Discontinuity Severity Class for AWS-D1.5 45", 
	"AWS-D 60  Discontinuity Severity Class for AWS-D1.5 60", "AWS-D 70  Discontinuity Severity Class for AWS-D1.5 70", 
	"AWS-D CL  Discontinuity Severity Class for AWS-D1.5", "Scale  Scale factor", 
	"%(U(r))  Signal amplitude at reference ultrasonic cursor position", 
	"%(U(m))  Signal amplitude at measurement ultrasonic cursor position"		  /* 203 Field */
};
/*{"A%  Peak amplitude in gate A", "AdBA  Signal in gate minus gate level for gate A", "AdBr  Diff.between signal and reference in gate A","B%  Peak amplitude in gate B"};
 */
const gchar *field_en[]		=
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
const gchar *field_unit_en[]		=
{
	"%", "dB", "dB",  "%", "dB",  "dB", "us", "us", "us", "us", 
	"mm", "%",  "%",  "%", "%",   "us", "us", "us", "us", "us", 
	"us", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", 
	"mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", "mm", 
	"  ", "  ", "%",  "%",  "us", "mm", "mm", "mm", "mm", "dB", 
	"dB", "dB", "dB", "  ", "  ", "  ", "  ", "X",  "%",  "%" 
};
const gchar *field_unit_en_mm[]		=
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


const gchar *warning_info_en[] = {
	"max laws is 256!"
};

