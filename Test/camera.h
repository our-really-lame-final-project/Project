void CameraUpdate(float *cameraPosition, int screenw, int screenh, float x, float y, int width, int height)
{
    cameraPosition[0] = -(screenw/ 2.0) + (x + width / 2);
    cameraPosition[1] = -(screenh / 2.0) + (y + height / 2);
    if(cameraPosition[0] < 0)
        cameraPosition[0] = 0;
    if(cameraPosition[1] < 0)
        cameraPosition[1] = 0;
}
