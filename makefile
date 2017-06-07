uber: main.c
	gcc main.c -std=c11 -lX11 -lcairo -o uber

debug:
	gcc -g main.c -std=c11 -lX11 -lcairo -o debug
	gdb ./debug

install: uber
	mv uber /usr/bin/uber
