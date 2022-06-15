/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 14:58:29 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/15 22:07:14 by dcelsa           ###   ########.fr       */
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
# define KEY_UP 126
# define KEY_LEFT 123
# define KEY_DOWN 125
# define KEY_RIGHT 124
# define KEY_ESC 53
# define KEY_CMND 259
# define KEY_SHIFT 257
# define KEY_SPACE 49
# define KEY_CNTRL 256
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
#  define DEFANG 15
# endif

# ifndef FRAMECLR
#  define FRAMECLR 0x00FFFFFF
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

# ifndef INVINP
#  define INVINP "needs one arg: [filename].rt"
# endif

# ifndef INVFILE
#  define INVFILE "Error"
# endif

# ifndef DUPDET
#  define DUPDET "ambient light, lightspot or camera have more than one definition"
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
	t_bool		determined;
}	t_light;

typedef struct s_poly {
	int		dots[3];
	t_cart	*txtr;
	t_cart	srcnorm;
	t_cart	norm;
}	t_poly;

typedef struct s_vrtx {
	t_cart	dot;
	t_cart	norm;
}	t_vrtx;

typedef struct s_dots {
	int		dotsnum;
	t_vrtx	*dots;
	t_vrtx	*pos;
	float	scale;
	int		(*rout)[2];
	int		routsize;
}	t_dots;

typedef struct s_polys {
	int			polynum;
	t_data		*txtr;
	t_poly		*poly;
}	t_polys;

typedef struct s_obj {
	t_dots		dots;
	t_polys		polys;
	t_cart		colrs;
	t_crdstm	crdstm;
	float		outframe;
	t_rot		*rot;
}	t_obj;

typedef struct s_camera {
	t_crdstm	crdstm;
	t_bool		framemod;
	float		focus;
	float		fov;
	t_list		*objs;
	t_rot		*rot;
	t_cart		lightpos;
	t_bool		determined;
	t_cart		corners[CRNRS];
}	t_camera;

typedef struct s_win {
	void		*win;
	t_camera	camera;
	t_res		res;
	t_res		cntr;
	char		*header;
}	t_win;

typedef struct s_mouse {
	int	yshift;
}	t_mouse;

typedef struct s_keybrd {
	t_bool	zrot;
	t_bool	zoom;
	t_bool	focus;
	t_bool	movecam;
	t_bool	legend;
}	t_keybrd;

typedef struct s_info
{
	void		*mlx_ptr;
	t_win		win;
	t_light		a_light;
	t_list		*objects;
	t_light		lights;
	t_rot		rot;
	t_data		data;
	char		*prog;
	t_mouse		mouse;
	t_keybrd	keybrd;
	int			kal;
}	t_info;

// Parsing the file.

void	crdstmdefiner(t_crdstm *crdstm);
float	cylinderparser(char *str, t_obj *obj, char *prog);
int		file_check(char *file, char *prog);
void	ft_read_information(int fd, t_info *info);
void	planeparser(char *str, t_obj *obj, char *prog);
float	sphereparser(char *str, t_obj *obj, char *prog);

// Parsing utilities.

float	ft_atof(const char *num);
int		ft_clear_char_array(char **array);
char	*ft_get_color_values(char *str, t_cart *color, char *prog);
char	*ft_get_position_values(char *prog, char *str, t_cart *pos);
char	*skipnumnspaces(char *str);

/*
We can modify atoi so it checks whether the string ends with the number. 
So no 10.1 in simple integers.
*/

// Object-like elements information.

int		circledotsfiller(t_vrtx *dots, float radius, t_axis *rotcircle, t_bool skippols);
float	cylinderbuilder(t_dots *dots, t_polys *polys, float radius, float height);
void	definepols(t_vrtx *dots, float radius, t_axis *rotcircle);
void	polarjointing(t_vrtx *dots, t_poly *poly, void *txtr, int dotnum);
void	polarsurfing(t_vrtx *dots, t_poly **poly, int lttd, void *txtr);
float	spherebuilder(t_dots *dots, t_polys *polys, float radius);
void	surfing(t_poly *poly, int *dotindxs, t_vrtx *dots, void *txtr);
int 	ft_fill_cylinder_info(char **piece, t_info *info);
int 	ft_fill_sphere_info(char **piece, t_info *info);
int		ft_fill_plane_info(char **piece, t_info *info);

// Main execution utilities.

