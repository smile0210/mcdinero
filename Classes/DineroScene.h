//
//  DineroScene.h
//  McDinero
//
//  Created by Smile0210 on 20/02/15.
//
//

#ifndef __McDinero__DineroScene__
#define __McDinero__DineroScene__

#include "cocos2d.h"
#include "McDineroTypes.h"

USING_NS_CC;


class DineroScene : public NodeGrid
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(DineroScene);
    
    void update(float dt);
    
    virtual void onEnter();
    
private:
    
    bool onTouchBegan(Touch* touch, Event  *event);
    float movementDirection;
    Size visibleSize;
    Sprite *character;
    ParticleSystemQuad *characterEmmiter;
    Sprite *heartOne;
    Sprite *heartTwo;
    Sprite *heartThree;
    
    Sprite *progressBar;
    Sprite *progressBarEmpty;
    ProgressTimer *barProgressTimer;
    
    Label *playerScoreLabel;
    Label *scoreInfoLabel;
    Label *infoLabel;
    
    Texture2D *characterLeftTexture;
    Texture2D *characterLeftGlassesTexture;
    Texture2D *characterLeftCloakTexture;
    Texture2D *characterLeftCrownTexture;
    
    Texture2D *characterRightTexture;
    Texture2D *characterRightGlassesTexture;
    Texture2D *characterRightCloakTexture;
    Texture2D *characterRightCrownTexture;
    
    Texture2D *moneyTexture01;
    Texture2D *moneyTexture02;
    Texture2D *moneyTexture05;
    Texture2D *moneyTexture10;
    Texture2D *moneyTexture20;
    Texture2D *moneyTexture50;
    Texture2D *moneyTexture100;
    Texture2D *moneyTexture200;
    Texture2D *moneyTexture500;
    Texture2D *centenario;
    
    Texture2D *errorTexture;
    Texture2D *crownTexture;
    
    Texture2D *pauPauYellowTexture;
    Texture2D *pauPauRedTexture;
    Texture2D *pauPauGreenTexture;
    Texture2D *pauPauOrangeTexture;
    Texture2D *pauPauPurpleTexture;
    Texture2D *pauPauPinkTexture;
    
    Texture2D *heartTexture;
    Texture2D *progressTexture;
    Texture2D *progressTextureEmpty;
    Texture2D *pauseTexture;
    Texture2D *playTexture;
    
    Sprite *pauseButtonNode;
    
    double playerMovementDuration;
    CharacterEquipment characterEquipment;
    CharacterEquipment maxEquipment;
    int maxDownTime;
    int playerScore;
    int playerLives;
    int playerGoalScore;
    int maxEquiomentCount;
    int countMoneyCath;
    int avoidedCrowns;
    int avoidedCrownsContinuesFinal;
    int avoidedCrownsContinues;
    int avoidedErrors;
    int avoidedErrorsContinuesFinal;
    int avoidedErrorsContinues;
    int achievementsUnloked;
    
    double waitDurationCash;
    double waitDurationError;
    double waitDurationCrown;
    
    void initVariables();
    void loadTextures();
    void updateCharacterForPosition(CharacterDirection direction);
    void removeActionsToScene();
    void addActionsToScene();
    void addCash();
    void addError();
    void addCrown();
    void addPauPau();
    void createLives();
    void createProgressBar();
    void updateProgressBar();
    void deleteNodesFromScene();
    void goToRapScene();
    void goToGameOverScene();
    void updateMetrics();
    void blinkCharacter();
    void updateLives();
    void removeNodeAfterBlink(Node *node);
    void updateScore(int points);
    void animatScoreInfoLabel(int points);
    bool onContactBegin(PhysicsContact &contact);
    void beginMoveCharacter();
    void onEnterFinish();
    void blinkCharacterWithPauPau(PauPauType pauPauType);
    void immunityCharacter();
    void endImmunityCharacter();
    void createLive();
    void addHeartAfterBlink(Node *node);
    void togglePausePlayGame();
    void blinkAndHide(Node *node);
    void enterRussianMode();
    void exitRussianMode();
    void enterBonus();
    void exitBonus();
    void makeBonusMoney();
    
    void avoidedCrown();
    void avoidedError();
    void updateCountForKey(char* key);
    
    bool canPlayDineroSound;
    bool isGamePaused;
    bool isBonusActive;
    
    int bonusCount;
    int bonusTotalPoints;
    
    void resetDineroSoundFlag();
    
    float randomTime();
    double randomDelayTime(double x);
    double randomDelayTime(double x, double y);
    void createWaves();
    void finishWaves();
    
    void enterMiniatureMode();
    void exitMiniatureMode();
    float characterYPositionMutiplier;
    
    void updateAchievements();
    void removeCharacterEmmiter();
    
    void enterGigantMoney();
    void exitGigantMoney();
    
    float moneyScaleMultiplier;
    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    template<typename T>
//    std::string to_string(const T& t)
//    {
//        std::ostringstream os;
//        os << t;
//        return os.str();
//    }
//#endif

    
};

#endif /* defined(__McDinero__DineroScene__) */
