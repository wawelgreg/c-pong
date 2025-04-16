#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



#define SCREEN_WIDTH 85
#define SCREEN_HEIGHT 25
#define PADDLE_WIDTH 5
#define LEN (((SCREEN_WIDTH + 1) * SCREEN_HEIGHT) + 1)

typedef struct ball {
	float x;
	float y;
	float x_v;
	float y_v;
	int row;
	int col;
} Ball;

typedef struct player {
	int row;
	unsigned int score;
	int up_key;
	int down_key;
} Player;

Ball b;
Player p_one;
Player p_two;
char screen[LEN] = { '\0' };

void draw_frame(char screen[], int max_h, int max_w, int len);
void xy_to_colrow(Ball* ball_ptr);
void draw_ball(Ball* ball_ptr, int max_h, int max_w, char screen[]);
void update_ball_coords(Ball* ball_ptr, int max_h, int max_w);
void take_player_input(Player* p_ptr);

int main() {
	b.x = SCREEN_WIDTH / 2.0;
	b.y = SCREEN_HEIGHT / 2.0;
	b.x_v = 0.32;
	b.y_v = 0.1;
	b.row = 0;
	b.col = 0;
	struct Ball* b_ptr = &b;

	p_one.score = 0;
	p_one.row = (int)ceil(SCREEN_HEIGHT / 2);
	struct Player* p_one_ptr;

	p_two.score = 0;
	p_two.row = (int)ceil(SCREEN_HEIGHT / 2);
	struct Player* p_two_ptr;

	while (1) {
		// Draw border to screen string
		draw_frame(screen, SCREEN_HEIGHT, SCREEN_WIDTH, LEN);

		// Take user one and two input


		// Draw player paddle locations

		// Convert ball x/y -> row/col
		xy_to_colrow(b_ptr);
		// Draw ball to screen string
		draw_ball(b_ptr, SCREEN_HEIGHT, SCREEN_WIDTH, screen);
		// Print string on terminal
		printf("%s\n", screen); // Actual draw on terminal
		// Calculate new ball position
		update_ball_coords(b_ptr, SCREEN_HEIGHT, SCREEN_WIDTH);

		sleep:
		#ifdef _WIN32
		Sleep(10);
		#else
		usleep(10);  /* sleep for some milliSeconds */
		#endif

		/*#ifdef _WIN32
		system("cls");
		#else
		system("clear");
		#endif*/
		printf("\033[0;0H");
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
				screen[i++] = ' '; // Previously '#'
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
	ball_ptr->col = (int)round(ball_ptr->x)-1;
	ball_ptr->row = (int)round(ball_ptr->y)-1;
	printf("[col%4d row%4d x %4.2f y %4.2f]\n", (int)ball_ptr->col, (int)ball_ptr->row, ball_ptr->x, ball_ptr->y);
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

void update_ball_coords(Ball* ball_ptr, int max_h, int max_w) {
	if (ball_ptr->y + ball_ptr->y_v >= max_h - 1) {
		ball_ptr->y_v *= -1.0;
	}
	else if (ball_ptr->y + ball_ptr->y_v <= 2) {
		ball_ptr->y_v *= -1.0;
	}
	else if (ball_ptr->x + ball_ptr->x_v >= max_w - 1) {
		ball_ptr->x_v *= -1.0;
	}
	else if (ball_ptr->x + ball_ptr->x_v <= 2) {
		ball_ptr->x_v *= -1.0;
	}

	ball_ptr->x += ball_ptr->x_v;
	ball_ptr->y += ball_ptr->y_v;
}

void take_player_input(Player* p_ptr) {

}