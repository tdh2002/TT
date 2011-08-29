
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
#include <assert.h>
#include <string.h>
#include <glib/gprintf.h>

#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>

char SOURCE_FILE_NAME[FILE_NAME_MAX];
char SOURCE_FILE_PATH[FILE_NAME_MAX];
char TARGET_FILE_NAME[FILE_NAME_MAX];
char TARGET_FILE_PATH[FILE_NAME_MAX];

void screen_to_file();

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

/* 探头 PA opp UT oup  */
void save_probe_file (const gchar *file_path, PROBE_P p)
{
	int fd;
	int	i; 
    char tmp[] = "    ";
	if ((fd = open(file_path,  O_CREAT| O_RDWR,0666))<0) 
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		if (GROUP_VAL(group_mode) == PA_SCAN)
		{
			//lseek (fd, 4, SEEK_SET);
			i = write (fd,tmp,strlen(tmp));
			i = write (fd, p, sizeof(PROBE) - 4);
		}
		else if (GROUP_VAL(group_mode) == UT_SCAN)
		{
			//p->Frequency = p->Elem_qty | (p->Freq2 << 8);
			p->Freq2 = (p->Frequency & 0xff00) >> 8;
			p->Elem_qty = p->Frequency & 0xff;
			i = write (fd, p, sizeof(PROBE) );
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
	
	close(fd);
}

/* 楔块 PA opw UT ouw */
void save_wedge_file (const gchar *file_path, WEDGE_P p)
{
	int fd;
	int	i; 
	char tmp[] = " ";

	if ((fd = open(file_path, O_CREAT| O_RDWR,0666))<0) 
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		i = write (fd, p, 52);
		//lseek (fd, 1, SEEK_CUR);
		i = write (fd,tmp,strlen(tmp));
		i = write (fd, (void *)((int)(p) + 52), 64);
	}

	close(fd);
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
			xmlFree(key);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"SpecialColors")))
		{
			parseStory(doc, cur, 1, sp_col, col);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar*)"MainColors")))
		{
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

/* 读取当前group的聚焦法则 */
void read_law_file (const gchar *file_path, gint offset, gint group)
{
	char	version[16];
	gint	temp[32];
	int		i, j, k, l;
	FILE	*fd;

	if((fd = fopen(file_path, "rb")) == NULL)
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		l = fscanf(fd, "%s 	%d", version, &TMP(beam_qty[group]));
		k = TMP(beam_qty[group]);
		for (i = 0; i < k; i++)
		{
			l =	fscanf(fd, "%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d",
					&temp[0], &temp[1], &temp[2], &temp[3],
					&temp[4], &temp[5], &temp[6], &temp[7],
					&temp[8], &temp[9], &temp[10],&temp[11],
					&temp[12],&temp[13],&temp[14]);

			TMP(focal_law_all_beam[offset + i]).N_ActiveElements	= temp[0];
			TMP(focal_law_all_beam[offset + i]).frequency			= temp[1];
			TMP(focal_law_all_beam[offset + i]).cycle				= temp[2];
			TMP(focal_law_all_beam[offset + i]).sumgain				= temp[3];
			TMP(focal_law_all_beam[offset + i]).mode				= temp[4];
			TMP(focal_law_all_beam[offset + i]).filter				= temp[5];
			TMP(focal_law_all_beam[offset + i]).R_angle				= temp[6];	
			TMP(focal_law_all_beam[offset + i]).S_angle				= temp[7]; 
			TMP(focal_law_all_beam[offset + i]).T_first				= temp[8];
			TMP(focal_law_all_beam[offset + i]).R_first				= temp[9];		
			TMP(focal_law_all_beam[offset + i]).Scan_offset			= temp[10]; 
			TMP(focal_law_all_beam[offset + i]).Index_offset		= temp[11] ;
			TMP(focal_law_all_beam[offset + i]).G_delay				= temp[12];
			TMP(focal_law_all_beam[offset + i]).F_depth				= temp[13];
			TMP(focal_law_all_beam[offset + i]).M_velocity			= temp[14];

			for (j = 0; j < TMP(focal_law_all_beam[offset + i]).N_ActiveElements; j++)
			{
				l = fscanf(fd, "%d 	%d 	%d 	%d 	%d 	%d", &temp[0],&temp[1],
						&temp[2], &temp[3], &temp[4], &temp[5]);
				TMP(focal_law_all_elem[offset + i][j]).E_number		= temp[0];
				TMP(focal_law_all_elem[offset + i][j]).FL_gain		= temp[1];
				TMP(focal_law_all_elem[offset + i][j]).T_delay		= temp[2];
				TMP(focal_law_all_elem[offset + i][j]).R_delay		= temp[3];
				TMP(focal_law_all_elem[offset + i][j]).Amplitude	= temp[4];
				TMP(focal_law_all_elem[offset + i][j]).P_width		= temp[5];
			} 
		} 
	}
	fclose (fd);
}

