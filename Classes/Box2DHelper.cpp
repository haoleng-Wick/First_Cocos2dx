//
// Created by haoleng on 20-3-26.
//

#include "Box2DHelper.h"

#include "Box2D/Box2D.h"
#include "WelcomeScene.h"
b2World *Box2DHelper::createWorld(){
    auto world = new b2World(b2Vec2(0,-9.8f));
    return world;
}
b2Body* Box2DHelper::createBox(b2World* world, float posX,
            float posY,float width, float height,
            bool isStatic,void* userData){
    b2BodyDef bodyDef;              //声明
    bodyDef.type=isStatic?b2BodyType ::b2_staticBody : b2BodyType ::b2_dynamicBody;
    bodyDef.position.Set((posX+or_sizex)/RATIO,(posY+or_sizey)/RATIO);
    bodyDef.fixedRotation= false;
    bodyDef.userData=userData;  //绑定精灵

    b2PolygonShape shape;           //形状
    shape.SetAsBox(width/RATIO/2,height/RATIO/2);

    b2FixtureDef fixtureDef;        //物理属性
    fixtureDef.density=1;
    fixtureDef.restitution=0.4f;
    fixtureDef.friction=0.5f;
    fixtureDef.filter.groupIndex = WORLD_BORDER;
    fixtureDef.filter.categoryBits = WORLD_BORDER;
    fixtureDef.filter.maskBits = BALL | BORDER;
    fixtureDef.shape=&shape;

    auto box =world->CreateBody(&bodyDef);  //绑定刚体
    box->CreateFixture(&fixtureDef);

    return box;
}

b2Body* Box2DHelper::createHero(b2World* world, float posX,
                               float posY,float width, float height,
                               bool isStatic,void* userData){
    b2BodyDef bodyDef;              //声明
    bodyDef.type=isStatic?b2BodyType ::b2_staticBody : b2BodyType ::b2_dynamicBody;
    bodyDef.position.Set((posX+or_sizex)/RATIO,(posY+or_sizey)/RATIO);
    bodyDef.fixedRotation= true;
    bodyDef.userData=userData;  //绑定精灵

    b2PolygonShape shape;           //形状
    shape.SetAsBox(width/RATIO/2,height/RATIO/2);

    b2FixtureDef fixtureDef;        //物理属性
    fixtureDef.density=0.5f;
    fixtureDef.restitution=0.22f;
    fixtureDef.friction=0.1f;
    fixtureDef.filter.groupIndex = HERO;
    fixtureDef.filter.categoryBits = HERO;
    fixtureDef.filter.maskBits = BORDER | WORLD_BORDER |DEAD_FIELD;
    fixtureDef.shape=&shape;

    auto box =world->CreateBody(&bodyDef);  //绑定刚体
    box->CreateFixture(&fixtureDef);

    return box;
}

b2Body* Box2DHelper::createBar(b2World* world, float posX,
                               float posY,float width, float height,
                               bool isStatic,void* userData){

    b2BodyDef bodyDef;              //声明
    bodyDef.type=isStatic?b2BodyType ::b2_staticBody : b2BodyType ::b2_dynamicBody;
    bodyDef.position.Set((posX+or_sizex)/RATIO,(posY+or_sizey)/RATIO);
    bodyDef.fixedRotation= true;
    bodyDef.gravityScale = false;
    bodyDef.userData=userData;  //绑定精灵

    b2PolygonShape shape;           //形状
    shape.SetAsBox(width/RATIO/2,height/RATIO/2);

    b2FixtureDef fixtureDef;        //物理属性
    fixtureDef.density=10000;
    fixtureDef.restitution=0.2f;
    fixtureDef.friction=0.1f;
    fixtureDef.filter.groupIndex = BORDER;
    fixtureDef.filter.categoryBits = BORDER;
    fixtureDef.filter.maskBits = HERO;
    fixtureDef.shape=&shape;

    auto box =world->CreateBody(&bodyDef);  //绑定刚体
    box->CreateFixture(&fixtureDef);
    b2Vec2 vec2 = b2Vec2(0,1.3f);
    box->SetLinearVelocity(vec2);

    return box;
}
b2Body* Box2DHelper::createDeadborder(b2World* world, float posX,
                               float posY,float width, float height,
                               bool isStatic,void* userData){

    b2BodyDef bodyDef;              //声明
    bodyDef.type=isStatic?b2BodyType ::b2_staticBody : b2BodyType ::b2_dynamicBody;
    bodyDef.position.Set((posX+or_sizex)/RATIO,(posY+or_sizey)/RATIO);
    bodyDef.fixedRotation= false;
    bodyDef.gravityScale= false;
    bodyDef.userData=userData;  //绑定精灵

    b2PolygonShape shape;           //形状
    shape.SetAsBox(width/RATIO/2,height/RATIO/2);

    b2FixtureDef fixtureDef;        //物理属性
    fixtureDef.density=10000;
    fixtureDef.restitution=0.4f;
    fixtureDef.friction=0.4f;
    fixtureDef.filter.groupIndex = DEAD_FIELD;
    fixtureDef.filter.categoryBits = DEAD_FIELD;
    fixtureDef.filter.maskBits = HERO;
    fixtureDef.shape=&shape;

    auto box =world->CreateBody(&bodyDef);  //绑定刚体
    box->CreateFixture(&fixtureDef);

    return box;
}


b2Body* Box2DHelper::createEdge(b2World* world, float width,
                float height,void* userData){
    float wall_thick =20;    //围墙厚度
//    createBox(world,width/2,0,width,wall_thick,true,userData);//down
    createBox(world,width/2,height,width,wall_thick,true,userData);//top
    createBox(world,0,height/2,wall_thick,height,true,userData);//left
    createBox(world,width,height/2,wall_thick,height,true,userData);//right
    return nullptr;
}

