//
//  GameManager.cpp
//  McDinero
//
//  Created by Smile0210 on 24/02/15.
//
//

#include "GameManager.h"

static GameManager *_sharedGameManager = NULL;

GameManager* GameManager::getInstance()
{
    if (! _sharedGameManager)
    {
        _sharedGameManager = new GameManager();
        _sharedGameManager->init();
    }
    
    return _sharedGameManager;
}

GameManager::~GameManager(void)
{
    
}

bool GameManager::init(void)
{
    return true;
}

void GameManager::restartValues()
{
    upgradeAfterRap = false;
    bonusAfterRap = false;
    isGoingToRapScene = false;
    immunityActive = false;
    isRussianActive = false;
    isMiniatureActive = false;
    isGigantMoneyActive = false;
    
    totalNewAchievementsUnloked = 0;
}
