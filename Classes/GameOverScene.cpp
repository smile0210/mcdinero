//
//  GameOverScene.cpp
//  McDinero
//
//  Created by Smile0210 on 23/02/15.
//
//

#include "GameOverScene.h"
#include "GameManager.h"
#include "AudioEngine.h"
#include "McDineroTypes.h"
#include "GoalsScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdmobHelper.h"
#include "platform/android/jni/JniHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../proj.ios_mac/ios/IOSHelper.h"
#endif

Scene* GameOverScene::createScene(int points, CharacterEquipment equipment)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();
    layer->setTotalScore(points);
    layer->setMaxCharacterEquipment(equipment);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    initVariables();
    loadTextures();
    if (GameManager::getInstance()->soundActive)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kErrorSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kFalladoSound);
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize.height -= 75;
    
    resultScoreLabel = Label::createWithTTF("Tu puntuación", "fonts/JoystixMonospace.ttf", 25);
    resultScoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 5));
    this->addChild(resultScoreLabel, 10);
    
    scoreLabel = Label::createWithTTF("-", "fonts/JoystixMonospace.ttf", 45);
    scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    this->addChild(scoreLabel, 10);
    
    continueLabel = Label::createWithTTF("¡Aprende algo \n dinero!", "fonts/JoystixMonospace.ttf", 25);
    continueLabel->setAlignment(TextHAlignment::CENTER);
    
    continueLabel2 = Label::createWithTTF("(continuar)", "fonts/JoystixMonospace.ttf", 12);
    continueLabel2->setAlignment(TextHAlignment::CENTER);
    continueLabel2->setPosition(Vec2(visibleSize.width / 2, 10));
    this->addChild(continueLabel2, 10);
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto continueBackground = LayerColor::create(Color4B(255,0,0,255), visibleSize.width, continueLabel->getContentSize().height * 1.7);
    this->addChild(continueBackground);
    
    auto continueButton = MenuItemLabel::create(continueLabel, CC_CALLBACK_1(GameOverScene::goToMainScene, this));
    
    auto menu = Menu::create(continueButton, NULL);
    menu->alignItemsHorizontally();
    menu->setAnchorPoint(Vec2(0.5, 0));
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + continueLabel->getContentSize().height));
    this->addChild(menu);
    
    
    homeButton = MenuItemImage::create("BackWhite.png", "BackWhite.png", CC_CALLBACK_1(GameOverScene::goToMenuScene, this));
    homeButton->setScale(0.65);
    
    universalShareButton = MenuItemImage::create("share.png", "share.png", CC_CALLBACK_1(GameOverScene::universalShare, this));
    universalShareButton->setScale(0.65);
    
    facebookButton = MenuItemImage::create("FacebookWhite.png", "FacebookWhite.png", CC_CALLBACK_1(GameOverScene::facebookShare, this));
    facebookButton->setScale(0.65);
    
    twitterButton = MenuItemImage::create("TwitterWhite.png", "TwitterWhite.png", CC_CALLBACK_1(GameOverScene::twitterShare, this));
    twitterButton->setScale(0.65);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    auto menu2 = Menu::create(homeButton, universalShareButton, NULL);
    menu2->alignItemsHorizontallyWithPadding(170);
    
    menu2->setAnchorPoint(Vec2(0.5, 0));
    menu2->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height - 110));
    this->addChild(menu2);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    auto menu2 = Menu::create(homeButton, facebookButton, twitterButton, NULL);
    menu2->alignItemsHorizontallyWithPadding(80);
    
    menu2->setAnchorPoint(Vec2(0.5, 0));
    menu2->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height - 110));
    this->addChild(menu2);
    
#endif
    
    int totalAchievementsUnlocked = GameManager::getInstance()->totalNewAchievementsUnloked;
    
    if (totalAchievementsUnlocked > 0)
    {
        std::string achievementsStr;
        if (totalAchievementsUnlocked == 1)
        {
            achievementsStr = cocos2d::StringUtils::format("NUEVO TROFEO");
        }
        else
        {
            achievementsStr = cocos2d::StringUtils::format("%i NUEVOS TROFEOS", totalAchievementsUnlocked);
        }
        unlockedLabel = Label::createWithTTF(achievementsStr, "fonts/JoystixMonospace.ttf", 20);
        unlockedLabel->setAlignment(TextHAlignment::CENTER);
        
        auto blinkSequence = Sequence::create(
                                              DelayTime::create(1.0),
                                              FadeTo::create(0.25, 0),
                                              FadeTo::create(0.25, 255),
                                              DelayTime::create(0.5),
                                              NULL);
        
        unlockedLabel->runAction(RepeatForever::create(blinkSequence));
        
        auto menuUnlockedItem = MenuItemLabel::create(unlockedLabel, CC_CALLBACK_0(GameOverScene::goToGoalsScene, this));
        auto menuUnlocked = Menu::create(menuUnlockedItem, NULL);
        menuUnlocked->alignItemsHorizontally();
        //menuUnlocked->setAnchorPoint(Vec2(0.5, 0));
        menuUnlocked->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height - 160));
        this->addChild(menuUnlocked);
    }
    
    return true;
}

