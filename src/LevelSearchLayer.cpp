#include <Geode/Geode.hpp>
#include "ServerListener.h"
#include "Loquibot.h"
#include "GlobalVars.h"

#ifdef GEODE_IS_WINDOWS

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(LevelSearchLayer) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("LevelSearchLayer::init", -1024);
    }

    bool init(int p0) {

        if (!LevelSearchLayer::init(p0)) return false;

        if(!p0){

            GlobalVars::getSharedInstance()->isSearchScene = true;

            auto quickSearchMenu = this->getChildByIDRecursive("quick-search-menu");

            auto randomButtonSprite = SearchButton::create("GJ_longBtn04_001.png", "Requests", 0.5f, "GJ_sFollowedIcon_001.png");
            
            auto loquiSprite = CCSprite::create("loqui_icon.png"_spr);
            loquiSprite->setScale(0.5f);

            auto oldSprite = dynamic_cast<CCSprite*>(randomButtonSprite->getChildren()->objectAtIndex(1));
            oldSprite->setVisible(false);

            loquiSprite->setPosition({oldSprite->getPosition().x - 2, oldSprite->getPosition().y});

            randomButtonSprite->addChild(loquiSprite);

            auto randomTabButton = CCMenuItemSpriteExtra::create(randomButtonSprite, this, menu_selector(Loquibot::goToLevel));

            if(quickSearchMenu){
                quickSearchMenu->addChild(randomTabButton);
            }

        }

        return true;
    }

};

#endif