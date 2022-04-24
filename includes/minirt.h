/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 14:58:29 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/24 21:20:37 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "get_next_line.h"
# include "libft.h"
# include <mlx.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>

# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_W 13
# define KEY_UP 126
# define KEY_LEFT 123
# define KEY_DOWN 125
# define KEY_RIGHT 124
# define KEY_ESC 53

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

typedef int	t_bool;

typedef struct s_cords
{
	double	x;
	double	y;
	double	z;
}	t_cords;

typedef struct s_colrs
{
	int	r;
	int	g;
	int	b;
}	t_colrs;

typedef struct s_amlight
{
	double	light_ratio;
	t_colrs	color;
}	t_amlight;

typedef struct s_camera
{
	t_cords	position;
	t_cords	vector;
	int		fov;
}	t_camera;

typedef struct s_light
{
	double	light_ratio;
	t_cords	position;
	t_colrs	color;
}	t_light;

typedef struct s_obj
{
	char			*identifier;
	t_cords			position;
	int				diameter;
	int				height;
	t_cords			vector;
	t_colrs			color;
	struct s_obj	*next;
}	t_obj;

typedef struct	s_data {
	void	*img;
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
	int		x;
	int		y;
	int		z;
	t_color	color;
}	t_cart;

typedef struct s_fcart {
	float	x;
	float	y;
	float	z;
	t_color	color;
}	t_fcart;

typedef struct s_axis {
	t_cart	vector;
	float	length;
	float	ang;
}	t_axis;

typedef struct s_scale {
	float	def;
	float	cur;
	float	old;
}	t_scale;

typedef struct s_view {
	t_scale		scale;
	t_res		poscrd;
	t_axis		axis;
	float		dstnc;
	float		focus;
	t_bool		legend;
}	t_view;

typedef struct s_shift {
	t_res	crdstm;
	t_res	sum;
}	t_shift;

typedef struct s_img {
	t_data		img;
	t_shift		shift;
	t_res		res;
}	t_img;

typedef struct s_win {
	void	*win;
	t_res	res;
	t_res	cntr;
	t_view	view;
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
	t_bool	legend;
}	t_keybrd;

typedef struct s_info
{
	void		*mlx_ptr;
	void		*wnd_ptr;
	t_amlight	*a_light;
	t_camera	*camera;
	t_obj		*object;
	t_light		*light;
	int			total;
	t_data		data;
}	t_info;

// Parsing the file.

t_info	*ft_validate_file(char *filename);

// Parsing utilities.

int		ft_clear_char_array(char **array);
double	ft_atoidb(const char *num); 

/*
We can modify atoi so it checks whether the string ends with the number. 
So no 10.1 in simple integers.
*/

// Get and format elements information.

int		ft_get_position_values(char *str, t_cords *position);
int		ft_get_vector_values(char *str, t_cords *vector);
int		ft_get_color_values(char *str, t_colrs *color);

// Main elements information.

int		ft_fill_amb_light_info(char **piece, t_info *info);
int		ft_fill_camera_info(char **piece, t_info *info);
int 	ft_fill_light_info(char **piece, t_info *info);

// Object-like elements information.

int 	ft_fill_cylinder_info(char **piece, t_info *info);
int 	ft_fill_sphere_info(char **piece, t_info *info);
int		ft_fill_plane_info(char **piece, t_info *info);

// Main execution utilities.

int		ft_exit(void);
int		ft_clear_info(t_info *info);
int		ft_key_hook(int keycode, t_info *info);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

// Orientation and movement in space

void	axisbuilder(t_axis *v1, t_axis *v2, t_axis *axis);
void	vectorbuilder(float x, float y, float z, t_axis *vector);
void	engine(t_win *win, t_dot *dots);

// Hooks for orientation and movement in space

int		mousemove(int x, int y, t_cntrl *cntrl);
void	keyshifting(int keycode, t_cntrl *cntrl);
void	keyrotating(int keycode, t_cntrl *cntrl);
void	scrolling(int btn, t_cntrl *cntrl);
void	mouserotating(t_cntrl *cntrl, int x, int y);
void	mouseshifting(t_cntrl *cntrl, int x, int y);
void	mousezooming(t_cntrl *cntrl, int y);
int		keydownhndlr(int keycode, t_cntrl *cntrl);
int		keyuphndlr(int keycode, t_cntrl *cntrl);
int		btnpress(int btn, int x, int y, t_cntrl *cntrl);
int		btnup(int btn, int x, int y, t_cntrl *cntrl);

#endif
