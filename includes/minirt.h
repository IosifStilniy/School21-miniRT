/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 14:58:29 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/06 18:28:35 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include "../mlx/mlx.h"
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

# define X_EVENT_KEY_EXIT 17
# define X_EVENT_KEY_PRESS 2
# define X_EVENT_MOUSE_PRESS 4

# define ESC_TEXT "\033[92mClosing MiniRT.\033[0m"
# define ERR_TEXT "Error"

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
}				t_data;

typedef struct s_info
{
	void		*mlx_ptr;
	void		*wnd_ptr;
	t_amlight	*a_light;
	t_camera	*camera;
	t_obj		*object;
	t_light		*light;
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

#endif
