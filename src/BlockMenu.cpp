#include "BlockMenu.h"
#include "Loquibot.h"

void BlockMenu::setup() {
    
    ButtonSprite* blockIDSprite = ButtonSprite::create("Block Level");
    ButtonSprite* blockCreatorSprite = ButtonSprite::create("Block Creator");
    ButtonSprite* blockRequesterSprite = ButtonSprite::create("Block Requester");

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
    if (pRet && pRet->init(350, 200, "GJ_square01.png", "Block?")) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};
