

/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *		10		  20		30		  40		50		  60		70		  80
 */

#include "language.h"

const gchar *content_en10[] = {
	/* 一级菜单名称 */
	" Wizard  ",	" UT Settings",		" Gate/Alarm  ",	" Measurements  ",
	" Display  ",	" Probe/Part  ",	" Focal Law  ",		" Scan  ",
	" File  "	,	" Preferences  "
		/**
	"1 Wizard  ",	"2 UT Settings",	"3 Gate/Alarm  ",	"4 Measurements  ",
	"5 Display  ",	"6 Probe/Part  ",	"7 Focal Law  ",	"8 Scan  ",
	"9 File  "	,	"10 Preferences  "
	**/
};

static const gchar *content_en20[] = {
	/* 二级菜单第0页 */
	"Group", "Focal Law", "Calibration", NULL, NULL
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
	"Selection", "Overlay", "Zoom", "Color", "Properties"
};

static const gchar *content_en25[] = {
	/* 二级菜单第5页 */
	"Select", "Position", "Characterize", "Parts", NULL
};

static const gchar *content_en26[] = {
	/* 二级菜单第6页 */
	"Configuration", "Aperature", "Beam", "Laws", NULL
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
	"Pref.", "System", "Service", "Options", "Network"
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
	"Back", "Start", "Type", "Mode", NULL, NULL
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
	"Gain", "Start", "Range", "Wedge Delay" , "Velocity",  NULL, "Gain-r", "Gain(Ref.)"
};

static const gchar *content_en211[] = {
	/* 二级菜单第1页下的第1页三级菜单 */
	"Pulser" , "Tx/Rx mode", "Freq.(MHz)", "Voltage(V)", "PW(ns)" , "PRF"
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
	"Gate", "Parameters", "Start", "Width", "Threshold", NULL, "Synchro.", "Measure", "RF"
};

static const gchar *content_en221[] = {
	/* 二级菜单第2页下的第1页三级菜单 */
	"Alarm", "Group A", "Condition", "Operator", "Group B", "Condition"
};

static const gchar *content_en222[] = {
	/* 二级菜单第2页下的第2页三级菜单 */
	"Output", "Alarm #", "Count", "Sound", "Delay", "Hold Time"
};

static const gchar *content_en223[] = {
	/* 二级菜单第2页下的第3页三级菜单 */
	"Mode", "Curve", NULL, NULL, NULL, NULL
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
	"  Select  ","  Start  ", "  End  ","  Load  ", "Mode", NULL, "Contrast", "Brightness", "Min", "Max", "Mode"
};

