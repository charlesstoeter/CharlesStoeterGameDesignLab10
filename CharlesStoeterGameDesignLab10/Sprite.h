#pragma once
class sprite
{
public:
	~sprite();
	void updatesprite();
	void bouncesprite(int SCREEN_W, int SCREEN_H);
	void load_animated_sprite(int size);
	void drawSprite();
	int getX() { 
		return x; 
	}


	int getY() {
		return y; 
	}

	void assignRandomSpecialty(); // Assign one random behavior 


private:
	int x, y;
	int width, height;
	int xspeed, yspeed;
	int xdelay, ydelay;
	int xcount, ycount;
	int curframe, maxframe, animdir;
	int framecount, framedelay;
	ALLEGRO_BITMAP* image[9];

	bool CollisionIsTrue = false;
	bool SpinningSprite = false;
	bool ScaredSprite = false;
	bool BabySprite = false;
	bool FreezeSprite = false;

	float lastCollisionTime = 0; // time of last collision
	float scaleFactor = 1.0f;  // for BabySprite shrinking
	float freezeStartTime = 0;  // for FreezeSprite freeze delay




};