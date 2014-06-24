bool Collision(float x, float y, float ex, float ey, int width, int height)
{
    if(x + width < ex || x > ex + width || y + height < ey || y > ey + height)
    {
        return false;
    }
    return true;
}
