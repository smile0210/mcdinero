//
//  RapScene.cpp
//  McDinero
//
//  Created by Smile0210 on 23/02/15.
//
//

#include "RapScene.h"
#include "CustomDirector.h"
#include "DineroScene.h"
#include "GameManager.h"
#include "AudioEngine.h"
#include "BonusScene.h"
#include "McDineroTypes.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdmobHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../proj.ios_mac/ios/IOSHelper.h"
#endif

Scene* RapScene::createScene(CharacterEquipment equipment)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = RapScene::create();
    layer->setCharacterEquipment(equipment);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool RapScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    if (GameManager::getInstance()->soundActive)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kDineroSound);
    }
    
    GameManager::getInstance()->upgradeAfterRap = false;
    GameManager::getInstance()->bonusAfterRap = false;
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    initVariables();
    loadTextures();
    createProgressBar();
    createRapLabels();
    createAprendeLabels();
    
    backgroundImage = Sprite::createWithTexture(textureBackground);
    backgroundImage->setScale(1.54);
    backgroundImage->setAnchorPoint(Vec2(0.5, 0.0));
    backgroundImage->setPosition(Vec2(visibleSize.width / 2, 101));
    this->addChild(backgroundImage);
    
    timeToCompleteLabel = Label::createWithTTF("", "fonts/JoystixMonospace.ttf", 40);
    timeToCompleteLabel->setPosition(Vec2(visibleSize.width - 35, 130));
    this->addChild(timeToCompleteLabel);
    
    auto buttonBackground = LayerColor::create(Color4B(255, 0, 0, 255), visibleSize.width, 101);
    this->addChild(buttonBackground);
    
    rapLabel = Label::createWithTTF("RAP!", "fonts/JoystixMonospace.ttf", 60);
    auto rapButton = MenuItemLabel::create(rapLabel, CC_CALLBACK_0(RapScene::rap, this));
    
    auto menu = Menu::create(rapButton, NULL);
    menu->alignItemsHorizontally();
    menu->setAnchorPoint(Vec2(0.5, 0));
    menu->setPosition(Vec2(visibleSize.width / 2, 50));
    this->addChild(menu);
    
    character = Sprite::createWithSpriteFrameName("RA01.png");
    character->setAnchorPoint(Vec2(0.5, 0.0));
    character->setPosition(Vec2(visibleSize.width/2, 101));
    this->addChild(character);
    
    auto discountFuncPointer = static_cast<cocos2d::SEL_SCHEDULE>(&RapScene::discountFunc);
    auto countDownFuncPointer = static_cast<cocos2d::SEL_SCHEDULE>(&RapScene::countDown);
    this->schedule(discountFuncPointer, 0.5);
    this->schedule(countDownFuncPointer, 1.0);
    
    this->scheduleUpdate();

    return true;
}

void RapScene::onEnter()
{
    Layer::onEnter();
    
    switch (currentEquipment)
    {
        case None:
            playerGoalScore = 130;
            playerDiscountPoints = 5;
            break;
        case Glasses:
            playerGoalScore = 150;
            playerDiscountPoints = 6;
            break;
        case Cloak:
            playerGoalScore = 180;
            playerDiscountPoints = 8;
            break;
        case Crown:
            playerGoalScore = 200;
            playerDiscountPoints = 10   ;
            break;
        default:
            playerGoalScore = 130;
            playerDiscountPoints = 5;
            break;
    }
    
}

void RapScene::update(float dt)
{
    if (!backToDineroScene)
    {
        updateProgressBar();
    }
}

void RapScene::initVariables()
{
    playerScore = 0;
    playerGoalScore = 100;
    playerDiscountPoints = 5;
    
    canPlayRapSound = true;
    
    timeToComplete = 6;
    isCharacterAnimated = false;
}

