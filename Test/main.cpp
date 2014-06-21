// C++ ALLEGRO 5 MADE EASY TUTORIAL 4 - WINDOW PROPERTIES
// CODINGMADEEASY

#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>

int main()
{
    if(!al_init())
    {
        al_show_native_message_box(NULL, NULL, "Error", "Could not initialize Allegro 5", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    al_set_window_position(display, 200, 100);
    al_set_window_title(display, "Gaming Project");

    if(!display)
    {
        al_show_native_message_box(NULL, NULL, "Error", "Could not create Allegro 5 display", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_rest(5.0);
    al_destroy_display(display);

    return 0;
}
