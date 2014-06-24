void InitZombie(Zombie zombies[], int size,  ALLEGRO_BITMAP *image)
{
	for(int i = 0; i < size; i++)
	{
		zombies[i].ID = ENEMY;
		zombies[i].live = false;
		zombies[i].speed = 4;
		zombies[i].boundx = 32;
		zombies[i].boundy = 32;

        zombies[i].maxFrame = 143;
		zombies[i].curFrame = 0;
		zombies[i].frameCount = 0;
		zombies[i].frameDelay = 2;
		zombies[i].frameWidth = 64;
		zombies[i].frameHeight = 64;
		zombies[i].animationColumns = 3;

       //zombies[i].animationDirection = 1;
       //zombies[i].animationDirection = -1;

		zombies[i].image = image;
	}
}
void DrawZombie(Zombie zombies[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(zombies[i].live)
		{
            int fx = (zombies[i].curFrame % zombies[i].animationColumns) * 
                zombies[i].frameWidth;
			int fy = (zombies[i].curFrame / zombies[i].animationColumns) * 
                zombies[i].frameHeight;

			al_draw_bitmap_region(zombies[i].image, fx, fy, zombies[i].frameWidth,
				zombies[i].frameHeight, zombies[i].x - zombies[i].frameWidth / 3, 
                zombies[i].y - zombies[i].frameHeight / 4, 0);
		}
	}
}
void StartZombie(Zombie zombies[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(!zombies[i].live)
		{
			if(rand() % 500 == 0)
			{
				zombies[i].live = true;
				zombies[i].x = 800;
				zombies[i].y = 30 + rand() % (400 - 60);

				break;
			}
		}
	}
}
void UpdateZombie(Zombie zombies[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(zombies[i].live)
		{
			if(++zombies[i].frameCount >= zombies[i].frameDelay)
			{
				zombies[i].curFrame += zombies[i].animationDirection;
				if(zombies[i].curFrame >= zombies[i].maxFrame)
					zombies[i].curFrame = 0;
				else if( zombies[i].curFrame <= 0)
					zombies[i].curFrame = zombies[i].maxFrame - 1;

				zombies[i].frameCount = 0;
			}

			zombies[i].x -= zombies[i].speed;
		}
	}
}

