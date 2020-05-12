//
// Created by haoleng on 20-3-25.
//

#include <cocos/scripting/deprecated/CCDeprecated.h>
#include "GameScene.h"


USING_NS_CC;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}
bool GameScene::init() {
    this->scheduleUpdate();
    srand(static_cast<unsigned int>(time(NULL)));  //随机种子
    score =0;
    world =new b2World(b2Vec2(0,-2.0f));
//    world = Box2DHelper::createWorld();
    world->SetAllowSleeping(true);          //允许物体休眠
    world->SetContinuousPhysics(true);      //开启连续碰撞
    world->SetContactListener(this);        //碰撞侦听
    initBackground();
    addPlayer();
    addGround();
    for(int i =0;i<4;i++){
        addBar(i);
    }
//触摸事件
    auto Touch_Listener = EventListenerTouchOneByOne::create();
    Touch_Listener->onTouchBegan = [this](Touch *touch,Event* event){
        isTouch=true;
        if(isContacted)  {      //在板上的话就可以跑动了
        auto run = SpriteFrameCache::getInstance()->getSpriteFrameByName("stand.png");
        player->setSpriteFrame(run);
        player->stopAllActions();
        Vector<SpriteFrame *> runs(9);
        char run_str[160] = {};
        for (int i = 1; i <= 9; i++) {
                sprintf(run_str, "Run%d.png", i);
                auto hero_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(run_str);
                runs.pushBack(hero_frame);
            }
        auto run_r_animation = Animation::createWithSpriteFrames(runs, 0.1f);
        auto run_r_animate = Animate::create(run_r_animation);
        player->runAction(run_r_animate);
    }
        if(touch->getLocation().x<vis_size.width/2)
            left=true;
        else
            right=true;
        for(b2Body* b =world->GetBodyList();b;b=b->GetNext()){
            if(b->GetUserData()==player){
                if(left){
                    player->setFlippedX(true);
                    b->ApplyLinearImpulse(b2Vec2(-0.1f,0),b->GetWorldCenter(), true);   //冲量使其运动
                }
                else if(right){
                    player->setFlippedX(false);
                    b->ApplyLinearImpulse(b2Vec2(0.1f,0),b->GetWorldCenter(), true);
                }
            }
        }
        return true;
    };

    Touch_Listener->onTouchEnded =[this](Touch *touch,Event* event){
        left= false;right=false;isTouch= false;            //停止触屏即让标志复原
        for(b2Body* b =world->GetBodyList();b;b=b->GetNext()){
            if(b->GetUserData()==player){
                    b->SetLinearVelocity(b2Vec2(0,b->GetLinearVelocity().y));//水平速度值为0
            }
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(Touch_Listener,this);

    return true ;
}

void GameScene::initBackground() {
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/cano.mp3", true);
    label = Label::createWithTTF("0","fonts/Marker Felt.ttf",20);
    label->setColor(Color3B::RED);
    label->setPosition(vis_size.width+ori_size.x-40,vis_size.height+ori_size.y-20);
//    label->retain();//增加文字标签的索引计数
    this->addChild(label,2);
    bg1 = Sprite::create("background.jpg");
    bg1->setPosition(ori_size.x+vis_size.width/2,ori_size.y+vis_size.height/2);
    auto moveby = Sequence::create(MoveBy::create(5,Vec2(100,0)),MoveBy::create(5,Vec2(-100,0)),NULL);
    bg1->runAction(RepeatForever::create(moveby));
    this->addChild(bg1,0);
}

void GameScene::addGround() {
    Box2DHelper::createEdge(world,vis_size.width,vis_size.height, nullptr);

    auto bar = Sprite::create("bar.png");
    bar->setScaleX(1.25f);
    Box2DHelper::createBar(world,vis_size.width/2+ori_size.x,
                           ori_size.y+15+4*(vis_size.height/5),
                           bar->getContentSize().width*1.25f,bar->getContentSize().height,
                           false,bar);
    this->addChild(bar,2);

}
void GameScene::addPlayer() {
    player = Hero::create();
    player->setScale(0.3f);
    this->addChild(player,2,11);
    Box2DHelper::createHero(world,vis_size.width/2,
                            15+4*(vis_size.height/5)+30,
                            26,60,false,player);
}

void GameScene::addBar(int i) {
        auto bar = Sprite::create("bar.png");
        bar->setScaleX(1.25f);
        int r = random(10,225);
        int g = random(1,214);
        int b = random(1,225);
            bar->setColor(Color3B(r,g,b));
    if(i!=2){
        Box2DHelper::createBar(world,random(bar->getContentSize().width/2*1.25f,vis_size.width-bar->getContentSize().width/2*1.25f),
                15+i*(vis_size.height/5),
                bar->getContentSize().width*1.25f,bar->getContentSize().height,
                               false,bar);
    }else{Box2DHelper::createBar(world,vis_size.width/2,
                                 15+i*(vis_size.height/5),
                                 bar->getContentSize().width*1.25f,bar->getContentSize().height,
                                 false,bar);}
        this->addChild(bar,2);
}

void GameScene::addDoor() {
    auto door = Sprite::create("door.png");
    door->setScale(1.5f);
    door->setPosition(400+ori_size.x,50+ori_size.y);
    this->addChild(door,1);
    auto fu_=Sequence::create(MoveBy::create(1,Vec2(0,10)),MoveBy::create(1,Vec2(0,-10)),NULL);
    door->runAction(RepeatForever::create(fu_));
    ParticleSystem* fire_l = ParticleFireworks ::create();
    fire_l->setTexture(Director::getInstance()->getTextureCache()->addImage("star.png"));
    fire_l->setPosition(door->getPosition()+Vec2(-50,-40));
    ParticleSystem* fire_r = ParticleFireworks ::create();
//    fire_r->setScale(0.5f);
    fire_r->setTexture(Director::getInstance()->getTextureCache()->addImage("star.png"));
    fire_r->setPosition(door->getPosition()+Vec2(50,-40));
    this->addChild(fire_l,0);
    this->addChild(fire_r,0);
}

void GameScene::add_Dead_field() {
    ground = Sprite::create("ground.png");
    ground->setPosition(vis_size.width/2,2);
    this->addChild(ground,2);
    Box2DHelper::createDeadborder(world,vis_size.width/2,0,vis_size.width,20, true, ground);
}

void GameScene::BeginContact(b2Contact *contact) {
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    auto spriteA = (Sprite*)bodyA->GetUserData();
    auto spriteB = (Sprite*)bodyB->GetUserData();
    if(spriteA!= nullptr&&spriteB!= nullptr) {
        auto barsprite =new Sprite;
        if (spriteA == player || spriteB == player) {
                isContacted=true;
            if(spriteA==player) barsprite=spriteB;
            else barsprite =spriteA;
            while (barsprite->getColor()!=Color3B(225,215,0) && barsprite!=ground){
                SimpleAudioEngine::getInstance()->playEffect("music/down.ogg",false);
                    barsprite->setColor(Color3B(225,215,0));
                score++;
                char str[128] = {'0'};
                sprintf(str,"Score:%d",score);
                label->setString(str);
                label->setVisible(true);
            }
        }
    }
}

void GameScene::EndContact(b2Contact *contact) {
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    auto spriteA = (Sprite*)bodyA->GetUserData();
    auto spriteB = (Sprite*)bodyB->GetUserData();
    if(spriteA!= nullptr&&spriteB!= nullptr) {
        if (spriteA == player || spriteB == player) {
            isContacted= false;
        }
    }
}

void GameScene::logic() {
    if(!isContacted){
        auto jump = SpriteFrameCache::getInstance()->getSpriteFrameByName("fall.png");
        player->stopAllActions();
        player->setSpriteFrame(jump);
    }else if(!isTouch && isContacted){
            auto stand = SpriteFrameCache::getInstance()->getSpriteFrameByName("stand.png");
            player->stopAllActions();
            player->setSpriteFrame(stand);
    }
}

void GameScene::GameWin() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = WelcomeScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(2,scene));
}
void GameScene::GameLose() {
    SimpleAudioEngine::getInstance()->playEffect("music/fail.mp3");
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = WelcomeScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(2.3f,scene));
}

