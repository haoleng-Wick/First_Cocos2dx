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
    createWithSpriteFrameName("Idle__003.png");
    return true;
}