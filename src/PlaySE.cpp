//
//  PlaySE.cpp
//  MyGame
//
//  Created by sin on 2014/04/29.
//
//

#include "PlaySE.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

PlaySE::PlaySE(std::string sound)
{
    _sound = sound;
}

PlaySE* PlaySE::create(std::string sound)
{
    PlaySE* pRet = new PlaySE(sound);
    if (pRet)
    {
        // インスタンスが不要になったら自動で解放
        pRet->autorelease();
    }
    
    return pRet;
}

void PlaySE::update(float time)
{
    CC_UNUSED_PARAM(time);
    SimpleAudioEngine::getInstance()->playEffect(_sound.c_str());
}

ActionInstant* PlaySE::reverse()
{
    return (ActionInstant*)(PlaySE::create(_sound));
}