void GameScene::update(float delta) {
    logic();
    world->Step(0.03f,8,3);
    Sprite * s;
    for(b2Body * b = world->GetBodyList();b;b=b->GetNext()){
        if(b->GetUserData()) {
            s = (Sprite *) b->GetUserData();
            s->setPosition(b->GetPosition().x * RATIO, b->GetPosition().y * RATIO);
            s->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            if(s==player && player_win){
                world->DestroyBody(b);
                player->setVisible(false);
                player_win = false;
            }
        }
        if(b->GetPosition().y>(vis_size.height)/RATIO){
            s = (Sprite*)b->GetUserData();
            if(s!=NULL){
                s->removeFromParent();
            }
            world->DestroyBody(b);
            if(score<=56)
                addBar(0);
                else if(next_add){
                    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/welcome_bgm.mp3", false);
                    add_Dead_field();
                    addDoor();
                    gameover= true;
                next_add = false; //    由于是在update函数中，为防止重复叠加，需要设立标志
                }
            }
        }
        while ( gameover && player->getPositionX()>400+ori_size.x
                    && player->getPositionX()<450+ori_size.x
                    && player->getPositionY()<=50){         //主角到达传送门的判断

            player_win = true;
            auto player2=Sprite::create("000.png");
            player2->setPosition(400+ori_size.x,45);
            player2->setScale(0.3f);
            auto next = Spawn::create(RotateBy::create(3,360*3),ScaleBy::create(3, 0, 0),
                                      FadeOut::create(3),NULL);
            auto replace = CallFunc::create(std::bind( &GameScene::GameWin, this));
            player2->runAction(Sequence::create(next,replace,NULL));

            this->addChild(player2,4);
            gameover = false;
        }
        while (!gamelose && player->getPositionY()<-5+ori_size.y){//游戏结束
            GameLose();
            gamelose = true;
        }
    }
