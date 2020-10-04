//
//  DineroScene.cpp
//  McDinero
//
//  Created by Smile0210 on 20/02/15.
//
//

#include "DineroScene.h"
#include "MoneySpriteNode.h"
#include "GameOverScene.h"
#include "RapScene.h"
#include "GameManager.h"
#include "AudioEngine.h"
#include "AudioEngine.h"
#include "PauPauSpriteNode.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdmobHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../proj.ios_mac/ios/IOSHelper.h"
#endif

bool upgradeAfterRap = false;

Scene* DineroScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = DineroScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool DineroScene::init()
{
    
    if ( !NodeGrid::init() )
    {
        return false;
    }
    
    initVariables();
    loadTextures();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    playerScoreLabel = Label::createWithTTF("$0", "fonts/JoystixMonospace.ttf", 18);
    playerScoreLabel->setAnchorPoint(Vec2(0, 0));
    playerScoreLabel->setPosition(Vec2(8, visibleSize.height - 65));
    playerScoreLabel->setAlignment(TextHAlignment::LEFT);
    this->addChild(playerScoreLabel, 10);
    
    scoreInfoLabel = Label::createWithTTF("", "fonts/JoystixMonospace.ttf", 32);
    scoreInfoLabel->setAnchorPoint(Vec2(1, 0));
    scoreInfoLabel->setPosition(Vec2(visibleSize.width - 10, visibleSize.height - 50));
    scoreInfoLabel->setAlignment(TextHAlignment::RIGHT);
    scoreInfoLabel->setOpacity(0);
    this->addChild(scoreInfoLabel, 10);
    
    infoLabel = Label::createWithTTF("", "fonts/JoystixMonospace.ttf", 35);
    infoLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    infoLabel->setAlignment(TextHAlignment::CENTER);
    infoLabel->setOpacity(0);
    this->addChild(infoLabel, 10);
    
    character = Sprite::createWithTexture(characterRightTexture);
    character->setScale(0.5);
    
    auto physicsBody = PhysicsBody::createBox(Size(character->getContentSize().width * 0.5, character->getContentSize().height * 0.5));
    physicsBody->setDynamic(false);
    //physicsBody->setCategoryBitmask( characterCategory );
    physicsBody->setCollisionBitmask(characterCategory);
    physicsBody->setContactTestBitmask(true);
    //physicsBody->setContactTestBitmask(moneyCategory|errorCategory|crownCategory);
    character->setPhysicsBody(physicsBody);
    
    character->setPosition(Vec2(visibleSize.width/2 + origin.x, character->getContentSize().height * characterYPositionMutiplier));
    this->addChild(character, 10);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = CC_CALLBACK_2(DineroScene::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(DineroScene::onContactBegin, this);
    dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    pauseButtonNode = Sprite::createWithTexture(pauseTexture);
    
    auto pauseButton = MenuItemLabel::create(pauseButtonNode, CC_CALLBACK_0(DineroScene::togglePausePlayGame, this));
    pauseButton->setScale(0.5);
    
    auto menuPauseButton = Menu::create(pauseButton, NULL);
    menuPauseButton->alignItemsHorizontally();
    menuPauseButton->setAnchorPoint(Vec2(0.5, 0));
    menuPauseButton->setPosition(Vec2(visibleSize.width - 30, visibleSize.height - 75));
    this->addChild(menuPauseButton, 10);
    
    this->scheduleUpdate();
    
    this->createLives();
    this->createProgressBar();
    
    // This is the only action called here because we need to create pau pau until the user is playing after a minute
    auto makePauPau = Sequence::create(
                                       DelayTime::create(30 + randomDelayTime(15)),
                                       //DelayTime::create(3),
                                       CallFunc::create(CC_CALLBACK_0(DineroScene::addPauPau, this)),
                                       NULL);
    
    makePauPau->setTag(MakePauPauActionType);
    this->runAction(makePauPau);
    
    GameManager::getInstance()->restartValues();
    
    return true;
}

void DineroScene::update(float dt)
{
    
    if ((character->getPosition().x < 0 && movementDirection < 0) || (character->getPosition().x > visibleSize.width && movementDirection > 0))
    {
        character->stopActionByTag(MoveXActionType);
        
        if (character->getPosition().x < 0)
        {
            //character->setPosition(Vec2(1, character->getContentSize().height * characterYPositionMutiplier));
            character->setPosition(Vec2(1, character->getPositionY()));
        }
        else
        {
            //character->setPosition(Vec2(visibleSize.width - 1, character->getContentSize().height * characterYPositionMutiplier));
            character->setPosition(Vec2(visibleSize.width - 1, character->getPositionY()));
        }
        
    }
    
    if (characterEmmiter != nullptr)
    {
        characterEmmiter->setPosition(character->getPosition());
    }
    
}

bool DineroScene::onTouchBegan(Touch *touch, Event *event)
{
    if (!isGamePaused)
    {
        character->stopActionByTag(MoveXActionType);
        
        movementDirection *= -1;
        
        if (movementDirection < 0)
        {
            updateCharacterForPosition(Left);
        }
        else
        {
            updateCharacterForPosition(Right);
        }
        
        auto movementAction = RepeatForever::create(MoveBy::create(playerMovementDuration, Vec2(movementDirection, 0)));
        movementAction->setTag(MoveXActionType);
        character->runAction(movementAction);
    }
    
    return true;
}

void DineroScene::initVariables()
{
    movementDirection = -20.0;
    playerMovementDuration = 0.05;
    characterEquipment = None;
    maxEquipment = None;
    maxDownTime = 300;
    playerScore = 0;
    playerLives = 0;
    playerGoalScore = 1000;
    maxEquiomentCount = 0;
    countMoneyCath = 0;
    avoidedCrowns = 0;
    avoidedCrownsContinuesFinal = 0;
    avoidedCrownsContinues = 0;
    avoidedErrors = 0;
    avoidedErrorsContinuesFinal = 0;
    avoidedErrorsContinues = 0;
    achievementsUnloked = 0;
    
    waitDurationCash = 2.0;
    waitDurationError = 6.0;
    waitDurationCrown = 10.0;
    
    canPlayDineroSound = true;
    isGamePaused = false;
    isBonusActive = false;
    
    bonusCount = 0;
    bonusTotalPoints = 0;
    
    characterYPositionMutiplier = 0.25;
    moneyScaleMultiplier = 0.5;
    
}

void DineroScene::loadTextures()
{
    characterLeftTexture = Director::getInstance()->getTextureCache()->addImage("CharacterLeft.png");
    characterLeftGlassesTexture = Director::getInstance()->getTextureCache()->addImage("CharacterLeftGlasses.png");
    characterLeftCloakTexture = Director::getInstance()->getTextureCache()->addImage("CharacterLeftCloak.png");
    characterLeftCrownTexture = Director::getInstance()->getTextureCache()->addImage("CharacterLeftCrown.png");
    
    characterRightTexture = Director::getInstance()->getTextureCache()->addImage("CharacterRight.png");
    characterRightGlassesTexture = Director::getInstance()->getTextureCache()->addImage("CharacterRightGlasses.png");
    characterRightCloakTexture = Director::getInstance()->getTextureCache()->addImage("CharacterRightCloak.png");
    characterRightCrownTexture = Director::getInstance()->getTextureCache()->addImage("CharacterRightCrown.png");
    
    moneyTexture01 = Director::getInstance()->getTextureCache()->addImage("P01.png");
    moneyTexture02 = Director::getInstance()->getTextureCache()->addImage("P02.png");
    moneyTexture05 = Director::getInstance()->getTextureCache()->addImage("P05.png");
    moneyTexture10 = Director::getInstance()->getTextureCache()->addImage("P10.png");
    moneyTexture20 = Director::getInstance()->getTextureCache()->addImage("P20.png");
    moneyTexture50 = Director::getInstance()->getTextureCache()->addImage("P50.png");
    moneyTexture100 = Director::getInstance()->getTextureCache()->addImage("P100.png");
    moneyTexture200 = Director::getInstance()->getTextureCache()->addImage("P200.png");
    moneyTexture500 = Director::getInstance()->getTextureCache()->addImage("P500.png");
    centenario = Director::getInstance()->getTextureCache()->addImage("centenario.png");
    
    pauPauYellowTexture = Director::getInstance()->getTextureCache()->addImage("pau_amarillo.png");
    pauPauRedTexture = Director::getInstance()->getTextureCache()->addImage("pau_rojo.png");
    pauPauGreenTexture = Director::getInstance()->getTextureCache()->addImage("pau_verde.png");
    pauPauOrangeTexture = Director::getInstance()->getTextureCache()->addImage("pau_naranja.png");
    pauPauPurpleTexture = Director::getInstance()->getTextureCache()->addImage("pau_morado.png");
    pauPauPinkTexture = Director::getInstance()->getTextureCache()->addImage("pau_rosa.png");
    
    errorTexture = Director::getInstance()->getTextureCache()->addImage("ErrorChar.png");
    crownTexture = Director::getInstance()->getTextureCache()->addImage("EvilCrown.png");
    
    heartTexture = Director::getInstance()->getTextureCache()->addImage("PHeartLittle.png");
    progressTexture = Director::getInstance()->getTextureCache()->addImage("progress.png");
    progressTextureEmpty = Director::getInstance()->getTextureCache()->addImage("progressEmpty.png");
    playTexture = Director::getInstance()->getTextureCache()->addImage("play_button.png");
    pauseTexture = Director::getInstance()->getTextureCache()->addImage("pause_button.png");
    
//    if (GameManager::getInstance()->soundActive)
//    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kDineroSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kDineroMiniSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kErrorSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kErrorMiniSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kEspanaAtacaSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kEspanaAtacaMiniSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kPauPauPauSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kYiyiyiSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kYuyuyuSound);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kPauPauPauSound2);
//        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kAprendeSound);
//    }
    
}

void DineroScene::updateCharacterForPosition(CharacterDirection direction)
{
    switch (direction) {
        case Left:
            switch (characterEquipment) {
                case None:
                    character->setTexture(characterLeftTexture);
                    break;
                case Glasses:
                    character->setTexture(characterLeftGlassesTexture);
                    break;
                case Cloak:
                    character->setTexture(characterLeftCloakTexture);
                    break;
                case Crown:
                    character->setTexture(characterLeftCrownTexture);
                    break;
                default:
                    character->setTexture(characterLeftTexture);
                    break;
            }
            break;
            
        case Right:
            switch (characterEquipment) {
                case None:
                    character->setTexture(characterRightTexture);
                    break;
                case Glasses:
                    character->setTexture(characterRightGlassesTexture);
                    break;
                case Cloak:
                    character->setTexture(characterRightCloakTexture);
                    break;
                case Crown:
                    character->setTexture(characterRightCrownTexture);
                    break;
                default:
                    character->setTexture(characterRightTexture);
                    break;
            }
            break;
            
        default:
            switch (characterEquipment) {
                case None:
                    character->setTexture(characterRightTexture);
                    break;
                case Glasses:
                    character->setTexture(characterRightGlassesTexture);
                    break;
                case Cloak:
                    character->setTexture(characterRightCloakTexture);
                    break;
                case Crown:
                    character->setTexture(characterRightCrownTexture);
                    break;
                default:
                    character->setTexture(characterRightTexture);
                    break;
            }
            break;
    }
}

void DineroScene::removeActionsToScene()
{
    this->stopActionByTag(MakeCashActionType);
    this->stopActionByTag(MakeErrorActionType);
    this->stopActionByTag(MakeCrownActionType);
}

void DineroScene::addActionsToScene()
{
    removeActionsToScene();
    
    addCash();
    addError();
    addCrown();
    
}

void DineroScene::addCash()
{
    MoneySpriteNode *cashNode;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    int moneyType = rand() % ((playerScore > 5000) ? MoneyMax : Quinientos);
#else
    int moneyType = arc4random() % ((playerScore > 5000) ? MoneyMax : Quinientos);
#endif
    
    int multiplier = GameManager::getInstance()->isGigantMoneyActive ? 3 : 1;
    
    switch (moneyType) {
        case Uno:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture01, 1 * multiplier);
            break;
        case Dos:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture02, 2 * multiplier);
            break;
        case Cinco:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture05, 5 * multiplier);
            break;
        case Diez:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture10, 10 * multiplier);
            break;
        case Veinte:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture20, 20 * multiplier);
            break;
        case Cincuenta:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture50, 50 * multiplier);
            break;
        case Cien:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture100, 100 * multiplier);
            break;
        case Doscientos:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture200, 200 * multiplier);
            break;
        case Quinientos:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture500, 500 * multiplier);
            break;
        default:
            cashNode = MoneySpriteNode::createWithTextureAndPoints(moneyTexture01, 1 * multiplier);
            break;
    }
    
    cashNode->setScale(moneyScaleMultiplier);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    float positionX = rand() % (int)(visibleSize.width - cashNode->getBoundingBox().size.width);