/* 保存当前group的聚焦法则 */
void save_law_file (const gchar *file_path, gint offset, gint group)
{
	int		i, j;
	FILE	*fd;

	if((fd = fopen(file_path, "wb+")) == NULL)
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		fprintf(fd, "%s 	%d\015\012", "dp1.00", TMP(beam_qty[group]));
		for (i = 0; i < TMP(beam_qty[group]); i++)
		{
			fprintf(fd, "%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d 	%d\015\012",
					TMP(focal_law_all_beam[offset + i]).N_ActiveElements,
					TMP(focal_law_all_beam[offset + i]).frequency, 
					TMP(focal_law_all_beam[offset + i]).cycle, 
					TMP(focal_law_all_beam[offset + i]).sumgain, 
					TMP(focal_law_all_beam[offset + i]).mode,
					TMP(focal_law_all_beam[offset + i]).filter,
					TMP(focal_law_all_beam[offset + i]).R_angle,
					TMP(focal_law_all_beam[offset + i]).S_angle, 
					TMP(focal_law_all_beam[offset + i]).T_first, 
					TMP(focal_law_all_beam[offset + i]).R_first,
					TMP(focal_law_all_beam[offset + i]).Scan_offset, 
					TMP(focal_law_all_beam[offset + i]).Index_offset, 
					TMP(focal_law_all_beam[offset + i]).G_delay, 
					TMP(focal_law_all_beam[offset + i]).F_depth, 
					TMP(focal_law_all_beam[offset + i]).M_velocity );


			for (j = 0; j < TMP(focal_law_all_beam[offset + i]).N_ActiveElements; j++)
			{
					fprintf(fd, "%d 	%d 	%d 	%d 	%d 	%d\015\012",          
							TMP(focal_law_all_elem[offset + i][j]).E_number,
							TMP(focal_law_all_elem[offset + i][j]).FL_gain,  
							TMP(focal_law_all_elem[offset + i][j]).T_delay,
							TMP(focal_law_all_elem[offset + i][j]).R_delay,
							TMP(focal_law_all_elem[offset + i][j]).Amplitude, 
							TMP(focal_law_all_elem[offset + i][j]).P_width ); 
			} 

		} 
	}
	fclose (fd);
}

int Set_Source_File_Path(char *path)
{
    assert(path != NULL);
    strcpy(SOURCE_FILE_PATH,path);
    return 0;
}

char *Get_Source_File_Path()
{
    return (char *)&SOURCE_FILE_PATH[0];
}

int Set_Target_File_Path(char *path)
{
    assert(path != NULL);
    strcpy(TARGET_FILE_PATH,path);
    return 0;
}

char *Get_Target_File_Path()
{
    return (char *)&TARGET_FILE_PATH[0];
}

int Set_Source_File_Name(char *name)
{
    assert(name != NULL);
    strcpy(SOURCE_FILE_NAME,name);
    return 0;
}

char *Get_Source_File_Name()
{
    return (char *)&SOURCE_FILE_NAME[0];
} 

int Set_Target_File_Name(char *name)
{
    assert(name != NULL);
    strcpy(TARGET_FILE_NAME,name);
    return 0;
}

char *Get_Target_File_Name()
{
    return (char *)&TARGET_FILE_NAME[0];
}

int Scan_Target_File(GtkWidget *widget,GtkTreeModel *model,char *file_name)
{    
    int i;
    int value;
    char *value_name;
    GtkWidget *target_list = widget;
    GtkTreeIter target_iter;
    GtkTreeModel *target_model = model;

    value = gtk_tree_model_get_iter_from_string (target_model, &target_iter, "0");

    i = 0;

    while(value)
    {
        gtk_tree_model_get(target_model, &target_iter, 0, &value_name,  -1);
     
        if (strcmp(value_name,file_name) == 0)
            return -1;
         
        value = gtk_tree_model_iter_next(target_model,&target_iter);
         
        i++; 
    }

    add_to_list(target_list,file_name,i);    

    return 0;
}

int Select_File()
{
    
    return 0;
}

int Unselect_File()
{
    return 0;
}

gboolean Select_All_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
  GtkWidget *list = (GtkWidget *)data;
    
  GtkTreeSelection *selection;
    
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

  gtk_tree_selection_select_all(selection);  

    return 0;
}

int Unselect_All_File(GtkWidget *list)
{
    
  GtkTreeSelection *selection;
   
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

  gtk_tree_selection_unselect_all(selection);
  
   return 0;
}


