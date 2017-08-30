//
//  BlockSprite.h
//  MyGame
//
//  Created by sin on 2014/04/29.
//
//

#ifndef __MyGame__BlockSprite__
#define __MyGame__BlockSprite__

#include "cocos2d.h"
#include "Config.h"

class BlockSprite : public cocos2d::Sprite
{
protected:
    const char* getBlockImageFileName(kBlock blockType);
    
public:
    
    // アクセサ
    CC_SYNTHESIZE_READONLY(kBlock, _blockType, BlockType);
    CC_SYNTHESIZE_READONLY(int, _nextPosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, _nextPosY, NextPosY);
    
    // 移動先のx,yインデックスを初期化
    void initNextPos();
    void setNextPos(int nextPosX, int nextPosY);
    
    BlockSprite();
    virtual ~BlockSprite();
    virtual bool initWithBlockType(kBlock blockType);
    static BlockSprite* createWithBlockType(kBlock blockType);
};

#endif /* defined(__MyGame__BlockSprite__) */
