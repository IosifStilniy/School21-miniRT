SRCS		=	$(wildcard sources/*.c)			\
				$(wildcard sources/*/*.c)			\
				$(wildcard sources/*/*/*.c)			\
				$(wildcard get_next_line/*.c)	\

OBJS		=	$(SRCS:.c=.o)

B_OBJS		=	$(B_SRCS:.c=.o)

HEAD		=	includes

GNL_HEAD	=	get_next_line

NAME		=	miniRT

FRAMEWORKS	=	-framework OpenGL -framework AppKit

LIBNAME		=	libft.a

LIBPATH		=	libft

LIB			=	$(addprefix $(addsuffix /, $(LIBPATH)), $(LIBNAME))

MLX			=	mlx

DINLIB		=	libmlx.dylib

GCC			=	gcc

FLAGS		=	-Wall -Werror -Wextra -Imlx -I$(HEAD) -I$(LIBPATH) -I$(GNL_HEAD) #-g -fsanitize=address

RM			=	rm -f

%.o:	%.c $(HEAD)/minirt.h
		$(GCC) $(FLAGS) -c $< -o $@ 

$(NAME):	$(HEADS) $(LIB) $(OBJS)
			$(GCC) $(FLAGS) $(OBJS) $(LIB) $(DINLIB) $(FRAMEWORKS) -o $(NAME)

$(B_NAME):	$(B_HEADS) $(LIB) $(B_OBJS)
			$(GCC) $(FLAGS) $(B_OBJS) $(LIB)  $(FRAMEWORKS) -o $(B_NAME)

$(LIB):		lib

lib:		
			@$(MAKE) -C $(LIBPATH) bonus
			@$(MAKE) -C $(MLX)
			mv $(MLX)/$(DINLIB) ./

all:	$(NAME)

bonus:	$(B_NAME)
	
clean:
		@$(MAKE) -C $(LIBPATH) clean
		$(RM) $(OBJS) $(B_OBJS)

fclean: clean
		@$(MAKE) -C $(MLX) clean
		rm -f $(DINLIB)
		rm -f $(NAME)

re:		fclean all

.PHONY:	all clean fclean lib bonus
