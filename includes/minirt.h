/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 14:58:29 by ncarob            #+#    #+#             */
/*   Updated: 2022/05/21 17:46:35 by dcelsa           ###   ########.fr       */
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
# include <stdio.h>
# include <math.h>

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
#  define RESX 1920
# endif

# ifndef RESY
#  define RESY 1080
# endif

# ifndef OFST
#  define OFST 0.05
# endif

# ifndef SHIFT_SPEED
#  define SHIFT_SPEED 30
# endif

# ifndef DEFANG
#  define DEFANG 15
# endif

# ifndef PRMTVS
#  define PRMTVS "spplcy"
# endif

# ifndef NUMPRMTVS
#  define NUMPRMTVS 3
# endif

# ifndef RNDSGMNTS
#  define RNDSGMNTS 36
# endif

# ifndef INVINP
#  define INVINP "needs one arg: [filename].rt"
# endif

# ifndef INVFILE
#  define INVFILE "Error"
# endif

# ifndef DUPDET
#  define DUPDET "ambient light, light or camera have more than one definition"
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
#  define NUMSPACES "0123456789. \t\v\f\r"
# endif

typedef int	t_bool;

typedef struct	s_data {
	void	*img;
	t_res	res;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_res {
	int	x;
	int	y;
}	t_res;

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

typedef struct s_colrs {
	int	r;
	int	g;
	int	b;
}	t_colrs;

typedef struct s_light {
	double		light_ratio;
	t_colrs		color;
	t_cart		pos;
	t_bool		determined;
}	t_light;

typedef struct s_poly {
	int		dotcount;
	int		*dots;
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
}	t_dots;

typedef struct s_polys {
	int			polynum;
	t_data		*txtr;
	t_poly		*poly;
}	t_polys;

typedef struct s_obj {
	t_dots		dots;
	t_polys		polys;
	t_colrs		colrs;
	t_crdstm	crdstm;
	float		outframe;
	t_rot		*rot;
}	t_obj;

typedef struct s_camera {
	t_cart		pos;
	t_crdstm	crdstm;
	float		focus;
	float		xfov;
	float		yfov;
	t_bool		determined;
	t_list		*camobjs;
	t_bool		*objsinframe;
	t_rot		*rot;
	t_cart		corners[4];
}	t_camera;

typedef struct s_win {
	void		*win;
	t_camera	camera;
	t_res		res;
	t_res		cntr;
	char		*header;
}	t_win;

typedef struct s_trnaxs {
	t_axis	v1;
	t_axis	v2;
}	t_trnaxs;

typedef struct s_mouse {
	t_bool		shift;
	t_bool		rot;
	t_res		pos;
	t_trnaxs	vpos;
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
	t_data		img;
	char		*prog;
	int			total;
	t_mouse		mouse;
	t_keybrd	keybrd;
}	t_info;

// Parsing the file.

void	crdstmdefiner(t_crdstm *crdstm);
float	cylinderparser(char *str, t_obj *obj, char *prog);
int		file_check(char *file, char *prog);
void	ft_read_information(int fd, t_info *info);
void	planeparser(char *str, t_obj *obj, char *prog);
float	sphereparser(char *str, t_obj *obj, char *prog);

// Parsing utilities.

int		ft_clear_char_array(char **array);
double	ft_atoidb(const char *num); 

/*
We can modify atoi so it checks whether the string ends with the number. 
So no 10.1 in simple integers.
*/

// Parse utils.

char	*ft_get_color_values(char *str, t_colrs *color, char *prog);
char	*ft_get_position_values(char *prog, char *str, t_cart *pos);
float	ft_atof(const char *num);
char	*skipnumnspaces(char *str);

// Main elements information.

int		ft_fill_amb_light_info(char **piece, t_info *info);
int		ft_fill_camera_info(char **piece, t_info *info);
int 	ft_fill_light_info(char **piece, t_info *info);

// Object-like elements information.

void	backpsurfpatch(t_vrtx *dots, t_poly *poly, t_bool south, int lttd);
int		circledotsfiller(t_vrtx *dots, float radius, t_axis *rotcircle, t_bool skippols);
float	cylinderbuilder(t_dots *dots, t_polys *polys, float radius, float height);
void	definepols(t_vrtx *dots, float radius, t_axis *rotcircle);
void	frontpsurfpatch(t_vrtx *dots, t_poly *poly, t_bool south, int lttd);
float	spherebuilder(t_dots *dots, t_polys *polys, float radius);
void	surfing(t_poly *poly, int *dotindxs, int dotcount, t_vrtx *dots);
int 	ft_fill_cylinder_info(char **piece, t_info *info);
int 	ft_fill_sphere_info(char **piece, t_info *info);
int		ft_fill_plane_info(char **piece, t_info *info);

// Main execution utilities.

int		ft_exit(void);
int		ft_clear_info(t_info *info);
int		ft_key_hook(int keycode, t_info *info);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

// Orientation and movement in space

void	crdstmrot(t_crdstm *crdstm, t_rot *rot, t_cart *start, t_cart *end);
void	crdstmrotbyaxis(t_crdstm *crdstm, t_axis *zaxis, t_axis *xyaxis);
void	crdstmtranslation(t_cart *crdstm, t_cart *direction, float step);
void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm);
void	objrot(t_obj *camobj, t_crdstm *cam, t_crdstm *obj, t_cart *dst);
void	objtoobjaxis(t_crdstm *src, t_crdstm *dst, t_rot *rot);
void	objtoobjpos(t_cart *center, t_cart *dot);
void	quartrot(t_cart *pos, t_axis *axis);

// Vector utils

void	axisbuilder(t_cart *v1, t_cart *v2, t_axis *axis);
void	cartbuilder(float x, float y, float z, t_cart *dot);
void	cartcopy(t_cart *src, t_cart *dst, int count);
void	crdstmcopy(t_crdstm *src, t_crdstm *dst);
void	flatanglehandler(t_rot *rot, t_cart *ref);
void	vectodot(t_cart *vector, t_cart *start, t_bool normilize);
void	vectorbuilder(float x, float y, float z, t_axis *vector);
void	vectorsizing(float newlength, t_cart *src, t_cart *vecres, float *lngthres);
void	vectortoobj(t_cart *from, t_cart *to, t_axis *vector);
void	negativevector(t_cart *dot);
void	normbuilder(t_cart *centraldot, t_cart *dot1, t_cart *dot2, t_cart *norm);

// View constructor

void	createcamobjs(t_list **camobjs, t_list *objs, t_bool **objsinframe);
void	createview(t_list *objs, t_camera *camera, t_res *wincntr);

// Hooks for orientation and movement in space

int		mousemove(int x, int y, t_info *info);
void	keyshifting(int keycode, t_info *info);
void	keyrotating(int keycode, t_info *info);
void	scrolling(int btn, t_info *info);
void	mouserotating(t_info *info, int x, int y);
void	mouseshifting(t_info *info, int x, int y);
void	mousezooming(t_info *info, int y);
int		keydownhndlr(int keycode, t_info *info);
int		keyuphndlr(int keycode, t_info *info);
int		btnpress(int btn, int x, int y, t_info *info);
int		btnup(int btn, int x, int y, t_info *info);

//Utils

t_cart	*cartcast(t_list *lst);
t_bool	comparef(float num, float ref, float interval);
void	customerr(char *prog, char *txt, t_bool infile);
int		error_handler(char *prog, char *place, int funcres);
t_obj	*objcast(t_list *lst);

#endif
