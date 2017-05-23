bli: src/mpc.c src/bearlang.c src/bli.c
	gcc -g -o bli $^ -Iinclude/ -lreadline
all: bli
