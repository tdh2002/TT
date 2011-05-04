
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base_config.h"
#include <string.h>

#if 0
struct _Material 
{
	guint	Velocity_LW;	/* 声速 单位 0.01m/s 纵波 快点 */
	guint	Velocity_SW;	/* 声速 单位 0.01m/s 横波 慢点 */
	gchar	Name[20];		/* 材料名字 */
};
#endif

#if 0
	"Aluminum", "Steel common", "Steel stainless", "Brass", "Copper",
	"Iron", "Lead", "Nylon", "Silver", "Gold", "Zinc", "Titanium", 
	"Tin", "Epoxy resin", "Ice", "Nickel", "Plexiglass", "Polystyrene",
	"Porcelain", "PVC", "Quartz glass", "Rubber vulcanized", "Teflon", "Water",	/* 24 个材料 */
#endif
static const st_MATERIAL data[] =
{
	{640000, 325000, "Aluminum"},			/* 0 */
	{592000, 0, "Steel common"},			/* 1 */
	{574000, 0, "Steel stainless"},			/* 2 */
	{439900, 0, "Brass"},			/* 3 */
	{472000, 325000, "Copper"},			/* 4 */
	{640000, 325000, "Aluminum5"},			/* 5 */
	{640000, 325000, "Aluminum6"},			/* 6 */
	{640000, 325000, "Aluminum7"},			/* 7 */
	{640000, 325000, "Aluminum8"},			/* 8 */
	{640000, 325000, "Aluminum99"},			/* 8 */
	{640000, 325000, "Aluminum9"},			/* 10 */
	{640000, 325000, "Aluminum10"},			/* 11*/
	{640000, 325000, "Aluminum11"},			/* 12 */
	{640000, 325000, "Aluminum12"},			/* 13 */
	{640000, 325000, "Aluminum13"},			/* 14 */
	{640000, 325000, "Aluminum14"},			/* 15 */
	{640000, 325000, "Aluminum15"},			/* 16 */
	{640000, 325000, "Aluminum16"},			/* 17 */
	{640000, 325000, "Aluminum17"},			/* 18 */
	{640000, 325000, "Aluminum18"},			/* 19 */
	{640000, 325000, "Aluminum19"},			/* 20 */
	{640000, 325000, "Aluminum20"},			/* 21 */
	{640000, 325000, "Aluminum21"},			/* 22 */
	{640000, 325000, "Aluminum22"}			/* 23 */
};

gint parse_material_info (CONFIG_P p)
{
	guint	pos = p->part.Material_pos;
	/* 判断 p 是不是 st_MATERIAL_P ? */
	memcpy ((void *)(&(p->part.Material)), (void *)(&data[pos]), sizeof (st_MATERIAL));
	return 0;
}

static void print_material_info_real (st_MATERIAL_P p)
{
	return g_print ("LW: %dm/s SW: %dm/s name: %s\n", p->Velocity_LW / 100, p->Velocity_SW / 100, p->Name);
}

void print_material_info (CONFIG_P p)
{
	return print_material_info_real (&(p->part.Material));
}

gchar *get_material_name(CONFIG_P p)
{
	return p->part.Material.Name;
}

static guint get_material_lw_real (st_MATERIAL_P p)
{
	return p->Velocity_LW;
}

guint get_material_lw (CONFIG_P p)
{
	return get_material_lw_real (&(p->part.Material));
}

static guint get_material_sw_real (st_MATERIAL_P p)
{
	return p->Velocity_SW;
}

guint get_material_sw (CONFIG_P p)
{
	return get_material_sw_real (&(p->part.Material));
}





