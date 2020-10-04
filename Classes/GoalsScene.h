//
//  GoalsScene.h
//  McDinero
//
//  Created by Smile0210 on 10/03/15.
//
//

#ifndef __McDinero__GoalsScene__
#define __McDinero__GoalsScene__

#include "cocos2d.h"

USING_NS_CC;

class GoalsScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    
    virtual bool init();
    void initLabels();
    
    CREATE_FUNC(GoalsScene);
    
protected:
    NodeGrid* _gridNodeTarget;
    
private:
    Size visibleSize;
    void goToMainScene(Ref* sender);
    void goToGameOverScene(Ref* sender);
    
    Sprite *crownBronze;
    ParticleSystemQuad *crownBronzeParticles;
    Sprite *crownSilver;
    ParticleSystemQuad *crownSilverParticles;
    Sprite *crownGold;
    ParticleSystemQuad *crownGoldParticles;
    Sprite *crownDiamond;
    ParticleSystemQuad *crownDiamondParticles;
    
    Sprite *errorBronze;
    Sprite *errorSilver;
    ParticleSystemQuad *errorSilverParticles;
    Sprite *errorGold;
    ParticleSystemQuad *errorGoldParticles;
    Sprite *errorDiamond;
    ParticleSystemQuad *errorDiamondParticles;
    
    Sprite *pauPauRed;
    ParticleSystemQuad *pauPauRedParticles;
    Sprite *pauPauGreen;
    ParticleSystemQuad *pauPauGreenParticles;
    Sprite *pauPauYellow;
    ParticleSystemQuad *pauPauYellowParticles;
    Sprite *pauPauPurple;
    ParticleSystemQuad *pauPauPurpleParticles;
    
    Sprite *pauPauOrange;
    ParticleSystemQuad *pauPauOrangeParticles;
    Sprite *pauPauPink;
    ParticleSystemQuad *pauPauPinkParticles;
    Sprite *centenarioBonus;
    ParticleSystemQuad *centenarioParticles;
    Sprite *characterGold;
    ParticleSystemQuad *characterParticles;
    
    Label *achievementsLabel;
    Label *achievementsInstructionsLabel;
    
    void loadGoals();
    void itemPressed(Ref *sender);
    
    int initialXPosition;
    int deltaXPosition;
    
    int initialYPosition;
    int deltaYPosition;
    
    int positionsX[4];
    int positionsY[4];
    
    void initVariables();
    void loadUnlocked();
    
    bool crownOriginalUnlocked;
    bool crownBronzeUnlocked;
    bool crownSilverUnlocked;
    bool crownGoldUnlocked;
    
    bool errorOriginalUnlocked;
    bool errorBronzeUnlocked;
    bool errorSilverUnlocked;
    bool errorGoldUnlocked;
    
    bool pauPauGreenUnlocked;
    bool pauPauRedUnlocked;
    bool pauPauYellowUnlocked;
    bool pauPauPurpleUnlocked;
    bool pauPauPinkUnlocked;
    bool pauPauOrangeUnlocked;
    
    bool centenarioUnlocked;
    bool characterGoldUnlocked;
    
    Texture2D *backTexture;
    MenuItemImage *backButton;
    
    void goToPreviousScene();
    
};


#endif /* defined(__McDinero__GoalsScene__) */
