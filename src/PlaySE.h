//
//  PlaySE.h
//  MyGame
//
//  Created by sin on 2014/04/29.
//
//

#ifndef __MyGame__PlaySE__
#define __MyGame__PlaySE__

#include "cocos2d.h"

// 効果音を再生するアクション(一度のみの処理で完了するもの)
class PlaySE : public cocos2d::Show
{
public:
    PlaySE(std::string sound);
    virtual ~PlaySE(){}
    
    virtual void update(float time);
    virtual ActionInstant* reverse(void);
        
public:
    static PlaySE* create(std::string sound);
    
protected:
    std::string _sound;
};
#endif /* defined(__MyGame__PlaySE__) */
