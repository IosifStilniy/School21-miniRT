#include "minirt.h"

static void	dotdefiner(t_cart *dot, t_axis *rotlng, t_axis *rotltd, float radius)
{
	dot->x = radius;
	dot->y = 0;
	dot->z = 0;
	quartrot(dot, rotlng);
	quartrot(dot, rotltd);
}

static t_bool surfdefiner(t_cart *dots, t_poly *poly, int lngtd, int lttd)
{
	int	shift;

	if (!lttd || !lngtd)
		return (FALSE);
	poly->dotcount = 4;
	poly->dots = malloc(sizeof(*poly->dots) * poly->dotcount);
	poly->dots[0] = &dots[lttd * (RNDSGMNTS - 2) + 2 + lngtd];
	poly->dots[1] = &dots[lttd * (RNDSGMNTS - 2) + 2 + lngtd - 1];
	shift = (lttd - 1) * (RNDSGMNTS - 2) + 2;
	if (lttd == 1)
		shift = 1 + (lngtd > (RNDSGMNTS - 2) / 2);
	poly->dots[2] = &dots[shift + lngtd - 1];
	poly->dots[3] = &dots[shift + lngtd];
	normbuilder(&poly->dots[0], &poly->dots[1], &poly->dots[3], &poly->norm);
	poly->txtr = NULL;
}

static void	dotfiller(t_cart *dots, t_poly *polys, float radius, int *polyindx)
{
	int		lngtd;
	int		lttd;
	t_axis	rotlng;
	t_axis	rotltd;

	vectorbuilder(0, 0, 1, &rotlng);
	vectorbuilder(0, 1, 0, &rotltd);
	rotltd.ang = 0;
	lttd = -1;
	while (++lttd < RNDSGMNTS / 2)
	{
		rotlng.ang = -2 * M_PI / RNDSGMNTS * (!!lttd);
		lngtd = -1;
		while (++lngtd < RNDSGMNTS - 2 * (!!lttd))
		{
			rotlng.ang += 2 * M_PI / RNDSGMNTS * (1 + (lttd && lngtd == RNDSGMNTS / 2));
			dotdefiner(&dots[lttd * (RNDSGMNTS - 2) + 2 * (!!lttd) + lngtd], &rotlng, &rotltd, radius);
			if (lttd && lngtd)
				surfdefiner(dots, &polys[(*polyindx)++], lngtd, lttd);
		}
		rotltd.ang += 2 * M_PI / RNDSGMNTS;
	}
}

static void polarsurfacepatch(t_poly *poly, t_cart *polardot, t_cart *dot1, t_cart *dot2)
{
	poly->dotcount = 3;
	poly->dots = malloc(sizeof(*poly->dots) * poly->dotcount);
	poly->dots[0] = polardot;
	poly->dots[1] = dot1;
	poly->dots[2] = dot2;
	normbuilder(&poly->dots[0], &poly->dots[1], &poly->dots[2], &poly->norm);
}

static int	polarsurfacepatch(t_poly *polys, t_cart *dots, int indx, int lttd)
{
	int	frontshift;
	int	backshift;

	frontshift = (lttd - 1) * RNDSGMNTS + 2;
	if (lttd == 1)
		frontshift = 1;
	backshift = frontshift + RNDSGMNTS - 2 - 2 * (lttd > 1);
	polys[indx].dotcount = 3;
	polys[indx].dots = malloc(sizeof(*polys[indx].dots) * polys[indx].dotcount);
	polys[indx].dots[0] = 0;
	polys[indx].dots[1] = dots[lttd * (RNDSGMNTS - 2) + 2];
	polys[indx].dots[2] = dots[frontshift];
	normbuilder(&polys[indx].dots[0], &polys[indx].dots[1], &polys[indx].dots[2], &polys[indx].norm);
	polys[++indx].dotcount = 3;
	polys[indx].dots = malloc(sizeof(*polys[indx].dots) * polys[indx].dotcount);
	polys[indx].dots[0] = 0;
	polys[indx].dots[1] = dots[lttd * (RNDSGMNTS - 2) + 1 + RNDSGMNTS - 3];
	polys[indx].dots[2] = dots[backshift];
	normbuilder(&polys[indx].dots[0], &polys[indx].dots[1], &polys[indx].dots[2], &polys[indx].norm);
	return (++indx);
}

void	spherebuilder(t_obj *obj, float radius)
{
	int	lngtd;
	int	lttd;
	int	polyindx;
	int	frontshift;
	int	backshift;

	obj->dotsnum = RNDSGMNTS / 2 * (RNDSGMNTS - 2) + 2;
	obj->polynum = RNDSGMNTS / 2 * (RNDSGMNTS - 3) + 2 * RNDSGMNTS;
	obj->dots = malloc(sizeof(*obj->dots) * obj->dotsnum);
	obj->poly = malloc(sizeof(*obj->poly) * obj->polynum);
	polyindx = 0;
	dotfiller(obj->dots, obj->poly, radius, &polyindx);
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
	{
		frontshift = (lttd - 1) * RNDSGMNTS + 2;
		if (lttd == 1)
			frontshift = 1;
		polarsurfacepatch(&obj->poly[polyindx++], &obj->dots[0],
			&obj->dots[lttd * (RNDSGMNTS - 2) + 2], &obj->dots[frontshift]);
		backshift = frontshift + RNDSGMNTS - 2 - 2 * (lttd > 1);
		polarsurfacepatch(&obj->poly[polyindx++], &obj->dots[0],
			&obj->dots[lttd * (RNDSGMNTS - 2) + 1 + RNDSGMNTS - 3], &obj->dots[backshift]);
	}
}
