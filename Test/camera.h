// FUNCTION FOR UPDATING CAMERA POSITION.
/* ---------------------------------------------------------------------------------------------------------------------- */

void CameraUpdate(float *cameraPosition, int screenw, int screenh, float x, float y, int width, int height)
{
    // x & y are the player position plus height and width of char because the player position
    // is the top left corner of box screen, divide by 2 to get to the middle.
    cameraPosition[0] = -(screenw/ 2.0) + (x + width / 2);
    cameraPosition[1] = -(screenh / 2.0) + (y + height / 2);

    // The following if statements are for camera boundaries. It prevents the camera from leaving bounded regions.
    if (cameraPosition[0] < 0)
    {
        cameraPosition[0] = 0;
    }

    if(cameraPosition[0] > (58*64) - screenw)
    {
        cameraPosition[0] = (58*64)- screenw;
    }

    if (cameraPosition[1] < 0)
    {
        cameraPosition[1] = 0;
    }

    if(cameraPosition[1] > (35*64) - screenh)
    {
        cameraPosition[1] = (35*64) - screenh;
    }
}
