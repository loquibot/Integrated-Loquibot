#include <Geode/Geode.hpp>
#include "ServerListener.h"
#include "Loquibot.h"
#include "GlobalVars.h"
#include <alphalaneous.pages_api/include/PageMenu.h>

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

            auto requestsButtonSprite = SearchButton::create("GJ_longBtn04_001.png", "Requests", 0.5f, "GJ_sFollowedIcon_001.png");
            
            auto loquiSprite = CCSprite::create("loqui_icon.png"_spr);
            loquiSprite->setScale(0.5f);

            auto oldSprite = typeinfo_cast<CCSprite*>(requestsButtonSprite->getChildren()->objectAtIndex(1));
            oldSprite->setVisible(false);

            loquiSprite->setPosition({oldSprite->getPosition().x - 2, oldSprite->getPosition().y});

            requestsButtonSprite->addChild(loquiSprite);

            auto requestsButton = CCMenuItemSpriteExtra::create(requestsButtonSprite, this, menu_selector(Loquibot::goToLevel));

            if(quickSearchMenu){
                quickSearchMenu->addChild(requestsButton);
            }

            RowLayout* layout = RowLayout::create();
            layout->setGrowCrossAxis(true);
            layout->setCrossAxisOverflow(false);
            layout->setAxisAlignment(AxisAlignment::Center);
            layout->setCrossAxisAlignment(AxisAlignment::Center);
            layout->ignoreInvisibleChildren(true);
            layout->setGap(5.5f);

            quickSearchMenu->setContentSize({365, 116});
            quickSearchMenu->ignoreAnchorPointForPosition(false);
            quickSearchMenu->setLayout(layout);

            CCSize winSize = CCDirector::get()->getWinSize();
            quickSearchMenu->setPosition({quickSearchMenu->getPosition().x, winSize.height/2 + 28});

            static_cast<PageMenu*>(quickSearchMenu)->setPaged(9, PageOrientation::HORIZONTAL, 422);
        }

        return true;
    }
};

#endif