gboolean Copy_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{

    char *target_file;
    char *source_file;
    char command[1024] = "cp -r ";
    int value; 
    char *value_name;

    MY_SIGNAL *my_signal = (MY_SIGNAL *)data;

    GtkTreeIter source_iter;
  
    GtkTreeModel *source_model;
    GtkTreeModel *target_model;
    GtkTreeSelection *source_selection; 

    GtkWidget *target_list;
    
    target_list = my_signal->target_list;

    source_model = my_signal->source_model;

    source_selection = my_signal->source_selection;

    target_model = my_signal->target_model;

    g_print("entry copy_file function\n");

    value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");

    while(value)
    {
        if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
        {
            gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);

            g_printf("value_name = %s\n",value_name);

            memset(command,0,sizeof(command));
            
            strcpy(command,"cp -r ");

            target_file = Get_Target_File_Path();

            source_file = Get_Source_File_Path();    

            strcat(command,source_file);

			if ( !( ( strlen(source_file) == 1 ) && ( strcmp(source_file,"/") == 0) ) )
			{
				if( strcmp(source_file + strlen(source_file) - strlen("/"), "/") != 0 )
				{
					strcat(command,"/");
				}
			}

            strcat(command,value_name);

            strcat(command," ");

            strcat(command,target_file);

            value = system(command); 
              
            Scan_Target_File(target_list,target_model,value_name);
			value = gtk_tree_model_iter_next(source_model,&source_iter); 
 
            g_free(value_name);

            g_printf("selection \n");
        }

        value = gtk_tree_model_iter_next(source_model,&source_iter); 

    }

    return 0;

}

gboolean Move_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
    char *target_file;
    char *source_file;
    char command[1024] = "mv ";
    int value; 
    char *value_name;

    MY_SIGNAL *my_signal = (MY_SIGNAL *)data;

    GtkTreeIter source_iter;
 
    GtkTreeModel *source_model;
    GtkTreeModel *target_model;
    GtkTreeSelection *source_selection; 
    
    GtkListStore *source_list_store;
    GtkWidget *source_list;
    GtkWidget *target_list;

    source_model = my_signal->source_model;

    target_model = my_signal->target_model;

    source_selection = my_signal->source_selection;

    source_list = my_signal->source_list;

    target_list = my_signal->target_list;    

    source_list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(source_list)));

    value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");

    while(value)   
    {
        if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
        {

            gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);

            g_printf("value_name = %s\n",value_name);

            memset(command,0,sizeof(command));
              
            strcpy(command,"mv ");

            target_file = Get_Target_File_Path();

            source_file = Get_Source_File_Path();    

            strcat(command,source_file);

			if ( !( ( strlen(source_file) == 1 ) && ( strcmp(source_file,"/") == 0) ) )
			{
				if( strcmp(source_file + strlen(source_file) - strlen("/"), "/") != 0 )
				{
					strcat(command,"/");
				}
			}

			strcat(command,value_name);

            strcat(command," ");

            strcat(command,target_file);

            value = system(command); 

            Scan_Target_File(target_list,target_model,value_name);
            
            g_free(value_name);

            g_printf("selection \n");

            gtk_list_store_remove(source_list_store,&source_iter);

            value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");
        }
        else
        {
        value = gtk_tree_model_iter_next(source_model,&source_iter);
        } 

    }
    
    //g_object_unref(source_list_store); 
  
    return 0;
}

gboolean Delect_File(GtkWidget *widget,	GdkEventButton *event,	gpointer       data)
{
    char *source_file;
    char command[1024] = "rm -r ";
    int value; 
    char *value_name;

    MY_SIGNAL *my_signal = (MY_SIGNAL *)data;

    GtkTreeIter source_iter;
  
    GtkTreeModel *source_model;
    GtkTreeSelection *source_selection; 

    GtkListStore *source_list_store;
    GtkWidget *source_list;

    source_model = my_signal->source_model;

    source_selection = my_signal->source_selection;

    source_list = my_signal->source_list;
    
    source_list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(source_list)));

    value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");

    while(value)   
    {
        if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
        {

            gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);

            g_printf("value_name = %s\n",value_name);

            memset(command,0,sizeof(command));
              
            strcpy(command,"rm -r ");

            source_file = Get_Source_File_Path();    

            strcat(command,source_file);

			if ( !( ( strlen(source_file) == 1 ) && ( strcmp(source_file,"/") == 0) ) )
			{
				if( strcmp(source_file + strlen(source_file) - strlen("/"), "/") != 0 )
				{
					strcat(command,"/");
				}
			}

            strcat(command,value_name);

            value = system(command); 
      
            g_free(value_name);

            g_printf("selection \n");

            gtk_list_store_remove(source_list_store,&source_iter);

            value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");
        }
        else
        {
            value = gtk_tree_model_iter_next(source_model,&source_iter);
        } 

    }
    
    //g_object_unref(list_store); 
  
    return 0;
}

int Rename_File(char *file_name)
{
    return 0;
}

