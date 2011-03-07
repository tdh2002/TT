
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 * file_op.h
 * 定义各种各种配置文件的路径 以及打开方法
 *
 *
 * 探头 楔块 报告 配置文件 数据
 *
 * 谭登华
 * 2011-03-07
 *
 */

#include "file_op.h"
#include "drawui.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>

/* 探头 PA opp UT oup  */
void read_probe_file (const gchar *file_path, PROBE_P p)
{
	int fd;
	int	i; 
	if ((fd = open(file_path, O_RDONLY ))<0) 
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		if (GROUP_VAL(group_mode) == PA_SCAN)
		{
			lseek (fd, 4, SEEK_SET);
			i = read (fd, p, sizeof(PROBE) - 4);
		}
		else if (GROUP_VAL(group_mode) == UT_SCAN)
		{
			i = read (fd, p, sizeof(PROBE) );
			p->Frequency = p->Elem_qty | (p->Freq2 << 8);
		}
		close (fd);
	}
}

/* 楔块 PA opw UT ouw */
void read_wedge_file (const gchar *file_path, WEDGE_P p)
{
	int fd;
	int	i; 
	if ((fd = open(file_path, O_RDONLY ))<0) 
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		i = read (fd, p, 52);
		lseek (fd, 1, SEEK_CUR);
		i = read (fd, (void *)((int)(p) + 52), 64);
	}
}

