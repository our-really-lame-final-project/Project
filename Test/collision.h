// COLLISION BOOLEAN.
/* ---------------------------------------------------------------------------------------------------------------------- */

bool Collision(float x, float y, float ex, float ey, int width, int height)
{
    // A if statement for checking player's location to a restricted location.
    if(x + width < ex || x > ex + width || y + height < ey || y > ey + height)
    {
        // False, since player is not in a restricted zone.
        return false;
    }

    // Then return true if ever.
    return true;
}

// OBSTRUCT / REPEL FUNCTION.
/* ---------------------------------------------------------------------------------------------------------------------- */

float obstruct(float& x, float& y, int dir, float moveSpeed)
{
    // If a player / npc is in a restricted location, repel them the opposite direction.
    // from the top
    if(dir == 0)
    {
        y -= moveSpeed;
    }
    // if coming from the right

    else if(dir == 1)
    {
        x += moveSpeed;
    }

    // if coming from the left
    else if(dir == 2)
    {
        x -= moveSpeed;
    }
    // if coming from the bottom
    else if(dir == 3)
    {
        y += moveSpeed;
    }
}
