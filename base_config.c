
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base_config.h"

static const st_MATERIAL data[] =
{
	{640000, 325000, "Aluminum"},			/* 0 */
	{592000, 0, "Steel common"},		/* 1 */
	{640000, 325000, "Aluminum2"},			/* 2 */
	{640000, 325000, "Aluminum3"},			/* 3 */
	{640000, 325000, "Aluminum4"},			/* 4 */
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

gint parse_material_info (gint pos, st_MATERIAL_P p)
{
	/* 判断 p 是不是 st_MATERIAL_P ? */
	memcpy (p, &data[pos], sizeof (st_MATERIAL));
	return 0;
}

void print_material_info (st_MATERIAL_P p)
{
	return g_print ("LW: %dm/s SW: %dm/s name: %s\n", p->Velocity_LW / 100, p->Velocity_SW / 100, p->Name);
}

gchar *get_material_name(st_MATERIAL_P p)	
{
	return p->Name;
}

guint get_material_lw (st_MATERIAL_P p)
{
	return p->Velocity_LW;
}

guint get_material_sw (st_MATERIAL_P p)
 {
	return p->Velocity_SW;
}
