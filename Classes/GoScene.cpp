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

    auto ExitItem = MenuItemFont::create("Back",CC_CALLBACK_1(GoScene::Go_back,this));
    ExitItem->setColor(Color3B::GRAY);
    ExitItem->setPosition(origin_size+Vec2(vis_size.width-66,30));
    auto Menu = Menu::create(ExitItem,NULL);
    Menu->setPosition(Vec2::ZERO);
    this->addChild(Menu,2);

    mlabel = Label::createWithTTF("0","fonts/comicz.ttf",20);
    mlabel->setColor(Color3B::GREEN);
    mlabel->setPosition(origin_size+Vec2(vis_size.width/2,vis_size.height-100));
    char x = 'o';
    sprintf(buffer,"Let's G%c,move your finger!",x);
    mlabel->setString(buffer);
    mlabel->setVisible(true);
    this->addChild(mlabel,2);

    simpleAudioEngine = SimpleAudioEngine::getInstance();
    simpleAudioEngine->stopBackgroundMusic();
    auto Listener =EventListenerTouchOneByOne::create();
    Listener->onTouchBegan = [this](Touch* touch,Event* event){
        touch_begin=Vec2(touch->getLocation().x,touch->getLocation().y);
        return true;
    };
    Listener->onTouchEnded=[this](Touch* touch,Event *event){
        touch_end=Vec2(touch->getLocation().x,touch->getLocation().y);
        if(count<=100){
            count++;
            auto ve = touch_end-touch_begin;
            auto select = random(1,50);
            int num = select%4;
            if(num == 0)
                add_ball(add_Position,ve);
            else if(num == 1)
                add_bar(add_Position,ve);
            else if(num == 2)
                add_box(add_Position,ve);
            else if(num == 3)
                add_wubianxing(add_Position,ve);
        } else if(count==101){
            Start_gravity();
        }
        return true;
    };
    auto _LListener = EventListenerAcceleration::create([this](Acceleration *a, Event *)
                                                        {
                                                            auto vec2 = Vec2((float)a->x,(float)a->y);
                                                            vec2.normalize();
                                                            this->getScene()->getPhysicsWorld()->setGravity(vec2*270);
                                                            return true;
                                                        });
    auto contact_listener=EventListenerPhysicsContact::create();
    contact_listener->onContactBegin = [this](PhysicsContact& contact){
        if(count<=100){
            auto spriteA= contact.getShapeA()->getBody()->getNode();
            auto spriteB= contact.getShapeB()->getBody()->getNode();
            if(spriteA && spriteB){
                if(spriteA->getTag()==666) {
                    spriteB->runAction(Sequence::create(CallFunc::create(std::bind( &GoScene::music, this)), RemoveSelf::create(true), NULL));
                }else if(spriteB->getTag()==666) {
                    spriteA->runAction(Sequence::create(CallFunc::create(std::bind( &GoScene::music, this)), RemoveSelf::create(true), NULL));
                }
            }
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contact_listener,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_LListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(Listener,this);
    return true;
}
void GoScene::music() {
    score++;
    if(score ==1){
        sprintf(buffer,"%d person was sent away",score);
    }else if(score >=2 && score <15){
        sprintf(buffer,"just %d people were sent away",score);
    }else if(score>=15 && score <30){
        mlabel->setColor(Color3B::RED);
        sprintf(buffer,"wa ,%d people were sent away",score);
    }else{
        mlabel->setColor(Color3B::YELLOW);
        sprintf(buffer,"Cool,%d people",score);
    }
    mlabel->setString(buffer);

    auto boom = ParticleExplosion ::create();
    boom->setSpeed(300);
    boom->setPosition(Hole_Position+Vec2(0,10));
    this->addChild(boom);

    simpleAudioEngine->playEffect("music/good.mp3",false);
        auto rand = random(0,3);
        if(rand==0){add_Position=origin_size+Vec2(10,vis_size.height/2-50);
                arrow1->setVisible(true);arrow2->setVisible(false);arrow3->setVisible(false);}
        else if(rand==1){add_Position=origin_size+Vec2(10,vis_size.height/2-200);
            arrow1->setVisible(false);arrow2->setVisible(true);arrow3->setVisible(false);}
        else {add_Position=origin_size+Vec2(10,vis_size.height/2-400);
            arrow1->setVisible(false);arrow2->setVisible(false);arrow3->setVisible(true);}
}
void GoScene::onEnter() {
    Layer::onEnter();
    add_edge();
    add_hole();
    add_arrows();
}
void GoScene::add_arrows() {
    arrow1 = Sprite::create("go.png");
    arrow2 = Sprite::create("go.png");
    arrow3 = Sprite::create("go.png");

    arrow2->setVisible(false);
    arrow3->setVisible(false);
    arrow1->setPosition(origin_size+Vec2(10,vis_size.height/2-50));
    arrow2->setPosition(origin_size+Vec2(10,vis_size.height/2-200));
    arrow3->setPosition(origin_size+Vec2(10,vis_size.height/2-400));

    this->addChild(arrow1);this->addChild(arrow2);this->addChild(arrow3);
}

void GoScene::add_hole() {
    wood2 = Sprite::create("wood.png");
    wood3 = Sprite::create("wood.png");
    bottom = Sprite::create("wood.png");
    bottom->runAction(RotateBy::create(0,90));
    bottom->setTag(666);
    wood2->setScaleY(2.0f);
    wood3->setScaleY(2.0f);
    bottom->setOpacity(100);
    wood2->setOpacity(100);
    wood3->setOpacity(100);
    wood2->runAction(RotateBy::create(0,-15));
    wood3->runAction(RotateBy::create(0,15));
    auto bottom_body = PhysicsBody::createBox(bottom->getContentSize(),PhysicsMaterial(1.2f,0,0.1f));
    auto wood2_body = PhysicsBody::createBox(wood2->getContentSize(),PhysicsMaterial(1.2f,0.2f,0.1f));
    auto wood3_body = PhysicsBody::createBox(wood2->getContentSize(),PhysicsMaterial(1.2f,0.2f,0.1f));
    bottom_body->setDynamic(false);
    wood2_body->setDynamic(false);
    wood3_body->setDynamic(false);
    bottom_body->setContactTestBitmask(0x64);
    wood2_body->setContactTestBitmask(0x64);
    wood3_body->setContactTestBitmask(0x64);

    bottom->setPhysicsBody(bottom_body);
    wood2->setPhysicsBody(wood2_body);
    wood3->setPhysicsBody(wood3_body);
    bottom->setPosition(origin_size+Vec2(vis_size.width-116,vis_size.height/2-20));
    Hole_Position= origin_size+Vec2(vis_size.width-120,vis_size.height/2);
    wood2->setPosition(Hole_Position+Vec2(-31,12));
    wood3->setPosition(wood2->getPosition()+Vec2(67,0));
    this->addChild(bottom);
    this->addChild(wood2);
    this->addChild(wood3);
    bottom->runAction(RepeatForever::create(Sequence::create(FadeOut::create(3),DelayTime::create(2),FadeIn::create(3),  NULL)));
    wood2->runAction(RepeatForever::create(Sequence::create(FadeOut::create(3),DelayTime::create(2),FadeIn::create(3),  NULL)));
    wood3->runAction(RepeatForever::create(Sequence::create(FadeOut::create(3),DelayTime::create(2),FadeIn::create(3),  NULL)));

    smoke = ParticleGalaxy ::create();
    smoke->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
    smoke->setPosition(Hole_Position-Vec2(0,20));
    this->addChild(smoke);

}

void GoScene::add_bar(Vec2 vec1,Vec2 vec2) {
    auto bar = Sprite::create("bar.png");
    bar->setScale(0.5f);
    auto bar_body = PhysicsBody::createBox(bar->getContentSize(),
            PhysicsMaterial(3.0f,0.8f,0.6f));
    bar_body->setDynamic(true);
    bar_body->setVelocity(vec2);
    bar_body->setContactTestBitmask(0x65);

    bar->setPhysicsBody(bar_body);
    bar->setPosition(vec1);
    this->addChild(bar);
}
void GoScene::add_ball(Vec2 vec1,Vec2 vec2) {
    auto ball = Sprite::create("basketball.png");
    ball->setScale(0.375f);
    auto ball_body = PhysicsBody::createCircle(ball->getContentSize().width/2,
                                            PhysicsMaterial(1.5f,0.8f,0.2f));
    ball_body->setDynamic(true);
    ball_body->setVelocity(vec2);
    ball_body->setContactTestBitmask(0x65);

    ball->setPhysicsBody(ball_body);
    ball->setPosition(vec1);
    this->addChild(ball);
}
void GoScene::add_box(Vec2 vec1,Vec2 vec2) {
    auto box = Sprite::create("smile.png");
    box->setScale(0.75f);
    auto box_body = PhysicsBody::createBox(box->getContentSize(),
            PhysicsMaterial(3.0f,0.7f,0.4f));
    box_body->setDynamic(true);
    box_body->setVelocity(vec2);
    box_body->setContactTestBitmask(0x65);

    box->setPhysicsBody(box_body);
    box->setPosition(vec1);
    this->addChild(box);
}
void GoScene::add_wubianxing(Vec2 vec1,Vec2 vec2) {
    auto sjx = Sprite::create("sjx.png");
    sjx->setScale(0.75f);
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
    sjx_body->setContactTestBitmask(0x65);

    sjx->setPhysicsBody(sjx_body);
    sjx->setPosition(vec1);
    this->addChild(sjx);
}

void GoScene::add_edge() {
    Node* node =Node::create();
    auto edges = PhysicsBody::createEdgeBox(vis_size,PHYSICSBODY_MATERIAL_DEFAULT,4);
    node->setPosition(Vec2(vis_size.width/2+origin_size.x,vis_size.height/2+origin_size.y));
    node->setPhysicsBody(edges);
    this->addChild(node);

}

void GoScene::Start_gravity() {
        count=102;
        simpleAudioEngine->playBackgroundMusic("music/welcome_bgm.mp3", false);
        smoke->runAction(RemoveSelf::create(true));
        bottom->runAction(RemoveSelf::create(true));wood2->runAction(RemoveSelf::create(true));wood3->runAction(RemoveSelf::create(true));
        mlabel->setVisible(false);
        arrow1->setVisible(false); arrow2->setVisible(false); arrow3->setVisible(false);
        ParticleSystem* ps0 = ParticleRain::create();
        ps0->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
        ps0->setPosition(Vec2(vis_size.width/2+origin_size.x,
                             vis_size.height-2+origin_size.y));
        this->addChild(ps0,1);
        ParticleSystem* ps = ParticleRain::create();
        ps->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
        ps->setPosition(Vec2(vis_size.width/2+origin_size.x,
                             vis_size.height+origin_size.y));
        this->addChild(ps,1);
        Device::setAccelerometerEnabled(true); //启动重力传感器
}

void GoScene::Go_back(cocos2d::Ref *pSender) {
    Director::getInstance()->popScene();
    Device::setAccelerometerEnabled(false);
}