void RapScene::loadTextures()
{
//    textureProgressBar = Director::getInstance()->getTextureCache()->addImage("progressVerical.png");
//    textureProgressBarEmpty = Director::getInstance()->getTextureCache()->addImage("progressEmptyVertical.png");
    textureProgressBar = Director::getInstance()->getTextureCache()->addImage("progressVerical.png");
    textureProgressBarEmpty = Director::getInstance()->getTextureCache()->addImage("progressEmptyVertical.png");
    textureBackground = Director::getInstance()->getTextureCache()->addImage("PNGBG02.png");
    textureGlasses = Director::getInstance()->getTextureCache()->addImage("GlassesRap.png");
    textureCloak = Director::getInstance()->getTextureCache()->addImage("CloakRap.png");
    textureCrown = Director::getInstance()->getTextureCache()->addImage("CrownRap.png");
    textureBlingbling = Director::getInstance()->getTextureCache()->addImage("bling.png");
    
    Vector<SpriteFrame*> animFrames(15);
    char str[100] = {0};
    for(int i = 1; i < 15; i++)
    {
        sprintf(str, "RA%02d.png",i);
        auto frame = SpriteFrame::create(str,Rect(0, 0, 390 * 0.5, 350 * 0.5));
        SpriteFrameCache::getInstance()->addSpriteFrame(frame, str);
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
    animate = RepeatForever::create(Animate::create(animation));
    animate->setTag(RapActionType);
    animate->retain();
    
}

void RapScene::updateProgressBar()
{
    float progress = (float)playerScore / (float)playerGoalScore;
    if (progress >= 1)
    {
        backToDineroScene = true;
    }
    
    barProgressTimer->setPercentage(progress * 100);
}

void RapScene::createProgressBar()
{
    progressBarEmpty = Sprite::createWithTexture(textureProgressBarEmpty);
    progressBarEmpty->setScale(0.5);
    progressBarEmpty->setAnchorPoint(Vec2(0.0, 0.0));
    progressBarEmpty->setPosition(Vec2(10, 110));
    this->addChild(progressBarEmpty, 9);
    
    progressBar = Sprite::createWithTexture(textureProgressBar);
    
    barProgressTimer = ProgressTimer::create(progressBar);
    barProgressTimer->setScale(0.5);
    barProgressTimer->setAnchorPoint(Vec2(0.0, 0.0));
    barProgressTimer->setType(ProgressTimer::Type::BAR);
    barProgressTimer->setMidpoint(Vec2(0, 0));
    barProgressTimer->setBarChangeRate(Vec2(0, 1));
    barProgressTimer->setPosition(Vec2(10, 110));
    this->addChild(barProgressTimer, 10);
    
    barProgressTimer->setPercentage(40);
}

void RapScene::rap()
{
    if (backToDineroScene)
    {
        if (!sceneComplete)
        {
            sceneComplete = true;
            goToDineroScene(sceneComplete);
        }
    }
    else
    {
        createCheck();
        playerScore += 10;
        
        animateRapLabels();
        
        if (GameManager::getInstance()->soundActive)
        {
            if (!rapSoundId)
            {
                rapSoundId = AudioEngine::play2d(kDineroSound);
            }
            if (AudioEngine::getState(rapSoundId) == AudioEngine::AudioState::PAUSED)
            {
                AudioEngine::play2d(kDineroSound);
            }
        }
        
        if (!isCharacterAnimated)
        {
            if (character->getActionByTag(RapActionType))
            {
                character->resume();
            }
            else
            {
                character->runAction(animate);
            }
            isCharacterAnimated = true;
        }
    }
    
}

void RapScene::createCheck()
{
    this->stopActionByTag(CheckActionType);
    
    auto checkAction = Sequence::create(
                                        DelayTime::create(1.0),
                                        CallFunc::create(CC_CALLBACK_0(RapScene::checkFucn, this)),
                                        NULL);
    checkAction->setTag(CheckActionType);
    
    this->runAction(checkAction);
    
}

void RapScene::checkFucn()
{
    character->pause();
    if (GameManager::getInstance()->soundActive)
    {
        AudioEngine::pause(rapSoundId);
    }
    isCharacterAnimated = false;
}

void RapScene::discountFunc(float dt)
{
    if (playerScore > playerDiscountPoints && !backToDineroScene)
    {
        playerScore -= playerDiscountPoints;
    }
}

void RapScene::goToDineroScene(bool complete)
{
    if (complete)
    {
        animationBeforeChangeScene();
    }
    else
    {
        goToSceneAfterAnimation();
    }
}

void RapScene::countDown(float dt)
{
    if (!backToDineroScene)
    {
        std::string str = cocos2d::StringUtils::format("%i", timeToComplete);
        timeToCompleteLabel->setString(str);
        
        if (timeToComplete == 0)
        {
            goToDineroScene(false);
        }
        
        timeToComplete--;
    }
}

void RapScene::createRapLabels()
{
    dineroLabel1 = Label::createWithTTF("Dinero!", "fonts/JoystixMonospace.ttf", 25);
    dineroLabel1->setPosition(Vec2((visibleSize.width / 2) - 80, visibleSize.height - 50));
    dineroLabel1->setOpacity(0);
    this->addChild(dineroLabel1, 10);
    
    dineroLabel2 = Label::createWithTTF("Dinero!", "fonts/JoystixMonospace.ttf", 25);
    dineroLabel2->setPosition(Vec2((visibleSize.width / 2) + 80, visibleSize.height - 50));
    dineroLabel2->setOpacity(0);
    this->addChild(dineroLabel2, 10);
    
    dineroLabel3 = Label::createWithTTF("Dinero!", "fonts/JoystixMonospace.ttf", 25);
    dineroLabel3->setPosition(Vec2((visibleSize.width / 2) - 80, visibleSize.height - 100));
    dineroLabel3->setOpacity(0);
    this->addChild(dineroLabel3, 10);
    
    dineroLabel4 = Label::createWithTTF("Dinero!", "fonts/JoystixMonospace.ttf", 25);
    dineroLabel4->setPosition(Vec2((visibleSize.width / 2) + 80, visibleSize.height - 100));
    dineroLabel4->setOpacity(0);
    this->addChild(dineroLabel4, 10);
    
    
}

void RapScene::animateRapLabels()
{
    auto blinkSequence = FadeOut::create(0.2);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    int labelToAnimate = rand() % 4;
#else
    int labelToAnimate = arc4random() % 4;
#endif
    
    switch (labelToAnimate)
    {
        case 0:
            dineroLabel1->setOpacity(255);
            dineroLabel1->runAction(blinkSequence);
            break;
        case 1:
            dineroLabel2->setOpacity(255);
            dineroLabel2->runAction(blinkSequence);
            break;
        case 2:
            dineroLabel3->setOpacity(255);
            dineroLabel3->runAction(blinkSequence);
            break;
        case 3:
            dineroLabel4->setOpacity(255);
            dineroLabel4->runAction(blinkSequence);
            break;
        default:
            dineroLabel1->setOpacity(255);
            dineroLabel1->runAction(blinkSequence);
            break;
    }
    
}


void RapScene::animationBeforeChangeScene()
{
    Sprite *equipmentNode;
    float duration;
    const char *effect = "";
    
    switch (currentEquipment)
    {
        case None:
            if (GameManager::getInstance()->soundActive)
            {
//                CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kAprendeSound);
                effect = kAprendeSound;
            }
            equipmentNode = Sprite::createWithTexture(textureGlasses);
            duration = 1.0;
            aprendeLabel1->setString("Aprende algo");
            aprendeLabel2->setString("¡DINERO!");
            GameManager::getInstance()->upgradeAfterRap = true;
            break;
            
        case Glasses:
            if (GameManager::getInstance()->soundActive)
            {
//                CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kSoyEspanaSound);
                effect = kSoyEspanaSound;
            }
            equipmentNode = Sprite::createWithTexture(textureCloak);
            duration = 1.0;
            aprendeLabel1->setString("Yo soy");
            aprendeLabel2->setString("¡ESPAÑA!");
            GameManager::getInstance()->upgradeAfterRap = true;
            break;
            
        case Cloak:
            if (GameManager::getInstance()->soundActive)
            {
//                CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kMonarquiaSound);
                effect = kMonarquiaSound;
            }
            equipmentNode = Sprite::createWithTexture(textureCrown);
            duration = 1.5;
            aprendeLabel1->setString("Monarquia");
            aprendeLabel2->setString("¡ESPAÑOLA!");
            GameManager::getInstance()->upgradeAfterRap = true;
            break;
            
        case Crown:
            if (GameManager::getInstance()->soundActive)
            {
//                CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kAtaquesDinderoSound);
                effect = kAtaquesDinderoSound;
            }
            equipmentNode = Sprite::createWithTexture(textureBlingbling);
            duration = 1.5;
            aprendeLabel1->setString("Ataques de");
            aprendeLabel2->setString("¡DINERO!");
            GameManager::getInstance()->bonusAfterRap = true;
            break;
            
        default:
            if (GameManager::getInstance()->soundActive)
            {
//                CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kAprendeSound);
                effect = kAprendeSound;
            }
            equipmentNode = Sprite::createWithTexture(textureGlasses);
            duration = 1.0;
            aprendeLabel1->setString("Aprende algo");
            aprendeLabel2->setString("¡DINERO!");
            break;
    }
    
    //equipmentNode->setScale(1.5);
    equipmentNode->setAnchorPoint(Vec2(0.5, 0.0));
    equipmentNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
    this->addChild(equipmentNode);
    
    if (GameManager::getInstance()->soundActive)
    {
        AudioEngine::stop(rapSoundId);
        AudioEngine::play2d(effect);
    }
    
    animateAprendeLabels();
    
    equipmentNode->runAction(Sequence::create(
                                              MoveTo::create(duration, Vec2(visibleSize.width / 2, 101)),
                                              DelayTime::create(1.5),
                                              CallFunc::create(CC_CALLBACK_0(RapScene::goToSceneAfterAnimation, this)), NULL));

}

void RapScene::createAprendeLabels()
{
    aprendeLabel1 = Label::createWithTTF("", "fonts/JoystixMonospace.ttf", 25);
    aprendeLabel1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    aprendeLabel1->setOpacity(0);
    this->addChild(aprendeLabel1);
    
    aprendeLabel2 = Label::createWithTTF("", "fonts/JoystixMonospace.ttf", 25);
    aprendeLabel2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 90));
    aprendeLabel2->setOpacity(0);
    this->addChild(aprendeLabel2);
}

void RapScene::animateAprendeLabels()
{
    aprendeLabel1->runAction(RepeatForever::create(Blink::create(1.0, 20)));
    aprendeLabel2->runAction(RepeatForever::create(Blink::create(1.0, 20)));
}

void RapScene::goToSceneAfterAnimation()
{
    GameManager::getInstance()->isGoingToRapScene = false;
    CustomDirector *customDirector = (CustomDirector *)Director::getInstance();
    customDirector->popSceneWithTransition<TransitionFade>(1.0);
}