void report_build_start(char *file_name)
{
    FILE *fp = NULL;  

    fp = fopen(file_name,"w+");

    if(fp == NULL)
    {
        return ;
    }

    fprintf(fp,"<html>\n");
    fprintf(fp,"<head>\n");
    fprintf(fp,"<style>\n");
    fprintf(fp,"table { font-size: 9; font:\"tahoma\"; }\n");
    fprintf(fp,"body { font-size: 9; font:\"tahoma\"; }\n");
    fprintf(fp,"h3 { page-break-before: always }\n");
    fprintf(fp,"\n");
    fprintf(fp,".general_cell { width : 110; valign:\"bottom\"; }\n");
    fprintf(fp,"</style>\n");
    fprintf(fp,"</head>\n");
    fprintf(fp,"<body>\n");
    fprintf(fp,"<br>\n");

    fclose(fp);
}

void report_build_header(char *file_name)
{
    FILE *fp = NULL;

    fp = fopen(file_name,"r+");

    if(fp == NULL)
    {
        return ;
    }

    fseek(fp,0,SEEK_END);

    fprintf(fp,"<table width=\"690\" border=\"1\">\n");
    fprintf(fp,"<tr><td>\n");
    fprintf(fp,"<table>\n");
    
    fprintf(fp,"<tr><td class = \"general_cell\"><b>Report Date</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Report Version</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Setup File Name</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Inspection Date</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Inspection Version</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Save Mode</b></td>\n");
    fprintf(fp,"</tr>\n\n");
    
    fprintf(fp,"<tr><td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr><td class = \"general_cell\"><b>Doppler Type</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Doppler Serial</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Module Type</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Module Serial #</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Cailbration Due</b></td>\n");
    fprintf(fp,"    <td class = \"general_cell\"><b>Data File Name</b></td>\n");
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"<tr><td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"    <td class  = \"general_cell\">N/A</td>\n");
    fprintf(fp,"</tr>\n\n");

    fprintf(fp,"</table>\n");
    fprintf(fp,"</td></tr>\n");
    fprintf(fp,"</table>\n\n");

    fprintf(fp,"<br>\n");

    fclose(fp);
}

void report_build_user_field(char *file_name)
{
    FILE *fp = NULL;

	int i;

    fp = fopen(file_name,"r+");

    if(fp == NULL)
    {
        return ;
    }

    fseek(fp,0,SEEK_END);

    fprintf(fp,"<table width=\"690\" border=\"1\">\n");
    fprintf(fp,"<tr><td>\n");

    fprintf(fp,"<table width = \"100%%\" >\n");

	for (i=0;i<10;i++)
	{
		if (get_report_userfield_enable(pp->p_config,i))
		{
			fprintf(fp,"<tr><td width =\"50%%\"><table>\n");
			fprintf(fp,"<tr><td class = \"general_cell\"><b>%s</b></td>",
				get_report_userfield_label(pp->p_config,i));
			fprintf(fp,"<td>%s</td></tr>\n",
				get_report_userfield_content(pp->p_config,i));
			fprintf(fp,"</table></td></tr>\n\n");
		}
	}

    fprintf(fp,"</table>\n");
    fprintf(fp,"</tr></td>\n");
    fprintf(fp,"</table>\n\n");

    fclose(fp);
}

void report_build_group_start(char *file_name,int group)
{

    FILE *fp = NULL;

    fp = fopen(file_name,"r+");

    if (fp == NULL)
    {
        return ;
    }

    fseek(fp,0,SEEK_END);

    fprintf(fp,"<H3></H3>\n");
    fprintf(fp,"<TABLE WIDTH=\"690\" BORDER=\"0\">\n");
    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD><HR></TD>\n");
    fprintf(fp,"</TR>\n");
    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD><B>Group %d</B></TD>\n",group + 1);
    fprintf(fp,"</TR>\n");
    fprintf(fp,"</TABLE>\n\n");

    fclose(fp);
}

void report_build_probe(char *file_name,int group)
{
    FILE *fp = NULL;

    fp = fopen(file_name,"r+");

    if ( fp == NULL )
    {
        return ;
    }

    fseek(fp,0,SEEK_END);

    fprintf(fp,"<BR>\n");
    fprintf(fp,"Probe Characterization\n");
    fprintf(fp,"<TABLE WIDTH=\"690\" BORDER=\"1\">\n");
    fprintf(fp,"<TR><TD>\n");
    fprintf(fp,"<TABLE>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Probe Model</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Probe Serial #</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B></B></TD>\n");
    fprintf(fp,"  	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n"); 
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Probe Frequency</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Peak Frequency</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Wedge Model</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Wedge Angle</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Probe Aperture</B></TD>\n");
    fprintf(fp,"  	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">0.0º</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"  	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Lower Frequency</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Higher Frequency</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Center Frequency</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Bandwidth (MHz)</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Bandwidth (%%)</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>-6 dB</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>-20 dB</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"  	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Date</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Time</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Procedure</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Calibration Block</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Characterization Gain</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>A%%</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</T   D>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"</TABLE>\n");
    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n\n\n\n");

    fclose(fp);
}

