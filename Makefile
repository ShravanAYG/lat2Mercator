all:	
	gcc main.c -o projection -lSDL2 -lSDL2_image -lSDL2_ttf -Wall -Wextra -lm
	gcc countries.c -o countries -Wall -Wextra
clean:
	rm countries projection

