//
//  BlockSprite.h
//  Nyangame
//
//  Created by P on 2014/07/04.
//
//

#ifndef __Nyangame__BlockSprite__
#define __Nyangame__BlockSprite__

#include <iostream>
#include "cocos2d.h"
#include "Config.h"

class BlockSprite : public cocos2d::Sprite
{
protected:
    const char* getBlockImageFileName(kBlock blockType);
    
public:
    CC_SYNTHESIZE_READONLY(kBlock, m_blockType, BlockType);
    CC_SYNTHESIZE_READONLY(int, m_nextPosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, m_nextPosY, NextPosY);
    
    void initNextPos();
    void setNextPos(int nextPosX, int nextPosY);
    
    BlockSprite();
    virtual ~BlockSprite();
    virtual bool initWithBlockType(kBlock blockType);
    static BlockSprite* createWithBlockType(kBlock blockType);
};


#endif /* defined(__Nyangame__BlockSprite__) */
