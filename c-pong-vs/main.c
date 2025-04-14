#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define LEN (((SCREEN_WIDTH + 1) * SCREEN_HEIGHT) + 1)

void draw_frame(char screen[], int max_h, int max_w, int len);

char screen[LEN] = { '\0' };

struct Ball {
	float x;
	float y;
	float x_v;
	float y_v;
	int row;
	int col;
};


int main() {
	struct Ball ball;
	ball.x = SCREEN_WIDTH / 2.0;
	ball.y = SCREEN_HEIGHT / 2.0;
	ball.x_v = 0.0;
	ball.y_v = 0.0;
	ball.row = 0;
	ball.col = 0;



	while (1) {
		// Draw border to screen string
		draw_frame(screen, SCREEN_HEIGHT, SCREEN_WIDTH, LEN);
		// Convert ball x/y -> row/col

		// Draw ball to screen string

		// Print string on terminal
		printf("%s", screen); // Actual draw on terminal
		// Calculate new ball position

		//sleep:
		#ifdef _WIN32
		Sleep(100);
		#else
		usleep(100);  /* sleep for 100 milliSeconds */
		#endif

		#ifdef _WIN32
		system("cls");
		#else
		system("clear");
		#endif
	}

}

void draw_frame(char screen[], int max_h, int max_w, int len) {

	int i = 0;
	int row = 0;
	int col = 0;
	while (i < len - 2) {
		if ((row == 0 || row == max_h - 1) && col < max_w) {
			screen[i++] = '#';
			++col;
			continue;
		}

		if ((row != 0 && row != max_h) && col < max_w) {
			if (col == 0 || col == max_w - 1) {
				screen[i++] = '#';
			}
			else {
				screen[i++] = ' ';
			}

			++col;
			continue;
		}

		screen[i++] = '\n';
		col = 0;
		++row;
	}
	screen[i] = '\0';
}

void 