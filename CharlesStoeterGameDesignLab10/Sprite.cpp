#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <stdio.h>
#include "Sprite.h"
#include <cstdlib>

#include <iostream>
using namespace std;

void sprite::drawSprite()
{
	if (SpinningSprite) {
		float cx = al_get_bitmap_width(image[curframe]) / 2.0f;
		float cy = al_get_bitmap_height(image[curframe]) / 2.0f;
		al_draw_tinted_rotated_bitmap(image[curframe],
			currentColor,  // 🆕 Tinted!
			cx, cy,
			x + cx, y + cy,
			rotationAngle, 0);
	}
	else if (ScaredSprite) {
		al_draw_tinted_bitmap(image[curframe], currentColor, x, y, 0); // 🆕
	}
	else if (BabySprite) {
		float cx = al_get_bitmap_width(image[curframe]) / 2.0f;
		float cy = al_get_bitmap_height(image[curframe]) / 2.0f;
		al_draw_tinted_scaled_bitmap(
			image[curframe],                      // bitmap
			al_map_rgb(255, 255, 255),            // tint color (default: white)
			0, 0, width, height,                  // source region
			x, y, width * scaleFactor, height * scaleFactor,  // destination size
			0                                      // flags
		);
	}


	else {
		al_draw_bitmap(image[curframe], x, y, 0);
	}
}


void sprite::updatesprite(double currentTime) {



	if (FreezeSprite && CollisionIsTrue && (currentTime - freezeStartTime < 5)) {
		// Still frozen — do not update
		return;
	}

	// Unfreeze logic
	if (FreezeSprite && CollisionIsTrue && (currentTime - freezeStartTime >= 5)) {
		CollisionIsTrue = false;
		cout << "FreezeSprite: UNFROZEN!\n";
	}

	// Simulate freeze every 6 seconds
	if (FreezeSprite && (currentTime - lastCollisionTime > 6)) {
		lastCollisionTime = currentTime;
		freezeStartTime = currentTime;
		CollisionIsTrue = true;
		cout << "FreezeSprite: FROZEN for 5 seconds!\n";
	}

	//update x position
	if (++xcount > xdelay)
	{
		xcount = 0;
		x += xspeed;
	}

	//update y position
	if (++ycount > ydelay)
	{
		ycount = 0;
		y += yspeed;
	}


	if (framecount++ > framedelay)
	{
		framecount = 0;
		curframe++;
		if (curframe >= maxframe)
			curframe = 0;
	}


	if (SpinningSprite) {
		rotationAngle += 0.05f;  // Adjust speed as needed
		if (rotationAngle > ALLEGRO_PI * 2)
			rotationAngle -= ALLEGRO_PI * 2; // wrap angle
	}


	if (ScaredSprite && (currentTime - lastCollisionTime > 1)) {
		lastCollisionTime = currentTime;
		currentColor = getRandomColor(); // change color
		x = rand() % 600;  // teleport within screen bounds
		y = rand() % 400;
		cout << "ScaredSprite: Color changed + teleported!\n";
	} 



	if (BabySprite && (currentTime - lastCollisionTime > 3)) { // Simulate 1 collision every 3s
		lastCollisionTime = currentTime;

		// Shrink in half
		scaleFactor *= 0.5f;
		x = rand() % 600;
		y = rand() % 400;

		if (scaleFactor < 0.1f) {
			cout << "BabySprite died\n";
			// Optional: deactivate movement or reset
		}
		else {
			cout << "BabySprite scaled to: " << scaleFactor << "\n";
		}
	}


	
}

void sprite::bouncesprite(int SCREEN_W, int SCREEN_H)
{
	//simple screen bouncing behavior
	if (x < 0)
	{
		x = 0;
		xspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (x > SCREEN_W - width)
	{
		x = SCREEN_W - width;
		xspeed = rand() % 2 - 6;
		animdir *= -1;
	}

	if (y < 0)
	{
		y = 0;
		yspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (y > SCREEN_H - height)
	{
		y = SCREEN_H - height;
		yspeed = rand() % 2 - 6;
		animdir *= -1;
	}

}

void sprite::load_animated_sprite(int size)
{
	//load the animated sprite
	char s[80];
	maxframe = size;
	for (int n = 0; n < size; n++)
	{
		sprintf_s(s, "Alien%d.bmp", n);
		image[n] = al_load_bitmap(s);
		if (!image[n]) {
			cout << "Failed to load bitmap: " << s << endl;
			exit(1); // Or handle it more gracefully
		}

		al_convert_mask_to_alpha(image[n], al_map_rgb(255, 255, 255));
	}
	width = al_get_bitmap_width(image[0]);
	height = al_get_bitmap_height(image[0]);
	curframe = 0;
	framedelay = 5;
	framecount = 0;


}

sprite::~sprite()
{
	for (int i = 0; i < maxframe; i++)
		al_destroy_bitmap(image[i]);
}





void sprite::assignRandomSpecialty() {
	// Reset all specialty flags
	SpinningSprite = false;
	ScaredSprite = false;
	BabySprite = false;
	FreezeSprite = false;

	currentColor = al_map_rgb(255, 255, 255);

	// Randomly assign one specialty type
	int type = rand() % 4;  // 0 to 3

	switch (type) {
	case 0:
		SpinningSprite = true;
		cout << "Assigned: SpinningSprite\n";
		break;
	case 1:
		ScaredSprite = true;
		cout << "Assigned: ScaredSprite\n";
		break;
	case 2:
		BabySprite = true;
		scaleFactor = 1.0f;
		cout << "Assigned: BabySprite\n";
		break;
	case 3:
		FreezeSprite = true;
		cout << "Assigned: FreezeSprite\n";
		break;
	}

	CollisionIsTrue = false;
	lastCollisionTime = 0;
	freezeStartTime = 0;
}





ALLEGRO_COLOR sprite::getRandomColor() {
	return al_map_rgb(rand() % 256, rand() % 256, rand() % 256);
}
