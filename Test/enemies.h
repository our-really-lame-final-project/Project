// FUNCTION FOR ZOMBIE MOVEMENT.
/* ---------------------------------------------------------------------------------------------------------------------- */

void BrainsZombie(float x, float y, float& zx, float& zy, int& dir, float moveSpeed)
{
    // An if statement to check for zombie collision.
    if (!Collision(x, y, zx, zy, 64, 64))
    {
        // An if statement to check for if the x coordinate for the zombie is less than the player's position.
        // If so, then move towards the player one frame.
        if (zx < x)
        {
            dir = 2;
            zx += moveSpeed / 2.25;
        }

        // An if statement to check for if the x coordinate for the zombie is greater than the player's position.
        // If so, then move towards the player one frame.
        if (zx > x)
        {
            dir = 1;
            zx -= moveSpeed / 2.25;
        }

        // An if statement to check for if the y coordinate for the zombie is less than the player's position.
        // If so, then move towards the player one frame.
        if (zy < y)
        {
            dir = 0;
           zy += moveSpeed / 2.25;
        }

        // An if statement to check for if the y coordinate for the zombie is greater than the player's position.
        // If so, then move towards the player one frame.
        if (zy > y)
        {
            dir = 3;
            zy -= moveSpeed / 2.25;
        }
    }

    // Return null, end of function.
    return;
}
