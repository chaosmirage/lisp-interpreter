CC = clang

FLAGS = -Wall -Wextra -Werror
DEBUGFLAGS = -g $(FLAGS)
OUTDIR = bin

default: lisp.c
	$(CC) $(FLAGS) $^ -o $(OUTDIR)/lisp && $(OUTDIR)/lisp

debug: lisp.c
	$(CC) $(DEBUGFLAGS) $^ -o $(OUTDIR)/lisp && $(OUTDIR)/./lisp
