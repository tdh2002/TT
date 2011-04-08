
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

char SOURCE_FILE_NAME[FILE_NAME_MAX];
char SOURCE_FILE_PATH[FILE_NAME_MAX];
char TARGET_FILE_NAME[FILE_NAME_MAX];
char TARGET_FILE_PATH[FILE_NAME_MAX];

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

/* 保存当前group的聚焦法则 */
void read_law_file (const gchar *file_path, gint offset, gint group)
{
	char version[16];
	int i, j, k;
	FILE *fd;


//	if ((fd = open(file_path, O_RDWR | O_CREAT |  O_TRUNC, 0644))<0) 
	if((fd = fopen(file_path, "rb")) == NULL)
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		g_print("read focal law data\n");
		fscanf(fd, "%s	%d", version, &k);
		g_print("read focal law data %d %d\n", &TMP(beam_qty[group]));
		k = TMP(beam_qty[group]);
		for (i = 0; i < k; i++)
		{
			fscanf(fd, "%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d",
					&CFG(focal_law_all_beam[offset + i]).N_ActiveElements, 
					&CFG(focal_law_all_beam[offset + i]).frequency, 
					&CFG(focal_law_all_beam[offset + i]).cycle, 
					&CFG(focal_law_all_beam[offset + i]).sumgain,
					&CFG(focal_law_all_beam[offset + i]).mode,
					&CFG(focal_law_all_beam[offset + i]).filter, 
					&CFG(focal_law_all_beam[offset + i]).R_angle,
					&CFG(focal_law_all_beam[offset + i]).S_angle, 
					&CFG(focal_law_all_beam[offset + i]).T_first, 
					&CFG(focal_law_all_beam[offset + i]).R_first,
					&CFG(focal_law_all_beam[offset + i]).Scan_offset, 
					&CFG(focal_law_all_beam[offset + i]).Index_offset, 
					&CFG(focal_law_all_beam[offset + i]).G_delay,
					&CFG(focal_law_all_beam[offset + i]).F_depth,
					&CFG(focal_law_all_beam[offset + i]).M_velocity );

			for (j = 0; j < CFG(focal_law_all_beam[offset + i]).N_ActiveElements; j++)
			{
					fscanf(fd, "%d	%d	%d	%d	%d	%d",          
							&CFG(focal_law_all_elem[offset + i][j]).E_number, 
							&CFG(focal_law_all_elem[offset + i][j]).FL_gain,   
							&CFG(focal_law_all_elem[offset + i][j]).T_delay,
							&CFG(focal_law_all_elem[offset + i][j]).R_delay,
							&CFG(focal_law_all_elem[offset + i][j]).Amplitude, 
							&CFG(focal_law_all_elem[offset + i][j]).P_width ); 
			} 
		} 
	}
	fclose (fd);
}

/* 保存当前group的聚焦法则 */
void save_law_file (const gchar *file_path, gint offset, gint group)
{
	int i, j;
	FILE *fd;

//	if ((fd = open(file_path, O_RDWR | O_CREAT |  O_TRUNC, 0644))<0) 
	if((fd = fopen(file_path, "wb+")) == NULL)
	{
		perror("open:");
		exit(1);
	}
	else 
	{
		g_print("Save focal law data\n");
		fprintf(fd, "%s	%d\015\012", "dp1.00", TMP(beam_qty[group]));
		for (i = 0; i < TMP(beam_qty[group]); i++)
		{
			fprintf(fd, "%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\015\012",
					CFG(focal_law_all_beam[offset + i]).N_ActiveElements, CFG(focal_law_all_beam[offset + i]).frequency, CFG(focal_law_all_beam[offset + i]).cycle, CFG(focal_law_all_beam[offset + i]).sumgain, CFG(focal_law_all_beam[offset + i]).mode,
					CFG(focal_law_all_beam[offset + i]).filter, CFG(focal_law_all_beam[offset + i]).R_angle,CFG(focal_law_all_beam[offset + i]).S_angle, CFG(focal_law_all_beam[offset + i]).T_first, CFG(focal_law_all_beam[offset + i]).R_first,
					CFG(focal_law_all_beam[offset + i]).Scan_offset, CFG(focal_law_all_beam[offset + i]).Index_offset, CFG(focal_law_all_beam[offset + i]).G_delay, CFG(focal_law_all_beam[offset + i]).F_depth, CFG(focal_law_all_beam[offset + i]).M_velocity );


			for (j = 0; j < CFG(focal_law_all_beam[offset + i]).N_ActiveElements; j++)
			{
					fprintf(fd, "%d	%d	%d	%d	%d	%d\015\012",          
							CFG(focal_law_all_elem[offset + i][j]).E_number,
							CFG(focal_law_all_elem[offset + i][j]).FL_gain,  
							CFG(focal_law_all_elem[offset + i][j]).T_delay,
							CFG(focal_law_all_elem[offset + i][j]).R_delay,
							CFG(focal_law_all_elem[offset + i][j]).Amplitude, 
							CFG(focal_law_all_elem[offset + i][j]).P_width ); 
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
    char command[1024] = "cp ";
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

    value = gtk_tree_model_get_iter_from_string (source_model, &source_iter, "0");

    while(value)
    {
        if (gtk_tree_selection_iter_is_selected(source_selection,&source_iter))
        {
            gtk_tree_model_get(source_model, &source_iter, 0, &value_name,  -1);

            g_printf("value_name = %s\n",value_name);

            memset(command,0,sizeof(command));
              
            strcpy(command,"cp ");

            target_file = Get_Target_File_Path();

            source_file = Get_Source_File_Path();    

            strcat(command,source_file);

            strcat(command,value_name);

            strcat(command," ");

            strcat(command,target_file);

            value = system(command); 
              
            Scan_Target_File(target_list,target_model,value_name);
 
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
    char command[1024] = "mv ";
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
              
            strcpy(command,"rm ");

            source_file = Get_Source_File_Path();    

            strcat(command,source_file);

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



