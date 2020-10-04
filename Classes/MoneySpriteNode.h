//
//  MoneySpriteNode.h
//  McDinero
//
//  Created by Smile0210 on 22/02/15.
//
//

#ifndef __McDinero__MoneySpriteNode__
#define __McDinero__MoneySpriteNode__

#include "cocos2d.h"

USING_NS_CC;

class MoneySpriteNode : public Sprite
{
public:
    MoneySpriteNode();
    ~MoneySpriteNode();
    static MoneySpriteNode* createWithTexture(Texture2D *texture);
    static MoneySpriteNode* createWithTextureAndPoints(Texture2D *texture, int points);
    void setPoints(int points)
    {
        this->points = points;
    }
    int getPoints()
    {
        return points;
    }
private:
    int points;
};

#endif /* defined(__McDinero__MoneySpriteNode__) */
