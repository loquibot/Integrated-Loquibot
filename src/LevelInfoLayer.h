#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include "ServerListener.h"
#include "Loquibot.h"
#include "GlobalVars.h"
#include "GJGameLevel.h"
#include <alphalaneous.pages_api/include/PageMenu.h>

matjson::Value getFromArray(int id);

bool isLoquiDownload = false;

class $modify(FLAlertLayer) {

    struct Fields {
        bool m_showable = true;
    };

    bool init(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5, bool p6, float p7, float p8) {
        
        if (isLoquiDownload) {
            m_fields->m_showable = false;

            geode::createQuickPopup(p1, p2, p3, p4, nullptr, true);

            return true;
        }
        return FLAlertLayer::init(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    }

    void show(){
        if(m_fields->m_showable) {
            FLAlertLayer::show();
        }
    }
};

class $modify(LoquiLevelInfoLayer, LevelInfoLayer) {

    struct Fields {
        bool m_isLevelRequest = false;
        std::string m_message = "";
    };

    void levelDownloadFinished(GJGameLevel* p0){
        if(m_fields->m_isLevelRequest){
            isLoquiDownload = true;
        }

        LevelInfoLayer::levelDownloadFinished(p0);

        isLoquiDownload = false;
    }

    void levelDownloadFailed(int p0){
        
        if(m_fields->m_isLevelRequest){
            isLoquiDownload = true;
        }

        LevelInfoLayer::levelDownloadFailed(p0);

        isLoquiDownload = false;
    }

    bool init(GJGameLevel* level, bool a2) {

        if (level->m_levelID == GlobalVars::getSharedInstance()->currentID){
            isLoquiDownload = true;
        }

        if (!LevelInfoLayer::init(level, a2)) return false;

        if (level->m_levelID != GlobalVars::getSharedInstance()->currentID) {
            GlobalVars::getSharedInstance()->isLoquiMenu = false;
            return false;
        }
        m_fields->m_message = static_cast<LoquiGJGameLevel*>(level)->m_fields->m_message;

        isLoquiDownload = false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        Loquibot::getSharedInstance()->m_isClickable = true;

        m_fields->m_isLevelRequest = true;
        GlobalVars::getSharedInstance()->isLoquiMenu = true;

        matjson::Value listLevel = getFromArray(level->m_levelID);

        std::string requester;

        if(listLevel != nullptr){
            requester = listLevel["requester"].asString().unwrapOr("");
        }
        else {
            requester = GlobalVars::getSharedInstance()->requester;
        }

        if (Mod::get()->getSettingValue<bool>("show-auto-pin")) {
            GlobalVars::getSharedInstance()->autoPinCheck = true;

            if (CCMenu* rightSideMenu = typeinfo_cast<CCMenu*>(getChildByID("right-side-menu"))) {
                if (CCMenuItemSpriteExtra* globedButton = typeinfo_cast<CCMenuItemSpriteExtra*>(rightSideMenu->getChildByID("dankmeme.globed2/share-room-btn"))) {
                    CCMenu* checkboxMenu = CCMenu::create();
                    checkboxMenu->setContentSize({76, 30});
                    checkboxMenu->setAnchorPoint({0, 0});
                    checkboxMenu->ignoreAnchorPointForPosition(false);
                    checkboxMenu->setScale(0.7f);
                    checkboxMenu->setPosition(-5, -3);

                    RowLayout* layout = RowLayout::create();
                    layout->setGap(0);
                    checkboxMenu->setLayout(layout);

                    CCMenuItemToggler* toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(LoquiLevelInfoLayer::onPinToggle), 1);
                    checkboxMenu->addChild(toggler);

                    globedButton->addChild(checkboxMenu);

                    if (bool pin = Mod::get()->getSavedValue<bool>("auto-pin")) {
                        toggler->toggle(pin);
                        globedButton->activate();
                    }

                    CCLabelBMFont* autoLabel = CCLabelBMFont::create("Auto", "bigFont.fnt");
                    autoLabel->setScale(0.7f);

                    checkboxMenu->addChild(autoLabel);
                    checkboxMenu->updateLayout();
                }
            }

            GlobalVars::getSharedInstance()->autoPinCheck = false;
        }
        
        auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        nextButtonSprite->setFlipX(true);

        auto topButtonSprite = CCSprite::createWithSpriteFrameName("GJ_orderUpBtn_001.png");
        topButtonSprite->setScale(0.925f);

        float buttonXPos = winSize.width - 100;

        auto nextButton = CCMenuItemSpriteExtra::create(nextButtonSprite, this,
            menu_selector(Loquibot::goToNextLevel));

        nextButton->setID("next-button"_spr);
        nextButton->setPosition({ buttonXPos, winSize.height / 2 + 30 });

        auto topButton = CCMenuItemSpriteExtra::create(topButtonSprite, this,
            menu_selector(Loquibot::goToTopLevel));

        topButton->setID("top-button"_spr);
        topButton->setPosition({ buttonXPos, winSize.height / 2 + 110 });

        auto randomButtonSprite = CCSprite::create("LB_randomBtn.png"_spr);
        randomButtonSprite->setFlipX(true);
        randomButtonSprite->setScale(0.9);
        auto randomButton = CCMenuItemSpriteExtra::create(randomButtonSprite, this,
            menu_selector(Loquibot::goToRandomLevel));
        randomButton->setID("random-button"_spr);
        randomButton->setPosition({ buttonXPos, winSize.height / 2 - 20 });

        auto undoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
        undoButtonSprite->setScale(0.9f);
        auto undoButton = CCMenuItemSpriteExtra::create(undoButtonSprite, this,
            menu_selector(Loquibot::goToUndoLevel));
        undoButton->setID("undo-button"_spr);
        undoButton->setPosition({ 80, winSize.height / 2 + 30 });

        auto blockButtonSprite = CCSprite::create("LB_blockLevelBtn.png"_spr);
        auto blockButton = CCMenuItemSpriteExtra::create(blockButtonSprite, this,
            menu_selector(Loquibot::showBlockMenu));

        blockButton->setPosition({ buttonXPos, winSize.height / 2 + 90 });

        auto topLabel = CCLabelBMFont::create("Top", "bigFont.fnt");
        topLabel->setPosition({ topButtonSprite->getScaledContentSize().width/2, 13 });
        topLabel->setScale(0.4f);
        topLabel->setZOrder(10);
        topButton->addChild(topLabel);

        auto requesterLabel = CCLabelBMFont::create("-", "chatFont.fnt");

        requesterLabel->setString(("Requested by " + requester).c_str());
        requesterLabel->setOpacity(175);
        requesterLabel->setZOrder(10);
        requesterLabel->setScale(0.5f);
        requesterLabel->setID("requester-label"_spr);

        auto requesterMenu = CCMenu::create();

        float heightOffset = 60.f;

        if (Loader::get()->isModLoaded("n.level-pronouns")) {
            heightOffset += 5.f;
        }

        CCMenuItemSpriteExtra* requesterButton = CCMenuItemSpriteExtra::create(requesterLabel, this, menu_selector(Loquibot::copyRequesterName));
        requesterButton->setPosition({ winSize.width / 2, winSize.height - heightOffset });
        requesterButton->setID("requester-button"_spr);

        CCSprite* iSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        iSprite->setScale(0.4f);

        CCMenuItemSpriteExtra* chatMessageBtn = CCMenuItemSpriteExtra::create(iSprite, this, menu_selector(LoquiLevelInfoLayer::showChatMessage));
        chatMessageBtn->setPosition({ requesterButton->getPositionX() + requesterButton->getScaledContentWidth()/2 + 10, winSize.height - heightOffset });
        chatMessageBtn->setID("chat-message-btn"_spr);

        requesterMenu->addChild(requesterButton);
        requesterMenu->addChild(chatMessageBtn);
        requesterMenu->ignoreAnchorPointForPosition(false);
        requesterMenu->setID("requester-menu"_spr);

        this->addChild(requesterMenu);

        auto menu = CCMenu::create();
        menu->setID("main-button-menu"_spr);
        menu->setScale(0.6f);
        menu->setContentSize({45, 110});

        float bgScale = 0.5f;

