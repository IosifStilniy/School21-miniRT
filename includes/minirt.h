/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 14:58:29 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 16:58:02 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "structs.h"

// Parsing the file.

char	*avoidonelinevrtxs(t_vrtx vrtxs[3], char *line, t_import *imp);
void	centroiddefiner(t_cart *dots, int dotsnum);
float	coneparser(char *str, t_obj *obj, char *prog, void *mlx);
void	crdstmdefiner(t_crdstm *crdstm);
float	cylinderparser(char *str, t_obj *obj, char *prog, void *mlx);
void	checkvrtxsnorms(t_vrtx vrtxs[3], t_cart *polynorm);
int		file_check(char *file, char *prog, t_bool scenefile);
void	ft_read_information(int fd, t_info *info);
float	hyperboloidparser(char *str, t_obj *obj, char *prog, void *mlx);
t_cart	*getcart(t_list *v, int indx);
char	*getfilename(char *start, char *end, char *prog, t_bool txtr);
void	modelparser(int fd, t_import *imp);
char	*vrtxparser(char *line, t_list *vt, t_list *vn, t_vrtx *vrtx);
char	*notendedline(char *line);
float	objparser(char *line, t_obj *obj, char *prog, void *mlx);
void	planeparser(char *str, t_obj *obj, char *prog, void *mlx);
void	primitivesbuilder(char *str, t_list **objs, t_info *info,
			t_rot *rot);
float	sphereparser(char *str, t_obj *obj, char *prog, void *mlx);
void	txtrparsing(char *str, t_obj *obj, void *mlx, char *prog);

// Parsing utilities.

float	ft_atof(const char *num);
int		ft_clear_char_array(char **array);
char	*ft_get_color_values(char *str, t_cart *color, char *prog);
char	*ft_get_position_values(char *prog, char *str, t_cart *pos);
float	inverseuv(float uv);
char	*skipnumnspaces(char *str, t_bool onlyspaces);

/*
We can modify atoi so it checks whether the string ends with the number. 
So no 10.1 in simple integers.
*/

// Object-like elements information.

void	buttsurf(int central, int strnum, t_poly *polys, t_cart *dots);
void	buttvrtxing(t_poly *poly, t_data *txtr);
int		circledotsfiller(t_cart *dots, float radius,
			t_axis *rotcircle, t_bool skippols);
float	conebuilder(t_dots *dots, t_polys *polys, float radius, float height);
float	cylinderbuilder(t_dots *dots, t_polys *polys,
			float radius, float height);
void	cylindersurfdefiner(t_poly *polys, t_cart *dots,
			void *txtr, int dotnum);
void	cylindermapping(t_cart *dots, t_poly *polys, int polynum);
void	definecylindervrtxs(t_cart *dots, t_poly *polys,
			int polynum, t_data *txtr);
void	definespherevrtxs(t_cart *dots, t_poly *polys, int polynum);
void	definepols(t_cart *dots, float radius, t_axis *rotcircle);
void	polarjointing(t_cart *dots, t_poly *poly, void *txtr, int dotnum);
void	polarsurfing(t_cart *dots, t_poly **poly, int lttd, void *txtr);
void	repairbackpatch(t_vrtx vrtxs[3]);
void	repairspherenormal(t_poly *poly, int dotindxs[3],
			t_cart *dots, void *txtr);
float	spherebuilder(t_dots *dots, t_polys *polys, float radius);
void	spheremapping(t_poly *polys, int polynum);
void	surfing(t_poly *poly, int *dotindxs, t_cart *dots, void *txtr);
int		ft_fill_cylinder_info(char **piece, t_info *info);
int		ft_fill_sphere_info(char **piece, t_info *info);
int		ft_fill_plane_info(char **piece, t_info *info);

// Main execution utilities.

int		ft_exit(void);
int		ft_clear_info(t_info *info);
int		ft_key_hook(int keycode, t_info *info);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

// Orientation and movement in space

void	camfromobjcrdstm(t_crdstm *cam, t_attach *attached);
void	camrotating(t_camera *camera, t_info *info, int x, int y);
void	camshifting(t_camera *camera, t_info *info,
			t_cart *objsdir, float step);
void	crdstmrotbyaxis(t_crdstm *crdstm, t_axis *zaxis, t_axis *xyaxis);
void	dotcrdstmtrnsltn(t_cart *src, t_cart *dst,
			t_cart *scale, t_crdstm *crdstm);
void	dottranslation(t_cart *dot, t_cart *direction, float step);
void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm, float *outframe);
void	objtoobjpos(t_cart *center, t_cart *dot);
void	rotateattached(t_cart *dir, t_axis *axis, t_info *info);
void	quartrot(t_cart *pos, t_axis *axis);

// Matrix

void	worldtocammatrix(float transmatrix[4][4], float crdstmmatrix[4][4],
			float posmatrix[4][4], t_crdstm *cam);
void	transpos(t_cart	*pos, float transmatrix[4][4]);

// Vector utils

void	axisbuilder(t_cart *v1, t_cart *v2, t_axis *axis);
void	cartbuilder(float x, float y, float z, t_cart *dot);
void	cartcopy(t_cart *src, t_cart *dst, int count);
void	vectodot(t_cart *vector, t_cart *start);
void	vectorbuilder(float x, float y, float z, t_axis *vector);
float	vectorlength(t_cart *dot);
void	vectorsizing(float newlength, t_cart *src,
			t_cart *vecres, float *lngthres);
void	negativevector(t_cart *dot);
void	normbuilder(t_cart *centraldot, t_cart *dot1,
			t_cart *dot2, t_cart *norm);

// View constructor

void	cornerbuilder(t_cart corners[CRNRS], t_res *wincntr, float focus);
void	createcamobjs(t_list **camobjs, t_list *objs,
			t_cart **lightpos, int lightcount);
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

void	ft_checker_poly(t_cart *cuv);
void	my_mlx_get_pixel(t_data *data, t_cart *color);
void	ft_checker_plane(t_cart *ox, t_cart *oy, t_cart *color, t_cart *phit);
void	ft_texture_plane(t_crdstm *vects, t_cart *color,
			t_cart *phit, t_data *data);

void	ft_cast_ray(t_ray *ray, t_cart *direction, t_cart *origin);
void	ft_shadowing(unsigned int *color, t_cart *phit,
			t_cart norm_colr[2], t_info *info);

float	ft_hit_sphere(t_ray ray, t_obj *sphere);
int		ft_hit_triangle(t_cart phit, t_poly *poly, t_cart *dots, float k[3]);
void	ft_hit_poly(t_ray ray, t_cart *c_norm_colr,
			t_obj *object, float *c_dist);
void	ft_hit_pplane(t_ray ray, t_cart *norm_vector,
			t_cart *pos, float *closest_distance);
void	ft_hit_plane(t_ray ray, t_cart *norm_colr,
			t_obj *object, float *closest_distance);

#endif /* MINIRT_H */
