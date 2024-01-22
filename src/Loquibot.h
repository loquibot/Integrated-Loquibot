#pragma once

#ifndef __LOQUIBOT_H
#define __LOQUIBOT_H

#include <Geode/Geode.hpp>

using namespace cocos2d;

class Loquibot {
protected:
    static Loquibot* instance;
public:
    void showButtons(CCObject*);
    void goToLevel(CCObject*);
    void goToNextLevel(CCObject*);
    void goToTopLevel(CCObject*);
    void goToRandomLevel(CCObject*);
    void goToUndoLevel(CCObject*);
    void goToMainScene(CCObject*);
    void blockLevel(CCObject*);
    void blockCreator(CCObject*);
    void blockRequester(CCObject*);
    void openLevelMenu(CCObject*);
    void operator=(const Loquibot &) = delete;

    static Loquibot* getSharedInstance(){

        if (!instance) {
            instance = new Loquibot();
        };
        return instance;
    }
private:
};


#endif