#else
    float positionX = arc4random() % (int)(visibleSize.width - cashNode->getBoundingBox().size.width);
#endif
    
    positionX += (cashNode->getBoundingBox().size.width / 2);
    cashNode->setPosition(Vec2(positionX, visibleSize.height + cashNode->getBoundingBox().size.height));
    cashNode->setName("cash");
    cashNode->setTag(CashType);
    
    auto physicsBody = PhysicsBody::createBox(Size(cashNode->getContentSize().width * moneyScaleMultiplier, cashNode->getContentSize().height * moneyScaleMultiplier));
    physicsBody->setCollisionBitmask(moneyCategory);
    physicsBody->setContactTestBitmask(characterCategory);
    physicsBody->setGravityEnable(false);
    cashNode->setPhysicsBody(physicsBody);
    
    cashNode->runAction(Sequence::create(
                                         MoveTo::create(randomTime(), Vec2(positionX, -160)),
                                         RemoveSelf::create(),
                                         NULL));
    
    this->addChild(cashNode);
    
    auto makeCash = Sequence::create(DelayTime::create(randomDelayTime(waitDurationCash)),CallFunc::create(CC_CALLBACK_0(DineroScene::addCash,  this)), NULL);
    makeCash->setTag(MakeCashActionType);
    this->runAction(makeCash);
}

