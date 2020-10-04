//
//  GoalsScene.cpp
//  McDinero
//
//  Created by Smile0210 on 10/03/15.
//
//

#include "GoalsScene.h"
#include "GameManager.h"
#include "CustomDirector.h"
#include "McDineroTypes.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdmobHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../proj.ios_mac/ios/IOSHelper.h"
#endif

Scene* GoalsScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GoalsScene::create();
    
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GoalsScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    initialXPosition = visibleSize.width / 8;
    deltaXPosition = visibleSize.width / 4;
    
    initialYPosition = ((visibleSize.height - 200) / 8);
    deltaYPosition = (visibleSize.height - 200) / 4;
    
    for (int i = 0; i < 4; i++)
    {
        positionsX[i] = initialXPosition + (i * deltaXPosition);
        positionsY[i] = initialYPosition + (i * deltaYPosition);
    }
    
    initVariables();
    loadUnlocked();
    loadGoals();
    
    achievementsLabel = Label::createWithTTF("Trofeos", "fonts/JoystixMonospace.ttf", 25);
    achievementsLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 30));
    this->addChild(achievementsLabel, 10);
    
    achievementsInstructionsLabel = Label::createWithTTF("Selecciona un trofeo \npara saber cómo\nobtenerlo", "fonts/JoystixMonospace.ttf", 15);
    achievementsInstructionsLabel->setAlignment(TextHAlignment::CENTER);
    achievementsInstructionsLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 120));
    this->addChild(achievementsInstructionsLabel, 40);
    
    backButton = MenuItemImage::create("BackWhite.png", "BackWhite.png", CC_CALLBACK_0(GoalsScene::goToPreviousScene, this));
    backButton->setScale(0.45);
    
    auto backMenu = Menu::create(backButton, NULL);
    backMenu->alignItemsHorizontally();
    
    backMenu->setAnchorPoint(Vec2(0.5, 0));
    backMenu->setPosition(Vec2(30, visibleSize.height - 30));
    this->addChild(backMenu);
    
    return true;
}

void GoalsScene::goToMainScene(cocos2d::Ref *sender)
{
    CustomDirector *customDirector = (CustomDirector *)Director::getInstance();
    customDirector->popSceneWithTransition<TransitionFade>(1.0);
}

void GoalsScene::goToGameOverScene(Ref* sender)
{
    CustomDirector *customDirector = (CustomDirector *)Director::getInstance();
    customDirector->popSceneWithTransition<TransitionFade>(1.0);
}

