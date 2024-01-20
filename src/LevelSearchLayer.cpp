#include <Geode/Geode.hpp>
#include "ServerListener.h"
#include "Loquibot.h"
#include "GlobalVars.h"

#ifdef GEODE_IS_WINDOWS

#include <Geode/modify/LevelSearchLayer.hpp>

class $modify(LevelSearchLayer) {

    bool init(int a1) {

        if (!LevelSearchLayer::init(a1)) return false;

        GlobalVars::getSharedInstance()->isSearchScene = true;

        auto buttonSprite = CCSprite::create("loqui.png"_spr);

        auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
            menu_selector(Loquibot::goToLevel));

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        button->setPosition({ winSize.width - 24, winSize.height - 170 });

        auto menu = CCMenu::create();
        menu->addChild(button);

        menu->setPosition({ 0,0 });

        this->addChild(menu);



        return true;
    }

};

#endif