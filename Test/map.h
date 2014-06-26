#define BlockSize 64

using namespace std;

enum LoadState { TileSet, Map };
ALLEGRO_BITMAP *tileSet;

// MAP LOAD FROM FILE.
/* ---------------------------------------------------------------------------------------------------------------------- */

void LoadMap(const char *filename, vector< vector <int> > &map)
{
    // Set state to NULL status.
    int state = NULL;

    // Set openfile to object for use in ifstream.
    ifstream openfile(filename);

    // If statement for if a file is open.
    if(openfile.is_open())
    {
        string line, value;
        int space;

        // While loop to grab the lines from the text file.
        while(!openfile.eof())
        {
            getline(openfile, line);

            // If, else, and switch statements to grab tilesets for the map image.
            if(line.find("[TileSet]") != string::npos)
            {
                state = TileSet;
                continue;
            }
            else if (line.find("[Map]") != string::npos)
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
                    stringstream str(line);
                    vector<int> tempVector;

                    while(!str.eof())
                    {
                        getline(str, value, ' ');
                        if(value.length() > 0)
                            tempVector.push_back(atoi(value.c_str()));
                    }
                    map.push_back(tempVector);
                    break;
            }
        }
    }

    // Else statement.
    else
    {
        // Do nothing.
        ;
    }
}

// DRAW MAP FUNCTION.
/* ---------------------------------------------------------------------------------------------------------------------- */

void DrawMap(vector <vector <int> > map)
{
    // Nested for loops to draw out the map appropriately.
    for(int i = 0; i < map.size(); i++)
    {
        for(int j = 0; j < map[i].size(); j++)
        {
            al_draw_bitmap_region(tileSet, map[i][j] * BlockSize, 0, BlockSize,
                    64, j * BlockSize, i * BlockSize, NULL);
            if (map[1][j] == 9)
            {
                // Do nothing.
                ;
            }
        }
    }
}
