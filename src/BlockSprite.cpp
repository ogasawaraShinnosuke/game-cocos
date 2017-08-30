//
//  BlockSprite.cpp
//  MyGame
//
//  Created by sin on 2014/04/29.
//
//

#include "BlockSprite.h"

BlockSprite::BlockSprite()
{
    initNextPos();
}

BlockSprite::~BlockSprite()
{
    
}

// 移動先の初期化
void BlockSprite::initNextPos()
{
    _nextPosX = -1;
    _nextPosY = -1;
}

// 移動先インデックスをセット
void BlockSprite::setNextPos(int nextPosX, int nextPosY)
{
    _nextPosX = nextPosX;
    _nextPosY = nextPosY;
}

BlockSprite* BlockSprite::createWithBlockType(kBlock blockType)
{
    BlockSprite *pRet = new BlockSprite();
    if (pRet && pRet->initWithBlockType(blockType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool BlockSprite::initWithBlockType(kBlock blockType)
{
    if (!Sprite::initWithFile(getBlockImageFileName(blockType)))
    {
        return false;
    }
    
    _blockType = blockType;
    
    return true;
}

// 駒のファイル名を返す.
const char* BlockSprite::getBlockImageFileName(kBlock blockType)
{
    switch (blockType) {
        case kBlockRed:
            return "red.png";
            
        case kBlockBlue:
            return "blue.png";
            
        case kBlockYellow:
            return "yellow.png";
            
        case kBlockGreen:
            return "green.png";
            
        case kBlockGray:
            return "gray.png";
            
        default:
            CCAssert(false, "invalid blockType");
            return "";
    }
}
