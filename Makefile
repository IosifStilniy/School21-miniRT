SRCS	=	sources/miniRT_main.c \
			sources/miniRT_parser.c \
			sources/miniRT_parser_utils.c \
			sources/miniRT_elements_objs.c \
			sources/miniRT_elements_main.c \
			get_next_line/get_next_line.c \
			get_next_line/get_next_line_utils.c \

OBJS	=	$(SRCS:.c=.o)

B_OBJS	=	$(B_SRCS:.c=.o)

HEADS	=	./includes/

NAME	=	miniRT

LNAME	=	libft/libft.a

MLBNAME	=	mlx/libmlx.a

GCC		=	gcc

FLAGS	=	-Wall -Werror -Wextra -Imlx -g # -fsanitize=address

RM		=	rm -f

%.o: %.c
	$(GCC) $(FLAGS) -c $< -o $@ 

$(NAME): $(OBJS)
	$(MAKE) -C libft all
	$(MAKE) -C mlx all
	$(GCC) $(FLAGS) $(OBJS) $(LNAME) $(MLBNAME) -lz -framework OpenGL -framework AppKit -o $(NAME)

$(B_NAME): $(B_OBJS)
	$(MAKE) -C libft all
	$(MAKE) -C mlx all
	$(GCC) $(FLAGS) $(B_OBJS) $(LNAME) $(MLBNAME) -lz -framework OpenGL -framework AppKit -o $(B_NAME)

all:	$(NAME)

bonus:	$(B_NAME)
	
clean:
		$(MAKE) -C libft clean
		$(MAKE) -C mlx clean
		$(RM) $(OBJS) $(B_OBJS)

fclean: clean
		$(RM) $(LNAME)
		$(RM) $(NAME)
		$(RM) $(B_NAME)
		$(RM) $(MLBNAME)

re:		fclean all