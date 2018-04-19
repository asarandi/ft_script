NAME		=	ft_script
SRCFILES	=	main.c file_op.c
OBJFILES	=	$(SRCFILES:%.c=%.o)
SRC			=	$(addprefix src/,$(SRCFILES))
OBJ			=	$(addprefix obj/,$(OBJFILES))
CC			=	gcc
CFLAGS		+=	-g -Wextra -Wall #-Werror
INC			=	-I libft/inc -I inc/
LIB			=	-L libft/ -lft

all:$(NAME)

$(NAME): $(OBJ)
	make -C libft/
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

objdir:
	mkdir -p obj/

obj/%.o: src/%.c | objdir
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

rmobj:
	rm -rf obj/

rmbin:
	rm -rf $(NAME)

again: rmobj rmbin all

clean: rmobj
	make clean -C libft/

fclean: clean rmbin
	make fclean -C libft/

re: fclean all
