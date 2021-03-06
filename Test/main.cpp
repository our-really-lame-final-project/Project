#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>//Dialog boxes.
#include<allegro5/allegro_ttf.h>//read font files.
#include<allegro5/allegro_font.h>//font.
#include<allegro5/allegro_image.h>//images.
#include<allegro5/allegro_primitives.h>//to draw images.
#include<allegro5/allegro_audio.h>//Play audio.
#include<allegro5/allegro_acodec.h>//Play different file types for audio.

#define ScreenWidth 800//sets screen width variable.
#define ScreenHeight 600//sets screen height variable.

//When using pointers(*) you need to make sure to destroy them before you run and build the program.

int main()
{
    const float FPS = 60.0;//Sets fps to 60 frames per second
    enum Direction { DOWN, LEFT, RIGHT, UP};//Declaring key constants.

    if(!al_init())//if allegro 5 does not initialize show error message.
    {
        al_show_native_message_box(NULL, NULL, "Error", "Could not initialize Allegro 5", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);//display format.
    ALLEGRO_DISPLAY *display = al_create_display(ScreenWidth, ScreenHeight);//sets display resolution.
    al_set_window_position(display, 283, 50);//sets display windows position.
    al_set_window_title(display, "Gaming Project");//sets a window title.

    if(!display)//if display does not initialize show error message.
    {
        al_show_native_message_box(NULL, NULL, "Error", "Could not create Allegro 5 display", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    bool done = false, draw = true;//Setting loop to false and true.
    bool active = false;//Setting animation to false;
    float x = 10, y = 10, moveSpeed = 5;//sets player position and speed.
    int dir = DOWN;//Sets players direction.
    int sourceX = 32, sourceY = 0;//Standing position.
    float velx, vely; //Changes jump speed with gravity.
    velx = vely = 0;//Set default value .
    bool jump = false;//Sets jump loop to false.
    float jumpSpeed = 15;//Sets players jump speed.
    const float gravity = 1;//Sets realistic gravity settings.

    al_init_font_addon();//initialize font addon.
    al_init_ttf_addon();//initialize ttf files.
    al_init_primitives_addon();//Initialize primitives to draw.
    al_install_keyboard();//Installs the keyboard.
    al_install_audio();//Installs the audio.
    al_init_image_addon();//Initializes image.
    al_init_acodec_addon();//Initializes sound.
    al_reserve_samples(2);//Number of samples playing.

    ALLEGRO_FONT *font = al_load_font("orbitron-black.ttf", 36, NULL);//Font input.
    al_draw_text(font, al_map_rgb(44, 117, 255), ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, "DOGE STORY");//Draws text with given font.

    ALLEGRO_SAMPLE *soundEffect = al_load_sample("soundEffect.wav");//Load sound file.
    ALLEGRO_SAMPLE *song = al_load_sample("GetLucky8Bit.ogg"); //Load the song file.
    ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);//Attaches song into sample instance to create it.
    al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);//Choose play mode for song like looping the song.
    al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());//Attach the song to the default mixer.

    ALLEGRO_BITMAP *player = al_load_bitmap("Test.png");//Creates bitmap.
    ALLEGRO_KEYBOARD_STATE keyState;//Tells the program what key was pressed.

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30);//Set timer to fps.
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();//Function to create an event.
    al_register_event_source(event_queue, al_get_keyboard_event_source());//Registers keyboard event.
    al_register_event_source(event_queue, al_get_timer_event_source(timer));//Registers timer event.
    al_register_event_source(event_queue, al_get_display_event_source(display));//Registers display event.

    al_play_sample_instance(songInstance);//Starts the song.
    al_flip_display();//shows the font.
    al_rest(4.0);//sets screen timer.

    al_start_timer(timer);//Starts the timer.
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

        if(events.type == ALLEGRO_EVENT_TIMER)
        {//Sets instructions for keys and timer function.
            active = true;
           /* if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
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
            }*/
            if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
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
            }
            if(active)
                sourceX += al_get_bitmap_width(player) / 3;
            else
                sourceX = 32;

            if(sourceX >= al_get_bitmap_width(player))
                sourceX = 0;

            sourceY = dir;//Direction of player.

            if(!jump)
                vely += gravity; //Sets gravity active for jump..
            else
                vely = 0;//Sets gravity to zero.

            x += velx; //Sets direction speed for gravity.
            y += vely;//Sets direction speed for gravity.

            jump = (y + 32 >= 600);//Sets jump border for player.

            if(jump)
                y = 600 - 32;//Places players height into correct position.

            draw = true;//If any keys are used, them draw will return true and draw the image.
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;//Closes the program when user clicks on the x button.
        }

        if(draw)//Draws the image when keys are inputted.
        {
            draw = false;
            al_draw_bitmap_region(player, sourceX, sourceY * al_get_bitmap_height(player) / 4, 32, 32, x, y, NULL);//Draws the bitmap animation on screen.
            //al_draw_bitmap(player, x, y + 440, NULL);//Draws the bitmap on screen.
            //al_draw_rectangle(x, y, x + 10, y + 10, al_map_rgb(44, 117, 255), 1);//Draws rectangle.
            al_flip_display();//Displays the image.
            al_clear_to_color(al_map_rgb(0, 0, 0));//Set background color.
        }
    }

    ALLEGRO_FONT *font1 = al_load_font("orbitron-black.ttf", 36, NULL);//Font input.
    al_draw_text(font1, al_map_rgb(44, 117, 255), ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, "THE END OF DOGE");//Draws text with given font.
    al_flip_display();//shows the font.
    al_rest(1.0);//sets screen timer.

    al_destroy_font(font);//destroy font.
    al_destroy_font(font1);//destroy font.
    al_destroy_display(display);//destroy display.
    al_destroy_timer(timer);//destroy timer.
    al_destroy_bitmap(player);//Destroy play bitmap.
    al_destroy_sample(soundEffect);//Destroy sound.
    al_destroy_sample(song);//Destroy song.
    al_destroy_sample_instance(songInstance);//Destroy song instance.
    al_destroy_event_queue(event_queue);//destroy event_queue

    return 0;
}
