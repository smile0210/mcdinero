//
//  PauPauSpriteNode.cpp
//  McDinero
//
//  Created by Smile0210 on 02/03/15.
//
//

#include "PauPauSpriteNode.h"


PauPauSpriteNode::PauPauSpriteNode() { }

PauPauSpriteNode::~PauPauSpriteNode() { }

PauPauSpriteNode* PauPauSpriteNode::createWithTexture(Texture2D *texture)
{
    PauPauSpriteNode *sprite = new (std::nothrow) PauPauSpriteNode();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

PauPauSpriteNode* PauPauSpriteNode::createWithTextureAndPauPauType(Texture2D *texture, PauPauType type)
{
    PauPauSpriteNode *sprite = new (std::nothrow) PauPauSpriteNode();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        sprite->pauPauType = type;
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
