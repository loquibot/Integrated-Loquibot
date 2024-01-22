#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "ServerListener.h"
#include "Loquibot.h"
#include "GlobalVars.h"
#include "GJGameLevel.h"

matjson::Value getFromArray(int id);

class $modify(LevelInfoLayer) {

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

                auto topButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
                topButtonSprite->setRotation(90);

                float buttonXPos = winSize.width - 80;

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

                auto undoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
                auto undoButton = CCMenuItemSpriteExtra::create(undoButtonSprite, this,
                    menu_selector(Loquibot::goToUndoLevel));
                undoButton->setTag(1347);
                undoButton->setPosition({ 80, winSize.height / 2 + 30 });

                auto blockButtonSprite = CCSprite::create("LB_blockLevelBtn.png"_spr);
                auto blockButton = CCMenuItemSpriteExtra::create(blockButtonSprite, this,
                    menu_selector(Loquibot::blockLevel));

                blockButton->setPosition({ 0, 96 });

                auto blockUserButtonSprite = CCSprite::create("LB_blockUserBtn.png"_spr);
                auto blockUserButton = CCMenuItemSpriteExtra::create(blockUserButtonSprite, this,
                    menu_selector(Loquibot::blockRequester));

                blockUserButton->setPosition({ 0, 48 });

                auto blockCreatorButtonSprite = CCSprite::create("LB_blockCreatorBtn.png"_spr);
                auto blockCreatorButton = CCMenuItemSpriteExtra::create(blockCreatorButtonSprite, this,
                    menu_selector(Loquibot::blockCreator));

                blockCreatorButton->setPosition({ 0, 0 });


                auto randomLabel = CCLabelBMFont::create("Random", "bigFont.fnt");
                randomLabel->setPosition({ 25, 8 });
                randomLabel->setScale(0.2f);
                randomLabel->setZOrder(10);
                //randomButton->addChild(randomLabel);

                auto nextLabel = CCLabelBMFont::create("Next", "bigFont.fnt");
                nextLabel->setPosition({ 25, 8 });
                nextLabel->setScale(0.2f);
                nextLabel->setZOrder(10);
                //nextButton->addChild(nextLabel);

                auto backLabel = CCLabelBMFont::create("Back", "bigFont.fnt");
                backLabel->setPosition({ 10, 8 });
                backLabel->setScale(0.2f);
                backLabel->setZOrder(10);
                //undoButton->addChild(backLabel);

                auto topLabel = CCLabelBMFont::create("Top", "bigFont.fnt");
                topLabel->setPosition({ 17, 8 });
                topLabel->setScale(0.4f);
                topLabel->setZOrder(10);
                topButton->addChild(topLabel);

                auto requesterLabel = CCLabelBMFont::create("-", "bigFont.fnt");

                requesterLabel->setColor(ccGREEN);
                requesterLabel->setString(("Sent By " + requester).c_str());

                requesterLabel->setPosition({ winSize.width / 2, winSize.height - 60 });
                requesterLabel->setZOrder(10);
                requesterLabel->setScale(0.3f);
                requesterLabel->setTag(357832);
                this->addChild(requesterLabel);


                auto menu = CCMenu::create();
                menu->setTag(4323);

                auto actionButtonMenu = CCMenu::create();

                menu->addChild(nextButton);
                menu->addChild(randomButton);
                menu->addChild(undoButton);

                auto topButtonMenu = CCMenu::create();

                topButtonMenu->setPosition({buttonXPos, winSize.height / 2 + 80 });
				topButtonMenu->setContentSize({0, 0});
                topButtonMenu->setScale(0.5f);
                topButtonMenu->setTag(4324);

                topButtonMenu->addChild(topButton);


                actionButtonMenu->addChild(blockButton);
                actionButtonMenu->addChild(blockUserButton);
                actionButtonMenu->addChild(blockCreatorButton);

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
                childMenu->setScale(0.6f);
                childMenu->addChild(button);

                backMenu->addChild(childMenu);

                this->addChild(menu);
                this->addChild(actionButtonMenu);

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

};

matjson::Value getFromArray(int id){

    matjson::Array levels = GlobalVars::getSharedInstance()->currentLevelList;

    for(int i = 0; i < levels.size(); i++){
        matjson::Value level = levels[i];
        if(level["id"] == id) return level;
    }

    return nullptr;
}
