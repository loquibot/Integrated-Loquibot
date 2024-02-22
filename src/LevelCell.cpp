#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include "GJGameLevel.h"
#include "Loquibot.h"
#include "GlobalVars.h"
#include "RequestsLayer.h"
#include "ServerListener.h"

using namespace geode::prelude;

class $modify(LevelCell){

    void loadCustomLevelCell() {

        LevelCell::loadCustomLevelCell();

        bool isRequest = ((LoquiGJGameLevel*)m_level)->m_fields->m_isRequest;

        bool onReqScene = GlobalVars::getSharedInstance()->onReqScene;

        if(isRequest && onReqScene){

            if(!Loader::get()->isModLoaded("cvolton.betterinfo")){
                CCLabelBMFont* idLabel = CCLabelBMFont::create(fmt::format("{}", m_level->m_levelID.value()).c_str(), "chatFont.fnt");
                idLabel->setPosition({346, m_height - 2});
                idLabel->setAnchorPoint({1,1});
                idLabel->setScale(0.45f);
                idLabel->setOpacity(152);
                m_mainLayer->addChild(idLabel);
            }

            CCLabelBMFont* requester = CCLabelBMFont::create((((LoquiGJGameLevel*)m_level)->m_fields->m_requester).c_str(), "chatFont.fnt");
            requester->setPosition({346, 2});
            requester->setAnchorPoint({1,0});
            requester->setScale(0.45f);
            requester->setOpacity(152);
            m_mainLayer->addChild(requester);

            CCMenuItemSpriteExtra* viewButton = m_button;
            viewButton->removeAllChildren();
            viewButton->setContentSize({30, 30});
            viewButton->setPosition({viewButton->getPositionX()+15, viewButton->getPositionY()+1});

            CCSprite* playSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
            playSprite->setAnchorPoint({0, 0});
            playSprite->setScale(0.35f);

            viewButton->addChild(playSprite);
        }
    }

    void onClick(CCObject* obj){

        bool isRequest = ((LoquiGJGameLevel*)m_level)->m_fields->m_isRequest;

        if(isRequest && isLevelRequestsScene()){
            GlobalVars::getSharedInstance()->currentID = m_level->m_levelID;
            ServerListener::sendMessage(fmt::format("set_pos {}", getLevelPos(m_level->m_levelID)));
        }
        LevelCell::onClick(obj);
    }

    bool isLevelRequestsScene(){
        cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
        cocos2d::CCScene* scene = director->getRunningScene();

        RequestsLayer* layer = dynamic_cast<RequestsLayer*>(scene->getChildren()->objectAtIndex(0));

        return layer != nullptr;
    }

    int getLevelPos(int id){

        matjson::Array levels = GlobalVars::getSharedInstance()->currentLevelList;

        for(int i = 0; i < levels.size(); i++){
            matjson::Value level = levels[i];
            if(level["id"] == id) return i;
        }

        return 0;
    }
};