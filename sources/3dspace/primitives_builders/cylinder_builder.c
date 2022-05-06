#include "minirt.h"

static void	surfdefiner(t_cart *dots, t_cart *shift, t_poly *polys)
{
	int		i;

	shift->z = height;
	i = 0;
	while (++i < RNDSGMNTS)
	{
		shift[i].z = height;
		polys[i - 1].dotcount = 4;
		polys[i - 1].dots = malloc(sizeof(*polys->dots) * polys[i - 1].dotcount);
		polys[i - 1].dots[0] = &dots[i];
		polys[i - 1].dots[1] = &shift[i];
		polys[i - 1].dots[2] = &shift[i - 1];
		polys[i - 1].dots[3] = &dots[i - 1];
		normbuilder(&shift[i], &shift[i - 1], &dots[i], &polys[i - 1].norm);
		polys[i - 1].txtr = NULL;
	}
	polys[--i].dotcount = 4;
	polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
	polys[i].dots[0] = dots;
	polys[i].dots[1] = shift;
	polys[i].dots[2] = &shift[RNDSGMNTS - 1];
	polys[i].dots[3] = &dots[RNDSGMNTS - 1 - 1];
	normbuilder(polys[i]->dots, &polys[i].dots[1], &polys[i].dots[3], &polys[indx].norm);
	polys[i].txtr = NULL;
}

static void	buttsurf(t_cart *central, t_cart *dots, t_poly *polys)
{
	int	i;

	i = -1;
	while (++i < RNDSGMNTS - 1)
	{
		polys[i].dotcount = 3;
		polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
		polys[i].dots[0] = central;
		polys[i].dots[1] = &dots[i];
		polys[i].dots[2] = &dots[i + 1];
		normbuilder(polys[i].dots, &polys[i].dots[1], &polys[i].dots[2], &polys[i].norm);
		polys[i].txtr = NULL;
	}
	polys[i].dotcount = 3;
	polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
	polys[i].dots[0] = central;
	polys[i].dots[1] = &dots[i];
	polys[i].dots[2] = dots;
	normbuilder(polys[i].dots, &polys[i].dots[1], &polys[i].dots[2], &polys[i].norm);
	polys[i].txtr = NULL;
}

void	cylinderbuilder(t_obj *obj, float radius, float height)
{
	obj->dotsnum = RNDSGMNTS * 2 + 2;
	obj->polynum = RNDSGMNTS * 3;
	obj->dots = malloc(sizeof(*obj->dots) * obj->dotsnum);
	cartbuilder(0, 0, 0, obj->dots);
	cartbuilder(0, 0, height, obj->dots + 1);
	obj->poly = malloc(sizeof(*obj->polynum) * obj->polynum);
	circledotsfiller(&obj->dots[2], radius, NULL, FALSE);
	circledotsfiller(&obj->dots[RNDSGMNTS + 2], radius, NULL, FALSE);
	surfdefiner(&obj->dots[2], &obj->dots[RNDSGMNTS + 2], obj->poly);
	buttsurf(&obj->dots[0], &obj->dots[2], &obj->poly[RNDSGMNTS]);
	buttsurf(&obj->dots[1], &obj->dots[RNDSGMNTS + 2], &obj->poly[2 * RNDSGMNTS]);
}
