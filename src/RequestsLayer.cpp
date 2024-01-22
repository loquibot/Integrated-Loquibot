#include "RequestsLayer.h"
#include "GlobalVars.h"

RequestsLayer* RequestsLayer::create(CCArray* levels) {
    auto ret = new RequestsLayer();
    if (ret && ret->init(levels)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool RequestsLayer::init(CCArray* levels) {

    GlobalVars::getSharedInstance()->onReqScene = true;

    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto size = backgroundSprite->getContentSize(); 
    
    backgroundSprite->setScaleX(winSize.width / size.width);
    backgroundSprite->setScaleY(winSize.height / size.height);
    backgroundSprite->setID("background"_spr);
    
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setColor({0, 102, 255});
    
    backgroundSprite->setZOrder(-2);
    addChild(backgroundSprite);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
        this,
        menu_selector(RequestsLayer::onBack)
    );
    backBtn->setID("exit-button"_spr);

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});
    menuBack->setID("exit-menu"_spr);

    CCSprite* m_cornerBL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerBL->setPosition({0,0});
    m_cornerBL->setAnchorPoint({0,0});
    addChild(m_cornerBL, -1);
    m_cornerBL->setID("corner-bl"_spr);

    CCSprite* m_cornerTL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerTL->setPosition({0,winSize.height});
    m_cornerTL->setAnchorPoint({0,1});
    m_cornerTL->setFlipY(true);
    m_cornerTL->setID("corner-ul"_spr);
    addChild(m_cornerTL, -1);

    CCSprite* m_cornerTR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerTR->setPosition({winSize.width,winSize.height});
    m_cornerTR->setAnchorPoint({1,1});
    m_cornerTR->setFlipX(true);
    m_cornerTR->setFlipY(true);
    m_cornerTR->setID("corner-ur"_spr);
    addChild(m_cornerTR, -1);

    CCSprite* m_cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_cornerBR->setPosition({winSize.width,0});
    m_cornerBR->setAnchorPoint({1,0});
    m_cornerBR->setFlipX(true);
    m_cornerBR->setID("corner-br"_spr);
    addChild(m_cornerBR, -1);

    addChild(menuBack);

    CustomListView* customListView = CustomListView::create(levels, nullptr, 240, 356, 0, BoomListType::Level4, 0);

    GJListLayer* listLayer = GJListLayer::create(customListView, "", {191, 114, 62, 255}, 356, 240, 1);
    listLayer->setTag(736357);
    listLayer->setAnchorPoint({0.5, 0.5});
    listLayer->ignoreAnchorPointForPosition(false);
    listLayer->setPosition(winSize.width/2, winSize.height/2 - 5);

    CCSprite* levelRequestsLabel = CCSprite::create("levelRequestsLabel.png"_spr);
    levelRequestsLabel->setPosition({winSize.width/2, winSize.height - 28});
    levelRequestsLabel->setScale(0.8f);

    addChild(listLayer);
    addChild(levelRequestsLabel);

    setKeypadEnabled(true);

    return true;
}



void RequestsLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}


void RequestsLayer::onBack(CCObject* object) {
    GlobalVars::getSharedInstance()->onReqScene = false;
    keyBackClicked();
}

CCScene* RequestsLayer::scene(CCArray* levels) {
    auto layer = RequestsLayer::create(levels);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
