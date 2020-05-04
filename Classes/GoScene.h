//
// Created by haoleng on 20-3-26.
//

#ifndef PROJ_ANDROID_GOSCENE_H
#define PROJ_ANDROID_GOSCENE_H

#include <cocos/scripting/deprecated/CCDeprecated.h>
#include "cocos2d.h"

class GoScene :public cocos2d::Layer{
private:
    int count  = 0;
    cocos2d::Size vis_size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin_size = cocos2d::Director::getInstance()->getVisibleOrigin();
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();

    void add_edge();
    void add_ball(cocos2d::Vec2 vec2);
    void add_bar(cocos2d::Vec2 vec2);
    void add_box(cocos2d::Vec2 vec2);
    void add_wubianxing(cocos2d::Vec2 vec2);

    void Go_back(cocos2d::Ref* pSender);
    void Start_gravity();
    CREATE_FUNC(GoScene);
};


#endif //PROJ_ANDROID_GOSCENE_H
