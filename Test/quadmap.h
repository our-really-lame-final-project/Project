using namespace std;

void quad_map(int screenx, int screeny)
{
// BITMAP RELATED.
/* ---------------------------------------------------------------------------------------------------------------------- */

    // Load bitmap images for use in the program.
    ALLEGRO_BITMAP *player = al_load_bitmap("Jamal-Sprite.png");
    ALLEGRO_BITMAP *zombiesImage = al_load_bitmap("RPG/Zombie-Sprite.png");

// FRAMERATE RELATED.
/* ---------------------------------------------------------------------------------------------------------------------- */

    // Set framerate constants for the game to run at.
    const float FPS = 40.0;//Sets fps to 60 frames per second
    const float frameFPS = 10.0;//Set fps for animation for walking.

// KEYSTATE, CAMERA, MAP, AND BOOLEAN RELATED DECLARATIONS.
/* ---------------------------------------------------------------------------------------------------------------------- */

    // Set objects for keyState (for when a key press is found) and camera (for a camera to follow the player's position).
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TRANSFORM camera;

    // Declare constants to representing the directions.
    enum Direction {DOWN, LEFT, RIGHT, UP};

    // Declare map vector for use. And create the map.
    vector< vector <int> > map;
    srand(time(NULL));
    LoadMap("Map1.txt", map);

    // Declare boolean for use in setting animation to false and setting loop to false and true.
    bool done = false, draw = true, active = false;


// TIMER RELATED.
/* ---------------------------------------------------------------------------------------------------------------------- */

    // Set objects for framerate timings. One for the game framerate and the other for the framerate for walking animations.
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER *frameTimer = al_create_timer(1.0 / frameFPS);

// EVENT RELATED.
/* ---------------------------------------------------------------------------------------------------------------------- */

    // Set objects for "event queues", based on player input, and the framerate timers.
    // Functions to create an event.
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    //Registers  a timer event.
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //Register another type timer event.
    al_register_event_source(event_queue, al_get_timer_event_source(frameTimer));
    //Registers a keyboard event.
    al_register_event_source(event_queue, al_get_keyboard_event_source());

// POSITIONING AND SPAWNING RELATED.
/* ---------------------------------------------------------------------------------------------------------------------- */

    // Declare one zombies position.
    float zx = 832, zy = 128;

    // Declare the player's starting positions.
    float x = 300,
          y = 300,
          moveSpeed = 10,
          cameraPosition[2] = {0, 0};

    int dir = DOWN,
        sourceX = 64,
        sourceY = 0;

// START OF WHILE LOOP FOR GAME.
/* ---------------------------------------------------------------------------------------------------------------------- */

     // Start timers for game.
    al_start_timer(timer);
    al_start_timer(frameTimer);

    // A while loop, so that the game runs until the user quits via ESC.
    while(!done)//loop until user is done.
    {
<<<<<<< HEAD

// EVENT INPUTS.
/* ---------------------------------------------------------------------------------------------------------------------- */

        // Create an event to read the user's keystrokes.
        ALLEGRO_EVENT events;
        // Wait for the user to input an event.
        al_wait_for_event(event_queue, &events);
        // Save the user's event.
        al_get_keyboard_state(&keyState);

// SWITCH STATEMENT FOR ARROW INPUTS.
/* ---------------------------------------------------------------------------------------------------------------------- */

        // An if statement to register the esc key for use in exitting the game.
        if(events.type == ALLEGRO_EVENT_KEY_UP)
        {
            //Reads the key.
            switch(events.keyboard.keycode)
            {
                case ALLEGRO_KEY_ESCAPE:
                    //Ends the program for keyboard.
                    done = true;
            }
        }

        // An else if statement for map reinitialization.
        else if (events.type == ALLEGRO_EVENT_KEY_DOWN)
=======
        ALLEGRO_EVENT events;//Creates the event.
        al_wait_for_event(event_queue, &events);//Waits until a event occurs.
        al_get_keyboard_state(&keyState);//Saves current keyboard state.
        // Key up is to start event when key released it
        // key down is for when key is pressed.
        if (events.type == ALLEGRO_EVENT_KEY_DOWN)
>>>>>>> origin/dev-new
        {
            if(events.keyboard.keycode == ALLEGRO_KEY_L)
            {
                map.clear();
                LoadMap("Map1.txt", map);
            }
            if(events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                done = true;//Ends the program for keyboard.
            }
        }

<<<<<<< HEAD
        // An else if for keystroke reads.
        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            //Sets instructions for keys and timer function.
=======
        if(events.type == ALLEGRO_EVENT_TIMER)
        {//Sets instructions for keys and timer function.
            //StartZombie(zombies, Num_Zombie);
			//UpdateZombie(zombies, Num_Zombie);
>>>>>>> origin/dev-new
            if(events.timer.source == timer)
            {
                active = true;

                // Read down arrow.
                if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
                {
                    y += moveSpeed;
                    dir = DOWN;
                }
                // Read up arrow.
                else if(al_key_down(&keyState, ALLEGRO_KEY_UP))
                {
                    y -= moveSpeed;
                    dir = UP;
                }

                // Read right arrow.
                else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                {
                    x += moveSpeed;
                    dir = RIGHT;
                }

                // Read left arrow.
                else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                {
                    x -= moveSpeed;
                    dir = LEFT;
                }

                // Else statement to disable active.
                else
                {
                    active = false;
                }

// CAMERA REPOSITIONING RELATED.
/* ---------------------------------------------------------------------------------------------------------------------- */

                // Function to reposition the camera to the player's position. Also prevents the camera from viewing
                // over out of bounds areas.
                CameraUpdate(cameraPosition, screenx, screeny, x, y, 64, 64);

                al_identity_transform(&camera);
                al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
                al_use_transform(&camera);

// COLLISION LOOP FOR PLAYER AND ZOMBIE.
/* ---------------------------------------------------------------------------------------------------------------------- */

                // Nested for and if loop/statements to check the boundaries set in the map and initiate a
                // collision
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

                            if (Collision(zx, zy,j * 64, i * 64, 64, 64))
                            {
                                obstruct(zx, zy, dir, moveSpeed);
                            }

                        }
                    }
                }
            }