        auto menuBG = CCScale9Sprite::create("square02b_001.png");
        menuBG->setColor({0, 0, 0});
        menuBG->setOpacity(127);
        menuBG->setContentSize(CCSize{45, 90} / bgScale);
        menuBG->setPosition({winSize.width - 74, winSize.height/2 + 30});
        menuBG->setScale(bgScale * 0.75f);
        this->addChild(menuBG);

        auto columnLayout = ColumnLayout::create();
        columnLayout->setAxisReverse(true);
        columnLayout->setGap(10);

        menu->setLayout(columnLayout);

        menu->addChild(nextButton);
        menu->addChild(randomButton);
        menu->addChild(topButton);
        menu->addChild(blockButton);
        menu->setPosition({winSize.width - 74, winSize.height/2 + 30});
        menu->updateLayout();

        this->addChild(menu);

        static_cast<PageMenu*>(menu)->setPaged(2, PageOrientation::VERTICAL, 110);

        CCMenu* rightSideMenu = typeinfo_cast<CCMenu*>(this->getChildByID("right-side-menu"));

        rightSideMenu->updateLayout();

        CCMenu* backMenu = typeinfo_cast<CCMenu*>(this->getChildByID("back-menu"));
        auto levelListSprite = CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png");
        levelListSprite->setScale(0.60f);

        CCMenuItemSpriteExtra* levelListButton = CCMenuItemSpriteExtra::create(levelListSprite, this, menu_selector(Loquibot::openLevelMenu));
        levelListButton->setID("level-list-button"_spr);

        backMenu->addChild(levelListButton);
        backMenu->addChild(undoButton);

        backMenu->updateLayout();


        CCNode* title = this->getChildByID("title-label");

        float titleWidth = title->getContentSize().width;

        if (titleWidth >= 300) {
            setRelativeScale(title, 0.9);
        }

        if (level->m_levelID == GlobalVars::getSharedInstance()->idWithYouTube) {
            Loquibot::getSharedInstance()->showYouTube(this);
        }
        fixLevelInfoSize();
        schedule(schedule_selector(LoquiLevelInfoLayer::checkLengthPos));
    
        return true;
    }

    void onPinToggle(CCObject* object) {
        CCMenuItemToggler* toggler = static_cast<CCMenuItemToggler*>(object);
        Mod::get()->setSavedValue("auto-pin", !toggler->isOn());
    }

    void showChatMessage(CCObject* object) {
        geode::createQuickPopup("Message", m_fields->m_message, "Close", nullptr, nullptr, true);
    }

    void updateLabelValues(){
        LevelInfoLayer::updateLabelValues();
        if(m_fields->m_isLevelRequest) fixLevelInfoSize();
    }

    void onBack(CCObject* object) {

        if (GlobalVars::getSharedInstance()->isLoquiMenu) {
            if (GlobalVars::getSharedInstance()->deleting) {

                Loquibot::getSharedInstance()->goToNextLevel(this);
                GlobalVars::getSharedInstance()->deleting = false;
            }
            else {
                if (GlobalVars::getSharedInstance()->isSearchScene) {
                    auto scene = LevelSearchLayer::scene(0);
                    auto transition = CCTransitionFade::create(0.5f, scene);
                    CCDirector::sharedDirector()->replaceScene(transition);
                }
                else {
                    auto scene = MenuLayer::scene(false);
                    auto transition = CCTransitionFade::create(0.5f, scene);
                    CCDirector::sharedDirector()->replaceScene(transition);
                }
                
            }
			GlobalVars::getSharedInstance()->currentID = -1;

        }
        else {
            LevelInfoLayer::onBack(object);
        }
        GlobalVars::getSharedInstance()->isLoquiMenu = false;
    }


    void FLAlert_Clicked(FLAlertLayer* p0, bool p1){

        if(p0->getTag() == 4 && p1){ //onDelete
            GlobalVars::getSharedInstance()->deleting = true;
        }
        LevelInfoLayer::FLAlert_Clicked(p0, p1);
    }


