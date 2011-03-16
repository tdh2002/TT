
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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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

int parseStory(xmlDocPtr doc, xmlNodePtr cur, int x, guint *sp_col, guint *col)
{
	xmlChar *key;
	xmlChar *Red, *Green, *Blue, *Fire;
	cur = cur->xmlChildrenNode;
	int  i = 0;
	while(cur != NULL)
	{
		if((!xmlStrcmp(cur->name, (const xmlChar*)"Version")))
		{
			key=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			g_print("%s=>%s\n", cur->name, key);
			xmlFree(key);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"SpecialColors")))
		{
			g_print("[%s]\n", cur->name);
			parseStory(doc, cur, 1, sp_col, col);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"MainColors")))
		{
			g_print("[%s]\n", cur->name);
			parseStory(doc, cur, 2, sp_col, col);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"Color")))
		{
			Red		= xmlGetProp(cur, (const xmlChar*)"R");
			Green	= xmlGetProp(cur, (const xmlChar*)"G");
			Blue	= xmlGetProp(cur, (const xmlChar*)"B");
			Fire	= xmlGetProp(cur, (const xmlChar*)"F");

			if (x == 2)
				col[i++] = (atoi((const char *)(Red))) << 16 |
					(atoi((const char *)(Green))) << 8 | (atoi((const char *)Blue));
			else if (x == 1)
				sp_col[i++] = (atoi((const char *)(Red))) << 16 |
					(atoi((const char *)(Green))) << 8 | (atoi((const char *)Blue));
			xmlFree(Red);
			xmlFree(Green);
			xmlFree(Blue);
		}
		cur = cur->next;
	}
	return 0;

}

/* 读取XML的调色板信息 */
void read_palette_file (const gchar *file_path, guint *sp_col, guint *col)
{
	/* 定义2个指针 doc指向整个dom；cur指向结点 以后遍历树就靠这个指针 */
	xmlDocPtr doc;
	xmlNodePtr cur;
	/*获取doc指针 也是把其他格式转成utf8的功能 */
	doc = xmlParseFile (file_path);
	if (doc == NULL)
		exit(1);
	/* 取得结点指针 */
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL)
	{
		xmlFreeDoc (doc);
		exit(1);
	}
	/* 取得根结点指针 这里一定要是根结点*/
	if (xmlStrcmp(cur->name, (const xmlChar *)"Palette"))
	{
		xmlFreeDoc (doc);
		exit(1);
	}
	sp_col[0] = 0x12345678;
	sp_col[1] = 0x12345678;
	sp_col[2] = 0x12345678;
	/*通过这个递归函数，遍历出所有感兴趣的结点。*/
	parseStory (doc, cur, 0, sp_col, col);

	xmlFreeDoc (doc);
	return ;
}

