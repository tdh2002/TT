
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

#define PA_PROBE_PATH	"source/system/Model/Probe/PA/"
#define UT_PROBE_PATH	"source/system/Model/Probe/UT/"
#define PA_WEDGE_PATH	"source/system/Model/Wedge/PA/"
#define UT_WEDGE_PATH	"source/system/Model/Wedge/UT/"


/*#define PALETTE_PATH	"/home/gll/arraytop/TT/source/system/Sample/Palette/"*/
#define PALETTE_PATH	"/home/tt/TT/source/system/Sample/Palette/"


#endif
