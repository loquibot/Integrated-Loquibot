#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class RequestsLayer : public cocos2d::CCLayer {
protected:
    virtual bool init(CCArray* levels);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:
    static RequestsLayer* create(CCArray* levels);
    static cocos2d::CCScene* scene(CCArray* levels);

};