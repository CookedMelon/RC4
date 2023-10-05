all:
	@cc \
		-std=c99 -pedantic \
		-Wall -Wextra \
		-O2 -funroll-loops \
		RC4.c \
		op.c \
		-o RC4