void DineroScene::addError()
{
    if (playerScore > 100)
    {
        auto errorNode = Sprite::createWithTexture(errorTexture);
        
        errorNode->setScale(0.65);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        float positionX = rand() % (int)(visibleSize.width - errorNode->getBoundingBox().size.width);
#else
        float positionX = arc4random() % (int)(visibleSize.width - errorNode->getBoundingBox().size.width);
#endif
        
        positionX += (errorNode->getBoundingBox().size.width / 2);
        errorNode->setPosition(Vec2(positionX, visibleSize.height + errorNode->getBoundingBox().size.height));
        errorNode->setName("error");
        errorNode->setTag(ErrorType);
        
        auto physicsBody = PhysicsBody::createBox(Size(errorNode->getContentSize().width * 0.65, errorNode->getContentSize().height * 0.65));
        physicsBody->setCollisionBitmask(errorCategory);
        physicsBody->setContactTestBitmask(characterCategory);
        physicsBody->setGravityEnable(false);
        errorNode->setPhysicsBody(physicsBody);
        
        errorNode->runAction(RepeatForever::create(Sequence::create(
                                                                    RotateBy::create(0.05, -6.0),
                                                                    RotateBy::create(0.05, 0.0),
                                                                    RotateBy::create(0.05, 6.0),
                                                                    RotateBy::create(0.05, 0.0), NULL)));
        errorNode->runAction(RepeatForever::create(Blink::create(1, 8)));
        errorNode->runAction(Sequence::create(
                                              MoveTo::create(randomTime(), Vec2(positionX, -160)),
                                              CallFunc::create(CC_CALLBACK_0(DineroScene::avoidedError, this)),
                                              RemoveSelf::create(),
                                              NULL));
        
        this->addChild(errorNode);
        
    }
    
    auto makeError = Sequence::create( DelayTime::create(randomDelayTime(waitDurationError) + 0.25), CallFunc::create(CC_CALLBACK_0(DineroScene::addError, this)), NULL);
    makeError->setTag(MakeErrorActionType);
    this->runAction(makeError);
    
}

void DineroScene::addCrown()
{
    if (playerScore > 100)
    {
        auto crownNode = Sprite::createWithTexture(crownTexture);
        
        crownNode->setScale(0.33);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        float positionX = rand() % (int)(visibleSize.width - crownNode->getBoundingBox().size.width);
#else
        float positionX = arc4random() % (int)(visibleSize.width - crownNode->getBoundingBox().size.width);
#endif
        
        positionX += (crownNode->getBoundingBox().size.width / 2);
        crownNode->setPosition(Vec2(positionX, visibleSize.height + crownNode->getBoundingBox().size.height));
        crownNode->setName("crown");
        crownNode->setTag(CrownType);
        
        ParticleSystemQuad *crownEmmiter = ParticleSystemQuad::create("particles/particle_texture_crown.plist");
        crownEmmiter->setScale(0.35);
        crownEmmiter->setPosition(crownNode->getPosition());
        crownEmmiter->setTag(CrownEmmiterType);
        this->addChild(crownEmmiter);
        
        auto physicsBody = PhysicsBody::createBox(Size(crownNode->getContentSize().width * 0.33, crownNode->getContentSize().height * 0.33));
        physicsBody->setCollisionBitmask(crownCategory);
        physicsBody->setContactTestBitmask(characterCategory);
        physicsBody->setGravityEnable(false);
        crownNode->setPhysicsBody(physicsBody);
        
        float goDownTime = randomTime();
        
        crownNode->runAction(Sequence::create(
                                              MoveTo::create(goDownTime, Vec2(positionX, -160)),
                                              CallFunc::create(CC_CALLBACK_0(DineroScene::avoidedCrown, this)),
                                              RemoveSelf::create(),
                                              NULL));
        
        crownEmmiter->runAction(Sequence::create(
                                                 MoveTo::create(goDownTime, Vec2(positionX, -160)),
                                                 RemoveSelf::create(),
                                                 NULL));
        
        this->addChild(crownNode);
        
    }
    
    auto makeCrown = Sequence::create(DelayTime::create(randomDelayTime(waitDurationCrown) + 0.5), CallFunc::create(CC_CALLBACK_0(DineroScene::addCrown, this)), NULL);
    makeCrown->setTag(MakeCrownActionType);
    this->runAction(makeCrown);
    
}

void DineroScene::addPauPau()
{
    PauPauSpriteNode *pauPauNode;
    ParticleSystemQuad *particle;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    int pauPauType = rand() % PauPauMax;
#else
    int pauPauType = arc4random() % PauPauMax;
#endif
    
    switch (pauPauType) {
        case PauPauAmarillo:
            pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauYellowTexture, PauPauAmarillo);
            particle = ParticleSystemQuad::create("particles/particle_texture_paupau_yellow.plist");
            break;
        case PauPauRojo:
            pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauRedTexture, PauPauRojo);
            particle = ParticleSystemQuad::create("particles/particle_texture_paupau_red.plist");
            break;
        case PauPauVerde:
            pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauGreenTexture, PauPauVerde);
            particle = ParticleSystemQuad::create("particles/particle_texture_paupau_green.plist");
            break;
        case PauPauNaranja:
            pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauOrangeTexture, PauPauNaranja);
            particle = ParticleSystemQuad::create("particles/particle_texture_paupau_orange.plist");
            break;
        case PauPauMorado:
            pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauPurpleTexture, PauPauMorado);
            particle = ParticleSystemQuad::create("particles/particle_texture_paupau_purple.plist");
            break;
        case PauPauRosa:
            pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauPinkTexture, PauPauRosa);
            particle = ParticleSystemQuad::create("particles/particle_texture_paupau_pink.plist");
            break;
        default:
            pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauYellowTexture, PauPauAmarillo);
            particle = ParticleSystemQuad::create("particles/particle_texture_paupau_yellow.plist");
            break;
    }
    
//    pauPauNode = PauPauSpriteNode::createWithTextureAndPauPauType(pauPauPinkTexture, PauPauRosa);
//    particle = ParticleSystemQuad::create("particles/particle_texture_paupau_pink.plist");
    
    pauPauNode->setScale(0.4);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    float positionX = rand() % (int)(visibleSize.width - pauPauNode->getBoundingBox().size.width);
#else
    float positionX = arc4random() % (int)(visibleSize.width - pauPauNode->getBoundingBox().size.width);
