#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "ServerListener.h"
#include "Loquibot.h"
#include "GlobalVars.h"

class $modify(MenuLayer) {

	bool init() {
        if (!MenuLayer::init()) return false;

        GlobalVars::getSharedInstance()->isSearchScene = false;

        auto buttonSprite = CCSprite::create("loqui.png"_spr);

        auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
            menu_selector(Loquibot::goToLevel));

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        button->setPosition({ winSize.width - 40, winSize.height / 2 - 30 });

        auto menu = this->getChildByID("right-side-menu");
        menu->addChild(button);
        menu->updateLayout();

        return true;
	}

};
