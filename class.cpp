//
//  class.cpp
//  ps9
//
//  Created by Amethyst on 2017/12/1.
//  Copyright © 2017年 CMU. All rights reserved.
//

#include <stdio.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "yspngenc.h"

#include "class.h"
#include "menu.h"


#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "FEMModel.h"
using namespace std;
//////////////////////////////////////////////////////////draw
void DrawBackground()
{
    glColor3ub(160,82,45);
    glBegin(GL_POLYGON);
    glVertex2i(0,300);
    glVertex2i(100,300);
    glVertex2i(150,400);
    glVertex2i(140,480);
    glVertex2i(200,600);
    glVertex2i(0,600);
    glVertex2i(0,300);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2i(800,300);
    glVertex2i(700,300);
    glVertex2i(660,440);
    glVertex2i(600,480);
    glVertex2i(560,600);
    glVertex2i(800,600);
    glVertex2i(800,300);
    glEnd();

    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);

    glColor3ub(32,178,170);
    glVertex2i(170,540);

    glColor3ub(32,178,170);
    glVertex2i(580,540);

    glColor3ub(255,255,255);
    glVertex2i(185,570);

    glColor3ub(255,255,255);
    glVertex2i(570,570);
    glEnd();
    //
    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);

    glColor3ub(255,255,255);
    glVertex2i(570,570);

    glColor3ub(255,255,255);
    glVertex2i(185,570);

    glColor3ub(32,178,170);
    glVertex2i(560,600);

    glColor3ub(32,178,170);
    glVertex2i(200,600);
    glEnd();

}

void DrawSampleBridge()
{
    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2i(100,300);
    glVertex2i(700,300);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2i(100,300);
    glVertex2i(175,225);
    glVertex2i(250,300);
    glVertex2i(325,225);
    glVertex2i(400,300);
    glVertex2i(475,225);
    glVertex2i(550,300);
    glVertex2i(625,225);
    glVertex2i(700,300);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(175,225);
    glVertex2i(625,225);
    glEnd();

}

void DrawNotice()
{
    YsRawPngDecoder png;
    png.wid = png.wid ;
    png.hei = png.hei ;
    if (YSOK == png.Decode("vehicles.png"))
    {
        png.Flip();
    }
    else
    {
        printf("Cannot open file.\n");
    }
    glRasterPos2i(550,80);
    glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);

}


////////////////////////////////////////////////////////////car
void Car::car_initial(int type)
{
    if(type==1)
    {
        weight=30;
    }
    if(type==2)
    {
        weight=40;
    }
    if(type==3)
    {
        weight=50;
    }

    velocity=5;
    state=0;
    x=0;
    y=299;
}

void Car::Run()
{
    x=x+velocity;

}
void Car::Judge()
{
}
void Car::Arrive()
{
}

void Car::Fall()
{
    if(x<=800)
    {
        x=x+velocity;
        double dt=0.2;
        double vy=9.8*dt;
        y+=vy;
    }
}

void Car::Draw(int type)
{
    YsRawPngDecoder png;
    png.wid = png.wid ;
    png.hei = png.hei ;

    if(type==1)
    {
        if (YSOK == png.Decode("vehicle1.png"))
        {
            png.Flip();
        }
        else
        {
            printf("Cannot open file.\n");
        }
    }
    if(type==2)
    {
        if (YSOK == png.Decode("vehicle2.png"))
        {
            png.Flip();
        }
        else
        {
            printf("Cannot open file.\n");
        }

    }
    if(type==3)
    {
        if (YSOK == png.Decode("vehicle3.png"))
        {
            png.Flip();
        }
        else
        {
            printf("Cannot open file.\n");
        }

    }

    glRasterPos2i(x, y);
    glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
}

///////////////////////////////////////////////bridgegame

BridgeConstruct::BridgeConstruct()
{
    state=0;
}

void BridgeConstruct::RunGame()
{
    Car car;

	FEMModel model;
	string tempInput = "example.txt";
	model.ReadTXTFiles(tempInput.c_str());
	model.print();

    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();

        if(FSKEY_1==key)
        {
            car.type=1;
            car.car_initial(1);
        }
        if(FSKEY_2==key)
        {
            car.type=2;
            car.car_initial(2);
        }
        if(FSKEY_3==key)
        {
            car.type=3;
            car.car_initial(3);
        }
        if(FSKEY_ESC==key)
        {
            break;
        }
        if(FSKEY_SPACE==key)
        {
            car.state=1;
        }
        if(car.x>=700)
        {
            car.state=3;
        }

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

        DrawBackground();

        DrawNotice();

        if(car.state==1)
        {
            car.Run();
        }
        if(car.state==2)
        {
            car.Fall();
        }
        if(car.state==3)
        {
            car.Arrive();
        }


        model.Assemble(car.weight,car.x);

        model.LUDecomposition();

        model.SolverU();
        model.SolverStress();

        car.Draw(car.type);


        FsSwapBuffers();
        FsSleep(25);
    }
}
