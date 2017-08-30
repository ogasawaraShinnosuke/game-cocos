//
//  GameScene.cpp
//  MyGame
//
//  Created by sin on 2014/04/29.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "BlockSprite.h"
#include "PlaySE.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Scene* GameScene::scene()
{
    Scene* scene = Scene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    return scene;
};

// 初期化
bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // タップイベントを取得する.
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // Android固定処理
    // バックキー・メニューキーイベントを取得する
//    setKeypadEnabled(true);
    
    // 変数初期化
    initForVariables();
    
    // 背景表示
    showBackground();
    
    // 駒表示
    showBlock();
    
    // ラベル作成
    showLabel();
    
    // ハイスコア処理
    showHighScoreLabel();
    
    // リセットボタン作成
    showResetButton();
    
    // 効果音の事前読み込み
    SimpleAudioEngine::getInstance()->preloadEffect(MP3_REMOVE_BLOCK);
    
    return true;
}

// タッチ開始イベント
bool GameScene::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    // アニメーション中はタップ処理を受け付けない
    return !_animation;
}

void GameScene::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    // タップポイント取得
    Point touchPoint = _background->convertTouchToNodeSpace(pTouch);
    
    // タップした駒のタグと駒の種類を取得
    int tag = 0;
    kBlock blockType;
    getTouchBlockTag(touchPoint, tag, blockType);
    
    if (tag != 0)
    {
        // 隣接する駒を検索する
        list<int> sameColoerBlockTags = getSameColorBlockTags(tag, blockType);
        
        if (sameColoerBlockTags.size() > 1)
        {
            // 得点加算 (消した駒数-2)の10乗
            _score += pow(sameColoerBlockTags.size() - 2, 3);
            
            // アニメーション開始
            _animation = true;
            
            // 隣接する駒を削除する
            removeBlock(sameColoerBlockTags, blockType);
            
            
            // 駒削除後のアニメーション
            movingBlocksAnimation1(sameColoerBlockTags);
        }
    }
}

// 背景表示
void GameScene::showBackground()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    // 背景を生成
    _background = Sprite::create(PNG_BACKGROUND);
    _background->setPosition(Point(winSize.width / 2, winSize.height / 2));
    addChild(_background, kZOrderBackground, kTagBackground);
}

// 変数初期化
void GameScene::initForVariables()
{
    // 乱数初期化
    srand((unsigned)time(nullptr));
    
    // 駒の一辺の長さを取得
    BlockSprite* pBlock = BlockSprite::createWithBlockType(kBlockRed);
    _blockSize = pBlock->getContentSize().height;
    
    // 駒種類の配列生成
    blockTypes.push_back(kBlockRed);
    blockTypes.push_back(kBlockBlue);
    blockTypes.push_back(kBlockYellow);
    blockTypes.push_back(kBlockGreen);
    blockTypes.push_back(kBlockGray);
    
    // アニメーション発生フラグを落とす
    _animation = false;
    
    // 得点を初期化
    _score = 0;
}

// 駒表示
void GameScene::showBlock()
{
    // 8*8の駒を作成
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            // ランダムで駒を作成
            kBlock blockType = (kBlock)(rand() % kBlockCount);
            
            // 対応する駒配列にタグを追加
            int tag = getTag(x, y);
            _blockTags[blockType].push_back(tag);
            
            // 駒を作成
            BlockSprite* pBlock = BlockSprite::createWithBlockType(blockType);
            pBlock->setPosition(getPosition(x, y));
            _background->addChild(pBlock, kZOrderBlock, tag);
            
        }
    }
}
// 位置取得
Point GameScene::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = _background->getContentSize().width * 0.3;
    float offsetY = _background->getContentSize().height * 0.2;
    return Point((posIndexX + 0.5) * _blockSize + offsetX, (posIndexY + 0.5) * _blockSize + offsetY);
}

// タグ取得
int GameScene::getTag(int posIndexX, int posIndexY)
{
    return kTagBaseBlock + posIndexX * 100 + posIndexY;
}

// タップされた駒のタグを取得
void GameScene::getTouchBlockTag(cocos2d::Point touchPoint, int &tag, kBlock &blockType)
{
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            int currentTag = getTag(x, y);
            // 駒を取得
            Node* node = _background->getChildByTag(currentTag);
            if (node && node->boundingBox().containsPoint(touchPoint))
            {
                tag = currentTag;
                blockType = ((BlockSprite*)node)->getBlockType();
                
                return;
            }
        }
    }
}

// 駒配列にあるか検索
bool GameScene::hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag)
{
    list<int>::iterator it;
    for (it = blockTagList.begin(); it != blockTagList.end(); ++it)
    {
        if (*it == searchBlockTag)
        {
            return true;
        }
    }
    
    return false;
}

