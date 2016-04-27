# A-Pathfinding
C++ A* Pathfinding implementation using SDL2

Information
======
This implementation of the A* pathfinding algorithm uses a tile map and is written so the algorithm can be extracted and used quickly and efficiently in other projects.

The algorithm is found within the [PathAlgorithm.cpp](https://github.com/rorywebdev/A-Pathfinding/blob/master/src/PathAlgorithm.cpp) file. Currently it uses SDL_Point objects to store the visual information for the path, however these can be removed with a few line changes.

Processing of the algorithm is done frame agnostically, however it is not reactive to system performance. The algorithm will use up to a specific amount of time per frame based on a hard-set limit (unless the completed path is found at which point it will end the processing) meaning time to compute the path will increase on slower machines.

The included tile map implementation uses multiple terrain types and generates random environments based on the weighting set. These can be modified in the UI to increase or decrease certain terrain types. Walls are the only impenetrable terrain type.

Input, general window management and rendering are done using SDL2.

Included Files
======
The included Visual Studio project is set up to allow instant editing (SDL2 and SDL2TTF are included).

No extra files should be required to run or open the project.

All code files are found within the **src** folder.

**A pre-built version can be found in the Release folder.**

Requirements
======
- Visual Studio 2012 (minimum)

The project is created and built in 2013, however the code is compliant with 2012's compiler and will work in either.
