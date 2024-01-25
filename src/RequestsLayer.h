#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class RequestsLayer : public cocos2d::CCLayer {
protected:
    virtual bool init(CCArray* levels, bool levelsEnabled);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void runToggle(CCObject* obj);
    void runClear(CCObject* obj);
    CCMenuItemToggler* m_toggleQueue;
public:
    void updateToggle(bool enabled);
    static RequestsLayer* create(CCArray* levels, bool levelsEnabled);
    static cocos2d::CCScene* scene(CCArray* levels, bool levelsEnabled);
};