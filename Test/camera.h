void CameraUpdate(float *cameraPosition, float x, float y, int width, int height)
{
    cameraPosition[0] = -(800 / 2) + (x + width / 2);
    cameraPosition[1] = -(600 / 2) + (y + height / 2);
    if(cameraPosition[0] < 0)
        cameraPosition[0] = 0;
    if(cameraPosition[1] < 0)
        cameraPosition[1] = 0;
}
