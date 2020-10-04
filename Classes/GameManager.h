//
//  GameManager.h
//  McDinero
//
//  Created by Smile0210 on 24/02/15.
//
//

#ifndef __McDinero__GameManager__
#define __McDinero__GameManager__

#include "cocos2d.h"

USING_NS_CC;

class GameManager : public Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static GameManager* getInstance(void);
    void restartValues();
    
public:
    virtual ~GameManager();
    bool init(void);
    bool upgradeAfterRap;
    bool bonusAfterRap;
    bool soundActive;
    bool backgroundMusicLoaded;
    bool isGoingToRapScene;
    
    bool immunityActive;
    bool isRussianActive;
    bool isMiniatureActive;
    bool isGigantMoneyActive;
    
    int pauPauGreenCollected;
    int pauPauRedCollected;
    int pauPauYellowCollected;
    int pauPauPurpleCollected;
    int pauPauPinkCollected;
    int pauPauOrangeCollected;
    int rapBonusCollected;
    
    int totalNewAchievementsUnloked;
    
    int idBackGroundMusic;
};

#endif /* defined(__McDinero__GameManager__) */
