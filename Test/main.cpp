#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>//Dialog boxes.
#include <allegro5/allegro_ttf.h>//read font files.
#include <allegro5/allegro_font.h>//font.
#include <allegro5/allegro_image.h>//images.
#include <allegro5/allegro_primitives.h>//to draw images.
#include <allegro5/allegro_audio.h>//Play audio.
#include <allegro5/allegro_acodec.h>//Play different file types for audio.
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "collision.h"
#include "camera.h"
#include "map.h"
#include "Objects.h"
#include "splashscreens.h"
#include "enemies.h"

const int Num_Zombie = 10;
int main()
{
    // DISPLAY ==================================================================//
    Zombie zombies[Num_Zombie];
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_DISPLAY_MODE disp_data;

    const float FPS = 30.0;//Sets fps to 60 frames per second
    const float frameFPS = 15.0;//Set fps for animation for walking.
    enum Direction { DOWN, LEFT, RIGHT, UP};//Declaring key constants.

    if(!al_init())//if allegro 5 does not initialize show error message.
    {
        al_show_native_message_box(NULL, NULL, "Error", "Could not initialize Allegro 5",
                NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // al_get_display 1st arg is a num for an index for a list of screen resolutions
    // we must loop through all the indices to find the highest screen res cuz
    // the order seems to different on diff OSes. &disp_data is struct, gets populated by
    // this func with monitor data such height and width.
    int max_res_index = 0, max_res_width, max_res_height;
    for (int c = 0; c < al_get_num_display_modes(); c++)
    {
        al_get_display_mode(max_res_index, &disp_data);
        max_res_height = disp_data.height;
        max_res_width = disp_data.width;

        al_get_display_mode(c, &disp_data);
        if (disp_data.width >= max_res_width && disp_data.height >= max_res_height)
        {
            max_res_index = c;
        }
    }
    al_get_display_mode(max_res_index, &disp_data);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);//display format.
    // creates the display with the monitors width & height
    display = al_create_display(disp_data.width, disp_data.height);

    al_set_window_title(display, "Gaming Project");//sets a window title.

    if(!display)//if display does not initialize show error message.
    {
        al_show_native_message_box(NULL, NULL, "Error",
                "Could not create Allegro 5 display", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }


    //INSTALL & INITIATION
    al_install_audio();//Installs the audio.
    al_init_acodec_addon();//Initializes sound.
    al_init_font_addon();//initialize font addon.
    al_init_ttf_addon();//initialize ttf files.
    al_init_image_addon();//Initializes image.
    al_init_primitives_addon();//Initialize primitives to draw.
    al_install_keyboard();//Installs the keyboard.


    // SOUND SETUP =================================================================== //
    // =============================================================================== //
    al_reserve_samples(1);//Number of samples playing.
    ALLEGRO_SAMPLE *song = al_load_sample("TestGetLucky8Bit.ogg"); //Load the song file.
    if (!song)
    {
        //al_show_native_message_box(NULL, NULL, "Error", "Could not load song", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        std::cout << "could not load song\n";
        //return -1;
    }

    //Attaches song into sample instance to create it.
    ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);
    //Choose play mode for song like looping the song.
    al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
    //Attach the song to the default mixer.
    al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
    al_play_sample_instance(songInstance); //Starts the song.

    // INTRO SPLASH SCREEN
    intro_splash(disp_data.width, disp_data.height);

    // BITMAP & IMAGE SETUP ==================================================== //
    // =============================================================================== //

    ALLEGRO_BITMAP *player = al_load_bitmap("Jamal-Sprite.png");//Creates bitmap for player.
    ALLEGRO_BITMAP *block = al_load_bitmap("block.png");//Creates bitmap for player.
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");//Creates an object.

    ALLEGRO_BITMAP *npc = al_load_bitmap("RPG/Comp1.png");
    ALLEGRO_BITMAP *zombiesImage;
    zombiesImage = al_load_bitmap("Zombie-Sprite.png");
    // KEYBOARD & TIMER SETUP ======================================================== //
    // =============================================================================== //

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
    //Registers display event.
    al_register_event_source(event_queue, al_get_display_event_source(display));
    //Registers keyboard event.
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    bool done = false, draw = true;//Setting loop to false and true.
    bool active = false;//Setting animation to false;
    //sets player position and speed.
    float x = 10, y = 10, moveSpeed = 5, cameraPosition[2] = {0, 0};
    int dir = DOWN, sourceX = 64, sourceY = 0;//Standing position & direction.
    std::vector< std::vector <int> > map;
    srand(time(NULL));
	InitZombie(zombies, Num_Zombie, zombiesImage);
    LoadMap("Map1.txt", map);


    al_start_timer(timer);//Starts the timer.
    al_start_timer(frameTimer);
    //Do not enter any code besides the while loop after the timer in order to prevent any malfunctions with the program.


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
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;//Closes the program when user clicks on the x button.
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

            CameraUpdate(cameraPosition, disp_data.width, disp_data.height, x, y, 64, 64);

                al_identity_transform(&camera);
                al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
                al_use_transform(&camera);

                if(Collision(x, y, 200, 150, 32, 32))
                {
                    obstruct(x, y, dir, moveSpeed);
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
            draw = true;//If any keys are used, them draw will return true and draw the image.
        }

        if(draw)//Draws the image when keys are inputted.
        {
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY * 64, 64, 64);
            //al_draw_bitmap(background, 0, 0, NULL);
            al_clear_to_color(al_map_rgb(255, 255, 255));//Set background color.
            DrawMap(map);
            DrawZombie(zombies, Num_Zombie);
            al_draw_bitmap(npc, 700, 100, NULL);
            al_draw_bitmap(subBitmap, x, y, NULL);
            al_flip_display();//Displays the image.
            al_clear_to_color(al_map_rgb(0, 0, 0));//Set background color.
            al_destroy_bitmap(subBitmap);
        }
    }

    end_game_splash(disp_data.width, disp_data.height);

// DESTORY ALL THE THINGS ================================================================== //
    al_destroy_bitmap(npc); // destroy npc
    al_destroy_display(display);//destroy display.
    al_destroy_timer(timer);//destroy timer.
    al_destroy_bitmap(player);//Destroy play bitmap.
    al_destroy_bitmap(zombiesImage);
    al_destroy_bitmap(block);//Destroy block bitmap.
    //al_destroy_bitmap(background);//Destroy play bitmap.
    al_destroy_sample(song);//Destroy song.
    al_destroy_sample_instance(songInstance);//Destroy song instance.
    al_destroy_event_queue(event_queue);//destroy event_queue

    return 0;
}
