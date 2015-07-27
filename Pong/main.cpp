#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "data.h"
#include <iostream>

const int FPS  = 80;
int height     = 0;
int width      = 0;
int side;
bool start     = false;
bool firstGame = true;
bool pause     = false;
bool end       = false;

enum paddle_direction{UP, DOWN};
enum players{P1, P2};
bool keys[2][2] = {{ false, false }, { false, false }};

PLAYER player1;
PLAYER player2;
BALL ball;

ALLEGRO_FONT* font = NULL;
ALLEGRO_FONT* scorefont = NULL;

void init(PLAYER* p1, PLAYER* p2, BALL* ball);
void nextRound(PLAYER* p1, PLAYER* p2, BALL* ball, int flag);
void render();
void update(PLAYER* p, int di);
void update(BALL* ball);
void updateGame(PLAYER* p1, PLAYER* p2, BALL* ball);

int main()
{
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_DISPLAY_MODE disp;
	ALLEGRO_EVENT_QUEUE* eq = NULL;

	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_TIMER* ball_timer = NULL;

	bool run = true;
	bool draw = false;

	if (!al_init())
	{
		return -1;
	}

	al_get_display_mode(al_get_num_display_modes() - 1, &disp);

	std::cout << disp.height << " " << height << std::endl;
	std::cout << disp.width << " " << width << std::endl;

	height = disp.height / 3 * 2;
	width = height / 9 * 16;

	std::cout << disp.height << " " << height << std::endl;
	std::cout << disp.width << " " << width << std::endl;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(width, height);
	al_set_window_title(display, "PONG V2");

	if (display == NULL)
	{
		return -1;
	}

	init(&player1, &player2, &ball);

	eq = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	ball_timer = al_create_timer(15.0);

	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	font = al_load_ttf_font("./arial.ttf", 18, 0);
	scorefont = al_load_ttf_font("./arial.ttf", (height * width) / 36000, 0);

	al_register_event_source(eq, al_get_keyboard_event_source());
	al_register_event_source(eq, al_get_display_event_source(display));
	al_register_event_source(eq, al_get_timer_event_source(timer));

	al_hide_mouse_cursor(display);
	al_start_timer(timer);

	while (run)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(eq, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			draw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			run = false;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_S:
					keys[P1][DOWN] = true;
					break;
				case ALLEGRO_KEY_W:
					keys[P1][UP] = true;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[P2][DOWN] = true;
					break;
				case ALLEGRO_KEY_UP:
					keys[P2][UP] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					start = true;
					firstGame = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					run = false;
					break;
				case ALLEGRO_KEY_P:
					if (pause == true) pause = false;
					else if (pause == false) pause = true;
					break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_S:
					keys[P1][DOWN] = false;
					break;
				case ALLEGRO_KEY_W:
					keys[P1][UP] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[P2][DOWN] = false;
					break;
				case ALLEGRO_KEY_UP:
					keys[P2][UP] = false;
					break;
			}
		}
		if (draw && al_event_queue_is_empty(eq))
		{
			if (!pause)
			{
				if (keys[P1][UP])update(&player1, -1);
				if (keys[P1][DOWN])update(&player1, 1);
				if (keys[P2][UP])update(&player2, -1);
				if (keys[P2][DOWN])update(&player2, 1);

				update(&ball);
				updateGame(&player1, &player2, &ball);
				render();
				draw = false;
			}
		}
	}

	al_destroy_display(display);
	al_destroy_event_queue(eq);

	return 0;
}

