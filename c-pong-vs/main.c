#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define SLEEP_MS 10
#define SCREEN_WIDTH 60
#define SCREEN_HEIGHT 25
#define PADDLE_WIDTH 10
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
	unsigned int score;
	int row;
	int col;
	unsigned int paddle_width;
	int paddle_char;
	int up_key;
	int down_key;
} Player;

void draw_frame(char screen[], int max_h, int max_w, int len);
void xy_to_colrow(Ball* ball_ptr);
void draw_ball(Ball* ball_ptr, int max_h, int max_w, char screen[]);
void update_ball_coords(Ball* ball_ptr, int max_h, int max_w);
void take_player_input(Player* p_ptr, int max_h);
void draw_paddle(char screen[], Player* p_ptr);
void print_player_details(Player* p_one_ptr, Player* p_two_ptr);

int main() {
	Ball b = {
		SCREEN_WIDTH / 2.0,				// x
		SCREEN_HEIGHT / 2.0,			// y
		0.32,							// x_v
		0.1,							// y_v
		0,								// row
		0								// col
	};

	Player p_one = {
		0,								// score
		(int)ceil(SCREEN_HEIGHT / 2),	// row
		0,								// col
		PADDLE_WIDTH,					// paddle width
		'|',							// paddle character
		'w',							// up_key
		's'								// down_key
	};

	Player p_two = {
		0,								// score
		(int)ceil(SCREEN_HEIGHT / 2),	// row
		SCREEN_WIDTH-1,					// col
		PADDLE_WIDTH,					// paddle width
		'|',							// paddle character
		72,								// up_key
		80								// down_key
	};

	struct Ball* b_ptr = &b;
	struct Player* p_one_ptr = &p_one;
	struct Player* p_two_ptr = &p_two;

	char screen[LEN] = { '\0' };		// Game frame string
	
	while (1) {
		// Draw border to screen string
		draw_frame(screen, SCREEN_HEIGHT, SCREEN_WIDTH, LEN);

		// Take user one and two input
		take_player_input(p_one_ptr, SCREEN_HEIGHT);
		take_player_input(p_two_ptr, SCREEN_HEIGHT);

		// Draw player paddle locations
		draw_paddle(screen, p_one_ptr);
		draw_paddle(screen, p_two_ptr);
		
		// Display player info
		print_player_details(p_one_ptr, p_two_ptr);

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
		Sleep(SLEEP_MS);
		#else
		usleep(SLEEP_MS);  /* sleep for some milliSeconds */
		#endif

		// Move cursor to beginning of terminal
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

void take_player_input(Player* p_ptr, int max_h) {
	int key_code = 0;
	if (_kbhit()) {
		key_code = _getch();
		if (key_code == p_ptr->up_key && p_ptr->row - floor(p_ptr->paddle_width / 2) - 1 > 0) {
			p_ptr->row -= 1;
		}
		else if (key_code == p_ptr->down_key) {
			if (p_ptr->paddle_width % 2 != 0 && p_ptr->row + floor(p_ptr->paddle_width / 2) + 1 < max_h - 1) {
				p_ptr->row += 1;
			}
			else if (p_ptr->row + floor(p_ptr->paddle_width / 2) < max_h - 1) {
				p_ptr->row += 1;
			}
		}
	}
}

void draw_paddle(char screen[], Player* p_ptr) {
	int col = p_ptr->col;
	int row = p_ptr->row - (p_ptr->paddle_width / 2);
	int i = (row * (SCREEN_WIDTH + 1)) + col;

	for (int j = 0; j < p_ptr->paddle_width; ++j) {
		screen[i] = p_ptr->paddle_char;
		i += SCREEN_WIDTH + 1;
	}
}

void print_player_details(Player* p_one_ptr, Player* p_two_ptr) {
	printf("P1: %2d P2: %2d P1_row: %2d P2_row: %2d\n",
		p_one_ptr->score, p_two_ptr->score,
		p_one_ptr->row, p_two_ptr->row);
}