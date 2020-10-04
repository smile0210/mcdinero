//
//  BonusScene.cpp
//  McDinero
//
//  Created by Smile0210 on 05/03/15.
//
//

#include "BonusScene.h"
#include "GameManager.h"
#include "CustomDirector.h"

Scene* BonusScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BonusScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool BonusScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    auto backLabel = Label::createWithTTF("BACK", "fonts/JoystixMonospace.ttf", 25);

    auto testButton = MenuItemLabel::create(backLabel, CC_CALLBACK_1(BonusScene::goToMainScene, this));
    
    auto menuButton = Menu::create(testButton, NULL);
    menuButton->alignItemsHorizontally();
    menuButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(menuButton);

    return true;
}

void BonusScene::goToMainScene(cocos2d::Ref *sender)
{
    CustomDirector *customDirector = (CustomDirector *)Director::getInstance();
    customDirector->popSceneWithTransition<TransitionFade>(1.0);
}









