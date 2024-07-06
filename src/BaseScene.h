//
// Created by ken-soares on 06/07/2024.
//

#ifndef BASESCENE_H
#define BASESCENE_H



class BaseScene {
public:
    BaseScene();
    virtual ~BaseScene();
    virtual void draw();
    virtual int update(int count);
};



#endif //BASESCENE_H
