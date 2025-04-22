#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define SLEEP_MS 20
#define SCREEN_WIDTH 60
#define SCREEN_HEIGHT 25
#define PADDLE_WIDTH 10
#define LEN (unsigned int)(((SCREEN_WIDTH + 1) * SCREEN_HEIGHT) + 1)

enum ownership_state {
	P_ONES_BALL = 1, P_TWOS_BALL
};

enum game_state {
	START = 0, NEW_BALL, POINT_SCORED
};

typedef struct game {
	unsigned int screen_w;
	unsigned int screen_h;
	int game_state;
	unsigned int sleep_ms;
} Game;

typedef struct ball {
	float x;
	float y;
	float x_v;
	float y_v;
	int row;
	int col;
	int ball_ownership;
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

void draw_frame(char screen[], Game* g_ptr, unsigned int len);
void xy_to_colrow(Ball* ball_ptr);
void draw_ball(Ball* ball_ptr, Game* g_ptr, char screen[]);
void check_for_paddle_on_vector(char screen[], Ball* ball_ptr, Game* g_ptr);
void check_for_collision(char screen[], Ball* ball_ptr, Game* g_ptr);
void update_ball_coords(Ball* ball_ptr);
void take_player_input(Player* p_ptr, Game* g_ptr);
void draw_paddle(char screen[], Player* p_ptr, Game* g_ptr);
void print_player_details(Player* p_one_ptr, Player* p_two_ptr);
char get_char_at_rowcol(char screen[], int row, int col, int max_w);

int main() {
	Game g = {
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		START,
		SLEEP_MS
	};

	Ball b = {
		SCREEN_WIDTH / 2.0,				// x
		SCREEN_HEIGHT / 2.0,			// y
		0.32f,							// x_v
		0.1f,							// y_v
		0,								// row
		0,								// col
		P_ONES_BALL
	};

	Player p_one = {
		0,								// score
		(int)ceil(SCREEN_HEIGHT / 2),	// row
		3,								// col
		PADDLE_WIDTH,					// paddle width
		'|',							// paddle character
		'w',							// up_key
		's'								// down_key
	};

	Player p_two = {
		0,								// score
		(int)ceil(SCREEN_HEIGHT / 2),	// row
		SCREEN_WIDTH-4,					// col
		PADDLE_WIDTH,					// paddle width
		'|',							// paddle character
		72,								// up_key
		80								// down_key
	};

	Game* g_ptr = &g;
	Ball* b_ptr = &b;
	Player* p_one_ptr = &p_one;
	Player* p_two_ptr = &p_two;

	char screen[LEN] = { '\0' };		// Game frame string
	srand(time(NULL));

	while (1) {
		switch (g_ptr->game_state)
		{
		case START:
			pick_start_player(g_ptr);
			break;
		case NEW_BALL:
			// Draw border to screen string
			draw_frame(screen, g_ptr, LEN);

			// Take user one and two input
			take_player_input(p_one_ptr, g_ptr);
			take_player_input(p_two_ptr, g_ptr);

			// Draw player paddle locations
			draw_paddle(screen, p_one_ptr, g_ptr);
			draw_paddle(screen, p_two_ptr, g_ptr);

			// Display player info
			print_player_details(p_one_ptr, p_two_ptr);

			// Convert ball x/y -> row/col
			xy_to_colrow(b_ptr);

			// Draw ball to screen string
			draw_ball(b_ptr, g_ptr, screen);

			// Print string on terminal
			printf("%s\n", screen); // Actual draw on terminal

			check_for_paddle_on_vector(screen, b_ptr, g_ptr);

			// Change vector of ball as necessary
			check_for_collision(screen, b_ptr, g_ptr);

			// Calculate new ball position
			update_ball_coords(b_ptr);

			break;
		case POINT_SCORED:
			break;
		default:
			break;
		}

		#ifdef _WIN32
		Sleep(SLEEP_MS);
		#else
		usleep(SLEEP_MS);  /* sleep for some milliSeconds */
		#endif

		// Move cursor to beginning of terminal
		printf("\033[0;0H");
	}

}

void pick_random_ball_direction(Ball* ball_ptr) {
	// rand() % 2 == 0 ? ball_ptr->ball_ownership = P_ONES_BALL : ball_ptr->ball_ownership = P_TWOS_BALL;
}

void draw_frame(char screen[], Game* g_ptr, unsigned int len) {
	unsigned int i = 0;
	unsigned int row = 0;
	unsigned int col = 0;
	while (i < len - 2) {
		if ((row == 0 || row == g_ptr->screen_h - 1) && col < g_ptr->screen_w) {
			screen[i++] = '#';
			++col;
			continue;
		}

		if ((row != 0 && row != g_ptr->screen_h) && col < g_ptr->screen_w) {
			if (col == 0 || col == g_ptr->screen_w - 1) {
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

void draw_ball(Ball* ball_ptr, Game* g_ptr, char screen[]) {
	int i = 0;
	int row_count = 0;
	int col_count = 0;

	while (row_count < ball_ptr->row) {
		i += (g_ptr->screen_w + 1);
		++row_count;
	}
	i += ball_ptr->col;
	
	screen[i] = '0';
}

void check_for_paddle_on_vector(char screen[], Ball* ball_ptr, Game* g_ptr) {
	char projected_vector_char = 
		get_char_at_rowcol(screen, 
			((int)round(ball_ptr->y) - 1), 
			((int)round(ball_ptr->x + ball_ptr->x_v) - 1), 
			g_ptr->screen_w);

	if (projected_vector_char == '|') {
		switch (ball_ptr->ball_ownership) {
		case P_ONES_BALL:
			ball_ptr->ball_ownership = P_TWOS_BALL;
			break;
		case P_TWOS_BALL:
			ball_ptr->ball_ownership = P_ONES_BALL;
			break;
		default:
			break;
		}
		ball_ptr->x_v *= -1.0;
	} 
	printf("Ownership of ball: %d\n", ball_ptr->ball_ownership);
}

char get_char_at_rowcol(char screen[], int row, int col, int max_w) {
	return screen[(row * (max_w + 1)) + col];
}

void check_for_collision(char screen[], Ball* ball_ptr, Game* g_ptr) {
	
	if (ball_ptr->y + ball_ptr->y_v >= g_ptr->screen_h - 1) {
		ball_ptr->y_v *= -1.0;
	}
	else if (ball_ptr->y + ball_ptr->y_v <= 2) {
		ball_ptr->y_v *= -1.0;
	}
	else if (ball_ptr->x + ball_ptr->x_v >= g_ptr->screen_w - 1) {
		ball_ptr->x_v *= -1.0;
	}
	else if (ball_ptr->x + ball_ptr->x_v <= 2) {
		ball_ptr->x_v *= -1.0;
	}
}

void update_ball_coords(Ball* ball_ptr) {
	ball_ptr->x += ball_ptr->x_v;
	ball_ptr->y += ball_ptr->y_v;
}

void take_player_input(Player* p_ptr, Game* g_ptr) {
	int key_code = 0;
	if (_kbhit()) {
		key_code = _getch();
		if (key_code == p_ptr->up_key && p_ptr->row - floor(p_ptr->paddle_width / 2) - 1 > 0) {
			p_ptr->row -= 1;
		}
		else if (key_code == p_ptr->down_key) {
			if (p_ptr->paddle_width % 2 != 0 && p_ptr->row + floor(p_ptr->paddle_width / 2) + 1 < g_ptr->screen_h - 1) {
				p_ptr->row += 1;
			}
			else if (p_ptr->row + floor(p_ptr->paddle_width / 2) < g_ptr->screen_h - 1) {
				p_ptr->row += 1;
			}
		}
	}
}

void draw_paddle(char screen[], Player* p_ptr, Game* g_ptr) {
	int col = p_ptr->col;
	int row = p_ptr->row - (p_ptr->paddle_width / 2);
	int i = (row * (g_ptr->screen_w + 1)) + col;

	for (unsigned int j = 0; j < p_ptr->paddle_width; ++j) {
		screen[i] = p_ptr->paddle_char;
		i += g_ptr->screen_w + 1;
	}
}

void print_player_details(Player* p_one_ptr, Player* p_two_ptr) {
	printf("P1: %2d P2: %2d P1_row: %2d P2_row: %2d\n",
		p_one_ptr->score, p_two_ptr->score,
		p_one_ptr->row, p_two_ptr->row);
}