#endif
    
    positionX += (pauPauNode->getBoundingBox().size.width / 2);
    pauPauNode->setPosition(Vec2(positionX, visibleSize.height + pauPauNode->getBoundingBox().size.height));
    pauPauNode->setName("paupau");
    pauPauNode->setTag(PauPauNodeType);

    particle->setScale(0.4);
    particle->setTag(PauPauEmmiterType);
    particle->setPosition(Vec2(positionX, visibleSize.height + pauPauNode->getBoundingBox().size.height));
    
    auto physicsBody = PhysicsBody::createBox(Size(pauPauNode->getContentSize().width * 0.33, pauPauNode->getContentSize().height * 0.33));
    physicsBody->setCollisionBitmask(paupauCategory);
    physicsBody->setContactTestBitmask(characterCategory);
    physicsBody->setGravityEnable(false);
    pauPauNode->setPhysicsBody(physicsBody);
    
    pauPauNode->runAction(RepeatForever::create(Sequence::create(
                                                                 MoveBy::create(0.4, Vec2(30, 0)),
                                                                 MoveBy::create(0.8, Vec2(-60, 0)),
                                                                 MoveBy::create(0.4, Vec2(30, 0)),
                                                                 NULL)));
    
    particle->runAction(RepeatForever::create(Sequence::create(
                                                               MoveBy::create(0.4, Vec2(30, 0)),
                                                               MoveBy::create(0.8, Vec2(-60, 0)),
                                                               MoveBy::create(0.4, Vec2(30, 0)),
                                                               NULL)));
    
    
    pauPauNode->runAction(Sequence::create(
                                           MoveTo::create(3, Vec2(positionX, -160)),
                                           RemoveSelf::create(),
                                           NULL));
    
    particle->runAction(Sequence::create(
                                         MoveTo::create(3, Vec2(positionX, -160)),
                                         RemoveSelf::create(),
                                         NULL));
    
    this->addChild(particle);
    this->addChild(pauPauNode);
    
    auto makePauPau = Sequence::create(
                                       DelayTime::create(30 + randomDelayTime(15)),
                                       //DelayTime::create(3),
                                       CallFunc::create(CC_CALLBACK_0(DineroScene::addPauPau, this)),
                                       NULL);
    
    makePauPau->setTag(MakePauPauActionType);
    this->runAction(makePauPau);
    
}

void DineroScene::createLives()
{
    playerLives = 3;
    
    heartOne = Sprite::createWithTexture(heartTexture);
    heartOne->setPosition(Vec2(15, visibleSize.height - 15));
    this->addChild(heartOne, 10);
    
    heartTwo = Sprite::createWithTexture(heartTexture);
    heartTwo->setPosition(Vec2(35, visibleSize.height - 15));
    this->addChild(heartTwo, 10);
    
    heartThree = Sprite::createWithTexture(heartTexture);
    heartThree->setPosition(Vec2(55, visibleSize.height - 15));
    this->addChild(heartThree, 10);
}

void DineroScene::updateLives()
{
    if (characterEquipment != None)
    {
        
        switch (characterEquipment)
        {
            case Crown:
                characterEquipment = Cloak;
                break;
            case Cloak:
                characterEquipment = Glasses;
                break;
            case Glasses:
                characterEquipment = None;
                break;
            default:
                characterEquipment = None;
                break;
        }
        
        if (movementDirection < 0)
        {
            updateCharacterForPosition(Left);
        }
        else
        {
            updateCharacterForPosition(Right);
        }
        
        updateProgressBar();
        
    }
    else
    {
        if (playerLives > 0)
        {
            playerLives -= 1;
            
            switch (playerLives)
            {
                case 0:
                    removeNodeAfterBlink(heartOne);
                    break;
                case 1:
                    removeNodeAfterBlink(heartTwo);
                    break;
                case 2:
                    removeNodeAfterBlink(heartThree);
                    break;
                default:
                    CCLOG("Unknown state of lives...");
                    break;
            }
            
            updateProgressBar();
            
        }
    }
    
}

void DineroScene::createProgressBar()
{
    progressBarEmpty = Sprite::createWithTexture(progressTextureEmpty);
    progressBarEmpty->setScale(0.4);
    progressBarEmpty->setAnchorPoint(Vec2(0.0, 0.0));
    progressBarEmpty->setPosition(Vec2(8, visibleSize.height - 40));
    this->addChild(progressBarEmpty, 9);
    
    progressBar = Sprite::createWithTexture(progressTexture);
    
    barProgressTimer = ProgressTimer::create(progressBar);
    barProgressTimer->setScale(0.4);
    barProgressTimer->setAnchorPoint(Vec2(0.0, 0.0));
    barProgressTimer->setType(ProgressTimer::Type::BAR);
    barProgressTimer->setMidpoint(Vec2(0, 0));
    barProgressTimer->setBarChangeRate(Vec2(1, 0));
    barProgressTimer->setPosition(Vec2(8, visibleSize.height - 40));
    this->addChild(barProgressTimer, 10);
    
    barProgressTimer->setPercentage(0);
}

void DineroScene::updateProgressBar()
{
    float progress = (float)playerScore / (float)playerGoalScore;
    
    if (progress > 1)
    {
        character->stopActionByTag(MoveXActionType);
        deleteNodesFromScene();
        if (!GameManager::getInstance()->isGoingToRapScene)
        {
            goToRapScene();
        }
    }
    
    barProgressTimer->setPercentage(progress * 100);
}

float DineroScene::randomTime()
{
    //    return (((float) rand() / RAND_MAX) * maxDownTime) + 100;
    return (float)((rand() % maxDownTime) + 100) / 100;
}

double DineroScene::randomDelayTime(double x)
{
    // This will generate a number from 0.0 to 1.0, inclusive.
    // float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // This will generate a number from 0.0 to some arbitrary float, X:
    // float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X));
    // This will generate a number from some arbitrary LO to some arbitrary HI:
    // float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    
    double r = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/x));
    return r;
}

double DineroScene::randomDelayTime(double x, double y)
{
    // This will generate a number from 0.0 to 1.0, inclusive.
    // float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // This will generate a number from 0.0 to some arbitrary float, X:
    // float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X));
    // This will generate a number from some arbitrary LO to some arbitrary HI:
    // float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    
    double r = x + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(y-x)));
    return r;
}

void DineroScene::deleteNodesFromScene()
{
    
    Node *errorNode;
    
    while ((errorNode = this->getChildByTag(ErrorType)) != NULL)
    {
        errorNode->stopAllActions();
        errorNode->removeFromParent();
    }
    
    Node *crownNode;
    
    while ((crownNode = this->getChildByTag(CrownType)) != NULL)
    {
        crownNode->stopAllActions();
        crownNode->removeFromParent();
    }
    
    Node *crownEmmiterNode;
    
    while ((crownEmmiterNode = this->getChildByTag(CrownEmmiterType)) != NULL)
    {
        crownEmmiterNode->stopAllActions();
        crownEmmiterNode->removeFromParent();
    }
    
    Node *cashNode;
    
    while ((cashNode = this->getChildByTag(CashType)) != NULL)
    {
        cashNode->stopAllActions();
        cashNode->removeFromParent();
    }
    
    Node *paupauNode;
    
    while ((paupauNode = this->getChildByTag(PauPauNodeType)) != NULL)
    {
        paupauNode->stopAllActions();
        paupauNode->removeFromParent();
    }
    
    Node *pauPauEmmiterNode;
    
    while ((pauPauEmmiterNode = this->getChildByTag(PauPauEmmiterType)) != NULL)
    {
        pauPauEmmiterNode->stopAllActions();
        pauPauEmmiterNode->removeFromParent();
    }
    
}

void DineroScene::goToRapScene()
{
    GameManager::getInstance()->isGoingToRapScene = true;
    auto scene = RapScene::createScene(characterEquipment);
    Director::getInstance()->pushScene(TransitionFade::create(1, scene));
}

