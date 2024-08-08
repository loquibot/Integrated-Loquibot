#include "BlockMenu.h"
#include "Loquibot.h"
#include <Geode/Geode.hpp>

void BlockMenu::setup() {
    
    ButtonSprite* blockIDSprite = ButtonSprite::create("Level", 100, true, "bigFont.fnt", "geode.loader/GE_button_05.png", 30, 1);
    ButtonSprite* blockCreatorSprite = ButtonSprite::create("Creator", 100, true, "bigFont.fnt", "geode.loader/GE_button_05.png", 30, 1);
    ButtonSprite* blockRequesterSprite = ButtonSprite::create("Requester", 100, true, "bigFont.fnt", "geode.loader/GE_button_05.png", 30, 1);

    CCMenuItemSpriteExtra* blockIDButton = CCMenuItemSpriteExtra::create(blockIDSprite, this, menu_selector(Loquibot::blockLevel));
    CCMenuItemSpriteExtra* blockCreatorButton = CCMenuItemSpriteExtra::create(blockCreatorSprite, this, menu_selector(Loquibot::blockCreator));
    CCMenuItemSpriteExtra* blockRequesterButton = CCMenuItemSpriteExtra::create(blockRequesterSprite, this, menu_selector(Loquibot::blockRequester));

    blockIDButton->setPosition({0, 20});
    blockCreatorButton->setPosition({0, -20});
    blockRequesterButton->setPosition({0, -60});

    CCMenu* buttonMenu = CCMenu::create();

    buttonMenu->addChild(blockIDButton);
    buttonMenu->addChild(blockCreatorButton);
    buttonMenu->addChild(blockRequesterButton);

    m_mainLayer->addChild(buttonMenu);
    setTouchEnabled(true);
}

BlockMenu* BlockMenu::create() {
    auto pRet = new BlockMenu();
    if (pRet && pRet->init(200, 200, "geode.loader/GE_square03.png", "Block?")) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};
