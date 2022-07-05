/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:23:19 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 15:26:11 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

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
# include "defines.h"

typedef int				t_bool;
typedef unsigned int	t_ui;

typedef struct s_res {
	int	x;
	int	y;
}	t_res;

typedef struct s_data {
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

typedef struct s_ray {
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

#endif /* STRUCTS_H */