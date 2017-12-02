#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "yspng.h"
#include "yspngenc.h"

#include "fssimplewindow.h"
#include "ysglfontdata.h"
//#include "visualization.h"
//#include "detect.h"
//#include "fssimplewindow.h"
//#include "yspng.h"
#include "menu.h"
#include "class.h"

//using namespace std;

int main(void)
{
    srand((int)time(nullptr));
    
    FsOpenWindow(0,0,800,600,1);
    
    for(;;)
    {
        GameMenu menu;
        menu.RunGameMenu();
        if(FSKEY_ESC==menu.key)
        {
            break;
        }
        else if(FSKEY_S==menu.key)
        {
            BridgeConstruct game;
            game.RunGame();
        }
    }
    
    return 0;
}


