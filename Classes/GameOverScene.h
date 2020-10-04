//
//  GameOverScene.h
//  McDinero
//
//  Created by Smile0210 on 23/02/15.
//
//

#ifndef __McDinero__GameOverScene__
#define __McDinero__GameOverScene__

#include "cocos2d.h"
#include "DineroScene.h"
#include "MenuScene.h"

USING_NS_CC;

class GameOverScene : public Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int points, CharacterEquipment equipment);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);
    
    virtual void onEnter();
    
    void setTotalScore(int score)
    {
        totalScore = score;
    }
    void setMaxCharacterEquipment(CharacterEquipment equipment)
    {
        maxCharacterEquipment = equipment;
    }
    
private:
    Size visibleSize;
    int totalScore;
    Texture2D *backTexture;
    Texture2D *facebookTexture;
    Texture2D *twitterTexture;
    Texture2D *universalShareTexture;
    
    MenuItemImage *homeButton;
    MenuItemImage *facebookButton;
    MenuItemImage *twitterButton;
    MenuItemImage *universalShareButton;
    MenuItemImage *continueButton;
    
    Label *continueLabel;
    Label *continueLabel2;
    Label *scoreLabel;
    Label *resultScoreLabel;
    
    Label *unlockedLabel;
    
    CharacterEquipment maxCharacterEquipment = None;
    
    Sprite *character;
    bool hasNewHighScore;
    bool contentCreated;
    void initVariables();
    void loadTextures();
    
    void goToMainScene(Ref *sender);
    void goToMenuScene(Ref *sender);
    void facebookShare(Ref *sender);
    void twitterShare(Ref *sender);
    void universalShare(Ref *sender);
    void createCharacter();
    void stopErrorSound();
    
    int noErrorSoundId;
    
    void goToGoalsScene();
    
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

#endif /* defined(__McDinero__GameOverScene__) */
