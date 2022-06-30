#include "minirt.h"

static void	matrixmult(float a[4][4], float b[4][4], float res[4][4])
{
	int	i;
	int	j;
	int	k;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			res[i][j] = 0;
			k = -1;
			while (++k < 4)
				res[i][j] += a[i][k] * b[k][j];
		}
	}
}

void	worldtocammatrix(float transmatrix[4][4], float crdstmmatrix[4][4], float posmatrix[4][4], t_crdstm *cam)
{
	crdstmmatrix[0][0] = cam->ox.vector.x;
	crdstmmatrix[0][1] = cam->ox.vector.y;
	crdstmmatrix[0][2] = cam->ox.vector.z;
	crdstmmatrix[0][3] = 0;
	crdstmmatrix[1][0] = cam->oy.vector.x;
	crdstmmatrix[1][1] = cam->oy.vector.y;
	crdstmmatrix[1][2] = cam->oy.vector.z;
	crdstmmatrix[1][3] = 0;
	crdstmmatrix[2][0] = cam->oz.vector.x;
	crdstmmatrix[2][1] = cam->oz.vector.y;
	crdstmmatrix[2][2] = cam->oz.vector.z;
	crdstmmatrix[2][3] = 0;
	ft_bzero(crdstmmatrix[3], sizeof(float) * 4);
	ft_bzero(posmatrix[0], sizeof(float) * 4);
	posmatrix[0][0] = 1;
	posmatrix[0][3] = -cam->pos.x;
	ft_bzero(posmatrix[1], sizeof(float) * 4);
	posmatrix[1][1] = 1;
	posmatrix[1][3] = -cam->pos.y;
	ft_bzero(posmatrix[2], sizeof(float) * 4);
	posmatrix[2][2] = 1;
	posmatrix[2][3] = -cam->pos.z;
	ft_bzero(posmatrix[3], sizeof(float) * 4);
	posmatrix[3][3] = 1;
	matrixmult(crdstmmatrix, posmatrix, transmatrix);
}

void	transpos(t_cart	*pos, float transmatrix[4][4])
{
	float	p[4];
	float	k[4];
	int		i;
	int		j;

	p[0] = pos->x;
	p[1] = pos->y;
	p[2] = pos->z;
	p[3] = 1;
	i = -1;
	while (++i < 4)
	{
		k[i] = 0;
		j = -1;
		while (++j < 4)
			k[i] += transmatrix[i][j] * p[j];
	}
	cartbuilder(k[0], k[1], k[2], pos);
}
