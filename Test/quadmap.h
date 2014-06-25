void quad_map(int screenx, int screeny)
{
    // BITMAP & IMAGE SETUP ==================================================== //
    // =============================================================================== //
    //Creates bitmap for player.
    ALLEGRO_BITMAP *player = al_load_bitmap("Jamal-Sprite.png");
    ALLEGRO_BITMAP *zombiesImage = al_load_bitmap("RPG/Zombie-Sprite.png");
    // KEYBOARD & TIMER SETUP ======================================================== //
    // =============================================================================== //

    const float FPS = 40.0;//Sets fps to 60 frames per second
    const float frameFPS = 10.0;//Set fps for animation for walking.

    ALLEGRO_KEYBOARD_STATE keyState;//Tells the program what key was pressed.
    ALLEGRO_TRANSFORM camera;

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);//Set timer to 60 fps.
    ALLEGRO_TIMER *frameTimer = al_create_timer(1.0 / frameFPS);//Set timer to 15 fps.

    // EVENT QUEUES ================================================================= //
    // ============================================================================== //


    //Function to create an event.
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    //Registers timer event.
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //Register timer event.
    al_register_event_source(event_queue, al_get_timer_event_source(frameTimer));
    //Registers keyboard event.
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    const int Num_Zombie = 10;
    Zombie zombies[Num_Zombie];
    enum Direction { DOWN, LEFT, RIGHT, UP};//Declaring key constants.
    //Setting animation to false Setting loop to false and true.
    bool done = false, draw = true, active = false;
    //sets player position and speed.
    float x = 300, y = 300, moveSpeed = 10, cameraPosition[2] = {0, 0};
    int dir = DOWN, sourceX = 64, sourceY = 0;//Standing position & direction.
    std::vector< std::vector <int> > map;
    srand(time(NULL));
	InitZombie(zombies, Num_Zombie, zombiesImage);
    LoadMap("Map1.txt", map);


    al_start_timer(timer);//Starts the timer.
    al_start_timer(frameTimer);
    // Do not enter any code besides the while loop after the timer in order to prevent
    // any malfunctions with the program.
    while(!done)//loop until user is done.
    {
        ALLEGRO_EVENT events;//Creates the event.
        al_wait_for_event(event_queue, &events);//Waits until a event occurs.
        al_get_keyboard_state(&keyState);//Saves current keyboard state.
        // Key up is to start event when key released it
        // key down is for when key is pressed.
        if(events.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(events.keyboard.keycode)//Reads the key.
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;//Ends the program for keyboard.
            }
        }
        else if (events.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if(events.keyboard.keycode == ALLEGRO_KEY_L)
            {
                map.clear();
                LoadMap("Map1.txt", map);
            }
        }

        else if(events.type == ALLEGRO_EVENT_TIMER)
        {//Sets instructions for keys and timer function.
            StartZombie(zombies, Num_Zombie);
			UpdateZombie(zombies, Num_Zombie);
            if(events.timer.source == timer)
            {
                active = true;
                if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
                {
                    y += moveSpeed;
                    dir = DOWN;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_UP))
                {
                    y -= moveSpeed;
                    dir = UP;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                {
                    x += moveSpeed;
                    dir = RIGHT;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                {
                    x -= moveSpeed;
                    dir = LEFT;
                }
                else
                    active = false;

                CameraUpdate(cameraPosition, screenx, screeny, x, y, 64, 64);

                al_identity_transform(&camera);
                al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
                al_use_transform(&camera);

                for(unsigned int i = 0; i < map.size(); i++)
                {
                    for(unsigned int j = 0; j < map[i].size(); j++)
                    {
                        if (map[i][j] == 9 || map[i][j] == 1 ||
                                map[i][j] == 10 || map[i][j] == 4)
                        {
                            if (Collision(x, y, j * 64, i * 64, 64, 64))
                            {
                                obstruct(x, y, dir, moveSpeed);
                            }
                        }
                    }
                }
            }
            else if (events.timer.source == frameTimer)
            {
                if(active)
                    sourceX += al_get_bitmap_width(player) / 3;
                else
                    sourceX = 64;

                if(sourceX >= al_get_bitmap_width(player))
                    sourceX = 0;

                sourceY = dir;//Direction of player.
            }
            //If any keys are used, them draw will return true and draw the image.
            draw = true;
        }

        if(draw)//Draws the image when keys are inputted.
        {
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY * 64, 64, 64);
            al_clear_to_color(al_map_rgb(255, 255, 255));//Set background color.
            DrawMap(map);
            //DrawZombie(zombies, Num_Zombie);
            al_draw_bitmap_region(zombiesImage, 0, 0, 64, 64, 832, 128, NULL);
            al_draw_bitmap(subBitmap, x, y, NULL);
            al_flip_display();//Displays the image.
            al_clear_to_color(al_map_rgb(0, 0, 0));//Set background color.
            al_destroy_bitmap(subBitmap);
        }
    }
    al_destroy_timer(timer);//destroy timer.
    al_destroy_bitmap(player);//Destroy play bitmap.
    al_destroy_bitmap(zombiesImage);
    al_destroy_event_queue(event_queue);//destroy event_queue
}
