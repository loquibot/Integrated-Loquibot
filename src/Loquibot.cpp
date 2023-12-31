#include "includes.h"
#include "ServerListener.h"
#include "Loquibot.h"
#include "BlockLevelAlertProtocol.h"
#include "BlockCreatorAlertProtocol.h"
#include "BlockRequesterAlertProtocol.h"
#include <signal.h>
using namespace cocos2d;

std::string notice = "Please open <cg>loquibot</c> to continue. If it is open, try again";
std::string title = "loquibot Not Opened!";
std::string confirm = "Okay";

void hideButtons(CCObject* obj) {

    CCMenuItemSpriteExtra* button = reinterpret_cast<CCMenuItemSpriteExtra*>(obj);
    
    auto menu = button->getParent();

    if (menu->getTag() == 4324) {
        menu = menu->getParent();
    }

    if (menu->getTag() == 4323) {

        auto nextButton = menu->getChildByTag(1345);
        auto randomButton = menu->getChildByTag(1346);
        auto undoButton = menu->getChildByTag(1347);
        auto topMenu = menu->getChildByTag(4324);

        nextButton->setVisible(false);
        randomButton->setVisible(false);
        undoButton->setVisible(false);
        topMenu->setVisible(false);

        auto winSize = CCDirector::sharedDirector()->getWinSize();


        CCSprite* loadingSprite = CCSprite::create("loadingCircle.png");


        loadingSprite->setPosition({ winSize.width - 80, winSize.height / 2 + 70 });

        loadingSprite->setScale(0.5);
        loadingSprite->setBlendFunc({ GL_ONE, GL_ONE_MINUS_CONSTANT_ALPHA });

        loadingSprite->runAction(CCRepeatForever::create(
            CCRotateBy::create(1.0f, 360)
            )
        );

        menu->getParent()->addChild(loadingSprite);
    }
}

void Loquibot::goToLevel(CCObject* obj) {

    if (!_loquiOpen) ServerListener::connectAsync();
    
    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!_loquiOpen) {
        alertLayer->show();
    }
    else {

        ServerListener::sendMessage("get");
        _isButtonPressed = true;

        hideButtons(obj);
    }
}

void Loquibot::goToNextLevel(CCObject* obj) {

    if (!_loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!_loquiOpen) {
        alertLayer->show();
    }
    else {

        ServerListener::sendMessage("next_get");
        _isButtonPressed = true;

        hideButtons(obj);
    }
}

void Loquibot::goToTopLevel(CCObject* obj) {

    if (!_loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!_loquiOpen) {
        alertLayer->show();
    }
    else {

        ServerListener::sendMessage("top_get");
        _isButtonPressed = true;

        hideButtons(obj);
    }
}


void Loquibot::goToRandomLevel(CCObject* obj) {
   
    if (!_loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!_loquiOpen) {
        alertLayer->show();
    }
    else {
        ServerListener::sendMessage("random_get");
        _isButtonPressed = true;

        hideButtons(obj);
    }
}

void Loquibot::goToUndoLevel(CCObject* obj) {

    if (!_loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(nullptr, title.c_str(), notice, confirm.c_str(), nullptr, 300);

    if (!_loquiOpen) {
        alertLayer->show();
    }
    else {
        ServerListener::sendMessage("undo_get");
        hideButtons(obj);

        _isButtonPressed = true;
    }
}


void Loquibot::blockLevel(CCObject*) {

    if (!_loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(new BlockLevelAlertProtocol, "Block Level?", ("Block <cr>" + std::to_string(_currentID) + "</c>?"), "Cancel", "Okay", 300);
    
    alertLayer->show();
}

void Loquibot::blockCreator(CCObject*) {

    if (!_loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(new BlockCreatorAlertProtocol, "Block Creator?", ("Block <cr>" + _creator + "</c>?"), "Cancel", "Okay", 300);

    alertLayer->show();
}

void Loquibot::blockRequester(CCObject*) {

    if (!_loquiOpen) ServerListener::connectAsync();

    auto alertLayer = FLAlertLayer::create(new BlockRequesterAlertProtocol, "Block Requester?", ("Block <cr>" + _requester + "</c>?"), "Cancel", "Okay", 300);

    alertLayer->show();
}

void Loquibot::goToMainScene(CCObject*) {
    auto layer = MenuLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    
    auto transition = CCTransitionFade::create(0.5f, scene);
   
    CCDirector::sharedDirector()->pushScene(transition);
}


