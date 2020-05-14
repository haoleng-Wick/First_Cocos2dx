//
// Created by haoleng on 20-3-26.
//

#ifndef PROJ_ANDROID_GOSCENE_H
#define PROJ_ANDROID_GOSCENE_H

#include <cocos/scripting/deprecated/CCDeprecated.h>
#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/SimpleAudioEngine.h"

using namespace CocosDenshion;

class GoScene :public cocos2d::Layer{
private:
    int count  = 0;
    int score = 0; char buffer[64];
    cocos2d::Label* mlabel;
    cocos2d::Size vis_size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin_size = cocos2d::Director::getInstance()->getVisibleOrigin();
    cocos2d::Vec2 touch_begin;
    cocos2d::Vec2 touch_end;
    cocos2d::Vec2 Hole_Position;
    cocos2d::Vec2 add_Position = origin_size+cocos2d::Vec2(10,vis_size.height/2-50);
    cocos2d::Sprite* arrow1;cocos2d::Sprite* arrow2;cocos2d::Sprite* arrow3;
    SimpleAudioEngine * simpleAudioEngine;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();

    void add_edge();
    void add_ball(cocos2d::Vec2 vec1,cocos2d::Vec2 vec2);
    void add_bar(cocos2d::Vec2 vec1,cocos2d::Vec2 vec2);
    void add_box(cocos2d::Vec2 vec1,cocos2d::Vec2 vec2);
    void add_wubianxing(cocos2d::Vec2 vec1,cocos2d::Vec2 vec2);
    void add_hole();
    void music();
    void add_arrows();

    void Go_back(cocos2d::Ref* pSender);
    void Start_gravity();
    CREATE_FUNC(GoScene);
};


#endif //PROJ_ANDROID_GOSCENE_H