void report_build_setup(char *file_name,int group)
{
    FILE *fp = NULL;
	gint grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
    
    fp = fopen(file_name,"r+");

    if (fp == NULL)
    {
         return ;
    }
    
    fseek(fp,0,SEEK_END);

    fprintf(fp,"<BR>\n");
    fprintf(fp,"Setup\n");
    fprintf(fp,"<TABLE WIDTH=\"690\" BORDER=\"1\">\n");
    fprintf(fp,"<TR><TD>\n");
    fprintf(fp,"<B></B>\n");

    fprintf(fp,"<TABLE>\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Beam Delay</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Start (Half Path)</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Range (Half Path)</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>PRF</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Type</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Averaging Factor</B></TD>\n");
    fprintf(fp,"</TR>\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n",BEAM_INFO(group,beam_delay));
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n", get_group_val (get_group_by_id (pp->p_config, group), GROUP_START));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n", get_group_val (get_group_by_id (pp->p_config, group), GROUP_RANGE));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",
			get_group_val (p_grp, GROUP_PRF_VAL));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[ GROUP_MODE_P +(GROUP_VAL_POS(group,group_mode))]);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d</TD>\n",1 << get_group_val (get_group_by_id (pp->p_config, group), GROUP_AVERAGING));
    fprintf(fp,"</TR>\n\n");

	fprintf(fp,"<TR>\n");
	fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Scale Type</B></TD>\n");
	fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Scale Factor</B></TD>\n");
	fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Video Filter</B></TD>\n");
	fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Pretrig.</B></TD>\n");
	fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Rectification</B></TD>\n");
	fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Band-Pass Filter</B></TD>\n");
	fprintf(fp,"</TR>\n\n");

	fprintf(fp,"<TR>\n");
	fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
	fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%f</TD>\n",(get_group_val (get_group_by_id (pp->p_config, group), GROUP_RANGE) / 10.0) / GROUP_VAL_POS(group,point_qty));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",
			menu_content[ OFF_ON + get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER)]);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");//Pretrig    
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[ RECTIFIER + (get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER))]);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",
			menu_content[ FILTER + get_group_val (get_group_by_id (pp->p_config, group), GROUP_FILTER)]);
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Voltage</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Gain</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Mode</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Wave Type</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Sound Velocity</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Pulse Width</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%.1f dB</TD>\n", get_group_val (get_group_by_id (pp->p_config, group), GROUP_GAIN) / 100.0);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%s</TD>\n",
			menu_content[ TX_RX_MODE + get_group_val (p_grp, GROUP_TX_RX_MODE)]);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">N/A</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d 0.01m/s</TD>\n", get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY));
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%f ns</TD>\n",
			get_group_val (p_grp, GROUP_PW_VAL)	/ 100.0);
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Scan Offset</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Index Offset</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Skew</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B></B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d</TD>\n",GROUP_VAL_POS(group,scan_offset));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",GROUP_VAL_POS(group,index_offset));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",GROUP_VAL_POS(group,skew));
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n");

    fprintf(fp,"</TABLE>\n\n");

    fprintf(fp,"<HR>\n\n");

    fprintf(fp,"<TABLE>\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Gate</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Start</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Width</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Threshold</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Synchro</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">I</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n",pp->p_config->group[group].gate[2].start);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n",pp->p_config->group[group].gate[2].width);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",pp->p_config->group[group].gate[2].height);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[SYNCHRO + pp->p_config->group[group].gate[2].synchro]);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">A</TD>");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n",pp->p_config->group[group].gate[0].start);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n",pp->p_config->group[group].gate[0].width);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",pp->p_config->group[group].gate[0].height);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[SYNCHRO + pp->p_config->group[group].gate[0].synchro]);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">B</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n",pp->p_config->group[group].gate[1].start);
	fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d ns</TD>\n",pp->p_config->group[group].gate[1].width);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",pp->p_config->group[group].gate[1].height);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[SYNCHRO + pp->p_config->group[group].gate[1].synchro]);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"</TABLE>\n");

    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n\n");

    fclose(fp);
}

