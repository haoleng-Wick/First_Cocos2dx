//
// Created by haoleng on 20-3-24.
//

#include <AudioEngine.h>
#include "WelcomeScene.h"
#include "../cocos2d/cocos/scripting/deprecated/CCDeprecated.h"
#include "GoScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;


Scene* WelcomeScene::createScene() {
    auto scene = Scene::create();
    auto layer = WelcomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool WelcomeScene::init() {

    _engine = nullptr;
    _audioID = 0;
    _loop = false;
    this->scheduleUpdate();
    init_music();
    init_Background();
    init_Menu();
    init_hero(origin_size.x+70);
    or_sizex=origin_size.x;
    or_sizey=origin_size.y;

    return true;
}

void WelcomeScene::init_music() {
    _engine = SimpleAudioEngine::getInstance();
    _engine->preloadBackgroundMusic("music/cano.mp3");
    _engine->preloadBackgroundMusic("welcome_bgm.mp3");
    _engine->preloadEffect("music/bt.mp3");
    _engine->preloadEffect("music/down.ogg");
    _engine->preloadEffect("music/fail.mp3");
    _engine->preloadEffect("music/good.mp3");

    _engine->playBackgroundMusic("music/welcome_bgm.mp3",_loop);
}

void WelcomeScene::init_Background() {
    auto bg = Sprite::create("bg.jpg");
    bg->setPosition(Vec2(vis_size.width/2+origin_size.x,
                      vis_size.height/2+origin_size.y));
    bg->setScale(2.0);
    bg->runAction(RepeatForever::create(RotateBy::create(30,360)));
    this->addChild(bg,0);
}
void WelcomeScene::init_Menu() {
    auto StartItem = MenuItemFont::create("Start",CC_CALLBACK_1(WelcomeScene::Menu_StartCallback,this));
    auto OtherItem = MenuItemFont::create("MiniGame",CC_CALLBACK_1(WelcomeScene::Menu_OtherCallback,this));
    auto ExitItem = MenuItemFont::create("Exit",CC_CALLBACK_1(WelcomeScene::Menu_ExitCallback,this));

    StartItem->setScale(2.0f);
    OtherItem->setScale(2.0f);
    ExitItem->setScale(2.0f);
    StartItem->setPosition(Vec2(vis_size.width/2+origin_size.x,
                                vis_size.height/2+origin_size.y+80));
    OtherItem->setPosition(Vec2(vis_size.width/2+origin_size.x,
                                vis_size.height/2+origin_size.y));
    ExitItem->setPosition(Vec2(vis_size.width/2+origin_size.x,
                               vis_size.height/2+origin_size.y-80));

    auto Menu = Menu::create(StartItem,OtherItem,ExitItem,NULL);
    Menu->setPosition(Vec2::ZERO);
    this->addChild(Menu,2);
}
void WelcomeScene::init_hero(float x) {
        hero = Hero::create();
        hero->setPosition(Vec2(x,vis_size.height/3+origin_size.y));
    auto hero_cache = SpriteFrameCache::getInstance();
    hero_cache->addSpriteFramesWithFile("static.plist");
    Vector<SpriteFrame* > stands(10);
    char stand_str[200] = {};
    for(int i =1;i<=10;i++){
        sprintf(stand_str,"Idle%d.png",i);
        auto hero_frame = hero_cache->getSpriteFrameByName(stand_str);
        stands.pushBack(hero_frame);
    }
    auto stand_animation = Animation::createWithSpriteFrames(stands,0.1f);
    stand_animate = Animate::create(stand_animation);
    hero->runAction(RepeatForever::create(stand_animate));
        this->addChild(hero,1);
}

void WelcomeScene::init_herostart() {
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFadeDown::create(1,scene));
}

//菜单事件
void WelcomeScene::Menu_StartCallback(cocos2d::Ref *pSender) {
    _engine->stopBackgroundMusic();

    _engine->playEffect("music/bt.mp3",_loop);
    ParticleSystem* ps = ParticleFlower ::create();
    ps->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
    ps->setPosition(Vec2(vis_size.width/2+origin_size.x,
                         vis_size.height/2+origin_size.y+80));
    this->addChild(ps,1);
    auto callfunc = CallFunc::create(std::bind(&WelcomeScene::init_herostart,this));
    hero->runAction(Sequence::create(DelayTime::create(1),callfunc,NULL));
}

void WelcomeScene::Menu_OtherCallback(cocos2d::Ref *pSender) {
    _engine->playEffect("music/bt.mp3",_loop);
    float xx = hero->getPositionX();
    hero->removeFromParent();
    init_hero(xx);

    auto scene = GoScene::createScene();
    Director::getInstance()->pushScene(TransitionFadeUp::create(1,scene));
}

void WelcomeScene::Menu_ExitCallback(cocos2d::Ref *pSender) {
    Director::getInstance()->end();
}

void WelcomeScene::update(float delta) {
    float x= hero->getPositionX()-hero->getContentSize().width/5;
    if(x>origin_size.x+vis_size.width){
        hero->setPositionX(origin_size.x-hero->getContentSize().width/5);
    }
}