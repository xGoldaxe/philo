SRC = main.c \
ft_atoi.c \
routine.c \
new_usleep.c \
parsing.c \
mutex_operations.c \

CC = gcc

HEADER = philo.h

OBJ = $(SRC:%.c=./.build/%.o)

NAME = philo

RM = rm -rf

LIBC = ar -rc

CFLAGS = -Wall -Wextra -Werror

# COLORS
NONE			= \033[0m
CL_LINE			= \033[2K
B_RED			= \033[1;31m
B_GREEN			= \033[1;32m
B_MAGENTA 		= \033[1;35m
B_CYAN 			= \033[1;36m

./.build/%.o : %.c
	@$(CC) ${CFLAGS} -I. -o $@ -c $?
	@printf "${B_MAGENTA}Compilling $? ...\n${NONE}"

all :	
	@mkdir -p .build
	@make ${NAME}

${NAME} : $(OBJ)
	${CC} -o ${NAME} ${OBJ} -lpthread -D_REENTRANT
	@printf "${B_GREEN}==>{${NAME}} LINKED SUCCESFULLY<==${NONE}\n"

clean :
	@${RM} .build
	@printf "${B_RED}XX{${NAME}} CLEANED SUCCESFULLY XX${NONE}\n"

fclean :	clean
	@${RM} ${NAME}
	@printf "${B_RED}XX{${NAME}} FCLEAN SUCCESFULL XX${NONE}\n"

re :	fclean all