#pragma once

#ifndef __LOQUIBOT_H
#define __LOQUIBOT_H

class Loquibot {
protected:
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
private:
};

#endif