
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>

#include "ServerListener.h"
#include "Loquibot.h"

using namespace geode::prelude;
void goToProfile();

$execute{
    ServerListener::connectAsync();
}

class $modify(MenuLayer) {

	bool init() {
        if (!MenuLayer::init()) return false;

        _isSearchScene = false;
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

class $modify(LevelSearchLayer) {

    bool init() {
        if (!LevelSearchLayer::init()) return false;
        _isSearchScene = true;

        auto buttonSprite = CCSprite::create("loqui.png"_spr);

        auto button = CCMenuItemSpriteExtra::create(buttonSprite, this,
            menu_selector(Loquibot::goToLevel));

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        button->setPosition({ winSize.width - 24, winSize.height - 130 });

        auto menu = CCMenu::create();
        menu->addChild(button);

        menu->setPosition({ 0,0 });

        this->addChild(menu);

        return true;
    }

};


class $modify(LevelInfoLayer) {

    bool init(GJGameLevel * level) {
        if (!LevelInfoLayer::init(level)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (level->m_levelID == _currentID) {
            try {
                _isLoquiMenu = true;
                level->m_creatorName = _creator;
                level->m_accountID = _accountID;

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
                topButton->setPosition({ buttonXPos + 206, winSize.height / 2 + 160 });

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

                if (!_isViewer) {
                    requesterLabel->setColor(ccRED);
                    requesterLabel->setString(("Sent By " + _requester + " (Offline)").c_str());
                }
                else {
                    requesterLabel->setColor(ccGREEN);
                    requesterLabel->setString(("Sent By " + _requester).c_str());
                }

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

                topButtonMenu->setPosition({ 0,0 });
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


                this->addChild(menu);
                this->addChild(actionButtonMenu);

            }
            catch (...) {}

        }
        else {
            _isLoquiMenu = false;
        }
        return true;
    }


    void keyBackClicked() {
        if (_isLoquiMenu) {

            auto scene = CCScene::create();

            if (_isSearchScene) {
                auto layerSearch = LevelSearchLayer::create();
                scene->addChild(layerSearch);
            }
            else {
                auto layerMenu = MenuLayer::scene(false);
                scene->addChild(layerMenu);
            }

            auto transition = CCTransitionFade::create(0.5f, scene);

            CCDirector::sharedDirector()->replaceScene(transition);
        }
        else {
            LevelInfoLayer::keyBackClicked();
        }
    }

    void onViewProfile(CCObject* object) {
        if (_isLoquiMenu) {
            goToProfile();
        }
        else {
            LevelInfoLayer::onViewProfile(object);
        }
    }

    void onBack(CCObject* object) {
        if (_isLoquiMenu) {
            if (_deleting) {
                (new Loquibot())->goToNextLevel(this);
                _deleting = false;
            }
            else {
                auto scene = CCScene::create();

                if (_isSearchScene) {
                    auto layerSearch = LevelSearchLayer::create();
                    scene->addChild(layerSearch);
                }
                else {
                    auto layerMenu = MenuLayer::scene(false);
                    scene->addChild(layerMenu);
                }
                auto transition = CCTransitionFade::create(0.5f, scene);
                CCDirector::sharedDirector()->replaceScene(transition);
            }
        }
        else {
            LevelInfoLayer::onBack(object);
        }
    }

    void onDelete(CCObject* object) {
        _deleting = true;
        LevelInfoLayer::onDelete(object);
    }

};

void goToProfile() {
    ProfilePage* page = ProfilePage::create(_accountID, false);
    page->show();
}

