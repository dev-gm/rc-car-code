#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>

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

State state = OFF;

bool send_state() {
	if (status == 0)
		status = write(s, &state, 1);
	else if (status < 0)
		fprintf(stderr, "Server gone down");
	return status >= 0;
}

const int btn_len = 100;

const SDL_Rect buttons[3][3] = {
	{
		{btn_len*0, btn_len*0, btn_len, btn_len},
		{btn_len*0, btn_len*1, btn_len, btn_len},
		{btn_len*0, btn_len*2, btn_len, btn_len},
	},
	{
		{btn_len*1, btn_len*0, btn_len, btn_len},
		{btn_len*1, btn_len*1, btn_len, btn_len},
		{btn_len*1, btn_len*2, btn_len, btn_len},
	},
	{
		{btn_len*2, btn_len*0, btn_len, btn_len},
		{btn_len*2, btn_len*1, btn_len, btn_len},
		{btn_len*2, btn_len*2, btn_len, btn_len},
	},
};

int main() {
	struct sockaddr_rc addr = { 0 };
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    str2ba(dest, &addr.rc_bdaddr);
    status = connect(s, (struct sockaddr *) &addr, sizeof(addr));
	if (status < 0) {
		fprintf(stderr, "Could not connect to %s\n", dest);
		close(s);
		return EXIT_FAILURE;
	}
	bool quit = false;
	SDL_Event event;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to init SDL2");
		return EXIT_FAILURE;
	}
	SDL_Window *window = SDL_CreateWindow(
		"RC Car Controller",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		680, 480,
		0
	);
	if (!window) {
		fprintf(stderr, "Failed to create window\n");
		return EXIT_FAILURE;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	while (!quit) {
		for (int i = 0; i < 3; ++i)
			SDL_RenderDrawRects(renderer, buttons[i], 3);
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_QUIT: {
				quit = true;
				break;
			} case SDL_MOUSEBUTTONDOWN: {
				Sint32 x = event.button.x, y = event.button.y;
				SDL_Rect button;
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						button = buttons[i][j];
						if (x >= button.x &&
							y >= button.y &&
							x < button.x + button.w &&
							y < button.y + button.h) {
							state = ((x + 1) * (y + 1)) - 1;
							if (!send_state()) {
								fprintf(stderr, "HC-06 server went down\n");
								quit = true;
							}
						}
					}
				}
			}
		}
	}
    close(s);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
