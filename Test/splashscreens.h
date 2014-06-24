void intro_splash(int width, int height)
{
    ALLEGRO_FONT *font = al_load_font("orbitron-black.ttf", 36, NULL);//Font input.
    //Draws text with given font.
    al_draw_text(font, al_map_rgb(44, 117, 255), width / 2,
            height / 2, ALLEGRO_ALIGN_CENTRE, "JAMAL QUEST");
    al_flip_display();//shows the font.
    al_rest(1.0);//sets screen timer to 1.0.
    al_destroy_font(font);//destroy font.
}

void end_game_splash(int width, int height)
{
    ALLEGRO_FONT *font1 = al_load_font("orbitron-black.ttf", 36, NULL);//Font input.
    al_draw_text(font1, al_map_rgb(44, 117, 255), width / 2, height / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    al_flip_display();//shows the font.
    al_rest(1.0);//sets screen timer.
    al_destroy_font(font1);//destroy font.
}
