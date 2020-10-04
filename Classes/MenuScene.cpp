//
//  MenuScene.cpp
//  McDinero
//
//  Created by Smile0210 on 19/02/15.
//
//

#include "MenuScene.h"
#include "DineroScene.h"
#include "AudioEngine.h"
#include "GameManager.h"
#include "GoalsScene.h"
#include "McDineroTypes.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdmobHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../proj.ios_mac/ios/IOSHelper.h"
#endif

USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool MenuScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    initLabels();
    
    crownTexture = Director::getInstance()->getTextureCache()->addImage("EvilCrown.png");
    errorTexture = Director::getInstance()->getTextureCache()->addImage("ErrorChar.png");
    pauPauTexture = Director::getInstance()->getTextureCache()->addImage("pau_verde.png");
    
    crownSprite = Sprite::createWithTexture(crownTexture);
    crownSprite->setScale(0.33);
    crownSprite->setPosition(Vec2((visibleSize.width / 3) - 40, (visibleSize.height / 2) - 70));
    this->addChild(crownSprite, 10);
    
    crownEmmiter = ParticleSystemQuad::create("particles/particle_texture_crown.plist");
    crownEmmiter->setScale(0.35);
    crownEmmiter->setPosition(Vec2((visibleSize.width / 3) - 40, (visibleSize.height / 2) - 70));
    this->addChild(crownEmmiter);
    
    errorSprite = Sprite::createWithTexture(errorTexture);
    errorSprite->setScale(0.65);
    errorSprite->setPosition(Vec2((visibleSize.width / 2), (visibleSize.height / 2) - 70));
    
    errorSprite->runAction(RepeatForever::create(Sequence::create(
                                                                  RotateBy::create(0.05, -6.0),
                                                                  RotateBy::create(0.05, 0.0),
                                                                  RotateBy::create(0.05, 6.0),
                                                                  RotateBy::create(0.05, 0.0), NULL)));
    errorSprite->runAction(RepeatForever::create(Blink::create(1, 8)));
    
    this->addChild(errorSprite);
    
    pauPauSprite = Sprite::createWithTexture(pauPauTexture);
    pauPauSprite->setScale(0.45);
    pauPauSprite->setPosition(Vec2(((visibleSize.width / 3) * 2) + 40, (visibleSize.height / 2) - 70));
    
    this->addChild(pauPauSprite, 10);
    
    ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/particle_texture_paupau.plist");
    particle->setScale(0.45);
    particle->setPosition(Vec2(((visibleSize.width / 3) * 2) + 40, (visibleSize.height / 2) - 70));
    this->addChild(particle);
    
    if (GameManager::getInstance()->soundActive)
    {
        startBackgroundMusic();
    }
    
    return true;
}

void MenuScene::startBackgroundMusic()
{
    int idBackGroundMusic = AudioEngine::play2d(kBackgroundMusic);
    GameManager::getInstance()->idBackGroundMusic = idBackGroundMusic;
    AudioEngine::setLoop(GameManager::getInstance()->idBackGroundMusic, true);
}

