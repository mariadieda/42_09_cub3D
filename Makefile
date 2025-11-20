
NAME=cub3D
SRCS=src/main.c src/rendering.c src/player.c src/clean_up.c src/parsing/parse.c\
	 gnl/get_next_line.c gnl/get_next_line_utils.c
OBJS=${SRCS:.c=.o} 	# Change of file extension
INCL=inc	  		 # File headers
LIBFT_DIR=libft
LIBFT=libft/libft.a
MINLB_DIR=minilibx-linux
MLX=$(MINLB_DIR)/libmlx.a
CC=cc            # Define compiler
RM=rm -f         # Remove file without error, even if it does not exist
CFLAGS=-Wall -Wextra -Werror -g -I$(INCL) -I gnl -I$(MINLB_DIR)  # Define options for compilation #-fsanitize=address for MAC work
LDFLAGS=-L$(LIBFT_DIR) -lft -L$(MINLB_DIR) -lmlx -lXext -lX11 -lm # linker flags for linking stage

# Default target to build the library
all: $(LIBFT) $(MLX) $(NAME)

# Automate the compilation of .c files into .o files including header file(s)
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

$(LIBFT):
	${MAKE} -C ${LIBFT_DIR}

$(MLX):
	$(MAKE) -C $(MINLB_DIR)

# Rule to build the executable
$(NAME): $(OBJS) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

# Remove all object files, cleaning up the intermediate build environment
clean:
	${RM} $(OBJS)
	${MAKE} clean -C $(LIBFT_DIR)

# Full clean, removing executable and object files
fclean: clean
	${RM} $(NAME)
	${MAKE} fclean -C $(LIBFT_DIR)


# Complete recompilation
re: fclean all

.PHONY: libft all clean fclean re