void GameOverScene::initVariables()
{
    hasNewHighScore = false;
    contentCreated = false;
    noErrorSoundId = 0;
}

void GameOverScene::loadTextures()
{
    backTexture = Director::getInstance()->getTextureCache()->addImage("BackWhite.png");
    facebookTexture = Director::getInstance()->getTextureCache()->addImage("FacebookWhite.png");
    twitterTexture = Director::getInstance()->getTextureCache()->addImage("TwitterWhite.png");
    universalShareTexture = Director::getInstance()->getTextureCache()->addImage("share.png");
}

void GameOverScene::createCharacter()
{
    Texture2D *characterTexture1;
    Texture2D *characterTexture2;
    
    switch (maxCharacterEquipment)
    {
        case None:
            characterTexture1 = Director::getInstance()->getTextureCache()->addImage("CharacterRight.png");
            characterTexture2 = Director::getInstance()->getTextureCache()->addImage("CharacterRight2.png");
            break;
        case Glasses:
            characterTexture1 = Director::getInstance()->getTextureCache()->addImage("CharacterRightGlasses.png");
            characterTexture2 = Director::getInstance()->getTextureCache()->addImage("CharacterRightGlasses2.png");
            break;
        case Cloak:
            characterTexture1 = Director::getInstance()->getTextureCache()->addImage("CharacterRightCloak.png");
            characterTexture2 = Director::getInstance()->getTextureCache()->addImage("CharacterRightCloak2.png");
            break;
        case Crown:
            characterTexture1 = Director::getInstance()->getTextureCache()->addImage("CharacterRightCrown.png");
            characterTexture2 = Director::getInstance()->getTextureCache()->addImage("CharacterRightCrown2.png");
            break;
        default:
            characterTexture1 = Director::getInstance()->getTextureCache()->addImage("CharacterRight.png");
            characterTexture2 = Director::getInstance()->getTextureCache()->addImage("CharacterRight2.png");
            break;
    }
    
    character = Sprite::createWithTexture(characterTexture1);
    character->setAnchorPoint(Vec2(0.5, 0));
    character->setPosition(Vec2(visibleSize.width / 2, continueLabel->getContentSize().height * 1.7));
    this->addChild(character);
    
    if (GameManager::getInstance()->soundActive)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        int option = rand() % 2;
#else
        int option = arc4random() % 2;
#endif
        
        if (option == 0)
        {
            noErrorSoundId = AudioEngine::play2d(kErrorSound);
            this->runAction(Sequence::create(DelayTime::create(3.75), CallFunc::create(CC_CALLBACK_0(GameOverScene::stopErrorSound, this)), NULL));
        }
        else
        {
            noErrorSoundId = AudioEngine::play2d(kFalladoSound);
        }
    }
}

void GameOverScene::goToMainScene(Ref *sender)
{
    if (GameManager::getInstance()->soundActive)
    {
        AudioEngine::stop(noErrorSoundId);
        AudioEngine::play2d(kAprendeSound);
    }
    
    GameManager::getInstance()->upgradeAfterRap = false;
    GameManager::getInstance()->bonusAfterRap = false;
    auto scene = DineroScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void GameOverScene::goToMenuScene(Ref *sender)
{
    if (GameManager::getInstance()->soundActive)
    {
        AudioEngine::stop(noErrorSoundId);
    }
 
    GameManager::getInstance()->upgradeAfterRap = false;
    GameManager::getInstance()->bonusAfterRap = false;
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void GameOverScene::facebookShare(cocos2d::Ref *sender)
{
    //CCLOG("FACEBOOK");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::shareScore(totalScore, hasNewHighScore, 0);
#endif
}

void GameOverScene::universalShare(cocos2d::Ref *sender)
{
    //CCLOG("UNIVERSAL");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "alertJNI", "(III)V"))
    {
        return;
    }
    
    int hsInt = hasNewHighScore ? 1 : 0;
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, totalScore, hsInt, maxCharacterEquipment);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void GameOverScene::twitterShare(cocos2d::Ref *sender)
{
    //CCLOG("TWITTER");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::shareScore(totalScore, hasNewHighScore, 1);
#endif
}

void GameOverScene::onEnter()
{
    Layer::onEnter();
    
    if (!contentCreated)
    {
        createCharacter();
        
        UserDefault *userDefault = UserDefault::getInstance();
        
        auto prevHighScore = userDefault->getIntegerForKey("HighScore");
        
        if (prevHighScore < totalScore)
        {
            userDefault->setIntegerForKey("HighScore", totalScore);
            hasNewHighScore = true;
        }
        
        userDefault->flush();
        
        std::string score = cocos2d::StringUtils::format("%i",totalScore);
        scoreLabel->setString(score);
        
        if (hasNewHighScore)
        {
            resultScoreLabel->setString("Nuevo record");
            resultScoreLabel->runAction(RepeatForever::create(Blink::create(1, 4)));
        }
        
        contentCreated = true;
    }
}


void GameOverScene::stopErrorSound()
{
    AudioEngine::stop(noErrorSoundId);
}

void GameOverScene::goToGoalsScene()
{
    auto scene = GoalsScene::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(1, scene));
}


