#pragma once

#ifndef __LOLEVELINFOLAYER_HPP
#define __LOLEVELINFOLAYER_HPP

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "ServerListener.h"
#include "Loquibot.h"
#include "GlobalVars.h"
#include "GJGameLevel.h"

matjson::Value getFromArray(int id);

class $modify(LoquiLevelInfoLayer, LevelInfoLayer) {

    struct Fields {
        bool m_isLevelRequest = false;
    };

    bool init(GJGameLevel * level, bool a2) {

        if (!LevelInfoLayer::init(level, a2)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (level->m_levelID == GlobalVars::getSharedInstance()->currentID) {

            m_fields->m_isLevelRequest = true;
            GlobalVars::getSharedInstance()->isLoquiMenu = true;

            matjson::Value listLevel = getFromArray(level->m_levelID);

            std::string requester;

            if(listLevel != nullptr){
                requester = listLevel["requester"].as_string();
            }
            else {
                requester = GlobalVars::getSharedInstance()->requester;
            }
            

            auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            nextButtonSprite->setFlipX(true);

            auto topButtonSprite = CCSprite::createWithSpriteFrameName("GJ_orderUpBtn_001.png");

            float buttonXPos = winSize.width - 100;

            auto nextButton = CCMenuItemSpriteExtra::create(nextButtonSprite, this,
                menu_selector(Loquibot::goToNextLevel));

            nextButton->setID("next-button"_spr);
            nextButton->setPosition({ buttonXPos, winSize.height / 2 + 30 });

            auto topButton = CCMenuItemSpriteExtra::create(topButtonSprite, this,
                menu_selector(Loquibot::goToTopLevel));

            topButton->setID("top-button"_spr);
            topButton->setPosition({ 0, 0});

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
            blockButtonSprite->setScale(0.75f);
            auto blockButton = CCMenuItemSpriteExtra::create(blockButtonSprite, this,
                menu_selector(Loquibot::showBlockMenu));

            blockButton->setPosition({ buttonXPos, winSize.height / 2 + 90 });

            auto topLabel = CCLabelBMFont::create("Top", "bigFont.fnt");
            topLabel->setPosition({ 25, 13 });
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

            CCMenuItemSpriteExtra* requesterButton = CCMenuItemSpriteExtra::create(requesterLabel, requesterMenu, menu_selector(Loquibot::copyRequesterName));
            requesterButton->setPosition({ winSize.width / 2, winSize.height - 60 });
            requesterButton->setID("requester-button"_spr);

            requesterMenu->addChild(requesterButton);
            requesterMenu->ignoreAnchorPointForPosition(false);
            requesterMenu->setID("requester-menu"_spr);

            this->addChild(requesterMenu);

            auto menu = CCMenu::create();
            menu->setID("main-button-menu"_spr);
            menu->setScale(0.8f);
            menu->setAnchorPoint({1, 0.5});
            auto actionButtonMenu = CCMenu::create();

            menu->addChild(blockButton);
            menu->addChild(nextButton);
            menu->addChild(randomButton);

            auto topButtonMenu = CCMenu::create();

            topButtonMenu->setPosition({buttonXPos - 24, winSize.height - 20}); //469, 240
            topButtonMenu->setContentSize({0, 0});
            topButtonMenu->setScale(0.5f);
            topButtonMenu->setID("top-button-menu"_spr);

            topButtonMenu->addChild(topButton);

            actionButtonMenu->setContentSize({ 0,0 });

            actionButtonMenu->setPosition({ winSize.width / 2 - 55, winSize.height / 2 + 25 });
            actionButtonMenu->setScale(0.5f);

            menu->setPosition({ 0,0 });
            this->addChild(topButtonMenu);

            CCMenu* otherMenu = typeinfo_cast<CCMenu*>(this->getChildByID("other-menu"));

            CCMenuItemSpriteExtra* listButton = typeinfo_cast<CCMenuItemSpriteExtra*>(otherMenu->getChildByID("list-button"));
            CCMenuItemSpriteExtra* favoritesButton = typeinfo_cast<CCMenuItemSpriteExtra*>(otherMenu->getChildByID("favorite-button"));

            listButton->setPosition(favoritesButton->getPositionX(), favoritesButton->getPositionY()+40);

            CCMenu* backMenu = typeinfo_cast<CCMenu*>(this->getChildByID("back-menu"));
            auto levelListSprite = CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png");
            levelListSprite->setScale(0.60f);

            CCMenuItemSpriteExtra* levelListButton = CCMenuItemSpriteExtra::create(levelListSprite, this, menu_selector(Loquibot::openLevelMenu));
            levelListButton->setID("level-list-button"_spr);

            backMenu->addChild(levelListButton);
            backMenu->addChild(undoButton);

            backMenu->updateLayout();

            this->addChild(menu);
            this->addChild(actionButtonMenu);

            CCNode* title = this->getChildByID("title-label");

            float titleWidth = title->getContentSize().width;

            if(titleWidth >= 300){
                setRelativeScale(title, 0.9);
            }

            if(level->m_levelID == GlobalVars::getSharedInstance()->idWithYouTube){
                Loquibot::getSharedInstance()->showYouTube(this);
            }
            fixLevelInfoSize();
            schedule(schedule_selector(LoquiLevelInfoLayer::checkLengthPos));
        }
        else {
            GlobalVars::getSharedInstance()->isLoquiMenu = false;
        }
        return true;
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
                auto scene = CCScene::create();

                if (GlobalVars::getSharedInstance()->isSearchScene) {
#ifdef GEODE_IS_WINDOWS
                    scene = LevelSearchLayer::scene(0);
                    auto transition = CCTransitionFade::create(0.5f, scene);
                    CCDirector::sharedDirector()->replaceScene(transition);
#endif
                }
                else {
                    auto layerMenu = MenuLayer::scene(false);
                    scene->addChild(layerMenu);
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

    matjson::Array levels = GlobalVars::getSharedInstance()->currentLevelList;

    for(int i = 0; i < levels.size(); i++){
        matjson::Value level = levels[i];
        if(level["id"] == id) return level;
    }

    return nullptr;
}

#endif