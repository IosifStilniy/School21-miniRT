/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 14:58:29 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/03 21:51:30 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "get_next_line.h"
# include "libft.h"
# include <mlx.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/errno.h>
# include <fcntl.h>
# include <string.h>
# include <math.h>
# include <stdio.h>

# define KEY_Q 12
# define KEY_E 14
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_W 13
# define KEY_L 37
# define KEY_C 8
# define KEY_R 15
# define KEY_I 34
# define KEY_N 45
# define KEY_UP 126
# define KEY_LEFT 123
# define KEY_DOWN 125
# define KEY_RIGHT 124
# define KEY_ESC 53
# define KEY_CMND 259
# define KEY_SHIFT 257
# define KEY_SPACE 49
# define KEY_CNTRL 256
# define KEY_CLBRCT 30
# define KEY_OPBRCT 33
# define LMB 1
# define MMB 3
# define SCRL_UP 5
# define SCRL_DOWN 4

# define X_EVENT_KEY_EXIT 17
# define X_EVENT_KEY_PRESS 2
# define X_EVENT_MOUSE_PRESS 4

# define ESC_TEXT "\033[92mClosing MiniRT.\033[0m"
# define ERR_TEXT "Error"

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

# ifndef RESX
#  define RESX 800
# endif

# ifndef RESY
#  define RESY 600
# endif

# ifndef SHIFT_SPEED
#  define SHIFT_SPEED 5
# endif

# ifndef DEFANG
#  define DEFANG 2.f
# endif

# ifndef FRAMECLR
#  define FRAMECLR 0x00FFFFFF
# endif

# ifndef NORMCLR
#  define NORMCLR 0x00FF0000
# endif

# ifndef CRNRS
#  define CRNRS 5
# endif

# ifndef GRIDSIZE
#  define GRIDSIZE 20
# endif

# ifndef GRIDLINES
#  define GRIDLINES 6
# endif

# ifndef PRMTVS
#  define PRMTVS "spplcy"
# endif

# ifndef NUMPRMTVS
#  define NUMPRMTVS 3
# endif

# ifndef RNDSGMNTS
#  define RNDSGMNTS 24
# endif

# ifndef CAMLEGEND
#  define CAMLEGEND "sources/interface/caminterface.xpm"
# endif

# ifndef OBJLEGEND
#  define OBJLEGEND "sources/interface/objinterface.xpm"
# endif

# ifndef INVINP
#  define INVINP "needs one arg: [filename].rt"
# endif

# ifndef INVFILE
#  define INVFILE "Error"
# endif

# ifndef DUPDET
#  define DUPDET "ambient light, lightspot or camera have more than one definition or none of them"
# endif

# ifndef INVDEF
#  define INVDEF "invalid definition of scene parameters"
# endif

# ifndef INVNUM
#  define INVNUM "invalid num type: float for coordinates and int for color"
# endif

# ifndef INVCRD
#  define INVCRD "for definition orientation in space needed 3 coords of float type in format 'X,Y,Z' [and additionally coords of norm vector with Length = 1 for cameras and models]"
# endif

# ifndef SPACES
#  define SPACES " \t\v\f\r"
# endif

# ifndef NUMSPACES
#  define NUMSPACES "-+0123456789. \t\v\f\r"
# endif

# ifndef WORLD
#  define WORLD NULL
# endif

typedef int				t_bool;
typedef unsigned int	t_ui;

typedef struct s_res {
	int	x;
	int	y;
}	t_res;

