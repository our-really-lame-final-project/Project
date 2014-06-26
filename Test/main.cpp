/* Alexander Woo, Kevin Ortiz, Michael Ackerman.
 * Jamal Quest
 * CS002 - Final Project Design.
 * Last Modified: June 25th, 2014.
 *
 * DESIGN:
 *______________________________________________________________________
 *
 * INPUT:
 * 1. The user should use permitted keys to move their characters around to dodge stuff.
 *    Only a couple of keys will be valid. Up, left, right, and down arrows will be for movement.
 *    Escape key will permit the user to exit the game.
 *    Also, another button will be used to enter the in start screen.
 *    ADDED BIT: A key for in-game map editing for later use.
 *
 * 2. No real validation needed besides the keys used for movement and etcetera.
 *    Any invalid key should be automatically ignored.
 *    Escape, arrow direction buttons, and 2 other buttons for use only.
 *
 * 3. Pseudocode:
 *    All this Allegro stuff about key inputs and stuff.
 *    Specifics come later. Set constants for the movement option though.
 *    Bunch of function calls to Allegro classes for this stuff though.
 *    Stuff like sound, sprite reading, and other game stuff should be called.
 *
 *______________________________________________________________________
 *
 * PROCESSING:
 * 1. The game should process the user’s inputs to either move their character sprite or whatever other option.
 *    From that, the game should spawn mobs to go after the player.
 *    Preferably, the player would be programmed to have a weapon of some sort.
 *    But that isn’t a priority until we understand more Allegro.
 *    The players and the mobs should recognize boundaries of walls and objects, and be repelled from them.
 *    Also, the mobs should instantly end the game the moment they touch the player.
 *
 * 2. Multiple functions and headers and classes will be called for use.
 *    The Allegro classes the most glaringly obvious.
 *    The functions/header that should be made will reduce the amount of clutter stuck in the main file.
 *
 * 3. Psuedocode:
 * functions for moving the character thanks to player input.
 * functions to make mobs and make them move towards the player.
 * functions for making the player and the mobs recognize out of bound areas and walls/objects.
 * function for centering the camera around the player at all times. Also recognize game out of bound regions and centering appropriately.
 * functions for drawing the in-game map.
 * functions for redrawing the map and character sprites appropriately.
 *
 * OUTPUT:
 * 1.  No output, besides a game over screen if the player fails the game.
 *     But visual output from the game will be recognized.
 *     Quite a number of changes occur due to user interaction.
 *
 * 2. No need for output units of measure.
 *    Nothing is returned besides game visual feedback to the player.
 *
 *3. Pseudocode:
 * Those functions mentioned in the processing pseudocode is the same for output.
 * It should redraw any changes, as it should be all looped until the player loses or manually exits the program.
 *
 * END OF DESIGN.
 */

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
    al_install_mouse();


    // SOUND SETUP =================================================================== //
    // =============================================================================== //
    al_reserve_samples(1);//Number of samples playing.
    ALLEGRO_SAMPLE *song = al_load_sample("TestGetLucky8Bit.ogg"); //Load the song file.
    if (!song)
    {
        //al_show_native_message_box(NULL, NULL, "Error", "Could not load song", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        //return -1;
    }

    //Attaches song into sample instance to create it.
    ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);
    //Choose play mode for song like looping the song.
    al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
    //Attach the song to the default mixer.
    al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
    al_play_sample_instance(songInstance); //Starts the song.


    // TIMER
    ALLEGRO_TIMER *globaltimer = NULL;

    //SETUP GLOBAL EVENT QUEUE
    ALLEGRO_EVENT_QUEUE *global_queue = NULL;
    global_queue = al_create_event_queue();
    al_register_event_source(global_queue, al_get_display_event_source(display));
    al_register_event_source(global_queue, al_get_mouse_event_source());


    //intro splash
    intro_splash(disp_data.width, disp_data.height);

    while(1)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(global_queue, &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            quad_map(disp_data.width, disp_data.height);
            break;
        }
    }


    al_destroy_event_queue(global_queue);

    // END GAME SPLASH
    end_game_splash(disp_data.width, disp_data.height);

    // DESTORY ALL THE THINGS ========================================================= //
    al_destroy_sample_instance(songInstance);//Destroy song instance.
    al_destroy_sample(song);//Destroy song.
    al_destroy_display(display);//destroy display.
    return 0;
}