int		ft_exit(void);
int		ft_clear_info(t_info *info);
int		ft_key_hook(int keycode, t_info *info);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

// Orientation and movement in space

void	camrotating(t_win *win, int x, int y);
void	camshifting(t_camera *camera, t_cart *camdir, t_cart *objsdir, float step);
void	crdstmrot(t_crdstm *crdstm, t_rot *rot, t_cart *start, t_cart *end);
void	crdstmrotbyaxis(t_crdstm *crdstm, t_axis *zaxis, t_axis *xyaxis);
void	dotcrdstmtrnsltn(t_cart *src, t_cart *dst, int scale, t_crdstm *crdstm);
void	dottranslation(t_cart *dot, t_cart *direction, float step);
void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm);
void	objtoobjaxis(t_crdstm *src, t_crdstm *dst, t_rot *rot);
void	objtoobjpos(t_cart *center, t_cart *dot);
void	vrtxtranslation(t_vrtx *vrtxs, int dotnum, t_cart *direction, float step);
void	quartrot(t_cart *pos, t_axis *axis);

// Vector utils

void	axisbuilder(t_cart *v1, t_cart *v2, t_axis *axis);
void	cartbuilder(float x, float y, float z, t_cart *dot);
void	cartcopy(t_cart *src, t_cart *dst, int count);
void	crdstmcopy(t_crdstm *src, t_crdstm *dst);
void	flatanglehandler(t_rot *rot, t_cart *ref);
void	vectodot(t_cart *vector, t_cart *start, t_bool normilize);
void	vectorbuilder(float x, float y, float z, t_axis *vector);
float	vectorlength(t_cart *dot);
void	vectorsizing(float newlength, t_cart *src, t_cart *vecres, float *lngthres);
void	negativevector(t_cart *dot);
void	normbuilder(t_cart *centraldot, t_cart *dot1, t_cart *dot2, t_cart *norm);

// View constructor

void	createcamobjs(t_list **camobjs, t_list *objs);
void	createframerouts(t_list *objs);
void	initview(t_list *objs, t_camera *camera);
void	framepic(t_win *win, t_list *camobjs, t_data *img, void *mlx);
void	paintline(t_cart start, t_cart end, float focus, t_data *img);
void	planeframing(t_obj *plane, t_camera *camera, t_data *img);

// Hooks for orientation and movement in space

void	keyshifting(int keycode, t_info *info);
void	scrolling(int btn, t_info *info);
int		mousemove(int x, int y, t_info *info);
void	mouserotating(t_info *info, int x, int y);
void	mouseshifting(t_info *info, int x, int y);
void	mousezooming(t_info *info, int y);
int		keydownhndlr(int keycode, t_info *info);
int		keyuphndlr(int keycode, t_info *info);
int		btnpress(int btn, int x, int y, t_info *info);
int		btnup(int btn, int x, int y, t_info *info);

//Utils

t_bool	comparef(float num, float ref, float interval);
void	customerr(char *prog, char *txt, t_bool infile);
int		error_handler(char *prog, char *place, int funcres);
t_obj	*objcast(t_list *lst);

// MyFUncs

void	ft_draw_screen(t_info *info);

float	ft_max(float a, float b);
t_cart	ft_inverse_vector(t_cart vect);
float	ft_get_vector_length(t_cart vect);
t_cart	ft_summ_vectors(t_cart vect_a, t_cart vect_b);
t_cart	ft_get_vector_norm(t_cart vector, float length);
float	ft_get_dot_product(t_cart vect_a, t_cart vect_b);
t_cart	ft_multiply_vector(t_cart vect, float multiplier);
t_cart	ft_multiply_vectors(t_cart vect_a, t_cart vect_b);
t_cart	ft_substract_vectors(t_cart vect_a, t_cart vect_b);
t_cart	ft_get_cross_product(t_cart vect_a, t_cart vect_b);

int		ft_find_light(t_cart phit, t_cart norm, t_cart color, t_info *info);

float	ft_get_intersection_with_poly(t_cart ray_dir,
	t_cart ray_orig, t_cart norm_vector, t_cart pos);
float	ft_get_intersection_with_plane(t_cart ray_dir,
			t_cart ray_orig, t_obj *plane);
float	ft_get_intersection_with_sphere(t_cart ray_dir,
			t_cart ray_orig, t_obj *sphere);
int		ft_get_intersection_with_triangle(t_cart p[3],
			t_cart phit, t_cart norm_vector);


#endif