void render()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_line(width / 2, 
				 0, 
				 width / 2, 
				 height, 
				 al_map_rgb(123, 123, 123), 
				 width / 500);

	al_draw_text(font, 
				 al_map_rgb(123, 255, 255), 
				 width / 2, 
				 height - 30, 
				 ALLEGRO_ALIGN_CENTER, 
				 "Press 'P' to PAUSE");

	al_draw_filled_rectangle(player1.x, 
							 player1.y, 
							 player1.x + player1.size_x, 
							 player1.y + player1.size_y, 
							 al_map_rgb(255, 255, 255));

	al_draw_filled_rectangle(player2.x, 
							 player2.y, 
							 player2.x - player2.size_x, 
							 player2.y + player2.size_y, 
							 al_map_rgb(255, 255, 255));

	al_draw_filled_circle(ball.x, 
						  ball.y, 
						  ball.size, 
						  al_map_rgb(255, 255, 255));

	al_draw_textf(scorefont, 
				  al_map_rgb(255, 255, 255), 
				  width / 4, 
				  height / 12, 
				  ALLEGRO_ALIGN_CENTER, "%d", player1.score);

	al_draw_textf(scorefont, 
				  al_map_rgb(255, 255, 255), 
				  width - width / 4, height / 12, 
				  ALLEGRO_ALIGN_CENTER, "%d", player2.score);

	al_flip_display();
}

void init(PLAYER* p1, PLAYER* p2, BALL* ball)
{
	p1->size_x  = width / 100;
	p1->size_y  = height / 5;
	p1->x       = p1->size_x;
	p1->y       = height / 2 - p1->size_y / 2;
	p1->hit_x   = p1->x + p1->size_x;
	p1->hit_y1  = p1->y;
	p1->hit_y2  = p1->hit_y1 + p1->size_y;
	p1->score   = 0;
	p1->speed   = height / (height / 10);

	p2->size_x  = width / 100;
	p2->size_y  = height / 5;
	p2->x       = width - p2->size_x;
	p2->y       = height / 2 - p1->size_y / 2;
	p2->hit_x   = p2->x - p2->size_x;
	p2->hit_y1  = p2->y;
	p2->hit_y2  = p2->hit_y1 + p2->size_y;
	p2->score   = 0;
	p2->speed   = height / (height / 10);

	ball->x     = width / 2;
	ball->y     = height / 2;
	ball->size  = width / 175;
	ball->speed = width / 150;
	ball->dX    = ball->speed;
	ball->dY    = ball->speed;
}

void nextRound(PLAYER* p1, PLAYER* p2, BALL* ball, int flag)
{
	p1->y = height / 2 - p1->size_y / 2;
	p2->y = height / 2 - p1->size_y / 2;

	if (flag == P1)
	{
		ball->x = p1->x + p1->size_x + ball->size;
		ball->y = p1->y + p1->size_y / 2;
	}
	else if (flag == P2)
	{
		ball->x = p2->x - p2->size_x - ball->size;
		ball->y = p2->y + p2->size_y / 2;
	}

	start = false;
}

void update(PLAYER* p, int di)
{
	p->y += p->speed * di;
	p->hit_y1 = p->y;
	p->hit_y2 = p->hit_y1 + p->size_y;

	if (p->y <= 0) p->y = 1;
	else if (p->y + p->size_y >= height) p->y = height - p->size_y - 1;
}

void update(BALL* ball)
{
	if (start)
	{
		ball->x += ball->dX;
		ball->y += ball->dY;
	}
}

void updateGame(PLAYER* p1, PLAYER* p2, BALL* ball)
{
	if (ball->y <= 0 || ball->y + ball->size >= height)
	{
		ball->dY *= -1;
	}

	if (ball->x <= p1->hit_x)
	{
		if (ball->y + ball->size >= p1->hit_y1 && ball->y + ball->size <= p1->hit_y2)
		{
			ball->dX *= -1;
		}
	}
	else if (ball->x + ball->size >= p2->hit_x)
	{
		if (ball->y + ball->size >= p2->hit_y1 && ball->y + ball->size <= p2->hit_y2)
		{
			ball->dX *= -1;
		}
	}

	if (ball->x + ball->size <= 0)
	{
		p2->score += 1;
		nextRound(p1, p2, ball, P2);
		side = P2;
		ball->dX = -ball->speed;
	}
	else if (ball->x >= width)
	{
		p1->score += 1;
		nextRound(p1, p2, ball, P1);
		side = P1;
		ball->dX = ball->speed;
	}

	if (!start && !firstGame)
	{
		if (side == P1)
		{
			ball->y = p1->y + p1->size_y / 2;
		}
		else if (side == P2)
		{
			ball->y = p2->y + p2->size_y / 2;
		}
	}
}