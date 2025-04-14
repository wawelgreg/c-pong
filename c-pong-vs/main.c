#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



#define SCREEN_WIDTH 81
#define SCREEN_HEIGHT 25
#define LEN (((SCREEN_WIDTH + 1) * SCREEN_HEIGHT) + 1)

typedef struct ball {
	float x;
	float y;
	float x_v;
	float y_v;
	int row;
	int col;
} Ball;
Ball b;
char screen[LEN] = { '\0' };

void draw_frame(char screen[], int max_h, int max_w, int len);
void xy_to_colrow(Ball* ball_ptr);
void draw_ball(Ball* ball_ptr, int max_h, int max_w, char screen[]);

int main() {
	b.x = SCREEN_WIDTH / 2.0;
	b.y = SCREEN_HEIGHT / 2.0;
	b.x_v = 0.0;
	b.y_v = 0.0;
	b.row = 0;
	b.col = 0;

	struct Ball* b_ptr = &b;

	while (1) {
		// Draw border to screen string
		draw_frame(screen, SCREEN_HEIGHT, SCREEN_WIDTH, LEN);
		// Convert ball x/y -> row/col
		xy_to_colrow(b_ptr);
		// Draw ball to screen string
		draw_ball(b_ptr, SCREEN_HEIGHT, SCREEN_WIDTH, screen);
		// Print string on terminal
		printf("%s\n", screen); // Actual draw on terminal
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

void xy_to_colrow(Ball* ball_ptr) {
	ball_ptr->col = (int)round(ball_ptr->x);
	ball_ptr->row = (int)round(ball_ptr->y);
	printf("col %d row %d x %.2f y %.2f\n", (int)ball_ptr->col, (int)ball_ptr->row, ball_ptr->x, ball_ptr->y);
}

void draw_ball(Ball* ball_ptr, int max_h, int max_w, char screen[]) {
	int i = 0;
	int row_count = 0;
	int col_count = 0;

	while (row_count < ball_ptr->row) {
		i += (max_w + 1);
		++row_count;
	}
	i += ball_ptr->col;

	screen[i] = '*';
}