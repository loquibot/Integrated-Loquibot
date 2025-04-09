#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include "GlobalVars.h"

class $modify(LoquiFLAlertLayer, FLAlertLayer) {

    
    bool init(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5, bool p6, float p7, float p8) {
        if (!FLAlertLayer::init(p0, p1, p2, p3, p4, p5, p6, p7, p8)) return false;

        if (GlobalVars::getSharedInstance()->autoPinCheck && p2 == "Are you sure you want to <cy>pin</c> this level to the <cg>current room</c>?") {
            p0->FLAlert_Clicked(this, true);
            keyBackClicked();
        }

        return true;
    }

    void show() {
        if (GlobalVars::getSharedInstance()->autoPinCheck) return;
        FLAlertLayer::show();
    }
};
