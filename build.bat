chcp 65001

gcc -I include -c -o obj/bmp.o src/bmp.c
gcc -I include -c -o obj/io.o src/io.c
gcc -I include -c -o obj/1bpp.o src/1bpp.c
gcc -I include -c -o obj/2bpp.o src/2bpp.c
gcc -I include -c -o obj/4bpp.o src/4bpp.c
gcc -I include -c -o obj/8bpp.o src/8bpp.c
gcc -I include -c -o obj/16bpp.o src/16bpp.c

gcc -I include -c -o obj/main.o main.c

gcc -o bin/bmp-editor obj\bmp.o obj\io.o obj\1bpp.o obj\2bpp.o obj\4bpp.o obj\8bpp.o obj\16bpp.o obj\main.o 