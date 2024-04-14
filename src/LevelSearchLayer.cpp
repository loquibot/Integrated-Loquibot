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

        menu->updateLayout();


        return true;
    }

};

#endif