    void fixLevelInfoSize() {
        CCSprite* lengthIcon = typeinfo_cast<CCSprite*>(this->getChildByID("length-icon"));
        CCSprite* downloadsIcon = typeinfo_cast<CCSprite*>(this->getChildByID("downloads-icon"));
        CCSprite* orbsIcon = typeinfo_cast<CCSprite*>(this->getChildByID("orbs-icon"));
        CCSprite* likesIcon = typeinfo_cast<CCSprite*>(this->getChildByID("likes-icon"));
        
        CCLabelBMFont* lengthLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("length-label"));
        CCLabelBMFont* downloadsLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("downloads-label"));
        CCLabelBMFont* orbsLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("orbs-label"));
        CCLabelBMFont* likesLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("likes-label"));
        CCLabelBMFont* exactLengthLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("exact-length-label"));

        float scale = 0.8f;

        setRelativeScale(lengthIcon, scale);
        setRelativeScale(downloadsIcon, scale);
        setRelativeScale(orbsIcon, scale);
        setRelativeScale(likesIcon, scale);
        setRelativeScale(lengthLabel, scale-0.1);
        setRelativeScale(downloadsLabel, scale);
        setRelativeScale(orbsLabel, scale);
        setRelativeScale(likesLabel, scale);
        setRelativeScale(exactLengthLabel, scale);

        setRelativePosition(downloadsIcon, -5, -5);
        setRelativePosition(likesIcon, -5, 0);
        setRelativePosition(lengthIcon, -5, 5);
        setRelativePosition(orbsIcon, -5, 10);

        setRelativePosition(downloadsLabel, -10, -5);
        setRelativePosition(likesLabel, -10, 0);
        setRelativePosition(lengthLabel, -10, 5);
        setRelativePosition(orbsLabel, -10, 10);
        setRelativePosition(exactLengthLabel, -10, 7);

    }
    
    void setRelativeScale(CCNode* node, float scale){

        if(!node) return;

        CCFloat* pScale = typeinfo_cast<CCFloat*>(node->getUserObject("original-scale"));

        if(!pScale){
            pScale = CCFloat::create(node->getScale());
            node->setUserObject("original-scale", pScale);
        }

        node->setScale(pScale->getValue() * scale);
    }

    void checkLengthPos(float dt){
        if(CCNode* label = getChildByID("length-label")){
            CCNode* icon = getChildByID("length-icon");
            if(label->getPosition().y == icon->getPosition().y){
                
                if(CCNode* exactLength = getChildByID("exact-length-label")){
                    if(exactLength->isVisible()){
                        label->setPosition({label->getPosition().x, label->getPosition().y + 5});
                        label->setUserObject("original-y", CCFloat::create(label->getPosition().y));
                        exactLength->setPosition({exactLength->getPosition().x, exactLength->getPosition().y + 5});
                        exactLength->setUserObject("original-y", CCFloat::create(exactLength->getPosition().y));
                    }
                }
            }
        }
    }

    void setRelativePosition(CCNode* node, float relPosX, float relPosY){

        if(!node) return;

        if(node->getID() == "length-label"){

            CCNode* icon = node->getParent()->getChildByID("length-icon");
            if(node->getPosition().y == icon->getPosition().y){
                
                if(CCNode* exactLength = node->getParent()->getChildByID("exact-length-label")){
                    if(exactLength->isVisible()){
                        node->setPosition({node->getPosition().x, node->getPosition().y + 5});
                        node->setUserObject("original-y", CCFloat::create(node->getPosition().y));
                        exactLength->setPosition({exactLength->getPosition().x, exactLength->getPosition().y + 5});
                        exactLength->setUserObject("original-y", CCFloat::create(exactLength->getPosition().y));
                    }
                }
            }
        }

        CCFloat* pX = typeinfo_cast<CCFloat*>(node->getUserObject("original-x"));
        CCFloat* pY = typeinfo_cast<CCFloat*>(node->getUserObject("original-y"));
        if(!pX && !pY){
            pX = CCFloat::create(node->getPosition().x);
            pY = CCFloat::create(node->getPosition().y);

            node->setUserObject("original-x", pX);
            node->setUserObject("original-y", pY);
        }

        node->setPosition({pX->getValue() + relPosX, pY->getValue() + relPosY});
    }
};

matjson::Value getFromArray(int id){

    std::vector<matjson::Value> levels = GlobalVars::getSharedInstance()->currentLevelList;

    for(int i = 0; i < levels.size(); i++){
        matjson::Value level = levels[i];
        if(level["id"] == id) return level;
    }

    return nullptr;
}
