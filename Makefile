SRCS		=	$(wildcard sources/*.c)			\
				$(wildcard sources/*/*.c)			\
				$(wildcard get_next_line/*.c)	\

OBJS		=	$(SRCS:.c=.o)

B_OBJS		=	$(B_SRCS:.c=.o)

HEADS		=	includes		\
				get_next_line	\

NAME		=	miniRT

FRAMEWORKS	=	-framework OpenGL -framework AppKit

LIBNAME		=	libft.a

LIBPATH		=	libft

LIB			=	$(addprefix $(addsuffix /, $(LIBPATH)), $(LIBNAME))

MLX			=	mlx

GCC			=	gcc

FLAGS		=	-Wall -Werror -Wextra -Imlx -I$(HEADS) #-g -fsanitize=address

RM			=	rm -f

%.o:	%.c
		$(GCC) $(FLAGS) -c $< -o $@ 

$(NAME):	$(HEADS) $(LIB) $(OBJS)
			$(GCC) $(FLAGS) $(OBJS) $(LIB) -L$(MLX) -l$(MLX) $(FRAMEWORKS) -o $(NAME)

$(B_NAME):	$(B_HEADS) $(LIB) $(B_OBJS)
			$(GCC) $(FLAGS) $(B_OBJS) $(LIB) -L$(MLX) -l$(MLX) $(FRAMEWORKS) -o $(B_NAME)

$(LIB):		lib

lib:		
			@$(MAKE) -C $(LIBPATH)
			@$(MAKE) -C $(MLX)
			cp $(MLX)/libmlx.dylib ./

all:	$(NAME)

bonus:	$(B_NAME)
	
clean:
		@$(MAKE) -C $(LIBPATH) clean
		$(RM) $(OBJS) $(B_OBJS)

fclean: clean
		@$(MAKE) -C $(MLX) clean

re:		fclean all

.PHONY:	all clean fclean lib bonus