void report_build_calculator(char *file_name,int group)
{
    FILE *fp = NULL;
    
    fp = fopen(file_name,"r+");

    if (fp == NULL)
    {
        return ;
    }

    fseek(fp,0,SEEK_END);

    fprintf(fp,"<BR>\n");
    fprintf(fp,"Calculator\n");
    fprintf(fp,"<TABLE WIDTH=\"690\" BORDER=\"1\">\n");
    fprintf(fp,"<TR><TD>\n");
    
    fprintf(fp,"<TABLE>\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\"><B>Used Element Qty.</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>First TX Element</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Last TX Element</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>First RX Element</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Last RX Element</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Resolution</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,Elem_qty));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,First_tx_elem));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,Last_tx_elem));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,First_rx_elem));
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,Last_rx_elem));
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,Elem_step));
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"</TABLE>\n\n");

    fprintf(fp,"<TABLE>\n\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\"><B>Wave Type</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Start Angle</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Stop Angle</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Angle Resolution</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Focal Depth</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Law Configuration</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[WAVE_TYPE + LAW_VAL_POS(group,Wave_type)]);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,Angle_min));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,Angle_max));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%f</TD>\n",LAW_VAL_POS(group,Angle_step) / 100.0);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%d</TD>\n",LAW_VAL_POS(group,Focal_type));
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"</TABLE>\n\n");

    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n\n\n");

    fclose(fp);
}

void report_build_part(char *file_name,int group)
{
    FILE *fp = NULL;

    fp = fopen(file_name,"r+");

    if (fp == NULL)
    {
        return ;
    }

    fseek(fp,0,SEEK_END);

    fprintf(fp,"<BR>\n");
    fprintf(fp,"Part\n");
    fprintf(fp,"<TABLE WIDTH=\"690\" BORDER=\"1\">\n");
    fprintf(fp,"<TR><TD>\n");

    fprintf(fp,"<TABLE>\n\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\"><B>Material</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Geometry</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Thickness</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B></B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"</TR>\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[MATERIAL + get_part_material(pp->p_config)]);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[GEOMETRY + get_part_geometry(pp->p_config)]);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%.2f</TD>\n",get_part_thickness(pp->p_config) / 1000.0);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n");
    fprintf(fp,"</TABLE>\n");

    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n");
    
    fclose(fp);
}

void report_build_scan_area(char *file_name,int group)
{
    FILE *fp = NULL;

    fp = fopen(file_name,"r+");

    if (fp == NULL)
    {
        return ;
    }
	
	fseek(fp,0,SEEK_END);

	fprintf(fp,"<BR>\n");
    fprintf(fp,"Scan Area\n");
    
    fprintf(fp,"<TABLE WIDTH=\"690\" BORDER=\"1\">\n");
    fprintf(fp,"<TR><TD>\n");
	fprintf(fp,"<TABLE>\n");
	fprintf(fp,"<TR><TD>\n");
    fprintf(fp,"<TABLE>\n\n");
    
    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Scan Start</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Scan Length</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Scan Resolution</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>  Index Start</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Index Length</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Index Resolution</B></TD>\n");
    fprintf(fp,"</TR>\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%.2f</TD>\n", get_area_scanstart (pp->p_config) / 1000.0);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%.2f</TD>\n", get_area_scanend (pp->p_config) / 1000.0);
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%.2f</TD>\n", get_area_scanresolution (pp->p_config) / 1000.0);

	//if ()
	fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">%.2f</TD>\n", get_area_indexstart (pp->p_config) / 1000.0);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%.2f</TD>\n", get_area_indexend (pp->p_config) / 1000.0);
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%.2f</TD>\n", get_area_indexresolution (pp->p_config) / 1000.0);
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Synchro</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Scan Speed</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"<TR>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%s</TD>\n",menu_content[I_SCAN+get_inspec_source (pp->p_config)]);
 
	if (UNIT_MM == get_unit(pp->p_config))
		fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%.3f</TD>\n",get_inspec_speed (pp->p_config)/1000.0);
	else
		fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">%.3f</TD>\n",get_inspec_speed (pp->p_config)/1000.0 * 0.03937);
    
	fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\">&nbsp;</TD>\n");
    fprintf(fp,"</TR>\n\n");

    fprintf(fp,"</TABLE>\n");
    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n\n");

    fprintf(fp,"<HR>\n");
    fprintf(fp,"<TABLE>\n");
    fprintf(fp,"<TR><TD>\n"); 
    fprintf(fp,"<TABLE>\n\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\"><B>Axis</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Encoder</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>Encoder Type</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Encoder Resolution</B></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"><B>Polarity</B></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD></TR>\n\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD></TR>\n\n");

    fprintf(fp,"<TR><TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"    <TD CLASS = \"GENERAL_CELL\"></TD>\n");
    fprintf(fp,"	<TD CLASS = \"GENERAL_CELL\"><B>&nbsp;</B></TD></TR>\n\n");

    fprintf(fp,"</TABLE>\n");
    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n");
    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n\n");

    fclose(fp);
}

void report_build_group_config(char *file_name,int group)
{
    report_build_group_start(file_name,group);

	if (get_report_format_probe(pp->p_config))
    	report_build_probe(file_name,group);

	if (get_report_format_setup(pp->p_config))
    	report_build_setup(file_name,group);

    report_build_calculator(file_name,group);

    report_build_part(file_name,group);

    report_build_scan_area(file_name,group);
}

