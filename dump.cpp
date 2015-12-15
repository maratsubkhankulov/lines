    /*
    // Employ the stupid search
    // Here search through the array iteratively
    
    int lastIndex = 0;
    int lastX = -1;
    int lastY = 0;
    int x = 0, y = 0;
    
    // Add the first tile to be part of the path
    pathX[lastIndex] = selectedBallX;
    pathY[lastIndex] = selectedBallY;
    lastX = selectedBallX;
    lastY = selectedBallY;
    lastIndex++;
    
    // iterate over the grid starting at tile A (selected tile)
    // if this tile is not selected, unoccupied and is a neighbour on the leading tile
    for (int n = 0; n < 6; n++)
    {
        for (int w = 0; w < gW; w++)
        {
         for (int h = 0; h < gH; h++)
         {
             if (neighbours(lastX, lastY, w, h) && balls[w][h] == NULL && T[w][h] == 'u')
             {
                 // add this tile to the path
                 T[w][h] = 's';
                 pathX[lastIndex] = w;
                 pathY[lastIndex] = h;
                 lastX = w;
                 lastY = h;
                 lastIndex++;
                 if (w == selectedTileX && h == selectedTileY)
                 {
                    lastPathIndex = lastIndex;
                    return true; // path has been found
                 }
             } else
             {
                 // set this tile as visited
                 T[w][h] = 'u';
             }
         }
        }
    }            
    
    // end of Dumb Search
    */
