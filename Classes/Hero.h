//
// Created by haoleng on 20-3-24.
//

#ifndef PROJ_ANDROID_HERO_H
#define PROJ_ANDROID_HERO_H

#include "cocos2d.h"

class Hero :public cocos2d::Sprite{
public:

    virtual bool init();
    CREATE_FUNC(Hero);
};


#endif //PROJ_ANDROID_HERO_H