typedef struct	s_data {
	void	*img;
	t_res	res;
	t_res	cntr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_cart {
	float	x;
	float	y;
	float	z;
}	t_cart;

typedef	struct s_ray {
	t_cart	dir;
	t_cart	orig;
}	t_ray;

typedef struct s_axis {
	t_cart	vector;
	float	length;
	float	ang;
}	t_axis;

typedef struct s_rot {
	t_cart	*start;
	t_cart	*end;
	t_axis	axis;
	t_axis	xyaxis;
}	t_rot;

typedef struct s_crdstm {
	t_cart	pos;
	t_axis	ox;
	t_axis	oy;
	t_axis	oz;
}	t_crdstm;

typedef struct s_light {
	double		light_ratio;
	t_cart		color;
	t_cart		pos;
}	t_light;

typedef struct s_vrtx {
	int		dot;
	t_cart	srcnorm;
	t_cart	norm;
	t_cart	uv;
}	t_vrtx;

typedef struct s_poly {
	t_vrtx	vrtxs[3];
	t_data	*txtr;
	t_data	*heightmap;
	t_bool	interpolate;
	t_cart	srcnorm;
	t_cart	norm;
}	t_poly;

typedef struct s_dots {
	int		dotsnum;
	t_cart	*dots;
	t_cart	*pos;
	t_cart	*scale;
	int		(*rout)[2];
	int		routsize;
}	t_dots;

typedef struct s_polys {
	int		polynum;
	t_data	txtr;
	t_bool	checkerboard;
	t_poly	*poly;
}	t_polys;

typedef struct s_obj {
	t_dots		dots;
	t_polys		polys;
	t_cart		*colrs;
	t_crdstm	crdstm;
	float		outframe;
	t_rot		*rot;
}	t_obj;

typedef struct t_trans {
	float		pos[4][4];
	float		crdstm[4][4];
	float		trans[4][4];
}	t_trans;

typedef struct s_attach {
	t_obj		*obj;
	t_crdstm	crdstm;
}	t_attach;

typedef struct s_camera {
	t_crdstm	crdstm;
	float		focus;
	float		fov;
	t_list		*objs;
	t_cart		*lightpos;
	int			lightcount;
	t_cart		corners[CRNRS];
	t_attach	attached;
}	t_camera;

typedef struct s_win {
	void		*win;
	t_camera	*camera;
	t_list		*cameras;
	t_res		res;
	t_res		cntr;
	char		*header;
}	t_win;

typedef struct s_mouse {
	int	yshift;
}	t_mouse;

typedef struct s_keybrd {
	t_bool	render;
	t_bool	interface;
	t_bool	legend;
	t_bool	normalpaint;
}	t_keybrd;

typedef struct s_button {
	t_res	leftup;
	t_res	bottomright;
}	t_button;

typedef struct s_step {
	float	step;
	float	stepping;
}	t_step;

typedef struct s_sens {
	t_step		mouse;
	t_step		move;
	float		mval;
	float		kval;
}	t_sens;

typedef struct s_settings {
	t_step		fov;
	t_step		size;
	t_step		color;
	t_sens		sens;
}	t_settings;

typedef struct s_intrfc {
	t_settings	settings;
	t_res		campos;
	t_res		objpos;
	t_res		frame;
	t_button	arrows[24];
	t_button	attach;
	t_obj		*selected;
	t_data		cam;
	t_data		obj;
}	t_intrfc;

typedef struct s_info
{
	void		*mlx_ptr;
	t_win		win;
	t_light		a_light;
	t_list		*objects;
	t_list		*lights;
	t_rot		rot;
	t_data		data;
	char		*prog;
	char		*camtext;
	t_mouse		mouse;
	t_keybrd	keybrd;
	t_intrfc	interface;
}	t_info;

typedef struct s_import {
	t_list	*v;
	t_list	*vn;
	t_list	*vt;
	t_list	*f;
}	t_import;


// Parsing the file.

char	*avoidonelinevrtxs(t_vrtx vrtxs[3], char *line, t_import *imp);
void	centroiddefiner(t_cart *dots, int dotsnum);
void	crdstmdefiner(t_crdstm *crdstm);
float	cylinderparser(char *str, t_obj *obj, char *prog, void *mlx);
void	checkvrtxsnorms(t_vrtx vrtxs[3], t_cart *polynorm);
int		file_check(char *file, char *prog, t_bool scenefile);
void	ft_read_information(int fd, t_info *info);
t_cart	*getcart(t_list *v, int indx);
char	*getfilename(char *start, char *end);
void	modelparser(int fd, t_import *imp);
char	*vrtxparser(char *line, t_list *vt, t_list *vn, t_vrtx *vrtx);
char	*notendedline(char *line);
float	objparser(char *line, t_obj *obj, char *prog, void *mlx);
void	planeparser(char *str, t_obj *obj, char *prog, void *mlx);
int		primitivedefiner(char *str);
float	sphereparser(char *str, t_obj *obj, char *prog, void *mlx);
void	txtrparsing(char *str, t_data *txtr, void *mlx, t_bool *checkerboard);

// Parsing utilities.

float	ft_atof(const char *num);
int		ft_clear_char_array(char **array);
char	*ft_get_color_values(char *str, t_cart *color, char *prog);
char	*ft_get_position_values(char *prog, char *str, t_cart *pos);
char	*skipnumnspaces(char *str, t_bool onlyspaces);

/*
We can modify atoi so it checks whether the string ends with the number. 
So no 10.1 in simple integers.
*/

// Object-like elements information.

int		circledotsfiller(t_cart *dots, float radius, t_axis *rotcircle, t_bool skippols);
float	cylinderbuilder(t_dots *dots, t_polys *polys, float radius, float height);
void	cylindermapping(t_cart *dots, t_poly *polys, int polynum);
void	definecylindervrtxs(t_cart *dots, t_poly *polys, int polynum);
void	definespherevrtxs(t_cart *dots, t_poly *polys, int polynum);
void	definepols(t_cart *dots, float radius, t_axis *rotcircle);
void	polarjointing(t_cart *dots, t_poly *poly, void *txtr, int dotnum);
void	polarsurfing(t_cart *dots, t_poly **poly, int lttd, void *txtr);
void	repairspherenormal(t_poly *poly, int dotindxs[3], t_cart *dots, void *txtr);
float	spherebuilder(t_dots *dots, t_polys *polys, float radius);
void	spheremapping(t_poly *polys, int polynum);
void	surfing(t_poly *poly, int *dotindxs, t_cart *dots, void *txtr);
int 	ft_fill_cylinder_info(char **piece, t_info *info);
int 	ft_fill_sphere_info(char **piece, t_info *info);
int		ft_fill_plane_info(char **piece, t_info *info);

// Main execution utilities.

int		ft_exit(void);
int		ft_clear_info(t_info *info);
int		ft_key_hook(int keycode, t_info *info);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

// Orientation and movement in space

void	camfromobjcrdstm(t_crdstm *cam, t_attach *attached);
void	camrotating(t_camera *camera, t_info *info, int x, int y);
void	camshifting(t_camera *camera, t_info *info, t_cart *objsdir, float step);
void	crdstmrotbyaxis(t_crdstm *crdstm, t_axis *zaxis, t_axis *xyaxis);
void	dotcrdstmtrnsltn(t_cart *src, t_cart *dst, t_cart *scale, t_crdstm *crdstm);
void	dottranslation(t_cart *dot, t_cart *direction, float step);
void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm, float *outframe);
void	objtoobjpos(t_cart *center, t_cart *dot);
void	rotateattached(t_cart *dir, t_axis *axis, t_info *info);
void	quartrot(t_cart *pos, t_axis *axis);

