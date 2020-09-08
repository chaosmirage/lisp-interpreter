CC = clang

NAME = lisp
SRC = $(NAME).c
EXENAME = $(NAME)

FLAGS = -Wall -Wextra

default: runtest

runtest:
	$(CC) test.c $(SRC) -o test && ./test