void DineroScene::goToGameOverScene()
{
    updateAchievements();
    auto scene = GameOverScene::createScene(playerScore, maxEquipment);
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void DineroScene::updateMetrics()
{
    if (waitDurationCash >= 1.0)
    {
        waitDurationCash -= 0.3;
    }
    else if (waitDurationCash >= 0.9)
    {
        waitDurationCash -= 0.1;
    }
    else if (waitDurationCash >= 0.3)
    {
        waitDurationCash -= 0.05;
    }
    
    if (waitDurationError >= 2.0)
    {
        waitDurationError -= 0.25;
    }
    
    if (waitDurationCrown >= 10)
    {
        waitDurationCrown -= 1.0;
    }
    else if (waitDurationCrown >= 2.0)
    {
        waitDurationCrown -= 0.25;
    }
    
    if (maxDownTime >= 100)
    {
        maxDownTime -= 50;
    }
    else if (maxDownTime >= 60)
    {
        maxDownTime -= 20;
    }
    
    addActionsToScene();
    
}

void DineroScene::blinkCharacter()
{
    //    character->runAction(Sequence::create(Blink::create(0.25, 15), Show::create(), NULL));
    
    if (!character->getActionByTag(BlinkActionType))
    {
        //        auto actionToRed = TintBy::create(0.05, -27, -255, -227);
        //        auto actionToRedBack = actionToRed->reverse();
        
        auto redBlink = Repeat::create(Sequence::create(TintTo::create(0.05, 255, 0, 0), TintTo::create(0.05, 255, 255, 255), NULL), 3);
        
        //        auto blink = Sequence::create(Blink::create(0.25, 6), Show::create(), NULL);
        redBlink->setTag(BlinkActionType);
        character->runAction(redBlink);
    }
}

void DineroScene::removeNodeAfterBlink(Node *node)
{
    auto sequence = Sequence::create(Blink::create(0.6, 8), Hide::create(), NULL);
    node->runAction(sequence);
    
    if (playerLives == 0)
    {
        goToGameOverScene();
    }
}

void DineroScene::updateScore(int points)
{
    playerScore += points;
    if (playerScore < 0)
    {
        playerScore = 0;
    }
    //playerScoreLabel.text = "$\(playerScore)"
//    __String *scoreStr = __String::createWithFormat("%i",playerScore);
    std::string scoreStr = "$" + std::to_string(playerScore);
    playerScoreLabel->setString(scoreStr);
    
    animatScoreInfoLabel(points);
    updateProgressBar();
}

void DineroScene::animatScoreInfoLabel(int points)
{
    scoreInfoLabel->setOpacity(255);
    
    if (points > 0)
    {
        if (GameManager::getInstance()->isRussianActive && !infoLabel->getActionByTag(BlinkAndHydeActionType))
        {
            infoLabel->setVisible(true);
            infoLabel->setOpacity(255);
            std::string russianPointsStr = "$" + cocos2d::StringUtils::format("%i x 2", points / 2);
            infoLabel->setString(russianPointsStr);
            infoLabel->runAction(FadeOut::create(0.2));
        }
        else if (GameManager::getInstance()->isGigantMoneyActive && !infoLabel->getActionByTag(BlinkAndHydeActionType))
        {
            infoLabel->setVisible(true);
            infoLabel->setOpacity(255);
            std::string russianPointsStr = "$" + cocos2d::StringUtils::format("%i x 2", points / 3);
            infoLabel->setString(russianPointsStr);
            infoLabel->runAction(FadeOut::create(0.2));
        }
    }
    else
    {
        std::string pointsStr = std::to_string(points);
        scoreInfoLabel->setString(pointsStr);
    }
    scoreInfoLabel->runAction(FadeOut::create(0.2));
    
}

bool DineroScene::onContactBegin(PhysicsContact &contact)
{
    
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    if ((characterCategory == a->getCollisionBitmask() && moneyCategory == b->getCollisionBitmask()) ||
        (characterCategory == b->getCollisionBitmask() && moneyCategory == a->getCollisionBitmask()))
    {
        if (GameManager::getInstance()->soundActive && canPlayDineroSound)
        {
            canPlayDineroSound = false;
            if (GameManager::getInstance()->isRussianActive == false)
            {
                if (GameManager::getInstance()->isMiniatureActive)
                {
                    AudioEngine::play2d(kDineroMiniSound);
                }
                else
                {
                    AudioEngine::play2d(kDineroSound);
                }
                this->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(DineroScene::resetDineroSoundFlag, this)), NULL));
            }
            else
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
                int sound = rand() % 3;
#else
                int sound = arc4random() % 3;
#endif
      
                if (sound == 0)
                {
                    AudioEngine::play2d(kYuyuyuSound);
                    this->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(DineroScene::resetDineroSoundFlag, this)), NULL));
                }
                else if (sound == 1)
                {
                    AudioEngine::play2d(kYiyiyiSound);
                    this->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(DineroScene::resetDineroSoundFlag, this)), NULL));
                }
                else
                {
                    AudioEngine::play2d(kPauPauPauSound2);
                    this->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(DineroScene::resetDineroSoundFlag, this)), NULL));
                }
            }
            
            
        }
        
        int points = 1;
        
        if (moneyCategory == a->getCollisionBitmask())
        {
            if (a->getNode() != NULL)
            {
                MoneySpriteNode *moneySprite = (MoneySpriteNode *)a->getNode();
                points = moneySprite->getPoints();
                a->getNode()->removeFromParent();
            }
        }
        else
        {
            if (b->getNode() != NULL)
            {
                MoneySpriteNode *moneySprite = (MoneySpriteNode *)b->getNode();
                points = moneySprite->getPoints();
                b->getNode()->removeFromParent();
            }
        }
        
        
        if (!isBonusActive)
        {
            countMoneyCath++;
            if (countMoneyCath == 50)
            {
                countMoneyCath = 0;
                updateMetrics();
            }
            
            if (GameManager::getInstance()->isRussianActive)
            {
                updateScore(points * 2);
            }
            else
            {
                updateScore(points);
            }
        }
        else
        {
            //CCLOG("SUMAR PUNTOS CON BONUS ACTIVO");
            bonusTotalPoints += points;
            infoLabel->setOpacity(255);
            std::string russianPointsStr = cocos2d::StringUtils::format("Bonus \n+%i",bonusTotalPoints);
            infoLabel->setString(russianPointsStr);
            infoLabel->runAction(FadeOut::create(0.1));
        }
        
    }
    else if ((characterCategory == a->getCollisionBitmask() && errorCategory == b->getCollisionBitmask()) ||
             (characterCategory == b->getCollisionBitmask() && errorCategory == a->getCollisionBitmask()))
    {
        //CCLOG("CONTACTO CON ERROR");
        if (!GameManager::getInstance()->immunityActive)
        {
            if (GameManager::getInstance()->soundActive)
            {
                if (GameManager::getInstance()->isMiniatureActive)
                {
                    AudioEngine::play2d(kErrorMiniSound);
                }
                else
                {
                    AudioEngine::play2d(kErrorSound);
                }
            }
            
            blinkCharacter();
            avoidedErrorsContinues = 0;
            
            if (errorCategory == a->getCollisionBitmask())
            {
                if (a->getNode() != NULL)
                {
                    a->getNode()->removeFromParent();
                }
            }
            else
            {
                if (b->getNode() != NULL)
                {
                    b->getNode()->removeFromParent();
                }
            }
            
            int pointsToDiscount = 50;
            
            switch (characterEquipment)
            {
                case None:
                    pointsToDiscount = 200;
                    break;
                case Glasses:
                    pointsToDiscount = 300;
                    break;
                case Cloak:
                    pointsToDiscount = 400;
                    break;
                case Crown:
                    pointsToDiscount = 600;
                    break;
                default:
                    pointsToDiscount = 100;
                    break;
            }
            
            updateScore(-pointsToDiscount);
        }
        
    }
    else if ((characterCategory == a->getCollisionBitmask() && crownCategory == b->getCollisionBitmask()) ||
             (characterCategory == b->getCollisionBitmask() && crownCategory == a->getCollisionBitmask()))
    {
        if (!GameManager::getInstance()->immunityActive)
        {
            if (GameManager::getInstance()->soundActive)
            {
                if (GameManager::getInstance()->isMiniatureActive)
                {
                    AudioEngine::play2d(kEspanaAtacaMiniSound);
                }
                else
                {
                    AudioEngine::play2d(kEspanaAtacaSound);
                }
            }
            
            blinkCharacter();
            avoidedCrownsContinues = 0;
            
            playerGoalScore *= 1.1;
            
            if (GameManager::getInstance()->isMiniatureActive)
            {
                this->stopActionByTag(ExitMiniatureActionType);
                exitMiniatureMode();
            }
            else
            {
                updateLives();
            }
            
            if (crownCategory == a->getCollisionBitmask())
            {
                if (a->getNode() != NULL)
                {
                    a->getNode()->removeFromParent();
                }
            }
            else
            {
                if (b->getNode() != NULL)
                {
                    b->getNode()->removeFromParent();
                }
            }
        }
        
    }
    else if ((characterCategory == a->getCollisionBitmask() && paupauCategory == b->getCollisionBitmask()) ||
             (characterCategory == b->getCollisionBitmask() && paupauCategory == a->getCollisionBitmask()))
    {
        if (GameManager::getInstance()->soundActive)
        {
            AudioEngine::play2d(kPauPauPauSound);
        }
        
        PauPauType pauPauType = PauPauAmarillo;
        
        if (paupauCategory == a->getCollisionBitmask())
        {
            if (a->getNode() != NULL)
            {
                PauPauSpriteNode *pauPauSprite = (PauPauSpriteNode *)a->getNode();
                pauPauType = pauPauSprite->getPauPauType();
                a->getNode()->removeFromParent();
            }
        }
        else
        {
            if (b->getNode() != NULL)
            {
                PauPauSpriteNode *pauPauSprite = (PauPauSpriteNode *)b->getNode();
                pauPauType = pauPauSprite->getPauPauType();
                b->getNode()->removeFromParent();
            }
        }
        
        auto currentKey = kAPauPauYellowKey;
        
        switch (pauPauType)
        {
            case PauPauAmarillo:
                immunityCharacter();
                currentKey = kAPauPauYellowKey;
                blinkCharacterWithPauPau(pauPauType);
                break;
            case PauPauRojo:
                createLive();
                currentKey = kAPauPauRedKey;
                break;
            case PauPauVerde:
                enterRussianMode();
                currentKey = kAPauPauGreenKey;
                blinkCharacterWithPauPau(pauPauType);
                break;
            case PauPauNaranja:
                enterMiniatureMode();
                currentKey = kAPauPauOrangeKey;
                break;
            case PauPauMorado:
                createWaves();
                currentKey = kAPauPauPurpleKey;
                break;
            case PauPauRosa:
                enterGigantMoney();
                currentKey = kAPauPauPinkKey;
                break;
            default:
                immunityCharacter();
                currentKey = kAPauPauYellowKey;
                break;
        }
        
        UserDefault *userDefault = UserDefault::getInstance();
        int currentCount = userDefault->getIntegerForKey(currentKey);
        currentCount++;
        userDefault->setIntegerForKey(currentKey, currentCount);
        userDefault->flush();
        
    }
    
    return false;
}