// タップされた駒と同色でかつ接している駒の配列を返す
list<int> GameScene::getSameColorBlockTags(int baseTag, kBlock blockType)
{
    list<int> sameColorBlockTags;
    sameColorBlockTags.push_back(baseTag);
    
    list<int>::iterator it = sameColorBlockTags.begin();
    while (it != sameColorBlockTags.end()) {
        int tags[] = {
            *it + 100,
            *it - 100,
            *it + 1,
            *it - 1,
        };
        
        for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
        {
            // 既にリストにあるか検索
            if (!hasSameColorBlock(sameColorBlockTags, tags[i]))
            {
                // 駒配列にあるか検索
                if (hasSameColorBlock(_blockTags[blockType], tags[i]))
                {
                    sameColorBlockTags.push_back(tags[i]);
                }
            }
        }
        
        it++;
    }
    
    return sameColorBlockTags;
}

// 配列の駒を削除
void GameScene::removeBlock(std::list<int> blockTags, kBlock blockType)
{
    bool first = true;
    
    list<int>::iterator it = blockTags.begin();
    while (it != blockTags.end())
    {
        
        //既存配列から該当駒を削除
        _blockTags[blockType].remove(*it);
        
        // 対象となる駒を取得
        Node* block = _background->getChildByTag(*it);
        if (block)
        {
            // 駒が消えるアニメーションを生成
            ScaleTo* scale = ScaleTo::create(REMOVING_TIME, 0);
            
            // 駒を削除するアクションを生成
            CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(GameScene::removingBlock, this));
//            cocos2d::CallFunc* func = CallFunc::create([this]()
//                                                       {
//                                                           this->removingBlock(this);
//                                                       });
            
            // アクションをつなげる
            FiniteTimeAction* sequence = Sequence::create(scale, func, nullptr);
            
            FiniteTimeAction* action;
            if (first)
            {
                // 駒が消えるサウンドアクションを生成
                PlaySE* playSe = PlaySE::create(MP3_REMOVE_BLOCK);
                
                // アクションをつなげる
                action = Spawn::create(sequence, playSe, nullptr);
                
                first = false;
            }
            else
            {
                action = sequence;
            }
            
            // アクションをセットする
            block->runAction(action);
        }
        
        it++;
    }
    
    SimpleAudioEngine::getInstance()->playEffect(MP3_REMOVE_BLOCK);
}

// 駒の削除
void GameScene::removingBlock(cocos2d::Node *block)
{
    block->removeFromParentAndCleanup(true);
}

// 駒のインデックス取得
GameScene::PositionIndex GameScene::getPositionIndex(int tag)
{
    int pos1_x = (tag - kTagBaseBlock) / 100;
    int pos1_y = (tag - kTagBaseBlock) % 100;
    
    return PositionIndex(pos1_x, pos1_y);
}

// 新しい位置をセット
void GameScene::setNewPosition1(int tag, GameScene::PositionIndex posIndex)
{
    BlockSprite* blockSprite = (BlockSprite*)_background->getChildByTag(tag);
    int nextPosY = blockSprite->getNextPosY();
    if (nextPosY == -1)
    {
        nextPosY = posIndex.y;
    }
    
    // 移動先の位置をセット
    blockSprite->setNextPos(posIndex.x, --nextPosY);
}

// 消えた駒を埋めるように新しい位置をセット
void GameScene::searchNewPosition1(std::list<int> blocks)
{
    // 消える駒数分のループ
    list<int>::iterator it1 = blocks.begin();
    while (it1 != blocks.end())
    {
        PositionIndex posIndex1 = getPositionIndex(*it1);
        
        // 駒種類のループ
        vector<kBlock>::iterator it2 = blockTypes.begin();
        while (it2 != blockTypes.end())
        {
            // 各種類の駒数分のループ
            list<int>::iterator it3 = _blockTags[*it2].begin();
            while (it3 != _blockTags[*it2].end())
            {
                PositionIndex posIndex2 = getPositionIndex(*it3);
                
                if (posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y)
                {
                    // 消える駒の上に位置する駒に対して、移動先の位置をセットする
                    setNewPosition1(*it3, posIndex2);
                }
                
                it3++;
            }
            
            it2++;
        }
        
        it1++;
    }
}