static const gchar *content_en244[] = {
	/* 二级菜单第4页下的第4页三级菜单 */
	"  Scan  ","  Color  ", "  Envelope  ","  Source  ", "  Appearance  ", "  Overlay  ", "Compression", "Optimum","Ratio 1:1","Imterpolation","Color","Orientation"
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
	/*"  DHCP  ","  IP Address  ", "  Subnet Mask ","  Apply  ", "  Remote Pc  ", "  Connect  "*/
        "  IP Address  ", "  Subnet Mask ",NULL,NULL,NULL,NULL
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
const gchar *units_en[] = {"(mm)", "(in)", "(μs)", NULL, "(m/s)", "(in/μs)", "(dB)", "(1 to 97)", "(%)", "(ms)", "(mm/s)", "(1 to 113)", " "};


const gchar *all_menu_content_en[] = 
{
	"PC Pitch-and-Catch", "PE Pulse-Echo", "TT Through-Transmission", "",		/* 0 */
	"PC", "PE", "TT", "",														/* 4 */
	"1","1.5","2","2.25","3.5","4","5","7.5","10","12","15","20","UserDef","",	/* 8 */
	"50 Low", "100 Middle", "200 High",											/* 22 */
	"50", "100", "200",															/* 25 */
	"50 Low", "100 High",														/* 28 */
	"Auto",  "UserDef",															/* 30 */	
	"Auto Max", "Max/2", "Optimum","UserDef",									/* 32 */
	"None  0.5-21.0 MHz", "Auto", "1 MHz", "1.5MHz", "2MHz", 
	"2.25MHz", "4MHz", "5MHz", "7.5MHz", "10MHz", "12MHz", 
	"15MHz", "20MHz", "HR", "HR 10 MHz", "None",								/* 36 */
	"RF", "HW+", "HW-", "FW",													/* 42 */
	""																			/* 46 */

};

/*  0  */
const gchar *type_en[] = {"Encoder", "Ultrasound", "Code"};
const gchar *calibration_mode_en[] = {"Velocity", "Wedge Delay", "Sensitivity", "TCG"};

/*  1  */
const gchar *rectifier_en[] = {"RF", "HW+", "HW-", "FW", "FW"};
const gchar *averaging_en[] = {"1","2","4","8","16"};
const gchar *db_ref_en[] = {"Off", "On"};
const gchar *points_qty_en[] = {"Auto", "160", "320", "640", "UserDef"};
const gchar *sum_gain_en[] = {"Auto", "UserDef"};


/*  2  */
const gchar *gate_en[] = {"A ", "B ", "I "};
const gchar *parameters_en[] = {"Position", "Mode"};
const gchar *synchro_en[] = {"Pulse", "I/"};
const gchar *measure_en[] = {"Peak(^)", "Edge(/)"};
const gchar *RF_en[] = {"Absolute", "Positive","Negative"};


const gchar *gate_alarm_en[] = {"1", "2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
const gchar *groupA_en[] = {"1","2","3"};
const gchar *conditionA_en[] = {"None","Gate A","Gate B","Gate I","Not Gate A","Not Gate B","Not Gate I",">Max.Thickness","<Min.Thickness"};
const gchar *operator_en[] = {"AND","OR"};
const gchar *groupB_en[] = {"1","2","3"};
const gchar *conditionB_en[] = {"None","Gate A","Gate B","Gate I","Not Gate A","Not Gate B","Not Gate I",">Max.Thickness","<Min.Thickness"};
const gchar *output_en[] = {"Output 1", "Output 2","Output 3", "Analog 1", "Analog 2"};
const gchar *output_alarm_en[] = {"None", "All", "1", "2", "3", "4", "5", "6", "7", "8", "9","10","11","12","13","14","15","16"};
const gchar *sound_en[] = {"Off  No Sound", "300Hz  Audio output at 300 Hz","600Hz  Audio output at 600 Hz","1000Hz  Audio output at 1000 Hz","5000Hz  Audio output at 5000 Hz"};
const gchar *sound1_en[] = {"Off", "300Hz","600Hz","1000Hz","5000Hz"};
const gchar *curves_mode_en[] = {"Setup", "Edit"};
const gchar *curve_en[] = {"None","DAC","Linear DAC","TCG"};


/*  3  */
const gchar *list_en[] = {"1 Manual Weld 1: A%. DA^. PA^. SA^", "2 Auto. Weld 1: A%. DA^. ViA^. VsA^"};
const gchar *list1_en[] = {"1", "2"};
const gchar *field1_en[] = {"A%  Peak amplitude in gate A", "AdBA  Signal in gate minus gate level for gate A", "AdBr  Diff.between signal and reference in gate A","B%  Peak amplitude in gate B"};
const gchar *field_en[] = {"A%", "AdBA", "AdBr","B%"};
const gchar *selection_en[] = {"A-scan", "B-scan", "C-scan", "Strip Chart", "S-scan", "R and M", "Reference", "Measure", "Data"};
const gchar *source_en[] = {"A^", "A^-I^", "A^-I/", "B^", "B^-I^", "B^-I/", "B^-A^", "I^", "I/"};


/*  4  */
const gchar *display_en[] = {"A  A-Scan", "B  B-Scan", "C  C-Scan", "S  S-Scan", "A-B  A-Scan B-Scan", "A-B-C  A-Scan B-Scan C-Scan", "A-B-S  A-Scan B-Scan S-Scan", "A-C-[C] A-Scan C-Scan [C-Scan]", "A-S-[C]  A-Scan S-Scan [C-Scan]", "PA-TOFD  PA-TOFD", "Strip Chart-[A]  Strip Chart[A-Scan]"};
const gchar *displ_en[] = {"A", "B", "C", "S", "A-B", "A-B-C", "A-B-S", "A-C-[C]", "A-S-[C]", "PA-TOFD", "Strip Chart-[A]"};
const gchar *group_en[] = {"All", "Current"};
const gchar *utunit_en[] = {"True Depth","Sound Path", "Time"};
const gchar *grid_en[] = {"Blue", "Green", "Red", "Yellow", "Black", "Off"};
const gchar *zoom_display_en[] = {"A-Scan", "B-Scan", "S-Scan", "Strip Chart", "Uncorrected C-Scan", "Off"};
const gchar *select_en[] = {"Amplitude", "Rectification-TOFD", "Depth"};
const gchar *color_mode_en[] = {"Exclusion", "Compression"};
const gchar *scan_en[] = {"A-Scan", "B-Scan", "C-Scan", "S-Scan", "FFT", "Strip Chart"};
const gchar *color_en[] = {"Blue", "Green", "Yellow", "Black", "White", "Red"};
const gchar *envelope_en[] = {"None", "Infinite"};
const gchar *properties_source_en[] = {"Normal", "Highest(%)", "Thinnest", "All Laws"};
const gchar *appearance_en[] = {"Hollow", "Fill", "Hollow 2 Colors", "Fill 2 Colors"};
const gchar *overlay_en[] = {"None", "Peak", "Reference"};
const gchar *fft_color_en[] = {"Yellow", "Blue", "White","Black"};
const gchar *orientation_en[] = {"Vertical", "Horizontal"};


/*  5  */
const gchar *group_mode_en[] = {"UT  Conventional Ultrasound", "PA  Phassed Array"};
const gchar *group_mode1_en[] = {"UT", "PA"};
const gchar *probe_select_en[] = {"Select Tx/Rx", "Defines"};
const gchar *probe_skew_en[] = {"0.0", "90.0", "180.0", "270.0", "UserDef"};
const gchar *geometry_en[] = {"Plate", "OD", "ID"};
const gchar *material_en[] = {"AL OXIDE", "ALUMINUM", "BERYLIUM", "BRASS", "CADMIUM", "COPPER", "STEEL.MILD"};


/*  6  */
const gchar *law_config_en[] = {"Sectorial  Angular sector", "Linear  Linear sector", "Linear at 0  Linear sector with overlap", "Custom  Law file"};
const gchar *law_config1_en[] = {"Sectorial", "Linear", "Linear at 0", "Custom"};
const gchar *wave_type_en[] = {"LW  5890.0m/s", "SW  3240.0m/s"};
const gchar *wave_type1_en[] = {"LW", "SW"};



/*  7  */
const gchar *inspection_type_en[] = {"One-Line Scan", "Raster Scan", "Helicoidal Scan"};
const gchar *inspection_scan_en[] = {"Time", "Encoder 1", "Encoder 2"};
const gchar *start_mode_en[] = {"Reset All", "Reset Encoder", "Reset Data"};
const gchar *storage_en[] = {"Last", "A%", "Max.Thickness", "Min.Thickness"};
const gchar *inspec_data_en[] = {"All A & C scans", "All C scans"};


/*  8  */
const gchar *file_storage_en[] = {"Storage Card   \"\\Storage Card\"", "Internal Memory  \"\\User\"" };
const gchar *file_storage1_en[] = {"Storage Card", "Internal Memory" };
const gchar *save_mode_en[] = {"Inspection Data", "Indication Table", "Screen", "Report"};
const gchar *template_en[] = {"Complete  (Standard)"};
const gchar *paper_size_en[] = {"Letter", "Legal", "A4", "B4"};
const gchar *view_en[] = {"Table", "Current Layout", "Off"};
const gchar *file_select_en[] = {"1", "2","3", "4", "5", "6", "7", "8", "9", "10"};



/*  9  */
const gchar *pref_units_en[] = {"Millimeters", "Inches"};
const gchar *scheme_en[] = {"Indoor", "Outdoor"};
const gchar *gate_mode_en[] = {"By Group", "By Context"};
const gchar *select_key_en[] = {"F2", "F3", "F4", "F5", "F6", "DIN1", "DIN2", "DIN3", "DIN4"};
const gchar *assign_key_en[] = {"Gain", "Range", "Start", "Start/Stop", "Pause/Resume", "Calibrate", "Report", "Save Setup As", "Zoom", "Add Entry", "Display", "Context Select", "Gate", "User Field", "Buzzer", "Notes"};
const gchar *startup_mode_en[] = {"Automatic", "Manual"};
const gchar *mouse_en[] = {"Off", "Mouse", "Control Area"};




