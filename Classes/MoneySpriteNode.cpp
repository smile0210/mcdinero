//
//  MoneySpriteNode.cpp
//  McDinero
//
//  Created by Smile0210 on 22/02/15.
//
//

#include "MoneySpriteNode.h"

MoneySpriteNode::MoneySpriteNode() { }

MoneySpriteNode::~MoneySpriteNode() { }

MoneySpriteNode* MoneySpriteNode::createWithTexture(Texture2D *texture)
{
    MoneySpriteNode *sprite = new (std::nothrow) MoneySpriteNode();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        sprite->points = 0;
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MoneySpriteNode* MoneySpriteNode::createWithTextureAndPoints(Texture2D *texture, int points)
{
    MoneySpriteNode *sprite = new (std::nothrow) MoneySpriteNode();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        sprite->points = points;
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
