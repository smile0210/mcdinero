//
//  MenuScene.h
//  McDinero
//
//  Created by Smile0210 on 19/02/15.
//
//

#ifndef __McDinero__MenuScene__
#define __McDinero__MenuScene__

#include "cocos2d.h"

USING_NS_CC;

class MenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    
    virtual bool init();
    void initLabels();
    
    CREATE_FUNC(MenuScene);
    
private:
    Size visibleSize;
    Label *titleLabel;
    Label *titleLabel2;
    
    Label *highScoreLabel;
    
    Label *instructionsLabel1;
    Label *instructionsLabel2;
    Label *instructionsLabel3;
    Label *instructionsLabel4;
    Label *instructionsLabel5;
    
    Label *soundLabel;
    Label *achievementsLabel;
    
    Texture2D *crownTexture;
    Texture2D *errorTexture;
    Texture2D *pauPauTexture;
    
    Sprite *crownSprite;
    Sprite *errorSprite;
    Sprite *pauPauSprite;
    
    ParticleSystemQuad *crownEmmiter;
    
    void goToMainScene(Ref* sender);
    void toggleSound();
    void goToGoalsScene();
    void startBackgroundMusic();
};

#endif /* defined(__McDinero__MenuScene__) */
