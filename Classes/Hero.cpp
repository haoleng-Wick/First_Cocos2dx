//
// Created by haoleng on 20-3-24.
//

#include "Hero.h"

USING_NS_CC;

bool Hero::init() {
    Sprite::init();
    setScale(0.5);
    //向右跑动姿势
    auto run_cache = SpriteFrameCache::getInstance();
    run_cache->addSpriteFramesWithFile("run.plist");
    Vector<SpriteFrame* > runs(9);
    char run_str[160] = {};
    for(int i =1;i<=9;i++){
        sprintf(run_str,"Run%d.png",i);
        auto hero_frame = run_cache->getSpriteFrameByName(run_str);
        runs.pushBack(hero_frame);
    }
    auto run_r_animation = Animation::createWithSpriteFrames(runs,0.1f);
    auto run_r_animate = Animate::create(run_r_animation);
    //添加动作
    createWithSpriteFrameName("Idle1.png");
//        runAction(RepeatForever::create(run_r_animate));
    return true;
}