void report_build_image(char *file_name)
{

	char *screen_filename = "fb.jpg";

	char *fbfilename = "/dev/fb1";

    assert(file_name != NULL);

	screen_to_file(screen_filename,fbfilename);
}

void report_build_note(FILE *fp)
{
	if (fp == NULL)
		return ;

	fseek(fp,0,SEEK_END);

	fprintf(fp,"<TR>\n");
	fprintf(fp,"    <TD>Notes</TD>\n");
	fprintf(fp,"</TR>\n");

	fprintf(fp,"<TR><TD COLSPAN = 2>%s</TD></TR>\n",get_edit_notes_info(pp->p_config));	
}

void report_build_signature(char *file_name)
{
	FILE *fp;

	assert(file_name != NULL);

	fp = fopen(file_name,"r+");

	if (fp == NULL)
		return ;

	fseek(fp,0,SEEK_END);

	fprintf(fp,"<TABLE WIDTH=\"690\" BORDER=\"1\">\n");
	fprintf(fp,"<TR><TD>\n");
	fprintf(fp,"<TABLE>\n");

	//判断一下用户有没有选择生成那个notef
	if ( get_report_format_note(pp->p_config) )
		report_build_note(fp);

    fprintf(fp,"</TABLE>\n");
    fprintf(fp,"</TD></TR>\n");
    fprintf(fp,"</TABLE>\n\n");
	
	fclose(fp);
}

void report_build_end(char *file_name)
{
    FILE *fp = NULL;
    
    fp = fopen(file_name,"r+");

    if (fp == NULL)
    {
        return ;
    }
    
    fseek(fp,0,SEEK_END);

    fprintf(fp,"</body>\n");
    fprintf(fp,"</html>\n");

    fclose(fp);
}

struct _FBInfo;
typedef struct _FBInfo FBInfo;
typedef int (*UnpackPixel)(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b);

struct _FBInfo
{
	int fd;
	UnpackPixel unpack;
	unsigned char *bits;
	struct fb_fix_screeninfo fi;
	struct fb_var_screeninfo vi;
};

#define fb_width(fb)  ((fb)->vi.xres)
#define fb_height(fb) ((fb)->vi.yres)
#define fb_bpp(fb)    ((fb)->vi.bits_per_pixel>>3)
#define fb_size(fb)   ((fb)->vi.xres * (fb)->vi.yres * fb_bpp(fb))

static int fb_unpack_rgb565(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	unsigned short color = *(unsigned short*)pixel;

	*r = ((color >> 11) & 0xff) << 3;
	*g = ((color >> 5) & 0xff)  << 2;
	*b = (color & 0xff )<< 3;

	return 0;
}

static int fb_unpack_rgb24(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	*r = pixel[fb->vi.red.offset>>3];
	*g = pixel[fb->vi.green.offset>>3];
	*b = pixel[fb->vi.blue.offset>>3];

	return 0;
}

static int fb_unpack_argb32(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	*r = pixel[fb->vi.red.offset>>3];
	*g = pixel[fb->vi.green.offset>>3];
	*b = pixel[fb->vi.blue.offset>>3];

	return 0;
}

static int fb_unpack_none(FBInfo* fb, unsigned char* pixel, 
	unsigned char* r, unsigned char* g, unsigned char* b)
{
	*r = *g = *b = 0;

	return 0;
}

static void set_pixel_unpacker(FBInfo* fb)
{
	if(fb_bpp(fb) == 2)
	{
		fb->unpack = fb_unpack_rgb565;
	}
	else if(fb_bpp(fb) == 3)
	{
		fb->unpack = fb_unpack_rgb24;
	}
	else if(fb_bpp(fb) == 4)
	{
		fb->unpack = fb_unpack_argb32;
	}
	else
	{
		fb->unpack = fb_unpack_none;
		printf("%s: not supported format.\n", __func__);
	}

	return;
}

static int fb_open(FBInfo* fb, const char* fbfilename)
{
	fb->fd = open(fbfilename, O_RDWR);

	if (fb->fd < 0)
	{
		fprintf(stderr, "can't open %s\n", fbfilename);

		return -1;
	}

	if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fi) < 0)
		goto fail;

	if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vi) < 0)
		goto fail;

	fb->bits = mmap(0, fb_size(fb), PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);

	if (fb->bits == MAP_FAILED)
		goto fail;

	printf("---------------framebuffer---------------\n");
	printf("%s: \n  width : %8d\n  height: %8d\n  bpp   : %8d\n  r(%2d, %2d)\n  g(%2d, %2d)\n  b(%2d, %2d)\n",
		fbfilename, fb_width(fb), fb_height(fb), fb_bpp(fb), 
		fb->vi.red.offset, fb->vi.red.length,
		fb->vi.green.offset, fb->vi.green.length,
		fb->vi.blue.offset, fb->vi.blue.length);
	printf("-----------------------------------------\n");

	set_pixel_unpacker(fb);

	return 0;

