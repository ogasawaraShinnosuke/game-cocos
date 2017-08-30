//
//  Config.h
//  MyGame
//
//  Created by sin on 2014/04/29.
//
//

#ifndef MyGame_Config_h
#define MyGame_Config_h

enum kBlock
{
    kBlockRed,
    kBlockBlue,
    kBlockYellow,
    kBlockGreen,
    kBlockGray,
    kBlockCount,
};

// ラベルのフォーマット
#define ccsf(...) String::createWithFormat(__VA_ARGS__)->getCString()

#endif
