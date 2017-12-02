#include <stdio.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "class.h"
#include "menu.h"

void GameMenu::RunGameMenu(void)
{
    for(;;)
    {
        FsPollDevice();
        key=FsInkey();
        
        if(FSKEY_ESC==key || FSKEY_S==key)
        {
            return;
        }
        
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        
        DrawSampleBridge();
        DrawBackground();
        
        glColor3ub(0,255,0);
        glRasterPos2i(10,20);
        YsGlDrawFontBitmap12x16("S......START");
        glRasterPos2i(10,40);
        YsGlDrawFontBitmap12x16("ESC....CLOSE");		
        
        FsSwapBuffers();
        
        FsSleep(25);
    }
}


