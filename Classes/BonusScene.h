//
//  BonusScene.h
//  McDinero
//
//  Created by Smile0210 on 05/03/15.
//
//

#ifndef __McDinero__BonusScene__
#define __McDinero__BonusScene__

#include "cocos2d.h"

USING_NS_CC;

class BonusScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    
    virtual bool init();
    void initLabels();
    
    CREATE_FUNC(BonusScene);
    
private:
    Size visibleSize;
    void goToMainScene(Ref* sender);
};

#endif /* defined(__McDinero__BonusScene__) */
