//#include <bluetooth/bluetooth.h>
//#include <bluetooth/rfcomm.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
//#include <sys/socket.h>
//#include <unistd.h>

const char dest[18] = "01:23:45:67:89:AB";

int s, status = 0;

typedef enum {
    ON,
    OFF,
    BRAKE,
    FORWARDS,
    BACKWARDS,
    SLOW_TURN_LEFT,
    SHARP_TURN_LEFT,
    SLOW_TURN_RIGHT,
    SHARP_TURN_RIGHT,
} State;

State indices_to_state(int i, int j) {
	return (i * 3) + j;
}

char *state_to_str(State state) {
	switch (state) {
		case ON: return "ON";
		case OFF: return "OFF";
		case BRAKE: return "BRAKE";
		case FORWARDS: return "FORWARDS";
		case BACKWARDS: return "BACKWARDS";
		case SLOW_TURN_LEFT: return "SLOW_TURN_LEFT";
		case SHARP_TURN_LEFT: return "SHARP_TURN_LEFT";
		case SLOW_TURN_RIGHT: return "SLOW_TURN_RIGHT";
		case SHARP_TURN_RIGHT: return "SHARP_TURN_RIGHT";
	}
	return "NONE";
}

State state = OFF;

static inline bool send_state() {
	/*
	if (status == 0)
		status = write(s, &state, 1);
	else if (status < 0)
		fprintf(stderr, "Server gone down");
	*/
	printf("NEW STATE: %s\n", state_to_str(state));
	return status >= 0;
}

const int btn_count = 3, btn_len = 250, delay = 15;

int main() {
	SDL_Rect buttons[btn_count][btn_count];
	for (int i = 0; i < btn_count; ++i) {
		for (int j = 0; j < btn_count; ++j) {
			buttons[i][j].h = btn_len;
			buttons[i][j].w = btn_len;
			buttons[i][j].y = (btn_len * i) + (20 * (i+1));
			buttons[i][j].x = (btn_len * j) + (20 * (j+1));
		}
	}
	/*struct sockaddr_rc addr = { 0 };
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    str2ba(dest, &addr.rc_bdaddr);
    status = connect(s, (struct sockaddr *) &addr, sizeof(addr));
	if (status < 0) {
		fprintf(stderr, "Could not connect to %s\n", dest);
		close(s);
		return EXIT_FAILURE;
	}*/
	bool quit = false;
	SDL_Event event;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to init SDL2");
		return EXIT_FAILURE;
	}
	if (TTF_Init() < 0) {
		fprintf(stderr, "Failed to init TTF");
		return EXIT_FAILURE;
	}
	SDL_Window *window = SDL_CreateWindow(
		"RC Car Controller",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(btn_len * btn_count) + (20 * (btn_count+1)),
		(btn_len * btn_count) + (20 * (btn_count+1)),
		0
	);
	if (!window) {
		fprintf(stderr, "Failed to create window\n");
		return EXIT_FAILURE;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		fprintf(stderr, "Failed to init renderer\n");
		return EXIT_FAILURE;
	}
	TTF_Font *font = TTF_OpenFont("/usr/share/fonts/TTF/OpenSans-Regular.ttf", btn_len / 10);
	SDL_Color font_color = { .r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x0D, 0x0D, SDL_ALPHA_OPAQUE);
	SDL_Texture *text;
	SDL_Rect rect;
	for (int i = 0; i < btn_count; ++i) {
		SDL_RenderDrawRects(renderer, buttons[i], 3);
		SDL_RenderFillRects(renderer, buttons[i], 3);
		for (int j = 0; j < btn_count; ++j) {
			text = SDL_CreateTextureFromSurface(
				renderer,
				TTF_RenderText_Solid(font, state_to_str(indices_to_state(i, j)), font_color)
			);
			SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
			rect.y = (btn_len * i) + (20 * (i + 1)) + 10;
			rect.x = (btn_len * j) + (20 * (j + 1)) + 10;
			SDL_RenderCopy(renderer, text, NULL, &rect);
			SDL_DestroyTexture(text);
		}
	}
	SDL_RenderPresent(renderer);
	int32_t x, y;
	while (!quit) {
		if (!SDL_PollEvent(&event)) {
			if (!send_state()) {
				fprintf(stderr, "HC-06 server went down\n");
				quit = true;
			}
			goto delay;
		}
		switch (event.type) {
			case SDL_QUIT: {
				quit = true;
				break;
			} case SDL_MOUSEBUTTONDOWN: {
				x = event.button.x, y = event.button.y;
				SDL_Rect button;
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						button = buttons[i][j];
						if (x >= button.x &&
							y >= button.y &&
							x < button.x + button.w &&
							y < button.y + button.h) {
							state = indices_to_state(i, j);
							if (!send_state()) {
								fprintf(stderr, "HC-06 server went down\n");
								quit = true;
							}
							goto mousedown_handler_break;
						}
					}
				}
				mousedown_handler_break: break;
			} case SDL_MOUSEBUTTONUP: {
				state = OFF;
				break;
			}
		}
		delay: SDL_Delay(delay);
	}
    //close(s);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}
