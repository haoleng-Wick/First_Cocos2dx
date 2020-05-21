
#include "HelloWorldScene.h"
#include "WelcomeScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        closeItem->setScale(2.0f);
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    auto label = Label::createWithTTF("Oh No ", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setColor(Color3B::RED);
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height/2 - label->getContentSize().height+200));

        this->addChild(label, 1);
    }

    auto sprite = Sprite::create("No.jpg");
    if (sprite == nullptr)
    {
        problemLoading("No.jpg");
    }
    else
    {
        sprite->setScale(1.5f);
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        auto replace = CallFunc::create(std::bind( &HelloWorld::init_music, this));
        sprite->runAction(Sequence::create(DelayTime::create(2),replace,NULL));

        this->addChild(sprite, 0);
    }



    ParticleSystem* snow = ParticleSnow ::create();
    snow->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
    snow->setPosition(origin+Vec2(visibleSize.width/2,visibleSize.height*4));
    snow->setGravity(Vec2(0,-60));
    this->addChild(snow);
    ParticleSystem* snow2 = ParticleSnow ::create();
    snow2->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
    snow2->setPosition(origin+Vec2(visibleSize.width/2,visibleSize.height-10));
    snow2->setGravity(Vec2(0,-60));
    this->addChild(snow2);

    return true;
}

void HelloWorld::init_music() {
    simpleAudioEngine = SimpleAudioEngine::getInstance();
    simpleAudioEngine->playBackgroundMusic("music/mei.mp3",false);
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    simpleAudioEngine->stopBackgroundMusic();
    auto Scene = WelcomeScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1,Scene));
}