// Matrix

void	worldtocammatrix(float transmatrix[4][4], float crdstmmatrix[4][4], float posmatrix[4][4], t_crdstm *cam);
void	transpos(t_cart	*pos, float transmatrix[4][4]);

// Vector utils

void	axisbuilder(t_cart *v1, t_cart *v2, t_axis *axis);
void	cartbuilder(float x, float y, float z, t_cart *dot);
void	cartcopy(t_cart *src, t_cart *dst, int count);
void	vectodot(t_cart *vector, t_cart *start);
void	vectorbuilder(float x, float y, float z, t_axis *vector);
float	vectorlength(t_cart *dot);
void	vectorsizing(float newlength, t_cart *src, t_cart *vecres, float *lngthres);
void	negativevector(t_cart *dot);
void	normbuilder(t_cart *centraldot, t_cart *dot1, t_cart *dot2, t_cart *norm);

// View constructor

void	cornerbuilder(t_cart corners[CRNRS], t_res *wincntr, float focus);
void	createcamobjs(t_list **camobjs, t_list *objs, t_cart **lightpos, int lightcount);
void	createframerouts(t_list *objs);
void	initview(t_list *objs, t_camera *camera, t_list *lights);
void	framepainter(t_dots *dots, float focus, t_data *img, t_ui color);
void	framepic(t_win *win, t_bool normalprint, t_list *camobjs, t_data *img);
void	normpainter(t_obj *obj, float focus, t_data *img);
t_bool	objinframe(t_obj *obj, t_res *winctr, float focus);
void	paintline(t_cart src[2], t_ui color, float focus, t_data *img);
void	planeframing(t_obj *plane, t_camera *camera, t_data *img);

