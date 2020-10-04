//
//  RapScene.h
//  McDinero
//
//  Created by Smile0210 on 23/02/15.
//
//

#ifndef __McDinero__RapScene__
#define __McDinero__RapScene__

#include "cocos2d.h"
#include "DineroScene.h"

USING_NS_CC;

class RapScene : public Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    //static cocos2d::Scene* createScene();
    static cocos2d::Scene* createScene(CharacterEquipment equipment);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(RapScene);
    
    void setCharacterEquipment(CharacterEquipment equipment)
    {
        currentEquipment = equipment;
    }
    
    void initVariables();
    void loadTextures();
    void update(float dt);
    
    virtual void onEnter();
    
private:
    Size visibleSize;
    
    Texture2D *textureProgressBar;
    Texture2D *textureProgressBarEmpty;
    Texture2D *textureBackground;
    Texture2D *textureGlasses;
    Texture2D *textureCloak;
    Texture2D *textureCrown;
    Texture2D *textureBlingbling;
    
    CharacterEquipment currentEquipment;
    bool backToDineroScene;
    bool sceneComplete;
    
    MenuItem *button;
    Sprite *progressBarEmpty;
    Sprite *progressBar;
    Label *rapLabel;
    
    int playerScore;
    int playerGoalScore;
    int playerDiscountPoints;
    
    int rapSoundId;
    bool canPlayRapSound;
    
    int timeToComplete;
    Label *timeToCompleteLabel;
    
    Label *dineroLabel1;
    Label *dineroLabel2;
    Label *dineroLabel3;
    Label *dineroLabel4;
    
    Label *aprendeLabel1;
    Label *aprendeLabel2;
    
    Sprite *character;
    Sprite *backgroundImage;
    
    ProgressTimer *barProgressTimer;
    
    RepeatForever *animate;
    
    bool isCharacterAnimated;
    
    void updateProgressBar();
    void createProgressBar();
    void rap();
    void createCheck();
    void checkFucn();
    void discountFunc(float dt);
    void goToDineroScene(bool complete);
    void countDown(float dt);
    void createRapLabels();
    void animateRapLabels();
    void animationBeforeChangeScene();
    void createAprendeLabels();
    void animateAprendeLabels();
    void goToSceneAfterAnimation();
    
};

#endif /* defined(__McDinero__RapScene__) */











