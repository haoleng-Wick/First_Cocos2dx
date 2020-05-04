//
// Created by haoleng on 20-3-26.
//

#ifndef PROJ_ANDROID_BOX2DHELPER_H
#define PROJ_ANDROID_BOX2DHELPER_H

#define RATIO 100.0f

#include "Box2D/Box2D.h"
#include "GameScene.h"
#include "WelcomeScene.h"
#include "or_size.h"

class Box2DHelper {
public:
    static b2World* createWorld();
    static b2Body* createBox(b2World* world, float posX, float posY,
                             float width, float height,bool isStatic= false,void* userData = nullptr);
    static b2Body* createBar(b2World* world, float posX, float posY,
                             float width, float height,bool isStatic= false,void* userData = nullptr);

    static b2Body* createDeadborder(b2World* world, float posX, float posY,
                             float width, float height,bool isStatic= false,void* userData = nullptr);

    static b2Body* createHero(b2World* world, float posX, float posY,
                                float width, float height,bool isStatic= false,void* userData = nullptr);

    static b2Body* createEdge(b2World* world, float width, float height,void* userData=nullptr);
};


#endif //PROJ_ANDROID_BOX2DHELPER_H