void GoalsScene::loadGoals()
{
    //CROWNS
    
    int opacityForUnlocked = 120;
    
    if (crownOriginalUnlocked)
    {
        crownBronze = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrown.png"));
        crownBronzeParticles = ParticleSystemQuad::create("particles/particle_texture_crown.plist");
        crownBronzeParticles->setScale(0.4);
        crownBronzeParticles->setPosition(Vec2(positionsX[0], positionsY[3]));
        this->addChild(crownBronzeParticles);
    }
    else
    {
        crownBronze = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrown.png"));
        crownBronze->setOpacity(opacityForUnlocked);
    }
    
    auto crownBronzeMenuItem = MenuItemSprite::create(crownBronze, crownBronze, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    crownBronzeMenuItem->setTag(CrownBronzeAchievementType);
    crownBronzeMenuItem->setScale(0.33);
    
    if (crownBronzeUnlocked)
    {
        crownSilver = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrownBronze2.png"));
        crownSilverParticles = ParticleSystemQuad::create("particles/particle_texture_crown_bronze.plist");
        crownSilverParticles->setScale(0.4);
        crownSilverParticles->setPosition(Vec2(positionsX[1], positionsY[3]));
        this->addChild(crownSilverParticles);
    }
    else
    {
        crownSilver = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrownBronze2.png"));
        crownSilver->setOpacity(opacityForUnlocked);
    }
    
    auto crownSilverMenuItem = MenuItemSprite::create(crownSilver, crownSilver, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    crownSilverMenuItem->setTag(CrownSilverAchievementType);
    crownSilverMenuItem->setScale(0.33);
    
    if (crownSilverUnlocked)
    {
        crownGold = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrownSilver2.png"));
        crownGoldParticles = ParticleSystemQuad::create("particles/particle_texture_crown_silver.plist");
        crownGoldParticles->setScale(0.4);
        crownGoldParticles->setPosition(Vec2(positionsX[2], positionsY[3]));
        this->addChild(crownGoldParticles);
    }
    else
    {
        crownGold = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrownSilver2.png"));
        crownGold->setOpacity(opacityForUnlocked);
    }
    
    
    auto crownGoldMenuItem = MenuItemSprite::create(crownGold, crownGold, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    crownGoldMenuItem->setTag(CrownGoldAchievementType);
    crownGoldMenuItem->setScale(0.33);
    
    if (crownGoldUnlocked)
    {
        crownDiamond = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrownGold.png"));
        crownDiamondParticles = ParticleSystemQuad::create("particles/particle_texture_crown_gold.plist");
        crownDiamondParticles->setScale(0.4);
        crownDiamondParticles->setPosition(Vec2(positionsX[3], positionsY[3]));
        this->addChild(crownDiamondParticles);
    }
    else
    {
        crownDiamond = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("EvilCrownGold.png"));
        crownDiamond->setOpacity(opacityForUnlocked);
    }
    
    auto crownDiamondMenuItem = MenuItemSprite::create(crownDiamond, crownDiamond, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    crownDiamondMenuItem->setTag(CrownDiamondAchievementType);
    crownDiamondMenuItem->setScale(0.33);
    
    auto menuCrowns = Menu::create(crownBronzeMenuItem, crownSilverMenuItem, crownGoldMenuItem, crownDiamondMenuItem, NULL);
    menuCrowns->alignItemsHorizontallyWithPadding(33);
    menuCrowns->setPosition(Vec2(visibleSize.width / 2, positionsY[3]));
    this->addChild(menuCrowns, 10);
    
    //ERROR
    
    if (errorOriginalUnlocked)
    {
        errorBronze = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorChar.png"));
    }
    else
    {
        errorBronze = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorChar.png"));
        errorBronze->setOpacity(opacityForUnlocked);
    }
    
    
    auto errorBronzeMenuItem = MenuItemSprite::create(errorBronze, errorBronze, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    errorBronzeMenuItem->setTag(ErrorBronzeAchievementType);
    errorBronzeMenuItem->setScale(0.65);
    
    if (errorOriginalUnlocked)
    {
        errorBronzeMenuItem->runAction(RepeatForever::create(Sequence::create(
                                                                              RotateBy::create(0.05, -6.0),
                                                                              RotateBy::create(0.05, 0.0),
                                                                              RotateBy::create(0.05, 6.0),
                                                                              RotateBy::create(0.05, 0.0), NULL)));
        
        errorBronzeMenuItem->runAction(RepeatForever::create(Sequence::create(
                                                                              DelayTime::create(1),
                                                                              Blink::create(1, 8),
                                                                              Show::create(),
                                                                              NULL)));
    }
    
    if (errorBronzeUnlocked)
    {
        errorSilver = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorCharBronze.png"));
    }
    else
    {
        errorSilver = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorCharBronze.png"));
        errorSilver->setOpacity(opacityForUnlocked);
    }

    
    auto errorSilverMenuItem = MenuItemSprite::create(errorSilver, errorSilver, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    errorSilverMenuItem->setTag(ErrorSilverAchievementType);
    errorSilverMenuItem->setScale(0.65);
    
    if (errorBronzeUnlocked)
    {
        errorSilverMenuItem->runAction(RepeatForever::create(Sequence::create(
                                                                              RotateBy::create(0.05, -6.0),
                                                                              RotateBy::create(0.05, 0.0),
                                                                              RotateBy::create(0.05, 6.0),
                                                                              RotateBy::create(0.05, 0.0), NULL)));
        
        errorSilverParticles = ParticleSystemQuad::create("particles/particle_texture_general_bronze.plist");
        errorSilverParticles->setScale(0.4);
        errorSilverParticles->setPosition(Vec2(positionsX[1], positionsY[2]));
        this->addChild(errorSilverParticles);
    }
    
    
    if (errorSilverUnlocked)
    {
        errorGold = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorCharSilver.png"));
    }
    else
    {
        errorGold = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorCharSilver.png"));
        errorGold->setOpacity(opacityForUnlocked);
    }
    
    
    auto errorGoldMenuItem = MenuItemSprite::create(errorGold, errorGold, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    errorGoldMenuItem->setTag(ErrorGoldAchievementType);
    errorGoldMenuItem->setScale(0.65);
    
    if (errorSilverUnlocked)
    {
        errorGoldMenuItem->runAction(RepeatForever::create(Sequence::create(
                                                                            RotateBy::create(0.05, -6.0),
                                                                            RotateBy::create(0.05, 0.0),
                                                                            RotateBy::create(0.05, 6.0),
                                                                            RotateBy::create(0.05, 0.0), NULL)));
        
        errorGoldParticles = ParticleSystemQuad::create("particles/particle_texture_general_silver.plist");
        errorGoldParticles->setScale(0.4);
        errorGoldParticles->setPosition(Vec2(positionsX[2], positionsY[2]));
        this->addChild(errorGoldParticles);
    }
    
    if (errorGoldUnlocked)
    {
        errorDiamond = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorCharGold.png"));
    }
    else
    {
        errorDiamond = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("ErrorCharGold.png"));
        errorDiamond->setOpacity(opacityForUnlocked);
    }
    
    
    auto errorDiamondMenuItem = MenuItemSprite::create(errorDiamond, errorDiamond, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    errorDiamondMenuItem->setTag(ErrorDiamondAchievementType);
    errorDiamondMenuItem->setScale(0.65);
    
    if (errorGoldUnlocked)
    {
        errorDiamondMenuItem->runAction(RepeatForever::create(Sequence::create(
                                                                               RotateBy::create(0.05, -6.0),
                                                                               RotateBy::create(0.05, 0.0),
                                                                               RotateBy::create(0.05, 6.0),
                                                                               RotateBy::create(0.05, 0.0), NULL)));
        
        errorDiamondParticles = ParticleSystemQuad::create("particles/particle_texture_general_gold.plist");
        errorDiamondParticles->setScale(0.4);
        errorDiamondParticles->setPosition(Vec2(positionsX[3], positionsY[2]));
        this->addChild(errorDiamondParticles);
    }
    
    auto menuErrors = Menu::create(errorBronzeMenuItem, errorSilverMenuItem, errorGoldMenuItem, errorDiamondMenuItem, NULL);
    menuErrors->alignItemsHorizontallyWithPadding(40);
    menuErrors->setPosition(Vec2(visibleSize.width / 2, positionsY[2]));
    this->addChild(menuErrors, -10);
    
    //PAU PAUS
    
    if (pauPauGreenUnlocked)
    {
        pauPauGreen = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_verde.png"));
        pauPauGreenParticles = ParticleSystemQuad::create("particles/particle_texture_paupau_green.plist");
        pauPauGreenParticles->setScale(0.4);
        pauPauGreenParticles->setPosition(Vec2(positionsX[0], positionsY[1]));
        this->addChild(pauPauGreenParticles);
    }
    else
    {
        pauPauGreen = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_verde.png"));
        pauPauGreen->setOpacity(opacityForUnlocked);
    }
    
    auto pauPauGreenMenuItem = MenuItemSprite::create(pauPauGreen, pauPauGreen, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    pauPauGreenMenuItem->setTag(PauPauGreenAchievementType);
    pauPauGreenMenuItem->setScale(0.45);
    
    
    if (pauPauRedUnlocked)
    {
        pauPauRed = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_rojo.png"));
        pauPauRedParticles = ParticleSystemQuad::create("particles/particle_texture_paupau_red.plist");
        pauPauRedParticles->setScale(0.4);
        pauPauRedParticles->setPosition(Vec2(positionsX[1], positionsY[1]));
        this->addChild(pauPauRedParticles);
    }
    else
    {
        pauPauRed = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_rojo.png"));
        pauPauRed->setOpacity(opacityForUnlocked);
    }
    
    auto pauPauRedMenuItem = MenuItemSprite::create(pauPauRed, pauPauRed, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    pauPauRedMenuItem->setTag(PauPauRedAchievementType);
    pauPauRedMenuItem->setScale(0.45);
    
    if (pauPauYellowUnlocked)
    {
        pauPauYellow = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_amarillo.png"));
        pauPauYellowParticles = ParticleSystemQuad::create("particles/particle_texture_paupau_yellow.plist");
        pauPauYellowParticles->setScale(0.4);
        pauPauYellowParticles->setPosition(Vec2(positionsX[2], positionsY[1]));
        this->addChild(pauPauYellowParticles);
    }
    else
    {
        pauPauYellow = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_amarillo.png"));
        pauPauYellow->setOpacity(opacityForUnlocked);
    }
    
    
    auto pauPauYellowMenuItem = MenuItemSprite::create(pauPauYellow, pauPauYellow, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    pauPauYellowMenuItem->setTag(PauPauYellowAchievementType);
    pauPauYellowMenuItem->setScale(0.45);
    
    if (pauPauPurpleUnlocked)
    {
        pauPauPurple = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_morado.png"));
        pauPauPurpleParticles = ParticleSystemQuad::create("particles/particle_texture_paupau_purple.plist");
        pauPauPurpleParticles->setScale(0.4);
        pauPauPurpleParticles->setPosition(Vec2(positionsX[3], positionsY[1]));
        this->addChild(pauPauPurpleParticles);
    }
    else
    {
        pauPauPurple = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_morado.png"));
        pauPauPurple->setOpacity(opacityForUnlocked);
    }
    
    auto pauPauPurpleMenuItem = MenuItemSprite::create(pauPauPurple, pauPauPurple, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    pauPauPurpleMenuItem->setTag(PauPauPurpleAchievementType);
    pauPauPurpleMenuItem->setScale(0.45);
    
    auto menuPauPaus = Menu::create(pauPauGreenMenuItem, pauPauRedMenuItem, pauPauYellowMenuItem, pauPauPurpleMenuItem, NULL);
    menuPauPaus->alignItemsHorizontallyWithPadding(60);
    menuPauPaus->setPosition(Vec2(visibleSize.width / 2, positionsY[1]));
    this->addChild(menuPauPaus, 10);
    
    //MORE
    
    if (pauPauPinkUnlocked)
    {
        pauPauPink = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_rosa.png"));
        pauPauPinkParticles = ParticleSystemQuad::create("particles/particle_texture_paupau_pink.plist");
        pauPauPinkParticles->setScale(0.4);
        pauPauPinkParticles->setPosition(Vec2(positionsX[0], positionsY[0]));
        this->addChild(pauPauPinkParticles);
    }
    else
    {
        pauPauPink = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_rosa.png"));
        pauPauPink->setOpacity(opacityForUnlocked);
    }
    
    auto pauPauPinkMenuItem = MenuItemSprite::create(pauPauPink, pauPauPink, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    pauPauPinkMenuItem->setTag(PauPauPinkAchievementType);
    pauPauPinkMenuItem->setScale(0.45);
    
    if (pauPauOrangeUnlocked)
    {
        pauPauOrange = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_naranja.png"));
        pauPauOrangeParticles = ParticleSystemQuad::create("particles/particle_texture_paupau_orange.plist");
        pauPauOrangeParticles->setScale(0.4);
        pauPauOrangeParticles->setPosition(Vec2(positionsX[1], positionsY[0]));
        this->addChild(pauPauOrangeParticles);
    }
    else
    {
        pauPauOrange = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("pau_naranja.png"));
        pauPauOrange->setOpacity(opacityForUnlocked);
    }
    
    auto pauPauOrangeMenuItem = MenuItemSprite::create(pauPauOrange, pauPauOrange, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    pauPauOrangeMenuItem->setTag(PauPauOrangeAchievementType);
    pauPauOrangeMenuItem->setScale(0.45);
    
    if (centenarioUnlocked)
    {
        centenarioBonus = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("centenario.png"));
        centenarioParticles = ParticleSystemQuad::create("particles/particle_texture_general_gold.plist");
        centenarioParticles->setScale(0.4);
        centenarioParticles->setPosition(Vec2(positionsX[2], positionsY[0]));
        this->addChild(centenarioParticles);
    }
    else
    {
        centenarioBonus = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("centenario.png"));
        centenarioBonus->setOpacity(opacityForUnlocked);
    }
    
    auto centenarioBonusMenuItem = MenuItemSprite::create(centenarioBonus, centenarioBonus, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    centenarioBonusMenuItem->setTag(CentenarioAchievementType);
    centenarioBonusMenuItem->setScale(0.85);
    
    if (characterGoldUnlocked)
    {
        characterGold = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("CharacterRightGold2.png"));
        characterParticles = ParticleSystemQuad::create("particles/particle_texture_general_gold.plist");
        characterParticles->setScale(0.4);
        characterParticles->setPosition(Vec2(positionsX[3], positionsY[0]));
        this->addChild(characterParticles);
    }
    else
    {
        characterGold = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("CharacterRightGold2.png"));
        characterGold->setOpacity(opacityForUnlocked);
    }
    
    auto characterGoldMenuItem = MenuItemSprite::create(characterGold, characterGold, CC_CALLBACK_1(GoalsScene::itemPressed, this));
    characterGoldMenuItem->setTag(CharacterAchievementType);
    characterGoldMenuItem->setScale(0.35);
    
    auto menuPauPaus2 = Menu::create(pauPauPinkMenuItem, pauPauOrangeMenuItem, NULL);
    menuPauPaus2->alignItemsHorizontallyWithPadding(60);
    menuPauPaus2->setPosition(Vec2(visibleSize.width / 4, positionsY[0]));
    this->addChild(menuPauPaus2, 10);
    
    auto menuFinal = Menu::create(centenarioBonusMenuItem, characterGoldMenuItem, NULL);
    menuFinal->alignItemsHorizontallyWithPadding(28);
    menuFinal->setPosition(Vec2((visibleSize.width / 4) * 3, positionsY[0]));
    this->addChild(menuFinal, -10);
    
}

void GoalsScene::itemPressed(Ref *sender)
{
    Node *senderNode = (Node *)sender;
    switch (senderNode->getTag())
    {
        case CrownBronzeAchievementType:
            if (crownOriginalUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 25 CORONAS\nDURANTE UN JUEGO");
            }
            break;
        case CrownSilverAchievementType:
            if (crownBronzeUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 50 CORONAS\nDURANTE UN JUEGO");
            }
            break;
        case CrownGoldAchievementType:
            if (crownSilverUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 75 CORONAS\nDURANTE UN JUEGO");
            }
            break;
        case CrownDiamondAchievementType:
            if (crownGoldUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 40 CORONAS\nDE FORMA CONTINUA\nDURANTE UN JUEGO");
            }
            break;
        case ErrorBronzeAchievementType:
            if (errorOriginalUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 40 ERRORES\nDURANTE UN JUEGO");
            }
            break;
        case ErrorSilverAchievementType:
            if (errorBronzeUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 70 ERRORES\nDURANTE UN JUEGO");
            }
            break;
        case ErrorGoldAchievementType:
            if (errorSilverUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 100 ERRORES\nDURANTE UN JUEGO");
            }
            break;
        case ErrorDiamondAchievementType:
            if (errorGoldUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("ESQUIVA 50 ERRORES\nDE FORMA CONTINUA\nDURANTE UN JUEGO");
            }
            break;
        case PauPauGreenAchievementType:
            if (pauPauGreenUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("TOMA 5 VECES LA\nBEBIDA VERDE");
            }
            break;
        case PauPauRedAchievementType:
            if (pauPauRedUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("TOMA 5 VECES LA\nBEBIDA ROJA");
            }
            break;
        case PauPauYellowAchievementType:
            if (pauPauYellowUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("TOMA 5 VECES LA\nBEBIDA AMARILLA");
            }
            break;
        case PauPauPurpleAchievementType:
            if (pauPauPurpleUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("TOMA 5 VECES LA\nBEBIDA MORADA");
            }
            break;
        case PauPauPinkAchievementType:
            if (pauPauPinkUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("TOMA 5 VECES LA\nBEBIDA ROSA");
            }
            break;
        case PauPauOrangeAchievementType:
            if (pauPauOrangeUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("TOMA 5 VECES LA\nBEBIDA NARANJA");
            }
            break;
        case CentenarioAchievementType:
            if (centenarioUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("GANA 5 VECES LA\nLLUVIA DE DINERO");
            }
            break;
        case CharacterAchievementType:
            if (characterGoldUnlocked)
            {
                achievementsInstructionsLabel->setString("LOGRO DESBLOQUEADO");
            }
            else
            {
                achievementsInstructionsLabel->setString("CONSIGUE $100,000 EN\nUNA PARTIDA");
            }
            break;
        default:
            break;
    }
    
    achievementsInstructionsLabel->setOpacity(0);
    achievementsInstructionsLabel->runAction(FadeIn::create(0.2));
    
}

void GoalsScene::initVariables()
{
    crownOriginalUnlocked = false;
    crownBronzeUnlocked = false;
    crownSilverUnlocked = false;
    crownGoldUnlocked = false;
    
    errorOriginalUnlocked = false;
    errorBronzeUnlocked = false;
    errorSilverUnlocked = false;
    errorGoldUnlocked = false;
    
    pauPauGreenUnlocked = false;
    pauPauRedUnlocked = false;
    pauPauYellowUnlocked = false;
    pauPauPurpleUnlocked = false;
    pauPauPinkUnlocked = false;
    pauPauOrangeUnlocked = false;
    
    centenarioUnlocked = false;
    characterGoldUnlocked = false;
}

void GoalsScene::loadUnlocked()
{
    UserDefault *userDefault = UserDefault::getInstance();
    
    crownOriginalUnlocked = userDefault->getBoolForKey(kCrownOriginalUnlocked);
    crownBronzeUnlocked = userDefault->getBoolForKey(kCrownBronzeUnlocked);
    crownSilverUnlocked = userDefault->getBoolForKey(kCrownSilverUnlocked);
    crownGoldUnlocked = userDefault->getBoolForKey(kCrownGoldUnlocked);
    
    errorOriginalUnlocked = userDefault->getBoolForKey(kErrorOriginalUnlocked);
    errorBronzeUnlocked = userDefault->getBoolForKey(kErrorBronzeUnlocked);
    errorSilverUnlocked = userDefault->getBoolForKey(kErrorSilverUnlocked);
    errorGoldUnlocked = userDefault->getBoolForKey(kErrorGoldUnlocked);
    
    pauPauGreenUnlocked = userDefault->getBoolForKey(kPauPauGreenUnloked);
    pauPauYellowUnlocked = userDefault->getBoolForKey(kPauPauYellowUnloked);
    pauPauRedUnlocked = userDefault->getBoolForKey(kPauPauRedUnloked);
    pauPauPurpleUnlocked = userDefault->getBoolForKey(kPauPauPurpleUnloked);
    pauPauPinkUnlocked = userDefault->getBoolForKey(kPauPauPinkUnloked);
    pauPauOrangeUnlocked = userDefault->getBoolForKey(kPauPauOrangeUnloked);
    
    centenarioUnlocked = userDefault->getBoolForKey(kCentenarioUnlocked);
    characterGoldUnlocked = userDefault->getBoolForKey(kCharacterGoldUnlocked);
    
}

void GoalsScene::goToPreviousScene()
{
    CustomDirector *customDirector = (CustomDirector *)Director::getInstance();
    customDirector->popSceneWithTransition<TransitionFade>(1.0);
}







