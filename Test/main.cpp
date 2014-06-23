#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>//Dialog boxes.
#include <allegro5/allegro_ttf.h>//read font files.
#include <allegro5/allegro_font.h>//font.
#include <allegro5/allegro_image.h>//images.
#include <allegro5/allegro_primitives.h>//to draw images.
#include <allegro5/allegro_audio.h>//Play audio.
#include <allegro5/allegro_acodec.h>//Play different file types for audio.
#include <cmath>
#include <iostream>
#include "collision.h"

//#define ScreenWidth 800//sets screen width variable.
//#define ScreenHeight 600//sets screen height variable.

//When using pointers(*) you need to make sure to destroy them before you run and build the program.
void CameraUpdate(float *cameraPosition, float x, float y, int width, int height)
{
    cameraPosition[0] = -(800 / 2) + (x + width / 2);
    cameraPosition[1] = -(600 / 2) + (y + height / 2);
    if(cameraPosition[0] < 0)
        cameraPosition[0] = 0;
    if(cameraPosition[1] < 0)
        cameraPosition[1] = 0;
}

int main()
{
    const float FPS = 30.0;//Sets fps to 60 frames per second
    const float frameFPS = 15.0;//Set fps for animation for walking.
    enum Direction { DOWN, LEFT, RIGHT, UP};//Declaring key constants.

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_DISPLAY_MODE disp_data;

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
    bool done = false, draw = true;//Setting loop to false and true.
    bool active = false;//Setting animation to false;
    float x = 10, y = 10, moveSpeed = 5;//sets player position and speed.
    int dir = DOWN;//Sets players direction.
    int sourceX = 32, sourceY = 0;//Standing position.

    float cameraPosition[2] = {0, 0};
    /*---------------------------------------------------------------------------------*/
    //Gravity code.
    /*float velx, vely; //Changes jump speed with gravity.
    velx = vely = 0;//Set default value .
    bool jump = false;//Sets jump loop to false.
    float jumpSpeed = 15;//Sets players jump speed.
    const float gravity = 1;//Sets realistic gravity settings.*/
    /*--------------------------------------------------------------------------------*/


    // SOUND SETUP =================================================================== //
    // =============================================================================== //
    al_install_audio();//Installs the audio.
    al_init_acodec_addon();//Initializes sound.
    al_reserve_samples(2);//Number of samples playing.
    ALLEGRO_SAMPLE *soundEffect = al_load_sample("soundEffect.wav");//Load sound file.
    if (!soundEffect)
    {
        std::cout << "could not load sound effect\n";
    }
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

    // FONT ========================================================================== //
    // =============================================================================== //
    al_init_font_addon();//initialize font addon.
    al_init_ttf_addon();//initialize ttf files.

    ALLEGRO_FONT *font = al_load_font("orbitron-black.ttf", 36, NULL);//Font input.
    //Draws text with given font.
    al_draw_text(font, al_map_rgb(44, 117, 255), disp_data.width / 2, 
            disp_data.height / 2, ALLEGRO_ALIGN_CENTRE, "JAMAL QUEST");

    // BITMAP & IMAGE SETUP ==================================================== //
    // =============================================================================== //

    al_init_image_addon();//Initializes image.
    al_init_primitives_addon();//Initialize primitives to draw.
    ALLEGRO_BITMAP *player = al_load_bitmap("Test.png");//Creates bitmap for player.
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");//Creates an object.

    // KEYBOARD & TIMER SETUP ======================================================== //
    // =============================================================================== //
    al_install_keyboard();//Installs the keyboard.

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

    al_flip_display();//shows the font.
    al_rest(4.0);//sets screen timer to 4.0.

    al_start_timer(timer);//Starts the timer.
    al_start_timer(frameTimer);
    //Do not enter any code besides the while loop after the timer in order to prevent any malfunctions with the program.

    while(!done)//loop until user is done.
    {
        ALLEGRO_EVENT events;//Creates the event.
        al_wait_for_event(event_queue, &events);//Waits until a event occurs.
        al_get_keyboard_state(&keyState);//Saves current keyboard state.

        if(events.type == ALLEGRO_EVENT_KEY_UP)//Key up is to start event when key released it, key down is for when key is pressed.
        {
            switch(events.keyboard.keycode)//Reads the key.
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;//Ends the program for keyboard.
            }
        }

        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;//Closes the program when user clicks on the x button.
        }

        else if(events.type == ALLEGRO_EVENT_TIMER)
        {//Sets instructions for keys and timer function.
            if(events.timer.source == timer)
            {
                active = true;
                if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
                {
                    y += moveSpeed;
                    dir = DOWN;
                    al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                    //Plays the sound.(Sound name, gain volume, (left, right, balance speaker), speed, and repeat.)
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

                CameraUpdate(cameraPosition, x, y, 32, 32);

                al_identity_transform(&camera);
                al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
                al_use_transform(&camera);

                if(Collision(x, y, 200, 200, 10, 10))
                {//If collision true, run if statement.
                    if(dir == 0)
                        y -= moveSpeed;
                    else if(dir == 1)
                        x += moveSpeed;
                    else if(dir == 2)
                        x -= moveSpeed;
                    else if(dir == 3)
                        y += moveSpeed;
                }
                /*------------------------------------------------------------------------------------*/
                //Gravity code.
                /*if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                {
                    velx = moveSpeed;
                    dir = RIGHT;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                {
                    velx = -moveSpeed;
                    dir = LEFT;
                }
                else
                {
                    velx = 0;
                    active = false;//Sets animation off as long as user does not move.
                }
                if(al_key_down(&keyState, ALLEGRO_KEY_UP) && jump)
                {
                    vely = -jumpSpeed;
                    jump = false;
                }*/
                /*------------------------------------------------------------------------------------*/
            }

            else if (events.timer.source == frameTimer)
            {
                if(active)
                    sourceX += al_get_bitmap_width(player) / 3;
                else
                    sourceX = 32;

                if(sourceX >= al_get_bitmap_width(player))
                    sourceX = 0;

                sourceY = dir;//Direction of player.
            }
            draw = true;//If any keys are used, them draw will return true and draw the image.
        }
            /*------------------------------------------------------------------------------------*/
            //Gravity code.
            /*if(!jump)
                vely += gravity; //Sets gravity active for jump..
            else
                vely = 0;//Sets gravity to zero.

            x += velx; //Sets direction speed for gravity.
            y += vely;//Sets direction speed for gravity.

            jump = (y + 32 >= 600);//Sets jump border for player.

            if(jump)
                y = 568;//Places players height into correct position.*/
                /*------------------------------------------------------------------------------------*/

        if(draw)//Draws the image when keys are inputted.
        {
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY * 32, 32, 32);
            al_draw_bitmap(background, 0, 0, NULL);
            al_draw_bitmap(subBitmap, x, y, NULL);
            //al_draw_bitmap_region(player, sourceX, sourceY * al_get_bitmap_height(player) / 4, 32, 32, x, y, NULL);//Draws the bitmap animation on screen.
            //al_draw_bitmap_region(player2, 0, 0, 32, 32, 200, 200, NULL);//Draws the bitmap on the screen.
            //al_draw_bitmap(player, x, y + 440, NULL);//Draws the bitmap on screen.
            //al_draw_rectangle(x, y, x + 10, y + 10, al_map_rgb(44, 117, 255), 1);//Draws rectangle.
            al_flip_display();//Displays the image.
            al_clear_to_color(al_map_rgb(0, 0, 0));//Set background color.
            al_destroy_bitmap(subBitmap);
        }
    }

    ALLEGRO_FONT *font1 = al_load_font("orbitron-black.ttf", 36, NULL);//Font input.
    al_draw_text(font1, al_map_rgb(44, 117, 255), disp_data.width / 2, disp_data.height / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");//Draws text with given font.
    al_flip_display();//shows the font.
    al_rest(1.0);//sets screen timer.

    al_destroy_font(font);//destroy font.
    al_destroy_font(font1);//destroy font.
    al_destroy_display(display);//destroy display.
    al_destroy_timer(timer);//destroy timer.
    al_destroy_bitmap(player);//Destroy play bitmap.
    al_destroy_bitmap(background);//Destroy play bitmap.
    al_destroy_sample(soundEffect);//Destroy sound.
    al_destroy_sample(song);//Destroy song.
    al_destroy_sample_instance(songInstance);//Destroy song instance.
    al_destroy_event_queue(event_queue);//destroy event_queue

    return 0;
}
