//
//  PauPauSpriteNode.h
//  McDinero
//
//  Created by Smile0210 on 02/03/15.
//
//

#ifndef __McDinero__PauPauSpriteNode__
#define __McDinero__PauPauSpriteNode__

#include "cocos2d.h"
#include "McDineroTypes.h"

USING_NS_CC;

class PauPauSpriteNode : public Sprite
{
public:
    PauPauSpriteNode();
    ~PauPauSpriteNode();
    static PauPauSpriteNode* createWithTexture(Texture2D *texture);
    static PauPauSpriteNode* createWithTextureAndPauPauType(Texture2D *texture, PauPauType type);
    void setPauPauType(PauPauType type)
    {
        this->pauPauType = type;
    }
    PauPauType getPauPauType()
    {
        return pauPauType;
    }
private:
    PauPauType pauPauType;
};

#endif /* defined(__McDinero__PauPauSpriteNode__) */
