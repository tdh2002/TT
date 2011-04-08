
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * file_op.h
 * 定义各种各种配置文件的路径 以及打开方法
 *
 * 谭登华
 * 2011-03-07
 *
 */

#ifndef __FILE_OP_H_
#define __FILE_OP_H_

#include "base.h"

extern void read_probe_file (const gchar *file_path, PROBE_P p);	/* 读取探头信息 */
extern void read_wedge_file (const gchar *file_path, WEDGE_P p);	/* 读取楔块信息 */
extern void read_palette_file (const gchar *file_path, guint *sp_col, guint *col);
extern void save_law_file (const gchar *file_path, gint offset, gint group);
extern void read_law_file (const gchar *file_path, gint offset, gint group);


#define PA_UNKNOWN_PROBE	"source/system/Model/Probe/unknown_pa.opp"
#define UT_UNKNOWN_PROBE	"source/system/Model/Probe/unknown_ut.oup"

#define PA_PROBE_PATH	"source/system/Model/Probe/PA/"
#define UT_PROBE_PATH	"source/system/Model/Probe/UT/"
#define PA_WEDGE_PATH	"source/system/Model/Wedge/PA/"
#define UT_WEDGE_PATH	"source/system/Model/Wedge/UT/"

#define SYS_LAW_PATH	"/home/tt/TT/source/system/Sample/Law/"
#define USER_LAW_PATH	"/home/tt/TT/source/user/law/"

/*#define PALETTE_PATH	"/home/gll/arraytop/TT/source/system/Sample/Palette/"*/
#define PALETTE_PATH	"/home/tt/TT/source/system/Sample/Palette/"

#define FILE_NAME_MAX 256

extern char SOURCE_FILE_NAME[FILE_NAME_MAX];
extern char SOURCE_FILE_PATH[FILE_NAME_MAX];
extern char TARGET_FILE_NAME[FILE_NAME_MAX];
extern char TARGET_FILE_PATH[FILE_NAME_MAX];


extern int Set_Source_File_Path(char *path);

extern char *Get_Source_File_Path();

extern int Set_Target_File_Path(char *path);

extern char *Get_Target_File_Path();

extern int Set_Source_File_Name(char *name);

extern char *Get_Source_File_Name();

extern int Set_Target_File_Name(char *name);

extern char *Get_Target_File_Name();

extern int Scan_Target_File(GtkWidget *widget,GtkTreeModel *model,char *file_name);

extern int Select_File();

extern int Unselect_File();

extern gboolean Select_All_File (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern int Unselect_All_File();

extern gboolean Copy_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern gboolean Move_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern gboolean Delect_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data);

extern int Rename_File(char *file_name);



#endif
