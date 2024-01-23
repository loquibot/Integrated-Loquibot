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

    bool init(GJGameLevel * level, bool a2) {

        if (!LevelInfoLayer::init(level, a2)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (level->m_levelID == GlobalVars::getSharedInstance()->currentID) {
			GlobalVars::getSharedInstance()->lastLayer = this;
            try {
                GlobalVars::getSharedInstance()->isLoquiMenu = true;

                matjson::Value listLevel = getFromArray(level->m_levelID);

                std::string creator;
                std::string requester;
                int accountID;

                if(listLevel != nullptr){
                    creator = listLevel["creator"].as_string();
                    accountID = listLevel["accountID"].as_int();
                    requester = listLevel["requester"].as_string();
                }
                else{
                    creator = GlobalVars::getSharedInstance()->creator;
                    accountID = GlobalVars::getSharedInstance()->accountID;
                    requester = GlobalVars::getSharedInstance()->requester;
                }
                level->m_creatorName = creator;
                level->m_accountID = accountID;

                auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
                nextButtonSprite->setFlipX(true);

                auto topButtonSprite = CCSprite::createWithSpriteFrameName("GJ_orderUpBtn_001.png");

                float buttonXPos = winSize.width - 100;

                auto nextButton = CCMenuItemSpriteExtra::create(nextButtonSprite, this,
                    menu_selector(Loquibot::goToNextLevel));

                nextButton->setTag(1345);
                nextButton->setPosition({ buttonXPos, winSize.height / 2 + 30 });

                auto topButton = CCMenuItemSpriteExtra::create(topButtonSprite, this,
                    menu_selector(Loquibot::goToTopLevel));

                topButton->setTag(1348);
                topButton->setPosition({ 0, 0});

                auto randomButtonSprite = CCSprite::create("LB_randomBtn.png"_spr);
                randomButtonSprite->setFlipX(true);
                randomButtonSprite->setScale(0.9);
                auto randomButton = CCMenuItemSpriteExtra::create(randomButtonSprite, this,
                    menu_selector(Loquibot::goToRandomLevel));
                randomButton->setTag(1346);
                randomButton->setPosition({ buttonXPos, winSize.height / 2 - 20 });

                auto undoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png");
                undoButtonSprite->setScale(0.9f);
                auto undoButton = CCMenuItemSpriteExtra::create(undoButtonSprite, this,
                    menu_selector(Loquibot::goToUndoLevel));
                undoButton->setTag(1347);
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
                requesterLabel->setPosition({ winSize.width / 2, winSize.height - 60 });
                requesterLabel->setZOrder(10);
                requesterLabel->setScale(0.5f);
                requesterLabel->setTag(357832);
                this->addChild(requesterLabel);


                auto menu = CCMenu::create();
                menu->setTag(4323);
                menu->setScale(0.8f);
                menu->setAnchorPoint({1, 0.5});
                auto actionButtonMenu = CCMenu::create();

                menu->addChild(blockButton);
                menu->addChild(nextButton);
                menu->addChild(randomButton);

                auto topButtonMenu = CCMenu::create();

                topButtonMenu->setPosition({buttonXPos - 54, winSize.height / 2 + 168 }); //469, 240
				topButtonMenu->setContentSize({0, 0});
                topButtonMenu->setScale(0.5f);
                topButtonMenu->setTag(4324);

                topButtonMenu->addChild(topButton);

                //actionButtonMenu->addChild(blockButton);
                //actionButtonMenu->addChild(blockUserButton);
                //actionButtonMenu->addChild(blockCreatorButton);

                actionButtonMenu->setContentSize({ 0,0 });

                actionButtonMenu->setPosition({ winSize.width / 2 - 55, winSize.height / 2 + 25 });
                actionButtonMenu->setScale(0.5f);

                menu->setPosition({ 0,0 });
                menu->addChild(topButtonMenu);

				CCMenu* otherMenu = dynamic_cast<CCMenu*>(this->getChildByID("other-menu"));

				CCMenuItemSpriteExtra* listButton = dynamic_cast<CCMenuItemSpriteExtra*>(otherMenu->getChildByID("list-button"));
				CCMenuItemSpriteExtra* favoritesButton = dynamic_cast<CCMenuItemSpriteExtra*>(otherMenu->getChildByID("favorite-button"));

				listButton->setPosition(favoritesButton->getPositionX(), favoritesButton->getPositionY()+40);

                CCMenu* backMenu = dynamic_cast<CCMenu*>(this->getChildByID("back-menu"));
                auto levelListSprite = CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png");

                CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(levelListSprite, this, menu_selector(Loquibot::openLevelMenu));

                CCMenu* childMenu = CCMenu::create();
                childMenu->setContentSize({30,30});
                childMenu->setPosition({56, 14});
                undoButton->setPosition({105, 20});

                childMenu->setScale(0.6f);
                childMenu->addChild(button);

                backMenu->addChild(childMenu);
                backMenu->addChild(undoButton);

                this->addChild(menu);
                this->addChild(actionButtonMenu);

                CCNode* title = this->getChildByID("title-label");

                float titleWidth = title->getContentSize().width;

                if(titleWidth >= 300){
                    setRelativeScale(title, 0.9);
                }

                if(level->m_levelID == GlobalVars::getSharedInstance()->idWithYouTube){
                    Loquibot::getSharedInstance()->showYouTube();
                }

                fixLevelInfoSize();
            }
            catch (...) {}

        }
        else {
            GlobalVars::getSharedInstance()->isLoquiMenu = false;
        }
        return true;
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
                    auto layerSearch = LevelSearchLayer::create(0);
                    scene->addChild(layerSearch);
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
            GlobalVars::getSharedInstance()->lastLayer = nullptr;

        }
        else {
            LevelInfoLayer::onBack(object);
        }
    }

    void onDelete(CCObject* object) {
        GlobalVars::getSharedInstance()->deleting = true;
        LevelInfoLayer::onDelete(object);
    }

    void fixLevelInfoSize(){
        CCSprite* lengthIcon = dynamic_cast<CCSprite*>(this->getChildByID("length-icon"));
        CCSprite* downloadsIcon = dynamic_cast<CCSprite*>(this->getChildByID("downloads-icon"));
        CCSprite* orbsIcon = dynamic_cast<CCSprite*>(this->getChildByID("orbs-icon"));
        CCSprite* likesIcon = dynamic_cast<CCSprite*>(this->getChildByID("likes-icon"));

        CCLabelBMFont* lengthLabel = dynamic_cast<CCLabelBMFont*>(this->getChildByID("length-label"));
        CCLabelBMFont* downloadsLabel = dynamic_cast<CCLabelBMFont*>(this->getChildByID("downloads-label"));
        CCLabelBMFont* orbsLabel = dynamic_cast<CCLabelBMFont*>(this->getChildByID("orbs-label"));
        CCLabelBMFont* likesLabel = dynamic_cast<CCLabelBMFont*>(this->getChildByID("likes-label"));

        float scale = 0.8f;

        setRelativeScale(lengthIcon, scale);
        setRelativeScale(downloadsIcon, scale);
        setRelativeScale(orbsIcon, scale);
        setRelativeScale(likesIcon, scale);
        setRelativeScale(lengthLabel, scale);
        setRelativeScale(downloadsLabel, scale);
        setRelativeScale(orbsLabel, scale);
        setRelativeScale(likesLabel, scale);

        setRelativePosition(downloadsIcon, -5, -5);
        setRelativePosition(likesIcon, -5, 0);
        setRelativePosition(lengthIcon, -5, 5);
        setRelativePosition(orbsIcon, -5, 10);

        setRelativePosition(downloadsLabel, -10, -5);
        setRelativePosition(likesLabel, -10, 0);
        setRelativePosition(lengthLabel, -10, 5);
        setRelativePosition(orbsLabel, -10, 10);

    }
    
    void setRelativeScale(CCNode* node, float scale){
        node->setScale(node->getScale()*scale);
    }

    void setRelativePosition(CCNode* node, float relPosX, float relPosY){
        node->setPosition({node->getPositionX() + relPosX, node->getPositionY() + relPosY});
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