// 駒を移動する
void GameScene::moveFallBlock()
{
    // 駒種類のループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        // 各種類の駒数分のループ
        list<int>::iterator it2 = _blockTags[*it1].begin();
        while (it2 != _blockTags[*it1].end())
        {
            BlockSprite* blockSprite = (BlockSprite*)_background->getChildByTag(*it2);
            int nextPosX = blockSprite->getNextPosX();
            int nextPosY = blockSprite->getNextPosY();
            
            if (nextPosX != -1 || nextPosY != -1)
            {
                // 新しいタグをセットする
                int newTag = getTag(nextPosX, nextPosY);
                blockSprite->initNextPos();
                blockSprite->setTag(newTag);
                
                // タグ一覧の値も新しいタグに変更する
                *it2 = newTag;
                
                // アニメーションをセットする
                MoveTo* move = MoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                blockSprite->runAction(move);
            }
            
            it2++;
        }
        
        it1++;
    }
}

// 駒削除後のアニメーション
void GameScene::movingBlocksAnimation1(std::list<int> blocks)
{
    // 駒の新しい位置をセットする
    searchNewPosition1(blocks);
    
    // 新しい位置がセットされた駒のアニメーション
    moveFallBlock();
    
    // アニメーション終了時に次のアニメーション処理を開始する
    scheduleOnce(schedule_selector(GameScene::movingBlocksAnimation2), MOVING_TIME);
}

// 駒の移動完了
void GameScene::movedBlocks(float fDelta)
{
    // ラベル再表示
    showLabel();
    
    // アニメーション終了
    _animation = false;
    
    // ゲーム終了チェック
    if (!existsSameBlock())
    {
        // ハイスコア記録・表示
        saveHighScore();
        
        Size bgSize = _background->getContentSize();
        
        // ゲーム終了表示
        Sprite* gameOver = Sprite::create(PNG_GAMEOVER);
        gameOver->setPosition(Point(bgSize.width / 2, bgSize.height * 0.8));
        _background->addChild(gameOver, kZOrderGameOver, kTagGameOver);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
}

// 新しい位置をセット
void GameScene::setNewPosition2(int tag, GameScene::PositionIndex posIndex)
{
    BlockSprite* blockSprite = (BlockSprite*)_background->getChildByTag(tag);
    int nextPosX = blockSprite->getNextPosX();
    if (nextPosX == -1)
    {
        nextPosX = posIndex.x;
    }
    
    // 移動先の位置をセット
    blockSprite->setNextPos(--nextPosX, posIndex.y);
}

// 存在する列を取得する
map<int, bool> GameScene::getExistsBlockColumn()
{
    // 検索配列初期化
    map<int, bool> xBlock;
    for (int i = 0; i < MAX_BLOCK_X; i++)
    {
        xBlock[i] = false;
    }
    
    // 駒種類のループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        // 各種類の駒数分のループ
        list<int>::iterator it2 = _blockTags[*it1].begin();
        while (it2 != _blockTags[*it1].end())
        {
            // 存在する駒のX位置を記録
            xBlock[getPositionIndex(*it2).x] = true;
            
            it2++;
        }
        
        it1++;
    }
    
    return xBlock;
}

// 消えた駒列を埋めるように新しい位置をセット
void GameScene::searchNewPosition2()
{
    // 存在する列を取得する
    map<int, bool> xBlock = getExistsBlockColumn();
    
    // 駒が存在しないx位置を埋める
    bool first = true;
    for (int i = MAX_BLOCK_X - 1; i >= 0; i--)
    {
        if (xBlock[i])
        {
            // 駒が存在する
            first = false;
            continue;
        }
        else
        {
            // 駒が存在しない
            if (first)
            {
                // 右側に駒がない
                continue;
            }
            else
            {
                // この位置より右側にある駒を左に１つ寄せる
                
                // 駒種類のループ
                vector<kBlock>::iterator it1 = blockTypes.begin();
                while (it1 != blockTypes.end())
                {
                    // 各種類の駒数分のループ
                    list<int>::iterator it2 = _blockTags[*it1].begin();
                    while (it2 != _blockTags[*it1].end())
                    {
                        PositionIndex posIndex = getPositionIndex(*it2);
                        
                        if (i < posIndex.x)
                        {
                            // 移動先の位置をセットする
                            setNewPosition2(*it2, posIndex);
                        }
                        
                        it2++;
                    }
                    
                    it1++;
                }
            }
        }
    }
}

// 駒のアニメーション
void GameScene::movingBlocksAnimation2(float fDelta)
{
    // 駒の新しい位置をセットする
    searchNewPosition2();
    
    // 新しい位置がセットされた駒のアニメーション
    moveFallBlock();
    
    // アニメーション終了時に次の処理を開始する
    scheduleOnce(schedule_selector(GameScene::movedBlocks), MOVING_TIME);
}

