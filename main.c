#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void coord2Lat(int x, int y, int w, int h, float *longitude, float *latitude)
{
	*longitude = ((float)x / (float)w) * 360.0f - 180.0f;
	*latitude = atan(sinh(M_PI * (1.0f - 2.0f * ((float)y / (float)h)))) * 180.0f / M_PI;
}

void lat2EW(float longitude, float latitude, float *longLat, char *EW)
{
	if (longitude < 0.0f) {
		longLat[0] = -longitude;
		EW[1] = 'W';
	} else {
		longLat[0] = longitude;
		EW[1] = 'E';
	}

	if (latitude < 0.0f) {
		longLat[1] = -latitude;
		EW[0] = 'S';
	} else {
		longLat[1] = latitude;
		EW[0] = 'N';
	}
}

int main()
{
	SDL_Event e;
	SDL_Renderer *renderer;
	SDL_Texture *map, *pin_t, *text_t;
	SDL_Surface *pin, *textSur;
	SDL_Window *window;
	SDL_Rect loc, mapArea, text;
	TTF_Font *sans;
	char msg[255], EW[2];
	float longitude = 90, latitude = 90, longLat[2];
	int x = 0, y = 0, press = 0;
	char customLat[50] = "", lineEnd = '_';

	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	TTF_Init();
	SDL_CreateWindowAndRenderer(991, 818, 0, &window, &renderer);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	map = IMG_LoadTexture(renderer, "projection.jpg");
	pin = IMG_Load("pin.png");
	pin_t = SDL_CreateTextureFromSurface(renderer, pin);
	sans = TTF_OpenFont("Sans.ttf", 24);
	SDL_StartTextInput();

	SDL_Color textColor = { 255, 255, 255, 0 };
	lat2EW(longitude, latitude, longLat, EW);
	sprintf(msg, "Latitude: %0.3f%c Longitude: %0.3f%c", longLat[1], EW[0], longLat[0], EW[1]);

	textSur = TTF_RenderText_Solid(sans, msg, textColor);
	text_t = SDL_CreateTextureFromSurface(renderer, textSur);

	loc.w = 90;
	loc.h = 90;
	loc.x = x - 45;
	loc.y = y - 81;
	mapArea.w = 991;
	mapArea.h = 768;
	mapArea.x = 0;
	mapArea.y = 0;
	text.x = 0;
	text.y = 774;
	text.w = 550;
	text.h = 30;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	while (1) {
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, map, NULL, &mapArea);
		SDL_RenderCopy(renderer, pin_t, NULL, &loc);
		SDL_RenderCopy(renderer, text_t, NULL, &text);

		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&e) && e.type == SDL_QUIT)
			break;

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&x, &y);

			if (y <= 768) {
				SDL_RenderFillRect(renderer, &text);
				coord2Lat(x, y, 991, 768, &longitude, &latitude);
				lat2EW(longitude, latitude, longLat, EW);
				sprintf(msg, "Latitude: %0.3f%c Longitude: %0.3f%c", longLat[1], EW[0], longLat[0], EW[1]);

				SDL_FreeSurface(textSur);
				textSur = TTF_RenderText_Solid(sans, msg, textColor);
				SDL_DestroyTexture(text_t);
				text_t = SDL_CreateTextureFromSurface(renderer, textSur);

				loc.x = x - 45;
				loc.y = y - 81;
			} else {
				SDL_Point in = { x, y };
				if (SDL_PointInRect(&in, &text)) {
					if (!press) {
						press++;
						printf("%s\n", customLat);
						*customLat = '\0';
					} else
						press--;

				}
			}
		}

		if (e.type == SDL_TEXTINPUT && press) {
			strncat(customLat, e.text.text, sizeof(customLat) - strlen(customLat) - 1);
			sprintf(msg, "Input Latitude, Longitude: %s%c\n", customLat, lineEnd);
			SDL_FreeSurface(textSur);
			textSur = TTF_RenderText_Solid(sans, msg, textColor);
			SDL_DestroyTexture(text_t);
			text_t = SDL_CreateTextureFromSurface(renderer, textSur);

		}

		SDL_Delay(10);
	}

	SDL_DestroyTexture(text_t);
	TTF_CloseFont(sans);
	SDL_DestroyTexture(pin_t);
	SDL_DestroyTexture(map);
	SDL_FreeSurface(textSur);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
