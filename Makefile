NAME		=	ft_script
SRCFILES	=	exit.c \
				file_op.c \
				main.c \
				pty.c \
				record.c \
				record_done.c \
				record_input.c \
				record_output.c \
				record_pre.c \
				record_shell.c \
				record_wait.c \
				record_write.c \
				replay.c \
				replay_start_end.c \
				term.c

OBJFILES	=	$(SRCFILES:%.c=%.o)
SRC			=	$(addprefix src/,$(SRCFILES))
OBJ			=	$(addprefix obj/,$(OBJFILES))
CC			=	gcc
CFLAGS		+=	-O2 -Wextra -Wall -Werror
INC			=	-I libft/inc -I inc/
LIBFT		=	libft/libft.a

all:$(NAME)

$(NAME): $(OBJ) | $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT)

libft/libft.a:
	make -C libft/

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

norminette:
	norminette src/
	norminette inc/
	norminette libft/src/
	norminette libft/inc

norme: norminette

norm: norminette

