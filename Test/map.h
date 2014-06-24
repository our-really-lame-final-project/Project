#define BlockSize 64
enum LoadState { TileSet, Map };
ALLEGRO_BITMAP *tileSet;
void LoadMap(const char *filename, std::vector< std::vector <int> > &map)
{
    int state = NULL;
    std::ifstream openfile(filename);
    if(openfile.is_open())
    {
        std::string line, value;
        int space;

        while(!openfile.eof())
        {
            std::getline(openfile, line);

            if(line.find("[TileSet]") != std::string::npos)
            {
                state = TileSet;
                continue;
            }
            else if (line.find("[Map]") != std::string::npos)
            {
                state = Map;
                continue;
            }

            switch(state)
            {
            case TileSet:
                if(line.length() > 0)
                    tileSet = al_load_bitmap(line.c_str());
                break;
            case Map:
                std::stringstream str(line);
                std::vector<int> tempVector;

                while(!str.eof())
                {
                    std::getline(str, value, ' ');
                    if(value.length() > 0)
                        tempVector.push_back(atoi(value.c_str()));
                }
                map.push_back(tempVector);
                break;
            }
        }
    }
    else
    {
    }
}

void DrawMap(std::vector <std::vector <int> > map)
{
    for(int i = 0; i < map.size(); i++)
    {
        for(int j = 0; j < map[i].size(); j++)
        {
            al_draw_bitmap_region(tileSet, map[i][j] * BlockSize, 0, BlockSize, 64, j * BlockSize,
                i * BlockSize, NULL);
        }
    }
}