void DineroScene::beginMoveCharacter()
{
    auto movementAction = RepeatForever::create(MoveBy::create(playerMovementDuration, Vec2(movementDirection, 0)));
    movementAction->setTag(MoveXActionType);
    character->runAction(movementAction);
}

void DineroScene::onEnter()
{
    NodeGrid::onEnter();
    
    if (playerScore > 1500)
    {
        int deltaToGoal = playerScore * 1.5;
        
        if (deltaToGoal > 4000)
        {
            playerGoalScore += 4000;
        }
        else
        {
            playerGoalScore += deltaToGoal;
        }
        
    }
    else
    {
        playerGoalScore = 1500;
    }
    
    if (GameManager::getInstance()->upgradeAfterRap)
    {
        if (maxEquiomentCount < 3)
        {
            switch (characterEquipment)
            {
                case None:
                    maxEquipment = Glasses;
                    maxEquiomentCount++;
                    break;
                case Glasses:
                    maxEquipment = Cloak;
                    maxEquiomentCount++;
                    break;
                case Cloak:
                    maxEquipment = Crown;
                    maxEquiomentCount++;
                    break;
                default:
                    break;
            }
        }
        
                switch (characterEquipment)
                {
                    case None:
                        characterEquipment = Glasses;
                        break;
                    case Glasses:
                        characterEquipment = Cloak;
                        break;
                    case Cloak:
                        characterEquipment = Crown;
                        break;
                    default:
                        CCLOG("No more upgrades");
                        break;
                }
        
        if (movementDirection < 0)
        {
            updateCharacterForPosition(Left);
        }
        else
        {
            updateCharacterForPosition(Right);
        }
    }
    
    if (playerMovementDuration > 0.025)
    {
        playerMovementDuration -= 0.0025;
    }
    
    if (GameManager::getInstance()->bonusAfterRap)
    {
        removeActionsToScene();
        isBonusActive = true;
        enterBonus();
        //GameManager::getInstance()->bonusAfterRap = false;
    }
    else
    {
        this->runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create(CC_CALLBACK_0(DineroScene::onEnterFinish, this)) , NULL));
    }
    
    updateProgressBar();
    deleteNodesFromScene();
    
}

void DineroScene::onEnterFinish()
{
    updateMetrics();
}

void DineroScene::resetDineroSoundFlag()
{
    canPlayDineroSound = true;
}

void DineroScene::blinkCharacterWithPauPau(PauPauType pauPauType)
{
    int r = 255;
    int g = 255;
    int b = 255;
    switch (pauPauType)
    {
        case PauPauAmarillo:
            r = 180;
            g = 180;
            b = 0;
            characterEmmiter = ParticleSystemQuad::create("particles/particle_texture_paupau_yellow.plist");
            characterEmmiter->setScale(0.85);
            break;
        case PauPauRojo:
            r = 255;
            g = 51;
            b = 51;
            break;
        case PauPauVerde:
            r = 0;
            g = 204;
            b = 102;
            characterEmmiter = ParticleSystemQuad::create("particles/particle_texture_paupau_green.plist");
            characterEmmiter->setScale(0.85);
            break;
        case PauPauNaranja:
            CCLOG("PAU PAU NARANJA");
            characterEmmiter = ParticleSystemQuad::create("particles/particle_texture_paupau_orange.plist");
            characterEmmiter->setScale(0.45);
            break;
        case PauPauMorado:
            CCLOG("PAU PAU MORADO");
            break;
        case PauPauRosa:
            CCLOG("PAU PAU ROSA");
            break;
        default:
            CCLOG("PAU PAU DESCONOCIDO");
            break;
    }
    
    auto blinkYellowAction = Sequence::create(
                                              TintTo::create(0.125, r, g, b),
                                              TintTo::create(0.125, 255, 255, 255), NULL
                                              );
    auto blinkYellowActionQuick = Sequence::create(
                                                   TintTo::create(0.0625, r, g, b),
                                                   TintTo::create(0.0625, 255, 255, 255), NULL
                                                   );
    auto pauPauActiveAction = Sequence::create(
                                               Repeat::create(blinkYellowAction, 28),
                                               Repeat::create(blinkYellowActionQuick, 24),
                                               CallFunc::create(CC_CALLBACK_0(DineroScene::removeCharacterEmmiter, this)),
                                               NULL);
    pauPauActiveAction->setTag(PauPauActiveActionType);
    character->runAction(pauPauActiveAction);
    
    
    this->addChild(characterEmmiter);
}