// ラベル表示
void GameScene::showLabel()
{
    Size bgSize = _background->getContentSize();
    
    // 残数表示
    int tagsForLabel[] = {kTagRedLabel, kTagBlueLabel, kTagYellowLabel, kTagGreenLabel, kTagGrayLabel};
    const char* fontNames[] = {FONT_RED, FONT_BLUE, FONT_YELLOW, FONT_GREEN, FONT_GRAY};
    float heightRate[] = {0.60, 0.50, 0.40, 0.30, 0.20};
    
    // 駒種類のループ
    vector<kBlock>::iterator it = blockTypes.begin();
    while (it != blockTypes.end())
    {
        // 駒残数表示
        long count = _blockTags[*it].size();
        const char* countStr = ccsf("%02ld", count);
        Label* label = (Label*)_background->getChildByTag(tagsForLabel[*it]);
        if (!label)
        {
            // 駒残数生成
            label = Label::createWithBMFont(fontNames[*it], countStr);
            label->setPosition(Point(bgSize.width * 0.78, bgSize.height * heightRate[*it]));
            _background->addChild(label, kZOrderLabel, tagsForLabel[*it]);
        }
        else
        {
            label->setString(countStr);
        }
        
        it++;
    }
    
    // スコア表示
    const char* scoreStr = ccsf("%d", _score);
    Label* scoreLabel = (Label*)_background->getChildByTag(kTagScoreLabel);
    if (!scoreLabel)
    {
        // スコア生成
        scoreLabel = Label::createWithBMFont(FONT_WHITE, scoreStr);
        scoreLabel->setPosition(Point(bgSize.width * 0.78, bgSize.height * 0.70));
        _background->addChild(scoreLabel, kZOrderLabel, kTagScoreLabel);
    }
    else
    {
        scoreLabel->setString(scoreStr);
    }
}

// 全駒に対して、隣り合う駒が存在するかチェック
bool GameScene::existsSameBlock()
{
    // 駒種類のループ
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        // 各種類の駒数分のループ
        list<int>::iterator it2 = _blockTags[*it1].begin();
        while (it2 != _blockTags[*it1].end())
        {
            if (getSameColorBlockTags(*it2, *it1).size() > 1)
            {
                // 隣り合う駒が存在する場合は、trueを返す
                return true;
            }
            
            it2++;
        }
        
        it1++;
    }
    
    // 隣り合う駒が存在しない場合は、falseを返す
    return false;
}

// ハイスコアラベル表示
void GameScene::showHighScoreLabel()
{
    Size bgSize = _background->getContentSize();
    
    // ハイスコア表示
    int highScore = UserDefault::getInstance()->getIntegerForKey(KEY_HIGHSCORE, 0);
    const char* highScoreStr = ccsf("%d", highScore);
    Label* highScoreLabel = (Label*)_background->getChildByTag(kTagHighScoreLabel);
    if (!highScoreLabel)
    {
        // ハイスコア生成
        highScoreLabel = Label::createWithBMFont(FONT_WHITE, highScoreStr);
        highScoreLabel->setPosition(Point(bgSize.width * 0.78, bgSize.height * 0.80));
        _background->addChild(highScoreLabel, kZOrderLabel, kTagHighScoreLabel);
    }
    else
    {
        highScoreLabel->setString(highScoreStr);
    }
}

// ハイスコア記録・表示
void GameScene::saveHighScore()
{
    UserDefault* userDefault = UserDefault::getInstance();
    
    // ハイスコアを取得する
    int oldHighScore = userDefault->getIntegerForKey(KEY_HIGHSCORE, 0);
    if (oldHighScore < _score)
    {
        // ハイスコアを保持する
        userDefault->setIntegerForKey(KEY_HIGHSCORE, _score);
        userDefault->flush();
        
        // ハイスコアを表示する
        showHighScoreLabel();
    }
}

// リセットボタンタップ時の処理
void GameScene::menuResetCallback(Ref* pSender)
{
    GameScene* scene = GameScene::create();
    Director::getInstance()->replaceScene((Scene*)scene);
}

// リセットボタン作成
void GameScene::showResetButton()
{
    Size bgSize = _background->getContentSize();

    // リセットボタン作成
    auto resetButton = MenuItemImage::create(PNG_RESET, PNG_RESET,
                                                       CC_CALLBACK_1(GameScene::menuResetCallback, this));
    resetButton->setPosition(Point(bgSize.width / 5, bgSize.height * 0.80));

    // メニュー作成
    Menu* menu = Menu::create(resetButton, nullptr);
    menu->setPosition(Point(Size::ZERO));
    _background->addChild(menu);
}


/* Android固定処理 */

// バックキーイベント
//void onKeyReleasedonKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event)
//{
//    Director::getInstance()->end();
//}

// メニューキーイベント
//void GameScene::keyMenuClicked()
//{
//    menuResetCallback(nullptr);
//}
