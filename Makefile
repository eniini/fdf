# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/30 10:20:49 by eniini            #+#    #+#              #
#    Updated: 2021/01/19 14:11:21 by eniini           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf

# compiler
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# sources
SRCDIR = ./src/
OBJDIR = ./obj/
SRC =	main.c \
		events.c \
		read.c \
		draw_logic.c \
		draw_utils.c \
		rotations.c \
		color.c \
		save_state.c \
		create_bmp.c
OBJ = $(addprefix $(OBJDIR),$(SRC:.c=.o))
SRC_LINK = $(addprefix $(SRCDIR),$(SRC))

INC	= -Iincludes/

#libft
FT = ./libft/
FT_LIB = $(addprefix $(FT),libft.a)
FT_INC = -Ilibft/

#minilibx
MLX = ./minilibx-linux/
MLX_LIB_LINUX = $(addprefix $(MLX), libmlx_Linux.a)
MLX_LIB = $(addprefix $(MLX), libmlx.a)

SAVEFILE = .savestate

#linking
ifeq ($(shell uname), Linux)
	LIBRARIES = -L$(FT) -L$(MLX) -lmlx_Linux -lft -lXext -lX11 -lm
else
	LIBRARIES = -L$(MLX) -lmlx -I/usr/local/X11/include -L/usr/X11/lib \
	-lX11 -lXext -framework OpenGL -framework Appkit
endif

.PHONY:	obj all clean fclean re

ifeq ($(shell uname), Linux)
all : obj $(FT_LIB) $(MLX_LIB_LINUX) $(NAME)
else
all : obj $(FT_LIB) $(MLX_LIB) $(NAME)
endif

obj:
	@mkdir -p $(OBJDIR)

$(OBJDIR)%.o:$(SRCDIR)%.c
	@$(CC) $(CFLAGS) $(INC) $(FT_INC) -o $@ -c $<
	@echo "\033[0;32m[fdf] linking object files...\033[0m"

$(FT_LIB) :
	@make --no-print-directory -C $(FT)

$(MLX_LIB_LINUX) :
	@make --no-print-directory -C $(MLX)

$(MLX_LIB) :
	@make --no-print-directory -C $(MLX)

$(NAME) : $(OBJ)
	@$(CC) $(OBJ) $(LIBRARIES) -o $(NAME)
	@echo "\033[0;32m[fdf] binary created!\033[0m"

clean :
	@rm -rf $(OBJDIR)
	@echo "\033[0;32m[fdf] .obj files removed\033[0m"
	@rm  -f $(SAVEFILE)
	@echo "\033[0;32m[fdf] savedata removed\033[0m"
	@make --no-print-directory -C $(FT) clean


fclean : clean
	@rm -f $(NAME)
	@echo "\033[0;32m[fdf] program binary removed\033[0m"
	@make --no-print-directory -C$(FT) fclean
	@make --no-print-directory -C$(MLX) clean
	rm -f $(MLX_LIB)
	rm -f $(MLX_LIB_LINUX)
	@echo "\033[0;32m[fdf] MLX archive removed\033[0m"

re : fclean all