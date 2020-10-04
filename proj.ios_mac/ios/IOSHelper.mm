//
//  IOSHelper.cpp
//  McDinero
//
//  Created by Smile0210 on 15/03/15.
//
//

#include "IOSHelper.h"
#include "../proj.ios_mac/ios/AppController.h"

void IOSHelper::shareScore(int score, bool hasNewHighScore, int socialMedia)
{
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate shareScoreWithPoints:score hasNewScore:hasNewHighScore andSocial:socialMedia];
}