fail:
	printf("%s is not a framebuffer.\n", fbfilename);
	close(fb->fd);

	return -1;
}

static void fb_close(FBInfo* fb)
{
	munmap(fb->bits, fb_size(fb));
	close(fb->fd);

	return;
}

static int snap2jpg(const char * filename, int quality, FBInfo* fb)
{
	int row_stride = 0; 
	FILE * outfile = NULL;
	JSAMPROW row_pointer[1] = {0};
	struct jpeg_error_mgr jerr;
	struct jpeg_compress_struct cinfo;

	memset(&jerr, 0x00, sizeof(jerr));
	memset(&cinfo, 0x00, sizeof(cinfo));

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb+")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", filename);

		return -1;
	}

	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = fb_width(fb);
	cinfo.image_height = fb_height(fb);
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	jpeg_start_compress(&cinfo, TRUE);

	row_stride = fb_width(fb) * 2;
	JSAMPLE* image_buffer = malloc(3 * fb_width(fb));

	while (cinfo.next_scanline < cinfo.image_height) 
	{
		int i = 0;
		int offset = 0;
		unsigned char* line = fb->bits + cinfo.next_scanline * fb_width(fb) * fb_bpp(fb);

		for(i = 0; i < fb_width(fb); i++, offset += 3, line += fb_bpp(fb))
		{
			fb->unpack(fb, line, image_buffer+offset, image_buffer + offset + 1, image_buffer + offset + 2);
		}

		row_pointer[0] = image_buffer;
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);

	jpeg_destroy_compress(&cinfo);

	return 0;
}

//Ref: http://blog.chinaunix.net/space.php?uid=15059847&do=blog&cuid=2040565
static int snap2png(const char * filename, int quality, FBInfo* fb)
{
	FILE *outfile;
	if ((outfile = fopen(filename, "wb+")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", filename);
		return -1;
	}

	/* prepare the standard PNG structures */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	
	png_infop info_ptr = png_create_info_struct(png_ptr);

	/* setjmp() must be called in every function that calls a PNG-reading libpng function */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(outfile);
		return -1;
	}

	/* initialize the png structure */
	png_init_io(png_ptr, outfile);

	//
	int width = 0;
	int height = 0;
	int bit_depth = 8;
	int color_type = PNG_COLOR_TYPE_RGB;
	int interlace = 0;
	width = fb_width(fb);
	height = fb_height(fb);

	png_set_IHDR (png_ptr, info_ptr, width, height, bit_depth, color_type,
					(!interlace) ? PNG_INTERLACE_NONE : PNG_INTERLACE_ADAM7,
					PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* write the file header information */
	png_write_info(png_ptr, info_ptr);

	png_bytep row_pointers[height];
	png_byte* image_buffer = malloc(3 * width);

	int i = 0;
	int j = 0;
	unsigned char* line = NULL;
	for( ; i < height; i++ )
	{
		line = fb->bits + i * width * fb_bpp(fb);
		for(j = 0; j < width; j++, line += fb_bpp(fb))
		{
			int offset = j * 3;
			fb->unpack(fb, line, image_buffer+offset, image_buffer+offset+1, image_buffer+offset+2);
		}
		row_pointers[i] = image_buffer;
		png_write_rows(png_ptr, &row_pointers[i], 1);
	}
	
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(outfile);

	return 0;

}

void screen_to_file(char *filename, char *fbfilename) 
{
	FBInfo fb;

	//const char* filename   = "fb.jpg";
	//const char* fbfilename = "/dev/fb1";


	//filename   = argv[1];
	//fbfilename = argv[2];

	memset(&fb, 0x00, sizeof(fb));
	if (fb_open(&fb, fbfilename) == 0)
	{
		if(strstr(filename, ".png") != NULL)
		{
			snap2png(filename, 100, &fb);
		}
		else
		{
			snap2jpg(filename, 100, &fb);
		}
		fb_close(&fb);
	}

	return ;
}


void SAVE_DATA()
{
	int operate;

	char *screen_filename = "fb.jpg";

	char *fbfilename = "/dev/fb1";

	operate = get_file_save_mode(pp->p_config);

	if (operate == SAVE_MODE_INSPEC_DATA)
	{}
	else if (operate == SAVE_MODE_INSPEC_TABLE)
	{}
	else if (operate == SAVE_MODE_SCREEN)
	{
		screen_to_file(screen_filename,fbfilename);
	}
	else if (operate == SAVE_MODE_REPORT)
	{}

}