// Interface

t_bool	attachobj(t_camera *camera, t_obj *camobj, t_list *objs);
t_bool	changecart(int arrow, t_cart *param, float bounds[2], t_step *step);
t_bool	changeparams(int x, int y, t_intrfc *intrfc, t_win *win);
t_bool	inbounds(t_button btn, int x, int y);
void	initinterface(t_intrfc *interface, void *mlx, t_res *win);
void	interfacebuilder(t_info *info);
t_bool	resizeobj(int arrow, t_intrfc *intrfc);
void	roundselected(t_cart *pos, float outframe, t_win *win, void *mlx);
t_obj	*selectobject(t_list *camobjs, t_cart *vec);

// Hooks for orientation and movement in space

void	keyaxisbuilder(int keycode, t_cart *axis);
void	keydirbuilder(int keycode, t_cart *dir);
void	keyshifting(t_cart *dir, t_cart *axis, t_info *info);
void	scrolling(int btn, t_info *info);
int		mousemove(int x, int y, t_info *info);
void	mouserotating(t_info *info, int x, int y);
void	mouseshifting(t_info *info, int x, int y);
void	mousezooming(t_info *info, int y);
int		keydownhndlr(int keycode, t_info *info);
int		keyuphndlr(int keycode, t_info *info);
int		btnup(int btn, int x, int y, t_info *info);

//Utils

t_bool	comparef(float num, float ref, float interval);
void	customerr(char *prog, char *txt, t_bool infile);
int		error_handler(char *prog, char *place, int funcres);
t_light	*lightcast(t_list *lst);
t_obj	*objcast(t_list *lst);

// MyFUncs

t_ui	ft_create_trgb(int t, int r, int g, int b);
int		get_r(int trgb);
int		get_g(int trgb);
int		get_b(int trgb);
void	ft_draw_screen(t_info *info);

float	ft_max(float a, float b);
void	ft_vectnorm(t_cart *vector);
void	ft_vectlen(t_cart *vect, float *dest);
void	ft_invvect(t_cart *vect, t_cart *dest);
void	ft_dotprod(t_cart *vect_a, t_cart *vect_b, float *dest);
void	ft_subvects(t_cart *vect_a, t_cart *vect_b, t_cart *dest);
void	ft_multvect(t_cart *vect, float multiplier, t_cart *dest);
void	ft_multvects(t_cart *vect_a, t_cart *vect_b, t_cart *dest);
void	ft_summvects(t_cart *vect_a, t_cart *vect_b, t_cart *dest);
void	ft_crossprod(t_cart *vect_a, t_cart *vect_b, t_cart *dest);

void	ft_cast_ray(t_ray *ray, t_cart *direction, t_cart *origin);
void	ft_shadowing(unsigned int *color, t_cart *phit, t_cart norm_colr[2], t_info *info);

float	ft_hit_sphere(t_ray ray, t_obj *sphere);
int		ft_hit_triangle(t_cart phit, t_poly *poly, t_cart *dots, float k[3]);
void	ft_hit_poly(t_ray ray, t_cart *c_norm_colr, t_obj *object, float *c_dist);
void	ft_hit_pplane(t_ray ray, t_cart *norm_vector, t_cart *pos, float *closest_distance);
void	ft_hit_plane(t_ray ray, t_cart *norm_colr, t_obj *object, float *closest_distance);

#endif
