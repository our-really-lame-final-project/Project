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
#include "quadmap.h"

int main()
{
    // DISPLAY ==================================================================//
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

    // START THE MAP
    quad_map(max_res_width, max_res_height);

    // END GAME SPLASH
    end_game_splash(disp_data.width, disp_data.height);
    // DESTORY ALL THE THINGS ========================================================= //
    al_destroy_sample_instance(songInstance);//Destroy song instance.
    al_destroy_sample(song);//Destroy song.
    al_destroy_display(display);//destroy display.
    return 0;
}
