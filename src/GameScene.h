//
//  GameScene.h
//  MyGame
//
//  Created by sin on 2014/04/29.
//
//

#ifndef __MyGame__GameScene__
#define __MyGame__GameScene__

#include "cocos2d.h"
#include "Config.h"

// x軸とy軸の駒数
#define MAX_BLOCK_X 8
#define MAX_BLOCK_Y 8

// 駒を小さくするアニメーションの時間
#define REMOVING_TIME 0.1f

// 駒の移動に要する時間
#define MOVING_TIME 0.2f

// ハイスコア
#define KEY_HIGHSCORE "HighScore"

// Resource
// 背景
#define PNG_BACKGROUND "background.png"
// ゲーム終了
#define PNG_GAMEOVER "gameover.png"
// リセット
#define PNG_RESET "reset.png"
// 効果音
#define MP3_REMOVE_BLOCK "removeBlock.mp3"
// フォント
#define FONT_RED "redFont.fnt"
#define FONT_BLUE "blueFont.fnt"
#define FONT_YELLOW "yellowFont.fnt"
#define FONT_GREEN "greenFont.fnt"
#define FONT_GRAY "grayFont.fnt"
#define FONT_WHITE "whiteFont.fnt"

class GameScene : public cocos2d::Layer
{
protected:
    enum kTag
    {
        kTagBackground = 1,
        
        // ラベル
        kTagRedLabel,
        kTagBlueLabel,
        kTagYellowLabel,
        kTagGreenLabel,
        kTagGrayLabel,
        
        // 得点
        kTagScoreLabel,
        
        // ゲーム終了
        kTagGameOver,
        
        // ハイスコア
        kTagHighScoreLabel,

        // 駒の基準タグ
        kTagBaseBlock = 10000,
    };
    enum kZOrder
    {
        kZOrderBackground,
        
        // ラベル
        kZOrderLabel,
        
        // 駒の基準タグ
        kZOrderBlock,
        
        // ゲーム終了
        kZOrderGameOver,
    };
    
    // 駒の位置を表すインデックス
    struct PositionIndex
    {
        PositionIndex(int x1, int y1)
        {
            x = x1;
            y = y1;
        }
        
        int x;
        int y;
    };
    
    cocos2d::Sprite* _background;
    void showBackground();
    
    // 駒の一辺のサイズ
    float _blockSize;
    // 駒の種類ごとのグループ
    std::map<kBlock, std::list<int> > _blockTags;
    // 関数定義
    void initForVariables();
    void showBlock();
    cocos2d::Point getPosition(int posIndexX, int posIndexY);
    int getTag(int posIndexX, int posIndexY);
    
    // 駒の削除などのアクション
    void getTouchBlockTag(cocos2d::Point touchPoint, int &tag, kBlock &blockType);
    std::list<int> getSameColorBlockTags(int baseTag, kBlock blockType);
    void removeBlock(std::list<int> blockTags, kBlock blockType);
    bool hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag);
    
    // 駒の削除アニメーション
    void removingBlock(cocos2d::Node* block);
    
    // アニメーションに必要な変数の定義
    std::vector<kBlock> blockTypes;
    PositionIndex getPositionIndex(int tag);
    
    // 落下アニメーション
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(std::list<int> blocks);
    void moveFallBlock();
    void movingBlocksAnimation1(std::list<int> blocks);
    
    // 左詰めアニメーション
    bool _animation;
    void movedBlocks(float fDelta);
    std::map<int, bool> getExistsBlockColumn();
    void searchNewPosition2();
    void setNewPosition2(int tag, PositionIndex posIndex);
    void movingBlocksAnimation2(float fDelta);
    
    // ラベルの初期表示と更新
    void showLabel();
    
    // スコア
    int _score;
    
    // ゲーム終了
    bool existsSameBlock();
    
    // ハイスコア処理
    void saveHighScore();
    void showHighScoreLabel();
    
    // リセット
    void menuResetCallback(Ref* pSender);
    void showResetButton();
    
public:
    virtual bool init();
    static cocos2d::Scene* scene();
    CREATE_FUNC(GameScene);
    
    // タップ開始
    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    // タップ終了
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    
    // Android固定処理
    // バックキーイベント
//    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event);
    // メニューキーイベント
//    virtual void keyMenuClicked();
};

#endif /* defined(__MyGame__GameScene__) */