void DineroScene::immunityCharacter()
{
    infoLabel->setString("Invencible");
    blinkAndHide(infoLabel);
    GameManager::getInstance()->immunityActive = true;
    this->runAction(Sequence::create(
                                     DelayTime::create(10),
                                     CallFunc::create(CC_CALLBACK_0(DineroScene::endImmunityCharacter, this)),
                                     NULL));
}

void DineroScene::endImmunityCharacter()
{
    CCLOG("endImmunityCharacter");
    GameManager::getInstance()->immunityActive = false;
}

void DineroScene::createLive()
{
    if (playerLives < 3)
    {
        switch (playerLives)
        {
            case 1:
                addHeartAfterBlink(heartTwo);
                break;
            case 2:
                addHeartAfterBlink(heartThree);
                break;
            default:
                CCLOG("Unknown state of lives...");
                break;
        }
        
        playerLives += 1;
        
        infoLabel->setString("1 UP!");
        blinkAndHide(infoLabel);
    }
    else
    {
        int pointsToAdd = (playerGoalScore - playerScore) / 2;
        std::string pointsToAddStr = cocos2d::StringUtils::format("Pau Bonus\n+%i", pointsToAdd);
        infoLabel->setString(pointsToAddStr);
        blinkAndHide(infoLabel);
        updateScore(pointsToAdd);
    }
}

void DineroScene::addHeartAfterBlink(cocos2d::Node *node)
{
    auto sequence = Sequence::create(Blink::create(0.6, 8), Show::create(), NULL);
    node->runAction(sequence);
}

void DineroScene::togglePausePlayGame()
{
    if (isGamePaused)
    {
        isGamePaused = false;
        Director::getInstance()->resume();
        pauseButtonNode->setTexture(pauseTexture);
        if (GameManager::getInstance()->soundActive)
        {
//            CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
            AudioEngine::resume(GameManager::getInstance()->idBackGroundMusic);
        }
    }
    else
    {
        isGamePaused = true;
        Director::getInstance()->pause();
        pauseButtonNode->setTexture(playTexture);
        if (GameManager::getInstance()->soundActive)
        {
//            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            AudioEngine::pause(GameManager::getInstance()->idBackGroundMusic);
        }
    }
}

void DineroScene::blinkAndHide(cocos2d::Node *node)
{
    node->setOpacity(255);
    auto blinkAndHideAction = Sequence::create(
                                               Blink::create(1.5, 10),
                                               Hide::create(),
                                               NULL);
    blinkAndHideAction->setTag(BlinkAndHydeActionType);
    node->runAction(blinkAndHideAction);
}

void DineroScene::enterRussianMode()
{
    infoLabel->setString("Russian\nMode");
    blinkAndHide(infoLabel);
    GameManager::getInstance()->isRussianActive = true;
    this->runAction(Sequence::create(
                                     DelayTime::create(10),
                                     CallFunc::create(CC_CALLBACK_0(DineroScene::exitRussianMode, this)),
                                     NULL));
    AudioEngine::play2d(kAsdfSound);
}

void DineroScene::exitRussianMode()
{
    GameManager::getInstance()->isRussianActive = false;
}

void DineroScene::enterBonus()
{
    infoLabel->setString("");
    infoLabel->setVisible(true);
    makeBonusMoney();
}

void DineroScene::exitBonus()
{
    std::string pointsStr = cocos2d::StringUtils::format("Bonus\n+%i", bonusTotalPoints);
    infoLabel->setString(pointsStr);
    blinkAndHide(infoLabel);
    //Special case to update playerGoalScore
    playerGoalScore = (playerScore + bonusTotalPoints) * 1.25;
    updateScore(bonusTotalPoints);
    addActionsToScene();
    isBonusActive = false;
    bonusTotalPoints = 0;
    bonusCount = 0;
    
    if (GameManager::getInstance()->soundActive)
    {
        AudioEngine::play2d(kAprendeSound);
    }
    
    UserDefault *userDefault = UserDefault::getInstance();
    int currentCount = userDefault->getIntegerForKey(kACentenarioKey);
    currentCount++;
    userDefault->setIntegerForKey(kACentenarioKey, currentCount);
    userDefault->flush();
    
}

void DineroScene::makeBonusMoney()
{
    int x = 20;
    int y = visibleSize.height;
    
    for (int i = 0; i < 9; i++)
    {
        MoneySpriteNode *cashNode = MoneySpriteNode::createWithTextureAndPoints(centenario, 100);
        cashNode->setScale(0.5);
        cashNode->setPosition(Vec2(x, y));
        
        auto physicsBody = PhysicsBody::createBox(Size(cashNode->getContentSize().width * 0.4, cashNode->getContentSize().height * 0.4));
        physicsBody->setCollisionBitmask(moneyCategory);
        physicsBody->setContactTestBitmask(characterCategory);
        physicsBody->setGravityEnable(false);
        cashNode->setPhysicsBody(physicsBody);
        
        cashNode->runAction(Sequence::create(
                                             MoveTo::create(2, Vec2(x, -160)),
                                             RemoveSelf::create(),
                                             NULL));
        
        this->addChild(cashNode);
        x += cashNode->getBoundingBox().size.width + 10;
    }
    
    if (bonusCount < 55)
    {
        this->runAction(Sequence::create(DelayTime::create(0.125), CallFunc::create(CC_CALLBACK_0(DineroScene::makeBonusMoney, this)), NULL));
        bonusCount++;
    }
    else
    {
        //CCLOG("TERMINA EL BONUS");
        this->runAction(Sequence::create(DelayTime::create(2.25), CallFunc::create(CC_CALLBACK_0(DineroScene::exitBonus, this)), NULL));
    }
}


void DineroScene::avoidedCrown()
{
    avoidedCrowns++;
    avoidedCrownsContinues++;
    if (avoidedCrownsContinuesFinal < avoidedCrownsContinues)
    {
        avoidedCrownsContinuesFinal = avoidedCrownsContinues;
    }
}

void DineroScene::avoidedError()
{
    avoidedErrors++;
    avoidedErrorsContinues++;
    if (avoidedErrorsContinuesFinal < avoidedErrorsContinues)
    {
        avoidedErrorsContinuesFinal = avoidedErrorsContinues;
    }
}

