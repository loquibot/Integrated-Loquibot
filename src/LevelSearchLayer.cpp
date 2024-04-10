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

    bool init(int a1) {

        if (!LevelSearchLayer::init(a1)) return false;

        GlobalVars::getSharedInstance()->isSearchScene = true;

        auto buttonSprite = CCSprite::create("loqui.png"_spr);

        auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
            menu_selector(Loquibot::goToLevel));

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto menu = this->getChildByID("other-filter-menu");
        menu->addChild(button);

        auto lastButton = menu->getChildByID("lists-button");

        if(Loader::get()->isModLoaded("b1rtek.gddlintegration")){
            lastButton = menu->getChildByID("b1rtek.gddlintegration/gddl_search_button");
        }

        button->setPosition({lastButton->getPositionX() + 1, lastButton->getPositionY() - 50});


        return true;
    }

};

#endif