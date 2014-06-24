bool Collision(float x, float y, float ex, float ey, int width, int height)
{
    if(x + width < ex || x > ex + width || y + height < ey || y > ey + height)
    {
        return false;
    }
    return true;
}

float obstruct(float& x, float& y, int dir, float moveSpeed)
{
    // from the top
    if(dir == 0)
        y -= moveSpeed;
    // if comming from the right
    else if(dir == 1)
        x += moveSpeed;
    // if coming from the left
    else if(dir == 2)
        x -= moveSpeed;
    // if coming from the bottom
    else if(dir == 3)
        y += moveSpeed;
}
