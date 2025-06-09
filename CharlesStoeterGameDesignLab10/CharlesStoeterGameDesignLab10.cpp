#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "Sprite.h"
#include <iostream>
#include <ctime> 

void checkCollision(sprite aliens[], int index, int arraySize, int screenW, int screenH);

using namespace std;

int main(void)
{


	char buffer[256];
	_getcwd(buffer, 256);
	cout << "Working directory: " << buffer << endl;

	//variables
	int width = 640;
	int height = 480;
	bool done = false;

	bool redraw = true;
	const int FPS = 60;

	srand(time(NULL));


	//allegro variable
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;

	const int NUM_SPRITES = 5;
	sprite aliens[NUM_SPRITES];

	ALLEGRO_TIMER* timer = NULL;


	//program init
	if (!al_init())	//initialize Allegro
		return -1;

	display = al_create_display(width, height);//create our display object

	if (!display)//test display object
		return -1;

	al_install_keyboard();
	al_init_image_addon();


	for (int i = 0; i < NUM_SPRITES; i++) {
		aliens[i].load_animated_sprite(9);
		aliens[i].assignRandomSpecialty();

		aliens[i].setPosition(rand() % (width - 64), rand() % (height - 64));
	}

	
	
	timer = al_create_timer(1.0 / FPS);

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_set_target_bitmap(al_get_backbuffer(display));
	al_start_timer(timer);

	

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			for (int i = 0; i < NUM_SPRITES; i++) {
				aliens[i].bouncesprite(width, height);
				checkCollision(aliens, i, NUM_SPRITES, width, height); // check after bouncing
			}

			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;

		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{


			redraw = false;
			for (int i = 0; i < NUM_SPRITES; i++) {
				aliens[i].updatesprite(al_get_time());
			}


			for (int i = 0; i < NUM_SPRITES; i++) {
				aliens[i].drawSprite();
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}


	al_destroy_event_queue(event_queue);
	al_destroy_display(display); //destroy our display object

	return 0;
}


