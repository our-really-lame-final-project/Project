void CameraUpdate(float *cameraPosition, int screenw, int screenh, float x, float y, int width, int height)
{
    // x & y are the player pos. + height and width of char cuz the player pos
    // is the top left corner of box
    // screen / 2 to get to the middle
    cameraPosition[0] = -(screenw/ 2.0) + (x + width / 2);
    cameraPosition[1] = -(screenh / 2.0) + (y + height / 2);
    if (cameraPosition[0] < 0)
    {
        cameraPosition[0] = 0;
    }
    if(cameraPosition[0] > (58*64) - (screenw) - 32)
    {
        cameraPosition[0] = (58*64) - (screenw - 32);
    }
    if(cameraPosition[1] < 0 )
    {
        cameraPosition[1] = 0;
    }
    if(cameraPosition[1] > 2230 - screenh - 32)
    {
        cameraPosition[1] = 2230 - screenh - 32;
    }
}
