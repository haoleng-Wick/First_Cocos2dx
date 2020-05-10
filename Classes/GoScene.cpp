//
// Created by haoleng on 20-3-26.
//

#include "GoScene.h"

USING_NS_CC;

Scene* GoScene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->getPhysicsWorld()->setUpdateRate(1);
    scene->getPhysicsWorld()->setGravity(Vec2(0,-270.8f));
    auto layer = GoScene::create();
    scene->addChild(layer);
    return scene;
}

bool GoScene::init() {
    auto Listener =EventListenerTouchOneByOne::create();
    Listener->onTouchBegan = [this](Touch* touch,Event* event){
        touch_begin=Vec2(touch->getLocation().x,touch->getLocation().y);
        return true;
    };
    Listener->onTouchEnded=[this](Touch* touch,Event *event){
        touch_end=Vec2(touch->getLocation().x,touch->getLocation().y);
        if(count<=60){
            count++;
            auto ve = touch_end-touch_begin;
            auto select = random(1,50);
            int num = select%4;
            if(num == 0)
                add_ball(touch_begin,ve);
            else if(num == 1)
                add_bar(touch_begin,ve);
            else if(num == 2)
                add_box(touch_begin,ve);
            else if(num == 3)
                add_wubianxing(touch_begin,ve);
        } else{
            Start_gravity();
        }
        return true;
    };
    auto _LListener = EventListenerAcceleration::create([this](Acceleration *a, Event *)
                                                        {
                                                            auto vec2 = Vec2((float)a->x,(float)a->y);
                                                            vec2.normalize();
                                                            this->getScene()->getPhysicsWorld()->setGravity(vec2*500);
                                                            return true;
                                                        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_LListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(Listener,this);
    return true;
}
void GoScene::onEnter() {
    Layer::onEnter();

    add_edge();
}

void GoScene::add_bar(Vec2 vec1,Vec2 vec2) {
    auto bar = Sprite::create("bar.png");
    auto bar_body = PhysicsBody::createBox(bar->getContentSize(),
            PhysicsMaterial(3.0f,0.8f,0.6f));
    bar_body->setDynamic(true);
    bar_body->setVelocity(vec2);

    bar->setPhysicsBody(bar_body);
    bar->setPosition(vec1);
    this->addChild(bar);
}
void GoScene::add_ball(Vec2 vec1,Vec2 vec2) {
    auto ball = Sprite::create("basketball.png");
    ball->setScale(0.5f);
    auto ball_body = PhysicsBody::createCircle(ball->getContentSize().width/2,
                                            PhysicsMaterial(1.5f,0.8f,0.2f));
    ball_body->setDynamic(true);
    ball_body->setVelocity(vec2);

    ball->setPhysicsBody(ball_body);
    ball->setPosition(vec1);
    this->addChild(ball);
}
void GoScene::add_box(Vec2 vec1,Vec2 vec2) {
    auto box = Sprite::create("smile.png");
    auto box_body = PhysicsBody::createBox(box->getContentSize(),
            PhysicsMaterial(3.0f,0.7f,0.4f));
    box_body->setDynamic(true);
    box_body->setVelocity(vec2);

    box->setPhysicsBody(box_body);
    box->setPosition(vec1);
    this->addChild(box);
}
void GoScene::add_wubianxing(Vec2 vec1,Vec2 vec2) {
    auto sjx = Sprite::create("sjx.png");
    float x = sjx->getContentSize().width;
    float y = sjx->getContentSize().height;
    Point vertex[] ={
            Point(-x/3.1f,-y/4.0f),
            Point(x/1.6f-x/3.1f,-y/4.0f),
            Point(x/3.4f-x/3.1f,y/1.8f-y/4.0f)
    };
    auto sjx_body = PhysicsBody::createPolygon(vertex,3,PhysicsMaterial(2.5f,0.7f,0.5f));
    sjx_body->setDynamic(true);
    sjx_body->setVelocity(vec2);

    sjx->setPhysicsBody(sjx_body);
    sjx->setPosition(vec1);
    this->addChild(sjx);
}

void GoScene::add_edge() {
    Node* node =Node::create();
    auto edges = PhysicsBody::createEdgeBox(vis_size,PHYSICSBODY_MATERIAL_DEFAULT,2);
    node->setPosition(Vec2(vis_size.width/2+origin_size.x,vis_size.height/2+origin_size.y));
    node->setPhysicsBody(edges);
    this->addChild(node);

}

void GoScene::Start_gravity() {
        ParticleSystem* ps = ParticleRain::create();
        ps->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
        ps->setPosition(Vec2(vis_size.width/2+origin_size.x,
                             vis_size.height+origin_size.y));
        this->addChild(ps,1);
        Device::setAccelerometerEnabled(true); //启动重力传感器

        auto ExitItem = MenuItemFont::create("Back",CC_CALLBACK_1(GoScene::Go_back,this));
        ExitItem->setPosition(Vec2(vis_size.width+origin_size.x-50,
                               20+origin_size.y));
        auto Menu = Menu::create(ExitItem,NULL);
        Menu->setPosition(Vec2::ZERO);
        this->addChild(Menu,2);
}
void GoScene::Go_back(cocos2d::Ref *pSender) {
    Director::getInstance()->popScene();
    Device::setAccelerometerEnabled(false);
}

