/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base_config.h"
#include <string.h>

/* 材料 (Material) 28Byte OK */
typedef struct _Material 
{
	guint	Velocity_LW;	/* 声速 单位 0.01m/s 纵波 快点 */
	guint	Velocity_SW;	/* 声速 单位 0.01m/s 横波 慢点 */
	gchar	Name[20];		/* 材料名字 */
} st_MATERIAL;

static const st_MATERIAL data[] =
{
	{626000, 308000, "Aluminum"},			/* 0 */
	{592000, 323000, "Steel common"},		/* 1 */
	{566400, 0, "Steel stainless"},			/* 2 */
	{439400,0 , "Brass"},					/* 3 */
	{470000, 226000, "Copper"},				/* 4 */
	{5893, 323000, "Iron"},					/* 5 */
	{215900, 0, "Lead"},					/* 6 */
	{220000, 0, "Nylon"},					/* 7 */
	{360700, 0, "Silver"},					/* 8 */
	{325100, 0, "Gold"},					/* 8 */
	{421600, 0, "Zinc"},					/* 10 */
	{609600, 0, "Titanium"},				/* 11 */
	{332700, 0, "Tin"},						/* 12 */
	{254000, 110000, "Epoxy resin"},		/* 13 */
	{398800, 0, "Ice"},						/* 14 */
	{563900, 0, "Nickel"},					/* 15 */
	{273000, 146000, "Plexiglass"},			/* 16 */
	{235000, 115000, "Polystyrene"},		/* 17 */
	{584200, 0, "Porcelain"},				/* 18 */
	{238800, 0, "PVC"},						/* 19 */
	{563900, 0, "Quartz glass"},			/* 20 */
	{231100, 0, "Rubber vulcanized"},		/* 21 */
	{142200, 0, "Teflon"},					/* 22 */
	{148000, 0, "Water"}					/* 23 */
};

gchar *get_material_name(CONFIG *p)
{
	return (gchar *)(data[p->part.Material_pos].Name);
}

guint get_material_lw (CONFIG *p)
{
	return data[p->part.Material_pos].Velocity_LW;
}

guint get_material_sw (CONFIG *p)
{
	return data[p->part.Material_pos].Velocity_SW;
}

guint get_part_geometry (CONFIG *p)
{
	return p->part.Geometry;
}

void set_part_geometry (CONFIG *p, gpointer data)
{
	p->part.Geometry = (guchar)(GPOINTER_TO_UINT (data));
}

guint get_part_material (CONFIG *p)
{
	return p->part.Material_pos;
}

void set_part_material (CONFIG *p, gpointer data)
{
	p->part.Material_pos = (guchar)(GPOINTER_TO_UINT (data));
}

guint get_part_thickness (CONFIG *p)
{
	return p->part.Thickness;
}

void set_part_thickness (CONFIG *p, gpointer data)
{
	p->part.Thickness = (guint)(GPOINTER_TO_UINT (data));
}

guint get_part_diameter (CONFIG *p)
{
	return p->part.Diameter;
}

void set_part_diameter (CONFIG *p, gpointer data)
{
	p->part.Diameter = (guint)(GPOINTER_TO_UINT (data));
}





