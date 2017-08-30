#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // auto:型予測
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    director->setDisplayStats(true);
    
    // FPSの低下情報(デバッグモードのみ表示)
    if (COCOS2D_DEBUG > 0)
    {
        director->setDisplayStats(true);
    }
    else
    {
        director->setDisplayStats(false);
    }
    
    director->setAnimationInterval(1.0 / 60);
    
    // 画像が画面の高さにフィットして表示される設定
    director->setContentScaleFactor(768.0f / director->getWinSize().height);
    auto scene = GameScene::scene();
    director->runWithScene(scene);
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
