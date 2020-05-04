//
// Created by haoleng on 20-3-25.
//

#ifndef PROJ_ANDROID_GAMESCENE_H
#define PROJ_ANDROID_GAMESCENE_H


#include "cocos2d.h"
#include "Hero.h"
#include "Box2D/Box2D.h"
#include "Box2DHelper.h"
#include "GoScene.h"
#include "WelcomeScene.h"

class GameScene : public cocos2d::Layer,public b2ContactListener{
private:
    cocos2d::Size vis_size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 ori_size = cocos2d::Director::getInstance()->getVisibleOrigin();

    b2World *world;
    cocos2d::Sprite* player;
    cocos2d::Sprite* bg1;
    cocos2d::Label* label;
    int score;

    bool left = false;bool right = false;
    bool isTouch = false;
    bool isContacted = false;
    bool next_add = true;       //添加胜利之后界面的标志
    bool player_win = false;
    bool player_died = false;
    bool gameover = false;
    bool gamelose = false;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void update(float delta);
    void initBackground();

    virtual void BeginContact(b2Contact *contact);
    virtual void EndContact(b2Contact *contact);
    void addGround();
    void add_Dead_field();
    void addBar(int i);
    void addPlayer();
    void addDoor();

    void logic();
    void GameWin();
    void GameLose();

    CREATE_FUNC(GameScene);

};
#endif //PROJ_ANDROID_GAMESCENE_H
