#include "minirt.h"

static void	surfdefiner(t_cart *dots, t_poly *poly, int dotnum, int lttd)
{
	poly->dotcount = 4;
	poly->dots = malloc(sizeof(*poly->dots) * poly->dotcount);
	poly->dots[0] = &dots[dotnum];
	poly->dots[1] = &dots[dotnum - 1];
	poly->dots[2] = &dots[dotnum - 1 - (RNDSGMNTS - 2) * lttd];
	poly->dots[3] = &dots[dotnum - (RNDSGMNTS - 2) * lttd];
	normbuilder(&poly->dots[0], &poly->dots[1], &poly->dots[3], &poly->norm);
	poly->txtr = NULL;
}

static int	dotfiller(t_cart *dots, t_poly *polys, float radius)
{
	int		lttd;
	int		dotnum;
	int		dotsinround;
	int		polyindx;
	t_axis	rotltd;

	definepols(dots, radius, NULL);
	dotnum = 2 + circledotsfiller(&dots[2], radius, NULL, TRUE);
	vectorbuilder(0, 1, 0, &rotltd);
	polyindx = -1;
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
	{
		rotltd.ang = 2 * M_PI / RNDSGMNTS * lttd;
		dotsinround = circledotsfiller(&dots[dotnum++], radius, &rotltd, TRUE);
		while (--dotsinround)
			if ((dotsinround == (RNDSGMNTS - 2) / 2 + 1 && ++dotnum) || TRUE)
				surfdefiner(dots, &polys[++polyindx], dotnum++, lttd);
		dotnum++;
	}
	return (polyindx);
}

static void	psurfpatch(t_poly *poly, t_cart *polus, t_cart *dot1, t_cart *dot2)
{
	poly->dotcount = 3;
	poly->dots = malloc(sizeof(*poly->dots) * poly->dotcount);
	poly->dots[0] = polus;
	poly->dots[1] = dot1;
	poly->dots[2] = dot2;
	normbuilder(&poly->dots[0], &poly->dots[1], &poly->dots[2], &poly->norm);
	poly->txtr = NULL;
}

static int	jointing(t_cart *dots, t_poly *polys, int dotsnum, int indx)
{
	int		i;
	t_cart	*dot1;
	t_cart	*dot2;

	dot1 = dots + 2;
	i = 0;
	while (++i < RNDSGMNTS - 2)
	{
		if (i == (RNDSGMNTS - 2) / 2 - 1)
			continue ;
		polys[++indx].dotcount = 4;
		polys[indx].dots = malloc(sizeof(*polys[indx].dots) * polys[indx].dotcount);
		polys[indx].dots[0] = &dot1[i];
		polys[indx].dots[1] = &dot1[i - 1];
		polys[indx].dots[2] = &dot1[dotsnum - 2 - 1 - (i - 1)];
		polys[indx].dots[3] = &dot1[dotsnum - 2 - 1 - i];
		normbuilder(&dot1[i], &dot1[i - 1], &dot1[dotsnum - 3 - i], &polys[indx].norm);
		polys[indx].txtr = NULL;
	}
	psurfpatch(&polys[++indx], &dots[0], dot1, &dots[dotsnum - 1]);
	psurfpatch(&polys[++indx], &dots[0], dot1 + RNDSGMNTS - 2 - 1,
					  &dots[dotsnum - 1] - RNDSGMNTS - 2 - 1);
	dot1 = &dots[(RNDSGMNTS - 2) / 2 + 1];
	dot2 = &dots[dotsnum - (RNDSGMNTS - 2) / 2 - 1];
	psurfpatch(&polys[++indx], &dots[1], dot1, dot2);
	psurfpatch(&polys[++indx], &dots[1], dot1 + 1, dot2 - 1);
	return (indx);
}

void	spherebuilder(t_obj *obj, float radius)
{
	int	lttd;
	int	polyindx;

	obj->dotsnum = RNDSGMNTS / 2 * (RNDSGMNTS - 2) + 2;
	obj->polynum = RNDSGMNTS / 2 * (RNDSGMNTS - 3) + 2 * RNDSGMNTS;
	obj->dots = malloc(sizeof(*obj->dots) * obj->dotsnum);
	obj->poly = malloc(sizeof(*obj->poly) * obj->polynum);
	polyindx = dotfiller(obj->dots, obj->poly, radius);
	polyindx = jointing(obj->dots, &obj->poly, obj->dotsnum, polyindx);
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
	{
		psurfpatch(&obj->poly[++polyindx], &obj->dots[0],
			&obj->dots[1 + lttd * (RNDSGMNTS - 2)],
				&obj->dots[1 + (lttd - 1) * (RNDSGMNTS - 2)]);
		psurfpatch(&obj->poly[++polyindx], &obj->dots[0],
			&obj->dots[1 + lttd * (RNDSGMNTS - 2) + RNDSGMNTS - 3],
				&obj->dots[1 + (lttd - 1) * (RNDSGMNTS - 2) + RNDSGMNTS - 3]);
		psurfpatch(&obj->poly[++polyindx], &obj->dots[1],
			&obj->dots[1 + lttd * (RNDSGMNTS - 2) + RNDSGMNTS / 2],
				&obj->dots[1 + (lttd - 1) * (RNDSGMNTS - 2) + RNDSGMNTS / 2]);
		psurfpatch(&obj->poly[++polyindx], &obj->dots[1],
			&obj->dots[1 + lttd * (RNDSGMNTS - 2) + RNDSGMNTS / 2 + 1],
				&obj->dots[1 + (lttd - 1) * (RNDSGMNTS - 2) + RNDSGMNTS / 2 + 1]);
	}
}
