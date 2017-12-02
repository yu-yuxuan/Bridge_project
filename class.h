//
//  class.hpp
//  ps9
//
//  Created by Amethyst on 2017/12/1.
//  Copyright © 2017年 CMU. All rights reserved.
//

#ifndef class_h
#define class_h

void DrawBackground();
void DrawSampleBridge();

class Car
{
public:
    int type;
    double weight;
    double velocity;
    double x;
    double y;
    int state;//0:stop;   1:normal run;   2:fall;   3:arrive and stop
    void car_initial(int type);
    void Run();
    void Judge();
    void Fall();
    void Arrive();
    void Draw(int type);
};

class BridgeConstruct
{
public:
    int state;
    void RunGame();
    BridgeConstruct();
};


#endif /* class_hpp */