void MenuScene::initLabels()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    titleLabel = Label::createWithTTF("¡EL DINERO ES", "fonts/JoystixMonospace.ttf", 25);
    
    float deltaY = titleLabel->getContentSize().height;
    
    titleLabel2 = Label::createWithTTF("DINERO DINERO!", "fonts/JoystixMonospace.ttf", 25);
    
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height - deltaY));
    
    deltaY += titleLabel2->getContentSize().height;
    
    titleLabel2->setPosition(Vec2(origin.x + visibleSize.width/2,
                                  origin.y + visibleSize.height - deltaY));
    
    auto moveSequence = Sequence::create(
                                         MoveBy::create(0.5, Vec2(0, -10)),
                                         DelayTime::create(0.5),
                                         MoveBy::create(0.5, Vec2(0, 10)),
                                         DelayTime::create(0.5),
                                         NULL);
    
    auto moveSequence2 = Sequence::create(
                                          MoveBy::create(0.5, Vec2(0, -10)),
                                          DelayTime::create(0.5),
                                          MoveBy::create(0.5, Vec2(0, 10)),
                                          DelayTime::create(0.5),
                                          NULL);
    
    titleLabel->runAction(RepeatForever::create(moveSequence));
    titleLabel2->runAction(RepeatForever::create(moveSequence2));
    
    this->addChild(titleLabel, 1);
    this->addChild(titleLabel2, 1);
    
    UserDefault *userDefault = UserDefault::getInstance();
    auto prevHighScore = userDefault->getIntegerForKey("HighScore");
    
    if (prevHighScore > 0)
    {
        std::string str = cocos2d::StringUtils::format("High Score: %i", prevHighScore);
        highScoreLabel = Label::createWithTTF(str, "fonts/JoystixMonospace.ttf", 18);
    }
    else
    {
        highScoreLabel = Label::createWithTTF("High Score: -", "fonts/JoystixMonospace.ttf", 18);
    }
    
    deltaY += highScoreLabel->getContentSize().height + 40;
    
    highScoreLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                     origin.y + visibleSize.height - deltaY));
    
    // Obtener el Score
    this->addChild(highScoreLabel);
    
    bool hasSoundPreferences = userDefault->getBoolForKey("SoundPreferences");
    
    if (hasSoundPreferences)
    {
        GameManager::getInstance()->soundActive = userDefault->getBoolForKey("SoundActive");
    }
    else
    {
        GameManager::getInstance()->soundActive = true;
        userDefault->setBoolForKey("SoundPreferences", true);
        userDefault->setBoolForKey("SoundActive", true);
        userDefault->flush();
    }
    
    if (GameManager::getInstance()->soundActive)
    {
        soundLabel = Label::createWithTTF("Sound: on", "fonts/JoystixMonospace.ttf", 17);
    }
    else
    {
        soundLabel = Label::createWithTTF("Sound: off", "fonts/JoystixMonospace.ttf", 17);
    }
    
    auto soundButton = MenuItemLabel::create(soundLabel, CC_CALLBACK_0(MenuScene::toggleSound, this));
    
    
    achievementsLabel = Label::createWithTTF("Trofeos", "fonts/JoystixMonospace.ttf", 17);
    
    auto achievementsButton = MenuItemLabel::create(achievementsLabel, CC_CALLBACK_0(MenuScene::goToGoalsScene, this));
    
    auto menuButton = Menu::create(soundButton, achievementsButton, NULL);
    menuButton->alignItemsHorizontallyWithPadding(50);
    menuButton->setAnchorPoint(Vec2(0.5, 0));
    menuButton->setPosition(Vec2(origin.x + visibleSize.width / 2, 90));
    this->addChild(menuButton);
    
    instructionsLabel1 = Label::createWithTTF("Consigue todo el dinero que", "fonts/JoystixMonospace.ttf", 12);
    
    deltaY += instructionsLabel1->getContentSize().height + 30;
    
    instructionsLabel1->setPosition(Vec2(origin.x + visibleSize.width/2,
                                         origin.y + visibleSize.height - deltaY));
    
    instructionsLabel2 = Label::createWithTTF("puedas, toca la pantalla para", "fonts/JoystixMonospace.ttf", 12);
    
    deltaY += instructionsLabel2->getContentSize().height + 10;
    
    instructionsLabel2->setPosition(Vec2(origin.x + visibleSize.width/2,
                                         origin.y + visibleSize.height - deltaY));
    
    instructionsLabel3 = Label::createWithTTF("cambiar de dirección, esquiva", "fonts/JoystixMonospace.ttf", 12);
    
    deltaY += instructionsLabel3->getContentSize().height + 10;
    
    instructionsLabel3->setPosition(Vec2(origin.x + visibleSize.width/2,
                                         origin.y + visibleSize.height - deltaY));
    
    instructionsLabel4 = Label::createWithTTF("los errores y las coronas", "fonts/JoystixMonospace.ttf", 12);
    
    deltaY += instructionsLabel4->getContentSize().height + 10;
    
    instructionsLabel4->setPosition(Vec2(origin.x + visibleSize.width/2,
                                         origin.y + visibleSize.height - deltaY));
    
    instructionsLabel5 = Label::createWithTTF("descubre las bebidas", "fonts/JoystixMonospace.ttf", 12);
    
    deltaY += instructionsLabel5->getContentSize().height + 10;
    
    instructionsLabel5->setPosition(Vec2(origin.x + visibleSize.width/2,
                                         origin.y + visibleSize.height - deltaY));
    
    
    
    this->addChild(instructionsLabel1);
    this->addChild(instructionsLabel2);
    this->addChild(instructionsLabel3);
    this->addChild(instructionsLabel4);
    this->addChild(instructionsLabel5);
    
    
    auto startLabel = Label::createWithTTF("RAP RAP RAP", "fonts/JoystixMonospace.ttf", 28);
    
    auto blinkSequence = Sequence::create(
                                          DelayTime::create(1.0),
                                          FadeTo::create(0.25, 0),
                                          FadeTo::create(0.25, 255),
                                          //FadeIn::create(0),
                                          DelayTime::create(0.5),
                                          NULL);
    
    startLabel->runAction(RepeatForever::create(blinkSequence));
    
    auto startBackground = LayerColor::create(Color4B(255,0,0,255), visibleSize.width, startLabel->getContentSize().height * 1.9);
    this->addChild(startBackground);
    
    auto starButton = MenuItemLabel::create(startLabel, CC_CALLBACK_1(MenuScene::goToMainScene, this));
    
    auto menu = Menu::create(starButton, NULL);
    menu->alignItemsHorizontally();
    menu->setAnchorPoint(Vec2(0.5, 0));
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + startLabel->getContentSize().height));
    this->addChild(menu);
    
}

void MenuScene::goToMainScene(Ref *sender)
{
    if (GameManager::getInstance()->soundActive)
    {
        AudioEngine::play2d(kPerfectoSound);
    }
    auto scene = DineroScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void MenuScene::toggleSound()
{
    UserDefault *userDefault = UserDefault::getInstance();
    
    if (GameManager::getInstance()->soundActive)
    {
        userDefault->setBoolForKey("SoundActive", false);
        GameManager::getInstance()->soundActive = false;
        soundLabel->setString("Sound: off");
        AudioEngine::stop(GameManager::getInstance()->idBackGroundMusic);
    }
    else
    {
        userDefault->setBoolForKey("SoundActive", true);
        GameManager::getInstance()->soundActive = true;
        soundLabel->setString("Sound: on");
        startBackgroundMusic();
    }
    
    userDefault->flush();
}

void MenuScene::goToGoalsScene()
{
    auto scene = GoalsScene::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(1, scene));
}