void DineroScene::createWaves()
{
    infoLabel->setString("LEVITA");
    blinkAndHide(infoLabel);
    auto flip3d = Waves3D::create(10, Size(15,10), 15, 80);
    this->runAction(Sequence::create(flip3d, CallFunc::create(CC_CALLBACK_0(DineroScene::finishWaves, this)), NULL));
    character->runAction(Repeat::create(Sequence::create(MoveBy::create(2.5, Vec2(0, 150)), MoveBy::create(2.5, Vec2(0, -150)), NULL), 2));
}

void DineroScene::finishWaves()
{
    this->setGrid(nullptr);
}

void DineroScene::enterMiniatureMode()
{
    infoLabel->setString("Miniature\nMode");
    blinkAndHide(infoLabel);
    character->setScale(0.25);
    characterYPositionMutiplier = 0.125;
    character->setPosition(Vec2(character->getPositionX(), character->getContentSize().height * characterYPositionMutiplier));
    character->runAction(Sequence::create(Blink::create(1, 10), Show::create(), NULL));
    auto exitMiniatureAction = Sequence::create(DelayTime::create(10), CallFunc::create(CC_CALLBACK_0(DineroScene::exitMiniatureMode, this)), NULL);
    exitMiniatureAction->setTag(ExitMiniatureActionType);
    this->runAction(exitMiniatureAction);
    GameManager::getInstance()->isMiniatureActive = true;
}

void DineroScene::exitMiniatureMode()
{
    character->setScale(0.5);
    characterYPositionMutiplier = 0.25;
    character->setPosition(Vec2(character->getPositionX(), character->getContentSize().height * characterYPositionMutiplier));
    character->runAction(Sequence::create(Blink::create(1, 10), Show::create(), NULL));
    GameManager::getInstance()->isMiniatureActive = false;
}

void DineroScene::updateCountForKey(char* key)
{
    UserDefault *userDefault = UserDefault::getInstance();
    int currentCount = userDefault->getIntegerForKey(key);
    currentCount++;
    userDefault->setIntegerForKey(key, currentCount);
    userDefault->flush();
}

void DineroScene::updateAchievements()
{
    UserDefault *userDefault = UserDefault::getInstance();
    
    if (userDefault->getBoolForKey(kCrownOriginalUnlocked) == false)
    {
        if (avoidedCrowns >= 25)
        {
            userDefault->setBoolForKey(kCrownOriginalUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    if (userDefault->getBoolForKey(kCrownBronzeUnlocked) == false)
    {
        if (avoidedCrowns >= 50)
        {
            userDefault->setBoolForKey(kCrownBronzeUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    if (userDefault->getBoolForKey(kCrownSilverUnlocked) == false)
    {
        if (avoidedCrowns >= 75)
        {
            userDefault->setBoolForKey(kCrownSilverUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    if (userDefault->getBoolForKey(kCrownGoldUnlocked) == false)
    {
        if (avoidedCrownsContinuesFinal >= 40)
        {
            userDefault->setBoolForKey(kCrownGoldUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    // ERRORS
    
    if (userDefault->getBoolForKey(kErrorOriginalUnlocked) == false)
    {
        if (avoidedErrors >= 40)
        {
            userDefault->setBoolForKey(kErrorOriginalUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    if (userDefault->getBoolForKey(kErrorBronzeUnlocked) == false)
    {
        if (avoidedErrors >= 70)
        {
            userDefault->setBoolForKey(kErrorBronzeUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    if (userDefault->getBoolForKey(kErrorSilverUnlocked) == false)
    {
        if (avoidedErrors >= 100)
        {
            userDefault->setBoolForKey(kErrorSilverUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    if (userDefault->getBoolForKey(kErrorGoldUnlocked) == false)
    {
        if (avoidedErrorsContinuesFinal >= 50)
        {
            userDefault->setBoolForKey(kErrorGoldUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    
    //PAU PAUS
    
    if (userDefault->getBoolForKey(kPauPauGreenUnloked) == false)
    {
        int greenCount = userDefault->getIntegerForKey(kAPauPauGreenKey);
        if (greenCount >= 5)
        {
            userDefault->setBoolForKey(kPauPauGreenUnloked, true);
        }
    }
    
    if (userDefault->getBoolForKey(kPauPauYellowUnloked) == false)
    {
        int yellowCount = userDefault->getIntegerForKey(kAPauPauYellowKey);
        if (yellowCount >= 5)
        {
            userDefault->setBoolForKey(kPauPauYellowUnloked, true);
        }
    }
    
    if (userDefault->getBoolForKey(kPauPauRedUnloked) == false)
    {
        int redCount = userDefault->getIntegerForKey(kAPauPauRedKey);
        if (redCount >= 5)
        {
            userDefault->setBoolForKey(kPauPauRedUnloked, true);
        }
    }
    
    if (userDefault->getBoolForKey(kPauPauPurpleUnloked) == false)
    {
        int purpleCount = userDefault->getIntegerForKey(kAPauPauPurpleKey);
        if (purpleCount >= 5)
        {
            userDefault->setBoolForKey(kPauPauPurpleUnloked, true);
        }
    }
    
    if (userDefault->getBoolForKey(kPauPauPinkUnloked) == false)
    {
        int pinkCount = userDefault->getIntegerForKey(kAPauPauPinkKey);
        if (pinkCount >= 5)
        {
            userDefault->setBoolForKey(kPauPauPinkUnloked, true);
        }
    }
    
    if (userDefault->getBoolForKey(kPauPauOrangeUnloked) == false)
    {
        int orangeCount = userDefault->getIntegerForKey(kAPauPauOrangeKey);
        if (orangeCount >= 5)
        {
            userDefault->setBoolForKey(kPauPauOrangeUnloked, true);
        }
    }
    
    // CENTENARIO BONUS
    
    if (userDefault->getBoolForKey(kCentenarioUnlocked) == false)
    {
        int centenarioCount = userDefault->getIntegerForKey(kACentenarioKey);
        if (centenarioCount >= 5)
        {
            userDefault->setBoolForKey(kCentenarioUnlocked, true);
        }
    }
    
    // CHARACTER GOLD
    
    if (userDefault->getBoolForKey(kCharacterGoldUnlocked) == false)
    {
        if (playerScore >= 100000)
        {
            userDefault->setBoolForKey(kCharacterGoldUnlocked, true);
            achievementsUnloked++;
        }
    }
    
    userDefault->flush();
    
    GameManager::getInstance()->totalNewAchievementsUnloked = achievementsUnloked;
    
}

void DineroScene::removeCharacterEmmiter()
{
    characterEmmiter->removeFromParent();
    characterEmmiter = nullptr;
}


void DineroScene::enterGigantMoney()
{
    GameManager::getInstance()->isGigantMoneyActive = true;
    infoLabel->setString("DINERO\nGIGANTE");
    blinkAndHide(infoLabel);
    moneyScaleMultiplier = 1;
    this->runAction(Sequence::create(DelayTime::create(12),
                                     CallFunc::create(CC_CALLBACK_0(DineroScene::exitGigantMoney, this)),
                                     NULL));
}

void DineroScene::exitGigantMoney()
{
    GameManager::getInstance()->isGigantMoneyActive = false;
    moneyScaleMultiplier = 0.5;
}
