bool Collision(float x, float y, float ex, float ey, int pradius, int eradius)
{//Pythagoream theorem: using hypontenuse to find distance between objects.
    if(sqrt(pow(x - ex, 2) + pow(y - ey, 2)) < pradius + eradius)
    {
        return true;//Collision.
    }
    return false;//No collision.
}