// CHANGING PLAYER SPRITES RELATED.
/* ---------------------------------------------------------------------------------------------------------------------- */

            // Else if used for the player's sprite.
            else if (events.timer.source == frameTimer)
            {
                // If statement to grab the appropriate sprite.
                if(active)
                {
                    sourceX += al_get_bitmap_width(player) / 3;
                }

                // Else set sprite to the standing neutral one.
                else
                {
                    sourceX = 64;
                }

                if(sourceX >= al_get_bitmap_width(player))
                {
                    sourceX = 0;
                }

                // Gives the direction of the player.
                sourceY = dir;
            }

            //If any keys are used, them draw will return true and draw the image.
            draw = true;
        }

// DRAWING RELATED FUNCTIONS HERE.
/* ---------------------------------------------------------------------------------------------------------------------- */

        // If statement to start drawing the images when inputs are read.
        if(draw)
        {
            // Draw all the images. Constantly updates with keystroke inputs.
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY * 64, 64, 64);
            al_clear_to_color(al_map_rgb(255, 255, 255));//Set background color.
            DrawMap(map);
            BrainsZombie(x, y, zx, zy, dir, moveSpeed);
            al_draw_bitmap_region(zombiesImage, 0, 0, 64, 64, zx, zy, NULL);
            al_draw_bitmap(subBitmap, x, y, NULL);
            al_flip_display();//Displays the image.
            al_clear_to_color(al_map_rgb(0, 0, 0));//Set background color.
            al_destroy_bitmap(subBitmap);
        }
    }

 // ASSETS DESTRUCTION HERE.
 /* ---------------------------------------------------------------------------------------------------------------------- */

    // Destroy functions to remove all the features when the player exits.
    al_destroy_timer(timer);// Destroy timer.
    al_destroy_bitmap(player);//Destroy player bitmap.
    al_destroy_bitmap(zombiesImage); // Destroy zombie bitmap.
    al_destroy_event_queue(event_queue);// Destroy event_queue.

    return;
}
