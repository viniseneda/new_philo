SRCS	= philo.c thread_data.c atoi.c
OBJS	= ${SRCS:.c=.o}

CC	= cc -g3

CFLAGS = -Wall -Werror -Wextra

NAME	= philo

.c.o:
	${CC} ${CFLAGS} -c ${SRCS}

RM	= rm -f

$(NAME):	${OBJS}
	cc -o $(NAME) ${OBJS} -lpthread

all:	$(NAME)

clean:
	${RM} ${OBJS}

fclean:	clean
	${RM} ${NAME}

re:	fclean all

.PHONY: 	all clean fclean
