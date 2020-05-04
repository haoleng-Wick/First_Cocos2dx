//
// Created by haoleng on 20-3-24.
//

#ifndef PROJ_ANDROID_WELCOMESCENE_H
#define PROJ_ANDROID_WELCOMESCENE_H

#include <cocos/editor-support/cocostudio/SimpleAudioEngine.h>
#include "cocos2d.h"
#include "Hero.h"
#include "GameScene.h"
#include "or_size.h"

using namespace CocosDenshion;

enum _CategoryMask{
    WORLD_BORDER = 0x0001,		// 世界边界
    BALL         = 0x0002,		// 小球
    DEAD_FIELD   = 0x0004,		// 死亡区域
    BORDER       = 0x0010,		// 踏板
    HERO         = 0x0012,      //英雄
};

class WelcomeScene :public cocos2d::Layer{

private:
    cocos2d::Sprite* hero;
    cocos2d::Size vis_size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin_size = cocos2d::Director::getInstance()->getVisibleOrigin();
    cocos2d::Animate* stand_animate;

    SimpleAudioEngine *_engine;
    unsigned  int _audioID;
    bool _loop;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void update(float delta);

    void init_music();
    void init_Background();
    void init_Menu();
    void init_hero(float x);
    void init_herostart();

    void Menu_ExitCallback(cocos2d::Ref* pSender);
    void Menu_StartCallback(cocos2d::Ref* pSender);
    void Menu_OtherCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(WelcomeScene);
};


#endif //PROJ_ANDROID_WELCOMESCENE_H
