#include "minirt.h"

static void	definearrows(t_button *decrease, t_button *increase, t_res leftup, t_res *size)
{
	decrease->leftup = leftup;
	decrease->bottomright.x = leftup.x + size->x;
	decrease->bottomright.y = leftup.y + size->y;
	increase->leftup.x = leftup.x + 75;
	increase->leftup.y = leftup.y;
	increase->bottomright.x = increase->leftup.x + size->x;
	increase->bottomright.y = increase->leftup.y + size->y;
}

static void	definearrowframes(t_button arrows[20], int left, int yobj)
{
	t_res	size;

	size.x = 10;
	size.y = 20;
	definearrows(&arrows[0], &arrows[1], (t_res){left, 32}, &size);
	definearrows(&arrows[2], &arrows[3], (t_res){left, 55}, &size);
	definearrows(&arrows[4], &arrows[5], (t_res){left, 113}, &size);
	definearrows(&arrows[6], &arrows[7], (t_res){left, 138}, &size);
	definearrows(&arrows[8], &arrows[9], (t_res){left, yobj + 38}, &size);
	definearrows(&arrows[10], &arrows[11], (t_res){left, yobj + 60}, &size);
	definearrows(&arrows[12], &arrows[13], (t_res){left, yobj + 81}, &size);
	definearrows(&arrows[14], &arrows[15], (t_res){left, yobj + 100}, &size);
	definearrows(&arrows[16], &arrows[17], (t_res){left, yobj + 159}, &size);
	definearrows(&arrows[18], &arrows[19], (t_res){left, yobj + 177}, &size);
	definearrows(&arrows[20], &arrows[21], (t_res){left, yobj + 197}, &size);
	definearrows(&arrows[22], &arrows[23], (t_res){left, yobj + 219}, &size);
}

static void	initsettings(t_settings *settings)
{
	settings->fov.step = 2.5 * M_PI / 180;
	settings->fov.stepping = 0.5 * M_PI / 180;
	settings->size.step = 0.1;
	settings->size.stepping = 0.1;
	settings->color.step = 10.f / 255.f;
	settings->color.stepping = 1.f / 255.f;
	settings->sens.kval = 1;
	settings->sens.move.step = 0.1;
	settings->sens.move.stepping = 0.1;
	settings->sens.mval = 1;
	settings->sens.mouse.step = 0.1;
	settings->sens.mouse.stepping = 0.1;
}

void	initinterface(t_intrfc *interface, void *mlx, t_res *win)
{
	interface->cam.img = mlx_xpm_file_to_image(mlx, CAMLEGEND,
		&interface->cam.res.x, &interface->cam.res.y);
	interface->campos.x = win->x - interface->cam.res.x;
	interface->campos.y = 0;
	interface->obj.img = mlx_xpm_file_to_image(mlx, OBJLEGEND,
		&interface->obj.res.x, &interface->obj.res.y);
	interface->objpos.x = win->x - interface->obj.res.x;
	interface->objpos.y = interface->cam.res.y;
	interface->frame.x = interface->campos.x;
	interface->frame.y = interface->objpos.y + interface->obj.res.y;
	interface->selected = NULL;
	initsettings(&interface->settings);
	definearrowframes(interface->arrows, 63 + interface->frame.x,
		interface->objpos.y);
	interface->attach.leftup.x = interface->campos.x + 13;
	interface->attach.leftup.y = 183;
	interface->attach.bottomright.x = interface->attach.leftup.x + 100;
	interface->attach.bottomright.y = interface->attach.leftup.y + 20;
}
